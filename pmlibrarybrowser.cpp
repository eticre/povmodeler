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

#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QSplitter>
#include <QTimer>
#include <QPushButton>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QUrl>
#include <QMessageBox>
#include <QDialog>
#include <QInputDialog>
#include <stdlib.h>

#include "pmlineedits.h"
#include "pmdialogeditbase.h"
#include "pmlibrarymanager.h"
#include "pmlibraryhandle.h"
#include "pmlibraryobject.h"
#include "pmlibraryiconview.h"
#include "pmlibraryentrypreview.h"

//#include "kxmlguiclient.h"
//#include "kaction.h"
//#include "kactioncollection.h"
//#include "kstandardaction.h"
#include "pmcommandmanager.h"
#include "pmactions.h"


// ************** PMLibraryBrowser **************************
//eticre removed char

PMLibraryBrowserView::PMLibraryBrowserView( PMPart*, QWidget* parent )
      : PMViewBase( parent )
{
   QHBoxLayout* hl = new QHBoxLayout( this );
   //QWidget* hl = new QWidget(this);
   PMLibraryBrowserViewWidget* tv = new PMLibraryBrowserViewWidget( this );
   hl->addWidget( tv );
   this->setGeometry( 100, 100, 640, 480 );
      QRect rect( 300, 100, 800, 480 );
   hl->setGeometry( rect );
}

QString PMLibraryBrowserView::description() const
{
   return ( "Library View" );
}

PMLibraryBrowserViewWidget::PMLibraryBrowserViewWidget( QWidget* parent )
      : QWidget( parent )//eticre -,name
{
   setWindowTitle( "Library Objects" );

   QVBoxLayout* vl = new QVBoxLayout( this );
   QHBoxLayout* hl = new QHBoxLayout( this );

   vl->addItem( hl );

   m_pUpButton = new QPushButton( this );
   m_pUpButton->setIcon( QIcon::fromTheme( "go-up" ) );
   m_pNewSubLibraryButton = new QPushButton( this );
   m_pNewSubLibraryButton->setIcon( QIcon::fromTheme( "folder-new" ) );
   m_pNewObjectButton = new QPushButton( this );
   m_pNewObjectButton->setIcon( QIcon::fromTheme( "document-new" ) );
   m_pDeleteObjectButton = new QPushButton( this );
   m_pDeleteObjectButton->setIcon( QIcon::fromTheme( "edit-delete" ) );

   /*hide because todo*/
   //m_pUpButton->setHidden( 0 ); do not set hidden == 0-- widget is now visible and fail dock
   m_pNewSubLibraryButton->setHidden( 1 );
   m_pNewObjectButton->setHidden( 1 );
   m_pDeleteObjectButton->setHidden( 1 );


   QLabel* lbl = new QLabel( "Library: " );
   m_pLibraryComboBox = new QComboBox( this );
   m_pLibraryComboBox->addItems( PMLibraryManager::theManager()->availableLibraries() );
   m_pLibraryComboBox->setDuplicatesEnabled( false );
   m_pLibraryComboBox->setCurrentIndex( 0 );
   hl->addWidget( m_pUpButton );
   hl->addWidget( m_pNewSubLibraryButton );
   hl->addWidget( m_pNewObjectButton );
   hl->addWidget( m_pDeleteObjectButton );
   hl->addWidget( lbl );
   hl->addWidget( m_pLibraryComboBox );
   hl->addStretch( 1 );

   QSplitter *splitv = new QSplitter( this );
   m_pLibraryIconView = new PMLibraryIconView( splitv );
   m_pLibraryIconView->setMinimumSize( PMDialogEditBase::previewSize()+110, PMDialogEditBase::previewSize() );
   m_pLibraryEntryPreview = new PMLibraryEntryPreview( splitv );
   //m_pLibraryEntryPreview->setHidden( 1 );
   vl->addWidget( splitv, 99 );

   // Connect all the objects
   connect( m_pUpButton,
            SIGNAL( clicked() ),
            SLOT( slotUpButtonClicked() ) );

   connect( m_pNewSubLibraryButton,
            SIGNAL( clicked() ),
            SLOT( slotNewSubLibraryClicked() ) );

   connect( m_pNewObjectButton,
            SIGNAL( clicked() ),
            SLOT( slotNewObjectClicked() ) );

   connect( m_pDeleteObjectButton,
            SIGNAL( clicked() ),
            SLOT( slotDeleteClicked() ) );

   connect( m_pLibraryComboBox,
            SIGNAL( highlighted( const QString& ) ),
            SLOT( slotPathSelected( const QString& ) ) );

   connect( m_pLibraryIconView,
            SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ),
            SLOT( slotSelectionChanged( QListWidgetItem* ) ) );

   connect( m_pLibraryIconView,
            SIGNAL( itemClicked( QListWidgetItem* ) ),
            SLOT( slotSelectionExecuted( QListWidgetItem* ) ) );

   connect( m_pLibraryEntryPreview,
            SIGNAL( objectChanged() ),
            SLOT( slotIconViewRefresh() ) );

   // Set the selected library
   slotPathSelected( m_pLibraryComboBox->currentText() );
   //m_pCopyAction = actionCollection(KStandardAction::New, "file_new", this, SLOT(newFile()));
   //m_pCopyAction = KStandardAction::copy( this, SLOT( slotEditCopy() ), actionCollection() );

}

void PMLibraryBrowserViewWidget::resizeEvent( QResizeEvent* /*ev*/ )
{
}

void PMLibraryBrowserViewWidget::slotPathSelected( const QString& str )//eticre debug error 3
{
   PMLibraryHandle* h = PMLibraryManager::theManager()->getLibraryHandle( str );
   if( h )
   {
      m_pLibraryIconView->setLibrary( h );
      m_pCurrentLibrary = h;
      // This can never be a sub library
      m_pUpButton->setEnabled( false );
      // If the top library is read-only so is everthing below it
      m_topLibraryReadOnly = m_pCurrentLibrary->isReadOnly();
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

void PMLibraryBrowserViewWidget::slotIconViewRefresh()
{
   m_pLibraryIconView->refresh();
}

void PMLibraryBrowserViewWidget::slotIconViewSetLibrary()
{
   m_pLibraryIconView->setLibrary( m_pFutureLibrary );
   m_pCurrentLibrary = m_pFutureLibrary;
   m_pFutureLibrary = NULL;
}

void PMLibraryBrowserViewWidget::slotSelectionChanged( QListWidgetItem *item )
{
    if( !item ) return;

   PMLibraryIconViewItem* sel = static_cast<PMLibraryIconViewItem*>( item );
   m_pLibraryIconView->setCurrentItem( sel );

   if( sel->isSubLibrary() )
   {
      if( m_pLibraryEntryPreview->saveIfNeeded() )
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh() ) );
      else
         m_pLibraryEntryPreview->showPreview( sel->path(), m_topLibraryReadOnly, true );
   }
   else
   {
      if( m_pLibraryEntryPreview->saveIfNeeded() )
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh() ) );
      else
         m_pLibraryEntryPreview->showPreview( sel->path(), m_topLibraryReadOnly, false );
   }
   m_pLibraryIconView->setFocus();
}

void PMLibraryBrowserViewWidget::slotSelectionExecuted( QListWidgetItem* item )
{
   if( !item ) return;

   PMLibraryIconViewItem* sel = static_cast<PMLibraryIconViewItem*>( item );
   m_pLibraryIconView->setCurrentItem( sel );
   if( sel->isSubLibrary() )
   {
      // It's a sub library
      m_pFutureLibrary = new PMLibraryHandle( sel->path() );
      m_pLibraryEntryPreview->clearPreview();
      QTimer::singleShot( 100, this, SLOT( slotIconViewSetLibrary() ) );
      m_pUpButton->setEnabled( true );
   }
   else
   {
      // It's an object
      if( m_pLibraryEntryPreview->saveIfNeeded() )
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh() ) );
      else
         m_pLibraryEntryPreview->showPreview( sel->path(), m_topLibraryReadOnly, false );
   }
   m_pLibraryIconView->setFocus();
}

void PMLibraryBrowserViewWidget::slotUpButtonClicked()
{
  QDir pathManipulator( m_pCurrentLibrary->path() );

  pathManipulator.cdUp();
  m_pFutureLibrary = new PMLibraryHandle( pathManipulator.path() );
  if( !m_pFutureLibrary->isSubLibrary() )
     m_pUpButton->setEnabled( false );
  QTimer::singleShot( 100, this, SLOT( slotIconViewSetLibrary() ) );

  // Release the current Library
  delete m_pCurrentLibrary;
}

void PMLibraryBrowserViewWidget::slotDeleteClicked()
{
   PMLibraryIconViewItem* sel = static_cast<PMLibraryIconViewItem*>( m_pLibraryIconView->currentItem() );
   PMLibraryHandle::PMResult rst;
   if( !sel )
      return;
   else if( sel->isSubLibrary() )
      rst = m_pCurrentLibrary->deleteSubLibrary( sel->path() );
   else
      rst = m_pCurrentLibrary->deleteObject( sel->path() );

   switch( rst )
   {
      case PMLibraryHandle::Ok:
      {
         //KIO::Job *job = KIO::file_delete( sel->path() );
         QFile::remove( sel->path() );
         //connect( job, SIGNAL( result( KJob * ) ), SLOT( slotJobResult( KJob * ) ) );
      }
      break;
      case PMLibraryHandle::ReadOnlyLib:
         QMessageBox::information( this, tr( "warning" ) ,
                      tr( "This library is read only." ),
                      QMessageBox::Ok, QMessageBox::NoButton);
         break;
      case PMLibraryHandle::NotInLib:
         QMessageBox::information( this, tr( "warning" ) ,
                      tr( "The current library does not contain that item." ),
                      QMessageBox::Ok, QMessageBox::NoButton);
         break;
      default:
         QMessageBox::information( this, tr( "warning" ) ,
                      tr( "Could not remove item." ),
                      QMessageBox::Ok, QMessageBox::NoButton);
         break;
   }
}

void PMLibraryBrowserViewWidget::slotNewObjectClicked()
{
   m_pLibraryEntryPreview->saveIfNeeded();

   switch( m_pCurrentLibrary->createNewObject() )
   {
      case PMLibraryHandle::Ok:
         QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh() ) );
         break;
      case PMLibraryHandle::ReadOnlyLib:
         QMessageBox::information( this, tr( "warning" ) ,
                      tr( "This library is read only." ),
                      QMessageBox::Ok, QMessageBox::NoButton);
         break;
      default:
         QMessageBox::information( this, tr( "warning" ) ,
                      tr( "Could not create a new object." ),
                      QMessageBox::Ok, QMessageBox::NoButton);
   }
}

void PMLibraryBrowserViewWidget::slotNewSubLibraryClicked()
{
   bool result = false;

   m_pLibraryEntryPreview->saveIfNeeded();
   QString subLibraryName = QInputDialog::getText( this, tr( "Create Sub-Library" ),
                                                   tr( "Enter the sub-library name: " ),
                                                   QLineEdit::Normal, "",
                                                   &result );

   if( result )
   {
      switch( m_pCurrentLibrary->createNewSubLibrary( subLibraryName ) )
      {
         case PMLibraryHandle::Ok:
            m_pLibraryIconView->refresh();
            break;
         case PMLibraryHandle::ExistingDir:
            QMessageBox::information( this, tr( "warning" ) ,
                         tr( "That library already exists." ),
                         QMessageBox::Ok, QMessageBox::NoButton);
            break;
         case PMLibraryHandle::ReadOnlyLib:
            QMessageBox::information( this, tr( "warning" ) ,
                         tr( "This library is read only." ),
                         QMessageBox::Ok, QMessageBox::NoButton);
            break;
         default:
            QMessageBox::information( this, tr( "warning" ) ,
                         tr( "Could not create a new sub library." ),
                         QMessageBox::Ok, QMessageBox::NoButton);
      }
   }
}
/*
void PMLibraryBrowserViewWidget::slotJobResult( KJob * job )
{
   if( job->error() )
      job->uiDelegate()->showErrorMessage();
   QTimer::singleShot( 100, this, SLOT( slotIconViewRefresh() ) );
}*/

QString PMLibraryBrowserViewFactory::description() const
{
   return ( "Library View" );
}

#include "pmlibrarybrowser.moc"
