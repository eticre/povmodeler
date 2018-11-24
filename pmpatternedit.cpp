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


#include "pmpatternedit.h"
#include "pmpattern.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmvector.h"

#include <qwidget.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QFileDialog>

PMPatternEdit::PMPatternEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMPatternEdit::createTopWidgets()
{
   Base::createTopWidgets();

   QGridLayout* layout;
   QHBoxLayout* hl;
   QVBoxLayout* vl;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();
   layout = new QGridLayout();
   tl->addLayout( layout );
   hl = new QHBoxLayout();
//TODO PORT QT5    hl->setSpacing( QDialog::spacingHint() );
   hl->addWidget( new QLabel( tr( "Type:" ), this ) );
   m_pTypeCombo = new QComboBox( this );
   m_pTypeCombo->addItem( tr( "Agate" ) );
   m_pTypeCombo->addItem( tr( "Average" ) );
   m_pTypeCombo->addItem( tr( "Boxed" ) );
   m_pTypeCombo->addItem( tr( "Bozo" ) );
   m_pTypeCombo->addItem( tr( "Bumps" ) );
   m_pTypeCombo->addItem( tr( "Cells" ) );
   m_pTypeCombo->addItem( tr( "Crackle" ) );
   m_pTypeCombo->addItem( tr( "Cylindrical" ) );
   m_pTypeCombo->addItem( tr( "Density File" ) );
   m_pTypeCombo->addItem( tr( "Dents" ) );
   m_pTypeCombo->addItem( tr( "Gradient" ) );
   m_pTypeCombo->addItem( tr( "Granite" ) );
   m_pTypeCombo->addItem( tr( "Julia" ) );
   m_pTypeCombo->addItem( tr( "Leopard" ) );
   m_pTypeCombo->addItem( tr( "Mandel" ) );
   m_pTypeCombo->addItem( tr( "Marble" ) );
   m_pTypeCombo->addItem( tr( "Onion" ) );
   m_pTypeCombo->addItem( tr( "Planar" ) );
   m_pTypeCombo->addItem( tr( "Quilt" ) );
   m_pTypeCombo->addItem( tr( "Radial" ) );
   m_pTypeCombo->addItem( tr( "Ripples" ) );
   m_pTypeCombo->addItem( tr( "Slope" ) );
   m_pTypeCombo->addItem( tr( "Spherical" ) );
   m_pTypeCombo->addItem( tr( "Spiral1" ) );
   m_pTypeCombo->addItem( tr( "Spiral2" ) );
   m_pTypeCombo->addItem( tr( "Spotted" ) );
   m_pTypeCombo->addItem( tr( "Waves" ) );
   m_pTypeCombo->addItem( tr( "Wood" ) );
   m_pTypeCombo->addItem( tr( "Wrinkles" ) );
   m_pTypeCombo->addItem( tr( "Tiling" ) );//eticre-tiling-29

   if( this->part()->activeObject()->parent()->type() == "Normal" )
      m_pTypeCombo->addItem( tr( "Pigment" ) );//eticre-pigment-30

   if( this->part()->activeObject()->parent()->type() == "Density" )
      m_pTypeCombo->addItem( tr( "Pigment" ) );//eticre-pigment-30

   if( this->part()->activeObject()->parent()->type() == "Pigment" )
      m_pTypeCombo->addItem( tr( "Pigment" ) );

   if( this->part()->activeObject()->parent()->type() == "Texture" )
      m_pTypeCombo->addItem( tr( "Pigment" ) );


   hl->addWidget( m_pTypeCombo );
   hl->addStretch( 1 );
   layout->addLayout( hl, 0, 0, 1, 2 );

   m_pAgateTurbulenceLabel = new QLabel( tr( "Turbulence:" ), this );
   layout->addWidget( m_pAgateTurbulenceLabel, 1, 0 );
   m_pAgateTurbulenceEdit = new PMFloatEdit( this );
   layout->addWidget( m_pAgateTurbulenceEdit, 1, 1, Qt::AlignLeft );

   m_pCrackleWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pCrackleWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Form:" ), m_pCrackleWidget ) );

   m_pCrackleForm = new PMVectorEdit( "x", "y", "z", m_pCrackleWidget );
   hl->addWidget( m_pCrackleForm );
   hl->addStretch( 1 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Metric:" ), m_pCrackleWidget ), 0, 0 );
   m_pCrackleMetric = new PMIntEdit( m_pCrackleWidget );
   m_pCrackleMetric->setValidation( true, 1, false, 0 );
   gl->addWidget( m_pCrackleMetric, 0, 1 );
   gl->addWidget( new QLabel( tr( "Offset:" ), m_pCrackleWidget ), 1, 0 );
   m_pCrackleOffset = new PMFloatEdit( m_pCrackleWidget );
   gl->addWidget( m_pCrackleOffset, 1, 1 );
   m_pCrackleSolid = new QCheckBox( tr( "Solid:" ), m_pCrackleWidget );
   gl->addWidget( m_pCrackleSolid, 2, 0, 1, 2 );
   hl->addStretch( 1 );
   layout->addWidget( m_pCrackleWidget, 2, 0, 1, 2 );

   m_pDensityWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pDensityWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "File:" ), m_pDensityWidget ) );
   m_pDensityFile = new QLineEdit( m_pDensityWidget );
   hl->addWidget( m_pDensityFile, 1 );
   m_pDensityFileBrowse = new QPushButton( m_pDensityWidget );
   m_pDensityFileBrowse->setIcon( QIcon::fromTheme( "document-open" ) );
   hl->addWidget( m_pDensityFileBrowse );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Interpolation:" ), m_pDensityWidget ) );
   m_pDensityInterpolate = new QComboBox( m_pDensityWidget );
   m_pDensityInterpolate->addItem( tr( "None" ) );
   m_pDensityInterpolate->addItem( tr( "Trilinear" ) );
   hl->addWidget( m_pDensityInterpolate );
   hl->addStretch( 1 );
   layout->addWidget( m_pDensityWidget, 3, 0, 1, 2 );

   m_pGradientLabel = new QLabel( tr( "Gradient:" ), this );
   layout->addWidget( m_pGradientLabel, 4, 0 );
   m_pGradientEdit = new PMVectorEdit( "x", "y", "z", this );
   layout->addWidget( m_pGradientEdit, 4, 1 );

   m_pJuliaComplexLabel = new QLabel( tr( "Complex number:" ), this );
   layout->addWidget( m_pJuliaComplexLabel, 5, 0 );
   m_pJuliaComplex = new PMVectorEdit( "Real", "Imaginary", this );
   layout->addWidget( m_pJuliaComplex, 5, 1 );

   m_pFractalWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pFractalWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   m_pFractalMagnet = new QCheckBox( tr( "Magnet" ), m_pFractalWidget );
   hl->addWidget( m_pFractalMagnet );
   m_pFractalMagnetType = new QComboBox( m_pFractalWidget );
   m_pFractalMagnetType->addItem( tr( "Type 1" ) );
   m_pFractalMagnetType->addItem( tr( "Type 2" ) );
   hl->addWidget( m_pFractalMagnetType );
   hl->addStretch( 1 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Maximum iterations:" ), m_pFractalWidget ), 0, 0 );
   m_pMaxIterationsEdit = new PMIntEdit( m_pFractalWidget );
   m_pMaxIterationsEdit->setValidation( true, 1, false, 0 );
   gl->addWidget( m_pMaxIterationsEdit, 0, 1 );
   m_pFractalExponentLabel = new QLabel( tr( "Exponent:" ), m_pFractalWidget );
   gl->addWidget( m_pFractalExponentLabel, 1, 0 );
   m_pFractalExponent = new PMIntEdit( m_pFractalWidget );
   m_pFractalExponent->setValidation( true, 2, true, 33 );
   gl->addWidget( m_pFractalExponent, 1, 1 );
   hl->addStretch( 1 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Exterior type:" ), m_pFractalWidget ), 0, 0 );
   m_pFractalExtType = new QComboBox( m_pFractalWidget );
   m_pFractalExtType->addItem( tr( "0: Returns Just 1" ) );
   m_pFractalExtType->addItem( tr( "1: Iterations Until Bailout" ) );
   m_pFractalExtType->addItem( tr( "2: Real Part" ) );
   m_pFractalExtType->addItem( tr( "3: Imaginary Part" ) );
   m_pFractalExtType->addItem( tr( "4: Squared Real Part" ) );
   m_pFractalExtType->addItem( tr( "5: Squared Imaginary Part" ) );
   m_pFractalExtType->addItem( tr( "6: Absolute Value" ) );
   gl->addWidget( m_pFractalExtType, 0, 1 );
   gl->addWidget( new QLabel( tr( "Factor:" ), m_pFractalWidget ), 0, 2 );
   m_pFractalExtFactor = new PMFloatEdit( m_pFractalWidget );
   gl->addWidget( m_pFractalExtFactor, 0, 3 );
   gl->addWidget( new QLabel( tr( "Interior type:" ), m_pFractalWidget ), 1, 0 );
   m_pFractalIntType = new QComboBox( m_pFractalWidget );
   m_pFractalIntType->addItem( tr( "0: Returns Just 1" ) );
   m_pFractalIntType->addItem( tr( "1: Absolute Value Smallest" ) );
   m_pFractalIntType->addItem( tr( "2: Real Part" ) );
   m_pFractalIntType->addItem( tr( "3: Imaginary Part" ) );
   m_pFractalIntType->addItem( tr( "4: Squared Real Part" ) );
   m_pFractalIntType->addItem( tr( "5: Squared Imaginary Part" ) );
   m_pFractalIntType->addItem( tr( "6: Absolute Value Last" ) );
   gl->addWidget( m_pFractalIntType, 1, 1 );
   gl->addWidget( new QLabel( tr( "Factor:" ), m_pFractalWidget ), 1, 2 );
   m_pFractalIntFactor = new PMFloatEdit( m_pFractalWidget );
   gl->addWidget( m_pFractalIntFactor, 1, 3 );
   hl->addStretch( 1 );
   layout->addWidget( m_pFractalWidget, 6, 0, 1, 2 );

   m_pQuiltControlsLabel = new QLabel( tr( "Quilt controls:" ), this );
   m_pQuiltControl0Edit = new PMFloatEdit( this );
   m_pQuiltControl1Edit = new PMFloatEdit( this );
   hl = new QHBoxLayout();
   hl->addWidget( m_pQuiltControl0Edit );
   hl->addWidget( m_pQuiltControl1Edit );
   hl->addStretch( 1 );
   layout->addWidget( m_pQuiltControlsLabel, 7, 0 );
   layout->addLayout( hl, 7, 1 );

   m_pSlopeWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pSlopeWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   m_pPointAt = new PMVectorEdit( "x", "y", "z", m_pSlopeWidget );
   m_pPointAtFlag = new QCheckBox( tr( "PointAt" ), m_pSlopeWidget );
   hl->addWidget( m_pPointAtFlag );
   hl->addWidget( m_pPointAt );

   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Direction:" ), m_pSlopeWidget ) );
   m_pSlopeDirection = new PMVectorEdit( "x", "y", "z", m_pSlopeWidget );
   hl->addWidget( m_pSlopeDirection );
   hl = new QHBoxLayout();
   //vl->addLayout( hl );

   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Low slope:" ), m_pSlopeWidget ), 0, 0 );
   m_pSlopeLoSlope = new PMFloatEdit( m_pSlopeWidget );
   m_pSlopeLoSlope->setValidation( true, 0.0, true, 1.0 );
   gl->addWidget( m_pSlopeLoSlope, 0, 1 );
   gl->addWidget( new QLabel( tr( "High slope:" ), m_pSlopeWidget ), 1, 0 );
   m_pSlopeHiSlope = new PMFloatEdit( m_pSlopeWidget );
   m_pSlopeHiSlope->setValidation( true, 0.0, true, 1.0 );
   gl->addWidget( m_pSlopeHiSlope, 1, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   vl->addLayout( hl );
   m_pSlopeAltFlag = new QCheckBox( tr( "Altitiude" ), m_pSlopeWidget );
   hl->addWidget( m_pSlopeAltFlag );
   m_pSlopeAltitude = new PMVectorEdit( "x", "y", "z", m_pSlopeWidget );
   hl->addWidget( m_pSlopeAltitude );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pSlopeLoAltLabel = new QLabel( tr( "Low altitude:" ), m_pSlopeWidget );
   gl->addWidget( m_pSlopeLoAltLabel, 0, 0 );
   m_pSlopeLoAlt = new PMFloatEdit( m_pSlopeWidget );
   gl->addWidget( m_pSlopeLoAlt, 0, 1 );
   m_pSlopeHiAltLabel = new QLabel( tr( "High altitude:" ), m_pSlopeWidget );
   gl->addWidget( m_pSlopeHiAltLabel, 1, 0 );
   m_pSlopeHiAlt = new PMFloatEdit( m_pSlopeWidget );
   gl->addWidget( m_pSlopeHiAlt, 1, 1 );
   hl->addStretch( 1 );
   layout->addWidget( m_pSlopeWidget, 8, 0, 1, 2 );

   m_pSpiralNumberLabel = new QLabel( tr( "Spiral number:" ), this );
   m_pSpiralNumberEdit = new PMIntEdit( this );
   layout->addWidget( m_pSpiralNumberLabel, 9, 0 );
   layout->addWidget( m_pSpiralNumberEdit, 9, 1, Qt::AlignLeft );

   m_pTilingNumberLabel = new QLabel( tr( "Tiling:" ), this );
   m_pTilingNumberEdit = new PMIntEdit( this );
   layout->addWidget( m_pTilingNumberLabel, 9, 0 );
   layout->addWidget( m_pTilingNumberEdit, 9, 1, Qt::AlignLeft );

   m_pPigmentPatternEdit = new PMLinkEdit ( this );
   layout->addWidget( m_pPigmentPatternEdit, 9, 1, Qt::AlignLeft );
   m_pPigmentPatternEdit->setLinkPossibility( "Pigment" );

   m_pDepthLabel = new QLabel( tr( "Depth:" ), this );
   m_pDepthEdit = new PMFloatEdit( this );
   layout->addWidget( m_pDepthLabel, 10, 0 );
   layout->addWidget( m_pDepthEdit, 10, 1, Qt::AlignLeft );

   m_pNoiseGeneratorLabel = new QLabel( tr( "Noise generator:" ), this );
   m_pNoiseGenerator = new QComboBox( this );
   m_pNoiseGenerator->addItem( tr( "Use Global Setting" ) );
   m_pNoiseGenerator->addItem( tr( "Original" ) );
   m_pNoiseGenerator->addItem( tr( "Range Corrected" ) );
   m_pNoiseGenerator->addItem( tr( "Perlin" ) );
   layout->addWidget( m_pNoiseGeneratorLabel, 11, 0 );
   layout->addWidget( m_pNoiseGenerator, 11, 1 );

   m_pEnableTurbulenceEdit = new QCheckBox( tr( "Turbulence" ), this );
   tl->addWidget( m_pEnableTurbulenceEdit );
   m_pTurbulenceWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pTurbulenceWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget(  new QLabel( tr( "Value:" ), m_pTurbulenceWidget ) );
   m_pValueVectorEdit = new PMVectorEdit( "x", "y", "z", m_pTurbulenceWidget );
   hl->addWidget( m_pValueVectorEdit );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Octaves:" ), m_pTurbulenceWidget ), 0, 0 );
   m_pOctavesEdit = new PMIntEdit( m_pTurbulenceWidget );
   gl->addWidget( m_pOctavesEdit, 0, 1 );
   gl->addWidget( new QLabel( tr( "Omega:" ), m_pTurbulenceWidget ), 1, 0 );
   m_pOmegaEdit = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( m_pOmegaEdit, 1, 1 );
   gl->addWidget( new QLabel( tr( "Lambda:" ), m_pTurbulenceWidget ), 2, 0 );
   m_pLambdaEdit = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( m_pLambdaEdit, 2, 1 );
   hl->addStretch( 1 );
   tl->addWidget( m_pTurbulenceWidget );

   /* connect all signals to slots/signals */
   connect( m_pTypeCombo, SIGNAL( activated( int ) ), SLOT( slotComboChanged( int ) ) );

   connect( m_pAgateTurbulenceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pCrackleForm, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCrackleMetric, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCrackleOffset, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pCrackleSolid, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );

   connect( m_pDensityInterpolate, SIGNAL( activated( int ) ), SIGNAL( dataChanged() ) );
   connect( m_pDensityFile, SIGNAL( textChanged( const QString& ) ), SIGNAL( dataChanged() ) );
   connect( m_pDensityFileBrowse, SIGNAL( clicked() ), SLOT( slotDensityFileBrowseClicked() ) );

   connect( m_pGradientEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pJuliaComplex, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFractalMagnet, SIGNAL( clicked() ), SLOT( slotFractalMagnetClicked() ) );
   connect( m_pFractalMagnetType, SIGNAL( activated( int ) ), SIGNAL( dataChanged() ) );
   connect( m_pMaxIterationsEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFractalExponent, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFractalExtType, SIGNAL( activated( int ) ), SIGNAL( dataChanged() ) );
   connect( m_pFractalExtFactor, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFractalIntType, SIGNAL( activated( int ) ), SIGNAL( dataChanged() ) );
   connect( m_pFractalIntFactor, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pQuiltControl0Edit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pQuiltControl1Edit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pSlopeDirection, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pPointAt, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pPointAtFlag, SIGNAL( clicked() ), SLOT( slotPointAtFlagClicked() ) );
   connect( m_pSlopeLoSlope, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSlopeHiSlope, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSlopeAltFlag, SIGNAL( clicked() ), SLOT( slotSlopeAltFlagClicked() ) );
   connect( m_pSlopeAltitude, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSlopeLoAlt, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSlopeHiAlt, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pSpiralNumberEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pTilingNumberEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pPigmentPatternEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

   connect( m_pNoiseGenerator, SIGNAL( activated( int ) ), SIGNAL( dataChanged() ) );

   connect( m_pEnableTurbulenceEdit, SIGNAL( clicked() ), SLOT( slotTurbulenceClicked() ) );
   connect( m_pValueVectorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOctavesEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOmegaEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pLambdaEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDepthEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMPatternEdit::displayObject( PMObject* o )
{
   bool readOnly;

   if( o->isA( "Pattern" ) )
   {
      m_pDisplayedObject = ( PMPattern* ) o;
      readOnly = m_pDisplayedObject->isReadOnly();

      switch( m_pDisplayedObject->patternType() )
      {
         case PMPattern::PatternAgate:
            setPatternType( 0 );
            break;
         case PMPattern::PatternAverage:
            setPatternType( 1 );
            break;
         case PMPattern::PatternBoxed:
            setPatternType( 2 );
            break;
         case PMPattern::PatternBozo:
            setPatternType( 3 );
            break;
         case PMPattern::PatternBumps:
            setPatternType( 4 );
            break;
         case PMPattern::PatternCells:
            setPatternType( 5 );
            break;
         case PMPattern::PatternCrackle:
            setPatternType( 6 );
            break;
         case PMPattern::PatternCylindrical:
            setPatternType( 7 );
            break;
         case PMPattern::PatternDensity:
            setPatternType( 8 );
            break;
         case PMPattern::PatternDents:
            setPatternType( 9 );
            break;
         case PMPattern::PatternGradient:
            setPatternType( 10 );
            break;
         case PMPattern::PatternGranite:
            setPatternType( 11 );
            break;
         case PMPattern::PatternJulia:
            setPatternType( 12 );
            break;
         case PMPattern::PatternLeopard:
            setPatternType( 13 );
            break;
         case PMPattern::PatternMandel:
            setPatternType( 14 );
            break;
         case PMPattern::PatternMarble:
            setPatternType( 15 );
            break;
         case PMPattern::PatternOnion:
            setPatternType( 16 );
            break;
         case PMPattern::PatternPlanar:
            setPatternType( 17 );
            break;
         case PMPattern::PatternQuilted:
            setPatternType( 18 );
            break;
         case PMPattern::PatternRadial:
            setPatternType( 19 );
            break;
         case PMPattern::PatternRipples:
            setPatternType( 20 );
            break;
         case PMPattern::PatternSlope:
            setPatternType( 21 );
            break;
         case PMPattern::PatternSpherical:
            setPatternType( 22 );
            break;
         case PMPattern::PatternSpiral1:
            setPatternType( 23 );
            break;
         case PMPattern::PatternSpiral2:
            setPatternType( 24 );
            break;
         case PMPattern::PatternSpotted:
            setPatternType( 25 );
            break;
         case PMPattern::PatternWaves:
            setPatternType( 26 );
            break;
         case PMPattern::PatternWood:
            setPatternType( 27 );
            break;
         case PMPattern::PatternWrinkles:
            setPatternType( 28 );
            break;
         case PMPattern::PatternTiling://eticre-tiling-29
            setPatternType( 29 );
            break;
         case PMPattern::PatternPigment://eticre-pigment-30
            m_pPigmentPatternEdit->setReadOnly( o->isReadOnly() );
            setPatternType( 30 );
            break;
      }
      m_pTypeCombo->setEnabled( !readOnly );

      m_pAgateTurbulenceEdit->setValue( m_pDisplayedObject->agateTurbulence() );
      m_pAgateTurbulenceEdit->setReadOnly( readOnly );

      m_pCrackleForm->setVector( m_pDisplayedObject->crackleForm() );
      m_pCrackleForm->setReadOnly( readOnly );
      m_pCrackleMetric->setValue( m_pDisplayedObject->crackleMetric() );
      m_pCrackleMetric->setReadOnly( readOnly );
      m_pCrackleOffset->setValue( m_pDisplayedObject->crackleOffset() );
      m_pCrackleOffset->setReadOnly( readOnly );
      m_pCrackleSolid->setChecked( m_pDisplayedObject->crackleSolid() );
      m_pCrackleSolid->setEnabled( !readOnly );

      m_pDensityFile->setText( m_pDisplayedObject->densityFile() );
      m_pDensityFile->setEnabled( !readOnly );
      m_pDensityInterpolate->setCurrentIndex( m_pDisplayedObject->densityInterpolate() );
      m_pDensityInterpolate->setEnabled( !readOnly );

      m_pGradientEdit->setVector( m_pDisplayedObject->gradient() );
      m_pGradientEdit->setReadOnly( readOnly );

      m_pJuliaComplex->setVector( m_pDisplayedObject->juliaComplex() );
      m_pJuliaComplex->setReadOnly( readOnly );
      m_pFractalMagnet->setChecked( m_pDisplayedObject->fractalMagnet() );
      m_pFractalMagnet->setEnabled( !readOnly );
      m_pFractalMagnetType->setCurrentIndex( m_pDisplayedObject->fractalMagnetType() - 1 );
      m_pFractalMagnetType->setEnabled( !readOnly );
      m_pMaxIterationsEdit->setValue( m_pDisplayedObject->maxIterations() );
      m_pMaxIterationsEdit->setReadOnly( readOnly );
      m_pFractalExponent->setValue( m_pDisplayedObject->fractalExponent() );
      m_pFractalExponent->setReadOnly( readOnly );
      m_pFractalExtType->setCurrentIndex( m_pDisplayedObject->fractalExtType() );
      m_pFractalExtType->setEnabled( !readOnly );
      m_pFractalExtFactor->setValue( m_pDisplayedObject->fractalExtFactor() );
      m_pFractalExtFactor->setReadOnly( readOnly );
      m_pFractalIntType->setCurrentIndex( m_pDisplayedObject->fractalIntType() );
      m_pFractalIntType->setEnabled( !readOnly );
      m_pFractalIntFactor->setValue( m_pDisplayedObject->fractalIntFactor() );
      m_pFractalIntFactor->setReadOnly( readOnly );

      m_pQuiltControl0Edit->setValue( m_pDisplayedObject->quiltControl0() );
      m_pQuiltControl0Edit->setReadOnly( readOnly );
      m_pQuiltControl1Edit->setValue( m_pDisplayedObject->quiltControl1() );
      m_pQuiltControl1Edit->setReadOnly( readOnly );

      m_pSlopeDirection->setVector( m_pDisplayedObject->slopeDirection() );
      m_pSlopeDirection->setReadOnly( readOnly );
      m_pPointAt->setVector( m_pDisplayedObject->slopePointAt() );
      m_pPointAt->setReadOnly( readOnly );
      m_pPointAtFlag->setChecked( m_pDisplayedObject->slopePointAtFlag() );
      m_pPointAt->setReadOnly( readOnly );
      m_pSlopeLoSlope->setValue( m_pDisplayedObject->slopeLoSlope() );
      m_pSlopeLoSlope->setReadOnly( readOnly );
      m_pSlopeHiSlope->setValue( m_pDisplayedObject->slopeHiSlope() );
      m_pSlopeHiSlope->setReadOnly( readOnly );
      m_pSlopeAltFlag->setChecked( m_pDisplayedObject->slopeAltFlag() );
      m_pSlopeAltFlag->setEnabled( !readOnly );
      m_pSlopeAltitude->setVector( m_pDisplayedObject->slopeAltitude() );
      m_pSlopeAltitude->setReadOnly( readOnly );
      m_pSlopeLoAlt->setValue( m_pDisplayedObject->slopeLoAltitude() );
      m_pSlopeLoAlt->setReadOnly( readOnly );
      m_pSlopeHiAlt->setValue( m_pDisplayedObject->slopeHiAltitude() );
      m_pSlopeHiAlt->setReadOnly( readOnly );

      m_pSpiralNumberEdit->setValue( m_pDisplayedObject->spiralNumberArms() );
      m_pSpiralNumberEdit->setReadOnly( readOnly );

      m_pTilingNumberEdit->setValue( m_pDisplayedObject->tilingNumbers() );
      m_pTilingNumberEdit->setReadOnly( readOnly );

      m_pNoiseGenerator->setCurrentIndex( m_pDisplayedObject->noiseGenerator() );
      m_pNoiseGenerator->setEnabled( !readOnly );

      m_pEnableTurbulenceEdit->setChecked( m_pDisplayedObject->isTurbulenceEnabled() );
      m_pEnableTurbulenceEdit->setEnabled( !readOnly );
      m_pValueVectorEdit->setVector( m_pDisplayedObject->valueVector() );
      m_pValueVectorEdit->setReadOnly( readOnly );
      m_pOctavesEdit->setValue( m_pDisplayedObject->octaves() );
      m_pOctavesEdit->setReadOnly( readOnly );
      m_pOmegaEdit->setValue( m_pDisplayedObject->omega() );
      m_pOmegaEdit->setReadOnly( readOnly );
      m_pLambdaEdit->setValue( m_pDisplayedObject->lambda() );
      m_pLambdaEdit->setReadOnly( readOnly );

      if( o->parent() && ( o->parent()->type() == "Normal" ) )
      {
         m_pDepthEdit->setValue( m_pDisplayedObject->depth() );
         m_pDepthEdit->setReadOnly( readOnly );
         m_pDepthEdit->show();
         m_pDepthLabel->show();
         emit sizeChanged();
      }
      else
      {
         m_pDepthEdit->hide();
         m_pDepthLabel->hide();
         emit sizeChanged();
      }

      //eticre pigment_pattern normal and density and pigment!!!
      if( m_pDisplayedObject->patternType() == PMPattern::PatternPigment )
      {
          m_pPigmentPatternEdit->show();
          m_pDepthEdit->hide();
          m_pDepthLabel->hide();
      }


      slotFractalMagnetClicked();
      slotSlopeAltFlagClicked();
      slotPointAtFlagClicked();
      slotTurbulenceClicked();
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMPatternEdit: Can't display object\n";
}

void PMPatternEdit::setPatternType( int i )
{
   m_pTypeCombo->setCurrentIndex( i );
   slotComboChanged( i );
}

void PMPatternEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      switch( m_pTypeCombo->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setPatternType( PMPattern::PatternAgate );
            m_pDisplayedObject->setAgateTurbulence( m_pAgateTurbulenceEdit->value() );
            break;
         case 1:
            m_pDisplayedObject->setPatternType( PMPattern::PatternAverage );
            break;
         case 2:
            m_pDisplayedObject->setPatternType( PMPattern::PatternBoxed );
            break;
         case 3:
            m_pDisplayedObject->setPatternType( PMPattern::PatternBozo );
            m_pDisplayedObject->setNoiseGenerator(
               ( PMPattern::PMNoiseType ) m_pNoiseGenerator->currentIndex() );
            break;
         case 4:
            m_pDisplayedObject->setPatternType( PMPattern::PatternBumps );
            m_pDisplayedObject->setNoiseGenerator(
               ( PMPattern::PMNoiseType ) m_pNoiseGenerator->currentIndex() );
            break;
         case 5:
            m_pDisplayedObject->setPatternType( PMPattern::PatternCells );
            break;
         case 6:
            m_pDisplayedObject->setPatternType( PMPattern::PatternCrackle );
            m_pDisplayedObject->setCrackleForm( m_pCrackleForm->vector() );
            m_pDisplayedObject->setCrackleMetric( m_pCrackleMetric->value() );
            m_pDisplayedObject->setCrackleOffset( m_pCrackleOffset->value() );
            m_pDisplayedObject->setCrackleSolid( m_pCrackleSolid->isChecked() );
            break;
         case 7:
            m_pDisplayedObject->setPatternType( PMPattern::PatternCylindrical );
            break;
         case 8:
            m_pDisplayedObject->setPatternType( PMPattern::PatternDensity );
            m_pDisplayedObject->setDensityFile( m_pDensityFile->text() );
            m_pDisplayedObject->setDensityInterpolate( m_pDensityInterpolate->currentIndex() );
            break;
         case 9:
            m_pDisplayedObject->setPatternType( PMPattern::PatternDents );
            break;
         case 10:
            m_pDisplayedObject->setPatternType( PMPattern::PatternGradient );
            m_pDisplayedObject->setGradient( m_pGradientEdit->vector() );
            break;
         case 11:
            m_pDisplayedObject->setPatternType( PMPattern::PatternGranite );
            m_pDisplayedObject->setNoiseGenerator(
               ( PMPattern::PMNoiseType ) m_pNoiseGenerator->currentIndex() );
            break;
         case 12:
            m_pDisplayedObject->setPatternType( PMPattern::PatternJulia );
            m_pDisplayedObject->setJuliaComplex( m_pJuliaComplex->vector() );
            m_pDisplayedObject->setFractalMagnet( m_pFractalMagnet->isChecked() );
            m_pDisplayedObject->setFractalMagnetType( m_pFractalMagnetType->currentIndex() + 1 );
            m_pDisplayedObject->setMaxIterations( m_pMaxIterationsEdit->value() );
            m_pDisplayedObject->setFractalExponent( m_pFractalExponent->value() );
            m_pDisplayedObject->setFractalExtType( m_pFractalExtType->currentIndex() );
            m_pDisplayedObject->setFractalExtFactor( m_pFractalExtFactor->value() );
            m_pDisplayedObject->setFractalIntType( m_pFractalIntType->currentIndex() );
            m_pDisplayedObject->setFractalIntFactor( m_pFractalIntFactor->value() );
            break;
         case 13:
            m_pDisplayedObject->setPatternType( PMPattern::PatternLeopard );
            break;
         case 14:
            m_pDisplayedObject->setPatternType( PMPattern::PatternMandel );
            m_pDisplayedObject->setFractalMagnet( m_pFractalMagnet->isChecked() );
            m_pDisplayedObject->setFractalMagnetType( m_pFractalMagnetType->currentIndex() + 1 );
            m_pDisplayedObject->setMaxIterations( m_pMaxIterationsEdit->value() );
            m_pDisplayedObject->setFractalExponent( m_pFractalExponent->value() );
            m_pDisplayedObject->setFractalExtType( m_pFractalExtType->currentIndex() );
            m_pDisplayedObject->setFractalExtFactor( m_pFractalExtFactor->value() );
            m_pDisplayedObject->setFractalIntType( m_pFractalIntType->currentIndex() );
            m_pDisplayedObject->setFractalIntFactor( m_pFractalIntFactor->value() );
            break;
         case 15:
            m_pDisplayedObject->setPatternType( PMPattern::PatternMarble );
            break;
         case 16:
            m_pDisplayedObject->setPatternType( PMPattern::PatternOnion );
            break;
         case 17:
            m_pDisplayedObject->setPatternType( PMPattern::PatternPlanar );
            break;
         case 18:
            m_pDisplayedObject->setPatternType( PMPattern::PatternQuilted );
            m_pDisplayedObject->setQuiltControl0( m_pQuiltControl0Edit->value() );
            m_pDisplayedObject->setQuiltControl1( m_pQuiltControl1Edit->value() );
            break;
         case 19:
            m_pDisplayedObject->setPatternType( PMPattern::PatternRadial );
            break;
         case 20:
            m_pDisplayedObject->setPatternType( PMPattern::PatternRipples );
            break;
         case 21:
            m_pDisplayedObject->setPatternType( PMPattern::PatternSlope );
            m_pDisplayedObject->setSlopeDirection( m_pSlopeDirection->vector() );
            m_pDisplayedObject->setSlopePointAt( m_pPointAt->vector() );
            m_pDisplayedObject->setSlopePointAtFlag( m_pPointAtFlag->isChecked() );
            m_pDisplayedObject->setSlopeLoSlope( m_pSlopeLoSlope->value() );
            m_pDisplayedObject->setSlopeHiSlope( m_pSlopeHiSlope->value() );
            m_pDisplayedObject->setSlopeAltFlag( m_pSlopeAltFlag->isChecked() );
            m_pDisplayedObject->setSlopeAltitude( m_pSlopeAltitude->vector() );
            m_pDisplayedObject->setSlopeLoAlt( m_pSlopeLoAlt->value() );
            m_pDisplayedObject->setSlopeHiAlt( m_pSlopeHiAlt->value() );
            break;
         case 22:
            m_pDisplayedObject->setPatternType( PMPattern::PatternSpherical );
            break;
         case 23:
            m_pDisplayedObject->setPatternType( PMPattern::PatternSpiral1 );
            m_pDisplayedObject->setSpiralNumberArms( m_pSpiralNumberEdit->value() );
            break;
         case 24:
            m_pDisplayedObject->setPatternType( PMPattern::PatternSpiral2 );
            m_pDisplayedObject->setSpiralNumberArms( m_pSpiralNumberEdit->value() );
            break;
         case 25:
            m_pDisplayedObject->setPatternType( PMPattern::PatternSpotted );
            break;
         case 26:
            m_pDisplayedObject->setPatternType( PMPattern::PatternWaves );
            break;
         case 27:
            m_pDisplayedObject->setPatternType( PMPattern::PatternWood );
            break;
         case 28:
            m_pDisplayedObject->setPatternType( PMPattern::PatternWrinkles );
            m_pDisplayedObject->setNoiseGenerator(
               ( PMPattern::PMNoiseType ) m_pNoiseGenerator->currentIndex() );
            break;
          case 29:
            m_pDisplayedObject->setPatternType( PMPattern::PatternTiling );
            m_pDisplayedObject->setTilingNumbers( m_pTilingNumberEdit->value() );//eticre-tiling
            break;
          case 30:
               m_pDisplayedObject->setPatternType( PMPattern::PatternPigment );//eticre pigment_pattern
               m_pDisplayedObject->setPigmentPattern( m_pPigmentPatternEdit->link() );
            break;
      }
      m_pDisplayedObject->enableTurbulence( m_pEnableTurbulenceEdit->isChecked() );
      m_pDisplayedObject->setValueVector( m_pValueVectorEdit->vector() );
      m_pDisplayedObject->setOctaves( m_pOctavesEdit->value() );
      m_pDisplayedObject->setOmega( m_pOmegaEdit->value() );
      m_pDisplayedObject->setLambda( m_pLambdaEdit->value() );

      if( m_pDisplayedObject->parent() &&
          ( m_pDisplayedObject->parent()->type() == "Normal" ) )
         m_pDisplayedObject->setDepth( m_pDepthEdit->value() );
   }
}

bool PMPatternEdit::isDataValid()
{
   switch( m_pTypeCombo->currentIndex() )
   {
      case 0: // Agate
         if( !m_pAgateTurbulenceEdit->isDataValid() )
            return false;
         break;
      case 6: // Crackle
         if ( !m_pCrackleMetric->isDataValid() )
            return false;
         break;
      case 10: // Gradient
         if( !m_pGradientEdit->isDataValid() )
            return false;
         break;
      case 12: // Julia
      case 14: // Mandel
         if ( !m_pMaxIterationsEdit->isDataValid() )
            return false;
         if ( !m_pFractalExponent->isDataValid() )
            return false;
         break;
      case 18: // Quilt
         if( !m_pQuiltControl0Edit->isDataValid() )
            return false;
         if( !m_pQuiltControl1Edit->isDataValid() )
            return false;
         break;
      case 21: // Slope
         if( !m_pSlopeLoSlope->isDataValid() )
            return false;
         if( !m_pSlopeHiSlope->isDataValid() )
            return false;
         break;
      case 23: // Spiral1
      case 24: // Spiral2
         if( !m_pSpiralNumberEdit->isDataValid() )
            return false;
         break;
      case 29: //tiling
         if( !m_pTilingNumberEdit->isDataValid() )
            return false;
         break;
      case 30: //pigment_pattern todo datavalid
         if( m_pDisplayedObject->parent()->type() != "Normal" &&
             m_pDisplayedObject->parent()->type() != "Density" )
            return true;
         break;
      default:
         break;
   }
   if( m_pEnableTurbulenceEdit->isChecked() ) {
      if( !m_pValueVectorEdit->isDataValid() ) return false;
      if( !m_pOctavesEdit->isDataValid() ) return false;
      if( !m_pOmegaEdit->isDataValid() ) return false;
      if( !m_pLambdaEdit->isDataValid() ) return false;
   }
   if( m_pDisplayedObject->parent() &&
       m_pDisplayedObject->parent()->type() == "Normal" &&
       !m_pDepthEdit->isDataValid() )
      return false;

   return Base::isDataValid();
}

void PMPatternEdit::slotComboChanged( int c )
{
   m_pTilingNumberEdit->hide();
   m_pTilingNumberLabel->hide();
   m_pPigmentPatternEdit->hide();
   if( m_pDisplayedObject->parent()->type() == "Normal" && c != 30 )
   {
       m_pDepthEdit->show();
       m_pDepthLabel->show();
   }
   switch( c )
   {
      case 0: /* Agate */
         m_pAgateTurbulenceEdit->show();
         m_pAgateTurbulenceLabel->show();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 3:  /* Bozo */
      case 4:  /* Bumps */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->show();
         m_pNoiseGenerator->show();
         break;
      case 6: /* Crackle */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->show();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 8:  /* Density File */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->show();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 10:  /* Gradient */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->show();
         m_pGradientLabel->show();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 11:  /* Granite */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->show();
         m_pNoiseGenerator->show();
         break;
      case 12: /* Julia */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->show();
         m_pJuliaComplex->show();
         m_pFractalWidget->show();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 14:  /* Mandel */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->show();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 18:  /* Quilted */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->show();
         m_pQuiltControl0Edit->show();
         m_pQuiltControl1Edit->show();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 21:  /* Slope */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->show();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 23:  /* Spiral1 */
      case 24:  /* Spiral2 */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->show();
         m_pSpiralNumberLabel->show();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 28:  /* Wrinkles */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->show();
         m_pNoiseGenerator->show();
         break;
      case 29:  /* tiling */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pTilingNumberEdit->show();
         m_pTilingNumberLabel->show();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         break;
      case 30:  /* pigment */
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         m_pDepthEdit->hide();
         m_pDepthLabel->hide();
         m_pPigmentPatternEdit->show();
         m_pPigmentPatternEdit->setDisplayedObject( m_pDisplayedObject );
       break;
      default:
         m_pAgateTurbulenceEdit->hide();
         m_pAgateTurbulenceLabel->hide();
         m_pCrackleWidget->hide();
         m_pDensityWidget->hide();
         m_pGradientEdit->hide();
         m_pGradientLabel->hide();
         m_pJuliaComplexLabel->hide();
         m_pJuliaComplex->hide();
         m_pFractalWidget->hide();
         m_pQuiltControlsLabel->hide();
         m_pQuiltControl0Edit->hide();
         m_pQuiltControl1Edit->hide();
         m_pSlopeWidget->hide();
         m_pSpiralNumberEdit->hide();
         m_pSpiralNumberLabel->hide();
         m_pTilingNumberEdit->hide();
         m_pTilingNumberLabel->hide();
         m_pNoiseGeneratorLabel->hide();
         m_pNoiseGenerator->hide();
         m_pPigmentPatternEdit->hide();
         break;
   }

       emit dataChanged();
       emit sizeChanged();

}

void PMPatternEdit::slotDensityFileBrowseClicked()
{
   QString str = QFileDialog::getOpenFileName(0, QString(),  QString());

   if( !str.isEmpty() )
   {
      m_pDensityFile->setText( str );
      emit dataChanged();
   }
}

void PMPatternEdit::slotFractalMagnetClicked()
{
   if ( m_pFractalMagnet->isChecked() )
   {
      m_pFractalMagnetType->show();
      m_pFractalExponentLabel->hide();
      m_pFractalExponent->hide();
   }
   else
   {
      m_pFractalMagnetType->hide();
      m_pFractalExponentLabel->show();
      m_pFractalExponent->show();
   }
   m_pFractalWidget->updateGeometry();
   emit dataChanged();
   emit sizeChanged();
}

void PMPatternEdit::slotPointAtFlagClicked()
{
   if ( m_pPointAtFlag->isChecked() )
   {
      m_pPointAt->show();
      m_pSlopeAltFlag->setCheckState( Qt::Unchecked );
      m_pSlopeDirection->hide();
      slotSlopeAltFlagClicked();
   }
   else
   {
       m_pPointAt->hide();
       m_pSlopeDirection->show();
   }
   m_pSlopeWidget->updateGeometry();
   emit dataChanged();
   emit sizeChanged();
}

void PMPatternEdit::slotSlopeAltFlagClicked()
{
   if ( m_pSlopeAltFlag->isChecked() )
   {
      m_pSlopeAltitude->show();
      m_pSlopeLoAltLabel->show();
      m_pSlopeLoAlt->show();
      m_pSlopeHiAltLabel->show();
      m_pSlopeHiAlt->show();
      m_pPointAtFlag->setCheckState( Qt::Unchecked );
      slotPointAtFlagClicked();
   }
   else
   {
      m_pSlopeAltitude->hide();
      m_pSlopeLoAltLabel->hide();
      m_pSlopeLoAlt->hide();
      m_pSlopeHiAltLabel->hide();
      m_pSlopeHiAlt->hide();
   }
   m_pSlopeWidget->updateGeometry();
   emit dataChanged();
   emit sizeChanged();
}

void PMPatternEdit::slotTurbulenceClicked()
{
   if(m_pEnableTurbulenceEdit->isChecked() )
      m_pTurbulenceWidget->show();
   else
      m_pTurbulenceWidget->hide();

   emit dataChanged();
   emit sizeChanged();
}

#include "pmpatternedit.moc"
