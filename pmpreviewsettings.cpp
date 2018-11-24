/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmpreviewsettings.h"

#include "pmlineedits.h"
#include "pmdialogeditbase.h"
#include "pmdefaults.h"

#include <QLayout>
#include <QGroupBox>
#include <QCheckBox>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

PMPreviewSettings::PMPreviewSettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QHBoxLayout* hlayout;
   QVBoxLayout* vlayout;
   QVBoxLayout* gvl;
   QGridLayout* grid;
   QGroupBox* gb;

   vlayout = new QVBoxLayout( this );
   vlayout->setMargin( 0 );
   
   hlayout = new QHBoxLayout();
   vlayout->addLayout( hlayout );
   grid = new QGridLayout();
   vlayout->addLayout( grid );
   grid->addWidget( new QLabel( "Size:", this ), 0, 0 );
   m_pPreviewSize = new PMIntEdit( this );
   m_pPreviewSize->setValidation( true, 10, true, 400 );
   grid->addWidget( m_pPreviewSize, 0, 1 );

   grid->addWidget( new QLabel( "Gamma:", this ), 1, 0 );
   m_pPreviewGamma = new PMFloatEdit( this );
   grid->addWidget( m_pPreviewGamma, 1, 1 );
   hlayout->addStretch( 1 );

   gb = new QGroupBox( tr( "Rendered Objects" ), this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );

   gvl->addSpacing( 10 );
   m_pPreviewSphere = new QCheckBox( tr( "Sphere" ), gb );
   gvl->addWidget( m_pPreviewSphere );
   m_pPreviewCylinder = new QCheckBox( tr( "Cylinder" ), gb );
   gvl->addWidget( m_pPreviewCylinder );
   m_pPreviewBox = new QCheckBox( tr( "Box" ), gb );
   gvl->addWidget( m_pPreviewBox );

   gb = new QGroupBox( tr( "Wall" ), this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );

   gvl->addSpacing( 10 );
   m_pPreviewWall = new QCheckBox( tr( "Enable wall" ), gb );
   gvl->addWidget( m_pPreviewWall );
   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( tr( "Color 1:" ), gb ) );
   m_pWallColor1 = new QPushButton( gb );
   hlayout->addWidget( m_pWallColor1 );
   hlayout->addWidget( new QLabel( tr( "Color 2:" ), gb ) );
   m_pWallColor2 = new QPushButton( gb );
   hlayout->addWidget( m_pWallColor2 );
   hlayout->addStretch( 1 );

   gb = new QGroupBox( tr( "Floor" ), this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );

   gvl->addSpacing( 10 );
   m_pPreviewFloor = new QCheckBox( "Enable floor", gb );
   gvl->addWidget( m_pPreviewFloor );
   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( "Color 1:", gb ) );
   m_pFloorColor1 = new QPushButton( gb );
   hlayout->addWidget( m_pFloorColor1 );
   hlayout->addWidget( new QLabel( "Color 2:", gb ) );
   m_pFloorColor2 = new QPushButton( gb );
   hlayout->addWidget( m_pFloorColor2 );
   hlayout->addStretch( 1 );

   gb = new QGroupBox( "Antialiasing", this );
   vlayout->addWidget( gb );

   gvl = new QVBoxLayout( gb );
   gvl->addSpacing( 10 );
   m_pPreviewAA = new QCheckBox( "Enable antialiasing", gb );
   gvl->addWidget( m_pPreviewAA );
   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( "Depth:", gb ) );
   m_pPreviewAALevel = new PMIntEdit( gb );
   m_pPreviewAALevel->setValidation( true, 1, true, 9 );
   hlayout->addWidget( m_pPreviewAALevel );
   hlayout->addWidget( new QLabel( "Threshold:", gb ) );
   m_pPreviewAAThreshold = new PMFloatEdit( gb );
   hlayout->addWidget( m_pPreviewAAThreshold );
   hlayout->addStretch( 1 );
   
   vlayout->addStretch( 1 );
}

void PMPreviewSettings::displaySettings()
{
   m_pPreviewSize->setValue( PMDialogEditBase::previewSize() );
   m_pPreviewGamma->setValue( PMDialogEditBase::previewGamma() );
   m_pPreviewSphere->setChecked( PMDialogEditBase::previewShowSphere() );
   m_pPreviewCylinder->setChecked( PMDialogEditBase::previewShowCylinder() );
   m_pPreviewBox->setChecked( PMDialogEditBase::previewShowBox() );
   m_pPreviewAA->setChecked( PMDialogEditBase::isPreviewAAEnabled() );
   m_pPreviewAALevel->setValue( PMDialogEditBase::previewAADepth() );
   m_pPreviewAAThreshold->setValue( PMDialogEditBase::previewAAThreshold() );
   m_pPreviewWall->setChecked( PMDialogEditBase::previewShowWall() );
   m_pPreviewFloor->setChecked( PMDialogEditBase::previewShowFloor() );
   m_pFloorColor1->setStyleSheet( "background: " + PMDialogEditBase::previewFloorColor1().name() );
   m_pFloorColor2->setStyleSheet( "background: " + PMDialogEditBase::previewFloorColor2().name() );
   m_pWallColor1->setStyleSheet( "background: " + PMDialogEditBase::previewWallColor1().name() );
   m_pWallColor2->setStyleSheet( "background: " + PMDialogEditBase::previewWallColor2().name() );
}

void PMPreviewSettings::displayDefaults()
{
   m_pPreviewSize->setValue( c_defaultTPSize );
   m_pPreviewGamma->setValue( c_defaultTPGamma );
   m_pPreviewSphere->setChecked( c_defaultTPShowSphere );
   m_pPreviewCylinder->setChecked( c_defaultTPShowCylinder );
   m_pPreviewBox->setChecked( c_defaultTPShowBox );
   m_pPreviewAA->setChecked( c_defaultTPAA );
   m_pPreviewAALevel->setValue( c_defaultTPAADepth );
   m_pPreviewAAThreshold->setValue( c_defaultTPAAThreshold );
   m_pPreviewWall->setChecked( c_defaultTPShowWall );
   m_pPreviewFloor->setChecked( c_defaultTPShowFloor );
   m_pFloorColor1->setStyleSheet( "background: " + c_defaultTPFloorColor1.name() );
   m_pFloorColor2->setStyleSheet( "background: " + c_defaultTPFloorColor2.name() );
   m_pWallColor1->setStyleSheet( "background: " + c_defaultTPWallColor1.name() );
   m_pWallColor2->setStyleSheet( "background: " + c_defaultTPWallColor2.name() );
}

bool PMPreviewSettings::validateData()
{
   if( !m_pPreviewSize->isDataValid() )
   {
      emit showMe();
      m_pPreviewSize->setFocus();
      return false;
   }
   if( !m_pPreviewGamma->isDataValid() )
   {
      emit showMe();
      m_pPreviewGamma->setFocus();
      return false;
   }
   if( !m_pPreviewAALevel->isDataValid() )
   {
      emit showMe();
      m_pPreviewAALevel->setFocus();
      return false;
   }
   if( !m_pPreviewAAThreshold->isDataValid() )
   {
      emit showMe();
      m_pPreviewAAThreshold->setFocus();
      return false;
   }
   if( !( m_pPreviewSphere->isChecked() || m_pPreviewCylinder->isChecked()
          || m_pPreviewBox->isChecked() ) )
   {
      emit showMe();
      QMessageBox::information( this, "Error", "At least one object has to be selected.",
                    QMessageBox::Ok, QMessageBox::NoButton);
  
      return false;
   }
   return true;
}

void PMPreviewSettings::applySettings()
{
   PMDialogEditBase::setPreviewSize( m_pPreviewSize->value() );
   PMDialogEditBase::setPreviewGamma( m_pPreviewGamma->value() );
   PMDialogEditBase::previewShowSphere( m_pPreviewSphere->isChecked() );
   PMDialogEditBase::previewShowCylinder( m_pPreviewCylinder->isChecked() );
   PMDialogEditBase::previewShowBox( m_pPreviewBox->isChecked() );
   PMDialogEditBase::setPreviewAAEnabled( m_pPreviewAA->isChecked() );
   PMDialogEditBase::setPreviewAADepth( m_pPreviewAALevel->value() );
   PMDialogEditBase::setPreviewAAThreshold( m_pPreviewAAThreshold->value() );
   PMDialogEditBase::previewShowFloor( m_pPreviewFloor->isChecked() );
   PMDialogEditBase::previewShowWall( m_pPreviewWall->isChecked() );

   PMDialogEditBase::setPreviewWallColor1( getStyleColor( m_pWallColor1->styleSheet() ) );
   PMDialogEditBase::setPreviewWallColor2( getStyleColor( m_pWallColor2->styleSheet() ) );
   PMDialogEditBase::setPreviewFloorColor1( getStyleColor( m_pFloorColor1->styleSheet() ) );
   PMDialogEditBase::setPreviewFloorColor2( getStyleColor( m_pFloorColor2->styleSheet() ) );
}

#include "pmpreviewsettings.moc"
