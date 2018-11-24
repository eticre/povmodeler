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


#ifndef PMOBJECTMEMENTO_H
#define PMOBJECTMEMENTO_H



#include "pmvariant.h"
#include "pmcommand.h"
#include "pmmetaobject.h"
#include <qstring.h>
#include <QList>

/**
 * Class used by @ref PMObjectMemento to store one value.
 *
 * Each piece of data has two IDs: The type of the povray class that stored
 * the data and a unique ID within this class.
 *
 * @see PMMementoDataIterator
 */

class PMMementoData : public PMVariant
{
public:
   /**
    * Stores an integer
    */
   PMMementoData( PMMetaObject* classType, int vID, int data );
   /**
    * Stores an unsigned integer
    */
   PMMementoData( PMMetaObject* classType, int vID, unsigned int data );
   /**
    * Stores double
    */
   PMMementoData( PMMetaObject* classType, int vID, double data );
   /**
    * Stores a boolean
    */
   PMMementoData( PMMetaObject* classType, int vID, bool data );
   /**
    * Stores a @ref PMThreeState
    */
   PMMementoData( PMMetaObject* classType, int vID, PMThreeState data );
   /**
    * Stores a string
    */
   PMMementoData( PMMetaObject* classType, int vID, const QString& data );
   /**
    * Stores a @ref PMVector
    */
   PMMementoData( PMMetaObject* classType, int vID, const PMVector& data );
   /**
    * Stores a @ref PMColor
    */
   PMMementoData( PMMetaObject* classType, int vID, const PMColor& data );
   /**
    * Stores a pointer to a PMObject
    */
   PMMementoData( PMMetaObject* classType, int vID, PMObject* obj );

   /**
    * Returns the object type of the povray class that stored the data
    */
   PMMetaObject* objectType() const { return m_objectType; }
   /**
    * Returns the ID of the stored value
    */
   int valueID() const { return m_valueID; }
   
private:
   /**
    * Class that stored the information
    */
   PMMetaObject* m_objectType;
   /**
    * The unique ID within the m_objectType
    */
   int m_valueID;
};

/**
 * Helper class to store information about changed objects in @ref PMMemento
 */
class PMObjectChange
{
public:
   /**
    * Creates change information for the object obj.
    *
    * mode is a bitwise combination of @ref PMChange constants.
    */
   PMObjectChange( PMObject* obj, int mode )
   {
      m_pObject = obj;
      m_mode = mode;
   }
   PMObject* object() const { return m_pObject; }
   int mode() const { return m_mode; }
   void addMode( int mode ) { m_mode |= mode; }
private:
   PMObject* m_pObject;
   int m_mode;
};

/**
 * Class that stores the data of povray objects for undo/redo information
 *
 * This class should provide enough functionality for most objects. If not,
 * subclass this class.
 *
 * All objects have to use the memento class of its base class or an inherited
 * one.
 *
 * Only the changed attributes of an object are saved.
 *
 * See class @ref PMMementoData to see how values are stored.
 */
class PMMemento
{
public:
   /**
    * Creates a memento for the object originator
    */
   PMMemento( PMObject* originator );
   /**
    * Deletes the memento
    */
   virtual ~PMMemento();

   /**
    * Returns a pointer to the originator
    */
   PMObject* originator() const { return m_pOriginator; }
   /**
    * Returns a pointer to the memento data or 0 if this value is not
    * stored
    */
   PMMementoData* findData( PMMetaObject* classType, int vID ) const;
   /**
    * Adds the data object to the list of stored data. The menento may not
    * contain this information (objType and vID).
    *
    * The memento becomes the owner of this object.*/
   void addData( PMMementoData* data );

   /**
    * Adds an integer data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const int data );

   /**
    * Adds an unsigned data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const unsigned int data );

   /**
    * Adds an double data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const double data );

   /**
    * Adds an bool data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const bool data );

   /**
    * Adds an @ref PMThreeState data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const PMThreeState data );

   /**
    * Adds an @ref QString data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const QString& data );

   /**
    * Adds an @ref PMVector data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const PMVector& data );

   /**
    * Adds an @ref PMColor data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, const PMColor& data );

   /**
    * Adds an PMObject pointer data object. Will be ignored if the memento
    * already contains this data
    */
   void addData( PMMetaObject* classType, int vID, PMObject* obj );
   
   /**
    * Call this to store an id change
    */
   void addIDChange( PMMetaObject* classType, int vID, const QString& data );

   /**
    * Returns true if the memento contains changed data
    */
   bool containsChanges() const { return ( m_changedObjects.count() > 0 ); }
   
   /**
    * Call this function, if you stored some data that changed
    * the view structure of the originator
    */
   void setViewStructureChanged() { addChange( PMCViewStructure ); }
   /**
    * Call this function, if you stored some data that changed
    * the name or pixmap of the originator
    */
   void setDescriptionChanged() { addChange( PMCDescription ); }
   
   /**
    * Returns true if the id of a @ref PMDeclare was changed
    */
   bool idChanged() const { return m_pIDData != 0; }
   /**
    * Returns the old id
    */
   QString oldID() const;
   /**
    * If one object is changed, other objects can be changed as well.
    *
    * For example, if the linked declare of an object link is changed,
    * the old and new declare are changed as well (Remove the old link,
    * add the new link).
    *
    * This function returns an iterator to the list of all changed objects.
    * The first object is the originator.
    */
   QList<PMObjectChange*>& changedObjects() { return m_changedObjects; }
   /**
    * Adds the object to the list of changed objects.
    * Note that the originator is added automatically
    */
   void addChangedObject( PMObject* obj, int mode );
	/**
	 * Returns a list of data changes
	 */
	const QList<PMMementoData*>& changes() const { return m_data; }
protected:
   /**
    * Adds the change to the originator change object
    */
   void addChange( int mode );
private:   
   /**
    * The stored values
    */
   QList<PMMementoData*> m_data;
   /**
    * Additional pointer to the memento data for id changes of
    * @ref PMDeclare objects
    */
   PMMementoData* m_pIDData;
   /**
    * List of changes
    */
   QList<PMObjectChange*> m_changedObjects;
   PMObjectChange* m_pOriginatorChange;
   PMObject* m_pOriginator;
};

#endif
