//-*-C++-*-
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

#ifndef PMLIBRARYHANDLE_H
#define PMLIBRARYHANDLE_H



#include <QString>
#include <QHash>


/**
 * Class that holds all the information about a specific library.
 * The class can also create the library if it doesn't exists.
 *
 * A library is nothing more than a directory in the file system, and
 * an index file stored in XML format.
 *
 * One library can have one or more sub-libraries. Sub-libraries can
 * only contain the entries of object files and more sub-libraries.
 *
 */
class PMLibraryHandle
{
public:
   enum PMResult { Ok, CouldNotCreateDir, ExistingDir,
                   CouldNotCreateInfo, ReadOnlyLib,
                   CouldNotCreateFile, NotInLib };

   /**
    * Iterator for the objects in the library
    */
   typedef QHashIterator<QString, QString*> EntryIterator;

   /**
    * Constructor for an empty library
    */
   PMLibraryHandle();

   /**
    * Constructor for a given directory.
    */
   PMLibraryHandle( const QString& path );

   /**
    * Destructor
    */
   ~PMLibraryHandle();
   /**
    * Returns the library's name
    */
   QString name() const { return m_name; }

   /**
    * Returns the library's path
    */
   QString path() const { return m_path; }

   /**
    * Returns the library's author
    */
   QString author() const { return m_author; }

   /**
    * Returns the library's description
    */
   QString description() const { return m_description; }

   /**
    * Sets the library's name
    */
   void setName( const QString& name );

   /**
    * Sets the library's path
    */
   void setPath( const QString& path );

   /**
    * Sets the library's author
    */
   void setAuthor( const QString& author );

   /**
    * Sets the library's description
    */
   void setDescription( const QString& description );

   /**
    * Sets the library's erad only status
    */
   void setReadOnly( const bool rdonly = true );

   /**
    * Save the library's information file.
    */
   PMLibraryHandle::PMResult saveLibraryInfo();

   /**
    * Create the library's information file.
    */
   PMLibraryHandle::PMResult createLibrary();

   /**
    * Append a new object to the library.
    * Returns PMLibraryHandle::Ok if successful or the reason of failure.
    */
   PMLibraryHandle::PMResult createNewObject();
   /**
    * Adds an already existing object to the library
    * @param path The path for the object
    * @param name The name of the object
    * @return PMLibraryHandle::Ok if successful or the reason of failure
    */
   PMLibraryHandle::PMResult addObject( const QString& path, const QString& name );
   /**
    * Deletes an object out of the library. Only removes the entry from the library
    * doesn't delete the objects file
    * @param objectName The name of the object ( or objects path ) to delete
    * @return PMLibraryHandle::Ok if successul or the reason of failure
    */
   PMLibraryHandle::PMResult deleteObject( const QString& objectName );

   /**
    * Create a new sub library.
    */
   PMLibraryHandle::PMResult createNewSubLibrary( const QString subLibName );
   /**
    * Adds an already existing sub-library to the library
    * @param path The path for the sub library
    * @param subLibName The name of the sub library
    * @return PMLibraryHandle::Ok if successful or the reason of failure
    */
   PMLibraryHandle::PMResult addSubLibrary( const QString& path, const QString& name );
   /**
    * Deletes a sub library. Only removes the entry from the library
    * doesn't delete the objects file
    * @param subLibName The name of the sub library ( or sub libraries path ) to delete
    * @return PMLibraryHandle::Ok if successul or the reason of failure
    */
   PMLibraryHandle::PMResult deleteSubLibrary( const QString& subLibName );

   /**
    * Returns true if the library is set read-only
    */
   bool isReadOnly() const { return m_readOnly; }

   /**
    * Returns an object iterator. It has to be deleted afterwards.
    */
   PMLibraryHandle::EntryIterator* createObjectIterator();

   /**
    * Returns a sub-library iterator. It has to be deleted afterwards.
    */
   PMLibraryHandle::EntryIterator* createSubLibraryIterator();

   /**
    * Returns true if the library is a sub library.
    */
   bool isSubLibrary() const { return m_subLibrary; }

   /**
    * Changes the parent library if this is a sub library
    * @param parentPath The new parent path for this library
    * @return PMLibraryHandle::Ok if successful or the reason for failure
    */
   PMLibraryHandle::PMResult changeParentLibrary( const QString& parentPath );

private:

   /**
    * Makes the library a sub library.
    */
   void setSubLibrary( bool isSubLibrary );


   void loadLibraryInfo();

   QString m_name;
   QString m_path;
   QString m_author;
   QString m_description;
   bool    m_readOnly;
   bool    m_subLibrary;
   QHash<QString, QString*> m_objects;
   QHash<QString, QString*> m_libraries;
};

#endif
