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


#include "pmrawedit.h"
#include "pmraw.h"

#include <QLayout>
#include <QLabel>
#include <QTextEdit>
//#include <kglobalsettings.h>


PMRawEdit::PMRawEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMRawEdit::createTopWidgets()
{
   Base::createTopWidgets();

   topLayout()->addWidget( new QLabel( tr( "Povray code:" ), this ) );
   m_pEdit = new QTextEdit( this );
   m_pEdit->setWordWrapMode( QTextOption::NoWrap );
   //m_pEdit->setCurrentFont( KGlobalSettings::fixedFont() );
   topLayout()->addWidget( m_pEdit, 2 );
   
   connect( m_pEdit, SIGNAL( textChanged() ), SIGNAL( dataChanged() ) );
}

void PMRawEdit::displayObject( PMObject* o )
{
   if( o->isA( "Raw" ) )
   {
      m_pDisplayedObject = ( PMRaw* ) o;
      m_pEdit->setPlainText( m_pDisplayedObject->code() );

      m_pEdit->setReadOnly( o->isReadOnly() );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMRawEdit: Can't display object\n";
}

void PMRawEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setCode( m_pEdit->toPlainText() );
   }
}

bool PMRawEdit::isDataValid()
{
   return Base::isDataValid();
}

#include "pmrawedit.moc"
