/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmlatheedit.h"
#include "pmlathe.h"
#include "pmvectoredit.h"
#include "pmvectorlistedit.h"
#include "pmpart.h"

#include <QLayout>
#include <QLabel>

#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QList>
#include <QHBoxLayout>
#include <QMessageBox>


PMLatheEdit::PMLatheEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMLatheEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Spline type:" ), this ) );
   m_pSplineType = new QComboBox( this );
   m_pSplineType->addItem( tr( "Linear Spline" ) );
   m_pSplineType->addItem( tr( "Quadratic Spline" ) );
   m_pSplineType->addItem( tr( "Cubic Spline" ) );
   m_pSplineType->addItem( tr( "Bezier Spline" ) );
   hl->addWidget( m_pSplineType );
   hl->addStretch( 1 );

   connect( m_pSplineType, SIGNAL( activated( int ) ),
            SLOT( slotTypeChanged( int ) ) );
}

void PMLatheEdit::createBottomWidgets()
{
   QBoxLayout* tl = topLayout();
   tl->addWidget( new QLabel( tr( "Spline points:" ), this ) );
   
   m_pPoints = new PMVectorListEdit( "u", "v", this );
   connect( m_pPoints, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pPoints->selectionModel(),
            SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ),
            SLOT( slotSelectionChanged() ) );

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( m_pPoints, 2 );

   m_pAddAbove = new QPushButton( this );
   m_pAddAbove->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmaddpointabove" ) );
   m_pAddBelow = new QPushButton( this );
   m_pAddBelow->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmaddpoint" ) );
   m_pRemove = new QPushButton( this );
   m_pRemove->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmremovepoint" ) );
   connect( m_pAddAbove, SIGNAL( clicked() ), SLOT( slotAddPointAbove() ) );
   connect( m_pAddBelow, SIGNAL( clicked() ), SLOT( slotAddPointBelow() ) );
   connect( m_pRemove, SIGNAL( clicked() ), SLOT( slotRemovePoint() ) );

   QVBoxLayout* bl = new QVBoxLayout();
   hl->addLayout( bl );
   bl->addWidget( m_pAddAbove );
   bl->addWidget( m_pAddBelow );
   bl->addWidget( m_pRemove );
   bl->addStretch( 1 );

   m_pSturm = new QCheckBox( tr( "Sturm" ), this );
   tl->addWidget( m_pSturm );
   connect( m_pSturm, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );

   Base::createBottomWidgets();
}

void PMLatheEdit::displayObject( PMObject* o )
{
   if( o->isA( "Lathe" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMLathe* ) o;

      switch( m_pDisplayedObject->splineType() )
      {
         case PMLathe::LinearSpline:
            m_pSplineType->setCurrentIndex( 0 );
            break;
         case PMLathe::QuadraticSpline:
            m_pSplineType->setCurrentIndex( 1 );
            break;
         case PMLathe::CubicSpline:
            m_pSplineType->setCurrentIndex( 2 );
            break;
         case PMLathe::BezierSpline:
            m_pSplineType->setCurrentIndex( 3 );
            break;
      }
      m_pSplineType->setEnabled( !readOnly );
      m_pSturm->setChecked( m_pDisplayedObject->sturm() );
      m_pSturm->setEnabled( !readOnly );
      if( readOnly )
         m_pPoints->setEditTriggers(QAbstractItemView::NoEditTriggers);
      m_pPoints->setVectors( m_pDisplayedObject->points(), true );
      updateControlPointSelection();
      updatePointButtons();

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMLatheEdit: Can't display object\n";
}

void PMLatheEdit::updateControlPointSelection()
{
   PMControlPointList cp = part()->activeControlPoints();
	PMControlPointList::iterator it = cp.begin();
   int i;
   int np = cp.count() / 2;

   if( np == m_pPoints->size() )
   {
      m_pPoints->blockSelectionUpdates( true );
      bool sb = m_pPoints->signalsBlocked();
      m_pPoints->blockSignals( true );
      
      m_pPoints->clearSelection();
      for( i = 0; i < np; i++, ++it )
         if( ( *it )->selected() )
            m_pPoints->select( i );
      for( i = 0; i < np; i++, ++it )
         if( ( *it )->selected() )
            m_pPoints->select( i );
      
      m_pPoints->blockSignals( sb );
      m_pPoints->blockSelectionUpdates( false );
   }
}

void PMLatheEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setPoints( m_pPoints->vectors() );

      switch( m_pSplineType->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setSplineType( PMLathe::LinearSpline );
            break;
         case 1:
            m_pDisplayedObject->setSplineType( PMLathe::QuadraticSpline );
            break;
         case 2:
            m_pDisplayedObject->setSplineType( PMLathe::CubicSpline );
            break;
         case 3:
            m_pDisplayedObject->setSplineType( PMLathe::BezierSpline );
            break;
      }
      m_pDisplayedObject->setSturm( m_pSturm->isChecked() );
      Base::saveContents();
   }
}

bool PMLatheEdit::isDataValid()
{
   if( !m_pPoints->isDataValid() )
         return false;
   
   int np = m_pPoints->size();
   switch( m_pSplineType->currentIndex() )
   {
      case 0:
         if( np < 2 )
         {
            QMessageBox::warning( this, tr( "Linear splines need at least 2 points." ),
                                tr( "Error" ) );
            return false;
         }
         break;
      case 1:
         if( np < 3 )
         {
            QMessageBox::warning( this, tr( "Quadratic splines need at least 3 points." ),
                                tr( "Error" ) );
            return false;
         }
         break;
      case 2:
         if( np < 4 )
         {
            QMessageBox::warning( this, tr( "Cubic splines need at least 4 points." ),
                                tr( "Error" ) );
            return false;
         }
         break;
      case 3:
         if( ( np < 4 ) || ( ( np % 4 ) != 0 ) )
         {
            QMessageBox::warning( this, tr( "Bezier splines need 4 points for each segment." ),
                                tr( "Error" ) );
            return false;
         }
         break;
   }
   
   return Base::isDataValid();
}

void PMLatheEdit::slotTypeChanged( int )
{
   emit dataChanged();
}

void PMLatheEdit::slotAddPointAbove()
{
   int index = m_pPoints->currentIndex().row();
	QList<PMVector> points = m_pPoints->vectors();
	
   if( index >= 0 && index < points.count() )
   {
		PMVector v;
		if( index == 0 )
			v = points[0];
		else
			v = ( points[index] + points[index-1] ) / 2;
		
		points.insert( index, v );
		m_pPoints->setVectors( points, true );
		updatePointButtons();
		emit dataChanged();
		emit sizeChanged();
   }
}

void PMLatheEdit::slotAddPointBelow()
{
   int index = m_pPoints->currentIndex().row();
	QList<PMVector> points = m_pPoints->vectors();
	
   if( index >= 0 && index < points.count() )
   {
		PMVector v;
		if( index == points.count() - 1 )
			v = points[index];
		else
			v = ( points[index] + points[index+1] ) / 2;
		
		points.insert( index + 1, v );
		m_pPoints->setVectors( points, true );
		updatePointButtons();
		emit dataChanged();
		emit sizeChanged();
   }
}

void PMLatheEdit::slotRemovePoint()
{
   int row = m_pPoints->currentIndex().row();
	QList<PMVector> points = m_pPoints->vectors();

   if( row >= 0 && row < points.count() && points.count() > 1 )
   {
		points.removeAt( row );
		m_pPoints->setVectors( points, true );
		updatePointButtons();
		emit dataChanged();
		emit sizeChanged();
   }
}

void PMLatheEdit::slotSelectionChanged()
{
   PMControlPointList cp = part()->activeControlPoints();
	PMControlPointList::iterator it = cp.begin();
   int np = cp.count() / 2;
   int i;
   
   if( np == m_pPoints->size() )
   {
      for( i = 0; i < np; i++, ++it )
         ( *it )->setSelected( m_pPoints->isSelected( i ) );
      for( i = 0; i < np; i++, ++it )
         ( *it )->setSelected( m_pPoints->isSelected( i ) );
      emit controlPointSelectionChanged();
   }
   updatePointButtons();
}

void PMLatheEdit::updatePointButtons()
{
   int row = m_pPoints->currentIndex().row();
   m_pAddAbove->setEnabled( row >= 0 );
   m_pAddBelow->setEnabled( row >= 0 );
   m_pRemove->setEnabled( row >= 0 && m_pPoints->size() > 2 );
}

#include "pmlatheedit.moc"
