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


#ifndef PMBLOBCYLINDER_H
#define PMBLOBCYLINDER_H



#include "pmdetailobject.h"
#include "pmvector.h"
#include "pmviewstructure.h"

/**
 * Class for povray blob cylinder
 */

class PMBlobCylinder : public PMDetailObject
{
   typedef PMDetailObject Base;
public:
   /**
    * Creates a cylinder
    */
   PMBlobCylinder( PMPart* part );
   /**
    * Copy constructor
    */
   PMBlobCylinder( const PMBlobCylinder& c );

   /**
    * Deletes the cylinder
    */
   virtual ~PMBlobCylinder();

   /** */
   virtual PMObject* copy() const { return new PMBlobCylinder( *this ); }

   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMBlobCylinderEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view and dialog view
    */
   virtual QString pixmap() const { return QString( "pmblobcylinder" ); }

   /**
    * Return the end_1
    */
   PMVector end1() const { return m_end1; }
   /**
    * Sets end_1
    */
   void setEnd1( const PMVector& p );
   /**
    * Return the end_2
    */
   PMVector end2() const { return m_end2; }
   /**
    * Sets end_2
    */
   void setEnd2( const PMVector& p );
   /**
    * return the radius of the cylinder
    */
   double radius() const { return m_radius; }
   /**
    * Sets the radius of the cylinder
    */
   void setRadius( double radius );
   /**
    * Returns the strength
    */
   double strength() const { return m_strength; }
   /**
    * Sets the strength
    */
   void setStrength( double s );

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
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual bool hasDisplayDetail() const { return true; }
   /** */
   virtual void cleanUp() const;

protected:
   /** */
   virtual bool isDefault();
   /** */
   virtual void createViewStructure();
   /** */
   virtual PMViewStructure *defaultViewStructure() const;
   /** */
   virtual int viewStructureParameterKey() const { return s_parameterKey + globalDetailKey(); }

private:
   /**
    * Creates the lines for the view structure
    */
   static void createLines( PMLineArray& lines, int uStep, int vStep );
   /**
    * Creates the points for the view structure
    */
   static void createPoints( PMPointArray& points, const PMVector& end1,
                             const PMVector& end2, double radius, int uStep, int vStep );

   /**
    * IDs for @ref PMMementoData
    */
   enum PMBlobCylinderMementoID { PMEnd1ID, PMEnd2ID, PMRadiusID, PMStrengthID };
   /**
    * ends of cylinder
    */
   PMVector m_end1, m_end2;
   /**
    * radius of cylinder
    */
   double m_radius;
   double m_strength;
   /**
    * The default view structure. It can be shared between cylinders
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static int s_vStep;
   static int s_uStep;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
