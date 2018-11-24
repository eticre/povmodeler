/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : skoriane@nce.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmcylinderedit.h"
#include "pmcylinder.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>

//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>

#include <qcheckbox.h>

PMCylinderEdit::PMCylinderEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMCylinderEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   QHBoxLayout* layout;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();

   m_pEnd1 = new PMVectorEdit( "x", "y", "z", this );
   m_pEnd2 = new PMVectorEdit( "x", "y", "z", this );
   m_pRadius = new PMFloatEdit( this );
   m_pOpen = new QCheckBox( ( "type of the object Open" ), this );

   gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "End 1:" ), this ), 0, 0 );
   gl->addWidget( m_pEnd1, 0, 1 );
   gl->addWidget( new QLabel( tr( "End 2:" ), this ), 1, 0 );
   gl->addWidget( m_pEnd2, 1, 1 );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( new QLabel( tr( "Radius:" ), this ) );
   layout->addWidget( m_pRadius );
   layout->addStretch( 1 );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( m_pOpen );
  
   connect( m_pEnd1, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnd2, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadius, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOpen, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMCylinderEdit::displayObject( PMObject* o )
{
   if( o->isA( "Cylinder" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMCylinder* ) o;

      m_pEnd1->setVector( m_pDisplayedObject->end1() );
      m_pEnd2->setVector( m_pDisplayedObject->end2() );
      m_pRadius->setValue( m_pDisplayedObject->radius() );
      m_pOpen->setChecked( m_pDisplayedObject->open() );

      m_pEnd1->setReadOnly( readOnly );
      m_pEnd2->setReadOnly( readOnly );
      m_pRadius->setReadOnly( readOnly );
      m_pOpen->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
   qCritical(  ) << "PMCylinderEdit: Can't display object\n";
}

void PMCylinderEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setEnd1( m_pEnd1->vector() );
      m_pDisplayedObject->setEnd2( m_pEnd2->vector() );
      m_pDisplayedObject->setRadius( m_pRadius->value() );
      m_pDisplayedObject->setOpen( m_pOpen->isChecked() );
   }
}
	   
bool PMCylinderEdit::isDataValid()
{
   if( m_pEnd1->isDataValid() )
      if( m_pEnd2->isDataValid() )
         if( m_pRadius->isDataValid() )
            return Base::isDataValid();
   return false;
}

#include  "pmcylinderedit.moc"


