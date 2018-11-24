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


#include "pmlibrarybrowser.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qsplitter.h>
#include <qtimer.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>

#include <kio/job.h>
#include <kurl.h>
#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kdialog.h>
#include <kdiroperator.h>
#include <kiconloader.h>
#include <kinputdialog.h>

#include <stdlib.h>

#include "pmlineedits.h"
#include "pmdialogeditbase.h"
#include "pmlibrarymanager.h"
#include "pmlibraryhandle.h"
#include "pmlibraryobject.h"
#include "pmlibraryiconview.h"
#include "pmlibraryentrypreview.h"

// ************** PMLibraryBrowser **************************

PMLibraryBrowserView::PMLibraryBrowserView( PMPart* /*part*/, QWidget* parent /*= 0*/,
                                    const char* name /*=0*/ )
      : PMViewBase( parent )
{
   QHBoxLayout* hl = new QHBoxLayout( this );
   PMLibraryBrowserViewWidget* tv = new PMLibraryBrowserViewWidget( this );
   hl->addWidget( tv );
}

QString PMLibraryBrowserView::description( ) const
{
   return i18n( "Library View" );
}

PMLibraryBrowserViewWidget::PMLibraryBrowserViewWidget( QWidget* parent )
      : QWidget( parent, name )
{
   setCaption( i18n( "Library Objects" ) );

   QVBoxLayout* vl = new QVBoxLayout( this );
   vl->setSpacing( KDialog::spacingHint( ) );

   QHBoxLayout* hl = new QHBoxLayout( vl );

   m_pUpButton = new QPushButton( this );
   m_pUpButton->setPixmap( SmallIcon( "go-up" ) );
   m_pNewSubLibraryButton = new QPushButton( this );
   m_pNewSubLibraryButton->setPixmap( SmallIcon( "folder-new" ) );
   m_pNewObjectButton = new QPushButton( this );
   m_pNewObjectButton->setPixmap( SmallIcon( "document-new" ) );
   m_pDeleteObjectButton = new QPushButton( this );
   m_pDeleteObjectButton->setPixmap( SmallIcon( "edit-delete" ) );
   QLabel* lbl = new QLabel( i18n( "Library: " ), this );
   m_pLibraryComboBox = new QComboBox( this );
   m_pLibraryComboBox->insertStringList( PMLibraryManager::theManager( )->availableLibraries( ) );
   m_pLibraryComboBox->setDuplicatesEnabled( false );
   m_pLibraryComboBox->setCurrentItem( 0 );
   hl->addWidget( m_pUpButton );
   hl->addWidget( m_pNewSubLibraryButton );
   hl->addWidget( m_pNewObjectButton );
   hl->addWidget( m_pDeleteObjectButton );
   hl->addWidget( lbl );
   hl->addWidget( m_pLibraryComboBox );
   hl->addStretch( 1 );

   QSplitter *splitv = new QSplitter( this );
   m_pLibraryIconView = new PMLibraryIconView( splitv );
   m_pLibraryIconView->setMinimumSize( PMDialogEditBase::previewSize( )+20, PMDialogEditBase::previewSize( ) );
   m_pLibraryEntryPreview = new PMLibraryEntryPreview( splitv );
   vl->addWidget( splitv, 99 );

   // Connect all the objects
   connect( m_pUpButton, SIGNAL( clicked( ) ), SLOT( slotUpButtonClicked( ) ) );
   connect( m_pNewSubLibraryButton, SIGNAL( clicked( ) ), SLOT( slotNewSubLibraryClicked( ) ) );
   connect( m_pNewObjectButton,  SIGNAL( clicked( ) ), SLOT( slotNewObjectClicked( ) ) );
   connect( m_pDeleteObjectButton, SIGNAL( clicked( ) ), SLOT( slotDeleteClicked( ) ) );
   connect( m_pLibraryComboBox, SIGNAL( highlighted( const QString& ) ),
                                SLOT( slotPathSelected( const QString& ) ) );
   connect( m_pLibraryIconView, SIGNAL( selectionChanged( Q3IconViewItem* ) ),
                                SLOT( slotSelectionChanged( Q3IconViewItem* ) ) );
   connect( m_pLibraryIconView, SIGNAL( executed( Q3IconViewItem* ) ),
                                SLOT( slotSelectionExecuted( Q3IconViewItem* ) ) );
   connect( m_pLibraryEntryPreview, SIGNAL( objectChanged( ) ), SLOT( slotIconViewRefresh( ) ) );

   // Set the selected library
   slotPathSelected( m_pLibraryComboBox->currentText( ) );
}

void PMLibraryBrowserViewWidget::resizeEvent( QResizeEvent* /*ev*/ )
{
}

void PMLibraryBrowserViewWidget::slotPathSelected( const QString& str )
{
   PMLibraryHandle* h = PMLibraryManager::theManager( )->getLibraryHandle( str );
   if( h )
   {
      m_pLibraryIconView->setLibrary( h );
      m_pCurrentLibrary = h;
      // This can never be a sub library
      m_pUpButton->setEnabled( false );
      // If the top library is read-only so is everthing below it
      m_topLibraryReadOnly = m_pCurrentLibrary->isReadOnly( );
      m_pNewSubLibraryButton->setEnabled( !m_topLibraryReadOnly );
      m_pNewObjectButton->setEnabled( !m_topLibraryReadOnly );
      m_pDeleteObjectButton->setEnabled( !m_topLibraryReadOnly );
   }
   else
   {
      m_pUpButton->setEnabled( false );
      m_pNewSubLibraryButton->setEnabled( false );
      m_pNewObjectButton->setEnabled( false );
      m_pDeleteObjectButton->setEnabled( false );
   }
}

void PMLibraryBrowserViewWidget::slotIconViewRefresh( )
{
   m_pLibraryIconView->refresh( );
}

void PMLibraryBrowserViewWidget::slotIconViewSetLibrary( )
{
   m_pLibraryIconView->setLibrary( m_pFutureLibrary );
   m_pCurrentLibrary = m_pFutureLibrary;
   m_pFutureLibrary = NULL;
}

void PMLibraryBrowserViewWidget::slotSelectionChanged( Q3IconViewItem* item )
{
   PMLibraryIconViewItem* sel = static_cast<PMLibraryIconViewItem*>( item );
   m_pLibraryIconView->setCurrentItem( sel );
   if( sel->isSubLibrary( ) )
   {
      if( m_pLibraryEntryPreview->saveIfNeeded( ) )
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh( ) ) );
      else
         m_pLibraryEntryPreview->showPreview( sel->path( ), m_topLibraryReadOnly, true );
   }
   else
   {
      if( m_pLibraryEntryPreview->saveIfNeeded( ) )
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh( ) ) );
      else
         m_pLibraryEntryPreview->showPreview( sel->path( ), m_topLibraryReadOnly, false );
   }
   m_pLibraryIconView->setFocus();
}

void PMLibraryBrowserViewWidget::slotSelectionExecuted( Q3IconViewItem* item )
{
   PMLibraryIconViewItem* sel = static_cast<PMLibraryIconViewItem*>( item );
   m_pLibraryIconView->setCurrentItem( sel );
   if( sel->isSubLibrary( ) )
   {
      // It's a sub library
      m_pFutureLibrary = new PMLibraryHandle( sel->path( ) );
      m_pLibraryEntryPreview->clearPreview( );
      QTimer::singleShot( 100, this, SLOT( slotIconViewSetLibrary( ) ) );
      m_pUpButton->setEnabled( true );
   }
   else
   {
      // It's an object
      if( m_pLibraryEntryPreview->saveIfNeeded( ) )
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh( ) ) );
      else
         m_pLibraryEntryPreview->showPreview( sel->path( ), m_topLibraryReadOnly, false );
   }
   m_pLibraryIconView->setFocus();
}

void PMLibraryBrowserViewWidget::slotUpButtonClicked( )
{
  QDir pathManipulator( m_pCurrentLibrary->path( ) );

  pathManipulator.cdUp( );
  m_pFutureLibrary = new PMLibraryHandle( pathManipulator.path( ) );
  if( !m_pFutureLibrary->isSubLibrary( ) )
     m_pUpButton->setEnabled( false );
  QTimer::singleShot( 100, this, SLOT( slotIconViewSetLibrary( ) ) );

  // Release the current Library
  delete m_pCurrentLibrary;
}

void PMLibraryBrowserViewWidget::slotDeleteClicked( )
{
   PMLibraryIconViewItem* sel = static_cast<PMLibraryIconViewItem*>( m_pLibraryIconView->currentItem( ) );
   PMLibraryHandle::PMResult rst;
   if( !sel )
      return;
   else if( sel->isSubLibrary( ) )
      rst = m_pCurrentLibrary->deleteSubLibrary( sel->path( ) );
   else
      rst = m_pCurrentLibrary->deleteObject( sel->path( ) );

   switch( rst )
   {
      case PMLibraryHandle::Ok:
      {
         KIO::Job *job = KIO::del( sel->path() );
         connect( job, SIGNAL( result( KJob * ) ), SLOT( slotJobResult( KJob * ) ) );
      }
      break;
      case PMLibraryHandle::ReadOnlyLib:
         KMessageBox::error( this, i18n( "This library is read only." ), i18n( "Error" ) );
         break;
      case PMLibraryHandle::NotInLib:
         KMessageBox::error( this, i18n( "The current library does not contain that item." ), i18n( "Error" ) );
         break;
      default:
         KMessageBox::error( this, i18n( "Could not remove item." ), i18n( "Error" ) );
         break;
   }
}

void PMLibraryBrowserViewWidget::slotNewObjectClicked( )
{
   m_pLibraryEntryPreview->saveIfNeeded( );

   switch( m_pCurrentLibrary->createNewObject( ) )
   {
      case PMLibraryHandle::Ok:
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh( ) ) );
         break;
      case PMLibraryHandle::ReadOnlyLib:
         KMessageBox::error( this, i18n( "This library is read only." ), i18n( "Error" ) );
         break;
      default:
         KMessageBox::error( this, i18n( "Could not create a new object." ), i18n( "Error" ) );
   }
}

void PMLibraryBrowserViewWidget::slotNewSubLibraryClicked( )
{
   bool result = false;

   m_pLibraryEntryPreview->saveIfNeeded( );
   QString subLibraryName = KInputDialog::getText( i18n( "Create Sub-Library" ),
                                                   i18n( "Enter the sub-library name: " ),
                                                   i18n( "Unknown" ),
                                                   &result );

   if( result )
   {
      switch( m_pCurrentLibrary->createNewSubLibrary( subLibraryName ) )
      {
         case PMLibraryHandle::Ok:
            m_pLibraryIconView->refresh( );
            break;
         case PMLibraryHandle::ExistingDir:
            KMessageBox::error( this, i18n( "That library already exists." ), i18n( "Error" ) );
            break;
         case PMLibraryHandle::ReadOnlyLib:
            KMessageBox::error( this, i18n( "This library is read only." ), i18n( "Error" ) );
            break;
         default:
            KMessageBox::error( this, i18n( "Could not create a new sub library." ), i18n( "Error" ) );
      }
   }
}

void PMLibraryBrowserViewWidget::slotJobResult( KJob * job )
{
   if( job->error( ) )
      static_cast<KIO::Job*>(job)->showErrorDialog( this );
   QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh( ) ) );
}

QString PMLibraryBrowserViewFactory::description( ) const
{
   return i18n( "Library View" );
}

#include "pmlibrarybrowser.moc"
