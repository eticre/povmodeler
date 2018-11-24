/*
**************************************************************************
                                 description
                             --------------------
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

#include "pmlibraryhandle.h"
#include "pmlibraryobject.h"

#include <stdlib.h>
#include <unistd.h>

#include <qdir.h>
#include <qfile.h>
#include <qdom.h>
#include <qtextstream.h>
#include <qimage.h>

#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include "pmdebug.h"

PMLibraryHandle::PMLibraryHandle( )
{
   setPath( "" );
   setAuthor( i18n( "Unknown" ) );
   setName( i18n( "Unknown" ) );
   m_readOnly = false;
   m_objects.setAutoDelete( true );
   m_libraries.setAutoDelete( true );
}

PMLibraryHandle::PMLibraryHandle( const QString& path )
{
   setPath( path );
   setAuthor( i18n( "Unknown" ) );
   setName( i18n( "Unknown" ) );
   m_objects.setAutoDelete( true );
   m_libraries.setAutoDelete( true );
   loadLibraryInfo( );
}

PMLibraryHandle::~PMLibraryHandle( )
{
}

void PMLibraryHandle::setName( const QString& name )
{
   m_name = name;
}

void PMLibraryHandle::setPath( const QString& path )
{
   m_objects.clear( );
   m_path = path;
}

void PMLibraryHandle::setAuthor( const QString& author )
{
   m_author = author;
}

void PMLibraryHandle::setDescription( const QString& description )
{
   m_description = description;
}

void PMLibraryHandle::setReadOnly( const bool rdOnly )
{
   m_readOnly = rdOnly;
}

void PMLibraryHandle::loadLibraryInfo( )
{
   // 1. Open the information file (library_index.xml)
   QFile file( m_path + "/library_index.xml" );

   if( !file.open( QIODevice::ReadOnly ) )
   {
      kError( PMArea ) << "Could not find the library index." << endl;
      return;
   }

   // 2. Read the information
   QDomDocument doc( "KPOVLIBINDEX" );
   doc.setContent( &file );

   QDomElement e = doc.documentElement( );

   if( e.tagName( ) != "library" )
   {
      kError( PMArea ) << "This doesn't appear to be a library index." << endl;
      return;
   }

   // 3. The library entry
   setName( e.attribute( "name", i18n( "Unknown" ) ) );
   setAuthor( e.attribute( "author", i18n( "Unknown" ) ) );
   setDescription( e.attribute( "description", "" ) );
   if( e.attribute( "readonly", "false" ) == "false" )
      m_readOnly = false;
   else
      m_readOnly = true;

   if( e.attribute( "sublibrary", "false" ) == "false" )
      m_subLibrary = false;
   else
      m_subLibrary = true;

   // 4. The object entries
   QDomNode n = e.firstChild( );
   if( !n.isNull( ) )
   {
      if( n.isElement( ) )
      {
         QDomElement c = n.toElement( );
         if( c.tagName( ) == "object_list" )
         {
            n = n.firstChild( );
            while( !n.isNull( ) )
            {
               c = n.toElement( );
               if( c.tagName( ) == "object_entry" )
               {
                  m_objects.insert( c.attribute( "name", i18n( "Unknown" ) ),
                                    new QString( c.attribute( "file", "" ) ) );
               }
               else if( c.tagName( ) == "library_entry" )
               {
                  m_libraries.insert( c.attribute( "name", i18n( "Unknown" ) ),
                                    new QString( c.attribute( "file", "" ) ) );
               }
               n = n.nextSibling( );
            }
         }
      }
   }
}

PMLibraryHandle::PMResult PMLibraryHandle::createLibrary( )
{
   // Test if the library exists.
   QDir d( m_path );

   if( !d.exists( ) )
   {
      // If it doesn't, create it
      if( !d.mkdir( m_path ) )
      {
         return PMLibraryHandle::CouldNotCreateDir;
      }
   }
   else
   {
      return PMLibraryHandle::ExistingDir;
   }

   return saveLibraryInfo( );
}

PMLibraryHandle::PMResult PMLibraryHandle::saveLibraryInfo( )
{
   // Save the information to the index
   QFile file( m_path + "/library_index.xml" );
   if( !file.open( QIODevice::WriteOnly ) )
   {
      return PMLibraryHandle::CouldNotCreateInfo;
   }

   // Create the XML DOM tree
   QDomDocument doc( "KPOVLIBINDEX" );
   QDomElement e = doc.createElement( "library" );
   e.setAttribute( "name", name( ) );
   e.setAttribute( "author", author( ) );
   e.setAttribute( "description", description( ) );
   if( m_readOnly )
      e.setAttribute( "readonly", "true" );
   else
      e.setAttribute( "readonly", "false" );

   if( m_subLibrary )
      e.setAttribute( "sublibrary", "true" );
   else
      e.setAttribute( "sublibrary", "false" );

   // Add the object list to the tree
   QDomElement l = doc.createElement( "object_list" );
   for(Q3DictIterator<QString> it( m_objects ); it.current( ); ++it )
   {
      QDomElement n = doc.createElement( "object_entry" );
      n.setAttribute( "name", it.currentKey( ) );
      n.setAttribute( "file", *( it.current( ) ) );
      l.appendChild( n );
   }
   for(Q3DictIterator<QString> it( m_libraries ); it.current( ); ++it )
   {
      QDomElement n = doc.createElement( "library_entry" );
      n.setAttribute( "name", it.currentKey( ) );
      n.setAttribute( "file", *( it.current( ) ) );
      l.appendChild( n );
   }
   e.appendChild( l );
   doc.appendChild( e );

   // Save to the file
   QTextStream str( &file );
   str.setEncoding( QTextStream::UnicodeUTF8 );
   str << doc.toString( );
   file.close( );

   return PMLibraryHandle::Ok;
}

PMLibraryHandle::PMResult PMLibraryHandle::createNewObject( )
{
   /// @todo Need to replace mkdtemp and mkstemps before enabling libs
   return PMLibraryHandle::CouldNotCreateFile;
   /*
   PMLibraryObject aux;
   QCString s = m_path.toLatin1( );
   QString unknownIcon = locate( "data" , "kpovmodeler/questionmark.png" );
   QImage img;
   int fh;

   if( m_readOnly )
      return PMLibraryHandle::ReadOnlyLib;

   aux.setName( i18n( "Empty" ) );
   aux.setObjects( QByteArray( ) );
   img.load( unknownIcon, "PNG" );
   aux.setPreview( img );

   s = s + "/objXXXXXX.kpml";
   if( ( fh = mkstemps( s.data( ), 5 ) ) < 0 )
   {
      // Ooops! Error creating the file.
      return PMLibraryHandle::CouldNotCreateFile;
   }

   // Success creating the file
   close( fh );
   m_objects.insert( i18n( "Empty" ), new QString( s ) );
   aux.save( s );
   saveLibraryInfo( );
   return PMLibraryHandle::Ok;
   */
}

PMLibraryHandle::PMResult PMLibraryHandle::addObject( const QString& path, const QString& name )
{
   if( m_readOnly )
      return PMLibraryHandle::ReadOnlyLib;

   m_objects.insert( name, new QString( path ) );
   saveLibraryInfo( );
   return PMLibraryHandle::Ok;
}

PMLibraryHandle::PMResult PMLibraryHandle::deleteObject( const QString& objectName )
{
   if( m_readOnly )
      return PMLibraryHandle::ReadOnlyLib;

   if( !m_objects.remove( objectName ) )
   {
      PMLibraryHandle::EntryIterator itr( m_objects );
      for( ; itr.current( ); ++itr )
      {
         if( *(itr.current( )) == objectName )
         {
            m_objects.remove( itr.currentKey( ) );
            saveLibraryInfo( );
            return PMLibraryHandle::Ok;
         }
      }
   }
   return PMLibraryHandle::NotInLib;
}

PMLibraryHandle::PMResult PMLibraryHandle::createNewSubLibrary( const QString /*subLibName*/ )
{
   /// @todo Need to replace mkdtemp and mkstemps before enabling libs
   return PMLibraryHandle::CouldNotCreateFile;
   /*
   char* dirname;
   QCString s = m_path.toLatin1( );
   PMLibraryHandle aux;

   if( m_readOnly )
      return PMLibraryHandle::ReadOnlyLib;

   if( m_libraries.find( subLibName ) )
      return PMLibraryHandle::ExistingDir;

   s = s+ "/libXXXXXX";
   if ( !( dirname = mkdtemp( s.data( ) ) ) )
   {
      // Ooops! Error creating the file.
      return PMLibraryHandle::CouldNotCreateFile;
   }

   aux.setName( subLibName );
   aux.setAuthor( author( ) );
   aux.setPath( dirname );
   aux.saveLibraryInfo( );
   m_libraries.insert( subLibName, new QString( dirname ) );
   saveLibraryInfo( );
   return PMLibraryHandle::Ok;
   */
}

PMLibraryHandle::PMResult PMLibraryHandle::addSubLibrary( const QString& path, const QString& subLibName )
{
   if( m_readOnly )
      return PMLibraryHandle::ReadOnlyLib;

   m_libraries.insert( subLibName, new QString( path ) );
   saveLibraryInfo( );
   return PMLibraryHandle::Ok;
}

PMLibraryHandle::PMResult PMLibraryHandle::deleteSubLibrary( const QString& subLibName )
{
   if( m_readOnly )
      return PMLibraryHandle::ReadOnlyLib;

   if( !m_libraries.remove( subLibName ) )
   {
      PMLibraryHandle::EntryIterator itr( m_libraries );
      for( ; itr.current( ); ++itr )
      {
         if( *(itr.current( )) == subLibName )
         {
            m_libraries.remove( itr.currentKey( ) );
            saveLibraryInfo( );
            return PMLibraryHandle::Ok;
         }
      }
   }
   return PMLibraryHandle::NotInLib;
}

PMLibraryHandle::PMResult PMLibraryHandle::changeParentLibrary( const QString& parentPath )
{
   if( m_readOnly )
      return PMLibraryHandle::ReadOnlyLib;

   QString newPath = parentPath + "/" + m_path.section( '/', -1 );
   PMLibraryHandle::EntryIterator itr( m_libraries );
   for( ; itr.current( ); ++itr )
   {
      PMLibraryHandle lib = PMLibraryHandle( *itr.current( ) );
      lib.changeParentLibrary( newPath );
      m_libraries.replace( itr.currentKey( ), new QString( newPath + "/" + lib.path( ) ) );
   }

   PMLibraryHandle::EntryIterator objItr( m_objects );
   for( ; objItr.current( ); ++objItr )
   {
      QString test = newPath + "/" + objItr.current( )->section( '/', -1 );
      m_objects.replace( objItr.currentKey( ), new QString( newPath + "/" + objItr.current( )->section( '/', -1 ) ) );
   }

   saveLibraryInfo( );
   m_path = newPath;
   return PMLibraryHandle::Ok;
}

PMLibraryHandle::EntryIterator* PMLibraryHandle::createObjectIterator( )
{
   return new EntryIterator( m_objects );
}

PMLibraryHandle::EntryIterator* PMLibraryHandle::createSubLibraryIterator( )
{
   return new EntryIterator( m_libraries );
}

void PMLibraryHandle::setSubLibrary( bool isSubLibrary )
{
   m_subLibrary = isSubLibrary;
}
