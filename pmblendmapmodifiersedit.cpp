/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
    copyright            : (C) 2001 by Andreas Zehender
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


#include "pmblendmapmodifiersedit.h"
#include "pmblendmapmodifiers.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmvector.h"

#include <QLayout>
#include <QComboBox>
#include <QCheckBox>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QHBoxLayout>

PMBlendMapModifiersEdit::PMBlendMapModifiersEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMBlendMapModifiersEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEnableFrequencyEdit = new QCheckBox( tr( "Frequency:" ), this );
   m_pFrequencyEdit = new PMFloatEdit( this );
   hl->addWidget( m_pEnableFrequencyEdit );
   hl->addWidget( m_pFrequencyEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEnablePhaseEdit = new QCheckBox( tr( "Phase:" ), this );
   m_pPhaseEdit = new PMFloatEdit( this );
   hl->addWidget( m_pEnablePhaseEdit );
   hl->addWidget( m_pPhaseEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEnableWaveFormEdit = new QCheckBox( tr( "Wave form:" ), this );
   m_pWaveTypeCombo = new QComboBox( this );
   m_pWaveTypeCombo->addItem( tr( "Ramp" ) );
   m_pWaveTypeCombo->addItem( tr( "Triangle" ) );
   m_pWaveTypeCombo->addItem( tr( "Sine" ) );
   m_pWaveTypeCombo->addItem( tr( "Scallop" ) );
   m_pWaveTypeCombo->addItem( tr( "Cubic" ) );
   m_pWaveTypeCombo->addItem( tr( "Poly" ) );
   hl->addWidget( m_pEnableWaveFormEdit );
   hl->addWidget( m_pWaveTypeCombo );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pWaveExponentLabel = new QLabel( tr( "Exponent:" ), this );
   m_pWaveExponentEdit = new PMFloatEdit( this );
   hl->addWidget( m_pWaveExponentLabel );
   hl->addWidget( m_pWaveExponentEdit );
   hl->addStretch( 1 );

   connect( m_pEnableFrequencyEdit, SIGNAL( clicked() ), SLOT( slotFrequencyClicked() ) );
   connect( m_pFrequencyEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnablePhaseEdit, SIGNAL( clicked() ), SLOT( slotPhaseClicked() ) );
   connect( m_pPhaseEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnableWaveFormEdit, SIGNAL( clicked() ), SLOT( slotWaveFormClicked() ) );
   connect( m_pWaveExponentEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pWaveTypeCombo, SIGNAL( activated( int ) ), SLOT( slotTypeComboChanged( int ) ) );
}

void PMBlendMapModifiersEdit::displayObject( PMObject* o )
{
   QString str;

   if( o->isA( "BlendMapModifiers" ) )
   {
      m_pDisplayedObject = static_cast<PMBlendMapModifiers*>(o);

      m_pEnableFrequencyEdit->setChecked( m_pDisplayedObject->isFrequencyEnabled() );
      m_pEnablePhaseEdit->setChecked( m_pDisplayedObject->isPhaseEnabled() );
      m_pEnableWaveFormEdit->setChecked( m_pDisplayedObject->isWaveFormEnabled() );

      m_pFrequencyEdit->setValue( m_pDisplayedObject->frequency() );
      m_pPhaseEdit->setValue( m_pDisplayedObject->phase() );
      m_pWaveExponentEdit->setValue( m_pDisplayedObject->waveFormExponent() );

      switch( m_pDisplayedObject->waveFormType() )
      {
         case PMBlendMapModifiers::RampWave:
            m_pWaveTypeCombo->setCurrentIndex( 0 );
            break;
         case PMBlendMapModifiers::TriangleWave:
            m_pWaveTypeCombo->setCurrentIndex( 1 );
            break;
         case PMBlendMapModifiers::SineWave:
            m_pWaveTypeCombo->setCurrentIndex( 2 );
            break;
         case PMBlendMapModifiers::ScallopWave:
            m_pWaveTypeCombo->setCurrentIndex( 3 );
            break;
         case PMBlendMapModifiers::CubicWave:
            m_pWaveTypeCombo->setCurrentIndex( 4 );
            break;
         case PMBlendMapModifiers::PolyWave:
            m_pWaveTypeCombo->setCurrentIndex( 5 );
            break;
      }

      slotFrequencyClicked();
      slotPhaseClicked();
      slotWaveFormClicked();
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMBlendMapModifiersEdit: Can't display object\n";
}

void PMBlendMapModifiersEdit::setBlendMapModifiersType( int i )
{
   m_pWaveTypeCombo->setCurrentIndex( i );
   slotTypeComboChanged( i );
}

void PMBlendMapModifiersEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();

      m_pDisplayedObject->enableFrequency( m_pEnableFrequencyEdit->isChecked() );
      m_pDisplayedObject->enablePhase( m_pEnablePhaseEdit->isChecked() );
      m_pDisplayedObject->enableWaveForm( m_pEnableWaveFormEdit->isChecked() );

      if( m_pEnableFrequencyEdit->isChecked() )
         m_pDisplayedObject->setFrequency( m_pFrequencyEdit->value() );

      if( m_pEnablePhaseEdit->isChecked() )
         m_pDisplayedObject->setPhase( m_pPhaseEdit->value() );

      if( m_pEnableWaveFormEdit->isChecked() )
      {
         switch( m_pWaveTypeCombo->currentIndex() )
         {
            case 0: /* Ramp Wave */
               m_pDisplayedObject->setWaveFormType( PMBlendMapModifiers::RampWave );
               break;
            case 1: /* Triangle Wave */
               m_pDisplayedObject->setWaveFormType( PMBlendMapModifiers::TriangleWave );
               break;
            case 2: /* Sine Wave */
              m_pDisplayedObject->setWaveFormType( PMBlendMapModifiers::SineWave );
              break;
            case 3: /* Scallop Wave */
               m_pDisplayedObject->setWaveFormType( PMBlendMapModifiers::ScallopWave );
               break;
            case 4: /* Cubic Wave */
               m_pDisplayedObject->setWaveFormType( PMBlendMapModifiers::CubicWave );
               break;
            case 5: /* Poly Wave */
               m_pDisplayedObject->setWaveFormType( PMBlendMapModifiers::PolyWave );
               m_pDisplayedObject->setWaveFormExponent( m_pWaveExponentEdit->value() );
               break;
         }
      }
   }
}

bool PMBlendMapModifiersEdit::isDataValid()
{
   if( !m_pFrequencyEdit->isDataValid() ) return false;
   if( !m_pPhaseEdit->isDataValid() ) return false;

   switch( m_pWaveTypeCombo->currentIndex() )
   {
      case 5: /* Poly Wave */
         if( !m_pWaveExponentEdit->isDataValid() ) return false;
         break;
   }

   return Base::isDataValid();
}

void PMBlendMapModifiersEdit::slotTypeComboChanged( int c )
{
   switch( c )
   {
      case 5: /* Poly Wave */
         m_pWaveExponentLabel->show();
         m_pWaveExponentEdit->show();
         break;
      default:
         m_pWaveExponentLabel->hide();
         m_pWaveExponentEdit->hide();
         break;
   }  
   emit dataChanged();
   emit sizeChanged();
}

void PMBlendMapModifiersEdit::slotFrequencyClicked()
{
   if( m_pEnableFrequencyEdit->isChecked() )
      m_pFrequencyEdit->setEnabled( true );
   else
      m_pFrequencyEdit->setEnabled( false );
   emit dataChanged();
   emit sizeChanged();
}

void PMBlendMapModifiersEdit::slotPhaseClicked()
{
   if(m_pEnablePhaseEdit->isChecked() )
      m_pPhaseEdit->setEnabled( true );
   else
      m_pPhaseEdit->setEnabled( false );
   emit dataChanged();
   emit sizeChanged();
}

void PMBlendMapModifiersEdit::slotWaveFormClicked()
{
   if(m_pEnableWaveFormEdit->isChecked() )
   {
      m_pWaveTypeCombo->setEnabled( true );
      slotTypeComboChanged( m_pWaveTypeCombo->currentIndex() );
   }
   else
   {
      m_pWaveTypeCombo->setEnabled( false );
      m_pWaveExponentLabel->hide();
      m_pWaveExponentEdit->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

#include "pmblendmapmodifiersedit.moc"
