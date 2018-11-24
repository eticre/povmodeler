//-*-C++-*-
/*
**************************************************************************
                          pmsphere.h  -  description
                             -------------------
    copyright            : (C) 2001 by Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
    and                  : (C) 2001-2002 by Andreas Zehender
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


#ifndef PMSPHERE_H
#define PMSPHERE_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include "pmviewstructure.h"

/**
 * Class for povray sphere.
 */
class PMSphere : public PMSolidObject
{
   typedef PMSolidObject Base;

public:
   /**
    * Create an empty Sphere
    */
   PMSphere( PMPart* part );
   /**
    * Copy constructor
    */
   PMSphere( const PMSphere& s );
   /**
    * Delete the PMSphere
    */
   virtual ~PMSphere();

   /** */
   virtual PMObject* copy() const { return new PMSphere( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMSphereEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmsphere" ); }

   /**
    * Returns the centre of the sphere
    */
   PMVector centre() const { return m_centre; }
   /**
    * Set the centre of the sphere
    */
   void setCentre( const PMVector& centre );
   /**
    * returns the radius of the sphere
    */
   double radius() const { return m_radius; }
   /**
    * Sets the radius of the sphere
    */
   void setRadius( double radius );

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual bool hasDisplayDetail() const { return true; }

   /**
    * Sets the number of latitutes
    */
   static void setUSteps( int u );
   /**
    * Sets the number of longitudes
    */
   static void setVSteps( int v );
   /**
    * Returns the number or latitutes
    */
   static int uSteps() { return s_uStep; }
   /**
    * Returns the number or longitudes
    */
   static int vSteps() { return s_vStep; }
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
   virtual int viewStructureParameterKey() const { return s_parameterKey + globalDetailKey(); }

private:
   /**
    * Creates the faces for the view structure
    */
   static void createFaces( PMFaceArray& faces, int uStep, int vStep );
   /**
    * Creates the lines for the view structure
    */
   static void createLines( PMLineArray& lines, int uStep, int vStep );
   /**
    * Creates the points for the view structure
    */
   static void createPoints( PMPointArray& points, const PMVector& centre,
                             double radius, int uStep, int vStep );

   enum PMSphereMementoID { PMRadiusID, PMCentreID };
   /**
    * Radius of the sphere
    */
   double m_radius;
   /**
    * centre of the sphere
    */
   PMVector m_centre;

   static PMViewStructure* s_pDefaultViewStructure;
   static int s_vStep;
   static int s_uStep;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};


#endif
