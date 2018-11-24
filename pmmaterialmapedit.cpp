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


#include "pmmaterialmapedit.h"
#include "pmmaterialmap.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmpalettevalueedit.h"
#include "pmvector.h"

#include <qwidget.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

//Added by qt3to4:
#include <QHBoxLayout>
#include <QFileDialog>

PMMaterialMapEdit::PMMaterialMapEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMMaterialMapEdit::createTopWidgets()
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
   lbl = new QLabel( tr( "File name:" ), this );
   m_pImageFileNameEdit = new QLineEdit( this );
   m_pImageFileNameBrowse = new QPushButton( this );
   m_pImageFileNameBrowse->setIcon( QIcon::fromTheme( "document-open" ) );
   hl->addWidget( lbl );
   hl->addWidget( m_pImageFileNameEdit );
   hl->addWidget( m_pImageFileNameBrowse );
   hl->addStretch( 1 );

   m_pOnceEdit = new QCheckBox( tr( "Once" ), this );
   tl->addWidget( m_pOnceEdit );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( tr( "Interpolate:" ), this );
   m_pInterpolateTypeEdit = new QComboBox( this );
   m_pInterpolateTypeEdit->addItem( tr( "None" ) );
   m_pInterpolateTypeEdit->addItem( tr( "Bilinear" ) );
   m_pInterpolateTypeEdit->addItem( tr( "Normalized" ) );
   hl->addWidget( lbl );
   hl->addWidget( m_pInterpolateTypeEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   lbl = new QLabel( tr( "Map type:" ), this );
   m_pMapTypeEdit = new QComboBox( this );
   m_pMapTypeEdit->addItem( tr( "Planar" ) );
   m_pMapTypeEdit->addItem( tr( "Spherical" ) );
   m_pMapTypeEdit->addItem( tr( "Cylindrical" ) );
   m_pMapTypeEdit->addItem( tr( "Toroidal" ) );
   hl->addWidget( lbl );
   hl->addWidget( m_pMapTypeEdit );
   hl->addStretch( 1 );

   connect( m_pImageFileTypeEdit, SIGNAL( activated( int ) ), SLOT( slotImageFileTypeChanged( int ) ) );
   connect( m_pMapTypeEdit, SIGNAL( activated( int ) ), SLOT( slotMapTypeChanged( int ) ) );
   connect( m_pInterpolateTypeEdit, SIGNAL( activated( int ) ), SLOT( slotInterpolateTypeChanged( int ) ) );
   connect( m_pImageFileNameBrowse, SIGNAL( clicked() ), SLOT( slotImageFileBrowseClicked() ) );
   connect( m_pImageFileNameEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotImageFileNameChanged( const QString& ) ) );
   connect( m_pOnceEdit, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMMaterialMapEdit::displayObject( PMObject* o )
{
   bool readOnly;

   if( o->isA( "MaterialMap" ) )
   {
      m_pDisplayedObject = ( PMMaterialMap* ) o;
      readOnly = m_pDisplayedObject->isReadOnly();

      switch( m_pDisplayedObject->bitmapType() )
      {
         case PMMaterialMap::BitmapGif:
            m_pImageFileTypeEdit->setCurrentIndex( 0 );
            break;
         case PMMaterialMap::BitmapTga:
            m_pImageFileTypeEdit->setCurrentIndex( 1 );
            break;
         case PMMaterialMap::BitmapIff:
            m_pImageFileTypeEdit->setCurrentIndex( 2 );
            break;
         case PMMaterialMap::BitmapPpm:
            m_pImageFileTypeEdit->setCurrentIndex( 3 );
            break;
         case PMMaterialMap::BitmapPgm:
            m_pImageFileTypeEdit->setCurrentIndex( 4 );
            break;
         case PMMaterialMap::BitmapPng:
            m_pImageFileTypeEdit->setCurrentIndex( 5 );
            break;
         case PMMaterialMap::BitmapJpeg:
            m_pImageFileTypeEdit->setCurrentIndex( 6 );
            break;
         case PMMaterialMap::BitmapTiff:
            m_pImageFileTypeEdit->setCurrentIndex( 7 );
            break;
         case PMMaterialMap::BitmapSys:
            m_pImageFileTypeEdit->setCurrentIndex( 8 );
            break;
      }
      m_pImageFileTypeEdit->setEnabled( !readOnly );

      switch( m_pDisplayedObject->interpolateType() )
      {
         case PMMaterialMap::InterpolateNone:
            m_pInterpolateTypeEdit->setCurrentIndex( 0 );
            break;
         case PMMaterialMap::InterpolateBilinear:
            m_pInterpolateTypeEdit->setCurrentIndex( 1);
            break;
         case PMMaterialMap::InterpolateNormalized:
            m_pInterpolateTypeEdit->setCurrentIndex( 2 );
            break;
      }
      m_pInterpolateTypeEdit->setEnabled( !readOnly );

      switch( m_pDisplayedObject->mapType() )
      {
         case PMMaterialMap::MapPlanar:
            m_pMapTypeEdit->setCurrentIndex( 0 );
            break;
         case PMMaterialMap::MapSpherical:
            m_pMapTypeEdit->setCurrentIndex( 1 );
            break;
         case PMMaterialMap::MapCylindrical:
            m_pMapTypeEdit->setCurrentIndex( 2 );
            break;
         case PMMaterialMap::MapToroidal:
            m_pMapTypeEdit->setCurrentIndex( 3 );
            break;
      }
      m_pMapTypeEdit->setEnabled( !readOnly );

      m_pImageFileNameEdit->setText( m_pDisplayedObject->bitmapFile() );
      m_pImageFileNameEdit->setEnabled( !readOnly );
      m_pOnceEdit->setChecked( m_pDisplayedObject->isOnceEnabled() );
      m_pOnceEdit->setEnabled( !readOnly );

      Base::displayObject( o );
   }
    
}

void PMMaterialMapEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      switch( m_pImageFileTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapGif );
            break;
         case 1:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapTga );
            break;
         case 2:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapIff );
            break;
         case 3:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapPpm );
            break;
         case 4:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapPgm );
            break;
         case 5:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapPng );
            break;
         case 6:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapJpeg );
            break;
         case 7:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapTiff );
            break;
         case 8:
            m_pDisplayedObject->setBitmapType( PMMaterialMap::BitmapSys );
            break;
      }

      switch( m_pInterpolateTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setInterpolateType( PMMaterialMap::InterpolateNone );
            break;
         case 1:
            m_pDisplayedObject->setInterpolateType( PMMaterialMap::InterpolateBilinear );
            break;
         case 2:
            m_pDisplayedObject->setInterpolateType( PMMaterialMap::InterpolateNormalized );
            break;
      }

      switch( m_pMapTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setMapType( PMMaterialMap::MapPlanar );
            break;
         case 1:
            m_pDisplayedObject->setMapType( PMMaterialMap::MapSpherical );
            break;
         case 2:
            m_pDisplayedObject->setMapType( PMMaterialMap::MapCylindrical );
            break;
         case 3:
            m_pDisplayedObject->setMapType( PMMaterialMap::MapToroidal );
            break;
      }

      m_pDisplayedObject->setBitmapFileName( m_pImageFileNameEdit->text() );
      m_pDisplayedObject->enableOnce( m_pOnceEdit->isChecked() );
   }
}

bool PMMaterialMapEdit::isDataValid()
{
   return Base::isDataValid();
}

void PMMaterialMapEdit::slotInterpolateTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMMaterialMapEdit::slotImageFileTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMMaterialMapEdit::slotMapTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMMaterialMapEdit::slotImageFileNameChanged( const QString& /*a*/ )
{
   emit dataChanged();
}

void PMMaterialMapEdit::slotImageFileBrowseClicked()
{
   QString str = QFileDialog::getOpenFileName(0, QString(),  QString());

   if( !str.isEmpty() )
   {
      m_pImageFileNameEdit->setText( str );
      emit dataChanged();
   }
}

#include "pmmaterialmapedit.moc"
