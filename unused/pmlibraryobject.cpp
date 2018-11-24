//-*-C++-*-
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

#include <qstring.h>
#include <QList>
#include <qdom.h>
#include <qbuffer.h>
#include <qimage.h>
//Added by qt3to4:
#include <QTextStream>

#include <karchive.h>
#include <kurl.h>
#include <ktar.h>
#include <kfilterdev.h>

#include <stdlib.h>

#include "pmdebug.h"

PMLibraryObject::PMLibraryObject( )
{
   m_previewLoaded = false;
   m_objectsLoaded = false;
   m_preview = NULL;
   m_objects = NULL;
   m_data = NULL;
   m_name = QString::null;
   m_description = QString::null;
   m_keywords = QString::null;
}

PMLibraryObject::PMLibraryObject( KUrl u )
{
   m_previewLoaded = false;
   m_objectsLoaded = false;
   m_preview = NULL;
   m_objects = NULL;
   m_data = new KTar( u.path( ), "application/x-gzip" );
   loadLibraryInfo( );
}

PMLibraryObject::~PMLibraryObject( )
{
   delete m_data;
   if( m_previewLoaded )
      delete m_preview;
   if( m_objectsLoaded )
      delete m_objects;
}

void PMLibraryObject::loadLibraryInfo( )
{
   if( m_data )
   {
      m_data->open( QIODevice::ReadOnly );
      // Position in the root of the file
      const KArchiveDirectory* root = m_data->directory( );
      if( !root )
         return;

      // Find the object info
      const KArchiveEntry* entry = root->entry( "objectinfo.xml" );
      if( entry && entry->isFile( ) )
      {
			QByteArray tmp = ( ( KArchiveFile* )entry )->data( ); 
         QBuffer buffer( &tmp );
         buffer.open( QIODevice::ReadOnly );

         QDomDocument doc( "OBJECTINFO" );
         doc.setContent( &buffer );

         QDomElement e = doc.documentElement( );
         m_name = e.attribute( "name", "empty" );

         QDomNode c = e.firstChild( );
         while( !c.isNull( ) )
         {
            if( c.isElement( ) )
            {
               QDomElement ce = c.toElement( );
               // Description information
               if( ce.tagName( ) == "description" )
               {
                  QDomText te = ce.firstChild( ).toText( );
                  m_description = te.nodeValue( );
               }
               // Keywords information
               else if( ce.tagName( ) == "keywords" )
               {
                  QDomText te = ce.firstChild( ).toText( );
                  m_keywords = te.nodeValue( );
               }
               // Extra files list
               else if( ce.tagName( ) == "file_entries" )
               {
                  QDomNode entry = ce.firstChild( );
                  while( !entry.isNull( ) )
                  {
                     QDomElement entryElement = entry.toElement( );
                     if( entryElement.tagName( ) == "file" )
                        m_extraFiles.append( entryElement.attribute( "name", "" ) );
                     entry = entry.nextSibling( );
                  }
               }
            }
            c = c.nextSibling( );
         }
      }
      m_data->close( );
   }
}

QImage* PMLibraryObject::preview( )
{
   if( !m_previewLoaded )
   {
      if( m_data )
      {
         m_data->open( QIODevice::ReadOnly );
         // Position in the root of the file
         const KArchiveDirectory* root = m_data->directory( );
         if( !root )
            return NULL;

         // Find the object preview
         const KArchiveEntry* entry = root->entry( "preview.png" );
         if( entry && entry->isFile( ) )
         {
				QByteArray tmp = ( ( KArchiveFile* )entry )->data( ); 
            QBuffer buffer( &tmp );
            buffer.open( QIODevice::ReadOnly );
            m_preview = new QImage( buffer.readAll( ) );
            m_previewLoaded = true;
         }

         m_data->close( );
      }
   }
   return m_preview;
}

QByteArray* PMLibraryObject::objects( )
{
   if( !m_objectsLoaded )
   {
      if( m_data )
      {
         m_data->open( QIODevice::ReadOnly );
         // Position in the root of the file
         const KArchiveDirectory* root = m_data->directory( );
         if( !root )
            return NULL;

         // Find the object info
         const KArchiveEntry* entry = root->entry( "objectdata.kpm" );
         if( entry && entry->isFile( ) )
         {
            // Transfer the file contents to a QByteArray.
            // TODO Maybe there is a smarter way of doing this.
            char buf[256];
            int nbytes;
            QIODevice* aux_in = ( ( KArchiveFile* )entry )->device( );

            m_objects = new QByteArray( );
            QBuffer aux_out( m_objects );

            aux_in->open( QIODevice::ReadOnly );
            aux_out.open( QIODevice::WriteOnly );
            while( !aux_in->atEnd( ) )
            {
               nbytes = aux_in->readBlock( buf, 256 );
               aux_out.writeBlock( buf, nbytes );
            }
            delete aux_in;
            if( m_objects->size( ) != 0 )
               m_objectsLoaded = true;
            else
            {
               delete m_objects;
               m_objects = NULL;
            }
         }
      }
   }
   return m_objects;
}

void PMLibraryObject::setName( const QString& str )
{
   m_name = str;
}

void PMLibraryObject::setDescription( const QString& str )
{
   m_description = str;
}

void PMLibraryObject::setKeywords( const QString& str )
{
   m_keywords = str;
}

void PMLibraryObject::setPreview( const QImage& img )
{
   if( m_previewLoaded )
      delete m_preview;

   m_preview = new QImage( img );
   m_previewLoaded = true;
}

void PMLibraryObject::setObjects( const QByteArray& obj )
{
   if( m_objectsLoaded )
      delete m_objects;

   m_objects = new QByteArray( obj );
   m_objects->detach( );
   m_objectsLoaded = true;
}

void PMLibraryObject::save( const QString& fileName )
{
   // create the library file
   m_data = new KTar( fileName, "application/x-gzip" );
   m_data->open( QIODevice::WriteOnly );
   // save object info
   saveLibraryInfo( );
   // save preview
   savePreview( );
   // save library objects
   saveObjects( );
   m_data->close( );
}

void PMLibraryObject::saveLibraryInfo( )
{
   // Pre-condition for the execution of this method.
   if( !m_data || !m_data->isOpened( ) )
   {
      kError( PMArea ) << "Trying to save to an unopened data file." << endl;
      exit( 1 );
   }

   QBuffer buf;

   // Document type
   QDomDocument doc( "OBJECTINFO" );
   
   // The root element for the document has one attribute: name 
   QDomElement root = doc.createElement( "object" );
   doc.appendChild( root );
   root.setAttribute( "name", m_name );

   // Inside the root element we add the description node ...
   QDomElement description = doc.createElement( "description" );
   root.appendChild( description );
   description.appendChild( doc.createTextNode( m_description ) );

   // ... and the keywords node ...
   QDomElement keywords = doc.createElement( "keywords" );
   root.appendChild( keywords );
   keywords.appendChild( doc.createTextNode( m_keywords ) );

   // ... and finally the extra files information
   QDomElement file_list = doc.createElement( "file_list" );
   root.appendChild( file_list );
   QStringList::Iterator it = m_extraFiles.begin( );
   for( ; it != m_extraFiles.end( ); ++it )
   {
      QDomElement entry = doc.createElement( "file" );
      entry.setAttribute( "name", *it );
      file_list.appendChild( entry );
   }

   // Write the document to the buffer
   QByteArray array;
   QTextStream str( array, QIODevice::WriteOnly );
   str.setEncoding( QTextStream::UnicodeUTF8 );
   str << doc.toString( );
   m_data->writeFile( "objectinfo.xml", "user", "group", array.size( ), array.data( ) );
}

void PMLibraryObject::savePreview( )
{
   // Pre-condition for the execution of this method.
   if( !m_data || !m_data->isOpened( ) )
   {
      kError( PMArea ) << "Trying to save to an unopened data file." << endl;
      exit( 1 );
   }

   QByteArray array;
   QBuffer iods( array );
   QImageIO img_io( &iods, "PNG" );

   if( m_previewLoaded )
   {
      img_io.setImage( *m_preview );
      iods.open( QIODevice::WriteOnly );
      img_io.write( );
      m_data->writeFile( "preview.png", "user", "group", array.size( ), array.data( ) );
   }
}

void PMLibraryObject::saveObjects( )
{
   // Pre-condition for the execution of this method.
   if( !m_data || !m_data->isOpened( ) )
   {
      kError( PMArea ) << "Trying to save to an unopened data file." << endl;
      exit( 1 );
   }

   if( m_objectsLoaded )
      m_data->writeFile( "objectdata.kpm", "user", "group", m_objects->size( ), m_objects->data( ) );
}
