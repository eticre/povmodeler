/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#include "pmboxedit.h"
#include "pmbox.h"
#include "pmvectoredit.h"

#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QGridLayout>


PMBoxEdit::PMBoxEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMBoxEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   m_pCorner1 = new PMVectorEdit( "x", "y", "z", this );
   m_pCorner2 = new PMVectorEdit( "x", "y", "z", this );

   QBoxLayout* tl = topLayout();
   QGridLayout* gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Corner 1:" ), this ), 0, 0 );
   gl->addWidget( m_pCorner1, 0, 1 );
   gl->addWidget( new QLabel( tr( "Corner 2:" ), this ), 1, 0 );
   gl->addWidget( m_pCorner2, 1, 1 );

   connect( m_pCorner1, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCorner2, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMBoxEdit::displayObject( PMObject* o )
{
   if( o->isA( "Box" ) )
   {
      bool readOnly = o->isReadOnly();
      //m_pDisplayedObject = ( PMBox* ) o;
      m_pDisplayedObject = m_pDisplayedObject->objectCopy(o);

      m_pCorner1->setVector( m_pDisplayedObject->corner1() );
      m_pCorner2->setVector( m_pDisplayedObject->corner2() );

      m_pCorner1->setReadOnly( readOnly );
      m_pCorner2->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMBoxEdit: Can't display object\n";
}

void PMBoxEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setCorner1( m_pCorner1->vector() );
      m_pDisplayedObject->setCorner2( m_pCorner2->vector() );
   }
}

bool PMBoxEdit::isDataValid()
{
   if( m_pCorner1->isDataValid() )
      if( m_pCorner2->isDataValid() )
         return Base::isDataValid();
   return false;
}
#include "pmboxedit.moc"
