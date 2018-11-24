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


#include "pmobjectlinkedit.h"
#include "pmobjectlink.h"
#include "pmdeclare.h"
#include "pmobjectselect.h"
#include "pmlinkedit.h"

#include <qlayout.h>

PMObjectLinkEdit::PMObjectLinkEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMObjectLinkEdit::createTopWidgets()
{
   Base::createTopWidgets();

   m_pLinkEdit = new PMLinkEdit( this );
   QStringList l;
   l.append( "GraphicalObject" );
   l.append( "Light" );
   m_pLinkEdit->setLinkPossibilities( l );
   topLayout()->addWidget( m_pLinkEdit );
   connect( m_pLinkEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMObjectLinkEdit::displayObject( PMObject* o )
{
   if( o->isA( "ObjectLink" ) )
   {
      m_pDisplayedObject = ( PMObjectLink* ) o;
      m_pLinkEdit->setDisplayedObject( m_pDisplayedObject );
      m_pLinkEdit->setReadOnly( m_pDisplayedObject->isReadOnly() );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMObjectLinkEdit: Can't display object\n";
}

void PMObjectLinkEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setLinkedObject( m_pLinkEdit->link() );
   }
}

bool PMObjectLinkEdit::isDataValid()
{
   return Base::isDataValid();
}

#include "pmobjectlinkedit.moc"
