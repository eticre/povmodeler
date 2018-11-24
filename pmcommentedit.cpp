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


#include "pmcommentedit.h"
#include "pmcomment.h"

#include <QLayout>
#include <QTextEdit>
//#include <kglobalsettings.h>

PMCommentEdit::PMCommentEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMCommentEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   m_pEdit = new QTextEdit( this );
   m_pEdit->setWordWrapMode( QTextOption::NoWrap );
   //m_pEdit->setCurrentFont( KGlobalSettings::fixedFont() );
   topLayout()->addWidget( m_pEdit, 2 );
   
   connect( m_pEdit, SIGNAL( textChanged() ), SIGNAL( dataChanged() ) );
}

void PMCommentEdit::displayObject( PMObject* o )
{
   if( o->isA( "Comment" ) )
   {
      m_pDisplayedObject = ( PMComment* ) o;
      m_pEdit->setPlainText( m_pDisplayedObject->text() );

      m_pEdit->setReadOnly( o->isReadOnly() );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMCommentEdit: Can't display object\n";
}

void PMCommentEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setText( m_pEdit->toPlainText() );
   }
}

bool PMCommentEdit::isDataValid()
{
   return Base::isDataValid();
}
#include "pmcommentedit.moc"
