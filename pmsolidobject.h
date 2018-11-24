//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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


#ifndef PMSOLIDOBJECT_H
#define PMSOLIDOBJECT_H



#include "pmgraphicalobject.h"


/**
 * Class for povray solid objects
 */
class PMSolidObject : public PMGraphicalObject
{
   typedef PMGraphicalObject Base;
public:
   /**
    * Creates an empty PMSolidObject
    */
   PMSolidObject( PMPart* part );
   /**
    * Copy constructor
    */
   PMSolidObject( const PMSolidObject& s );

   /**
    * Deletes the object and all children
    */
   virtual ~PMSolidObject();
   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns the state of the hollow flag.
    * Values can be PMTrue, PMFalse, PMUnspecified.
    */
   PMThreeState hollow() const { return m_hollow; }
   /**
    * Sets the hollow flag. Values can be PMTrue, PMFalse, PMUnspecified.
    */
   void setHollow( PMThreeState h );

   /**
    * Returns the state of the inverse flag.
    */
   bool inverse() const { return m_inverse; }
   /**
    * Sets the inverse flag
    */
   void setInverse( bool yes );

   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMSolidObjectMementoID { PMInverseID, PMHollowID };

   bool m_inverse;
   PMThreeState m_hollow;

   static PMMetaObject* s_pMetaObject;
};


#endif
