/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#include "pmlightedit.h"
#include "pmlight.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmcoloredit.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>



PMLightEdit::PMLightEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMLightEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   QBoxLayout* tl = topLayout();
   
   m_pLocation = new PMVectorEdit( "x", "y", "z", this );
   m_pColor = new PMColorEdit( false, this );
   m_pType = new QComboBox( this );
   m_pType->addItem( tr( "Point Light" ) );
   m_pType->addItem( tr( "Spot Light" ) );
   m_pType->addItem( tr( "Cylindrical Light" ) );
   m_pType->addItem( tr( "Shadowless Light" ) );

   m_pRadius = new PMFloatEdit( this );
   m_pRadius->setValidation( false, 0, true, 90 );
   m_pRadiusLabel = new QLabel( tr( "Radius:" ), this );
   
   m_pFalloff = new PMFloatEdit( this );
   m_pFalloff->setValidation( false, 0, true, 90 );
   m_pFalloffLabel = new QLabel( tr( "Falloff:" ), this );
   
   m_pTightness = new PMFloatEdit( this );
   m_pTightness->setValidation( false, 0, true, 90 );
   m_pTightnessLabel = new QLabel( ( "Tightness:" ), this );

   m_pPointAt = new PMVectorEdit( "x", "y", "z", this );
   m_pPointAtLabel = new QLabel( ( "Point at:" ), this );

   m_pParallel = new QCheckBox( ( "Parallel" ), this );

   m_pAreaLight = new QCheckBox( ( "Area light" ), this );

   m_pAreaTypeLabel = new QLabel( ( "Area type:" ), this );
   m_pAreaType = new QComboBox( this );
   m_pAreaType->addItem( ( "Rectangular" ) );
   m_pAreaType->addItem( ( "Circular" ) );

   m_pAxis1 = new PMVectorEdit( "x", "y", "z", this );
   m_pAxis1Label = new QLabel( ( "Axis 1:" ), this );
   m_pAxis2 = new PMVectorEdit( "x", "y", "z", this );
   m_pAxis2Label = new QLabel( ( "Axis 2:" ), this );
   
   m_pSize1 = new PMIntEdit( this );
   m_pSize1->setValidation( true, 1, true, 50 );
   m_pSize1Label = new QLabel( ( "Size 1:" ), this );
   m_pSize2 = new PMIntEdit( this );
   m_pSize2->setValidation( true, 1, true, 50 );
   m_pSize2Label = new QLabel( ( "Size 2:" ), this );

   m_pAdaptive = new PMIntEdit( this );
   m_pAdaptive->setValidation( true, 0, false, 0 );
   m_pAdaptiveLabel = new QLabel( ( "Adaptive:" ), this );
   m_pOrient = new QCheckBox( ( "Orient" ), this );
   m_pJitter = new QCheckBox( ( "Jitter" ), this );

   m_pFading = new QCheckBox( ( "Fading" ), this );
   
   m_pFadeDistance = new PMFloatEdit( this );
   m_pFadeDistance->setValidation( true, 0, false, 0 );
   m_pFadeDistanceLabel = new QLabel( ( "Fade distance:" ), this );

   m_pFadePower = new PMIntEdit( this );
   m_pFadePower->setValidation( true, 0, false, 0 );
   m_pFadePowerLabel = new QLabel( ( "Fade power:" ), this );

   m_pMediaInteraction = new QCheckBox( ( "Media interaction" ), this );
   m_pMediaAttenuation = new QCheckBox( ( "Media attenuation" ), this );

   QHBoxLayout* hl;
   QGridLayout* gl;

   gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( new QLabel( ( "Location:" ), this ), 0, 0 );
   gl->addWidget( m_pLocation, 0, 1 );
   gl->addWidget( new QLabel( ( "Color:" ), this ), 1, 0, Qt::AlignTop );
   gl->addWidget( m_pColor, 1, 1 );
   gl->addWidget( new QLabel( ( "Type:" ), this ), 2, 0 );
   hl = new QHBoxLayout();
   gl->addLayout( hl, 2, 1 );
   hl->addWidget( m_pType );
   hl->addStretch( 1 );

   gl = new QGridLayout(); 
   tl->addLayout( gl );
   gl->addWidget( m_pRadiusLabel, 0, 0 );
   gl->addWidget( m_pRadius, 0, 1, Qt::AlignLeft );
   gl->addWidget( m_pFalloffLabel, 1, 0 );
   gl->addWidget( m_pFalloff, 1, 1, Qt::AlignLeft );
   gl->addWidget( m_pTightnessLabel, 2, 0 );
   gl->addWidget( m_pTightness, 2, 1, Qt::AlignLeft );
   gl->addWidget( m_pPointAtLabel, 3, 0 );
   gl->addWidget( m_pPointAt, 3, 1 );

   tl->addWidget( m_pParallel );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( m_pAreaLight, 0, 0, 1, 2 );
   gl->addWidget( m_pAreaTypeLabel, 1, 0 );
   gl->addWidget( m_pAreaType, 1, 1 );
   gl->addWidget( m_pAxis1Label, 2, 0 );
   gl->addWidget( m_pAxis1, 2, 1 );
   gl->addWidget( m_pAxis2Label, 3, 0 );
   gl->addWidget( m_pAxis2, 3, 1 );
   gl->addWidget( m_pSize1Label, 4, 0 );
   gl->addWidget( m_pSize1, 4, 1, Qt::AlignLeft );
   gl->addWidget( m_pSize2Label, 5, 0 );
   gl->addWidget( m_pSize2, 5, 1, Qt::AlignLeft );
   gl->addWidget( m_pAdaptiveLabel, 6, 0 );
   gl->addWidget( m_pAdaptive, 6, 1, Qt::AlignLeft );
   hl->addStretch( 1 );

   tl->addWidget( m_pOrient );
   tl->addWidget( m_pJitter );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( m_pFading, 0, 0, 1, 2 );
   gl->addWidget( m_pFadeDistanceLabel, 1, 0 );
   gl->addWidget( m_pFadeDistance, 1, 1 );
   gl->addWidget( m_pFadePowerLabel, 2, 0 );
   gl->addWidget( m_pFadePower, 2, 1 );
   hl->addStretch( 1 );

   tl->addWidget( m_pMediaInteraction );
   tl->addWidget( m_pMediaAttenuation );
   
   connect( m_pLocation, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pColor, SIGNAL( dataChanged() ),
            SIGNAL( dataChanged() ) );
   connect( m_pType, SIGNAL( activated( int ) ),
            SLOT( slotTypeActivated( int ) ) );
   connect( m_pRadius, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pFalloff, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pTightness, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pPointAt, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pParallel, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pAreaLight, SIGNAL( clicked() ), SLOT( slotAreaClicked() ) );
   connect( m_pAreaType, SIGNAL( activated ( int ) ), SLOT( slotOrientCheck() ) );
   connect( m_pAxis1, SIGNAL( dataChanged() ), SLOT( slotOrientCheck() ) );
   connect( m_pAxis2, SIGNAL( dataChanged() ), SLOT( slotOrientCheck() ) );
   connect( m_pSize1, SIGNAL( dataChanged() ), SLOT( slotOrientCheck() ) );
   connect( m_pSize2, SIGNAL( dataChanged() ), SLOT( slotOrientCheck() ) );
   connect( m_pAdaptive, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOrient, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pJitter, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pFading, SIGNAL( clicked() ), SLOT( slotFadingClicked() ) );
   connect( m_pFadeDistance, SIGNAL( dataChanged() ),
            SIGNAL( dataChanged() ) );
   connect( m_pFadePower, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMediaInteraction, SIGNAL( clicked() ),
            SIGNAL( dataChanged() ) );
   connect( m_pMediaAttenuation, SIGNAL( clicked() ),
            SIGNAL( dataChanged() ) );
}

void PMLightEdit::displayObject( PMObject* o )
{
   if( o->isA( "Light" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMLight* ) o;

      m_pLocation->setVector( m_pDisplayedObject->location() );
      m_pLocation->setReadOnly( readOnly );
      m_pColor->setColor( m_pDisplayedObject->color() );
      m_pColor->setReadOnly( readOnly );
      m_pType->setCurrentIndex( m_pDisplayedObject->lightType() );
      m_pType->setEnabled( !readOnly );
      slotTypeActivated( m_pDisplayedObject->lightType() );
      m_pRadius->setValue( m_pDisplayedObject->radius() );
      m_pRadius->setReadOnly( readOnly );
      m_pFalloff->setValue( m_pDisplayedObject->falloff() );
      m_pFalloff->setReadOnly( readOnly );
      m_pTightness->setValue( m_pDisplayedObject->tightness() );
      m_pTightness->setReadOnly( readOnly );
      m_pPointAt->setVector( m_pDisplayedObject->pointAt() );
      m_pPointAt->setReadOnly( readOnly );
      m_pParallel->setChecked( m_pDisplayedObject->parallel() );
      m_pParallel->setEnabled( !readOnly );
      m_pAreaLight->setChecked( m_pDisplayedObject->isAreaLight() );
      m_pAreaLight->setEnabled( !readOnly );
      m_pAreaType->setCurrentIndex( m_pDisplayedObject->areaType() );
      m_pAreaType->setEnabled( !readOnly );
      m_pAxis1->setVector( m_pDisplayedObject->axis1() );
      m_pAxis1->setReadOnly( readOnly );
      m_pAxis2->setVector( m_pDisplayedObject->axis2() );
      m_pAxis2->setReadOnly( readOnly );
      m_pSize1->setValue( m_pDisplayedObject->size1() );
      m_pSize1->setReadOnly( readOnly );
      m_pSize2->setValue( m_pDisplayedObject->size2() );
      m_pSize2->setReadOnly( readOnly );
      m_pAdaptive->setValue( m_pDisplayedObject->adaptive() );
      m_pAdaptive->setReadOnly( readOnly );
      m_pOrient->setChecked( m_pDisplayedObject->orient() );
      m_pOrient->setEnabled( orientEnabled( readOnly ) );
      m_pJitter->setChecked( m_pDisplayedObject->jitter() );
      m_pJitter->setEnabled( !readOnly );
      slotAreaClicked();
      m_pFading->setChecked( m_pDisplayedObject->fading() );
      m_pFading->setEnabled( !readOnly );
      m_pFadeDistance->setValue( m_pDisplayedObject->fadeDistance() );
      m_pFadeDistance->setReadOnly( readOnly );
      m_pFadePower->setValue( m_pDisplayedObject->fadePower() );
      m_pFadePower->setReadOnly( readOnly );
      slotFadingClicked();
      m_pMediaInteraction->setChecked( m_pDisplayedObject->mediaInteraction() );
      m_pMediaInteraction->setEnabled( !readOnly );
      m_pMediaAttenuation->setChecked( m_pDisplayedObject->mediaAttenuation() );
      m_pMediaAttenuation->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMLightEdit: Can't display object\n";
}

void PMLightEdit::saveContents()
{
   int index;

   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setLocation( m_pLocation->vector() );
      m_pDisplayedObject->setColor( m_pColor->color() );

      index = m_pType->currentIndex();
      if( ( index == 1 ) || ( index == 2 ) )
      {
         m_pDisplayedObject->setRadius( m_pRadius->value() );
         m_pDisplayedObject->setFalloff( m_pFalloff->value() );
         m_pDisplayedObject->setTightness( m_pTightness->value() );
         m_pDisplayedObject->setPointAt( m_pPointAt->vector() );
      }
      m_pDisplayedObject->setLightType( ( PMLight::PMLightType ) index );

      m_pDisplayedObject->setParallel( m_pParallel->isChecked() );

      if( m_pAreaLight->isChecked() )
      {
         m_pDisplayedObject->setAreaType(
            ( PMLight::PMAreaType ) m_pAreaType->currentIndex() );
         m_pDisplayedObject->setAxis1( m_pAxis1->vector() );
         m_pDisplayedObject->setAxis2( m_pAxis2->vector() );
         m_pDisplayedObject->setSize1( m_pSize1->value() );
         m_pDisplayedObject->setSize2( m_pSize2->value() );
         m_pDisplayedObject->setAdaptive( m_pAdaptive->value() );
         m_pDisplayedObject->setOrient( m_pOrient->isChecked() );
         m_pDisplayedObject->setJitter( m_pJitter->isChecked() );
      }
      m_pDisplayedObject->setAreaLight( m_pAreaLight->isChecked() );

      if( m_pFading->isChecked() )
      {
         m_pDisplayedObject->setFadePower( m_pFadePower->value() );
         m_pDisplayedObject->setFadeDistance( m_pFadeDistance->value() );
      }
      m_pDisplayedObject->setFading( m_pFading->isChecked() );
      
      m_pDisplayedObject->setMediaInteraction( m_pMediaInteraction->isChecked() );
      m_pDisplayedObject->setMediaAttenuation( m_pMediaAttenuation->isChecked() );
   }
}

bool PMLightEdit::isDataValid()
{
   int index;
   if( !m_pLocation->isDataValid() ) return false;
   if( !m_pColor->isDataValid() ) return false;

   index = m_pType->currentIndex();
   if( ( index == 1 ) || ( index == 2 ) )
   {
      if( !m_pRadius->isDataValid() ) return false;
      if( !m_pFalloff->isDataValid() ) return false;
      if( !m_pTightness->isDataValid() ) return false;
      if( !m_pPointAt->isDataValid() ) return false;
   }
   if( m_pAreaLight->isChecked() )
   {
      if( !m_pAxis1->isDataValid() ) return false;
      if( !m_pAxis2->isDataValid() ) return false;
      if( !m_pSize1->isDataValid() ) return false;
      if( !m_pSize2->isDataValid() ) return false;
      if( !m_pAdaptive->isDataValid() ) return false;
   }
   if( m_pFading->isChecked() )
   {
      if( !m_pFadeDistance->isDataValid() ) return false;
      if( !m_pFadePower->isDataValid() ) return false;
   }
   
   return Base::isDataValid();
}

void PMLightEdit::slotTypeActivated( int index )
{
   if( ( index == 1 ) || ( index == 2 ) )
   {
      m_pRadius->show();
      m_pRadiusLabel->show();
      m_pFalloff->show();
      m_pFalloffLabel->show();
      m_pTightness->show();
      m_pTightnessLabel->show();
      m_pPointAt->show();
      m_pPointAtLabel->show();
   }
   else
   {
      m_pRadius->hide();
      m_pRadiusLabel->hide();
      m_pFalloff->hide();
      m_pFalloffLabel->hide();
      m_pTightness->hide();
      m_pTightnessLabel->hide();
      m_pPointAt->hide();
      m_pPointAtLabel->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

void PMLightEdit::slotAreaClicked()
{
   if( m_pAreaLight->isChecked() )
   {
      m_pAreaTypeLabel->show();
      m_pAreaType->show();
      m_pAxis1->show();
      m_pAxis1Label->show();
      m_pAxis2->show();
      m_pAxis2Label->show();
      m_pSize1->show();
      m_pSize1Label->show();
      m_pSize2->show();
      m_pSize2Label->show();
      m_pAdaptive->show();
      m_pAdaptiveLabel->show();
      m_pOrient->show();
      m_pOrient->setEnabled( orientEnabled( !m_pJitter->isEnabled() ) );
      m_pJitter->show();
   }
   else
   {
      m_pAreaTypeLabel->hide();
      m_pAreaType->hide();
      m_pAxis1->hide();
      m_pAxis1Label->hide();
      m_pAxis2->hide();
      m_pAxis2Label->hide();
      m_pSize1->hide();
      m_pSize1Label->hide();
      m_pSize2->hide();
      m_pSize2Label->hide();
      m_pAdaptive->hide();
      m_pAdaptiveLabel->hide();
      m_pOrient->hide();
      m_pJitter->hide();
   }

   emit dataChanged();
   emit sizeChanged();
}

void PMLightEdit::slotOrientCheck()
{
   m_pOrient->setEnabled( orientEnabled( !m_pJitter->isEnabled() ) );
   emit dataChanged();
}

void PMLightEdit::slotFadingClicked()
{
   if( m_pFading->isChecked() )
   {
      m_pFadeDistance->show();
      m_pFadeDistanceLabel->show();
      m_pFadePower->show();
      m_pFadePowerLabel->show();
   }
   else
   {
      m_pFadeDistance->hide();
      m_pFadeDistanceLabel->hide();
      m_pFadePower->hide();
      m_pFadePowerLabel->hide();
   }
   emit dataChanged();
   emit sizeChanged();
}

bool PMLightEdit::orientEnabled( bool readOnly )
{
   if ( readOnly )
      return false;

   if ( m_pAreaLight )
   {
      if ( m_pAreaType->currentIndex() == 1 )
      {
         int size1 = m_pSize1->value();
         int size2 = m_pSize2->value();
         if ( size1 > 1 && size2 > 1 && size1 == size2 )
         {
            if ( m_pAxis1->vector().abs() == m_pAxis2->vector().abs() )
               return true;
         }
      }
   }
   m_pOrient->setChecked( false );
   return false;
}

#include "pmlightedit.moc"
