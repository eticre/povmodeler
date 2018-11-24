/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
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


#include "pminterioredit.h"
#include "pminterior.h"
#include "pmlineedits.h"
#include "pmcoloredit.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>



PMInteriorEdit::PMInteriorEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMInteriorEdit::createTopWidgets()
{
   Base::createTopWidgets();

   QBoxLayout* tl = topLayout();
   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   QGridLayout* layout = new QGridLayout();
   hl->addLayout( layout );
   m_pEnableIorEdit = new QCheckBox( tr( "Refraction:" ), this );
   m_pIorEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableIorEdit, 0, 0 );
   layout->addWidget( m_pIorEdit, 0, 1 );
   m_pEnableCausticsEdit = new QCheckBox( tr( "Caustics:" ), this );
   m_pCausticsEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableCausticsEdit, 1, 0 );
   layout->addWidget( m_pCausticsEdit, 1, 1 );
   m_pEnableDispersionEdit = new QCheckBox( tr( "Dispersion:" ), this );
   m_pDispersionEdit = new PMFloatEdit( this );
   m_pDispersionEdit->setValidation( true, 0, false, 0 );
   layout->addWidget( m_pEnableDispersionEdit, 2, 0 );
   layout->addWidget( m_pDispersionEdit, 2, 1 );
   m_pEnableDispSamplesEdit = new QCheckBox( tr( "Dispersion samples:" ), this );
   m_pDispSamplesEdit = new PMIntEdit( this );
   m_pDispSamplesEdit->setValidation( true, 2, false, 0 );
   layout->addWidget( m_pEnableDispSamplesEdit, 3, 0 );
   layout->addWidget( m_pDispSamplesEdit, 3, 1 );
   m_pEnableFadeDistanceEdit = new QCheckBox( tr( "Fade distance:" ), this );
   m_pFadeDistanceEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableFadeDistanceEdit, 4, 0 );
   layout->addWidget( m_pFadeDistanceEdit, 4, 1 );
   m_pEnableFadePowerEdit = new QCheckBox( tr( "Fade power:" ), this );
   m_pFadePowerEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableFadePowerEdit, 5, 0 );
   layout->addWidget( m_pFadePowerEdit, 5, 1 );
   hl->addStretch( 1 );

   connect( m_pIorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCausticsEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDispersionEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDispSamplesEdit, SIGNAL( dataChanged() ), SIGNAL ( dataChanged() ) );
   connect( m_pFadeDistanceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFadePowerEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnableIorEdit, SIGNAL( clicked() ), SLOT( slotIorClicked() ) );
   connect( m_pEnableCausticsEdit, SIGNAL( clicked() ), SLOT( slotCausticsClicked() ) );
   connect( m_pEnableDispersionEdit, SIGNAL( clicked() ), SLOT( slotDispersionClicked() ) );
   connect( m_pEnableDispSamplesEdit, SIGNAL( clicked() ), SLOT( slotDispSamplesClicked() ) );
   connect( m_pEnableFadeDistanceEdit, SIGNAL( clicked() ), SLOT( slotFadeDistanceClicked() ) );
   connect( m_pEnableFadePowerEdit, SIGNAL( clicked() ), SLOT( slotFadePowerClicked() ) );
}

void PMInteriorEdit::displayObject( PMObject* o )
{
   if( o->isA( "Interior" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMInterior* ) o;

      m_pIorEdit->setValue( m_pDisplayedObject->ior() );
      m_pIorEdit->setReadOnly( readOnly );
      m_pCausticsEdit->setValue( m_pDisplayedObject->caustics() );
      m_pCausticsEdit->setReadOnly( readOnly );
      m_pDispersionEdit->setValue( m_pDisplayedObject->dispersion() );
      m_pDispersionEdit->setReadOnly( readOnly );
      m_pDispSamplesEdit->setValue( m_pDisplayedObject->dispSamples() );
      m_pDispSamplesEdit->setReadOnly( readOnly );
      m_pFadeDistanceEdit->setValue( m_pDisplayedObject->fadeDistance() );
      m_pFadeDistanceEdit->setReadOnly( readOnly );
      m_pFadePowerEdit->setValue( m_pDisplayedObject->fadePower() );
      m_pFadePowerEdit->setReadOnly( readOnly );
      m_pEnableIorEdit->setChecked( m_pDisplayedObject->isIorEnabled() );
      m_pEnableIorEdit->setEnabled( !readOnly );
      m_pEnableCausticsEdit->setChecked( m_pDisplayedObject->isCausticsEnabled() );
      m_pEnableCausticsEdit->setEnabled( !readOnly );
      m_pEnableDispersionEdit->setChecked( m_pDisplayedObject->isDispersionEnabled() );
      m_pEnableDispersionEdit->setEnabled( !readOnly );
      m_pEnableDispSamplesEdit->setChecked( m_pDisplayedObject->isDispSamplesEnabled() );
      m_pEnableDispSamplesEdit->setEnabled( !readOnly );
      m_pEnableFadeDistanceEdit->setChecked( m_pDisplayedObject->isFadeDistanceEnabled() );
      m_pEnableFadeDistanceEdit->setEnabled( !readOnly );
      m_pEnableFadePowerEdit->setChecked( m_pDisplayedObject->isFadePowerEnabled() );
      m_pEnableFadePowerEdit->setEnabled( !readOnly );
      slotIorClicked();
      slotCausticsClicked();
      slotDispersionClicked();
      slotDispSamplesClicked();
      slotFadeDistanceClicked();
      slotFadePowerClicked();

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMInteriorEdit: Can't display object\n";
}

void PMInteriorEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setIor( m_pIorEdit->value() );
      m_pDisplayedObject->setCaustics( m_pCausticsEdit->value() );
      m_pDisplayedObject->setDispersion( m_pDispersionEdit->value() );
      m_pDisplayedObject->setDispSamples( m_pDispSamplesEdit->value() );
      m_pDisplayedObject->setFadeDistance( m_pFadeDistanceEdit->value() );
      m_pDisplayedObject->setFadePower( m_pFadePowerEdit->value() );
      m_pDisplayedObject->enableIor( m_pEnableIorEdit->isChecked() );
      m_pDisplayedObject->enableCaustics( m_pEnableCausticsEdit->isChecked() );
      m_pDisplayedObject->enableDispersion( m_pEnableDispersionEdit->isChecked() );
      m_pDisplayedObject->enableDispSamples( m_pEnableDispSamplesEdit->isChecked() );
      m_pDisplayedObject->enableFadeDistance( m_pEnableFadeDistanceEdit->isChecked() );
      m_pDisplayedObject->enableFadePower( m_pEnableFadePowerEdit->isChecked() );
   }
}

bool PMInteriorEdit::isDataValid()
{
   if( !m_pIorEdit->isDataValid() ) return false;
   if( !m_pCausticsEdit->isDataValid() ) return false;
   if( !m_pDispersionEdit->isDataValid() ) return false;
   if( !m_pDispSamplesEdit->isDataValid() ) return false;
   if( !m_pFadeDistanceEdit->isDataValid() ) return false;
   if( !m_pFadePowerEdit->isDataValid() ) return false;
   if( !m_pFadeDistanceEdit->isDataValid() ) return false;

   return Base::isDataValid();
}

void PMInteriorEdit::slotIorClicked()
{
   if( m_pEnableIorEdit->isChecked() )
   {
      m_pIorEdit->setEnabled( true );
      m_pEnableDispersionEdit->setEnabled( true );
      m_pEnableDispSamplesEdit->setEnabled( true );
   }
   else
   {
      m_pIorEdit->setEnabled( false );
      m_pEnableDispersionEdit->setEnabled( false );
      m_pEnableDispSamplesEdit->setEnabled( false );
      m_pEnableDispersionEdit->setChecked( false );
      m_pEnableDispSamplesEdit->setChecked( false );
      slotDispersionClicked();
      slotDispSamplesClicked();
   }
   emit dataChanged();
}

void PMInteriorEdit::slotCausticsClicked()
{
   m_pCausticsEdit->setEnabled( m_pEnableCausticsEdit->isChecked() );
   emit dataChanged();
}

void PMInteriorEdit::slotDispersionClicked()
{
   m_pDispersionEdit->setEnabled( m_pEnableDispersionEdit->isChecked() );
   emit dataChanged();
}

void PMInteriorEdit::slotDispSamplesClicked()
{
   m_pDispSamplesEdit->setEnabled( m_pEnableDispSamplesEdit->isChecked() );
   emit dataChanged();
}

void PMInteriorEdit::slotFadeDistanceClicked()
{
   m_pFadeDistanceEdit->setEnabled( m_pEnableFadeDistanceEdit->isChecked() );
   emit dataChanged();
}

void PMInteriorEdit::slotFadePowerClicked()
{
   m_pFadePowerEdit->setEnabled( m_pEnableFadePowerEdit->isChecked() );
   emit dataChanged();
}

#include "pminterioredit.moc"
