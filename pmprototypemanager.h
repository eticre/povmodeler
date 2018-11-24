//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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


#ifndef PMPTMANAGER_H
#define PMPTMANAGER_H

#include "pmobject.h"
#include <QMap>
#include <QList>

class PMPart;
/**
 * Description class for declarations types, used by @ref PMPrototypeManager
 * and @ref PMDeclare
 */
class PMDeclareDescription
{
public:
   PMDeclareDescription() { }
   PMDeclareDescription( const PMDeclareDescription& d )
   {
      type = d.type;
      description = d.description;
      pixmap = d.pixmap;
   }
   PMDeclareDescription( PMMetaObject* t, const QString& d, const QString& p )
   {
      type = t;
      description = d;
      pixmap = p;
   }
   PMDeclareDescription& operator=( const PMDeclareDescription& d )
   {
      type = d.type;
      description = d.description;
      pixmap = d.pixmap;
      return *this;
   }
   PMMetaObject* type;
   QString description;
   QString pixmap;
};

/**
 * Prototype manager for @ref PMObject.
 *
 * This class stores class and inheritance information for each
 * available object type.
 *
 * Each @ref PMPart class holds one instance of this class. The
 * available objects depend on the loaded plugins.
 *
 * Patterns: Prototype
 */
class PMPrototypeManager
{
public:
   /**
    * Creates a prototype manager for the part.
    */
   PMPrototypeManager( PMPart* part );
   /**
    * Deletes the prototype manager
    */
   ~PMPrototypeManager();
   /**
    * Adds the object to the list of prototypes. The prototype becomes
    * the owner of the object and will be delete immediately by the
    * prototype manager.
    */
   void addPrototype( PMObject* obj );
   /**
    * Adds a declaration type. Needed information is the class type,
    * the @ref description() and the @ref pixmap()
    */
   void addDeclarationType( const QString& className,
                            const QString& description,
                            const QString& pixmap );
   /**
    * Returns the list of available objects
    */
   const QList<PMMetaObject*>& prototypes() const;
   /**
    * Returns the list of available declaration types
    */
   const QList<PMDeclareDescription>& declarationTypes() const;
   /**
    * Returns a new PMObject by class name
    */
   PMObject* newObject( const QString& name ) const;
   /**
    * Returns the meta object by class name or 0 if this class does
    * not exist.
    * @see PMMetaObject
    */
   PMMetaObject* metaObject( const QString& name ) const;
   /**
    * Returns true if the class exists
    */
   bool existsClass( const QString& name ) const
   {
      return metaObject( name );
   }
   /**
    * Returns true if the second class is a base class for
    * the first class
    */
   bool isA( const QString& className, const QString& baseClassName ) const;
   /**
    * Returns true if the second class is a base class for
    * the first class
    */
   bool isA( PMMetaObject* c, const QString& baseClassName ) const;
   /**
    * Returns the real class if only the lower case version is know.
    * Used by the xml parser
    */
   QString className( const QString& lowercase ) const;
   /**
    * Returns a pointer to the part
    */
   PMPart* part() const { return m_pPart; }

private:
   QList<PMMetaObject*> m_prototypes;
	QMap<QString, PMMetaObject*> m_metaDict;
   QMap<QString, QString> m_lowerCaseDict;
   QList<PMDeclareDescription> m_declareDescriptions;
   PMPart* m_pPart;
};
#endif
