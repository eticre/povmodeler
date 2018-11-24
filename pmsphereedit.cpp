/*
**************************************************************************

                          pmsphereedit.cpp  -  description
                             -------------------
    begin                : Wed Jun 6 2001
    copyright            : (C) 2001 by Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmsphereedit.h"
#include "pmsphere.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QHBoxLayout>


PMSphereEdit::PMSphereEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMSphereEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();
   QHBoxLayout* layout;

   m_pCentre = new PMVectorEdit( "x", "y", "z", this );
   m_pRadius = new PMFloatEdit( this );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( new QLabel( tr( "Center:" ), this ) );
   layout->addWidget( m_pCentre );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( new QLabel( tr( "Radius:" ), this ) );
   layout->addWidget( m_pRadius );
   layout->addStretch( 1 );

   connect( m_pCentre, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadius, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMSphereEdit::displayObject( PMObject* o )
{
   if( o->isA( "Sphere" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMSphere* ) o;

      m_pCentre->setVector( m_pDisplayedObject->centre() );
      m_pRadius->setValue( m_pDisplayedObject->radius() );

      m_pCentre->setReadOnly( readOnly );
      m_pRadius->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMSphereEdit: Can't display object\n";
}

void PMSphereEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setCentre( m_pCentre->vector() );
      m_pDisplayedObject->setRadius( m_pRadius->value() );
   }
}

bool PMSphereEdit::isDataValid()
{
   if( m_pCentre->isDataValid() )
      if( m_pRadius->isDataValid() )
         return Base::isDataValid();
   return false;
}


#include "pmsphereedit.moc"
