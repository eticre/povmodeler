//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2004 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMDETAILOBJECT_H
#define PMDETAILOBJECT_H



#include "pmnamedobject.h"

class PMDetailObject : public PMNamedObject
{
   typedef PMNamedObject Base;
public:
   /**
    * Creates an empty PMDetailObject
    */
   PMDetailObject( PMPart* part );
   /**
    * Copy constrctor
    */
   PMDetailObject( const PMDetailObject& o );
   /**
    * Deletes the object
    */
   virtual ~PMDetailObject();

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns true if the object has detail settings
    */
   virtual bool hasDisplayDetail() const { return false; }

   /**
    * Returns the current display detail
    */
   int displayDetail() const;

   /**
    * Sets the global detail flag
    */
   void setGlobalDetail( bool yes );
   /**
    * Returns the global detail flag
    */
   bool globalDetail() const { return m_globalDetail; }

   /**
    * Sets the local detail level
    */
   void setLocalDetailLevel( int level );
   /**
    * Returns the local detail level
    */
   int localDetailLevel() const { return m_localDetailLevel; }

   /**
    * Sets the global detail level
    */
   static void setGlobalDetailLevel( int level );
   /**
    * Returns the global detail level
    */
   static int globalDetailLevel() { return s_globalDetailLevel; }

   /** */
   virtual void restoreMemento( PMMemento* s );

protected:
   /**
    * Global parameter key
    */
   static int globalDetailKey() { return s_globalDetailKey; }

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMNamedObjectMementoID { PMGlobalDetailID, PMLocalDetailLevelID };

   bool m_globalDetail;
   int m_localDetailLevel;

   static int s_globalDetailKey;
   static int s_globalDetailLevel;
   static PMMetaObject* s_pMetaObject;
};

#endif
