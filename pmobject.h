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


#ifndef PMOBJECT_H
#define PMOBJECT_H


#include "pmmatrix.h"
#include "pmcontrolpoint.h"
#include "pmmetaobject.h"
#include "pmdebug.h"
#include "pmtreeviewitem.h"
#include <QtXml/qdom.h>
#include <QImage>


class PMDialogEditBase;
class PMMemento;
class PMViewStructure;
class PMXMLHelper;
class PMDeclare;
class PMObjectAction;
class PMPart;

class PMObject;
typedef QList<PMObject*> PMObjectList;

/**
 * Base class for all povray objects
 *
 * Used pattern: Composite
 *
 * All list/child functionality is disabled in this class. Objects that
 * can have children has to be inherited from @ref PMCompositeObject.
 */
class PMObject
{
   friend class PMCompositeObject;
public:

   /**
    * point to connect obj and treview item
    */
   PMTreeViewItem* getTreeItem() {  if ( pmt_item ) return pmt_item; else return nullptr; }
   void setTreeItem( PMTreeViewItem* itm ) { pmt_item = itm; }
   /**
    * set Preview image
    */
   void setPreviewImage( QImage img ) { m_preview = img; }
   /**
    * get Preview image
    */
   virtual QImage getPreviewImage() { return m_preview; }
   /**
    * Hide tedious object from tree.
    */
   bool not_visible_in_tree;
   bool isVisibleInTree() const;
   void setTreeVisibility( bool nn );
   /**
    * used in qmap of preview image
    */
   int map_image_preview;
   /**
    * Creates an empty PMObject without parent.
    */
   PMObject( PMPart* part );
   /**
    * Copy constructor. All object pointers (parent, siblings) are set to 0!
    */
   PMObject( const PMObject& o );
   /**
    * Deletes the object and all children.
    */
   virtual ~PMObject();

   /**
    * Returns a new object of that type
    */
   PMObject* newObject() const;
   /**
    * Returns a deep copy of the object
    */
   virtual PMObject* copy() const = 0;

   /**
    * Returns the meta object for the class
    */
   virtual PMMetaObject* metaObject() const;

   /**
    * Returns true if the object is of type t or inherits the object
    * class with type t
    */
   bool isA( const QString& className ) const;

   /**
    * Returns the class name (not i18n'ed, without the PM prefix)
    */
   QString type() const;
   /**
    * same as @ref type()
    */
   QString className() const { return type(); }
   /**
    * Returns the class name of the object (povray name).
    * This is the name that is showed in dialogs and menus.
    */
   virtual QString description() const = 0;
   /**
    * Returns the name of the object. This is the name that helps
    * the user to identify a object (like "south wall", "floor" ...)
    */
   virtual QString name() const { return QString(); }
   /**
    * Returns true if the object can have a name
    */
   virtual bool canHaveName() const { return false; }
   /**
    * Returns true if the object should be exported for rendering
    */
   virtual bool exportPovray() const { return true; }

   /**
    * Returns a pointer to the parent object.
    */
   PMObject* parent() const { return m_pParent; }
   /**
    * Returns a pointer to the corresponding part
    */
   PMPart* part() const { return m_pPart; }

   /**
    * Returns true if an object with type className can be inserted
    * as child after the object after.
    *
    * The parser uses the third parameter for top level objects. These objects
    * have to be treated as if they are inserted after the object after.
    */
   bool canInsert( const QString& className, const PMObject* after,
                   const PMObjectList* objectsBetween = nullptr ) const;
   /**
    * Returns true if the object o can be inserted as child after the object
    * after.
    *
    * The parser uses the third parameter for top level objects. These objects
    * have to be treated as if they are inserted after the object after.
    */
   bool canInsert( const PMObject* o, const PMObject* after,
                   const PMObjectList* objectsBetween = nullptr ) const;
   /**
    * Returns the number of objects that can be inserted at that position
    */
   int canInsert( const PMObjectList& list, const PMObject* after ) const;
   /**
    * Returns the number of objects that can be inserted at that position
    */
   int canInsert( const QStringList& classes, const PMObject* after ) const;

   /**
    * Returns true if an insert or remove operation of children will
    * change data inside this class
    */
   virtual bool dataChangeOnInsertRemove() const { return false; }

   /**
    * Returns a pointer to the first child. Null for this class
    */
   virtual PMObject* firstChild() const { return nullptr; }
   /**
    * Returns a pointer to the last child. Null for this class
    */
   virtual PMObject* lastChild() const { return nullptr; }
   /**
    * Returns a pointer to the child object at position index,
    * or null if the index is out of range.
    */
   virtual PMObject* childAt( uint ) const { return nullptr; }
   /**
    * Returns the next sibling of that item
    */
   PMObject* nextSibling() const { return m_pNextSibling; }
   /**
    * Returns the previous sibling of that item
    */
   PMObject* prevSibling() const { return m_pPrevSibling; }

   /**
    * Returns true if the object contains the child object o
    */
   virtual bool containsChild( PMObject* ) const { return false; }
   /**
    * Returns the index of the child or -1 if not found
    */
   virtual int findChild( PMObject* ) { return -1; }
   /**
    * Inserts the object as child at index index.
    * If i is -1, the object is appended.
    * Returns true if successful
    */
   virtual bool insertChild( PMObject*, int )
   {
      //qCritical(  ) << "Tried to insert object into a non composite object" << "\n";
      return false;
   }
   /**
    * Inserts the object as child after the child object after
    */
   virtual bool insertChildAfter( PMObject* object, PMObject* after );
   /**
    * Inserts the object as child before the child object before
    */
   virtual bool insertChildBefore( PMObject* object, PMObject* before );
   /**
    * Appends the object as last child. Returns true if successful
    */
   virtual bool appendChild( PMObject* )
   {
      qCritical() << "Tried to insert object into a non composite object" << "\n";
      return false;
   }
   /**
    * Returns the number of children. 0 in this class
    */
   virtual int countChildren() const { return 0; }
   /**
    * Removes a child object. Does not delete it!
    * Returns true if successful
    */
   virtual bool takeChild( PMObject* );
   /**
    * Removes a child object at index i. Does not delete it!
    * Returns true if successful
    */
   virtual bool takeChild( uint );

   /**
    * Called when a child was removed. For classes that have to be informed
    * when children are removed
    */
   virtual void childRemoved( PMObject* ) { }
   /**
    * Called when a child was added. For classes that have to be informed
    * when children are added
    */
   virtual void childAdded( PMObject* ) { }

   /**
    * Returns true if the object needs auxiliary files.
    */
   virtual bool needsAuxiliaryFiles() const { return false; }
   /**
    * Returns a list of auxiliary files
    */
   virtual QStringList auxiliaryFiles() const { return QStringList(); }

   /**
    * Returns true if the object has a (povray) transformation matrix.
    * True for transformation objects.
    */
   virtual bool hasTransformationMatrix() const { return false; }
   /**
    * Returns the (povray) transformation of the object, if it has one,
    * otherwise an identity matrix
    */
   virtual PMMatrix transformationMatrix() const
   {
      qCritical() << "This object has no transformation matrix" << "\n";
      return PMMatrix::identity();
   }
   /**
    * Returns the matrix, the object is transformed with
    */
   PMMatrix transformedWith() const;

   /**
    * Returns the view structure ( see @ref PMViewStructure ) of the object
    * or 0, if it has none.
    *
    * The view structure will be created or updated if necessary.
    */
   virtual PMViewStructure* viewStructure() { return nullptr; }

   /**
    * Creates the control points and appends them to the list.
    *
    * The caller becomes the owner of the control points and has
    * to delete them.
    *
    * Control points are the interface between the graphical views and the
    * PMObject for graphical changes.
    */
   virtual void controlPoints( PMControlPointList& ) { }
   /**
    * Tells the object that the control points have changed
    */
   virtual void controlPointsChanged( PMControlPointList& ) { }
   /**
    * Tells the object that the control points have changed
    *
    * The object can add objects to the second parameter, if
    * additional objects were changed during the graphical change,
    * not only this object.
    *
    * If you leave the list empty, only this object was changed.
    * If you add children or other objects to the list, add this object
    * to the list, too, if it was changed!
    *
    * IMPORTANT: When you change additional objects, make sure that
    * a memento is created for these objects (@ref mementoCreated,
    * @ref createMemento) before changing data.
    *
    * Calls @ref controlPointsChanged by default.
    */
   virtual void controlPointsChangedList( PMControlPointList& list,
                                          PMObjectList& /*changedObjects*/ )
   {
      controlPointsChanged( list );
   }
   /**
    * Returns true if multiple control points can be selected
    */
   virtual bool multipleSelectControlPoints() const { return false; }

   /**
    * Each object can add actions to the context menu of the @ref PMGLView.
    *
    * Each time the user opens the context menu, this function is called.
    * Add all supported actions to this list and call the base class.
    *
    * The default implementation adds no action.
    *
    * @see PMObjectAction
    */
   virtual void addObjectActions( const PMControlPointList&,
                                  QList<PMObjectAction*>& ) { }

   /**
    * This member is called when the user selects an object action
    * in the context menu.
    *
    * The arguments are the selected action, the list of control points,
    * the 3D (!) positions of the control points (screen-x, screen-y and depth)
    * in the active opengl view and the 2D mouse position.
    */
   virtual void objectActionCalled( const PMObjectAction*,
                                    const PMControlPointList&,
                                    const QList<PMVector*>&,
                                    const PMVector& ) { }

   /**
    * Saves the object as povmodeler xml code.
    */
   QDomElement serialize( QDomDocument& doc ) const;
   /**
    * Adds the objects attributes and child objects to the element
    */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const = 0;
   /**
    * Reads the attributes from the QDomElement
    */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns the list of known properties
    */
   QStringList properties() const;

   /**
    * Sets a property and returns true if successful
    */
   bool setProperty( const QString&, const PMVariant& v );
   /**
    * Returns a property
    */
   PMVariant property( const QString& ) const;

   /**
    * Returns true if the object is selected
    */
   bool isSelected() { return m_selected; }
   /**
    * Sets this object to be selected if s is true and to not be selected
       if s is false
    */
   void setSelected( bool s );
   /**
    * Returns true if this item can be selected. An item cannot be selected
    * if a parent object is selected
    */
   bool isSelectable();
   /**
    * Returns the number of selected child items. All selected items in
    * any depth are counted
    */
   virtual int selectedChildren() const { return 0; }
   /**
    * Deselects recursively all child objects
    */
   virtual void deselectChildren() { }
   /**
    * Returns true if this object is read only
    */
   bool isReadOnly() const;
   /**
    * Makes this object read only, if yes == true. All children will
    * be read only, too
    */
   void setReadOnly( bool yes = true ) { m_readOnly = yes; }

   /**
    * Creates a new edit widget that can display this object and
    * returns a pointer to it.
    *
    * The widget will be created as a child of parent.
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const = 0;
   /**
    * Returns a pointer to the @ref PMDeclare object, that is linked to
    * that object, or 0, if this object contains no link
    */
   virtual PMDeclare* linkedObject() const { return nullptr; }

   /**
    * Tells the object to create a memento object.
    *
    * By default a simple @ref PMMemento will be created.
    * If the memento doesn't provide enough functionality, subclass the memento
    * and reimplement this function.
    *
    * NOTE: The new memento has to inherit the memento used in the base class!
    *
    * If a memento was created, the object saves all changes its
    * state. The memento is then used for undo/redo.
    *
    * First call this function, change attributes and then take the mementos
    * with @ref takeMemento
    *
    * If you want to restore the old state, call @ref restoreMemento with
    * the memento.
    */
   virtual void createMemento();
   /**
    * Returns true if a memento was created
    */
   bool mementoCreated() const { return m_pMemento != nullptr; }
   /**
    * Takes the memento. The caller is responsible to delete the memento
    */
   PMMemento* takeMemento();
   /**
    * Restores the state s
    */
   virtual void restoreMemento( PMMemento* s );

   /**
    * This function is called before the part is unloaded.
    *
    * Delete static pointer variables to clean up the memory.
    *
    * This method can be called multiple times when there are subclasses.
    * Set the pointer to 0 after deleting an object!
    */
   virtual void cleanUp() const;

protected:
   /**
    * Adds num to the number of selected objects in this object and all
    * parent objects. num can be negative.
    */
   virtual void adjustSelectedChildren( int /*num*/ ) { }
   /**
    * The memento where the old status of the object should be stored
    */
   PMMemento* m_pMemento;
private:
   /**
    * Pointer to treewidgetitem
    */
   PMTreeViewItem* pmt_item;
   /**
    * Pointer to the parent object. 0 if the object has no parent.
    */
   PMObject* m_pParent;
   /**
    * Pointer to the prev sibling. 0 if the object has no prev sibling.
    */
   PMObject* m_pPrevSibling;
   /**
    * Pointer to the next sibling. 0 if the object has no next sibling.
    */
   PMObject* m_pNextSibling;
   /**
    * true if this object is selected
    */
   bool m_selected;
   /**
    * true if this object is read only. All children will be read only, too
    */
   bool m_readOnly;
   /**
    * The meta object
    */
   static PMMetaObject* s_pMetaObject;
   /**
    * The corresponding part
    */
   PMPart* m_pPart;
   /**
    * Preview image
    */
   QImage m_preview;
};

#endif
