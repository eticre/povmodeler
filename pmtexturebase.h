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


#ifndef PMTEXTUREBASE_H
#define PMTEXTUREBASE_H



#include <QList>

#include "pmnamedobject.h"
#include "pmdeclare.h"

/**
 * Base class for all textures that can link to declares
 */
class PMTextureBase : public PMNamedObject
{
   typedef PMNamedObject Base;
public:
   /**
    * Creates an PMTextureBase
    */
   PMTextureBase( PMPart* part );
   /**
    * Copy constructor
    */
   PMTextureBase( const PMTextureBase& b );
   /**
    * Deletes the object
    */
   virtual ~PMTextureBase();

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void restoreMemento( PMMemento* s );

   /** */
   virtual PMDeclare* linkedObject() const { return m_pLinkedObject; }
   /**
    * Sets the linked object. Returns true if successful
    */
   bool setLinkedObject( PMDeclare* o );
   /**
    * Returns true if a linked object is set. Used by the
    * insert possibilities framework.
    */
   bool hasLinkedObject() const { return m_pLinkedObject; }

   /**
    * Method used by the properties framework
    */
   PMObject* linkedObjectProperty() const { return m_pLinkedObject; }
   /**
    * Method used by the properties framework
    */
   void setLinkedObjectProperty( PMObject* o );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMTextureBaseMementoID { PMLinkedObjectID };
   PMDeclare* m_pLinkedObject;

   static PMMetaObject* s_pMetaObject;
};


#endif
