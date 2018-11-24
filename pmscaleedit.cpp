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


#include "pmscaleedit.h"
#include "pmscale.h"
#include "pmvectoredit.h"

#include <qlayout.h>



PMScaleEdit::PMScaleEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMScaleEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   m_pVector = new PMVectorEdit( "x", "y", "z", this );
   topLayout()->addWidget( m_pVector );

   connect( m_pVector, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );   
}

void PMScaleEdit::displayObject( PMObject* o )
{
   if( o->isA( "Scale" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMScale* ) o;

      m_pVector->setVector( m_pDisplayedObject->scale() );
      m_pVector->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMScaleEdit: Can't display object\n";
}

void PMScaleEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setScale( m_pVector->vector() );
   }
}

bool PMScaleEdit::isDataValid()
{
   if( m_pVector->isDataValid() )
      return Base::isDataValid();
   return false;
}
#include "pmscaleedit.moc"
