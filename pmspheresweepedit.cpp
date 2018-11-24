/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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


#include "pmspheresweepedit.h"
#include "pmspheresweep.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmvectorlistedit.h"
#include "pmpart.h"

#include <qlayout.h>
#include <qlabel.h>

#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include <QList>
#include <QHBoxLayout>
#include <QMessageBox>

PMSphereSweepEdit::PMSphereSweepEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMSphereSweepEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Spline type:" ), this ) );
   m_pSplineType = new QComboBox( this );
   m_pSplineType->addItem( tr( "Linear Spline" ) );
   m_pSplineType->addItem( tr( "B-Spline" ) );
   m_pSplineType->addItem( tr( "Cubic Spline" ) );
   hl->addWidget( m_pSplineType );
   hl->addStretch( 1 );

   connect( m_pSplineType, SIGNAL( activated( int ) ),
            SLOT( slotTypeChanged( int ) ) );
}

void PMSphereSweepEdit::createBottomWidgets()
{
   QBoxLayout* tl = topLayout();
   tl->addWidget( new QLabel( tr( "Spheres:" ), this ) );

   m_pPoints = new PMVectorListEdit( "x", "y", "z", "r", this );
   connect( m_pPoints, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pPoints->selectionModel(),
            SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ),
            SLOT( slotSelectionChanged() ) );

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( m_pPoints, 2 );

   m_pAddAbove = new QPushButton( this );
   m_pAddAbove->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmaddpointabove.png" ) );
   m_pAddBelow = new QPushButton( this );
   m_pAddBelow->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmaddpoint.png" ) );
   m_pRemove = new QPushButton( this );
   m_pRemove->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmremovepoint.png" ) );
   connect( m_pAddAbove, SIGNAL( clicked() ), SLOT( slotAddPointAbove() ) );
   connect( m_pAddBelow, SIGNAL( clicked() ), SLOT( slotAddPointBelow() ) );
   connect( m_pRemove, SIGNAL( clicked() ), SLOT( slotRemovePoint() ) );

   QVBoxLayout* bl = new QVBoxLayout();
   hl->addLayout( bl );
   bl->addWidget( m_pAddAbove );
   bl->addWidget( m_pAddBelow );
   bl->addWidget( m_pRemove );
   bl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Tolerance" ), this ) );
   m_pTolerance = new PMFloatEdit( this );
   m_pTolerance->setValidation( true, 0, false, 0 );
   hl->addWidget( m_pTolerance );
   connect( m_pTolerance, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   Base::createBottomWidgets();
}

void PMSphereSweepEdit::displayObject( PMObject* o )
{
   if( o->isA( "SphereSweep" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = static_cast<PMSphereSweep*>(o);

      switch( m_pDisplayedObject->splineType() )
      {
         case PMSphereSweep::LinearSpline:
            m_pSplineType->setCurrentIndex( 0 );
            break;
         case PMSphereSweep::BSpline:
            m_pSplineType->setCurrentIndex( 1 );
            break;
         case PMSphereSweep::CubicSpline:
            m_pSplineType->setCurrentIndex( 2 );
            break;
      }
      m_pSplineType->setEnabled( !readOnly );
      m_pTolerance->setValue( m_pDisplayedObject->tolerance() );
      m_pTolerance->setReadOnly( readOnly );
      //m_pPoints->setReadOnly( readOnly );

      QList<PMVector> points = m_pDisplayedObject->points();
      QList<double> radii = m_pDisplayedObject->radii();
      QList<PMVector>::iterator pit = points.begin();
      QList<double>::iterator rit = radii.begin();
      for( ; pit != points.end() && rit != radii.end(); ++pit, ++rit )
      {
         ( *pit ).resize( 4 );
         ( *pit )[3] = *rit;
      }

      m_pPoints->setVectors( points, true );
      updateControlPointSelection();
      updatePointButtons();

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMSphereSweepEdit: Can't display object\n";
}

void PMSphereSweepEdit::updateControlPointSelection()
{
   /*
   PMControlPointList cp = part()->activeControlPoints();
   PMControlPointListIterator it( cp );
   int i;
   int np = cp.count() / 4;

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
   */
}

void PMSphereSweepEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      QList<PMVector> points = m_pPoints->vectors();
      QList<double> radii;
      QList<PMVector>::iterator pit = points.begin();
      for( ; pit != points.end(); ++pit )
      {
         radii.append( ( *pit )[3] );
         ( *pit ).resize( 3 );
      }

      m_pDisplayedObject->setPoints( points );
      m_pDisplayedObject->setRadii( radii );

      switch( m_pSplineType->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setSplineType( PMSphereSweep::LinearSpline );
            break;
         case 1:
            m_pDisplayedObject->setSplineType( PMSphereSweep::BSpline );
            break;
         case 2:
            m_pDisplayedObject->setSplineType( PMSphereSweep::CubicSpline );
            break;
      }
      m_pDisplayedObject->setTolerance( m_pTolerance->value() );
      Base::saveContents();
   }
}

bool PMSphereSweepEdit::isDataValid()
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
         if( np < 4 )
         {
            QMessageBox::warning( this, tr( "B-splines need at least 4 points." ),
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

void PMSphereSweepEdit::slotTypeChanged( int )
{
   emit dataChanged();
}

void PMSphereSweepEdit::slotAddPointAbove()
{
   int index = m_pPoints->currentIndex().row();
    QList<PMVector> points = m_pPoints->vectors();
	
   if( index >= 0 && index < points.count() )
   {
		PMVector v;
		if( index == 0 )
			v = points[index];
		else
			v = ( points[index] + points[index-1] ) / 2;
		
		points.insert( index, v );
		m_pPoints->setVectors( points, true );
        updatePointButtons();
        emit dataChanged();
        emit sizeChanged();
   }
}

void PMSphereSweepEdit::slotAddPointBelow()
{
   int index = m_pPoints->currentIndex().row();
    QList<PMVector> points = m_pPoints->vectors();
   if( index >= 0 && index < points.count() )
   {
		PMVector v;
        if( index == ( points.count() - 1 ) )
			v = points[index];
		else
			v = (points[index] + points[index+1]) / 2;
		
		points.insert( index+1, v );
		m_pPoints->setVectors( points, true );
        updatePointButtons();
        emit dataChanged();
   }
}

void PMSphereSweepEdit::slotRemovePoint()
{
   int row = m_pPoints->currentIndex().row();
    QList<PMVector> points = m_pPoints->vectors();

   if( row >= 0 && row < points.count() )
   {
      QList<PMVector>::iterator it = points.begin() + row;

      if( it != points.end() && points.size() > 1 )
      {
         points.erase( it );
         m_pPoints->setVectors( points, true );
         updatePointButtons();
         emit dataChanged();
         emit sizeChanged();
      }
   }
}

void PMSphereSweepEdit::slotSelectionChanged()
{
   /*
   PMControlPointList cp = part()->activeControlPoints();
   PMControlPointListIterator it( cp );
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
   */
   updatePointButtons();
}

void PMSphereSweepEdit::updatePointButtons()
{
   int row = m_pPoints->currentIndex().row();
   m_pAddAbove->setEnabled( row >= 0 );
   m_pAddBelow->setEnabled( row >= 0 );
   m_pRemove->setEnabled( row >= 0 && m_pPoints->size() > 2 );
}

#include "pmspheresweepedit.moc"
