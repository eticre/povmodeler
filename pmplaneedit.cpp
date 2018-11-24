/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmplaneedit.h"
#include "pmplane.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>

#include <QMessageBox>

PMPlaneEdit::PMPlaneEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMPlaneEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   QHBoxLayout* layout;
   QBoxLayout* tl = topLayout();

   m_pNormal = new PMVectorEdit( "x", "y", "z", this );
   m_pDistance = new PMFloatEdit( this );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( new QLabel( tr( "Normal:" ), this ) );
   layout->addWidget( m_pNormal );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( new QLabel( tr( "Distance:" ), this ) );
   layout->addWidget( m_pDistance );
   layout->addStretch( 1 );

   QPushButton* nb = new QPushButton( tr( "Normalize" ), this );
   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( nb );
   layout->addStretch( 1 );

   connect( m_pNormal, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDistance, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( nb, SIGNAL( clicked() ), SLOT( slotNormalize() ) );
}

void PMPlaneEdit::slotNormalize()
{
   PMVector normal = m_pNormal->vector();
   double distance = m_pDistance->value();
   double l = normal.abs();
   if( !approxZero( l ) )
   {
      m_pNormal->setVector( normal / l );
      m_pDistance->setValue( distance * l );
   }
}

void PMPlaneEdit::displayObject( PMObject* o )
{
   if( o->isA( "Plane" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMPlane* ) o;

      m_pNormal->setVector( m_pDisplayedObject->normal() );
      m_pDistance->setValue( m_pDisplayedObject->distance() );

      m_pNormal->setReadOnly( readOnly );
      m_pDistance->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMPlaneEdit: Can't display object\n";
}

void PMPlaneEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setNormal( m_pNormal->vector() );
      m_pDisplayedObject->setDistance( m_pDistance->value() );
   }
}
  
bool PMPlaneEdit::isDataValid()
{
   if( m_pNormal->isDataValid() )
   {
      if( approxZero( m_pNormal->vector().abs() ) )
      {
         QMessageBox::warning( this, tr( "The normal vector may not be a "
                                         "null vector." ),
                             tr( "Error" ) );
         return false;
         
      }
      if( m_pDistance->isDataValid() )
         return Base::isDataValid();
   }
   return false;
}

#include  "pmplaneedit.moc"
