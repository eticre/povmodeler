/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmdocumentationmap.h"

#include <QFile>
#include <QtXml/qdom.h>
#include <QList>
#include <QStandardPaths>
#include <QSettings>

#include "pmdebug.h"

PMDocumentationMap* PMDocumentationMap::s_pInstance = nullptr;
//K3StaticDeleter<PMDocumentationMap> PMDocumentationMap::s_staticDeleter;

QString PMDocumentationVersion::documentation( const QString& className ) const
{
   if( m_map.contains( className ) )
      return m_map[className];
   return m_index;
}

void PMDocumentationVersion::loadData( QDomElement& e )
{
   QString className;
   QString target;
   
   m_version = e.attribute( "number", "3.1" );
   m_index = e.attribute( "index", "index.htm" );

   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         className = me.attribute( "className", "" );
         target = me.attribute( "target", "" );
         if( !className.isEmpty() && !target.isEmpty() )
            m_map.insert( className, target );
      }
      m = m.nextSibling();
   }
}

PMDocumentationMap::PMDocumentationMap()
{
   m_pCurrentVersion = nullptr;
   m_mapLoaded = false;
}

PMDocumentationMap::~PMDocumentationMap()
{
   while( !m_maps.isEmpty() )
      delete m_maps.takeFirst();
}

void PMDocumentationMap::saveConfig()
{
   QSettings qset;
   qset.setValue( "Povray/DocumentationPath", m_documentationPath );
   qset.setValue( "Povray/DocumentationVersion", m_currentVersion );
}

void PMDocumentationMap::restoreConfig()
{
   QSettings qset;
   m_documentationPath = qset.value( "Povray/DocumentationPath" ).toString();
   m_currentVersion = qset.value( "Povray/DocumentationVersion", "3.1" ).toString();
}

void PMDocumentationMap::setDocumentationVersion( const QString& v )
{
   m_currentVersion = v;
   if( m_mapLoaded )
      findVersion();
}

QList<QString> PMDocumentationMap::availableVersions()
{
   if( !m_mapLoaded )
      loadMap();

   QList<QString> result;
   foreach( PMDocumentationVersion* it, m_maps )
      result.append( it->version() );
   
   return result;
}

QString PMDocumentationMap::documentation( const QString& objectName )
{
   if( !m_mapLoaded )
      loadMap();
   
   QString url;

   if( !m_documentationPath.isEmpty() )
      if( !m_documentationPath.endsWith( QString( "/" ) ) )
         m_documentationPath += "/";
   
   if( !m_documentationPath.isEmpty() && m_pCurrentVersion )
      url = m_documentationPath
            + m_pCurrentVersion->documentation( objectName );

   return url;
}

void PMDocumentationMap::loadMap()
{
   if( !m_mapLoaded )
   {
      m_mapLoaded = true;
      
      QString fileName = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kpovmodeler/povraydocmap.xml" );
      if( fileName.isEmpty() )
      {
         qCritical(  ) << "Povray documentation map not found" << endl;
         return;
      }
      
      QFile file( fileName );
      if( !file.open( QIODevice::ReadOnly ) )
      {
         qCritical(  ) << "Could not open the povray documentation map file"
                           << endl;
         return;
      }

      QDomDocument doc( "DOCMAP" );
      doc.setContent( &file );

      QDomElement e = doc.documentElement();
      QDomNode c = e.firstChild();

      QString str;

      while( !c.isNull() )
      {
         if( c.isElement() )
         {
            QDomElement ce = c.toElement();
            PMDocumentationVersion* v = new PMDocumentationVersion();
            m_maps.append( v );
            v->loadData( ce );
         }
         c = c.nextSibling();
      }

      findVersion();
   }
}

void PMDocumentationMap::findVersion()
{
   m_pCurrentVersion = 0;
   foreach( PMDocumentationVersion* it, m_maps )
   {
      if( it->version() == m_currentVersion )
      {
         m_pCurrentVersion = it;
	 break;
      }
   }
}

PMDocumentationMap* PMDocumentationMap::theMap()
{
   if( !s_pInstance )
      s_pInstance = new PMDocumentationMap();

     // s_staticDeleter.setObject( s_pInstance, new PMDocumentationMap() );
   return s_pInstance;
}
