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

#ifndef PMBOUNDEDBY_H
#define PMBOUNDEDBY_H



#include "pmcompositeobject.h"

/**
 * class for bounded_by povray statements
 */

class PMBoundedBy : public PMCompositeObject
{
   typedef PMCompositeObject Base;
public:
   /**
    * Creates an empty PMBoundedBy
    */
   PMBoundedBy( PMPart* part );
   /**
    * Deletes the object
    */
   ~PMBoundedBy();

   /** */
   virtual PMObject* copy() const { return new PMBoundedBy( *this ); }

   /** */
   virtual QString description() const;

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
    * Returns a new @ref PMBoundedByEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmboundedby" ); }

   /** */
   virtual void childRemoved( PMObject* );
   /** */
   virtual void childAdded( PMObject* );

   /**
    * Returns true if the object contains no child objects (exmbumcept comments)
    */
   bool clippedBy() const;

private:
   enum PMBoundedByMementoID { PMClippedByID };
   static PMMetaObject* s_pMetaObject;
};

#endif
