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

#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDropEvent>

//eticre
#include <QTextEdit>
#include <QList>

#include <QUrl>
#include <QLocale>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QApplication>
#include <QClipboard>


PMLibraryEntryPreview::PMLibraryEntryPreview( QWidget* parent ) :
   QWidget( parent )
{
   setAcceptDrops( true );
   // m_pPart = new PMPart( this, "treeview", NULL, false, true );
   m_pPart = new PMPart( this, this,  true, true, nullptr );
   m_pPart->setReadWrite( false );
   m_readOnly = false;
   m_modified = false;
   m_subLib = false;

   QVBoxLayout* vl = new QVBoxLayout( this );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );

   QHBoxLayout* hl = new QHBoxLayout( this );
   vl->addItem( hl );
   hl->addStretch( 1 );

   QGridLayout* gl = new QGridLayout( this );
   vl->addItem( gl );
   gl->setColumnStretch( 3, 1 );
   QLabel* lbl = new QLabel( tr( "Name: " ), this );
   m_pName = new QLineEdit( this );
   m_pName->setAlignment( Qt::AlignLeft );
   m_pName->setReadOnly( true );
   gl->addWidget( lbl, 0, 0 );
   gl->addWidget( m_pName , 0 , 1 , 1 , 3 );
   gl->setRowStretch( 0, 1 );

   lbl = new QLabel( tr( "Description:" ), this );
   lbl->setAlignment( Qt::AlignTop );
   m_pDescription = new QTextEdit( this );
   m_pDescription->setAlignment( Qt::AlignTop  | Qt::AlignLeft );
   m_pDescription->setWordWrapMode(QTextOption::WrapAnywhere);
   m_pDescription->setReadOnly( true );
   gl->addWidget( lbl, 1, 0 );
   gl->addWidget( m_pDescription , 1 , 1 , 1 , 3 );
   gl->setRowStretch( 1, 1 );

   m_pKeywordsLabel = new QLabel( tr( "Keywords:" ), this );
   m_pKeywordsLabel->setAlignment( Qt::AlignTop );
   m_pKeywords = new QTextEdit( this );
   m_pKeywords->setAlignment( Qt::AlignTop  | Qt::AlignLeft );
   m_pKeywords->setWordWrapMode(QTextOption::WrapAnywhere);
   m_pKeywords->setReadOnly( true );
   gl->addWidget( m_pKeywordsLabel , 2 , 0 );
   gl->addWidget( m_pKeywords , 2 , 1 , 1 , 3 );
   gl->setRowStretch( 2, 1);

   m_pContentsLabel = new QLabel( tr( "Contents:" ), this );
   m_pLabelPovrayPreview = new QLabel( tr( "Povray Preview:" ), this );
   m_pContentsPreview = new PMTreeView( m_pPart, this );//eticre copypaste library treeview
   gl->addWidget( m_pContentsLabel, 3, 0 );
   gl->setRowStretch( 3, 0 );
   gl->addWidget( m_pContentsPreview, 4, 0 , 1 , 2 );
   gl->addWidget( m_pLabelPovrayPreview , 4 , 2 , 1 , 2 );
   gl->setRowStretch( 4, 8 );

   hl = new QHBoxLayout( this );
   vl->addItem( hl );
   hl->addStretch( 1 );
   m_pSetPreviewImageButton = new QPushButton( tr( "Change Preview Image" ), this );
   m_pSetPreviewImageButton->setEnabled( false );
   hl->addWidget( m_pSetPreviewImageButton );

   //hl = new QHBoxLayout( vl );
   hl->addStretch( 1 );
   m_pApplyChanges = new QPushButton( tr( "&Apply" ), this );
   m_pApplyChanges->setEnabled( false );
   hl->addWidget( m_pApplyChanges );
   m_pCancelChanges = new QPushButton( tr( "&Cancel" ), this );
   m_pCancelChanges->setEnabled( false );
   hl->addWidget( m_pCancelChanges );
   m_pPovrayPreview = new QPushButton( tr( "&TexturePreview" ), this );
   m_pPovrayPreview->setEnabled( true );
   hl->addWidget( m_pPovrayPreview );

   connect( m_pPovrayPreview , SIGNAL( clicked() ), SLOT( slotPovrayPreview() ) );

   m_pObjectSelect = new PMObjectSelect();// eticre try preview

   count_preview = 1;

   connect( m_pObjectSelect ,
            SIGNAL( ok_image( QImage )),
            SLOT( slotRecivePovrayPreview( QImage ) ) );

   connect( m_pContentsPreview,
            SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
            this, SLOT( slotChangeSelected() ) );

   connect( m_pName,
            SIGNAL( textChanged( const QString& ) ),
            SLOT( slotTextChanged( const QString& ) ) );

   connect( m_pDescription,
            SIGNAL( textChanged() ),
            SLOT( slotTextChanged() ) );

   connect( m_pKeywords,
            SIGNAL( textChanged() ),
            SLOT( slotTextChanged() ) );

   connect( m_pSetPreviewImageButton,
            SIGNAL( clicked() ),
            SLOT( slotPreviewClicked() ) );

   connect( m_pPart,
            SIGNAL( modified() ),
            SLOT( slotTextChanged() ) );

   connect( m_pApplyChanges,
            SIGNAL( clicked() ),
            SLOT( slotApplyClicked() ) );

   connect( m_pCancelChanges,
            SIGNAL( clicked() ),
            SLOT( slotCancelClicked() ) );

   vl->addStretch( 1 );
}


void PMLibraryEntryPreview::slotEditCopy()
{
//QApplication::clipboard()->setMimeData( new PMObjectDrag( this->m_pPart, m_pObjectSelect ), QClipboard::Clipboard );
}

PMLibraryEntryPreview::~PMLibraryEntryPreview()
{
  /* if(m_pPart){
       delete m_pPart;
       m_pPart = nullptr;
   }*/// -//eticre
}

bool PMLibraryEntryPreview::showPreview( QString lib_path, bool readOnly, bool subLib )
{
   bool result = false;

   if( QFile::exists( lib_path + "/library_index.xml" ) )
   {
      result = saveIfNeeded();

      m_image = QImage();

      m_subLib = subLib;
      if( subLib )
      {
         // Load the new subLib to preview
         PMLibraryHandle lib( lib_path );
         m_pName->setText( lib.name() );
         m_pDescription->setText( lib.description() );
         m_pKeywords->setText( "" );
         m_pPart->setReadWrite( true );
         m_pPart->setScene( new PMScene( m_pPart ) );
         m_pContentsPreview->slotRefresh();
         m_pPart->setReadWrite( false );
      }
      else
      {
         // Load the new object to preview
         PMLibraryObject aux( lib_path );

         m_pName->setText( aux.name() );
         m_pDescription->setText( aux.description() );
         m_pKeywords->setText( aux.keywords() );
         if( aux.preview() )
         {
            m_image = aux.preview()->copy();
         }

         loadObjects( aux.objects() );// eticre error
         //Save the preview location
      }
      m_pKeywordsLabel->setEnabled( !subLib );
      m_pKeywords->setEnabled( !subLib );
      m_pContentsLabel->setEnabled( !subLib );
      m_pContentsPreview->setEnabled( !subLib );
      setReadOnly( readOnly );
      setModified( false );
      m_currentPath = lib_path;
   }
   return result;
}

//obj is entry .kpm(gzip) file
void PMLibraryEntryPreview::loadObjects( QByteArray *obj )
{
   mapPreview.clear();
   m_pLabelPovrayPreview->clear();
   m_pPart->setReadWrite( true );
   m_pPart->setScene( new PMScene( m_pPart ) );
   if( obj )
   {
     PMXMLParser parser( m_pPart, *obj );
     m_pPart->insertFromParser( tr( "Object Load" ), &parser, m_pPart->scene() );
   }
   m_pPart->setReadWrite( false );
   m_pContentsPreview->slotRefresh();
}

void PMLibraryEntryPreview::clearPreview()
{
   saveIfNeeded();
   m_pName->setText( "" );
   m_pDescription->setText( "" );
   m_pKeywords->setText( "" );
   m_image = QImage();
   m_pPart->setReadWrite( true );
   m_pPart->setScene( new PMScene( m_pPart ) );
   m_pContentsPreview->slotRefresh();
   m_pPart->setReadWrite( false );
   m_pLabelPovrayPreview->clear();
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

void PMLibraryEntryPreview::slotTextChanged()
{
   setModified( true );
}

void PMLibraryEntryPreview::slotTextChanged( const QString& /* s */)
{
   setModified( true );
}

void PMLibraryEntryPreview::slotPreviewClicked()
{
   //KFileDialog dlg( QString::null, "", NULL/*, "", false*/ );//eticre
   QUrl url;
   QFileDialog dlg( this, QString( "Preview" ) );
   //todo
    //KFileDialog dlg;
   dlg.setNameFilter( "image/jpeg image/gif image/tiff image/png image/bmp" );
   if( dlg.exec() == QDialog::Accepted )
   {
      m_image.load( dlg.selectedFiles().first() );
      setModified( true );
   }
}

void PMLibraryEntryPreview::slotApplyClicked()
{
   saveIfNeeded( true );
   emit objectChanged();
}

void PMLibraryEntryPreview::slotCancelClicked()
{
   setModified( false );
   showPreview( m_currentPath, m_readOnly, m_subLib );
}

bool PMLibraryEntryPreview::saveIfNeeded( bool forceSave )
{
   if ( m_modified )
   {
      // ask if we must save the changes
      if( forceSave || QMessageBox::question( this, tr( "Warning" ) ,
                       tr( "The object has been modified and not saved.\nDo you wish to save?" ),
                        QMessageBox::Yes | QMessageBox::No ) )

      {
         if( m_subLib )
         {
            PMLibraryHandle lib( m_currentPath );
            lib.setName( m_pName->text() );
            lib.setDescription( m_pDescription->toPlainText() );
            lib.saveLibraryInfo();
         }
         else
         {
            PMLibraryObject objToSave;
            QList<PMObject*>sortedList;
            PMObject* tmp;

            // First save the text parameters.
            objToSave.setName( m_pName->text() );
            objToSave.setDescription( m_pDescription->toPlainText() );
            objToSave.setKeywords( m_pKeywords->toPlainText() );

            // Gather the contents of the part.
            tmp = m_pPart->scene()->firstChild();
            while( tmp )
            {
               sortedList.append( tmp );
               tmp = tmp->nextSibling();
            }

            // Add them to the object to save.
            PMObjectDrag drag( m_pPart, sortedList );
            objToSave.setObjects( drag.data( "application/x-povmodeler" ) );//eticre encodedData

            // Add the preview image
            objToSave.setPreview( m_image.copy() );

            // Finally save the object to a file.
            qDebug() << m_currentPath << "\n";
            objToSave.save( m_currentPath );
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
   if ( !m_readOnly && event->mimeData()->hasImage() )
       event->acceptProposedAction();
}

void PMLibraryEntryPreview::dropEvent( QDropEvent* event )
{
   if( event->mimeData()->hasImage() )
   {
      m_image = ( qvariant_cast<QImage>(event->mimeData()->imageData() ) );
      setModified( true );
   }
}

void PMLibraryEntryPreview::setModified( bool modified )
{
   m_modified = modified;
   m_pApplyChanges->setEnabled( modified );
   m_pCancelChanges->setEnabled( modified );
}

void PMLibraryEntryPreview::slotPovrayPreview()
{
   m_pLabelPovrayPreview->clear();
   m_pLabelPovrayPreview->setText( "No texture preview available" );
   PMObject * o = m_pPart->activeObject();
   if( o )
   {
       m_pObjectSelect->setSelectedObject( o );
       if( o )
       {
           o->map_image_preview = count_preview;
           ++count_preview;
       }
       m_pObjectSelect->slotPreview();
   }

}

void PMLibraryEntryPreview::slotRecivePovrayPreview( QImage img )
{
    if( !img.isNull() )
    {
        m_pLabelPovrayPreview->setPixmap( QPixmap::fromImage( img ) );
        int i = m_pPart->activeObject()->map_image_preview;
        mapPreview[ i ] = img;
    }
     else
        m_pLabelPovrayPreview->setText( "Bad preview request" );

    m_pLabelPovrayPreview->setGeometry(0,0,300,300);

}

void PMLibraryEntryPreview::slotChangeSelected()
{
    int i = m_pPart->activeObject()->map_image_preview;
    QImage img = mapPreview[ i ];
    if(!img.isNull() )
        m_pLabelPovrayPreview->setPixmap( QPixmap::fromImage( img ) );
    else
    {
        m_pLabelPovrayPreview->clear();
        m_pLabelPovrayPreview->setText( "No Preview Image" );
        m_pLabelPovrayPreview->setGeometry(0,0,300,300);
    }
}


#include "pmlibraryentrypreview.moc"
