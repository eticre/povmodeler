/*
 ***************************************************************************
                          pmtorus.h  -  description
                             -------------------
    begin                : Fri Jun 22 2001
    copyright            : (C) 2001 Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
    copyright            : (C) 2002 Andreas Zehender
    email                : zehender@kde.org
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PMTORUS_H
#define PMTORUS_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include "pmviewstructure.h"

class PMViewStructure;

/**
 * Class for povray torus.
 */
class PMTorus : public PMSolidObject
{
   typedef PMSolidObject Base;

public:
   /**
    * Create an empty Sphere
    */
   PMTorus( PMPart* part );
   /**
    * Copy constructor
    */
   PMTorus( const PMTorus& t );
   /**
    * Delete the PMTorus
    */
   virtual ~PMTorus();

   /** */
   virtual PMObject* copy() const { return new PMTorus( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMTrousEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmtorus" ); }
   /**
    * set minor radius see povray documentation about torus
    */
   void setMinorRadius( double minor_radius );
   /**
    * set major radius see povray documentation about torus
    */
   void setMajorRadius( double major_radius );
   /**
    * use sturm algorithme
    */
   void setSturm( bool sturm );
   /**
    * return minor radius see povray documentation about torus
    */
   double minorRadius() const { return m_minorRadius; }
   /**
    * return major radius see povray documentation about torus
    */
   double majorRadius() const { return m_majorRadius; }
   /**
    * return if we must use sturm algorithm for the torus
    */
   bool sturm() const { return m_sturm; }
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

   /**
    * Sets the number of circles
    */
   static void setUSteps( int u );
   /**
    * Sets the number of point for each circle
    */
   static void setVSteps( int v );
   /**
    * Returns the number circles
    */
   static int uSteps() { return s_uStep; }
   /**
    * Returns the number of point for each circle
    */
   static int vSteps() { return s_vStep; }

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
   static void createLines( PMLineArray& lines, int uStep, int vStep );
   static void createPoints( PMPointArray& points, double minor_radius,
                             double  major_radius, int uStep, int vStep );
   enum PMTorusMementoID { PMMinorRadiusID, PMMajorRadiusID, PMSturmID };
   /**
    * Minor radius
    */
   double m_minorRadius;
   /**
    * Major radius
    */
   double m_majorRadius;
   /**
    * use sturm algorithm
    */
   bool m_sturm;
   /**
    * default view structure
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static int s_vStep;
   static int s_uStep;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
/*
x = (major + minor cos(u)) cos(v)
y = (major + minor cos(u)) sin(v)
z = minor sin (u) */


