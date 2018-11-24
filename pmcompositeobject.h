//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMCOMPOBJECT_H
#define PMCOMPOBJECT_H



#include "pmobject.h"

/**
 * Base class for all povray objects that can have child objects.
 *
 * Used pattern: Composite
 */
class PMCompositeObject : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates an empty PMCompositeObject
    */
   PMCompositeObject( PMPart* part );
   /**
    * Copy constructor
    */
   PMCompositeObject( const PMCompositeObject& o );
   /**
    * Deletes the object and all children.
    */
   virtual ~PMCompositeObject();

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /**
    * Returns a pointer to the first child.
    */
   virtual PMObject* firstChild() const { return m_pFirstChild; }
   /**
    * Returns a pointer to the last child.
    */
   virtual PMObject* lastChild() const { return m_pLastChild; }
   /**
    * Returns a pointer to the child object at position index,
    * or null if the index is out of range.
    */
   virtual PMObject* childAt( uint index ) const;





   /**
    * Returns true if the object contains the child object o
    */
   virtual bool containsChild( PMObject* o ) const
   { return ( ( PMObject* )this == o->parent() ); }
   /**
    * Returns the index of the object or -1 if not found
    */
   virtual int findChild( PMObject* o );

   /**
    * Inserts the object as child at index i.
    * If i is -1, the object is appended.
    * Returns true if successful.
    */
   virtual bool insertChild( PMObject* o, int i );
   /**
    * Inserts the object as child after the child object after
    */
   virtual bool insertChildAfter( PMObject* object, PMObject* after );
   /**
    * Inserts the object as child before the child object before
    */
   virtual bool insertChildBefore( PMObject* object, PMObject* before );
   /**
    * Appends the object as child. Returns true if successful.
    */
   virtual bool appendChild( PMObject* );
   /**
    * Returns the number of children.
    */
   virtual int countChildren() const;
   /**
    * Removes a child object. Does not delete it!
    * Returns true if successful.*/
   virtual bool takeChild( PMObject* o );
   /**
    * Removes a child object at index i. Does not delete it!
    */
   virtual bool takeChild( uint i );

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;

   /**
    * Returns the number of selected child items. All selected items in
    * any depth are counted
    */
   virtual int selectedChildren() const { return m_selectedChildren; }
   /**
    * Deselects recursively all child objects
    */
   virtual void deselectChildren();

   /**
    * Returns the view structure of the object.
    *
    * If the view structure has changed since the last call, the
    * structure is created or updated, or the default view structure
    * is used.
    *
    * If the default view structure can't be used, this function calls
    * createViewStructure, which has to create a non default view structure.
    */
   virtual PMViewStructure* viewStructure();
protected:
   /**
    * Adds num to the number of selected objects in this object and all
    * parent objects. num can be negative.
    */
   virtual void adjustSelectedChildren( int num );

   /**
    * Returns the default view structure for the object. This view
    * structure can be shared between all objects of this type.
    *
    * Only the data can be shared, NOT the pointer!!!
    *
    * The default view structure has to be created when this function
    * is called first.
    */
   virtual PMViewStructure* defaultViewStructure() const { return 0; }
   /**
    * Returns a key that represents the view structure parameter.
    *
    * Each time a view structure parameter is changed (=detail level),
    * this key has to be incremented.
    */
   virtual int viewStructureParameterKey() const { return 0; }

   /**
    * Returns true if the objects attributes are default and the default
    * view structure can be used
    */
   virtual bool isDefault() { return false; }
   /**
    * The object has to create or update a non default view structure
    */
   virtual void createViewStructure() { }
   /**
    * Calls setViewStructureChanged() for the memento (if one has been created)
    * and marks the current view structure as outdated.
    */
   void setViewStructureChanged();
   /**
    * The view structure.
    */
   PMViewStructure* m_pViewStructure;
private:
   /**
    * Pointer to the first child
    */
   PMObject* m_pFirstChild;
   /**
    * Pointer to the last child
    */
   PMObject* m_pLastChild;
   /**
    * number of selected child items.
    */
   int m_selectedChildren;
   /**
    * The modify flag for the view structure
    */
   bool m_bViewStructureChanged;
   static PMMetaObject* s_pMetaObject;
};

#endif
