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


#include "pmlibraryobject.h"
#include "pmlibraryhandle.h"
#include "pmlibraryentrypreview.h"
#include "pmdialogeditbase.h"
#include "pmpart.h"
#include "pmtreeview.h"
#include "pmxmlparser.h"
#include "pmscene.h"
#include "pmobjectdrag.h"

#include <qlabel.h>
#include <qlineedit.h>
#include <q3multilineedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <q3dragobject.h>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDropEvent>

#include <kurl.h>
#include <klocale.h>
#include <kdialog.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

PMLibraryEntryPreview::PMLibraryEntryPreview( QWidget* parent ) :
   QWidget( parent )
{
   setAcceptDrops( true );
   m_pPart = new PMPart( this, "treeview", NULL, false, true );
   m_pPart->setReadWrite( false );
   m_readOnly = true;
   m_modified = false;
   m_subLib = false;

   setMaximumSize( 1000, 1000 );
   QVBoxLayout* vl = new QVBoxLayout( this );
   vl->setSpacing( KDialog::spacingHint( ) );

   QHBoxLayout* hl = new QHBoxLayout( vl );
   hl->addStretch( 1 );

   QGridLayout* gl = new QGridLayout( vl );
   gl->setColoumnStretch( 1, 1 );
   QLabel* lbl = new QLabel( i18n( "Name: " ), this );
   m_pName = new QLineEdit( this );
   m_pName->setAlignment( Qt::AlignLeft );
   m_pName->setReadOnly( true );
   gl->addWidget( lbl, 0, 0 );
   gl->addWidget( m_pName, 0, 1 );

   lbl = new QLabel( i18n( "Description:" ), this );
   lbl->setAlignment( Qt::AlignTop );
   m_pDescription = new QTextEdit( this );
   m_pDescription->setAlignment( Qt::AlignTop  | Qt::AlignLeft );
   m_pDescription->setWordWrapMode(QTextOption::WrapAnywhere);
   m_pDescription->setReadOnly( true );
   gl->addWidget( lbl, 1, 0 );
   gl->addWidget( m_pDescription, 1, 1 );

   m_pKeywordsLabel = new QLabel( i18n( "Keywords:" ), this );
   m_pKeywordsLabel->setAlignment( Qt::AlignTop );
   m_pKeywords = new QTextEdit( this );
   m_pKeywords->setAlignment( Qt::AlignTop  | Qt::AlignLeft );
   m_pKeywords->setWordWrapMode(QTextOption::WrapAnywhere);
   m_pKeywords->setReadOnly( true );
   gl->addWidget( m_pKeywordsLabel, 2, 0 );
   gl->addWidget( m_pKeywords, 2, 1 );

   m_pContentsLabel = new QLabel( i18n( "Contents:" ), this );
   m_pContentsPreview = new PMTreeView( m_pPart, this );
   gl->addMultiCellWidget( m_pContentsLabel, 3, 3, 0, 1 );
   gl->addMultiCellWidget( m_pContentsPreview, 4, 4, 0, 1 );
   gl->setRowStretch(4, 1);

   hl = new QHBoxLayout( vl );
   hl->addStretch( 1 );
   m_pSetPreviewImageButton = new QPushButton( i18n( "Change Preview Image" ), this );
   m_pSetPreviewImageButton->setEnabled( false );
   hl->addWidget( m_pSetPreviewImageButton );

   hl = new QHBoxLayout( vl );
   hl->addStretch( 1 );
   m_pApplyChanges = new QPushButton( i18n( "&Apply" ), this );
   m_pApplyChanges->setEnabled( false );
   hl->addWidget( m_pApplyChanges );
   m_pCancelChanges = new QPushButton( i18n( "&Cancel" ), this );
   m_pCancelChanges->setEnabled( false );
   hl->addWidget( m_pCancelChanges );

   connect( m_pName,                  SIGNAL( textChanged( const QString& ) ),
                                      SLOT( slotTextChanged( const QString& ) ) );
   connect( m_pDescription,           SIGNAL( textChanged( ) ),
                                      SLOT( slotTextChanged( ) ) );
   connect( m_pKeywords,              SIGNAL( textChanged( ) ),
                                      SLOT( slotTextChanged( ) ) );
   connect( m_pSetPreviewImageButton, SIGNAL( clicked( ) ),
                                      SLOT( slotPreviewClicked( ) ) );
   connect( m_pPart,                  SIGNAL( modified( ) ),
                                      SLOT( slotTextChanged( ) ) );
   connect( m_pApplyChanges, SIGNAL( clicked( ) ), SLOT( slotApplyClicked( ) ) );
   connect( m_pCancelChanges, SIGNAL( clicked( ) ), SLOT( slotCancelClicked( ) ) );
   vl->addStretch( 1 );
}

PMLibraryEntryPreview::~PMLibraryEntryPreview( )
{
//   delete m_pPart;
}

bool PMLibraryEntryPreview::showPreview( KUrl u, bool readOnly, bool subLib )
{
   bool result = false;

   if( u.isLocalFile( ) )
   {
      result = saveIfNeeded( );

      m_image.reset( );

      m_subLib = subLib;
      if( subLib )
      {
         // Load the new subLib to preview
         PMLibraryHandle lib( u.path( ) );
         m_pName->setText( lib.name( ) );
         m_pDescription->setText( lib.description( ) );
         m_pKeywords->setText( "" );
         m_pPart->setReadWrite( true );
         m_pPart->setScene( new PMScene( m_pPart ) );
         m_pContentsPreview->slotRefresh( );
         m_pPart->setReadWrite( false );
      }
      else
      {
         // Load the new object to preview
         PMLibraryObject aux( u );

         m_pName->setText( aux.name( ) );
         m_pDescription->setText( aux.description( ) );
         m_pKeywords->setText( aux.keywords( ) );
         if( aux.preview( ) )
         {
            m_image = aux.preview( )->copy( );
         }
         loadObjects( aux.objects( ) );
         // Save the preview location
      }
      m_pKeywordsLabel->setEnabled( !subLib );
      m_pKeywords->setEnabled( !subLib );
      m_pContentsLabel->setEnabled( !subLib );
      m_pContentsPreview->setEnabled( !subLib );
      setReadOnly( readOnly );
      setModified( false );
      m_currentURL = u;
   }
   return result;
}

void PMLibraryEntryPreview::loadObjects( QByteArray* obj )
{

   m_pPart->setReadWrite( true );
   m_pPart->setScene( new PMScene( m_pPart ) );
   if( obj )
   {
      PMXMLParser parser( m_pPart, *obj );
      m_pPart->insertFromParser( i18n( "Object Load" ), &parser, m_pPart->scene( ) );
   }
   m_pPart->setReadWrite( false );
   m_pContentsPreview->slotRefresh( );
}

void PMLibraryEntryPreview::clearPreview( )
{
   saveIfNeeded( );
   m_pName->setText( "" );
   m_pDescription->setText( "" );
   m_pKeywords->setText( "" );
   m_image.reset( );
   m_pPart->setReadWrite( true );
   m_pPart->setScene( new PMScene( m_pPart ) );
   m_pContentsPreview->slotRefresh( );
   m_pPart->setReadWrite( false );
   setReadOnly( true );
   setModified( false );
}

void PMLibraryEntryPreview::setReadOnly( bool b )
{
   m_readOnly = b;
   if( b )
   {
      m_pName->setReadOnly( true );
      m_pDescription->setReadOnly( true );
      m_pKeywords->setReadOnly( true );
      m_pSetPreviewImageButton->setEnabled( false );
      m_pPart->setReadWrite( false );
   }
   else
   {
      m_pName->setReadOnly( false );
      m_pDescription->setReadOnly( false );
      if( m_subLib )
      {
         m_pKeywords->setReadOnly( true );
         m_pSetPreviewImageButton->setEnabled( false );
         m_pPart->setReadWrite( false );
      }
      else
      {
         m_pKeywords->setReadOnly( false );
         m_pSetPreviewImageButton->setEnabled( true );
         m_pPart->setReadWrite( true );
      }
   }
}

void PMLibraryEntryPreview::slotTextChanged( )
{
   setModified( true );
}

void PMLibraryEntryPreview::slotTextChanged( const QString& /* s */)
{
   setModified( true );
}

void PMLibraryEntryPreview::slotPreviewClicked( )
{
   KFileDialog dlg( QString::null, "", NULL/*, "", false*/ );

   dlg.setFilter( "image/jpeg image/gif image/tiff image/png image/bmp" );
   if( dlg.exec( ) == QDialog::Accepted )
   {
      m_image.load( dlg.selectedFile( ) );
      setModified( true );
   }
}

void PMLibraryEntryPreview::slotApplyClicked( )
{
   saveIfNeeded( true );
   emit objectChanged( );
}

void PMLibraryEntryPreview::slotCancelClicked( )
{
   setModified( false );
   showPreview( m_currentURL, m_readOnly, m_subLib );
}

bool PMLibraryEntryPreview::saveIfNeeded( bool forceSave )
{
   if ( m_modified )
   {
      // ask if we must save the changes
      if( forceSave || KMessageBox::questionYesNo( this,
                                      i18n( "The object has been modified and not saved.\nDo you wish to save?" ),
                                      i18n( "Warning" ), KStandardGuiItem::save(), KStandardGuiItem::discard() ) == KMessageBox::Yes )
      {
         if( m_subLib )
         {
            PMLibraryHandle lib( m_currentURL.path( ) );
            lib.setName( m_pName->text( ) );
            lib.setDescription( m_pDescription->text( ) );
            lib.saveLibraryInfo( );
         }
         else
         {
            PMLibraryObject objToSave;
            PM3ObjectList sortedList;
            PMObject* tmp;

            // First save the text parameters.
            objToSave.setName( m_pName->text( ) );
            objToSave.setDescription( m_pDescription->text( ) );
            objToSave.setKeywords( m_pKeywords->text( ) );

            // Gather the contents of the part.
            tmp = m_pPart->scene( )->firstChild( );
            while( tmp )
            {
               sortedList.append( tmp );
               tmp = tmp->nextSibling();
            }

            // Add them to the object to save.
            PMObjectDrag drag( m_pPart, sortedList );
            objToSave.setObjects( drag.encodedData( "application/x-kpovmodeler" ) );

            // Add the preview image
            objToSave.setPreview( m_image.copy( ) );

            // Finally save the object to a file.
            kDebug( 0 ) << m_currentURL.path( ) << "\n";
            objToSave.save( m_currentURL.path( ) );
         }
         setModified( false );
         return true;
      }
      setModified( false );
   }
   return false;
}

void PMLibraryEntryPreview::dragEnterEvent( QDragEnterEvent* event )
{
   event->accept( !m_readOnly && Q3ImageDrag::canDecode( event ) );
}

void PMLibraryEntryPreview::dropEvent( QDropEvent* event )
{
   QImage img;
   if( Q3ImageDrag::decode( event, img ) )
   {
      m_image = img;
      setModified( true );
   }
}

void PMLibraryEntryPreview::setModified( bool modified )
{
   m_modified = modified;
   m_pApplyChanges->setEnabled( modified );
   m_pCancelChanges->setEnabled( modified );
}

#include "pmlibraryentrypreview.moc"
