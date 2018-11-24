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


#include "pmconeedit.h"
#include "pmcone.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>

//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>

#include <qcheckbox.h>

PMConeEdit::PMConeEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMConeEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   QHBoxLayout* layout;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();

   m_pEnd1 = new PMVectorEdit( "x", "y", "z", this );
   m_pEnd2 = new PMVectorEdit( "x", "y", "z", this );
   m_pRadius1 = new PMFloatEdit( this );
   m_pRadius2 = new PMFloatEdit( this );   
   m_pOpen = new QCheckBox( ( "type of the object Open" ), this );

   gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "End 1:" ), this ), 0, 0 );
   gl->addWidget( m_pEnd1, 0, 1 );
   gl->addWidget( new QLabel( tr( "End 2:" ), this ), 1, 0 );
   gl->addWidget( m_pEnd2, 1, 1 );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   gl = new QGridLayout();
   layout->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Radius 1:" ), this ), 0, 0 );
   gl->addWidget( m_pRadius1, 0, 1 );
   gl->addWidget( new QLabel( tr( "Radius 2:" ), this ), 1, 0 );
   gl->addWidget( m_pRadius2, 1, 1 );
   layout->addStretch( 1 );

   tl->addWidget( m_pOpen );

   connect( m_pEnd1, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnd2, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadius1, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadius2, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOpen, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMConeEdit::displayObject( PMObject* o )
{
   if( o->isA( "Cone" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMCone* ) o;

      m_pEnd1->setVector( m_pDisplayedObject->end1() );
      m_pEnd2->setVector( m_pDisplayedObject->end2() );
      m_pRadius1->setValue( m_pDisplayedObject->radius1() );
      m_pRadius2->setValue( m_pDisplayedObject->radius2() );
      m_pOpen->setChecked( m_pDisplayedObject->open() );

      m_pEnd1->setReadOnly( readOnly );
      m_pEnd2->setReadOnly( readOnly );
      m_pRadius1->setReadOnly( readOnly );
      m_pRadius2->setReadOnly( readOnly );
      m_pOpen->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
   qCritical(  ) << "PMConeEdit: Can't display object\n";
}

void PMConeEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setEnd1( m_pEnd1->vector() );
      m_pDisplayedObject->setEnd2( m_pEnd2->vector() );
      m_pDisplayedObject->setRadius1( m_pRadius1->value() );
      m_pDisplayedObject->setRadius2( m_pRadius2->value() );
      m_pDisplayedObject->setOpen( m_pOpen->isChecked() );
   }
}
	   
bool PMConeEdit::isDataValid()
{
   if( m_pEnd1->isDataValid() )
      if( m_pEnd2->isDataValid() )
         if( m_pRadius1->isDataValid() )
            if( m_pRadius2->isDataValid() )
               return Base::isDataValid();
   return false;
}

#include  "pmconeedit.moc"
