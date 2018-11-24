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


#ifndef PMGRAPHICALOBJECT_H
#define PMGRAPHICALOBJECT_H



#include "pmdetailobject.h"


/**
 * Class for povray graphical objects
 *
 * Objects in povray can be:
 * Finite Solid Primitives, Finite Patch Primitives, Infinite Solid Primitives and Constructive Solid Geometry
 */
class PMGraphicalObject : public PMDetailObject
{
   typedef PMDetailObject Base;
public:
   /**
    * Creates an empty PMGraphicalObject
    */
   PMGraphicalObject( PMPart* part );
   /**
    * Copy constructor
    */
   PMGraphicalObject( const PMGraphicalObject& o );
   /**
    * Deletes the object and all children
    */
   virtual ~PMGraphicalObject();

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns the state of the no_shadow flag.
    */
   bool noShadow() const { return m_noShadow; }
   /**
    * Sets the no_shadow flag
    */
   void setNoShadow( bool yes );
   /**
    * Returns the state of the no_image flag.
    */
   bool noImage() const { return m_noImage; }
   /**
    * Sets the no_image flag
    */
   void setNoImage( bool yes );
   /**
    * Returns the state of the no_reflection flag
    */
   bool noReflection() const { return m_noReflection; }
   /**
    * Sets the no_reflection flag
    */
   void setNoReflection( bool yes );
   /**
    * Returns the state of the double_illuminate flag
    */
   bool doubleIlluminate() const { return m_doubleIlluminate; }
   /**
    * Sets the double_illuminate flag
    */
   void setDoubleIlluminate( bool yes );
   /**
    * Returns the visibility level
    */
   int visibilityLevel() const { return m_visibilityLevel; }
   /**
    * Sets the visibility level
    */
   void setVisibilityLevel( int level );
   /**
    * Returns true if the visibility level is relative to the objects parent
    */
   bool isVisibilityLevelRelative() const { return m_relativeVisibility; }
   /**
    * Sets the visibility level absolute or relative
    */
   void setVisibilityLevelRelative( bool relative );
   /**
    * Returns the export flag
    */
   virtual bool exportPovray() const { return m_export; }
   /**
    * Sets the export flag
    */
   void setExportPovray( bool ex );

   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMGraphicalObjectMementoID { PMNoShadowID, PMNoImageID, PMNoReflectionID,
                                     PMDoubleIlluminateID, PMVisibilityID,
                                     PMRelativeVisibilityID, PMExportID };

   bool m_noShadow;
   bool m_noImage;
   bool m_noReflection;
   bool m_doubleIlluminate;
   int m_visibilityLevel;
   bool m_relativeVisibility;
   bool m_export;
   static PMMetaObject* s_pMetaObject;
};


#endif
