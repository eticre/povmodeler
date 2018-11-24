/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmphotonsedit.h"
#include "pmphotons.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>



PMPhotonsEdit::PMPhotonsEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMPhotonsEdit::createTopWidgets()
{
   QGridLayout* gl;
	QHBoxLayout* hl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   m_pLayoutWidget = new QWidget( this );
   m_pTarget = new QCheckBox( tr( "Target" ), m_pLayoutWidget );
   m_pSpacingMultiLabel = new QLabel( tr( "Spacing multiplier:" ), m_pLayoutWidget );
   m_pSpacingMulti = new PMFloatEdit( m_pLayoutWidget );
   m_pSpacingMulti->setValidation( true, 0, false, 0 );

   m_pRefraction = new QCheckBox( tr( "Refraction" ), this );
   m_pReflection = new QCheckBox( tr( "Reflection" ), this );
   m_pCollect = new QCheckBox( tr( "Collect" ), this );
   m_pPassThrough = new QCheckBox( tr( "Pass through" ), this );
   m_pAreaLight = new QCheckBox( tr( "Area light" ), this );

	hl = new QHBoxLayout( m_pLayoutWidget );
//TODO PORT QT5 	hl->setSpacing( QDialog::spacingHint() );
	hl->setMargin( 0 );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( m_pTarget, 0, 0, 1, 2 );
   gl->addWidget( m_pSpacingMultiLabel, 1, 0 );
   gl->addWidget( m_pSpacingMulti, 1, 1 );
	hl->addStretch( 1 );
   tl->addWidget( m_pLayoutWidget );

   gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( m_pRefraction, 0, 0 );
   gl->addWidget( m_pReflection, 0, 1 );
   gl->addWidget( m_pCollect, 1, 0 );
   gl->addWidget( m_pPassThrough, 1, 1 );
   gl->addWidget( m_pAreaLight, 1, 0 );

   connect( m_pTarget, SIGNAL( clicked() ), SLOT( slotTargetClicked() ) );
   connect( m_pSpacingMulti, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRefraction, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pReflection, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pCollect, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pPassThrough, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pAreaLight, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMPhotonsEdit::displayObject( PMObject* o )
{
   if( o->isA( "Photons" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMPhotons* ) o;

      if ( o->parent() && ( o->parent()->isA( "Light" ) ) )
      {
         m_pLayoutWidget->hide();
         m_pCollect->hide();
         m_pPassThrough->hide();
         m_pAreaLight->show();
      }
      else
      {
         m_pLayoutWidget->show();
         m_pCollect->show();
         m_pPassThrough->show();
         m_pAreaLight->hide();
      }

      m_pTarget->setChecked( m_pDisplayedObject->target() );
      m_pTarget->setEnabled( !readOnly );
      m_pSpacingMulti->setValue( m_pDisplayedObject->spacingMulti() );
      m_pSpacingMulti->setReadOnly( readOnly );
      m_pRefraction->setChecked( m_pDisplayedObject->refraction() );
      m_pRefraction->setEnabled( !readOnly );
      m_pReflection->setChecked( m_pDisplayedObject->reflection() );
      m_pReflection->setEnabled( !readOnly );
      m_pCollect->setChecked( m_pDisplayedObject->collect() );
      m_pCollect->setEnabled( !readOnly );
      m_pPassThrough->setChecked( m_pDisplayedObject->passThrough() );
      m_pPassThrough->setEnabled( !readOnly );
      m_pAreaLight->setChecked( m_pDisplayedObject->areaLight() );
      m_pAreaLight->setEnabled( !readOnly );

      slotTargetClicked();

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMPhotonsEdit: Can't display object\n";
}

void PMPhotonsEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setTarget( m_pTarget->isChecked() );
      m_pDisplayedObject->setSpacingMulti( m_pSpacingMulti->value() );
      m_pDisplayedObject->setRefraction( m_pRefraction->isChecked() );
      m_pDisplayedObject->setReflection( m_pReflection->isChecked() );
      m_pDisplayedObject->setCollect( m_pCollect->isChecked() );
      m_pDisplayedObject->setPassThrough( m_pPassThrough->isChecked() );
      m_pDisplayedObject->setAreaLight( m_pAreaLight->isChecked() );
   }
}

bool PMPhotonsEdit::isDataValid()
{
   if( !m_pSpacingMulti->isDataValid() ) return false;

   return Base::isDataValid();
}

void PMPhotonsEdit::slotTargetClicked()
{
   if ( m_pTarget->isChecked() && m_pTarget->isEnabled() )
   {
      m_pSpacingMulti->setEnabled( true );
   }
   else
   {
      m_pSpacingMulti->setEnabled( false );
   }
	emit dataChanged();
}

#include "pmphotonsedit.moc"
