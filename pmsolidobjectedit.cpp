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


#include "pmsolidobjectedit.h"
#include "pmsolidobject.h"

#include <qcheckbox.h>
#include <qlayout.h>


PMSolidObjectEdit::PMSolidObjectEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMSolidObjectEdit::createBottomWidgets()
{
   m_pInverseButton = new QCheckBox( tr( "Inverse" ), this );
   m_pHollowButton = new QCheckBox( tr( "Hollow" ), this );

   m_pHollowButton->setTristate( true );
   
   topLayout()->addWidget( m_pInverseButton );
   topLayout()->addWidget( m_pHollowButton );

   connect( m_pHollowButton, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pInverseButton, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );

   Base::createBottomWidgets();
}

void PMSolidObjectEdit::displayObject( PMObject* o )
{
   if( o->isA( "SolidObject" ) )
   {
      bool readOnly = o->isReadOnly();
      
      m_pDisplayedObject = ( PMSolidObject* ) o;
      setCheckBox( m_pHollowButton, m_pDisplayedObject->hollow() );
      m_pInverseButton->setChecked( m_pDisplayedObject->inverse() );

      m_pHollowButton->setEnabled( !readOnly );
      m_pInverseButton->setEnabled( !readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMSolidObjectEdit: Can't display object\n";
}

void PMSolidObjectEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setHollow( checkBoxState( m_pHollowButton ) );
      m_pDisplayedObject->setInverse( m_pInverseButton->isChecked() );
      Base::saveContents();
   }
}

bool PMSolidObjectEdit::isDataValid()
{
   return Base::isDataValid();
}

#include "pmsolidobjectedit.moc"
