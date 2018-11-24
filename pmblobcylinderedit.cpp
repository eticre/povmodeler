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


#include "pmblobcylinderedit.h"
#include "pmblobcylinder.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <QHBoxLayout>
#include <QGridLayout>

#include <qcheckbox.h>

PMBlobCylinderEdit::PMBlobCylinderEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMBlobCylinderEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   QHBoxLayout* layout;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();

   m_pEnd1 = new PMVectorEdit( "x", "y", "z", this );
   m_pEnd2 = new PMVectorEdit( "x", "y", "z", this );
   m_pRadius = new PMFloatEdit( this );
   m_pStrength = new PMFloatEdit( this );

   gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "End 1:" ), this ), 0, 0 );
   gl->addWidget( m_pEnd1, 0, 1 );
   gl->addWidget( new QLabel( tr( "End 2:" ), this ), 1, 0 );
   gl->addWidget( m_pEnd2, 1, 1 );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Radius:" ), this ), 0, 0 );
   gl->addWidget( m_pRadius, 0, 1 );
   gl->addWidget( new QLabel( tr( "Strength:" ), this ), 1, 0 );
   gl->addWidget( m_pStrength, 1, 1 );
   layout->addStretch( 1 );

   connect( m_pEnd1, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnd2, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadius, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pStrength, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMBlobCylinderEdit::displayObject( PMObject* o )
{
   if( o->isA( "BlobCylinder" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMBlobCylinder* ) o;

      m_pEnd1->setVector( m_pDisplayedObject->end1() );
      m_pEnd2->setVector( m_pDisplayedObject->end2() );
      m_pRadius->setValue( m_pDisplayedObject->radius() );
      m_pStrength->setValue( m_pDisplayedObject->strength() );

      m_pEnd1->setReadOnly( readOnly );
      m_pEnd2->setReadOnly( readOnly );
      m_pRadius->setReadOnly( readOnly );
      m_pStrength->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
   qCritical(  ) << "PMBlobCylinderEdit: Can't display object\n";
}

void PMBlobCylinderEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setEnd1( m_pEnd1->vector() );
      m_pDisplayedObject->setEnd2( m_pEnd2->vector() );
      m_pDisplayedObject->setRadius( m_pRadius->value() );
      m_pDisplayedObject->setStrength( m_pStrength->value() );
   }
}
	   
bool PMBlobCylinderEdit::isDataValid()
{
   if( m_pEnd1->isDataValid() )
      if( m_pEnd2->isDataValid() )
         if( m_pRadius->isDataValid() )
            if( m_pStrength->isDataValid() )
               return Base::isDataValid();
   return false;
}

#include  "pmblobcylinderedit.moc"


