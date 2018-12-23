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

#include "pmcolorsettings.h"

#include "pmrendermanager.h"
#include "pmdefaults.h"

#include <QLayout>
#include <QLabel>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QColorDialog>


PMColorSettings::PMColorSettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QWidget* ww = new QWidget( this );
   ww->setStyleSheet( "border: none;" );
   QHBoxLayout* lw = new QHBoxLayout( this );
   lw->addWidget( ww );
   QHBoxLayout* hlayout;
   QVBoxLayout* vlayout;
   QGridLayout* grid;

   vlayout = new QVBoxLayout( this );
   vlayout->setMargin( 0 );
   grid = new QGridLayout();
   ww->setLayout( grid );
   vlayout->addLayout( grid );
   
   grid->addWidget( new QLabel( "Background:", this ), 0, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 0, 2 );
   m_pBackgroundColor = new QPushButton( this );
   hlayout->addWidget( m_pBackgroundColor );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( "Wire frame:", this ), 1, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 1, 2 );

   m_pGraphicalObjectsColor[0] = new QPushButton( this );
   m_pGraphicalObjectsColor[0]->setAutoFillBackground( true );

   hlayout->addWidget( m_pGraphicalObjectsColor[0] );
   hlayout->addWidget( new QLabel( "Selected:", this ) );

   m_pGraphicalObjectsColor[1] = new QPushButton( this );
   m_pGraphicalObjectsColor[1]->setAutoFillBackground( true );
   hlayout->addWidget( m_pGraphicalObjectsColor[1] );
   hlayout->addStretch( 1 );
   
   grid->addWidget( new QLabel( tr( "Control points:" ), this ), 2, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 2, 2 );

   m_pControlPointsColor[0] = new QPushButton( this );
   hlayout->addWidget( m_pControlPointsColor[0] );
   hlayout->addWidget( new QLabel( tr( "Selected:" ), this ) );

   m_pControlPointsColor[1] = new QPushButton( this );
   hlayout->addWidget( m_pControlPointsColor[1] );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( tr( "Axes:" ), this ), 3, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 3, 2 );
   grid->addWidget( new QLabel( "x", this ), 3, 1 );
   m_pAxesColor[0] = new QPushButton( this );
   hlayout->addWidget( m_pAxesColor[0] );
   hlayout->addWidget( new QLabel( "y", this ) );
   m_pAxesColor[1] = new QPushButton( this );
   hlayout->addWidget( m_pAxesColor[1] );
   hlayout->addWidget( new QLabel( "z", this ) );
   m_pAxesColor[2] = new QPushButton( this );
   hlayout->addWidget( m_pAxesColor[2] );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( tr( "Field of view:" ), this ), 4, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 4, 2 );
   m_pFieldOfViewColor = new QPushButton( this );
   hlayout->addWidget( m_pFieldOfViewColor );
   hlayout->addStretch( 1 );

   connect( m_pBackgroundColor, &QPushButton::clicked, [=] { emit btn(m_pBackgroundColor); } );
   connect( m_pGraphicalObjectsColor[0], &QPushButton::clicked, [=] { emit btn(m_pGraphicalObjectsColor[0]); } );
   connect( m_pGraphicalObjectsColor[1], &QPushButton::clicked, [=] { emit btn(m_pGraphicalObjectsColor[1]); } );
   connect( m_pControlPointsColor[0], &QPushButton::clicked, [=] { emit btn(m_pControlPointsColor[0]); } );
   connect( m_pControlPointsColor[1], &QPushButton::clicked, [=] { emit btn(m_pControlPointsColor[1]); } );
   connect( m_pAxesColor[0], &QPushButton::clicked, [=] { emit btn(m_pAxesColor[0]); } );
   connect( m_pAxesColor[1], &QPushButton::clicked, [=] { emit btn(m_pAxesColor[1]); } );
   connect( m_pAxesColor[2], &QPushButton::clicked, [=] { emit btn(m_pAxesColor[2]); } );
   connect( m_pFieldOfViewColor, &QPushButton::clicked, [=] { emit btn(m_pFieldOfViewColor); } );
   //connect( this, SIGNAL(btn(QPushButton*)), this, SLOT(selectColor(QPushButton*)) );
   connect( this, &PMColorSettings::btn, this, &PMColorSettings::selectColor );
}

void PMColorSettings::selectColor( QPushButton* b )
{
   QColor c;
   c = getStyleColor( b->styleSheet() );
   QColorDialog colordialog( this );
   c = colordialog.getColor( c );
   if( c.isValid() )
   {
      b->setStyleSheet( "background: " + c.name() );//+ "; border:none;" );
   }
}

void PMColorSettings::displaySettings()
{
   PMRenderManager* rm = PMRenderManager::theManager();

   m_pBackgroundColor->setStyleSheet( "background: " + rm->backgroundColor().name() );

   m_pGraphicalObjectsColor[0]->setStyleSheet( "background: " + rm->graphicalObjectColor( 0 ).name() );

   m_pGraphicalObjectsColor[1]->setStyleSheet( "background: " + rm->graphicalObjectColor( 1 ).name() );

   m_pControlPointsColor[0]->setStyleSheet( "background: " + rm->controlPointColor( 0 ).name() );

   m_pControlPointsColor[1]->setStyleSheet( "background: " + rm->controlPointColor( 1 ).name() );

   m_pAxesColor[0]->setStyleSheet( "background: " + rm->axesColor( 0 ).name() );

   m_pAxesColor[1]->setStyleSheet( "background: " + rm->axesColor( 1 ).name() );

   m_pAxesColor[2]->setStyleSheet( "background: " + rm->axesColor( 2 ).name() );

   m_pFieldOfViewColor->setStyleSheet( "background: " + rm->fieldOfViewColor().name() );
}

void PMColorSettings::displayDefaults()
{
   m_pBackgroundColor->setStyleSheet( "background: " + c_defaultBackgroundColor.name() );

   m_pGraphicalObjectsColor[0]->setStyleSheet( "background: " + c_defaultGraphicalObjectColor0.name() );

   m_pGraphicalObjectsColor[1]->setStyleSheet( "background: " + c_defaultGraphicalObjectColor1.name() );

   m_pControlPointsColor[0]->setStyleSheet( "background: " + c_defaultControlPointColor0.name() );

   m_pControlPointsColor[1]->setStyleSheet( "background: " + c_defaultControlPointColor1.name() );

   m_pAxesColor[0]->setStyleSheet( "background: " + c_defaultAxesColorX.name() );

   m_pAxesColor[1]->setStyleSheet( "background: " + c_defaultAxesColorY.name() );

   m_pAxesColor[2]->setStyleSheet( "background: " + c_defaultAxesColorZ.name() );

   m_pFieldOfViewColor->setStyleSheet( "background: " + c_defaultFieldOfViewColor.name() );
}

bool PMColorSettings::validateData()
{
   return true;
}

QColor PMColorSettings::getStyleColor( QString str )
{
    str.remove( 0, 12 );
    str.remove( " " );
    return QColor ( str.left(7) );
}

void PMColorSettings::applySettings()
{
   int i;
   PMRenderManager* rm = PMRenderManager::theManager();

   rm->setBackgroundColor( getStyleColor( m_pBackgroundColor->styleSheet() ) );

   for( i = 0; i < 2; i++ )
   {
         rm->setGraphicalObjectColor( i, getStyleColor( m_pGraphicalObjectsColor[i]->styleSheet() ) );
   }

   for( i = 0; i < 2; i++ )
   {
         rm->setControlPointColor( i, getStyleColor( m_pControlPointsColor[i]->styleSheet() ) );
   }

   for( i = 0; i < 3; i++ )
   {
         rm->setAxesColor( i, getStyleColor( m_pAxesColor[i]->styleSheet() ) );
   }

      rm->setFieldOfViewColor( getStyleColor( m_pFieldOfViewColor->styleSheet() ) );

      emit repaintViews();
}

#include "pmcolorsettings.moc"
