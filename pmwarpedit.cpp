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


#include "pmwarpedit.h"
#include "pmwarp.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>



PMWarpEdit::PMWarpEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMWarpEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QVBoxLayout* vl;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   QLabel* label = new QLabel( tr( "Warp type:" ), this );
   m_pWarpTypeEdit = new QComboBox( this );
   m_pWarpTypeEdit->addItem( tr( "Repeat" ) );
   m_pWarpTypeEdit->addItem( tr( "Black Hole" ) );
   m_pWarpTypeEdit->addItem( tr( "Turbulence" ) );
   m_pWarpTypeEdit->addItem( tr( "Cylindrical" ) );
   m_pWarpTypeEdit->addItem( tr( "Spherical" ) );
   m_pWarpTypeEdit->addItem( tr( "Toroidal" ) );
   m_pWarpTypeEdit->addItem( tr( "Planar" ) );
   hl = new QHBoxLayout();
   tl->addItem( hl );
   hl->addWidget( label );
   hl->addWidget( m_pWarpTypeEdit );
   hl->addStretch( 1 );
   
   /* Repeat Warp Objects */
   m_pRepeatWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pRepeatWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   gl = new QGridLayout();
   vl->addLayout( gl );
   m_pDirectionLabel = new QLabel( tr( "Direction:" ), m_pRepeatWidget );
   m_pDirectionEdit = new PMVectorEdit( "x", "y", "z", m_pRepeatWidget );
   gl->addWidget( m_pDirectionLabel, 0, 0 );
   gl->addWidget( m_pDirectionEdit, 0, 1 );
   m_pOffsetLabel = new QLabel( tr( "Offset:" ), m_pRepeatWidget );
   m_pOffsetEdit = new PMVectorEdit( "x", "y", "z", m_pRepeatWidget );
   gl->addWidget( m_pOffsetLabel, 1, 0 );
   gl->addWidget( m_pOffsetEdit, 1, 1 );
   m_pFlipLabel = new QLabel( tr( "Flip:" ), m_pRepeatWidget );
   m_pFlipEdit = new PMVectorEdit( "x", "y", "z", m_pRepeatWidget );
   gl->addWidget( m_pFlipLabel, 2, 0 );
   gl->addWidget( m_pFlipEdit, 2, 1 );
   
   /* Black Hole Warp Objects */
   m_pBlackHoleWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pBlackHoleWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   m_pLocationLabel = new QLabel( tr( "Location:" ), m_pBlackHoleWidget );
   m_pLocationEdit = new PMVectorEdit( "x", "y", "z", m_pBlackHoleWidget );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( m_pLocationLabel );
   hl->addWidget( m_pLocationEdit );
   m_pRadiusLabel = new QLabel( tr( "Radius:" ), m_pBlackHoleWidget );
   m_pRadiusEdit = new PMFloatEdit( m_pBlackHoleWidget );
   m_pStrengthLabel = new QLabel( tr( "Strength:" ), m_pBlackHoleWidget );
   m_pStrengthEdit = new PMFloatEdit( m_pBlackHoleWidget );
   m_pFalloffLabel = new QLabel( tr( "Falloff:" ), m_pBlackHoleWidget );
   m_pFalloffEdit = new PMFloatEdit( m_pBlackHoleWidget );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( m_pRadiusLabel, 0, 0 );
   gl->addWidget( m_pRadiusEdit, 0, 1 );
   gl->addWidget( m_pStrengthLabel, 1, 0 );
   gl->addWidget( m_pStrengthEdit, 1, 1 );
   gl->addWidget( m_pFalloffLabel, 2, 0 );
   gl->addWidget( m_pFalloffEdit, 2, 1 );
   hl->addStretch( 1 );
   
   m_pRepeatLabel = new QLabel( tr( "Repeat:" ), m_pBlackHoleWidget );
   m_pRepeatEdit = new PMVectorEdit( "x", "y", "z", m_pBlackHoleWidget );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( m_pRepeatLabel );
   hl->addWidget( m_pRepeatEdit );
   m_pTurbulenceLabel = new QLabel( tr( "Turbulence:" ), m_pBlackHoleWidget );
   m_pTurbulenceEdit = new PMVectorEdit( "x", "y", "z", m_pBlackHoleWidget );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( m_pTurbulenceLabel );
   hl->addWidget( m_pTurbulenceEdit );
   m_pInverseEdit = new QCheckBox( tr( "Inverse" ), m_pBlackHoleWidget );
   vl->addWidget( m_pInverseEdit );

   /* Turbulence Warp Objects */
   m_pTurbulenceWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pTurbulenceWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   m_pValueVectorLabel = new QLabel( tr( "Value:" ), m_pTurbulenceWidget );
   m_pValueVectorEdit = new PMVectorEdit( "x", "y", "z", m_pTurbulenceWidget );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   hl->addWidget( m_pValueVectorLabel );
   hl->addWidget( m_pValueVectorEdit );

   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pOctavesLabel = new QLabel( tr( "Octaves:" ), m_pTurbulenceWidget );
   m_pOctavesEdit = new PMIntEdit( m_pTurbulenceWidget );
   gl->addWidget( m_pOctavesLabel, 0, 0 );
   gl->addWidget( m_pOctavesEdit, 0, 1 );
   m_pOmegaLabel = new QLabel( tr( "Omega:" ), m_pTurbulenceWidget );
   m_pOmegaEdit = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( m_pOmegaLabel, 1, 0 );
   gl->addWidget( m_pOmegaEdit, 1, 1 );
   m_pLambdaLabel = new QLabel( tr( "Lambda:" ), m_pTurbulenceWidget );
   m_pLambdaEdit = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( m_pLambdaLabel, 2, 0 );
   gl->addWidget( m_pLambdaEdit, 2, 1 );
   hl->addStretch( 1 );

   /* Mapping Warp Objects */
   m_pMappingWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pMappingWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   label = new QLabel( tr( "Orientation:" ), m_pMappingWidget );
   m_pOrientationEdit = new PMVectorEdit( "x", "y", "z", m_pMappingWidget );
   hl = new QHBoxLayout();
   vl->addLayout( hl );//correct 7-04-2012 eticre vl<-->hl
   hl->addWidget( label );
   hl->addWidget( m_pOrientationEdit );

   gl = new QGridLayout();
   vl->addLayout( gl );
   label = new QLabel( tr( "Distance exponent:" ), m_pMappingWidget );
   m_pDistExpEdit = new PMFloatEdit( m_pMappingWidget );
   gl->addWidget( label, 0, 0 );
   gl->addWidget( m_pDistExpEdit, 0, 1 );
   m_pMajorRadiusLabel = new QLabel( tr( "Major radius:" ), m_pMappingWidget );
   m_pMajorRadiusEdit = new PMFloatEdit( m_pMappingWidget );
   gl->addWidget( m_pMajorRadiusLabel, 1, 0 );
   gl->addWidget( m_pMajorRadiusEdit, 1, 1 );

   vl = new QVBoxLayout();
   tl->addLayout( vl );
   vl->addSpacing( 0 );
   vl->addWidget( m_pRepeatWidget );
   vl->addWidget( m_pBlackHoleWidget );
   vl->addWidget( m_pTurbulenceWidget );
   vl->addWidget( m_pMappingWidget );
   
   connect( m_pWarpTypeEdit, SIGNAL( activated( int ) ), SLOT( slotComboChanged( int ) ) );
   connect( m_pDirectionEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOffsetEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFlipEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pLocationEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadiusEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pStrengthEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFalloffEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pInverseEdit, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pRepeatEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pTurbulenceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pValueVectorEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOctavesEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOmegaEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pLambdaEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOrientationEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDistExpEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMajorRadiusEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMWarpEdit::displayObject( PMObject* o )
{
   if( o->isA( "Warp" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMWarp* ) o;

      switch( m_pDisplayedObject->warpType() )
      {
         case PMWarp::Repeat:
            m_pWarpTypeEdit->setCurrentIndex( 0 );
            slotComboChanged( 0 );
            break;
         case PMWarp::BlackHole:
            m_pWarpTypeEdit->setCurrentIndex( 1 );
            slotComboChanged( 1 );
            break;
         case PMWarp::Turbulence:
            m_pWarpTypeEdit->setCurrentIndex( 2 );
            slotComboChanged( 2 );
            break;
         case PMWarp::Cylindrical:
            m_pWarpTypeEdit->setCurrentIndex( 3 );
            slotComboChanged( 3 );
            break;
         case PMWarp::Spherical:
            m_pWarpTypeEdit->setCurrentIndex( 4 );
            slotComboChanged( 4 );
            break;
         case PMWarp::Toroidal:
            m_pWarpTypeEdit->setCurrentIndex( 5 );
            slotComboChanged( 5 );
            break;
         case PMWarp::Planar:
            m_pWarpTypeEdit->setCurrentIndex( 6 );
            slotComboChanged( 6 );
            break;
      }
      m_pDirectionEdit->setVector( m_pDisplayedObject->direction() );
      m_pDirectionEdit->setReadOnly( readOnly );
      m_pOffsetEdit->setVector( m_pDisplayedObject->offset() );
      m_pOffsetEdit->setReadOnly( readOnly );
      m_pFlipEdit->setVector( m_pDisplayedObject->flip() );
      m_pFlipEdit->setReadOnly( readOnly );
      m_pLocationEdit->setVector( m_pDisplayedObject->location() );
      m_pLocationEdit->setReadOnly( readOnly );
      m_pRadiusEdit->setValue( m_pDisplayedObject->radius() );
      m_pRadiusEdit->setReadOnly( readOnly );
      m_pStrengthEdit->setValue( m_pDisplayedObject->strength() );
      m_pStrengthEdit->setReadOnly( readOnly );
      m_pFalloffEdit->setValue( m_pDisplayedObject->falloff() );
      m_pFalloffEdit->setReadOnly( readOnly );
      m_pInverseEdit->setChecked( m_pDisplayedObject->inverse() );
      m_pInverseEdit->setEnabled( !readOnly );
      m_pRepeatEdit->setVector( m_pDisplayedObject->repeat() );
      m_pRepeatEdit->setReadOnly( readOnly );
      m_pTurbulenceEdit->setVector( m_pDisplayedObject->turbulence() );
      m_pTurbulenceEdit->setReadOnly( readOnly );
      m_pValueVectorEdit->setVector( m_pDisplayedObject->valueVector() );
      m_pValueVectorEdit->setReadOnly( readOnly );
      m_pOctavesEdit->setValue( m_pDisplayedObject->octaves() );
      m_pOctavesEdit->setReadOnly( readOnly );
      m_pOmegaEdit->setValue( m_pDisplayedObject->omega() );
      m_pOmegaEdit->setReadOnly( readOnly );
      m_pLambdaEdit->setValue( m_pDisplayedObject->lambda() );
      m_pLambdaEdit->setReadOnly( readOnly );
      m_pOrientationEdit->setVector( m_pDisplayedObject->orientation() );
      m_pOrientationEdit->setReadOnly( readOnly );
      m_pDistExpEdit->setValue( m_pDisplayedObject->distExp() );
      m_pDistExpEdit->setReadOnly( readOnly );
      m_pMajorRadiusEdit->setValue( m_pDisplayedObject->majorRadius() );
      m_pMajorRadiusEdit->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMWarpEdit: Can't display object\n";
}

void PMWarpEdit::saveContents()
{
   bool mapping = false;

   if( m_pDisplayedObject )
   {
      Base::saveContents();
      switch( m_pWarpTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setWarpType( PMWarp::Repeat );
            m_pDisplayedObject->setDirection( m_pDirectionEdit->vector() );
            m_pDisplayedObject->setOffset( m_pOffsetEdit->vector() );
            m_pDisplayedObject->setFlip( m_pOffsetEdit->vector() );
            break;
         case 1:
            m_pDisplayedObject->setWarpType( PMWarp::BlackHole );
            m_pDisplayedObject->setLocation( m_pLocationEdit->vector() );
            m_pDisplayedObject->setRadius( m_pRadiusEdit->value() );
            m_pDisplayedObject->setStrength( m_pStrengthEdit->value() );
            m_pDisplayedObject->setFalloff( m_pFalloffEdit->value() );
            m_pDisplayedObject->setInverse( m_pInverseEdit->isChecked() );
            m_pDisplayedObject->setRepeat( m_pRepeatEdit->vector() );
            m_pDisplayedObject->setTurbulence( m_pTurbulenceEdit->vector() );
            break;
         case 2:
            m_pDisplayedObject->setWarpType( PMWarp::Turbulence );
            m_pDisplayedObject->setValueVector( m_pValueVectorEdit->vector() );
            m_pDisplayedObject->setOctaves( m_pOctavesEdit->value() );
            m_pDisplayedObject->setOmega( m_pOmegaEdit->value() );
            m_pDisplayedObject->setLambda( m_pLambdaEdit->value() );
            break;
         case 3:
            m_pDisplayedObject->setWarpType( PMWarp::Cylindrical );
            mapping = true;
            break;
         case 4:
            m_pDisplayedObject->setWarpType( PMWarp::Spherical );
            mapping = true;
            break;
         case 5:
            m_pDisplayedObject->setWarpType( PMWarp::Toroidal );
            m_pDisplayedObject->setMajorRadius( m_pMajorRadiusEdit->value() );
            mapping = true;
            break;
         case 6:
            m_pDisplayedObject->setWarpType( PMWarp::Planar );
            mapping = true;
            break;
      }

      if( mapping )
      {
         m_pDisplayedObject->setOrientation( m_pOrientationEdit->vector() );
         m_pDisplayedObject->setDistExp( m_pDistExpEdit->value() );
      }
   }
}

bool PMWarpEdit::isDataValid()
{
   double x,y,z;

   switch( m_pWarpTypeEdit->currentIndex() )
   {
      case 0:
         if( !m_pDirectionEdit->isDataValid() ||
             !m_pOffsetEdit->isDataValid() ||
             !m_pFlipEdit->isDataValid() )
            return false;
         //  The direction vector can only have one non-zero component
         x = m_pDirectionEdit->vector().x();
         y = m_pDirectionEdit->vector().y();
         z = m_pDirectionEdit->vector().z();
         if( ( x && ( y || z ) ) || ( y && ( x || z  )) || ( z && ( x || y ) ) )
            return false;
         break;
      case 1:
         break;
      case 2:
         if( !m_pOctavesEdit->isDataValid() ||
              m_pOctavesEdit->value() < 1 ||
              m_pOctavesEdit->value() > 10 )
            return false;
         break;
      case 3:
      case 4:
      case 5:
      case 6:
         if( !m_pOrientationEdit->isDataValid() ||
             !m_pDistExpEdit->isDataValid() ||
             !m_pMajorRadiusEdit->isDataValid() )
            return false;
         break;
   }
   return Base::isDataValid();
}

void PMWarpEdit::slotComboChanged( int c )
{
   switch ( c )
   {
      case 0:
         m_pRepeatWidget->show();
         m_pBlackHoleWidget->hide();
         m_pTurbulenceWidget->hide();
         m_pMappingWidget->hide();
         break;
      case 1:
         m_pRepeatWidget->hide();
         m_pBlackHoleWidget->show();
         m_pTurbulenceWidget->hide();
         m_pMappingWidget->hide();
         break;
      case 2:
         m_pRepeatWidget->hide();
         m_pBlackHoleWidget->hide();
         m_pTurbulenceWidget->show();
         m_pMappingWidget->hide();
         break;
      case 3:
      case 4:
      case 6:
         m_pRepeatWidget->hide();
         m_pBlackHoleWidget->hide();
         m_pTurbulenceWidget->hide();
         m_pMappingWidget->show();
         m_pMajorRadiusLabel->hide();
         m_pMajorRadiusEdit->hide();
         break;
      case 5:
         m_pRepeatWidget->hide();
         m_pBlackHoleWidget->hide();
         m_pTurbulenceWidget->hide();
         m_pMappingWidget->show();
         m_pMajorRadiusLabel->show();
         m_pMajorRadiusEdit->show();
         break;
   }
   emit dataChanged();
   emit sizeChanged();
}

#include "pmwarpedit.moc"
