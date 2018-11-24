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


#include "pmimagemapedit.h"
#include "pmimagemap.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmpalettevalueedit.h"
#include "pmvector.h"

#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <qlabel.h>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QList>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>

PMImageMapEdit::PMImageMapEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMImageMapEdit::createTopWidgets()
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

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEnableFilterAllEdit = new QCheckBox( tr( "Filter all" ), this );
   m_pFilterAllEdit = new PMFloatEdit( this );
   hl->addWidget( m_pEnableFilterAllEdit );
   hl->addWidget( m_pFilterAllEdit );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEnableTransmitAllEdit = new QCheckBox( tr( "Transmit all" ), this );
   m_pTransmitAllEdit = new PMFloatEdit( this );
   hl->addWidget( m_pEnableTransmitAllEdit );
   hl->addWidget( m_pTransmitAllEdit );
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

   connect( m_pImageFileTypeEdit, SIGNAL( activated( int ) ), SLOT( slotImageFileTypeChanged( int ) ) );
   connect( m_pMapTypeEdit, SIGNAL( activated( int ) ), SLOT( slotMapTypeChanged( int ) ) );
   connect( m_pInterpolateTypeEdit, SIGNAL( activated( int ) ), SLOT( slotInterpolateTypeChanged( int ) ) );
   connect( m_pImageFileNameBrowse, SIGNAL( clicked() ), SLOT( slotImageFileBrowseClicked() ) );
   connect( m_pImageFileNameEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotImageFileNameChanged( const QString& ) ) );
   connect( m_pEnableFilterAllEdit, SIGNAL( clicked() ), SLOT( slotFilterAllClicked() ) );
   connect( m_pEnableTransmitAllEdit, SIGNAL( clicked() ), SLOT( slotTransmitAllClicked() ) );
   connect( m_pFilterAllEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pTransmitAllEdit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pOnceEdit, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMImageMapEdit::createBottomWidgets()
{
   topLayout()->addWidget( new QLabel( tr( "Indexed filters" ), this ) );
   m_pFiltersWidget = new QWidget( this );
   topLayout()->addWidget( m_pFiltersWidget );
   topLayout()->addWidget( new QLabel( tr( "Indexed transmits" ), this ) );
   m_pTransmitsWidget = new QWidget( this );
   topLayout()->addWidget( m_pTransmitsWidget );
   
   
   Base::createBottomWidgets();
}

void PMImageMapEdit::displayObject( PMObject* o )
{
   bool readOnly;

   if( o->isA( "ImageMap" ) )
   {
      m_pDisplayedObject = ( PMImageMap* ) o;
      readOnly = m_pDisplayedObject->isReadOnly();

      switch( m_pDisplayedObject->bitmapType() )
      {
         case PMImageMap::BitmapGif:
            m_pImageFileTypeEdit->setCurrentIndex( 0 );
            break;
         case PMImageMap::BitmapTga:
            m_pImageFileTypeEdit->setCurrentIndex( 1 );
            break;
         case PMImageMap::BitmapIff:
            m_pImageFileTypeEdit->setCurrentIndex( 2 );
            break;
         case PMImageMap::BitmapPpm:
            m_pImageFileTypeEdit->setCurrentIndex( 3 );
            break;
         case PMImageMap::BitmapPgm:
            m_pImageFileTypeEdit->setCurrentIndex( 4 );
            break;
         case PMImageMap::BitmapPng:
            m_pImageFileTypeEdit->setCurrentIndex( 5 );
            break;
         case PMImageMap::BitmapJpeg:
            m_pImageFileTypeEdit->setCurrentIndex( 6 );
            break;
         case PMImageMap::BitmapTiff:
            m_pImageFileTypeEdit->setCurrentIndex( 7 );
            break;
         case PMImageMap::BitmapSys:
            m_pImageFileTypeEdit->setCurrentIndex( 8 );
            break;
      }
      m_pImageFileTypeEdit->setEnabled( !readOnly );

      switch( m_pDisplayedObject->interpolateType() )
      {
         case PMImageMap::InterpolateNone:
            m_pInterpolateTypeEdit->setCurrentIndex( 0 );
            break;
         case PMImageMap::InterpolateBilinear:
            m_pInterpolateTypeEdit->setCurrentIndex( 1);
            break;
         case PMImageMap::InterpolateNormalized:
            m_pInterpolateTypeEdit->setCurrentIndex( 2 );
            break;
      }
      m_pInterpolateTypeEdit->setEnabled( !readOnly );

      switch( m_pDisplayedObject->mapType() )
      {
         case PMImageMap::MapPlanar:
            m_pMapTypeEdit->setCurrentIndex( 0 );
            break;
         case PMImageMap::MapSpherical:
            m_pMapTypeEdit->setCurrentIndex( 1 );
            break;
         case PMImageMap::MapCylindrical:
            m_pMapTypeEdit->setCurrentIndex( 2 );
            break;
         case PMImageMap::MapToroidal:
            m_pMapTypeEdit->setCurrentIndex( 3 );
            break;
      }
      m_pMapTypeEdit->setEnabled( !readOnly );

      m_pImageFileNameEdit->setText( m_pDisplayedObject->bitmapFile() );
      m_pImageFileNameEdit->setEnabled( !readOnly );
      m_pOnceEdit->setChecked( m_pDisplayedObject->isOnceEnabled() );
      m_pOnceEdit->setEnabled( !readOnly );
      m_pEnableFilterAllEdit->setChecked( m_pDisplayedObject->isFilterAllEnabled() );
      m_pEnableFilterAllEdit->setEnabled( !readOnly );
      m_pFilterAllEdit->setValue( m_pDisplayedObject->filterAll() );
      m_pFilterAllEdit->setReadOnly( readOnly );
      m_pEnableTransmitAllEdit->setChecked( m_pDisplayedObject->isTransmitAllEnabled() );
      m_pEnableTransmitAllEdit->setEnabled( !readOnly );
      m_pTransmitAllEdit->setValue( m_pDisplayedObject->transmitAll() );
      m_pTransmitAllEdit->setReadOnly( readOnly );

      displayPaletteEntries( m_pDisplayedObject->filters(), m_pDisplayedObject->transmits() );

      slotFilterAllClicked();
      slotTransmitAllClicked();

      Base::displayObject( o );
   }
    
}

void PMImageMapEdit::displayPaletteEntries( const QList<PMPaletteValue>& filters,
                                            const QList<PMPaletteValue>& transmits )
{
   bool readOnly = m_pDisplayedObject->isReadOnly();

   int nfilters = ( int ) filters.count();
   int ntransmits = ( int ) transmits.count();

   int i;
   PMPaletteValueEdit* edit;
   QPushButton* button;
   QGridLayout* gl;
   QIcon addButtonIcon = QIcon( ":/smallicon/icons/povicons/small/pmaddpoint" );
   QIcon removeButtonIcon = QIcon( ":/smallicon/icons/povicons/small/pmremovepoint" );

   // First let's deal with the filter entries...
   if( m_pFiltersWidget->layout() )
      delete m_pFiltersWidget->layout();

   while( !m_filterEntries.isEmpty() )
      delete m_filterEntries.takeFirst();
   while( !m_filterAddButtons.isEmpty() )
      delete m_filterAddButtons.takeFirst();
   while( !m_filterRemoveButtons.isEmpty() )
      delete m_filterRemoveButtons.takeFirst();

   gl = new QGridLayout( m_pFiltersWidget );
//TODO PORT QT5    gl->setSpacing( QDialog::spacingHint() );
   gl->setMargin( 0 );

   button = new QPushButton( m_pFiltersWidget );
   button->setIcon( addButtonIcon );
   m_filterAddButtons.append( button );
   connect( button, SIGNAL( clicked() ), SLOT( slotAddFilterEntry() ) );
   gl->addWidget( button, 0, 1 );
   button->show();
   button->setEnabled( !readOnly );
   button->setToolTip( tr( "Add new filter" ) );

   for( i = 0; i < nfilters; i ++ )
   {
      edit = new PMPaletteValueEdit( m_pFiltersWidget );
      m_filterEntries.append( edit );
      connect( edit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
      gl->addWidget( edit, i+1, 0 );
      edit->setIndex( filters[ i ].index() );
      edit->setValue( filters[ i ].value() );
      edit->show();
      edit->setReadOnly( readOnly );

      button = new QPushButton( m_pFiltersWidget );
      button->setIcon( addButtonIcon );
      m_filterAddButtons.append( button );
      connect( button, SIGNAL( clicked() ), SLOT( slotAddFilterEntry() ) );
      gl->addWidget( button, i+1, 1 );
      button->show();
      button->setEnabled( !readOnly );
      button->setToolTip( tr( "Add new filter" ) );

      button = new QPushButton( m_pFiltersWidget );
      button->setIcon( removeButtonIcon );
      m_filterRemoveButtons.append( button );
      connect( button, SIGNAL( clicked() ), SLOT( slotRemoveFilterEntry() ) );
      gl->addWidget( button, i+1, 2 );
      button->show();
      button->setEnabled( !readOnly );
      button->setToolTip( tr( "Remove filter" ) );
   }

   // ...next the transmit entries
   while( !m_transmitEntries.isEmpty() )
      delete m_transmitEntries.takeFirst();
   while( !m_transmitAddButtons.isEmpty() )
      delete m_transmitAddButtons.takeFirst();
   while( !m_transmitRemoveButtons.isEmpty() )
      delete m_transmitRemoveButtons.takeFirst();

   // recreate the entry edits
   if( m_pTransmitsWidget->layout() )
      delete m_pTransmitsWidget->layout();

   gl = new QGridLayout( m_pTransmitsWidget );
//TODO PORT QT5    gl->setSpacing( QDialog::spacingHint() );
   gl->setMargin( 0 );

   button = new QPushButton( m_pTransmitsWidget );
   button->setIcon( addButtonIcon );
   m_transmitAddButtons.append( button );
   connect( button, SIGNAL( clicked() ), SLOT( slotAddTransmitEntry() ) );
   gl->addWidget( button, 0, 1 );
   button->show();
   button->setEnabled( !readOnly );
   button->setToolTip( tr( "Add new transmit" ) );

   for( i = 0; i < ntransmits; i ++ )
   {
      edit = new PMPaletteValueEdit( m_pTransmitsWidget );
      m_transmitEntries.append( edit );
      connect( edit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
      gl->addWidget( edit, i+1, 0 );
      edit->setIndex( transmits[ i ].index() );
      edit->setValue( transmits[ i ].value() );
      edit->show();
      edit->setReadOnly( readOnly );

      button = new QPushButton( m_pTransmitsWidget );
      button->setIcon( addButtonIcon );
      m_transmitAddButtons.append( button );
      connect( button, SIGNAL( clicked() ), SLOT( slotAddTransmitEntry() ) );
      gl->addWidget( button, i+1, 1 );
      button->show();
      button->setEnabled( !readOnly );
      button->setToolTip( tr( "Add new transmit" ) );

      button = new QPushButton( m_pTransmitsWidget );
      button->setIcon( removeButtonIcon );
      m_transmitRemoveButtons.append( button );
      connect( button, SIGNAL( clicked() ), SLOT( slotRemoveTransmitEntry() ) );
      gl->addWidget( button, i+1, 2 );
      button->show();
      button->setEnabled( !readOnly );
      button->setToolTip( tr( "Remove transmit" ) );
   }
}

void PMImageMapEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      switch( m_pImageFileTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapGif );
            break;
         case 1:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapTga );
            break;
         case 2:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapIff );
            break;
         case 3:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapPpm );
            break;
         case 4:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapPgm );
            break;
         case 5:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapPng );
            break;
         case 6:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapJpeg );
            break;
         case 7:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapTiff );
            break;
         case 8:
            m_pDisplayedObject->setBitmapType( PMImageMap::BitmapSys );
            break;
      }

      switch( m_pInterpolateTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setInterpolateType( PMImageMap::InterpolateNone );
            break;
         case 1:
            m_pDisplayedObject->setInterpolateType( PMImageMap::InterpolateBilinear );
            break;
         case 2:
            m_pDisplayedObject->setInterpolateType( PMImageMap::InterpolateNormalized );
            break;
      }

      switch( m_pMapTypeEdit->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setMapType( PMImageMap::MapPlanar );
            break;
         case 1:
            m_pDisplayedObject->setMapType( PMImageMap::MapSpherical );
            break;
         case 2:
            m_pDisplayedObject->setMapType( PMImageMap::MapCylindrical );
            break;
         case 3:
            m_pDisplayedObject->setMapType( PMImageMap::MapToroidal );
            break;
      }

      m_pDisplayedObject->setFilters( filters() );
      m_pDisplayedObject->setTransmits( transmits() );

      m_pDisplayedObject->setBitmapFileName( m_pImageFileNameEdit->text() );
      m_pDisplayedObject->enableFilterAll( m_pEnableFilterAllEdit->isChecked() );
      m_pDisplayedObject->setFilterAll( m_pFilterAllEdit->value() );
      m_pDisplayedObject->enableTransmitAll( m_pEnableTransmitAllEdit->isChecked() );
      m_pDisplayedObject->setTransmitAll( m_pTransmitAllEdit->value() );
      m_pDisplayedObject->enableOnce( m_pOnceEdit->isChecked() );
   }
}

bool PMImageMapEdit::isDataValid()
{
   if( !m_pFilterAllEdit->isDataValid() ) return false;
   if( !m_pTransmitAllEdit->isDataValid() ) return false;

   PMPaletteValueEdit* it;
   foreach( it, m_filterEntries )
      if( !it->isDataValid() )
         return false;
   foreach( it, m_transmitEntries )
      if( !it->isDataValid() )
         return false;
   return Base::isDataValid();
}

void PMImageMapEdit::slotInterpolateTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMImageMapEdit::slotImageFileTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMImageMapEdit::slotMapTypeChanged( const int /*a*/ )
{
   emit dataChanged();
}

void PMImageMapEdit::slotImageFileNameChanged( const QString& /*a*/ )
{
   emit dataChanged();
}

void PMImageMapEdit::slotImageFileBrowseClicked()
{
   QString str = QFileDialog::getOpenFileName(0, QString(),  QString());

   if( !str.isEmpty() )
   {
      m_pImageFileNameEdit->setText( str );
      emit dataChanged();
   }
}

void PMImageMapEdit::slotFilterAllClicked()
{
   if( m_pEnableFilterAllEdit->isChecked() )
      m_pFilterAllEdit->setEnabled( true );
   else
      m_pFilterAllEdit->setEnabled( false );
   emit sizeChanged();
   emit dataChanged();
}

void PMImageMapEdit::slotTransmitAllClicked()
{
   if( m_pEnableTransmitAllEdit->isChecked() )
      m_pTransmitAllEdit->setEnabled( true );
   else
      m_pTransmitAllEdit->setEnabled( false );
   emit sizeChanged();
   emit dataChanged();
}

QList<PMPaletteValue> PMImageMapEdit::filters()
{
   QList<PMPaletteValue> entries;
   foreach( PMPaletteValueEdit* it, m_filterEntries )
      entries.append( PMPaletteValue( it->index(), it->value() ) );

   return entries;
}

QList<PMPaletteValue> PMImageMapEdit::transmits()
{
   QList<PMPaletteValue> entries;
   foreach( PMPaletteValueEdit* it, m_transmitEntries )
      entries.append( PMPaletteValue( it->index(), it->value() ) );

   return entries;
}

void PMImageMapEdit::slotAddFilterEntry()
{
   QList<PMPaletteValue> entriesFilters;
   PMPaletteValue newEntry;
   QPushButton* button = ( QPushButton* ) sender();

   if( button )
   {
      int index = m_filterAddButtons.indexOf( button );
      if( index >= 0 )
      {
         entriesFilters = filters();
         if( index == 0 )
            entriesFilters.prepend( newEntry );
         else
         {
            entriesFilters.insert( index, newEntry );
         }
         displayPaletteEntries( entriesFilters, transmits() );
         emit sizeChanged();
         emit dataChanged();
      }
   }
}

void PMImageMapEdit::slotRemoveFilterEntry()
{
   QList<PMPaletteValue> entriesFilters;
   QPushButton* button = ( QPushButton* ) sender();

   if( button )
   {
      int index = m_filterRemoveButtons.indexOf( button );
      if( index >= 0 )
      {
         entriesFilters = filters();
         entriesFilters.removeAt( index );
         displayPaletteEntries( entriesFilters, transmits() );
         emit sizeChanged();
         emit dataChanged();
      }
   }
}

void PMImageMapEdit::slotAddTransmitEntry()
{
   QList<PMPaletteValue> entriesTransmits;
   PMPaletteValue newEntry;
   QPushButton* button = ( QPushButton* ) sender();

   if( button )
   {
      int index = m_transmitAddButtons.indexOf( button );
      if( index >= 0 )
      {
         entriesTransmits = transmits();
         if( index == 0 )
            entriesTransmits.prepend( newEntry );
         else
         {
            entriesTransmits.insert( index, newEntry );
         }
         displayPaletteEntries( filters(), entriesTransmits );
         emit sizeChanged();
         emit dataChanged();
      }
   }
}

void PMImageMapEdit::slotRemoveTransmitEntry()
{
   QList<PMPaletteValue> entriesTransmits;
   QPushButton* button = ( QPushButton* ) sender();

   if( button )
   {
      int index = m_transmitRemoveButtons.indexOf( button );
      if( index >= 0 )
      {
         entriesTransmits = transmits();
         entriesTransmits.removeAt( index );
         displayPaletteEntries( filters(), entriesTransmits );
         emit sizeChanged();
         emit dataChanged();
      }
   }
}

#include "pmimagemapedit.moc"
