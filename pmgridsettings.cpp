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

#include "pmgridsettings.h"

#include "pmlineedits.h"
#include "pmrendermanager.h"
#include "pmcontrolpoint.h"
#include "pmdefaults.h"

#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QColorDialog>

PMGridSettings::PMGridSettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QHBoxLayout* hlayout;
   QVBoxLayout* vlayout;
   QVBoxLayout* gvl;
   QGridLayout* grid;
   QGroupBox* gb;

   vlayout = new QVBoxLayout( this );
   vlayout->setMargin( 0 );
   gb = new QGroupBox( "Displayed Grid", this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );
   gvl->addSpacing( 10 );

   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( "Color:", gb ) );
   //m_pGridColor = new KColorButton( gb );
   m_pGridColor = new QPushButton( gb );
   hlayout->addWidget( m_pGridColor );
   hlayout->addStretch( 1 );

   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( "Distance:", gb ) );
   m_pGridDistance = new PMIntEdit( gb );
   m_pGridDistance->setValidation( true, 20, false, 0 );
   hlayout->addWidget( m_pGridDistance );
   hlayout->addStretch( 1 );

   gb = new QGroupBox( "Control Point Grid", this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );
//TODO PORT QT5    gvl->setSpacing( QDialog::spacingHint() );
//TODO PORT QT5    gvl->setMargin( QDialog::marginHint() );
   gvl->addSpacing( 10 );

   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   grid = new QGridLayout();
   hlayout->addLayout( grid );
   grid->addWidget( new QLabel( "2D/3D movement:", gb ), 0, 0 );
   m_pMoveGrid = new PMFloatEdit( gb );
   m_pMoveGrid->setValidation( true, 0.001, true, 100 );
   grid->addWidget( m_pMoveGrid, 0, 1 );
   
   grid->addWidget( new QLabel( "Scale:", gb ), 1, 0 );
   m_pScaleGrid = new PMFloatEdit( gb );
   m_pScaleGrid->setValidation( true, 0.001, true, 100 );
   grid->addWidget( m_pScaleGrid, 1, 1 );
   
   grid->addWidget( new QLabel( "Rotation:", gb ), 2, 0 );
   m_pRotateGrid = new PMFloatEdit( gb );
   m_pRotateGrid->setValidation( true, 0.001, true, 180 );
   grid->addWidget( m_pRotateGrid, 2, 1 );

   hlayout->addStretch( 1 );

   vlayout->addStretch( 1 );

   connect( m_pGridColor, &QPushButton::clicked, this, &PMGridSettings::selectColor );
}

void PMGridSettings::selectColor()
{
   QColor c;
   QString s;
   s = m_pGridColor->styleSheet();
   s.remove( 0, 12 );
   s.remove( " " );
   c = QColor(s);
   QColorDialog colordialog(this);
   c = colordialog.getColor(c);
   if( c.isValid() )
   {
      m_pGridColor->setStyleSheet( "background: " + c.name() );//+ "; border:none;" );
   }
}

void PMGridSettings::displaySettings()
{
   PMRenderManager* rm = PMRenderManager::theManager();
   m_pGridColor->setStyleSheet( "background: " + rm->gridColor().name() );
   m_pGridDistance->setValue( rm->gridDistance() );
   m_pMoveGrid->setValue( PMControlPoint::moveGrid() );
   m_pScaleGrid->setValue( PMControlPoint::scaleGrid() );
   m_pRotateGrid->setValue( PMControlPoint::rotateGrid() );
}

void PMGridSettings::displayDefaults()
{
   m_pGridColor->setStyleSheet( "background: " + c_defaultGridColor.name() );
   m_pGridDistance->setValue( c_defaultGridDistance );
   m_pMoveGrid->setValue( c_defaultMoveGrid );
   m_pScaleGrid->setValue( c_defaultScaleGrid );
   m_pRotateGrid->setValue( c_defaultRotateGrid );      
}

bool PMGridSettings::validateData()
{
   if( !m_pGridDistance->isDataValid() )
   {
      emit showMe();
      m_pGridDistance->setFocus();
      return false;
   }
   if( !m_pMoveGrid->isDataValid() )
   {
      emit showMe();
      m_pMoveGrid->setFocus();
      return false;
   }
   if( !m_pScaleGrid->isDataValid() )
   {
      emit showMe();
      m_pScaleGrid->setFocus();
      return false;
   }
   if( !m_pRotateGrid->isDataValid() )
   {
      emit showMe();
      m_pRotateGrid->setFocus();
      return false;
   }
   return true;
}

void PMGridSettings::applySettings()
{
   bool repaint = false;
   PMRenderManager* rm = PMRenderManager::theManager();
   QString str;
   str = m_pGridColor->styleSheet();
   str.remove( 0, 12 );
   str.remove( " " );
   str.truncate(7);
   if( rm->gridColor() != QColor(str) )
   {
      rm->setGridColor( QColor(str) );
      repaint = true;
   }
   if( rm->gridDistance() != m_pGridDistance->value() )
   {
      rm->setGridDistance( m_pGridDistance->value() );
      repaint = true;
   }
   PMControlPoint::setMoveGrid( m_pMoveGrid->value() );
   PMControlPoint::setScaleGrid( m_pScaleGrid->value() );
   PMControlPoint::setRotateGrid( m_pRotateGrid->value() );
   //connect( this, SIGNAL( repaintViews() ), rm, SIGNAL( renderingSettingsChanged() ) );
   if( repaint )
      emit repaintViews();
}

#include "pmgridsettings.moc"
