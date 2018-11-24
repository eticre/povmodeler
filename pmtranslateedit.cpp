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


#include "pmtranslateedit.h"
#include "pmtranslate.h"
#include "pmvectoredit.h"

#include <qlayout.h>



PMTranslateEdit::PMTranslateEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMTranslateEdit::createTopWidgets()
{
   Base::createTopWidgets();

   m_pVector = new PMVectorEdit( "x", "y", "z", this );
   topLayout()->addWidget( m_pVector );

   connect( m_pVector, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMTranslateEdit::displayObject( PMObject* o )
{
   if( o->isA( "Translate" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMTranslate* ) o;

      m_pVector->setVector( m_pDisplayedObject->translation() );
      m_pVector->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMTranslateEdit: Can't display object\n";
}

void PMTranslateEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setTranslation( m_pVector->vector() );
   }
}

bool PMTranslateEdit::isDataValid()
{
   if( m_pVector->isDataValid() )
      return Base::isDataValid();
   return false;
}
#include "pmtranslateedit.moc"
