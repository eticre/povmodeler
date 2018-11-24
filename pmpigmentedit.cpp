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


#include "pmpigmentedit.h"
#include "pmpigment.h"
#include "pmlinkedit.h"

#include <QLayout>
#include <QCheckBox>



PMPigmentEdit::PMPigmentEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMPigmentEdit::createTopWidgets()
{
   Base::createTopWidgets();
   m_pUVMapping = new QCheckBox( tr( "UV mapping" ), this );
   topLayout()->addWidget( m_pUVMapping );

   connect( m_pUVMapping, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMPigmentEdit::displayObject( PMObject* o )
{
   if( o->isA( "Pigment" ) )
   {
      m_pDisplayedObject = reinterpret_cast<PMPigment*>( o );
      bool readOnly = m_pDisplayedObject->isReadOnly();

      m_pUVMapping->setChecked( m_pDisplayedObject->uvMapping() );
      m_pUVMapping->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMPigmentEdit: Can't display object\n";
}

void PMPigmentEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setUVMapping( m_pUVMapping->isChecked() );
   }
}

#include "pmpigmentedit.moc"
