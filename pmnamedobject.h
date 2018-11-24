//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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

#ifndef PMNAMEDOBJECT_H
#define PMNAMEDOBJECT_H



#include "pmcompositeobject.h"

/**
 * class for all objects with free choose-able names (all objects except
 * include files, the scene and declares)
 */

class PMNamedObject : public PMCompositeObject
{
   typedef PMCompositeObject Base;
public:
   /**
    * Creates an empty PMNamedObject
    */
   PMNamedObject( PMPart* part );
   /**
    * Copy constructor
    */
   PMNamedObject( const PMNamedObject& o );
   /**
    * Deletes the object
    */
   ~PMNamedObject();

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns the name of the object. This is the name that helps
    * the user to identify a object (like "south_wall", "floor" ...) */
   virtual QString name() const { return m_name; }
   /**
    * Sets the name of the object.
    */
   virtual void setName( const QString& newName );
   /**
    * Returns true if the object can have a name
    */
   virtual bool canHaveName() const { return true; }

   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMNamedObjectMementoID { PMNameID };

   /**
    * Name of the object
    */
   QString m_name;

   static PMMetaObject* s_pMetaObject;
};

#endif
