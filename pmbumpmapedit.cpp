/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Passos Carvalho
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


#include "pmbumpmapedit.h"
#include "pmbumpmap.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmpalettevalueedit.h"
#include "pmvector.h"

#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

PMBumpMapEdit::PMBumpMapEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMBumpMapEdit::createTopWidgets()
{
   QLabel* lbl;
   QHBoxLayout* hl;
   QBoxLayout* tl = topLayout();

   Base::createTopWidgets();

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( tr( "File type:" ), this );
   m_pImageFileTypeEdit = new QComboBox( this );
   m_pImageFileTypeEdit->addItem( "gif" );
   m_pImageFileTypeEdit->addItem( "tga" );
   m_pImageFileTypeEdit->addItem( "iff" );
   m_pImageFileTypeEdit->addItem( "ppm" );
   m_pImageFileTypeEdit->addItem( "pgm" );
   m_pImageFileTypeEdit->addItem( "png" );
   m_pImageFileTypeEdit->addItem( "jpeg" );
   m_pImageFileTypeEdit->addItem( "tiff" );
   m_pImageFileTypeEdit->addItem( "sys" );
   hl->addWidget( lbl );
   hl->addWidget( m_pImageFileTypeEdit );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( "File name:", this );
   m_pImageFileNameEdit = new QLineEdit( this );
   m_pImageFileNameBrowse = new QPushButton( this );
   m_pImageFileNameBrowse->setIcon( QIcon::fromTheme( "document-open" ) );
   hl->addWidget( lbl );
   hl->addWidget( m_pImageFileNameEdit );
   hl->addWidget( m_pImageFileNameBrowse );
   hl->addStretch( 1 );

   m_pOnceEdit = new QCheckBox( "Once", this );
   tl->addWidget( m_pOnceEdit );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( "Interpolate:", this );
   m_pInterpolateTypeEdit = new QComboBox( this );
   m_pInterpolateTypeEdit->addItem( "None" );
   m_pInterpolateTypeEdit->addItem( "Bilinear" );
   m_pInterpolateTypeEdit->addItem( "Normalized" );
   hl->addWidget( lbl );
   hl->addWidget( m_pInterpolateTypeEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( "Map type:", this );
   m_pMapTypeEdit = new QComboBox( this );
   m_pMapTypeEdit->addItem( "Planar" );
   m_pMapTypeEdit->addItem( "Spherical" );
   m_pMapTypeEdit->addItem( "Cylindrical" );
   m_pMapTypeEdit->addItem( "Toroidal" );
   hl->addWidget( lbl );
   hl->addWidget( m_pMapTypeEdit );
   hl->addStretch( 1 );

   m_pUseIndexEdit = new QCheckBox( tr( "Use index" ), this );
   topLayout()->addWidget( m_pUseIndexEdit );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( "Bump size:", this );
   m_pBumpSizeEdit = new PMFloatEdit( this );
   hl->addWidget( lbl );
   hl->addWidget( m_pBumpSizeEdit );
   hl->addStretch( 1 );

   connect( m_pImageFileTypeEdit, SIGNAL( activated( int ) ), SLOT( slotImageFileTypeChanged( int ) ) );
   connect( m_pMapTypeEdit, SIGNAL( activated( int ) ), SLOT( slotMapTypeChanged( int ) ) );
   connect( m_pInterpolateTypeEdit, SIGNAL( activated( int ) ), SLOT( slotInterpolateTypeChanged( int ) ) );
   connect( m_pImageFileNameBrowse, SIGNAL( clicked() ), SLOT( slotImageFileBrowseClicked() ) );
   connect( m_pImageFileNameEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotImageFileNameChanged( const QString& ) ) );
   connect( m_pOnceEdit, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pUseIndexEdit, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pBumpSizeEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
}

void PMBumpMapEdit::displayObject( PMObject* o )
{
   bool readOnly;

   if( o->isA( "BumpMap" ) )
   {
      m_pDisplayedObject = ( PMBumpMap* ) o;
      readOnly = m_pDisplayedObject->isReadOnly();

      switch( m_pDisplayedObject->bitmapType() )
      {
         case PMBumpMap::BitmapGif:
            m_pImageFileTypeEdit->setCurrentIndex( 0 );
            break;
         case PMBumpMap::BitmapTga:
            m_pImageFileTypeEdit->setCurrentIndex( 1 );
            break;
         case PMBumpMap::BitmapIff:
            m_pImageFileTypeEdit->setCurrentIndex( 2 );
            break;
         case PMBumpMap::BitmapPpm:
            m_pImageFileTypeEdit->setCurrentIndex( 3 );
            break;
         case PMBumpMap::BitmapPgm:
            m_pImageFileTypeEdit->setCurrentIndex( 4 );
            break;
         case PMBumpMap::BitmapPng:
            m_pImageFileTypeEdit->setCurrentIndex( 5 );
            break;
         case PMBumpMap::BitmapJpeg:
            m_pImageFileTypeEdit->setCurrentIndex( 6 );
            break;
         case PMBumpMap::BitmapTiff:
            m_pImageFileTypeEdit->setCurrentIndex( 7 );
            break;
         case PMBumpMap::BitmapSys:
            m_pImageFileTypeEdit->setCurrentIndex( 8 );
            break;
      }
      m_pImageFileTypeEdit->setEnabled( !readOnly );

      switch( m_pDisplayedObject->interpolateType() )
      {
         case PMBumpMap::InterpolateNone:
            m_pInterpolateTypeEdit->setCurrentIndex( 0 );
            break;
         case PMBumpMap::InterpolateBilinear:
            m_pInterpolateTypeEdit->setCurrentIndex( 1);
            break;
         case PMBumpMap::InterpolateNormalized:
            m_pInterpolateTypeEdit->setCurrentIndex( 2 );
            break;
      }
      m_pInterpolateTypeEdit->setEnabled( !readOnly );

      switch( m_pDisplayedObject->mapType() )
      {
         case PMBumpMap::MapPlanar:
            m_pMapTypeEdit->setCurrentIndex( 0 );
            break;
         case PMBumpMap::MapSpherical:
            m_pMapTypeEdit->setCurrentIndex( 1 );
            break;
         case PMBumpMap::MapCylindrical:
            m_pMapTypeEdit->setCurrentIndex( 2 );
            break;
         case PMBumpMap::MapToroidal:
            m_pMapTypeEdit->setCurrentIndex( 3 );
            break;
      }
      m_pMapTypeEdit->setEnabled( !readOnly );

      m_pImageFileNameEdit->setText( m_pDisplayedObject->bitmapFile() );
      m_pImageFileNameEdit->setEnabled( !readOnly );
      m_pOnceEdit->setChecked( m_pDisplayedObject->isOnceEnabled() );
      m_pOnceEdit->setEnabled( !readOnly );
      m_pUseIndexEdit->setChecked( m_pDisplayedObject->isUseIndexEnabled() );
      m_pUseIndexEdit->setEnabled( !readOnly );
      m_pBumpSizeEdit->setValue( m_pDisplayedObject->bumpSize() );
      m_pBumpSizeEdit->setReadOnly( readOnly );

      Base::displayObject( o );
   }

}

void PMBumpMapEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      switch( m_pImageFileTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapGif );
            break;
         case 1:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapTga );
            break;
         case 2:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapIff );
            break;
         case 3:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapPpm );
            break;
         case 4:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapPgm );
            break;
         case 5:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapPng );
            break;
         case 6:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapJpeg );
            break;
         case 7:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapTiff );
            break;
         case 8:
            m_pDisplayedObject->setBitmapType( PMBumpMap::BitmapSys );
            break;
      }

      switch( m_pInterpolateTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setInterpolateType( PMBumpMap::InterpolateNone );
            break;
         case 1:
            m_pDisplayedObject->setInterpolateType( PMBumpMap::InterpolateBilinear );
            break;
         case 2:
            m_pDisplayedObject->setInterpolateType( PMBumpMap::InterpolateNormalized );
            break;
      }

      switch( m_pMapTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setMapType( PMBumpMap::MapPlanar );
            break;
         case 1:
            m_pDisplayedObject->setMapType( PMBumpMap::MapSpherical );
            break;
         case 2:
            m_pDisplayedObject->setMapType( PMBumpMap::MapCylindrical );
            break;
         case 3:
            m_pDisplayedObject->setMapType( PMBumpMap::MapToroidal );
            break;
      }

      m_pDisplayedObject->setBitmapFileName( m_pImageFileNameEdit->text() );
      m_pDisplayedObject->enableOnce( m_pOnceEdit->isChecked() );
      m_pDisplayedObject->enableUseIndex( m_pUseIndexEdit->isChecked() );
      m_pDisplayedObject->setBumpSize( m_pBumpSizeEdit->value() );
   }
}

bool PMBumpMapEdit::isDataValid()
{
   if( !m_pBumpSizeEdit->isDataValid() ) return false;

   return Base::isDataValid();
}

void PMBumpMapEdit::slotInterpolateTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMBumpMapEdit::slotImageFileTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMBumpMapEdit::slotMapTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMBumpMapEdit::slotImageFileNameChanged( const QString& /*a*/ )
{
   emit dataChanged();
}

void PMBumpMapEdit::slotImageFileBrowseClicked()
{
   QString str = QFileDialog::getOpenFileName(nullptr, QString(),  QString());

   if( !str.isEmpty() )
   {
      m_pImageFileNameEdit->setText( str );
      emit dataChanged();
   }
}

#include "pmbumpmapedit.moc"
