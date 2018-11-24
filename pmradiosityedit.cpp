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


#include "pmradiosityedit.h"
#include "pmradiosity.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>



PMRadiosityEdit::PMRadiosityEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMRadiosityEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QGridLayout *gl;
   QLabel* lbl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );

   lbl = new QLabel( tr( "Adc bailout:" ), this );
   m_pAdcBailout = new PMFloatEdit( this );
   m_pAdcBailout->setValidation( true, 0, true, 1 );
   gl->addWidget( lbl, 0, 0 );
   gl->addWidget( m_pAdcBailout, 0, 1 );

   m_pAlwaysSample = new QCheckBox( tr( "Always sample" ), this );
   gl->addWidget( m_pAlwaysSample, 1, 0, 1, 2 );

   lbl = new QLabel( tr( "Brightness:" ), this );
   m_pBrightness = new PMFloatEdit( this );
   m_pBrightness->setValidation( true, 0, false, 0 );
   gl->addWidget( lbl, 2, 0 );
   gl->addWidget( m_pBrightness, 2, 1 );

   lbl = new QLabel( tr( "Count:" ), this );
   m_pCount = new PMIntEdit( this );
   m_pCount->setValidation( true, 0, true, 1600 );
   gl->addWidget( lbl, 3, 0 );
   gl->addWidget( m_pCount, 3, 1 );

   lbl = new QLabel( tr( "Error boundary:" ), this );
   m_pErrorBound = new PMFloatEdit( this );
   m_pErrorBound->setValidation( true, 0, false, 0 );
   gl->addWidget( lbl, 4, 0 );
   gl->addWidget( m_pErrorBound, 4, 1 );

   lbl = new QLabel( tr( "Gray threshold:" ), this );
   m_pGrayThreshold = new PMFloatEdit( this );
   m_pGrayThreshold->setValidation( true, 0, true, 1 );
   gl->addWidget( lbl, 5, 0 );
   gl->addWidget( m_pGrayThreshold, 5, 1 );

   lbl = new QLabel( tr( "Low error factor:" ), this );
   m_pLowErrorFactor = new PMFloatEdit( this );
   m_pLowErrorFactor->setValidation( true, 0, true, 1 );
   gl->addWidget( lbl, 6, 0 );
   gl->addWidget( m_pLowErrorFactor, 6, 1 );

   lbl = new QLabel( tr( "Maximum sample:" ), this );
   m_pMaxSample = new PMFloatEdit( this );
   m_pMaxSample->setValidation( true, -1, false, 0 );
   gl->addWidget( lbl, 7, 0 );
   gl->addWidget( m_pMaxSample, 7, 1 );

   m_pMedia = new QCheckBox( tr( "Media" ), this );
   gl->addWidget( m_pMedia, 8, 0, 1, 2 );

   lbl = new QLabel( tr( "Minimum reuse:" ), this );
   m_pMinimumReuse = new PMFloatEdit( this );
   m_pMinimumReuse->setValidation( true, 0, true, 1 );
   gl->addWidget( lbl, 9, 0 );
   gl->addWidget( m_pMinimumReuse, 9, 1 );

   lbl = new QLabel( tr( "Nearest count:" ), this );
   m_pNearestCount = new PMIntEdit( this );
   m_pNearestCount->setValidation( true, 0, true, 20 );
   gl->addWidget( lbl, 10, 0 );
   gl->addWidget( m_pNearestCount, 10, 1 );

   m_pNormal = new QCheckBox( tr( "Normal" ), this );
   gl->addWidget( m_pNormal, 11, 0, 1, 2 );

   lbl = new QLabel( tr( "Pretrace start:" ), this );
   m_pPretraceStart = new PMFloatEdit( this );
   m_pPretraceStart->setValidation( true, 0, true, 1 );
   gl->addWidget( lbl, 12, 0 );
   gl->addWidget( m_pPretraceStart, 12, 1 );

   lbl = new QLabel( tr( "Pretrace end:" ), this );
   m_pPretraceEnd = new PMFloatEdit( this );
   m_pPretraceEnd->setValidation( true, 0, true, 1 );
   gl->addWidget( lbl, 13, 0 );
   gl->addWidget( m_pPretraceEnd, 13, 1 );

   lbl = new QLabel( tr( "Recursion limit:" ), this );
   m_pRecursionLimit = new PMIntEdit( this );
   m_pRecursionLimit->setValidation( true, 1, true, 20 );
   gl->addWidget( lbl, 14, 0 );
   gl->addWidget( m_pRecursionLimit, 14, 1 );

   hl->addStretch( 1 );

   connect( m_pAdcBailout, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAlwaysSample, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pBrightness, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCount, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pErrorBound, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pGrayThreshold, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pLowErrorFactor, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMaxSample, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMedia, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pMinimumReuse, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pNearestCount, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pNormal, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pPretraceStart, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pPretraceEnd, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRecursionLimit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMRadiosityEdit::displayObject( PMObject* o )
{
   if( o->isA( "Radiosity" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMRadiosity* ) o;

      m_pAdcBailout->setValue( m_pDisplayedObject->adcBailout() );
      m_pAdcBailout->setReadOnly( readOnly );
      m_pAlwaysSample->setChecked( m_pDisplayedObject->alwaysSample() );
      m_pAlwaysSample->setEnabled( !readOnly );
      m_pBrightness->setValue( m_pDisplayedObject->brightness() );
      m_pBrightness->setReadOnly( readOnly );
      m_pCount->setValue( m_pDisplayedObject->count() );
      m_pCount->setReadOnly( readOnly );
      m_pErrorBound->setValue( m_pDisplayedObject->errorBound() );
      m_pErrorBound->setReadOnly( readOnly );
      m_pGrayThreshold->setValue( m_pDisplayedObject->grayThreshold() );
      m_pGrayThreshold->setReadOnly( readOnly );
      m_pLowErrorFactor->setValue( m_pDisplayedObject->lowErrorFactor() );
      m_pLowErrorFactor->setReadOnly( readOnly );
      m_pMaxSample->setValue( m_pDisplayedObject->maxSample() );
      m_pMaxSample->setReadOnly( readOnly );
      m_pMedia->setChecked( m_pDisplayedObject->media() );
      m_pMedia->setEnabled( !readOnly );
      m_pMinimumReuse->setValue( m_pDisplayedObject->minimumReuse() );
      m_pMinimumReuse->setReadOnly( readOnly );
      m_pNearestCount->setValue( m_pDisplayedObject->nearestCount() );
      m_pNearestCount->setReadOnly( readOnly );
      m_pNormal->setChecked( m_pDisplayedObject->normal() );
      m_pNormal->setEnabled( !readOnly );
      m_pPretraceStart->setValue( m_pDisplayedObject->pretraceStart() );
      m_pPretraceStart->setReadOnly( readOnly );
      m_pPretraceEnd->setValue( m_pDisplayedObject->pretraceEnd() );
      m_pPretraceEnd->setReadOnly( readOnly );
      m_pRecursionLimit->setValue( m_pDisplayedObject->recursionLimit() );
      m_pRecursionLimit->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMRadiosityEdit: Can't display object\n";
}

void PMRadiosityEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setAdcBailout( m_pAdcBailout->value() );
      m_pDisplayedObject->setAlwaysSample( m_pAlwaysSample->isChecked() );
      m_pDisplayedObject->setBrightness( m_pBrightness->value() );
      m_pDisplayedObject->setCount( m_pCount->value() );
      m_pDisplayedObject->setErrorBound( m_pErrorBound->value() );
      m_pDisplayedObject->setGrayThreshold( m_pGrayThreshold->value() );
      m_pDisplayedObject->setLowErrorFactor( m_pLowErrorFactor->value() );
      m_pDisplayedObject->setMaxSample( m_pMaxSample->value() );
      m_pDisplayedObject->setMedia( m_pMedia->isChecked() );
      m_pDisplayedObject->setMinimumReuse( m_pMinimumReuse->value() );
      m_pDisplayedObject->setNearestCount( m_pNearestCount->value() );
      m_pDisplayedObject->setNormal( m_pNormal->isChecked() );
      m_pDisplayedObject->setPretraceStart( m_pPretraceStart->value() );
      m_pDisplayedObject->setPretraceEnd( m_pPretraceEnd->value() );
      m_pDisplayedObject->setRecursionLimit( m_pRecursionLimit->value() );
   }
}

bool PMRadiosityEdit::isDataValid()
{
   if( !m_pAdcBailout->isDataValid() ) return false;
   if( !m_pBrightness->isDataValid() ) return false;
   if( !m_pCount->isDataValid() ) return false;
   if( !m_pErrorBound->isDataValid() ) return false;
   if( !m_pGrayThreshold->isDataValid() ) return false;
   if( !m_pLowErrorFactor->isDataValid() ) return false;
   if( !m_pMaxSample->isDataValid() ) return false;
   if( !m_pMinimumReuse->isDataValid() ) return false;
   if( !m_pNearestCount->isDataValid() ) return false;
   if( !m_pPretraceStart->isDataValid() ) return false;
   if( !m_pPretraceEnd->isDataValid() ) return false;
   if( !m_pRecursionLimit->isDataValid() ) return false;

   return Base::isDataValid();
}

#include "pmradiosityedit.moc"
