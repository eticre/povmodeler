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


#include "pmfinishedit.h"
#include "pmfinish.h"
#include "pmlineedits.h"
#include "pmcoloredit.h"

#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QDialog>


PMFinishEdit::PMFinishEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMFinishEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();
   
   hl = new QHBoxLayout();
   tl->addLayout( hl );
   QGridLayout* layout = new QGridLayout();
   hl->addLayout( layout );
   m_pEnableEmissionEdit = new QCheckBox( tr( "Emission color" ), this );
   m_pEmissionColorLabel = new QLabel( tr( "Color:" ), this );
   m_pEmissionColorEdit = new PMColorEdit( true, this );
   layout->addWidget( m_pEnableEmissionEdit, 0, 0, 1, 2 );
   layout->addWidget( m_pEmissionColorLabel, 1, 0, Qt::AlignTop );
   layout->addWidget( m_pEmissionColorEdit, 1, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   layout = new QGridLayout();
   hl->addLayout( layout );
   m_pEnableAmbientEdit = new QCheckBox( tr( "Ambient color" ), this );
   m_pAmbientColorLabel = new QLabel( tr( "Color:" ), this );
   m_pAmbientColorEdit = new PMColorEdit( true, this );
   layout->addWidget( m_pEnableAmbientEdit, 0, 0, 1, 2 );
   layout->addWidget( m_pAmbientColorLabel, 1, 0, Qt::AlignTop );
   layout->addWidget( m_pAmbientColorEdit, 1, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   layout = new QGridLayout();
   hl->addLayout( layout );
   m_pEnableDiffuseEdit = new QCheckBox( tr( "Diffuse:" ), this );
   m_pDiffuseEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableDiffuseEdit, 0, 0 );
   layout->addWidget( m_pDiffuseEdit, 0, 1 );
   m_pEnableBrillianceEdit = new QCheckBox( tr( "Brilliance:" ), this );
   m_pBrillianceEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableBrillianceEdit, 1, 0 );
   layout->addWidget( m_pBrillianceEdit, 1, 1 );
   m_pEnableCrandEdit = new QCheckBox( tr( "Crand:" ), this );
   m_pCrandEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableCrandEdit, 2, 0 );
   layout->addWidget( m_pCrandEdit, 2, 1 );
   m_pConserveEnergyEdit = new QCheckBox(
      tr( "Conserve energy for reflection" ), this );
   layout->addWidget( m_pConserveEnergyEdit, 3, 0, 1, 2 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   layout = new QGridLayout();
   hl->addLayout( layout );
   m_pEnablePhongEdit = new QCheckBox( tr( "Phong:" ), this );
   m_pPhongEdit = new PMFloatEdit( this );
   m_pEnablePhongSizeEdit = new QCheckBox( tr( "Phong size:" ), this );
   m_pPhongSizeEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnablePhongEdit, 0, 0 );
   layout->addWidget( m_pPhongEdit, 0, 1 );
   layout->addWidget( m_pEnablePhongSizeEdit, 1, 0 );
   layout->addWidget( m_pPhongSizeEdit, 1, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   layout = new QGridLayout();
   hl->addLayout( layout );
   m_pEnableSpecularEdit = new QCheckBox( tr( "Specular:" ), this );
   m_pSpecularEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableSpecularEdit, 0, 0 );
   layout->addWidget( m_pSpecularEdit, 0, 1 );
   m_pEnableRoughnessEdit = new QCheckBox( tr( "Roughness:" ), this );
   m_pRoughnessEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableRoughnessEdit, 1, 0 );
   layout->addWidget( m_pRoughnessEdit, 1, 1 );
   m_pEnableMetallicEdit = new QCheckBox( tr( "Metallic:" ), this );
   m_pMetallicEdit = new PMFloatEdit( this );
   layout->addWidget( m_pEnableMetallicEdit, 2, 0 );
   layout->addWidget( m_pMetallicEdit, 2, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   layout = new QGridLayout();
   hl->addLayout( layout );
   m_pIridEdit = new QCheckBox( tr( "Iridiscence" ), this );
   layout->addWidget( m_pIridEdit, 0, 0, 1, 2 );
   m_pIridAmountLabel = new QLabel( tr( "Amount:" ), this );
   m_pIridAmountEdit = new PMFloatEdit( this );
   layout->addWidget( m_pIridAmountLabel, 1, 0 );
   layout->addWidget( m_pIridAmountEdit, 1, 1 );
   m_pIridThicknessLabel = new QLabel( tr( "Thickness:" ), this );
   m_pIridThicknessEdit = new PMFloatEdit( this );
   layout->addWidget( m_pIridThicknessLabel, 2, 0 );
   layout->addWidget( m_pIridThicknessEdit, 2, 1 );
   m_pIridTurbulenceEdit = new PMFloatEdit( this );
   m_pIridTurbulenceLabel = new QLabel( tr( "Turbulence:" ), this );
   layout->addWidget( m_pIridTurbulenceLabel, 3, 0 );
   layout->addWidget( m_pIridTurbulenceEdit, 3, 1 );
   m_pEnableReflectionEdit = new QCheckBox( tr( "Reflection" ), this );
   layout->addWidget( m_pEnableReflectionEdit, 4, 0, 1, 2 );
   hl->addStretch( 1 );

   m_pReflectionWidget = new QWidget( this );
   QVBoxLayout* vl = new QVBoxLayout( m_pReflectionWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   QGridLayout* gl = new QGridLayout();
   vl->addLayout( gl );
   m_pEnableReflectionMinEdit = new QCheckBox( tr( "Minimum:" ),
                                               m_pReflectionWidget );
   m_pReflectionMinColorEdit = new PMColorEdit( false, m_pReflectionWidget );
   gl->addWidget( m_pEnableReflectionMinEdit, 0, 0, Qt::AlignTop );
   gl->addWidget( m_pReflectionMinColorEdit, 0, 1 );
   QLabel* label = new QLabel( tr( "Maximum:" ), m_pReflectionWidget );
   m_pReflectionColorEdit = new PMColorEdit( false, m_pReflectionWidget );
   gl->addWidget( label, 1, 0, Qt::AlignTop );
   gl->addWidget( m_pReflectionColorEdit, 1, 1 );

   gl = new QGridLayout();
   vl->addLayout( gl );
   m_pReflectionFresnelEdit = new QCheckBox( tr( "Fresnel reflectivity" ),
                                             m_pReflectionWidget );
   gl->addWidget( m_pReflectionFresnelEdit, 0, 0, 1, 2 );
   m_pEnableRefFalloffEdit = new QCheckBox( tr( "Falloff:" ),
                                            m_pReflectionWidget );
   m_pReflectionFalloffEdit = new PMFloatEdit( m_pReflectionWidget );
   gl->addWidget( m_pEnableRefFalloffEdit, 1, 0 );
   gl->addWidget( m_pReflectionFalloffEdit, 1, 1 );
   m_pEnableRefExponentEdit = new QCheckBox( tr( "Exponent:" ),
                                             m_pReflectionWidget );
   m_pReflectionExponentEdit = new PMFloatEdit( m_pReflectionWidget );
   gl->addWidget( m_pEnableRefExponentEdit, 2, 0 );
   gl->addWidget( m_pReflectionExponentEdit, 2, 1 );
   m_pEnableRefMetallicEdit = new QCheckBox( tr( "Metallic:" ),
                                             m_pReflectionWidget );
   m_pReflectionMetallicEdit = new PMFloatEdit( m_pReflectionWidget );
   gl->addWidget( m_pEnableRefMetallicEdit, 3, 0 );
   gl->addWidget( m_pReflectionMetallicEdit, 3, 1 );
   vl->addStretch( 1 );
   layout->addWidget( m_pReflectionWidget, 5, 0, 1, 2 );

   connect( m_pEmissionColorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAmbientColorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDiffuseEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pBrillianceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCrandEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pConserveEnergyEdit, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pPhongEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pPhongSizeEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMetallicEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSpecularEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRoughnessEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pIridAmountEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pIridThicknessEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pIridTurbulenceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pIridEdit, SIGNAL( clicked() ), SLOT( slotIridClicked() ) );
   connect( m_pReflectionColorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pReflectionMinColorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pReflectionFresnelEdit, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pReflectionFalloffEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pReflectionExponentEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pReflectionMetallicEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   
   connect( m_pEnableEmissionEdit, SIGNAL( clicked() ), SLOT( slotEmissionClicked() ) );
   connect( m_pEnableAmbientEdit, SIGNAL( clicked() ), SLOT( slotAmbientClicked() ) );
   connect( m_pEnablePhongEdit, SIGNAL( clicked() ), SLOT( slotPhongClicked() ) );
   connect( m_pEnablePhongSizeEdit, SIGNAL( clicked() ), SLOT( slotPhongSizeClicked() ) );
   connect( m_pEnableDiffuseEdit, SIGNAL( clicked() ), SLOT( slotDiffuseClicked() ) );
   connect( m_pEnableBrillianceEdit, SIGNAL( clicked() ), SLOT( slotBrillianceClicked() ) );
   connect( m_pEnableCrandEdit, SIGNAL( clicked() ), SLOT( slotCrandClicked() ) );
   connect( m_pEnableSpecularEdit, SIGNAL( clicked() ), SLOT( slotSpecularClicked() ) );
   connect( m_pEnableRoughnessEdit, SIGNAL( clicked() ), SLOT( slotRoughnessClicked() ) );
   connect( m_pEnableMetallicEdit, SIGNAL( clicked() ), SLOT( slotMetallicClicked() ) );
   connect( m_pEnableReflectionEdit, SIGNAL( clicked() ), SLOT( slotReflectionClicked() ) );
   connect( m_pEnableReflectionMinEdit, SIGNAL( clicked() ), SLOT( slotReflectionMinClicked() ) );
   connect( m_pEnableRefFalloffEdit, SIGNAL( clicked() ), SLOT( slotRefFalloffClicked() ) );
   connect( m_pEnableRefExponentEdit, SIGNAL( clicked() ), SLOT( slotRefExponentClicked() ) );
   connect( m_pEnableRefMetallicEdit, SIGNAL( clicked() ), SLOT( slotRefMetallicClicked() ) );
}

void PMFinishEdit::displayObject( PMObject* o )
{
   if( o->isA( "Finish" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMFinish* ) o;

      m_pEmissionColorEdit->setColor( m_pDisplayedObject->emissionColor() );
      m_pEmissionColorEdit->setReadOnly( readOnly );
      m_pAmbientColorEdit->setColor( m_pDisplayedObject->ambientColor() );
      m_pAmbientColorEdit->setReadOnly( readOnly );
      m_pDiffuseEdit->setValue( m_pDisplayedObject->diffuse() );
      m_pDiffuseEdit->setReadOnly( readOnly );
      m_pBrillianceEdit->setValue( m_pDisplayedObject->brilliance() );
      m_pBrillianceEdit->setReadOnly( readOnly );
      m_pCrandEdit->setValue( m_pDisplayedObject->crand() );
      m_pCrandEdit->setReadOnly( readOnly );
      m_pConserveEnergyEdit->setChecked( m_pDisplayedObject->conserveEnergy() );
      m_pConserveEnergyEdit->setEnabled( !readOnly );
      m_pPhongEdit->setValue( m_pDisplayedObject->phong() );
      m_pPhongEdit->setReadOnly( readOnly );
      m_pPhongSizeEdit->setValue( m_pDisplayedObject->phongSize() );
      m_pPhongSizeEdit->setReadOnly( readOnly );
      m_pMetallicEdit->setValue( m_pDisplayedObject->metallic() );
      m_pMetallicEdit->setReadOnly( readOnly );
      m_pSpecularEdit->setValue( m_pDisplayedObject->specular() );
      m_pSpecularEdit->setReadOnly( readOnly );
      m_pRoughnessEdit->setValue( m_pDisplayedObject->roughness() );
      m_pRoughnessEdit->setReadOnly( readOnly );
      m_pIridEdit->setChecked( m_pDisplayedObject->irid() );
      m_pIridEdit->setEnabled( !readOnly );
      m_pIridAmountEdit->setValue( m_pDisplayedObject->iridAmount() );
      m_pIridAmountEdit->setReadOnly( readOnly );
      m_pIridThicknessEdit->setValue( m_pDisplayedObject->iridThickness() );
      m_pIridThicknessEdit->setReadOnly( readOnly );
      m_pIridTurbulenceEdit->setValue( m_pDisplayedObject->iridTurbulence() );
      m_pIridTurbulenceEdit->setReadOnly( readOnly );
      m_pReflectionColorEdit->setColor( m_pDisplayedObject->reflectionColor() );
      m_pReflectionColorEdit->setReadOnly( readOnly );
      m_pReflectionMinColorEdit->setColor( m_pDisplayedObject->reflectionMinColor() );
      m_pReflectionMinColorEdit->setReadOnly( readOnly );
      m_pReflectionFresnelEdit->setChecked( m_pDisplayedObject->reflectionFresnel() );
      m_pReflectionFresnelEdit->setEnabled( !readOnly );
      m_pReflectionFalloffEdit->setValue( m_pDisplayedObject->reflectionFalloff() );
      m_pReflectionFalloffEdit->setReadOnly( readOnly );
      m_pReflectionExponentEdit->setValue( m_pDisplayedObject->reflectionExponent() );
      m_pReflectionExponentEdit->setReadOnly( readOnly );
      m_pReflectionMetallicEdit->setValue( m_pDisplayedObject->reflectionMetallic() );
      m_pReflectionMetallicEdit->setReadOnly( readOnly );
      m_pEnableEmissionEdit->setChecked( m_pDisplayedObject->isEmissionEnabled() );
      m_pEnableEmissionEdit->setEnabled( !readOnly );
      m_pEnableAmbientEdit->setChecked( m_pDisplayedObject->isAmbientEnabled() );
      m_pEnableAmbientEdit->setEnabled( !readOnly );
      m_pEnablePhongEdit->setChecked( m_pDisplayedObject->isPhongEnabled() );
      m_pEnablePhongEdit->setEnabled( !readOnly );
      m_pEnablePhongSizeEdit->setChecked( m_pDisplayedObject->isPhongSizeEnabled() );
      m_pEnablePhongSizeEdit->setEnabled( !readOnly );
      m_pEnableDiffuseEdit->setChecked( m_pDisplayedObject->isDiffuseEnabled() );
      m_pEnableDiffuseEdit->setEnabled( !readOnly );
      m_pEnableBrillianceEdit->setChecked( m_pDisplayedObject->isBrillianceEnabled() );
      m_pEnableBrillianceEdit->setEnabled( !readOnly );
      m_pEnableCrandEdit->setChecked( m_pDisplayedObject->isCrandEnabled() );
      m_pEnableCrandEdit->setEnabled( !readOnly );
      m_pEnableMetallicEdit->setChecked( m_pDisplayedObject->isMetallicEnabled() );
      m_pEnableMetallicEdit->setEnabled( !readOnly );
      m_pEnableSpecularEdit->setChecked( m_pDisplayedObject->isSpecularEnabled() );
      m_pEnableSpecularEdit->setEnabled( !readOnly );
      m_pEnableRoughnessEdit->setChecked( m_pDisplayedObject->isRoughnessEnabled() );
      m_pEnableRoughnessEdit->setEnabled( !readOnly );
      m_pEnableReflectionEdit->setChecked( m_pDisplayedObject->isReflectionEnabled() );
      m_pEnableReflectionEdit->setEnabled( !readOnly );
      m_pEnableReflectionMinEdit->setChecked( m_pDisplayedObject->isReflectionMinEnabled() );
      m_pEnableReflectionMinEdit->setEnabled( !readOnly );
      m_pEnableRefFalloffEdit->setChecked( m_pDisplayedObject->isRefFalloffEnabled() );
      m_pEnableRefFalloffEdit->setEnabled( !readOnly );
      m_pEnableRefExponentEdit->setChecked( m_pDisplayedObject->isRefExponentEnabled() );
      m_pEnableRefExponentEdit->setEnabled( !readOnly );
      m_pEnableRefMetallicEdit->setChecked( m_pDisplayedObject->isRefMetallicEnabled() );
      m_pEnableRefMetallicEdit->setEnabled( !readOnly );
      slotIridClicked();
      slotEmissionClicked();
      slotAmbientClicked();
      slotPhongClicked();
      slotPhongSizeClicked();
      slotBrillianceClicked();
      slotDiffuseClicked();
      slotMetallicClicked();
      slotCrandClicked();
      slotSpecularClicked();
      slotRoughnessClicked();
      slotReflectionClicked();
      slotReflectionMinClicked();
      slotRefFalloffClicked();
      slotRefExponentClicked();
      slotRefMetallicClicked();
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMFinishEdit: Can't display object\n";
}

void PMFinishEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setEmissionColor( m_pEmissionColorEdit->color() );
      m_pDisplayedObject->setAmbientColor( m_pAmbientColorEdit->color() );
      m_pDisplayedObject->setDiffuse( m_pDiffuseEdit->value() );
      m_pDisplayedObject->setBrilliance( m_pBrillianceEdit->value() );
      m_pDisplayedObject->setCrand( m_pCrandEdit->value() );
      m_pDisplayedObject->setConserveEnergy( m_pConserveEnergyEdit->isChecked() );
      m_pDisplayedObject->setPhong( m_pPhongEdit->value() );
      m_pDisplayedObject->setPhongSize( m_pPhongSizeEdit->value() );
      m_pDisplayedObject->setMetallic( m_pMetallicEdit->value() );
      m_pDisplayedObject->setSpecular( m_pSpecularEdit->value() );
      m_pDisplayedObject->setRoughness( m_pRoughnessEdit->value() );
      m_pDisplayedObject->setIrid( m_pIridEdit->isChecked() );
      m_pDisplayedObject->setIridAmount( m_pIridAmountEdit->value() );
      m_pDisplayedObject->setIridThickness( m_pIridThicknessEdit->value() );
      m_pDisplayedObject->setIridTurbulence( m_pIridTurbulenceEdit->value() );
      m_pDisplayedObject->setReflectionColor( m_pReflectionColorEdit->color() );
      m_pDisplayedObject->setReflectionMinColor( m_pReflectionMinColorEdit->color() );
      m_pDisplayedObject->setReflectionFresnel( m_pReflectionFresnelEdit->isChecked() );
      m_pDisplayedObject->setReflectionFalloff( m_pReflectionFalloffEdit->value() );
      m_pDisplayedObject->setReflectionExponent( m_pReflectionExponentEdit->value() );
      m_pDisplayedObject->setReflectionMetallic( m_pReflectionMetallicEdit->value() );
      m_pDisplayedObject->enableEmission( m_pEnableEmissionEdit->isChecked() );
      m_pDisplayedObject->enableAmbient( m_pEnableAmbientEdit->isChecked() );
      m_pDisplayedObject->enablePhong( m_pEnablePhongEdit->isChecked() );
      m_pDisplayedObject->enablePhongSize( m_pEnablePhongSizeEdit->isChecked() );
      m_pDisplayedObject->enableBrilliance( m_pEnableBrillianceEdit->isChecked() );
      m_pDisplayedObject->enableDiffuse( m_pEnableDiffuseEdit->isChecked() );
      m_pDisplayedObject->enableCrand( m_pEnableCrandEdit->isChecked() );
      m_pDisplayedObject->enableMetallic( m_pEnableMetallicEdit->isChecked() );
      m_pDisplayedObject->enableRoughness( m_pEnableRoughnessEdit->isChecked() );
      m_pDisplayedObject->enableSpecular( m_pEnableSpecularEdit->isChecked() );
      m_pDisplayedObject->enableReflection( m_pEnableReflectionEdit->isChecked() );
      m_pDisplayedObject->enableReflectionMin( m_pEnableReflectionMinEdit->isChecked() );
      m_pDisplayedObject->enableRefFalloff( m_pEnableRefFalloffEdit->isChecked() );
      m_pDisplayedObject->enableRefExponent( m_pEnableRefExponentEdit->isChecked() );
      m_pDisplayedObject->enableRefMetallic( m_pEnableRefMetallicEdit->isChecked() );
   }
}

bool PMFinishEdit::isDataValid()
{
   if( !m_pDiffuseEdit->isDataValid() ) return false;
   if( !m_pBrillianceEdit->isDataValid() ) return false;
   if( !m_pCrandEdit->isDataValid() ) return false;
   if( !m_pPhongEdit->isDataValid() ) return false;
   if( !m_pPhongSizeEdit->isDataValid() ) return false;
   if( !m_pMetallicEdit->isDataValid() ) return false;
   if( !m_pSpecularEdit->isDataValid() ) return false;
   if( !m_pRoughnessEdit->isDataValid() ) return false;
   if( !m_pIridAmountEdit->isDataValid() ) return false;
   if( !m_pIridThicknessEdit->isDataValid() ) return false;
   if( !m_pIridTurbulenceEdit->isDataValid() ) return false;
   if( !m_pReflectionFalloffEdit->isDataValid() ) return false;
   if( !m_pReflectionExponentEdit->isDataValid() ) return false;
   if( !m_pReflectionMetallicEdit->isDataValid() ) return false;
   return Base::isDataValid();
}

void PMFinishEdit::slotIridClicked()
{
   if( m_pIridEdit->isChecked() ) 
   {
      m_pIridAmountLabel->show();
      m_pIridAmountEdit->show();
      m_pIridThicknessLabel->show();
      m_pIridThicknessEdit->show();
      m_pIridTurbulenceEdit->show();
      m_pIridTurbulenceLabel->show();
   } 
   else 
   {
      m_pIridAmountLabel->hide();
      m_pIridAmountEdit->hide();
      m_pIridThicknessLabel->hide();
      m_pIridThicknessEdit->hide();
      m_pIridTurbulenceEdit->hide();
      m_pIridTurbulenceLabel->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

void PMFinishEdit::slotEmissionClicked()
{
   if( m_pEnableEmissionEdit->isChecked() )
   {
      m_pEmissionColorEdit->show();
      m_pEmissionColorLabel->show();
   }
   else
   {
      m_pEmissionColorEdit->hide();
      m_pEmissionColorLabel->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

void PMFinishEdit::slotAmbientClicked()
{
   if( m_pEnableAmbientEdit->isChecked() )
   {
      m_pAmbientColorEdit->show();
      m_pAmbientColorLabel->show();
   }
   else
   {
      m_pAmbientColorEdit->hide();
      m_pAmbientColorLabel->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

void PMFinishEdit::slotPhongClicked()
{
   m_pPhongEdit->setEnabled( m_pEnablePhongEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotPhongSizeClicked()
{
   m_pPhongSizeEdit->setEnabled( m_pEnablePhongSizeEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotBrillianceClicked()
{
   m_pBrillianceEdit->setEnabled( m_pEnableBrillianceEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotDiffuseClicked()
{
   m_pDiffuseEdit->setEnabled( m_pEnableDiffuseEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotMetallicClicked()
{
   m_pMetallicEdit->setEnabled( m_pEnableMetallicEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotCrandClicked()
{
   m_pCrandEdit->setEnabled( m_pEnableCrandEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotSpecularClicked()
{
   m_pSpecularEdit->setEnabled( m_pEnableSpecularEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotRoughnessClicked()
{
   m_pRoughnessEdit->setEnabled( m_pEnableRoughnessEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotReflectionClicked()
{
   if( m_pEnableReflectionEdit->isChecked() )
      m_pReflectionWidget->show();
   else
      m_pReflectionWidget->hide();

   emit dataChanged();
   emit sizeChanged();
}

void PMFinishEdit::slotReflectionMinClicked()
{
   m_pReflectionMinColorEdit->setEnabled( m_pEnableReflectionMinEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotRefFalloffClicked()
{
   m_pReflectionFalloffEdit->setEnabled( m_pEnableRefFalloffEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotRefExponentClicked()
{
   m_pReflectionExponentEdit->setEnabled( m_pEnableRefExponentEdit->isChecked() );
   emit dataChanged();
}

void PMFinishEdit::slotRefMetallicClicked()
{
   m_pReflectionMetallicEdit->setEnabled( m_pEnableRefMetallicEdit->isChecked() );
   emit dataChanged();
}

#include "pmfinishedit.moc"
