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

#include "pmlibrarymanager.h"


#include <QFile>
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include "pmdebug.h"

PMLibraryManager* PMLibraryManager::s_pInstance = nullptr;

PMLibraryHandle* PMLibraryManager::getLibraryHandle( const QString& libraryName ) 
{
    QList<PMLibraryHandle>::iterator i;
    for ( i = m_libraries.begin(); i != m_libraries.end(); ++i )
    if( (*i).name() == libraryName )
       return &((*i));

    return nullptr;
}

PMLibraryManager::PMLibraryManager()
{
   //m_libraries.setAutoDelete( true );
   scanLibraries();
}

PMLibraryManager::~PMLibraryManager()
{
   m_libraries.clear();
}

void PMLibraryManager::saveConfig()
{
}

void PMLibraryManager::restoreConfig()
{
}

QStringList PMLibraryManager::availableLibraries()//eticre debug error 1
{
   QStringList result;
   QList<PMLibraryHandle>::iterator i;
   for ( i = m_libraries.begin(); i != m_libraries.end(); ++i )
       result.push_back( (*i).name() );

   return result;
}

PMLibraryManager* PMLibraryManager::theManager()
{
   if( !s_pInstance )
      s_pInstance = new PMLibraryManager();

   return s_pInstance;
}

void PMLibraryManager::scanLibraries()//eticre debug error 2
{
   QStringList libraryDirectories;
   
   // Search for sub directories in /.local/share/povmodeler/library
   //libraryDirectories = KGlobal::dirs()->findDirs( "data", "kpovmodeler/library" );
   QString lib_path;
   lib_path = QDir::homePath() + "/.local/share/povmodeler/library";
   QDir lib_dir( lib_path );
   libraryDirectories = lib_dir.entryList( QDir::NoDotAndDotDot | QDir::Dirs );
   for( QStringList::Iterator i = libraryDirectories.begin(); i != libraryDirectories.end(); ++i )
   {
      /* QDir curDir( lib_path + "/" + *i );//library dir eticre debug 6
       curDir.setFilter( QDir::NoDotAndDotDot | QDir::Files );
       QFileInfoList entries = curDir.entryInfoList();//files in library dir*/

       //QFileInfoListIterator it;
       //QListIterator<QFileInfo> it( entries );
       //QFileInfoList::Iterator it = entries.begin();

      // For each sub directory
      //while( it.hasNext() )
      //for( it = entries.begin(); it != entries.end(); ++it )

      //for( QFileInfoList::Iterator it = entries.begin(); it != entries.end(); ++it )
      //{
         // check for the existance of library_index.xml
         // If it's there it's a library
         if( QFile::exists( lib_path + "/" + *i + "/library_index.xml" ) )
         {
            // Create the corresponding PMLibraryHandle
            PMLibraryHandle h;

            h = PMLibraryHandle( lib_path + "/" + *i );
            if( !getLibraryHandle( h.name() ) )
               m_libraries.append( h );
            //else
               // a library with that name already exists
               //h.~PMLibraryHandle();
         }
      //}
   }
}

void PMLibraryManager::refresh()
{
   // TODO: Manage the list incrementaly so that previouly handed out 
   // PMLibraryHandle pointers are kept valid
   m_libraries.clear();
   scanLibraries();
}
