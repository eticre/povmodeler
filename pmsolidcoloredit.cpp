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


#include "pmsolidcoloredit.h"
#include "pmsolidcolor.h"
#include "pmcoloredit.h"

#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QHBoxLayout>



PMSolidColorEdit::PMSolidColorEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMSolidColorEdit::createTopWidgets()
{
   Base::createTopWidgets();

   QBoxLayout* tl = topLayout();

   QHBoxLayout* layout = new QHBoxLayout();
   tl->addLayout( layout );
   m_pColorEdit = new PMColorEdit( true, this );
   QLabel* label = new QLabel( tr( "Color:" ), this );

   layout->addWidget( label, 0, Qt::AlignTop );
   layout->addWidget( m_pColorEdit );

   connect( m_pColorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMSolidColorEdit::displayObject( PMObject* o )
{
   if( o->isA( "SolidColor" ) )
   {
      m_pDisplayedObject = ( PMSolidColor* ) o;
      m_pColorEdit->setColor( m_pDisplayedObject->color() );

      m_pColorEdit->setReadOnly( m_pDisplayedObject->isReadOnly() );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMSolidColorEdit: Can't display object\n";
}

void PMSolidColorEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setColor( m_pColorEdit->color() );
   }
}

bool PMSolidColorEdit::isDataValid()
{
   if( !m_pColorEdit->isDataValid() )
      return false;
   return Base::isDataValid();
}

#include "pmsolidcoloredit.moc"
