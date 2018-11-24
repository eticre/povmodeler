/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#include "pmheightfieldedit.h"
#include "pmheightfield.h"
#include "pmlineedits.h"

#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QFileDialog>

PMHeightFieldEdit::PMHeightFieldEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMHeightFieldEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Type:" ), this ) );
   m_pHeightFieldType = new QComboBox( this );
   hl->addWidget( m_pHeightFieldType );
   hl->addStretch( 0 );
   m_pHeightFieldType->addItem( "gif" );
   m_pHeightFieldType->addItem( "tga" );
   m_pHeightFieldType->addItem( "pot" );
   m_pHeightFieldType->addItem( "png" );
   m_pHeightFieldType->addItem( "pgm" );
   m_pHeightFieldType->addItem( "ppm" );
   m_pHeightFieldType->addItem( "sys" );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "File name:" ), this ) );
   m_pFileName = new QLineEdit( this );
   hl->addWidget( m_pFileName );
   m_pChooseFileName = new QPushButton( this );
   m_pChooseFileName->setIcon( QIcon::fromTheme( "document-open" ) );
   hl->addWidget( m_pChooseFileName );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Water level:" ), this ) );
   m_pWaterLevel = new PMFloatEdit( this );
   m_pWaterLevel->setValidation( true, 0.0, true, 1.0 );
   hl->addWidget( m_pWaterLevel );
   hl->addStretch( 1 );

   m_pHierarchy = new QCheckBox( tr( "Hierarchy" ), this );
   tl->addWidget( m_pHierarchy );

   m_pSmooth = new QCheckBox( tr( "Smooth" ), this );
   tl->addWidget( m_pSmooth );

   connect( m_pHeightFieldType, SIGNAL( activated( int ) ),
            SLOT( slotTypeChanged( int ) ) );
   connect( m_pFileName, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotFileNameChanged( const QString& ) ) );
   connect( m_pChooseFileName, SIGNAL( clicked() ),
            SLOT( slotFileNameClicked() ) );
   connect( m_pWaterLevel, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pHierarchy, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pSmooth, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMHeightFieldEdit::displayObject( PMObject* o )
{
   if( o->isA( "HeightField" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMHeightField* ) o;

      switch( m_pDisplayedObject->heightFieldType() )
      {
         case PMHeightField::HFgif:
            m_pHeightFieldType->setCurrentIndex( 0 );
            break;
         case PMHeightField::HFtga:
            m_pHeightFieldType->setCurrentIndex( 1 );
            break;
         case PMHeightField::HFpot:
            m_pHeightFieldType->setCurrentIndex( 2 );
            break;
         case PMHeightField::HFpng:
            m_pHeightFieldType->setCurrentIndex( 3 );
            break;
         case PMHeightField::HFpgm:
            m_pHeightFieldType->setCurrentIndex( 4 );
            break;
         case PMHeightField::HFppm:
            m_pHeightFieldType->setCurrentIndex( 5 );
            break;
         case PMHeightField::HFsys:
            m_pHeightFieldType->setCurrentIndex( 6 );
            break;
      }
      m_pFileName->setText( m_pDisplayedObject->fileName() );
      m_pWaterLevel->setValue( m_pDisplayedObject->waterLevel() );
      m_pHierarchy->setChecked( m_pDisplayedObject->hierarchy() );
      m_pSmooth->setChecked( m_pDisplayedObject->smooth() );

      m_pHeightFieldType->setEnabled( !readOnly );
      m_pFileName->setReadOnly( readOnly );
      m_pChooseFileName->setEnabled( !readOnly );
      m_pHierarchy->setEnabled( !readOnly );
      m_pSmooth->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMHeightFieldEdit: Can't display object\n";
}

void PMHeightFieldEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      switch( m_pHeightFieldType->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFgif );
            break;
         case 1:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFtga );
            break;
         case 2:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFpot );
            break;
         case 3:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFpng );
            break;
         case 4:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFpgm );
            break;
         case 5:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFppm );
            break;
         case 6:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFsys );
            break;
      }
      m_pDisplayedObject->setFileName( m_pFileName->text() );
      m_pDisplayedObject->setWaterLevel( m_pWaterLevel->value() );
      m_pDisplayedObject->setHierarchy( m_pHierarchy->isChecked() );
      m_pDisplayedObject->setSmooth( m_pSmooth->isChecked() );
   }
}

bool PMHeightFieldEdit::isDataValid()
{
   if( m_pWaterLevel->isDataValid() )
      return Base::isDataValid();
   return false;
}

void PMHeightFieldEdit::slotTypeChanged( int )
{
   emit dataChanged();
}

void PMHeightFieldEdit::slotFileNameChanged( const QString& )
{
   emit dataChanged();
}

void PMHeightFieldEdit::slotFileNameClicked()
{
   QString str = QFileDialog::getOpenFileName(0, QString(),  QString());

   if( !str.isEmpty() )
   {
      m_pFileName->setText( str );
      emit dataChanged();
   }
}

#include "pmheightfieldedit.moc"
