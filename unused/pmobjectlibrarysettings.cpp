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

#include <qlayout.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <klocale.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <Q3ListBox>

PMObjectLibrarySettings::PMObjectLibrarySettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QVBoxLayout* vlayout = new QVBoxLayout( this );
   vlayout->setSpacing( KDialog::spacingHint( ) );
   vlayout->setMargin( 0 );

   Q3GroupBox* gb = new Q3GroupBox( i18n( "Libraries" ), this );
   vlayout->addWidget( gb );

   QHBoxLayout* hlayout = new QHBoxLayout( gb );
   hlayout->setSpacing( KDialog::spacingHint( ) );
   hlayout->setMargin( KDialog::marginHint( ) + 5 );
   m_pObjectLibraries = new Q3ListBox( gb );
   connect( m_pObjectLibraries, SIGNAL( selectionChanged( ) ), SLOT( slotObjectLibraryChanged( ) ) );
   hlayout->addWidget( m_pObjectLibraries );
   QVBoxLayout* gvl = new QVBoxLayout( hlayout );
   m_pCreateObjectLibrary = new QPushButton( i18n( "Create..." ), gb );
   connect( m_pCreateObjectLibrary, SIGNAL( clicked( ) ), SLOT( slotCreateObjectLibrary( ) ) );
   gvl->addWidget( m_pCreateObjectLibrary );
   m_pRemoveObjectLibrary = new QPushButton( i18n( "Remove" ), gb );
   connect( m_pRemoveObjectLibrary, SIGNAL( clicked( ) ), SLOT( slotRemoveObjectLibrary( ) ) );
   gvl->addWidget( m_pRemoveObjectLibrary );
   m_pImportObjectLibrary = new QPushButton( i18n( "Import" ), gb );
   connect( m_pImportObjectLibrary, SIGNAL( clicked( ) ), SLOT( slotImportObjectLibrary( ) ) );
   gvl->addWidget( m_pImportObjectLibrary );
   m_pModifyObjectLibrary = new QPushButton( i18n( "Properties" ), gb );
   connect( m_pModifyObjectLibrary, SIGNAL( clicked( ) ), SLOT( slotModifyObjectLibrary( ) ) );
   gvl->addWidget( m_pModifyObjectLibrary );
   gvl->addStretch( 1 );

   gb = new Q3GroupBox( i18n( "Library Details" ), this );
   QGridLayout* grid = new QGridLayout( gb );
   grid->setSpacing( KDialog::spacingHint( ) );
   grid->setMargin( KDialog::marginHint( ) + 5 );
   QLabel* lbl = new QLabel( i18n( "Path" ), gb );
   grid->addWidget( lbl, 0, 0 );
   lbl = new QLabel( i18n( "Author" ), gb );
   grid->addWidget( lbl, 1, 0 );
   lbl = new QLabel( i18n( "Description" ), gb );
   grid->addWidget( lbl, 2, 0 );
   m_pLibraryPath = new QLabel( "", gb );
   m_pLibraryAuthor = new QLabel( "", gb );
   m_pLibraryDescription = new QLabel( "", gb );
   m_pReadOnlyText = new QLabel( "", gb );
   grid->addWidget( m_pLibraryPath, 0, 1 );
   grid->addWidget( m_pLibraryAuthor, 1, 1 );
   grid->addWidget( m_pLibraryDescription, 2, 1 );
   grid->addMultiCellWidget( m_pReadOnlyText, 3, 3, 0, 1 );
   grid->setColoumnStretch( 1, 1 );
   vlayout->addWidget( gb );

   vlayout->addStretch( 1 );
}

void PMObjectLibrarySettings::displaySettings( )
{
   m_pObjectLibraries->clear( );
   m_pObjectLibraries->insertStringList( PMLibraryManager::theManager( )->availableLibraries() );
}

void PMObjectLibrarySettings::applySettings( )
{
}

bool PMObjectLibrarySettings::validateData( )
{
   return true;
}

void PMObjectLibrarySettings::displayDefaults( )
{
}

void PMObjectLibrarySettings::slotImportObjectLibrary( )
{
   // TODO
}

void PMObjectLibrarySettings::slotModifyObjectLibrary( )
{
   PMLibraryHandle* h = PMLibraryManager::theManager( )->getLibraryHandle( m_pObjectLibraries->currentText( ) );
   if( h )
   {
      PMLibraryHandleEdit h_dlg( h, this );

      if( h_dlg.exec( ) == QDialog::Accepted )
      {
         h->saveLibraryInfo( );
         displaySettings( );
         for( unsigned i = 0; i < m_pObjectLibraries->count( ); ++i )
         {
            if( m_pObjectLibraries->text( i ) == h->name( ) )
            {
               m_pObjectLibraries->setSelected( i, true );
               break;
            }
         }
         slotObjectLibraryChanged( );
      }
   }
}

void PMObjectLibrarySettings::slotRemoveObjectLibrary( )
{
   m_pObjectLibraries->removeItem( m_pObjectLibraries->currentItem( ) );
}

void PMObjectLibrarySettings::slotCreateObjectLibrary( )
{
   PMLibraryHandle h;
   PMLibraryHandleEdit h_dlg( &h, this );
   QString libfilename;

   if( h_dlg.exec( ) == QDialog::Accepted )
   {
      libfilename = h.name( );
      h.setPath( locateLocal( "appdata", "library/" ) + libfilename.trimmed( ) + "/" );
      // Create the new library
      switch( h.createLibrary( ) )
      {
         case PMLibraryHandle::Ok:
            PMLibraryManager::theManager( )->refresh( );
            displaySettings( );
            break;
         case PMLibraryHandle::ExistingDir:
            KMessageBox::error( this, i18n( "The folder already exists." ) );
            break;
         case PMLibraryHandle::CouldNotCreateDir:
            KMessageBox::error( this, i18n( "Could not create the folder." ) );
            break;
         default:
            kError( PMArea ) << "Unexpected error in slotCreateObjectLibrary." << endl;
      }
   }
}

void PMObjectLibrarySettings::slotObjectLibraryChanged( )
{
   PMLibraryHandle* h = PMLibraryManager::theManager( )->getLibraryHandle( m_pObjectLibraries->currentText( ) );
   if( h )
   {
      m_pLibraryPath->setText( h->path( ) );
      m_pLibraryAuthor->setText( h->author( ) );
      m_pLibraryDescription->setText( h->description( ) );
      if( h->isReadOnly( ) )
         m_pReadOnlyText->setText( i18n( "This library is not modifiable." ) );
      else
         m_pReadOnlyText->setText( i18n( "This library is modifiable." ) );
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
