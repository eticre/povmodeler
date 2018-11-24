/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmnormaledit.h"
#include "pmnormal.h"
#include "pmlinkedit.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>



PMNormalEdit::PMNormalEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMNormalEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pBumpSizeCheck = new QCheckBox( tr( "Bump size" ), this );
   m_pBumpSizeEdit = new PMFloatEdit( this );
   hl->addWidget( m_pBumpSizeCheck );
   hl->addWidget( m_pBumpSizeEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   QLabel* lbl = new QLabel( tr( "Accuracy" ), this );
   m_pAccuracy = new PMFloatEdit( this );
   hl->addWidget( lbl );
   hl->addWidget( m_pAccuracy );
   hl->addStretch( 1 );

   m_pUVMapping = new QCheckBox( tr( "UV mapping" ), this );
   tl->addWidget( m_pUVMapping );

   connect( m_pBumpSizeCheck, SIGNAL( clicked() ), SLOT( slotBumpSizeClicked()) );
   connect( m_pBumpSizeEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged()) );
   connect( m_pAccuracy, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pUVMapping, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMNormalEdit::displayObject( PMObject* o )
{
   if( o->isA( "Normal" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMNormal* )o;

      m_pBumpSizeCheck->setChecked( m_pDisplayedObject->isBumpSizeEnabled() );
      m_pBumpSizeCheck->setEnabled( !readOnly );
      m_pBumpSizeEdit->setValue( m_pDisplayedObject->bumpSize() );
      m_pBumpSizeEdit->setReadOnly( readOnly );
      slotBumpSizeClicked();
      m_pAccuracy->setValue( m_pDisplayedObject->accuracy() );
      m_pAccuracy->setReadOnly( readOnly );
      m_pUVMapping->setChecked( m_pDisplayedObject->uvMapping() );
      m_pUVMapping->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMNormalEdit: Can't display object\n";
}

void PMNormalEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->enableBumpSize( m_pBumpSizeCheck->isChecked() );
      m_pDisplayedObject->setBumpSize( m_pBumpSizeEdit->value() );
      m_pDisplayedObject->setAccuracy( m_pAccuracy->value() );
      m_pDisplayedObject->setUVMapping( m_pUVMapping->isChecked() );
   }
}

bool PMNormalEdit::isDataValid()
{
   if( !m_pBumpSizeEdit->isDataValid() ) return false;
   if( !m_pAccuracy->isDataValid() ) return false;

   return Base::isDataValid();
}

void PMNormalEdit::slotBumpSizeClicked()
{
   if( m_pBumpSizeCheck->isChecked() )
      m_pBumpSizeEdit->show();
   else
      m_pBumpSizeEdit->hide();
   emit sizeChanged();
   emit dataChanged();
}

#include "pmnormaledit.moc"
