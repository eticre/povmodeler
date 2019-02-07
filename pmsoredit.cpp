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


#include "pmsoredit.h"
#include "pmsor.h"
#include "pmpart.h"

#include "qcombobox.h"
#include "qcheckbox.h"
#include "qmessagebox.h"


PMSurfaceOfRevolutionEdit::PMSurfaceOfRevolutionEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
   m_pPoints = new PMVectorListEdit( "u", "v", this );
}

PMSurfaceOfRevolutionEdit::~PMSurfaceOfRevolutionEdit()
{
    m_pPoints->close();
    delete m_pPoints;
    m_pPoints = nullptr;
}

void PMSurfaceOfRevolutionEdit::createBottomWidgets()
{
   QBoxLayout* tl = topLayout();
   tl->addWidget( new QLabel( tr( "Spline points:" ), this ) );
   

   //connect( m_pPoints, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pPoints, SIGNAL( selectionChanged() ), SLOT( slotSelectionChanged() ) );
   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( m_pPoints, 2 );

   m_pAddAbove = new QPushButton( this );
   m_pAddAbove->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmaddpointabove" )  );
   m_pAddBelow = new QPushButton( this );
   m_pAddBelow->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmaddpoint" )  );
   m_pRemove = new QPushButton( this );
   m_pRemove->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmremovepoint" )  );
   connect( m_pAddAbove, SIGNAL( clicked() ), SLOT( slotAddPointAbove() ) );
   connect( m_pAddBelow, SIGNAL( clicked() ), SLOT( slotAddPointBelow() ) );
   connect( m_pRemove, SIGNAL( clicked() ), SLOT( slotRemovePoint() ) );

   QVBoxLayout* bl = new QVBoxLayout();
   tl->addLayout( bl );
   bl->addWidget( m_pAddAbove );
   bl->addWidget( m_pAddBelow );
   bl->addWidget( m_pRemove );
   bl->addStretch( 1 );
   
   m_pOpen = new QCheckBox( ( "type of the object Open" ), this );
   topLayout()->addWidget( m_pOpen );
   connect( m_pOpen, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   m_pSturm = new QCheckBox( tr( "Sturm" ), this );
   topLayout()->addWidget( m_pSturm );
   connect( m_pSturm, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );

   Base::createBottomWidgets();
}

void PMSurfaceOfRevolutionEdit::displayObject( PMObject* o )
{
   if( o->isA( "SurfaceOfRevolution" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMSurfaceOfRevolution* ) o;

      m_pOpen->setChecked( m_pDisplayedObject->open() );
      m_pOpen->setEnabled( !readOnly );
      m_pSturm->setChecked( m_pDisplayedObject->sturm() );
      m_pSturm->setEnabled( !readOnly );
      m_pPoints->setVectors( m_pDisplayedObject->points(), true );
      updateControlPointSelection();
      updatePointButtons();

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMSurfaceOfRevolutionEdit: Can't display object\n";
}

void PMSurfaceOfRevolutionEdit::updateControlPointSelection()
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

      for( i = 0; i < np; i++, ++it )
         if( ( *it )->selected() )
         {
            m_pPoints->select( i );
         }
      m_pPoints->clearSelection();
      for( i = 0; i < np; i++, ++it )
         if( ( *it )->selected() )
         {
            m_pPoints->select( i );
         }

      m_pPoints->blockSignals( sb );
      m_pPoints->blockSelectionUpdates( false );
   }
}

void PMSurfaceOfRevolutionEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setPoints( m_pPoints->vectors() );
      m_pDisplayedObject->setOpen( m_pOpen->isChecked() );
      m_pDisplayedObject->setSturm( m_pSturm->isChecked() );
      Base::saveContents();
   }
}

bool PMSurfaceOfRevolutionEdit::isDataValid()
{
   if( !m_pPoints->isDataValid() )
      return false;

   int np = m_pPoints->size();
   if( np < 4 )
   {
      QMessageBox::warning( this, tr( "The surface of revolution object needs at least 4 points." ),
                          tr( "Error" ) );
      return false;
   }
   
   QList<PMVector> points = m_pPoints->vectors();
   QList<PMVector>::iterator it1 = points.begin();
   QList<PMVector>::iterator it2 = it1; ++it2;
   QList<PMVector>::iterator it3 = it2; ++it3;
   int pnr;

   for( pnr = 0; it3 != points.end(); ++it1, ++it2, ++it3, pnr++ )
   {
      if( ( pnr == 0 ) || ( pnr == ( np - 3 ) ) )
      {
         if( approxZero( ( *it1 )[1] - ( *it3 )[1], c_sorTolerance ) )
         {
            //m_pPoints->setCurrentCell( pnr, 1 );
            QMessageBox::warning( this, ( "The v coordinate of point " +
                                        QString::number( pnr + 1 ) + " and " +
                                        QString::number( pnr + 3 ) + " must be different." ),
                                        ( "Error" ) );
            return false;
         }
      }

      if( pnr != 0 )
      {
         if( ( ( *it2 )[1] - ( *it1 )[1] ) < c_sorTolerance )
         {
            //m_pPoints->setCurrentCell( pnr + 1, 1 );
            QMessageBox::warning( this, tr( "The v coordinates must be strictly increasing." ),
                                tr( "Error" ) );
            return false;
         }
      }
   }
   
   return Base::isDataValid();
}

void PMSurfaceOfRevolutionEdit::slotAddPointAbove()
{
   int index = m_pPoints->currentIndex().row();
	QList<PMVector> points = m_pPoints->vectors();
   if( index >= 0 && index < points.count() )
   {
		PMVector v;
		if( index == 0 )
			v = points[index];
		else
			v = (points[index] + points[index-1]) / 2;
		
		points.insert( index, v );
		m_pPoints->setVectors( points, true );
		updatePointButtons();
		emit dataChanged();
   }
}

void PMSurfaceOfRevolutionEdit::slotAddPointBelow()
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

void PMSurfaceOfRevolutionEdit::slotRemovePoint()
{
   int row = m_pPoints->currentIndex().row();
	QList<PMVector> points = m_pPoints->vectors();
		
   if( row >= 0 && row < points.count() )
   {
		points.removeAt( row );
		m_pPoints->setVectors( points, true );
		updatePointButtons();
		emit dataChanged();
   }
}

void PMSurfaceOfRevolutionEdit::slotSelectionChanged()
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

void PMSurfaceOfRevolutionEdit::updatePointButtons()
{
   int row = m_pPoints->currentIndex().row();
   m_pAddAbove->setEnabled( row >= 0 );
   m_pAddBelow->setEnabled( row >= 0 );
   m_pRemove->setEnabled( row >= 0 && m_pPoints->size() > 4 );
}

#include "pmsoredit.moc"
