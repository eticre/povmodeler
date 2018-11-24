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


#include "pmrotateedit.h"
#include "pmrotate.h"
#include "pmvectoredit.h"

#include <qlayout.h>



PMRotateEdit::PMRotateEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMRotateEdit::createTopWidgets()
{
   Base::createTopWidgets();

   m_pVector = new PMVectorEdit( "x", "y", "z", this );
   topLayout()->addWidget( m_pVector );

   connect( m_pVector, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMRotateEdit::displayObject( PMObject* o )
{
   if( o->isA( "Rotate" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMRotate* ) o;

      m_pVector->setVector( m_pDisplayedObject->rotation() );
      m_pVector->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMRotateEdit: Can't display object\n";
}

void PMRotateEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setRotation( m_pVector->vector() );
   }
}

bool PMRotateEdit::isDataValid()
{
   if( m_pVector->isDataValid() )
      return Base::isDataValid();
   return false;
}
#include "pmrotateedit.moc"
