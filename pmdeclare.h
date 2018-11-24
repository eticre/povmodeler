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

#ifndef PMDECLARE_H
#define PMDECLARE_H



#include "pmcompositeobject.h"
#include "pmsymboltable.h"

/**
 * Class for all povray declares
 */
class PMDeclare : public PMCompositeObject
{
   typedef PMCompositeObject Base;
public:
   /**
    * Constructor
    */
   PMDeclare( PMPart* part );
   /**
    * Copy constructor
    */
   PMDeclare( const PMDeclare& d );
   /**
    * Deletes the object
    */
   ~PMDeclare();

   /** */
   virtual PMObject* copy() const { return new PMDeclare( *this ); }

   /** */
   virtual QString description() const;
   /** */
   virtual QString pixmap() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual bool dataChangeOnInsertRemove() const { return true; }
   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMDeclareEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the id of the declare
    */
   virtual QString name() const { return m_id; }
   /**
    * Returns the id of the declare
    */
   QString id() const { return m_id; }
   /**
    * Returns the declare type
    */
   QString declareType() const;
   /**
    * not visual in tree
    */
   bool declare_not_in_tree;
   /**
    * The declare is a function
    */
   bool is_a_function;
   //bool isAfunction() { return is_a_function; }
   /**
    * Sets the id of the object.
    */
   void setID( const QString& id );
   /** */
   virtual bool canHaveName() const { return true; }

   /** */
   virtual void restoreMemento( PMMemento* s );
   /**
    * Returns an iterator to the list of objects, that are linked to that
    * declare
    */
   const PMObjectList& linkedObjects() const
   {
      return m_linkedObjects;
   }
   /**
    * Adds the object to the list of linked objects
    */
   void addLinkedObject( PMObject* o );
   /**
    * Removes the object from the list of linked objects
    */
   void removeLinkedObject( PMObject* o );

   /** */
   virtual void childAdded( PMObject* o );
   /** */
   virtual void childRemoved( PMObject* o );

private:
   /**
    * Sets the declare type
    */
   void setDeclareType( PMMetaObject* o );
   /**
    * Recalculates the declaration type
    */
   void updateDeclareType();
   /**
    * IDs for @ref PMMementoData
    */
   enum PMDeclareMementoID { PMIDID };

   /**
    * id of the declare
    */
   QString m_id;
   /**
    * The linked objects
    */
   PMObjectList m_linkedObjects;
   /**
    * The declare type
    */
   PMMetaObject* m_pDeclareType;

   static PMMetaObject* s_pMetaObject;
};

#endif
