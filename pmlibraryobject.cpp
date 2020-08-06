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
#include <QList>
#include <QBuffer>
#include <QImage>
#include <QImageWriter>
#include <QTextStream>
#include <QUrl>
#include <stdlib.h>
#include "pmdebug.h"
#include <QDebug>

PMLibraryObject::PMLibraryObject()
{
   m_previewLoaded = false;
   m_objectsLoaded = false;
   m_preview = nullptr;
   m_objects = nullptr;
   m_path = nullptr;
   m_name = QString();
   m_description = QString();
   m_keywords = QString();
}

PMLibraryObject::PMLibraryObject( QString url )
{
   m_previewLoaded = false;
   m_objectsLoaded = false;
   m_preview = nullptr;
   m_objects = nullptr;
   //m_path = new QString();
   m_path = url;
   loadLibraryInfo();
}

PMLibraryObject::~PMLibraryObject()
{
   m_path = nullptr;
   if( m_previewLoaded )
      delete m_preview;
   if( m_objectsLoaded )
      delete m_objects;
}


void PMLibraryObject::loadLibraryInfo()//eticre debug error 4
{
   if( m_path != "" )
   {
      //m_data->open( QIODevice::ReadOnly );
      // Position in the root of the file
      //const KArchiveDirectory* root = m_data->directory();
      //if( !root )
         //return;

      // Find the object info
      //const QFile* entry = root->entry( "objectinfo.xml" );
      QString fileinfo = m_path;
      fileinfo.append( "/objectinfo.xml" );
      if( QFile::exists( fileinfo ) )
      {
         QFile entry( fileinfo );
         entry.open( QIODevice::ReadOnly );
         QByteArray tmp = entry.readAll();
         QBuffer buffer( &tmp );
         buffer.open( QIODevice::ReadOnly );

         QDomDocument doc( "OBJECTINFO" );
         doc.setContent( &buffer );

         QDomElement e = doc.documentElement();
         m_name = e.attribute( "name", "empty" );

         QDomNode c = e.firstChild();
         while( !c.isNull() )
         {
            if( c.isElement() )
            {
               QDomElement ce = c.toElement();
               // Description information
               if( ce.tagName() == "description" )
               {
                  QDomText te = ce.firstChild().toText();
                  m_description = te.nodeValue();
               }
               // Keywords information
               else if( ce.tagName() == "keywords" )
               {
                  QDomText te = ce.firstChild().toText();
                  m_keywords = te.nodeValue();
               }
               // Extra files list
               else if( ce.tagName() == "file_entries" )
               {
                  QDomNode entry = ce.firstChild();
                  while( !entry.isNull() )
                  {
                     QDomElement entryElement = entry.toElement();
                     if( entryElement.tagName() == "file" )
                        m_extraFiles.append( entryElement.attribute( "name", "" ) );
                     entry = entry.nextSibling();
                  }
               }
            }
            c = c.nextSibling();
         }
      }
      //m_data->close();
   }
}

QImage* PMLibraryObject::preview()
{
   if( !m_previewLoaded )
   {
      if( m_path != "" )
      {
         QString fileinfo;
         fileinfo = m_path;
         fileinfo.append( "/preview.png" );
         if( QFile::exists( fileinfo ) )
         {
            m_preview = new QImage( fileinfo, nullptr );
            m_previewLoaded = true;
         }
      }
   }
   return m_preview;
}


QByteArray* PMLibraryObject::objects()
{
   if( !m_objectsLoaded )
   {
      if( m_path != "" )
      {
         QString fileinfo = m_path;
         fileinfo.append( "/objectdata.kpm" );
         QFile entry( fileinfo );
         if( entry.open( QIODevice::ReadOnly ) )
         {
            m_objects = new QByteArray( entry.readAll() );
            if( m_objects->size() != 0 )
            {
               m_objectsLoaded = true;
            }
            else
            {
               m_objects = nullptr;
            }
         }
      }
   }
   return  m_objects;
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
   m_objects->detach();
   m_objectsLoaded = true;
}

void PMLibraryObject::save( const QString& fileName )
{
   // create the library file
   //m_path = new KTar( fileName, "application/x-gzip" );
   //m_data->open( QIODevice::WriteOnly );
   // save object info
   saveLibraryInfo();
   // save preview
   savePreview();
   // save library objects
   saveObjects();
   //m_data->close();
}

void PMLibraryObject::saveLibraryInfo()
{
   // Pre-condition for the execution of this method.
 /*  if( !m_path || !m_path->isOpen() )//eticre isOpened
   {
      qCritical(  ) << "Trying to save to an unopened data file." << Qt::endl;
      exit( 1 );
   }*/

   QBuffer buf;

   // Document type
   QDomDocument doc( "OBJECTINFO" );

   // The root element for the document has one attribute: name
   QDomElement root = doc.createElement( "objinfo" );
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
   QStringList::Iterator it = m_extraFiles.begin();
   for( ; it != m_extraFiles.end(); ++it )
   {
      QDomElement entry = doc.createElement( "file" );
      entry.setAttribute( "name", *it );
      file_list.appendChild( entry );
   }

   // Write the document to the buffer
   QByteArray array;
   //QString qstmp;
   //qstmp=doc.toString();
   //array.append(&qstmp);
   //array.append( doc.toString() );
   array.append( doc.toByteArray() );
   //m_data->writeFile( "objectinfo.xml", "user", "group", array.data()  , array.size() );//eticre data<-->size
}

void PMLibraryObject::savePreview()
{
   // Pre-condition for the execution of this method.
   /*if( !m_data || !m_data->isOpen() )//eticre isOpened
   {
      qCritical(  ) << "Trying to save to an unopened data file." << Qt::endl;
      exit( 1 );
   }*/

   QByteArray array;
   QBuffer iods( &array );
   QImageWriter img_io( &iods, "PNG" );

   if( m_previewLoaded )
   {
      //img_io.setImage( *m_preview );
      iods.open( QIODevice::WriteOnly );
      img_io.write( *m_preview );//eticre add argument
      //m_data->writeFile( "preview.png", "user", "group", array.data() , array.size() );
   }
}

void PMLibraryObject::saveObjects()
{
   // Pre-condition for the execution of this method.
   /*if( !m_data || !m_data->isOpen() )//eticre --isOpened
   {
      qCritical(  ) << "Trying to save to an unopened data file." << Qt::endl;
      exit( 1 );
   }*/

   if( m_objectsLoaded )//eticre gzip--m_objects
   {
       QBuffer buffer;
       buffer.open( QBuffer::WriteOnly );

      // QIODevice* qio_dev = KFilterDev::device(&buffer, "application/x-gzip" , 0 );
       //qio_dev->open(QIODevice::WriteOnly);

       //int q = qio_dev->write( m_objects->data() );
       //qio_dev->close();

       m_objects->clear();
       m_objects->append( buffer.data() );

      //m_data->writeFile( "objectdata.kpm", "user", "group", m_objects->data() , m_objects->size() );
   }
}
