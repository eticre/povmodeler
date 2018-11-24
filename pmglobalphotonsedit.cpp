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


#include "pmglobalphotonsedit.h"
#include "pmglobalphotons.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>


PMGlobalPhotonsEdit::PMGlobalPhotonsEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMGlobalPhotonsEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QGridLayout* gl;
   QLabel* lbl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( tr( "Photon numbers" ), this );
   m_pNumberType = new QComboBox( this );
   m_pNumberType->addItem( tr( "Spacing" ) );
   m_pNumberType->addItem( tr( "Count" ) );
   m_pSpacing = new PMFloatEdit( this );
   m_pSpacing->setValidation( true, 0, false, 0 );
   m_pCount = new PMIntEdit( this );
   m_pCount->setValidation( true, 0, false, 0 );
   hl->addWidget( lbl );
   hl->addWidget( m_pNumberType );
   hl->addWidget( m_pSpacing );
   hl->addWidget( m_pCount );
   hl->addStretch( 1 );

   gl = new QGridLayout();
   tl->addLayout( gl );
   lbl = new QLabel( tr( "Gather" ), this );
   gl->addWidget( lbl, 0, 0 );
   lbl = new QLabel( tr( "Min:" ), this );
   gl->addWidget( lbl, 0, 1 );
   m_pGatherMin = new PMIntEdit( this );
   m_pGatherMin->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pGatherMin, 0, 2 );
   lbl = new QLabel( tr( "Max:" ), this );
   gl->addWidget( lbl, 0, 3 );
   m_pGatherMax = new PMIntEdit( this );
   m_pGatherMax->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pGatherMax, 0, 4 );

   lbl = new QLabel( tr( "Media" ), this );
   gl->addWidget( lbl, 1, 0 );
   lbl = new QLabel( tr( "Max stop:" ), this );
   gl->addWidget( lbl, 1, 1 );
   m_pMediaMaxSteps = new PMIntEdit( this );
   m_pMediaMaxSteps->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pMediaMaxSteps, 1, 2 );
   lbl = new QLabel( tr( "Factor:" ), this );
   gl->addWidget( lbl,1, 3 );
   m_pMediaFactor = new PMFloatEdit( this );
   m_pMediaFactor->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pMediaFactor, 1, 4 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( tr( "Jitter:" ), this );
   m_pJitter = new PMFloatEdit( this );
   m_pJitter->setValidation ( true, 0, false, 0 );
   hl->addWidget( lbl );
   hl->addWidget( m_pJitter );
   hl->addStretch( 1 );

   gl = new QGridLayout();
   tl->addLayout( gl );
   lbl = new QLabel( tr( "Max trace level:" ), this );
   m_pMaxTraceLevel = new PMIntEdit ( this );
   m_pMaxTraceLevel->setValidation( true, 0, false, 0 );
   m_pMaxTraceLevelGlobal = new QCheckBox( tr( "Use global" ), this );
   gl->addWidget( lbl, 0, 0 );
   gl->addWidget( m_pMaxTraceLevel, 0, 1 );
   gl->addWidget( m_pMaxTraceLevelGlobal, 0, 2 );

   lbl = new QLabel( tr( "Adc bailout:" ), this );
   m_pAdcBailout = new PMFloatEdit ( this );
   m_pAdcBailout->setValidation( true, 0, true, 1 );
   m_pAdcBailoutGlobal = new QCheckBox( tr( "Use global" ), this );
   gl->addWidget( lbl, 1, 0 );
   gl->addWidget( m_pAdcBailout, 1, 1 );
   gl->addWidget( m_pAdcBailoutGlobal, 1, 2 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( tr( "Autostop:" ), this );
   m_pAutostop = new PMFloatEdit ( this );
   m_pAutostop->setValidation( true, 0, true, 1 );
   hl->addWidget( lbl );
   hl->addWidget( m_pAutostop );
   hl->addStretch( 1 );

   gl = new QGridLayout();
   tl->addLayout( gl );
   lbl = new QLabel( tr( "Expand" ), this );
   gl->addWidget( lbl, 0, 0 );
   lbl = new QLabel( tr( "Increase:" ), this );
   gl->addWidget( lbl, 0, 1 );
   m_pExpandIncrease = new PMFloatEdit( this );
   m_pExpandIncrease->setValidation( true, 0, true, 1 );
   gl->addWidget( m_pExpandIncrease, 0, 2 );
   lbl = new QLabel( tr( "Minimum:" ), this );
   gl->addWidget( lbl, 0, 3 );
   m_pExpandMin = new PMIntEdit( this );
   m_pExpandMin->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pExpandMin, 0, 4 );

   lbl = new QLabel( tr( "Gather" ), this );
   gl->addWidget( lbl, 1, 0 );
   lbl = new QLabel( tr( "Radius:" ), this );
   gl->addWidget( lbl, 1, 1 );
   m_pRadiusGather = new PMFloatEdit( this );
   m_pRadiusGather->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pRadiusGather, 1, 2 );
   lbl = new QLabel( tr( "Multiplier:" ), this );
   gl->addWidget( lbl, 1, 3 );
   m_pRadiusGatherMulti = new PMFloatEdit( this );
   m_pRadiusGatherMulti->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pRadiusGatherMulti, 1, 4 );

   lbl = new QLabel( tr( "Media" ), this );
   gl->addWidget( lbl, 2, 0 );
   lbl = new QLabel( tr( "Radius:" ), this );
   gl->addWidget( lbl, 2, 1 );
   m_pRadiusMedia = new PMFloatEdit( this );
   m_pRadiusMedia->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pRadiusMedia, 2, 2 );
   lbl = new QLabel( tr( "Multiplier:" ), this );
   gl->addWidget( lbl, 2, 3 );
   m_pRadiusMediaMulti = new PMFloatEdit( this );
   m_pRadiusMediaMulti->setValidation( true, 0, false, 0 );
   gl->addWidget( m_pRadiusMediaMulti, 2, 4 );

   connect( m_pNumberType, SIGNAL( activated( int ) ),
            SLOT( slotNumberTypeActivated( int ) ) );
   connect( m_pSpacing, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCount, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pGatherMin, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pGatherMax, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMediaMaxSteps, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMediaFactor, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pJitter, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMaxTraceLevelGlobal, SIGNAL( clicked() ),
            SLOT( slotMaxTraceLevelGlobalClicked() ) );
   connect( m_pMaxTraceLevel, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAdcBailoutGlobal, SIGNAL( clicked() ),
            SLOT( slotAdcBailoutGlobalClicked() ) );
   connect( m_pAdcBailout, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAutostop, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pExpandIncrease, SIGNAL( dataChanged() ), SIGNAL ( dataChanged() ) );
   connect( m_pExpandMin, SIGNAL( dataChanged() ), SIGNAL ( dataChanged() ) );
   connect( m_pRadiusGather, SIGNAL( dataChanged() ), SIGNAL ( dataChanged() ) );
   connect( m_pRadiusGatherMulti, SIGNAL( dataChanged() ), SIGNAL ( dataChanged() ) );
   connect( m_pRadiusMedia, SIGNAL( dataChanged() ), SIGNAL ( dataChanged() ) );
   connect( m_pRadiusMediaMulti, SIGNAL( dataChanged() ), SIGNAL ( dataChanged() ) );
}

void PMGlobalPhotonsEdit::displayObject( PMObject* o )
{
   if( o->isA( "GlobalPhotons" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMGlobalPhotons* ) o;

      m_pNumberType->setCurrentIndex( m_pDisplayedObject->numberType() );
      m_pNumberType->setEnabled( !readOnly );
      m_pSpacing->setValue( m_pDisplayedObject->spacing() );
      m_pSpacing->setReadOnly( readOnly );
      m_pCount->setValue( m_pDisplayedObject->count() );
      m_pCount->setReadOnly( readOnly );
      m_pGatherMin->setValue( m_pDisplayedObject->gatherMin() );
      m_pGatherMin->setReadOnly( readOnly );
      m_pGatherMax->setValue( m_pDisplayedObject->gatherMax() );
      m_pGatherMax->setReadOnly( readOnly );
      m_pMediaMaxSteps->setValue( m_pDisplayedObject->mediaMaxSteps() );
      m_pMediaMaxSteps->setReadOnly( readOnly );
      m_pMediaFactor->setValue( m_pDisplayedObject->mediaFactor() );
      m_pMediaFactor->setReadOnly( readOnly );
      m_pJitter->setValue( m_pDisplayedObject->jitter() );
      m_pJitter->setReadOnly( readOnly );
      m_pMaxTraceLevelGlobal->setChecked( m_pDisplayedObject->maxTraceLevelGlobal() );
      m_pMaxTraceLevelGlobal->setEnabled( !readOnly );
      m_pMaxTraceLevel->setValue( m_pDisplayedObject->maxTraceLevel() );
      m_pMaxTraceLevel->setReadOnly( readOnly );
      m_pAdcBailoutGlobal->setChecked( m_pDisplayedObject->adcBailoutGlobal() );
      m_pAdcBailoutGlobal->setEnabled( !readOnly );
      m_pAdcBailout->setValue( m_pDisplayedObject->adcBailout() );
      m_pAdcBailout->setReadOnly( readOnly );
      m_pAutostop->setValue( m_pDisplayedObject->autostop() );
      m_pAutostop->setReadOnly( readOnly );
      m_pExpandIncrease->setValue( m_pDisplayedObject->expandIncrease() );
      m_pExpandIncrease->setReadOnly( readOnly );
      m_pExpandMin->setValue( m_pDisplayedObject->expandMin() );
      m_pExpandMin->setReadOnly( readOnly );
      m_pRadiusGather->setValue( m_pDisplayedObject->radiusGather() );
      m_pRadiusGather->setReadOnly( readOnly );
      m_pRadiusGatherMulti->setValue( m_pDisplayedObject->radiusGatherMulti() );
      m_pRadiusGatherMulti->setReadOnly( readOnly );
      m_pRadiusMedia->setValue( m_pDisplayedObject->radiusMedia() );
      m_pRadiusMedia->setReadOnly( readOnly );
      m_pRadiusMediaMulti->setValue( m_pDisplayedObject->radiusMediaMulti() );
      m_pRadiusMediaMulti->setReadOnly( readOnly );

      slotNumberTypeActivated( m_pDisplayedObject->numberType() );
      slotMaxTraceLevelGlobalClicked();
      slotAdcBailoutGlobalClicked();

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMGlobalPhotonsEdit: Can't display object\n";
}

void PMGlobalPhotonsEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setNumberType(
         ( PMGlobalPhotons::PMNumberType ) m_pNumberType->currentIndex() );
      m_pDisplayedObject->setSpacing( m_pSpacing->value() );
      m_pDisplayedObject->setCount( m_pCount->value() );
      m_pDisplayedObject->setGatherMin( m_pGatherMin->value() );
      m_pDisplayedObject->setGatherMax( m_pGatherMax->value() );
      m_pDisplayedObject->setMediaMaxSteps( m_pMediaMaxSteps->value() );
      m_pDisplayedObject->setMediaFactor( m_pMediaFactor->value() );
      m_pDisplayedObject->setJitter( m_pJitter->value() );
      m_pDisplayedObject->setMaxTraceLevelGlobal(
         m_pMaxTraceLevelGlobal->isChecked() );
      m_pDisplayedObject->setMaxTraceLevel( m_pMaxTraceLevel->value() );
      m_pDisplayedObject->setAdcBailoutGlobal(
         m_pAdcBailoutGlobal->isChecked() );
      m_pDisplayedObject->setAdcBailout( m_pAdcBailout->value() );
      m_pDisplayedObject->setAutostop( m_pAutostop->value() );
      m_pDisplayedObject->setExpandIncrease( m_pExpandIncrease->value() );
      m_pDisplayedObject->setExpandMin( m_pExpandMin->value() );
      m_pDisplayedObject->setRadiusGather( m_pRadiusGather->value() );
      m_pDisplayedObject->setRadiusGatherMulti( m_pRadiusGatherMulti->value() );
      m_pDisplayedObject->setRadiusMedia( m_pRadiusMedia->value() );
      m_pDisplayedObject->setRadiusMediaMulti( m_pRadiusMediaMulti->value() );
   }
}

bool PMGlobalPhotonsEdit::isDataValid()
{
   if( !m_pSpacing->isDataValid() ) return false;
   if( !m_pCount->isDataValid() ) return false;
   if( !m_pGatherMin->isDataValid() ) return false;
   if( !m_pGatherMax->isDataValid() ) return false;
   if( !m_pMediaMaxSteps->isDataValid() ) return false;
   if( !m_pMediaFactor->isDataValid() ) return false;
   if( !m_pJitter->isDataValid() ) return false;
   if( !m_pMaxTraceLevel->isDataValid() ) return false;
   if( !m_pAdcBailout->isDataValid() ) return false;
   if( !m_pAutostop->isDataValid() ) return false;
   if( !m_pExpandIncrease->isDataValid() ) return false;
   if( !m_pExpandMin->isDataValid() ) return false;
   if( !m_pRadiusGather->isDataValid() ) return false;
   if( !m_pRadiusGatherMulti->isDataValid() ) return false;
   if( !m_pRadiusMedia->isDataValid() ) return false;
   if( !m_pRadiusMediaMulti->isDataValid() ) return false;

   return Base::isDataValid();
}

void PMGlobalPhotonsEdit::slotNumberTypeActivated( int index )
{
   if( index == 0 )
   {
      m_pSpacing->show();
      m_pCount->hide();
   }
   else
   {
      m_pSpacing->hide();
      m_pCount->show();
   }
   emit dataChanged();
}

void PMGlobalPhotonsEdit::slotMaxTraceLevelGlobalClicked()
{
   if( m_pMaxTraceLevelGlobal->isChecked() )
      m_pMaxTraceLevel->setEnabled( false );
   else if ( m_pMaxTraceLevelGlobal->isEnabled() )
      m_pMaxTraceLevel->setEnabled( true );
   emit dataChanged();
   emit sizeChanged();
}

void PMGlobalPhotonsEdit::slotAdcBailoutGlobalClicked()
{
   if( m_pAdcBailoutGlobal->isChecked() )
      m_pAdcBailout->setEnabled( false );
   else if ( m_pAdcBailoutGlobal->isEnabled() )
      m_pAdcBailout->setEnabled( true );
   emit dataChanged();
   emit sizeChanged();
}

#include "pmglobalphotonsedit.moc"
