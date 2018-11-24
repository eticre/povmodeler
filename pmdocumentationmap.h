//-*-C++-*-
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

#ifndef PMDOCUMENTATIONMAP_H
#define PMDOCUMENTATIONMAP_H



#include <qstring.h>
#include <qmap.h>
#include <QList>

class QDomElement;

/**
 * Class used internally by @ref PMDocumentationMap
 */
class PMDocumentationVersion
{
public:
   /**
    * Constructor
    */
   PMDocumentationVersion() { }
   /**
    * Destructor
    */
   ~PMDocumentationVersion() { }

   QString version() const { return m_version; }
   void setVersion( const QString& str ) { m_version = str; }
   QString index() const { return m_index; }
   void setIndex( const QString& str ) { m_index = str; }

   QString documentation( const QString& className ) const;

   void loadData( QDomElement& e );
      
private:
   QString m_version;
   QString m_index;
   QMap< QString, QString > m_map;
};

/**
 * Class that maps the class name to the corresponding povray
 * user documentation file
 */
class PMDocumentationMap
{
public:
   /**
    * Destructor
    */
   ~PMDocumentationMap();
   /**
    * Returns the map instance (singleton)
    */
   static PMDocumentationMap* theMap();

   /**
    * Sets the path to the povray user documentation
    */
   void setPovrayDocumentationPath( const QString& s )
   {
      m_documentationPath = s;
   }
   /**
    * Returns the path to the povray user documentation
    */
   QString povrayDocumentationPath()
   {
      return m_documentationPath;
   }
   
   /**
    * Sets the povray documentation version
    */
   void setDocumentationVersion( const QString& str );
   /**
    * Returns the povray documentation version
    */
   QString documentationVersion() const { return m_currentVersion; }

   /**
    * Returns the list of available documentation versions
    */
   QList<QString> availableVersions();

   /**
    * Returns the povray documentation file for the
    * given object type
    */
   QString documentation( const QString& objectName );
   
   void saveConfig();
   void restoreConfig();

private:
   
   /**
    * Constructor
    */
   PMDocumentationMap();
   void loadMap();
   void findVersion();

   QString m_documentationPath;
   bool m_mapLoaded;
   QList< PMDocumentationVersion* > m_maps;
   PMDocumentationVersion* m_pCurrentVersion;
   QString m_currentVersion;

   static PMDocumentationMap* s_pInstance;
   //static K3StaticDeleter<PMDocumentationMap> s_staticDeleter;
};

#endif
