/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
    email                : zehender@kde.org
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmobjectlibrarysettings.h"

#include "pmlibrarymanager.h"
#include "pmlibraryhandle.h"
#include "pmlibraryhandleedit.h"
#include "pmdebug.h"

#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLocale>
#include <QMessageBox>

#include <QListWidget>
#include <QStandardPaths>

PMObjectLibrarySettings::PMObjectLibrarySettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QVBoxLayout* vlayout = new QVBoxLayout( this );
//TODO PORT QT5    vlayout->setSpacing( QDialog::spacingHint() );
   vlayout->setMargin( 0 );

   QGroupBox* gb = new QGroupBox( tr( "Libraries" ), this );
   vlayout->addWidget( gb );

   QHBoxLayout* hlayout = new QHBoxLayout( gb );
//TODO PORT QT5    hlayout->setSpacing( QDialog::spacingHint() );
//TODO PORT QT5    hlayout->setMargin( QDialog::marginHint() + 5 );
   m_pObjectLibraries = new QListWidget( gb );
   connect( m_pObjectLibraries, SIGNAL( selectionChanged() ), SLOT( slotObjectLibraryChanged() ) );
   hlayout->addWidget( m_pObjectLibraries );
   QVBoxLayout* gvl = new QVBoxLayout( this );
   hlayout->addItem( gvl );
   m_pCreateObjectLibrary = new QPushButton( tr( "Create..." ), gb );
   connect( m_pCreateObjectLibrary, SIGNAL( clicked() ), SLOT( slotCreateObjectLibrary() ) );
   gvl->addWidget( m_pCreateObjectLibrary );
   m_pRemoveObjectLibrary = new QPushButton( tr( "Remove" ), gb );
   connect( m_pRemoveObjectLibrary, SIGNAL( clicked() ), SLOT( slotRemoveObjectLibrary() ) );
   gvl->addWidget( m_pRemoveObjectLibrary );
   m_pImportObjectLibrary = new QPushButton( tr( "Import" ), gb );
   connect( m_pImportObjectLibrary, SIGNAL( clicked() ), SLOT( slotImportObjectLibrary() ) );
   gvl->addWidget( m_pImportObjectLibrary );
   m_pModifyObjectLibrary = new QPushButton( tr( "Properties" ), gb );
   connect( m_pModifyObjectLibrary, SIGNAL( clicked() ), SLOT( slotModifyObjectLibrary() ) );
   gvl->addWidget( m_pModifyObjectLibrary );
   gvl->addStretch( 1 );

   gb = new QGroupBox( tr( "Library Details" ), this );
   QGridLayout* grid = new QGridLayout( gb );
//TODO PORT QT5    grid->setSpacing( QDialog::spacingHint() );
//TODO PORT QT5    grid->setMargin( QDialog::marginHint() + 5 );
   QLabel* lbl = new QLabel( tr( "Path" ), gb );
   grid->addWidget( lbl, 0, 0 );
   lbl = new QLabel( tr( "Author" ), gb );
   grid->addWidget( lbl, 1, 0 );
   lbl = new QLabel( tr( "Description" ), gb );
   grid->addWidget( lbl, 2, 0 );
   m_pLibraryPath = new QLabel( "", gb );
   m_pLibraryAuthor = new QLabel( "", gb );
   m_pLibraryDescription = new QLabel( "", gb );
   m_pReadOnlyText = new QLabel( "", gb );
   grid->addWidget( m_pLibraryPath, 0, 1 );
   grid->addWidget( m_pLibraryAuthor, 1, 1 );
   grid->addWidget( m_pLibraryDescription, 2, 1 );
   //grid->addMultiCellWidget( m_pReadOnlyText, 3, 3, 0, 1 );
   grid->addWidget( m_pReadOnlyText, 3, 0, 1, 2);
   grid->setColumnStretch( 1, 1 );//eticre Coloumn
   vlayout->addWidget( gb );

   vlayout->addStretch( 1 );
}

void PMObjectLibrarySettings::displaySettings()
{
   m_pObjectLibraries->clear();
   m_pObjectLibraries->addItems( PMLibraryManager::theManager()->availableLibraries() );
}

void PMObjectLibrarySettings::applySettings()
{
}

bool PMObjectLibrarySettings::validateData()
{
   return true;
}

void PMObjectLibrarySettings::displayDefaults()
{
}

void PMObjectLibrarySettings::slotImportObjectLibrary()
{
   // TODO
}

void PMObjectLibrarySettings::slotModifyObjectLibrary()
{
   PMLibraryHandle* h = PMLibraryManager::theManager()->getLibraryHandle( m_pObjectLibraries->currentItem()->text() );
   if( h )
   {
      PMLibraryHandleEdit h_dlg( h, this );

      if( h_dlg.exec() == QDialog::Accepted )
      {
         h->saveLibraryInfo();
         displaySettings();
         for( int i = 0; i < m_pObjectLibraries->count(); ++i )
         {
            if( m_pObjectLibraries->item( i )->text() == h->name() )
            {
               m_pObjectLibraries->item( i )->setSelected( true );
               break;
            }
         }
         slotObjectLibraryChanged();
      }
   }
}

void PMObjectLibrarySettings::slotRemoveObjectLibrary()
{
   m_pObjectLibraries->takeItem(
      m_pObjectLibraries->row( m_pObjectLibraries->currentItem() ) );
}

void PMObjectLibrarySettings::slotCreateObjectLibrary()
{
   PMLibraryHandle h;
   PMLibraryHandleEdit h_dlg( &h, this );
   QString libfilename;

   if( h_dlg.exec() == QDialog::Accepted )
   {
      libfilename = h.name();
      //h.setPath( KStandardDirs::locateLocal( "appdata", "library/" ) + libfilename.trimmed() + "/" );
      h.setPath( QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QLatin1Char('/') + "library/" + libfilename.trimmed() + "/" );
      // Create the new library
      switch( h.createLibrary() )
      {
         case PMLibraryHandle::Ok:
            PMLibraryManager::theManager()->refresh();
            displaySettings();
            break;
         case PMLibraryHandle::ExistingDir:
            QMessageBox::information( this, tr( "warning" ) ,
                         tr( "The folder already exists." ),
                         QMessageBox::Ok, QMessageBox::NoButton);
            break;
         case PMLibraryHandle::CouldNotCreateDir:
            QMessageBox::information( this, tr( "warning" ) ,
                       tr( "Could not create the folder." ),
                       QMessageBox::Ok, QMessageBox::NoButton);
            break;
         default:
            qCritical() << "Unexpected error in slotCreateObjectLibrary." << Qt::endl;
      }
   }
}

void PMObjectLibrarySettings::slotObjectLibraryChanged()
{
   PMLibraryHandle* h = PMLibraryManager::theManager()->getLibraryHandle(
               m_pObjectLibraries->currentItem()->text() );
   if( h )
   {
      m_pLibraryPath->setText( h->path() );
      m_pLibraryAuthor->setText( h->author() );
      m_pLibraryDescription->setText( h->description() );
      if( h->isReadOnly() )
         m_pReadOnlyText->setText( tr( "This library is not modifiable." ) );
      else
         m_pReadOnlyText->setText( tr( "This library is modifiable." ) );
   }
   else
   {
      m_pLibraryPath->setText( "" );
      m_pLibraryAuthor->setText( "" );
      m_pLibraryDescription->setText( "" );
      m_pReadOnlyText->setText( "" );
   }
}

#include "pmobjectlibrarysettings.moc"
