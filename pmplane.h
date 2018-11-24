//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMPLANE_H
#define PMPLANE_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include "pmviewstructure.h"

/**
 * Class for povray plane
 */

class PMPlane : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Creates a plane
    */
   PMPlane( PMPart* part );
   /**
    * Copy constructor
    */
   PMPlane( const PMPlane& p );

   /**
    * Deletes the plane
    */
   virtual ~PMPlane();

   /** */
   virtual PMObject* copy() const { return new PMPlane( *this ); }

   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMPlaneEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view and dialog view
    */
   virtual QString pixmap() const { return QString( "pmplane" ); }

   /**
    * Return the normal
    */
   PMVector normal() const { return m_normal; }
   /**
    * Sets normal
    */
   void setNormal( const PMVector& p );
   /**
    * Return the distance
    */
   double distance() const { return m_distance; }
   /**
    * Sets the distance
    */
   void setDistance( double distance );

   /**
    * Sets the plane size (view structure)
    */
   static void setPlaneSize( double size );
   /**
    * Returns the plane size (view structure)
    */
   static double planeSize() { return s_planeSize; }

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual void cleanUp() const;

protected:
   /** */
   virtual bool isDefault();
   /** */
   virtual void createViewStructure();
   /** */
   virtual PMViewStructure* defaultViewStructure() const;
   /** */
   virtual int viewStructureParameterKey() const { return s_parameterKey; }

private:
   /**
    * Creates the points for the view structure
    */
   static void createPoints( PMPointArray& points, const PMVector& normal , double distance );

   /**
    * IDs for @ref PMMementoData
    */
   enum PMPlaneMementoID { PMNormalID, PMDistanceID };
   /**
    * normal of plane
    */
   PMVector m_normal;
   /**
    * distance from origin
    */
   double m_distance;
   /**
    * The default view structure. It can be shared between planes
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static double s_planeSize;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
