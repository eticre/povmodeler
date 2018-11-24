/*
**************************************************************************
                                description
                             -------------------
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

#include "pmblobsphereedit.h"
#include "pmblobsphere.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>


PMBlobSphereEdit::PMBlobSphereEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMBlobSphereEdit::createTopWidgets()
{
   Base::createTopWidgets();

   QBoxLayout* tl = topLayout();
   QHBoxLayout* layout;

   m_pCentre = new PMVectorEdit( "x", "y", "z", this );
   m_pRadius = new PMFloatEdit( this );
   m_pStrength = new PMFloatEdit( this );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( new QLabel( tr( "Center:" ), this ) );
   layout->addWidget( m_pCentre );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   QGridLayout* gl = new QGridLayout();
   layout->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Radius:" ), this ), 0, 0 );
   gl->addWidget( m_pRadius, 0, 1 );
   gl->addWidget( new QLabel( tr( "Strength:" ), this ), 1, 0 );
   gl->addWidget( m_pStrength, 1, 1 );
   layout->addStretch( 1 );

   connect( m_pCentre, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadius, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pStrength, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMBlobSphereEdit::displayObject( PMObject* o )
{
   if( o->isA( "BlobSphere" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMBlobSphere* ) o;

      m_pCentre->setVector( m_pDisplayedObject->centre() );
      m_pRadius->setValue( m_pDisplayedObject->radius() );
      m_pStrength->setValue( m_pDisplayedObject->strength() );

      m_pCentre->setReadOnly( readOnly );
      m_pRadius->setReadOnly( readOnly );
      m_pStrength->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMBlobSphereEdit: Can't display object\n";
}

void PMBlobSphereEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setCentre( m_pCentre->vector() );
      m_pDisplayedObject->setRadius( m_pRadius->value() );
      m_pDisplayedObject->setStrength( m_pStrength->value() );
   }
}

bool PMBlobSphereEdit::isDataValid()
{
   if( m_pCentre->isDataValid() )
      if( m_pRadius->isDataValid() )
         if( m_pStrength->isDataValid() )
            return Base::isDataValid();
   return false;
}


#include "pmblobsphereedit.moc"
