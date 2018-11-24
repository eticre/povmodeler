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


#include "pmmediaedit.h"
#include "pmmedia.h"
#include "pmlineedits.h"
#include "pmcoloredit.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include <QMessageBox>


PMMediaEdit::PMMediaEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMMediaEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Method:" ), this ) );
   m_pMethodEdit = new QComboBox( this );
   m_pMethodEdit->addItem( tr( "1 (Monte Carlo)" ) );
   m_pMethodEdit->addItem( tr( "2 (Smooth)" ) );
   m_pMethodEdit->addItem( tr( "3 (Adaptive sampling)" ) );
   hl->addWidget( m_pMethodEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Intervals:" ), this ) );
   m_pIntervalsEdit = new PMIntEdit( this );
   m_pIntervalsEdit->setValidation( true, 1, false, 0 );
   hl->addWidget( m_pIntervalsEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Samples" ), this ) );
   hl->addWidget( new QLabel( tr( "Min:" ), this ) );
   m_pSamplesMinEdit = new PMIntEdit( this );
   m_pSamplesMinEdit->setValidation( true, 1, false, 0 );
   hl->addWidget( m_pSamplesMinEdit );
   m_pSamplesMaxLabel = new QLabel( tr( "Max:" ), this );
   hl->addWidget( m_pSamplesMaxLabel );
   m_pSamplesMaxEdit = new PMIntEdit( this );
   hl->addWidget( m_pSamplesMaxEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Confidence:" ), this ), 0, 0 );
   m_pConfidenceEdit = new PMFloatEdit( this );
   m_pConfidenceEdit->setValidation( true, 0, true, 1 );
   gl->addWidget( m_pConfidenceEdit, 0, 1 );
   gl->addWidget( new QLabel( tr( "Variance:" ), this ), 1, 0 );
   m_pVarianceEdit = new PMFloatEdit( this );
   gl->addWidget( m_pVarianceEdit, 1, 1 );
   gl->addWidget( new QLabel( tr( "Ratio:" ), this ), 2, 0 );
   m_pRatioEdit = new PMFloatEdit( this );
   gl->addWidget( m_pRatioEdit, 2, 1 );
   hl->addStretch( 1 );

   m_pAAWidget = new QWidget( this );
   hl = new QHBoxLayout( m_pAAWidget );
//TODO PORT QT5    hl->setSpacing( QDialog::spacingHint() );
   hl->addWidget( new QLabel( tr( "Anti-aliasing" ), m_pAAWidget ) );
   hl->addWidget( new QLabel( tr( "Level:" ), m_pAAWidget ) );
   m_pAALevelEdit = new PMIntEdit( m_pAAWidget );
   m_pAALevelEdit->setValidation(  true, 1, false, 0 );
   hl->addWidget( m_pAALevelEdit );
   hl->addWidget( new QLabel( tr( "Threshold:" ), m_pAAWidget ) );
   m_pAAThresholdEdit = new PMFloatEdit( m_pAAWidget );
   m_pAAThresholdEdit->setValidation( true, 0.0, false, 0.0 );
   hl->addWidget( m_pAAThresholdEdit );
   hl->addStretch( 1 );
   topLayout()->addWidget( m_pAAWidget );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pEnableAbsorptionEdit = new QCheckBox( tr( "Absorption" ), this );
   gl->addWidget( m_pEnableAbsorptionEdit, 0, 0, 1, 2 );
   m_pAbsorptionEdit = new PMColorEdit( false, this );
   m_pAbsorptionLabel = new QLabel( tr( "Color:" ), this );
   gl->addWidget( m_pAbsorptionLabel, 1, 0, Qt::AlignTop );
   gl->addWidget( m_pAbsorptionEdit, 1, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pEnableEmissionEdit = new QCheckBox( tr( "Emission" ), this );
   gl->addWidget( m_pEnableEmissionEdit, 0, 0, 1, 2 );
   m_pEmissionEdit = new PMColorEdit( false, this );
   m_pEmissionLabel = new QLabel( tr( "Color:" ), this );
   gl->addWidget( m_pEmissionLabel, 1, 0, Qt::AlignTop );
   gl->addWidget( m_pEmissionEdit, 1, 1 );
   hl->addStretch( 1 );

   m_pEnableScatteringEdit = new QCheckBox( tr( "Scattering" ), this );
   topLayout()->addWidget( m_pEnableScatteringEdit );
   m_pScatteringWidget = new QWidget( this );
   QVBoxLayout* vl = new QVBoxLayout( m_pScatteringWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Type:" ), m_pScatteringWidget ) );
   m_pScatteringTypeEdit = new QComboBox( m_pScatteringWidget );
   m_pScatteringTypeEdit->addItem( tr( "Isotropic" ) );
   m_pScatteringTypeEdit->addItem( tr( "Mie Haze" ) );
   m_pScatteringTypeEdit->addItem( tr( "Mie Murky" ) );
   m_pScatteringTypeEdit->addItem( tr( "Rayleigh" ) );
   m_pScatteringTypeEdit->addItem( tr( "Henyey-Greenstein" ) );
   hl->addWidget( m_pScatteringTypeEdit );
   hl->addStretch( 1 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Color:" ), m_pScatteringWidget ), 0, Qt::AlignTop );
   m_pScatteringColorEdit = new PMColorEdit( false, m_pScatteringWidget );
   hl->addWidget( m_pScatteringColorEdit );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pScatteringEccentricityLabel = new QLabel( tr( "Eccentricity:" ), m_pScatteringWidget );
   gl->addWidget( m_pScatteringEccentricityLabel, 0, 0 );
   m_pScatteringEccentricityEdit = new PMFloatEdit( m_pScatteringWidget );
   gl->addWidget( m_pScatteringEccentricityEdit, 0, 1 );
   gl->addWidget( new QLabel( tr( "Extinction:" ), m_pScatteringWidget ), 1, 0 );
   m_pScatteringExtinctionEdit = new PMFloatEdit( m_pScatteringWidget );
   gl->addWidget( m_pScatteringExtinctionEdit, 1, 1 );
   hl->addStretch( 1 );
   tl->addWidget( m_pScatteringWidget );

   connect( m_pMethodEdit, SIGNAL( activated( int ) ), SLOT( slotMethodChanged( int ) ) );
   connect( m_pIntervalsEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSamplesMinEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSamplesMaxEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pConfidenceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pVarianceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRatioEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAALevelEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAAThresholdEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAbsorptionEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEmissionEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnableAbsorptionEdit, SIGNAL( clicked() ), SLOT( slotAbsorptionClicked() ) );
   connect( m_pEnableEmissionEdit, SIGNAL( clicked() ), SLOT( slotEmissionClicked() ) );
   connect( m_pEnableScatteringEdit, SIGNAL( clicked() ), SLOT( slotScatteringClicked() ) );
   connect( m_pScatteringTypeEdit, SIGNAL( activated( int ) ), SLOT( slotScatteringTypeChanged( int ) ) );
   connect( m_pScatteringColorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pScatteringEccentricityEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pScatteringExtinctionEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMMediaEdit::displayObject( PMObject* o )
{
   if( o->isA( "Media" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMMedia* ) o;

      m_pMethodEdit->setCurrentIndex( m_pDisplayedObject->method() - 1 );
      m_pMethodEdit->setEnabled( !readOnly );
      m_pIntervalsEdit->setValue( m_pDisplayedObject->intervals() );
      m_pIntervalsEdit->setReadOnly( readOnly );
      m_pSamplesMinEdit->setValue( m_pDisplayedObject->samplesMin() );
      m_pSamplesMinEdit->setReadOnly( readOnly );
      m_pSamplesMaxEdit->setValue( m_pDisplayedObject->samplesMax() );
      m_pSamplesMaxEdit->setReadOnly( readOnly );
      m_pConfidenceEdit->setValue( m_pDisplayedObject->confidence() );
      m_pConfidenceEdit->setReadOnly( readOnly );
      m_pVarianceEdit->setValue( m_pDisplayedObject->variance() );
      m_pVarianceEdit->setReadOnly( readOnly );
      m_pRatioEdit->setValue( m_pDisplayedObject->ratio() );
      m_pRatioEdit->setReadOnly( readOnly );
      m_pAALevelEdit->setValue( m_pDisplayedObject->aaLevel() );
      m_pAALevelEdit->setReadOnly( readOnly );
      m_pAAThresholdEdit->setValue( m_pDisplayedObject->aaThreshold() );
      m_pAAThresholdEdit->setReadOnly( readOnly );
      m_pAbsorptionEdit->setColor( m_pDisplayedObject->absorption() );
      m_pAbsorptionEdit->setReadOnly( readOnly );
      m_pEmissionEdit->setColor( m_pDisplayedObject->emission() );
      m_pEmissionEdit->setReadOnly( readOnly );
      m_pEnableAbsorptionEdit->setChecked( m_pDisplayedObject->isAbsorptionEnabled() );
      m_pEnableAbsorptionEdit->setEnabled( !readOnly );
      m_pEnableEmissionEdit->setChecked( m_pDisplayedObject->isEmissionEnabled() );
      m_pEnableEmissionEdit->setEnabled( !readOnly );
      m_pEnableScatteringEdit->setChecked( m_pDisplayedObject->isScatteringEnabled() );
      m_pEnableScatteringEdit->setEnabled( !readOnly );
      m_pScatteringTypeEdit->setCurrentIndex( m_pDisplayedObject->scatteringType() - 1 );
      m_pScatteringTypeEdit->setEnabled( !readOnly );
      m_pScatteringColorEdit->setColor( m_pDisplayedObject->scatteringColor() );
      m_pScatteringColorEdit->setReadOnly( readOnly );
      m_pScatteringEccentricityEdit->setValue( m_pDisplayedObject->scatteringEccentricity() );
      m_pScatteringEccentricityEdit->setReadOnly( readOnly );
      m_pScatteringExtinctionEdit->setValue( m_pDisplayedObject->scatteringExtinction() );
      m_pScatteringExtinctionEdit->setReadOnly( readOnly );

      slotMethodChanged( m_pMethodEdit->currentIndex() );
      slotAbsorptionClicked();
      slotEmissionClicked();
      slotScatteringClicked();
      slotScatteringTypeChanged( m_pScatteringTypeEdit->currentIndex() );
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMMediaEdit: Can't display object\n";
}

void PMMediaEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setMethod( m_pMethodEdit->currentIndex() + 1 );
      m_pDisplayedObject->setIntervals( m_pIntervalsEdit->value() );
      m_pDisplayedObject->setSamplesMin( m_pSamplesMinEdit->value() );
      m_pDisplayedObject->setSamplesMax( m_pSamplesMaxEdit->value() );
      m_pDisplayedObject->setConfidence( m_pConfidenceEdit->value() );
      m_pDisplayedObject->setVariance( m_pVarianceEdit->value() );
      m_pDisplayedObject->setRatio( m_pRatioEdit->value() );
      m_pDisplayedObject->setAALevel( m_pAALevelEdit->value() );
      m_pDisplayedObject->setAAThreshold( m_pAAThresholdEdit->value() );
      m_pDisplayedObject->setAbsorption( m_pAbsorptionEdit->color() );
      m_pDisplayedObject->setEmission( m_pEmissionEdit->color() );
      m_pDisplayedObject->setScatteringType( m_pScatteringTypeEdit->currentIndex() + 1 );
      m_pDisplayedObject->setScatteringColor( m_pScatteringColorEdit->color() );
      m_pDisplayedObject->setScatteringEccentricity( m_pScatteringEccentricityEdit->value() );
      m_pDisplayedObject->setScatteringExtinction( m_pScatteringExtinctionEdit->value() );
      m_pDisplayedObject->enableAbsorption( m_pEnableAbsorptionEdit->isChecked() );
      m_pDisplayedObject->enableEmission( m_pEnableEmissionEdit->isChecked() );
      m_pDisplayedObject->enableScattering( m_pEnableScatteringEdit->isChecked() );
   }
}

bool PMMediaEdit::isDataValid()
{
   if( !m_pIntervalsEdit->isDataValid() ) return false;
   if( !m_pSamplesMinEdit->isDataValid() ) return false;
   if( !m_pSamplesMaxEdit->isDataValid() ) return false;
   if( !m_pConfidenceEdit->isDataValid() ) return false;
   if( !m_pVarianceEdit->isDataValid() ) return false;
   if( !m_pRatioEdit->isDataValid() ) return false;
   if( !m_pAALevelEdit->isDataValid() ) return false;
   if( !m_pAAThresholdEdit->isDataValid() ) return false;
   if( !m_pAbsorptionEdit->isDataValid() ) return false;
   if( !m_pEmissionEdit->isDataValid() ) return false;
   if( !m_pScatteringColorEdit->isDataValid() ) return false;
   if( !m_pScatteringEccentricityEdit->isDataValid() ) return false;
   if( !m_pScatteringExtinctionEdit->isDataValid() ) return false;
   if( m_pMethodEdit->currentIndex() < 2 &&
       m_pSamplesMaxEdit->value() < m_pSamplesMinEdit->value() )
   {
      QMessageBox::warning( this, tr( "Maximum number of samples lower than minimum number." ),
                          tr( "Error" ) );
      return false;
   }

   return Base::isDataValid();
}

void PMMediaEdit::slotMethodChanged( int c )
{
   if ( c == 2 )
   {
      m_pAAWidget->show();
      m_pSamplesMaxLabel->hide();
      m_pSamplesMaxEdit->hide();
   }
   else
   {
      m_pAAWidget->hide();
      m_pSamplesMaxLabel->show();
      m_pSamplesMaxEdit->show();
      if ( m_pSamplesMaxEdit->value() < m_pSamplesMinEdit->value() )
         m_pSamplesMaxEdit->setValue( m_pSamplesMinEdit->value() );
   }
   emit dataChanged();
   emit sizeChanged();
}

void PMMediaEdit::slotAbsorptionClicked()
{
   if( m_pEnableAbsorptionEdit->isChecked() )
   {
      m_pAbsorptionEdit->show();
      m_pAbsorptionLabel->show();
   }
   else
   {
      m_pAbsorptionEdit->hide();
      m_pAbsorptionLabel->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

void PMMediaEdit::slotEmissionClicked()
{
   if( m_pEnableEmissionEdit->isChecked() )
   {
      m_pEmissionEdit->show();
      m_pEmissionLabel->show();
   }
   else
   {
      m_pEmissionEdit->hide();
      m_pEmissionLabel->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

void PMMediaEdit::slotScatteringClicked()
{
   if( m_pEnableScatteringEdit->isChecked() )
   {
      m_pScatteringWidget->show();
      if( m_pScatteringTypeEdit->currentIndex() == 4 )
      {
         m_pScatteringEccentricityLabel->show();
         m_pScatteringEccentricityEdit->show();
      }
      else
      {
         m_pScatteringEccentricityLabel->hide();
         m_pScatteringEccentricityEdit->hide();
      }
   }
   else
      m_pScatteringWidget->hide();

   emit dataChanged();
   emit sizeChanged();
}

void PMMediaEdit::slotScatteringTypeChanged( int c )
{
   if( c == 4 )
   {
      m_pScatteringEccentricityLabel->show();
      m_pScatteringEccentricityEdit->show();
   }
   else
   {
      m_pScatteringEccentricityLabel->hide();
      m_pScatteringEccentricityEdit->hide();
   }
   m_pScatteringWidget->updateGeometry();
   //m_pScatteringWidget->adjustSize();  eticre
   emit dataChanged();
   emit sizeChanged();
}

#include "pmmediaedit.moc"
