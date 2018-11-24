/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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


#include "pmfogedit.h"
#include "pmfog.h"
#include "pmlineedits.h"
#include "pmvectoredit.h"
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


PMFogEdit::PMFogEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMFogEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QVBoxLayout* vl;
   QGridLayout* gl;
   QLabel* lbl;
   QBoxLayout* tl = topLayout();
   
   Base::createTopWidgets();

   lbl = new QLabel( tr( "Fog type:" ), this );
   m_pFogTypeEdit = new QComboBox( this );
   m_pFogTypeEdit->addItem( tr( "Constant" ) );
   m_pFogTypeEdit->addItem( tr( "Ground" ) );
   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( lbl );
   hl->addWidget( m_pFogTypeEdit );
   hl->addStretch( 1 );

   lbl = new QLabel( tr( "Distance:" ), this );
   m_pDistance = new PMFloatEdit( this );
   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( lbl );
   hl->addWidget( m_pDistance );
   hl->addStretch( 1 );
   
   lbl = new QLabel( tr( "Color:" ), this );
   m_pColor = new PMColorEdit( true, this );
   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( lbl );
   hl->addWidget( m_pColor );
   hl->addStretch( 1 );

   m_pTurbulenceCheck = new QCheckBox( tr( "Turbulence" ), this );
   tl->addWidget( m_pTurbulenceCheck );

   m_pTurbulenceWidget = new QWidget( this );
   vl = new QVBoxLayout( m_pTurbulenceWidget );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   lbl = new QLabel( tr( "Value: " ), m_pTurbulenceWidget );
   m_pTurbulenceVector = new PMVectorEdit( "x", "y", "z", m_pTurbulenceWidget);
   hl->addWidget( lbl );
   hl->addWidget( m_pTurbulenceVector );
   hl->addStretch( 1 );
   hl = new QHBoxLayout();
   vl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   lbl = new QLabel( tr( "Octaves:" ), m_pTurbulenceWidget );
   m_pOctaves = new PMIntEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 0, 0 );
   gl->addWidget( m_pOctaves, 0, 1 );
   lbl = new QLabel( tr( "Omega:" ), m_pTurbulenceWidget );
   m_pOmega = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 1, 0 );
   gl->addWidget( m_pOmega, 1, 1 );
   lbl = new QLabel( tr( "Lambda:" ), m_pTurbulenceWidget );
   m_pLambda = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 2, 0 );
   gl->addWidget( m_pLambda, 2, 1 );
   lbl = new QLabel( tr( "Depth:" ), m_pTurbulenceWidget );
   m_pDepth = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 3, 0 );
   gl->addWidget( m_pDepth, 3, 1 );
   hl->addStretch( 1 );
   tl->addWidget( m_pTurbulenceWidget );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pFogOffsetLabel = new QLabel( tr( "Offset: " ), this );
   m_pFogOffset = new PMFloatEdit( this );
   m_pFogAltLabel = new QLabel( tr( "Altitude: " ), this );
   m_pFogAlt = new PMFloatEdit( this );
   gl->addWidget( m_pFogOffsetLabel, 0, 0 );
   gl->addWidget( m_pFogOffset, 0, 1 );
   gl->addWidget( m_pFogAltLabel, 1, 0 );
   gl->addWidget( m_pFogAlt, 1, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pUpLabel = new QLabel( tr( "Up: " ), this );
   m_pUp = new PMVectorEdit( "x", "y", "z", this );
   hl->addWidget( m_pUpLabel );
   hl->addWidget( m_pUp );

   connect( m_pFogTypeEdit, SIGNAL( activated( int ) ), SLOT( slotFogTypeChanged( int ) ) );
   connect( m_pDistance, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pColor, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pTurbulenceCheck, SIGNAL( clicked() ), SLOT( slotTurbulenceClicked() ) );
   connect( m_pTurbulenceVector, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOctaves, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOmega, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pLambda, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDepth, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFogOffset, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFogAlt, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pUp, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMFogEdit::displayObject( PMObject* o )
{
   if( o->isA( "Fog" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMFog* ) o;

      m_pFogTypeEdit->setCurrentIndex( m_pDisplayedObject->fogType() - 1 );
      m_pFogTypeEdit->setEnabled( !readOnly );
      m_pDistance->setValue( m_pDisplayedObject->distance() );
      m_pDistance->setReadOnly( readOnly );
      m_pColor->setColor( m_pDisplayedObject->color() );
      m_pColor->setReadOnly( readOnly );
      m_pTurbulenceCheck->setChecked( m_pDisplayedObject->isTurbulenceEnabled() );
      m_pTurbulenceCheck->setEnabled( !readOnly );
      m_pTurbulenceVector->setVector( m_pDisplayedObject->valueVector() );
      m_pTurbulenceVector->setReadOnly( readOnly );
      m_pOctaves->setValue( m_pDisplayedObject->octaves() );
      m_pOctaves->setReadOnly( readOnly );
      m_pOmega->setValue( m_pDisplayedObject->omega() );
      m_pOmega->setReadOnly( readOnly );
      m_pLambda->setValue( m_pDisplayedObject->lambda() );
      m_pLambda->setReadOnly( readOnly );
      m_pDepth->setValue( m_pDisplayedObject->depth() );
      m_pDepth->setReadOnly( readOnly );
      m_pFogOffset->setValue( m_pDisplayedObject->fogOffset() );
      m_pFogOffset->setReadOnly( readOnly );
      m_pFogAlt->setValue( m_pDisplayedObject->fogAlt() );
      m_pFogAlt->setReadOnly( readOnly );
      m_pUp->setVector( m_pDisplayedObject->up() );
      m_pUp->setReadOnly( readOnly );

      slotTurbulenceClicked();
      slotFogTypeChanged( m_pFogTypeEdit->currentIndex() );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMFogEdit: Can't display object\n";
}

void PMFogEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setFogType( m_pFogTypeEdit->currentIndex() + 1 );
      m_pDisplayedObject->setDistance( m_pDistance->value() );
      m_pDisplayedObject->setColor( m_pColor->color() );
      m_pDisplayedObject->enableTurbulence( m_pTurbulenceCheck->isChecked() );
      m_pDisplayedObject->setValueVector( m_pTurbulenceVector->vector() );
      m_pDisplayedObject->setOctaves( m_pOctaves->value() );
      m_pDisplayedObject->setOmega( m_pOmega->value() );
      m_pDisplayedObject->setLambda( m_pLambda->value() );
      m_pDisplayedObject->setDepth( m_pDepth->value() );
      m_pDisplayedObject->setFogOffset( m_pFogOffset->value() );
      m_pDisplayedObject->setFogAlt( m_pFogAlt->value() );
      m_pDisplayedObject->setUp( m_pUp->vector() );
      Base::saveContents();
   }
}

bool PMFogEdit::isDataValid()
{
   return Base::isDataValid();
}

void PMFogEdit::slotTurbulenceClicked()
{
   if( m_pTurbulenceCheck->isChecked() )
      m_pTurbulenceWidget->show();
   else
      m_pTurbulenceWidget->hide();

   emit dataChanged();
   emit sizeChanged();
}

void PMFogEdit::slotFogTypeChanged( int val )
{
   switch( val )
   {
      case 0:  // Constant Fog
         m_pFogOffsetLabel->hide();
         m_pFogOffset->hide();
         m_pFogAltLabel->hide();
         m_pFogAlt->hide();
         m_pUpLabel->hide();
         m_pUp->hide();
         break;
      case 1:   // Ground Fog
         m_pFogOffsetLabel->show();
         m_pFogOffset->show();
         m_pFogAltLabel->show();
         m_pFogAlt->show();
         m_pUpLabel->show();
         m_pUp->show();
         break;
      default:
         break;
   }

   emit dataChanged();
   emit sizeChanged();
}

#include "pmfogedit.moc"
