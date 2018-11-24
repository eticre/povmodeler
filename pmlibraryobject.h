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

#ifndef PMLIBRARYOBJECT_H
#define PMLIBRARYOBJECT_H



#include <QString>
#include <QMap>
#include <QList>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QtXml>

class QUrl;
class QImage;

/**
 * This class implements a library object.
 *
 * A library object has a name, a textual description, a graphical 
 * preview and the object data, of course. It also contains a collection of
 * keywords.
 *
 * When an instance of PMLibraryObject is created, the objects description is
 * loaded.
 *
 * The graphical preview and the objects data are loaded only if needed.
 *
 */
class PMLibraryObject
{
public:
   /** 
    * Constructor for the library object. Creates an empty library object. 
    */
   PMLibraryObject();
   /** 
    * Constructor for the library object. 
    * Loads the object data from the specified library object file.
    */
   PMLibraryObject( QString url );
   /** 
    * Destructor 
    */
   ~PMLibraryObject();

   /** 
    * Name of the library object. 
    */
   QString name() const { return m_name; }
   /** 
    * Textual description of the library object. 
    */
   QString description() const { return m_description; }
   /** 
    * List of keywords for search of the library object. 
    */
   QString keywords() const { return m_keywords; }
   /** 
    * Graphical Preview. 
    */
   QImage* preview();
   /**
    * True is the preview has been loaded.
    */
   bool isPreviewLoaded() const { return m_previewLoaded; }
   /** 
    * Objects for the scene 
    */
   QByteArray* objects();
   bool areObjectsLoaded() const { return m_objectsLoaded; }

   /** 
    * Set the library object name 
    */
   void setName( const QString& str );
   /** 
    * Set the library object description 
    */
   void setDescription( const QString& str );
   /** 
    * Set the library object keywords 
    */
   void setKeywords( const QString& str );
   /** 
    * Set the preview image 
    */
   void setPreview( const QImage& img );
   /** 
    * Set the object data 
    */
   void setObjects( const QByteArray& obj );

   /** 
    * Save the library object to a file 
    */
   void save( const QString& fileName );

private:
   void loadLibraryInfo();
   void saveLibraryInfo();
   void savePreview();
   void saveObjects();

   bool          m_previewLoaded;
   bool          m_objectsLoaded;
   QString       m_name;
   QString       m_description;
   QString       m_keywords;
   QString         m_path;
   QImage*       m_preview;
   QByteArray*   m_objects;
   QStringList   m_extraFiles;
   const QFile*  entry;
   QDomDocument* dom_doc;
};

#endif
