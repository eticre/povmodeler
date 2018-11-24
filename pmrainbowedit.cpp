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


#include "pmrainbowedit.h"
#include "pmrainbow.h"
#include "pmlineedits.h"
#include "pmvectoredit.h"
#include "pmvector.h"
#include "pmmath.h"

#include <qlayout.h>
#include <qcheckbox.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>

#include <QMessageBox>


PMRainbowEdit::PMRainbowEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMRainbowEdit::createTopWidgets()
{
   QHBoxLayout* hl;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();
   Base::createTopWidgets();

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEnableDirectionEdit = new QCheckBox( tr( "Direction:" ), this );
   m_pDirectionEdit = new PMVectorEdit( "x", "y", "z", this );
   hl->addWidget( m_pEnableDirectionEdit );
   hl->addWidget( m_pDirectionEdit );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pEnableAngleEdit = new QCheckBox( tr( "Angle:" ), this );
   m_pAngleEdit = new PMFloatEdit( this );
   gl->addWidget( m_pEnableAngleEdit, 0, 0 );
   gl->addWidget( m_pAngleEdit, 0, 1 );

   m_pEnableWidthEdit = new QCheckBox( tr( "Width:" ), this );
   m_pWidthEdit = new PMFloatEdit( this );
   gl->addWidget( m_pEnableWidthEdit, 1, 0 );
   gl->addWidget( m_pWidthEdit, 1, 1 );
   
   m_pEnableDistanceEdit = new QCheckBox( tr( "Distance:" ), this );
   m_pDistanceEdit = new PMFloatEdit( this );
   gl->addWidget( m_pEnableDistanceEdit, 2, 0 );
   gl->addWidget( m_pDistanceEdit, 2, 1 );
   
   m_pEnableJitterEdit = new QCheckBox( tr( "Jitter:" ), this );
   m_pJitterEdit = new PMFloatEdit( this );
   gl->addWidget( m_pEnableJitterEdit, 3, 0 );
   gl->addWidget( m_pJitterEdit, 3, 1 );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEnableUpEdit = new QCheckBox( tr( "Up:" ), this );
   m_pUpEdit = new PMVectorEdit( "x", "y", "z", this );
   hl->addWidget( m_pEnableUpEdit );
   hl->addWidget( m_pUpEdit );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   m_pEnableArcAngleEdit = new QCheckBox( tr( "Arc angle:" ), this );
   m_pArcAngleEdit = new PMFloatEdit( this );
   m_pArcAngleEdit->setValidation( true, 0, true, 360 );
   gl->addWidget( m_pEnableArcAngleEdit, 0, 0 );
   gl->addWidget( m_pArcAngleEdit, 0, 1 );
   m_pEnableFalloffAngleEdit = new QCheckBox( tr( "Falloff angle:" ), this );
   m_pFalloffAngleEdit = new PMFloatEdit( this );
   m_pFalloffAngleEdit->setValidation( true, 0, true, 360 );
   gl->addWidget( m_pEnableFalloffAngleEdit, 1, 0 );
   gl->addWidget( m_pFalloffAngleEdit, 1, 1 );
   hl->addStretch( 1 );

   connect( m_pDirectionEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAngleEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pWidthEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pDistanceEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pJitterEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pUpEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pArcAngleEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFalloffAngleEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pEnableDirectionEdit, SIGNAL( clicked() ), SLOT( slotDirectionClicked() ) );
   connect( m_pEnableAngleEdit, SIGNAL( clicked() ), SLOT( slotAngleClicked() ) );
   connect( m_pEnableWidthEdit, SIGNAL( clicked() ), SLOT( slotWidthClicked() ) );
   connect( m_pEnableDistanceEdit, SIGNAL( clicked() ), SLOT( slotDistanceClicked() ) );
   connect( m_pEnableJitterEdit, SIGNAL( clicked() ), SLOT( slotJitterClicked() ) );
   connect( m_pEnableUpEdit, SIGNAL( clicked() ), SLOT( slotUpClicked() ) );
   connect( m_pEnableArcAngleEdit, SIGNAL( clicked() ), SLOT( slotArcAngleClicked() ) );
   connect( m_pEnableFalloffAngleEdit, SIGNAL( clicked() ), SLOT( slotFalloffAngleClicked() ) );
}

void PMRainbowEdit::displayObject( PMObject* o )
{
   if( o->isA( "Rainbow" ) )
   {
      bool readOnly =  o->isReadOnly();
      m_pDisplayedObject = ( PMRainbow* ) o;

      m_pDirectionEdit->setVector( m_pDisplayedObject->direction() );
      m_pDirectionEdit->setReadOnly( readOnly );
      m_pAngleEdit->setValue( m_pDisplayedObject->angle() );
      m_pAngleEdit->setReadOnly( readOnly );
      m_pWidthEdit->setValue( m_pDisplayedObject->width() );
      m_pWidthEdit->setReadOnly( readOnly );
      m_pDistanceEdit->setValue( m_pDisplayedObject->distance() );
      m_pDistanceEdit->setReadOnly( readOnly );
      m_pJitterEdit->setValue( m_pDisplayedObject->jitter() );
      m_pJitterEdit->setReadOnly( readOnly );
      m_pUpEdit->setVector( m_pDisplayedObject->up() );
      m_pUpEdit->setReadOnly( readOnly );
      m_pArcAngleEdit->setValue( m_pDisplayedObject->arcAngle() );
      m_pArcAngleEdit->setReadOnly( readOnly );
      m_pFalloffAngleEdit->setValue( m_pDisplayedObject->falloffAngle() );
      m_pFalloffAngleEdit->setReadOnly( readOnly );
      m_pEnableDirectionEdit->setChecked( m_pDisplayedObject->isDirectionEnabled() );
      m_pEnableDirectionEdit->setEnabled( !readOnly );
      m_pEnableAngleEdit->setChecked( m_pDisplayedObject->isAngleEnabled() );
      m_pEnableAngleEdit->setEnabled( !readOnly );
      m_pEnableWidthEdit->setChecked( m_pDisplayedObject->isWidthEnabled() );
      m_pEnableWidthEdit->setEnabled( !readOnly );
      m_pEnableDistanceEdit->setChecked( m_pDisplayedObject->isDistanceEnabled() );
      m_pEnableDistanceEdit->setEnabled( !readOnly );
      m_pEnableJitterEdit->setChecked( m_pDisplayedObject->isJitterEnabled() );
      m_pEnableJitterEdit->setEnabled( !readOnly );
      m_pEnableUpEdit->setChecked( m_pDisplayedObject->isUpEnabled() );
      m_pEnableUpEdit->setEnabled( !readOnly );
      m_pEnableArcAngleEdit->setChecked( m_pDisplayedObject->isArcAngleEnabled() );
      m_pEnableArcAngleEdit->setEnabled( !readOnly );
      m_pEnableFalloffAngleEdit->setChecked( m_pDisplayedObject->isFalloffAngleEnabled() );
      m_pEnableFalloffAngleEdit->setEnabled( !readOnly );
      slotDirectionClicked();
      slotAngleClicked();
      slotWidthClicked();
      slotDistanceClicked();
      slotJitterClicked();
      slotUpClicked();
      slotArcAngleClicked();
      slotFalloffAngleClicked();
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMRainbowEdit: Can't display object\n";
}

void PMRainbowEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setDirection( m_pDirectionEdit->vector() );
      m_pDisplayedObject->setAngle( m_pAngleEdit->value() );
      m_pDisplayedObject->setWidth( m_pWidthEdit->value() );
      m_pDisplayedObject->setDistance( m_pDistanceEdit->value() );
      m_pDisplayedObject->setJitter( m_pJitterEdit->value() );
      m_pDisplayedObject->setUp( m_pUpEdit->vector() );
      m_pDisplayedObject->setArcAngle( m_pArcAngleEdit->value() );
      m_pDisplayedObject->setFalloffAngle( m_pFalloffAngleEdit->value() );
      m_pDisplayedObject->enableDirection( m_pEnableDirectionEdit->isChecked() );
      m_pDisplayedObject->enableAngle( m_pEnableAngleEdit->isChecked() );
      m_pDisplayedObject->enableWidth( m_pEnableWidthEdit->isChecked() );
      m_pDisplayedObject->enableDistance( m_pEnableDistanceEdit->isChecked() );
      m_pDisplayedObject->enableJitter( m_pEnableJitterEdit->isChecked() );
      m_pDisplayedObject->enableUp( m_pEnableUpEdit->isChecked() );
      m_pDisplayedObject->enableArcAngle( m_pEnableArcAngleEdit->isChecked() );
      m_pDisplayedObject->enableFalloffAngle( m_pEnableFalloffAngleEdit->isChecked() );
   }
}

bool PMRainbowEdit::isDataValid()
{
   double f_angle;

   if( !m_pDirectionEdit->isDataValid() ) return false;
   if( !m_pAngleEdit->isDataValid() ) return false;
   if( !m_pWidthEdit->isDataValid() ) return false;
   if( !m_pDistanceEdit->isDataValid() ) return false;
   if( !m_pJitterEdit->isDataValid() ) return false;
   if( !m_pUpEdit->isDataValid() ) return false;
   if( !m_pArcAngleEdit->isDataValid() ) return false;
   if( !m_pFalloffAngleEdit->isDataValid() ) return false;
   if( m_pFalloffAngleEdit->value() > m_pArcAngleEdit->value() ) 
   {
      QMessageBox::warning( this, tr( "Arc angle is smaller than falloff angle in rainbow." ),
                          tr( "Error" ) );
      return false;
   }
   if( approxZero( fabs( PMVector::dot( m_pDirectionEdit->vector(), m_pDirectionEdit->vector() ) ) ) )
   {
      QMessageBox::warning( this, tr( "Direction vector is zero." ),
                          tr( "Error" ) );
      return false;
   }
   if( approxZero( fabs( PMVector::dot( m_pUpEdit->vector(), m_pUpEdit->vector() ) ) ) )
   {
      QMessageBox::warning( this, tr( "Up vector is zero." ),
                          tr( "Error" ) );
      return false;
   }
   f_angle = fabs( rad2Deg( PMVector::angle( m_pDirectionEdit->vector(), m_pUpEdit->vector() ) ) );
   if( f_angle == 0.0 || f_angle == 180.0 )
   {
      QMessageBox::warning( this, tr( "Direction and up vectors are co-linear." ),
                          tr( "Error" ) );
      return false;
   }
   return Base::isDataValid();
}

void PMRainbowEdit::slotDirectionClicked()
{
   m_pDirectionEdit->setEnabled( m_pEnableDirectionEdit->isChecked() );
   emit dataChanged();
}

void PMRainbowEdit::slotAngleClicked()
{
   m_pAngleEdit->setEnabled( m_pEnableAngleEdit->isChecked() );
   emit dataChanged();
}

void PMRainbowEdit::slotWidthClicked()
{
   m_pWidthEdit->setEnabled( m_pEnableWidthEdit->isChecked() );
   emit dataChanged();
}

void PMRainbowEdit::slotDistanceClicked()
{
   m_pDistanceEdit->setEnabled( m_pEnableDistanceEdit->isChecked() );
   emit dataChanged();
}

void PMRainbowEdit::slotJitterClicked()
{
   m_pJitterEdit->setEnabled( m_pEnableJitterEdit->isChecked() );
   emit dataChanged();
}

void PMRainbowEdit::slotUpClicked()
{
   m_pUpEdit->setEnabled( m_pEnableUpEdit->isChecked() );
   emit dataChanged();
}

void PMRainbowEdit::slotArcAngleClicked()
{
   m_pArcAngleEdit->setEnabled( m_pEnableArcAngleEdit->isChecked() );
   emit dataChanged();
}

void PMRainbowEdit::slotFalloffAngleClicked()
{
   m_pFalloffAngleEdit->setEnabled( m_pEnableFalloffAngleEdit->isChecked() );
   emit dataChanged();
}

#include "pmrainbowedit.moc"
