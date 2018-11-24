//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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


#ifndef PMSPHERESWEEP_H
#define PMSPHERESWEEP_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include <math.h>

/**
 * Class for povray sphere sweep objects.
 */

class PMSphereSweep : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * The spline type
    */
   enum SplineType { LinearSpline, BSpline, CubicSpline };
   /**
    * Creates an empty PMSphereSweep
    */
   PMSphereSweep( PMPart* part );
   /**
    * Copy constructor
    */
   PMSphereSweep( const PMSphereSweep& l );
   /**
    * deletes the PMSphereSweep
    */
   virtual ~PMSphereSweep();

   /** */
   virtual PMObject* copy() const { return new PMSphereSweep( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMSphereSweepEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmspheresweep" ); }

   /** */
   virtual void createMemento();
   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual bool hasDisplayDetail() const { return true; }
   /** */
   virtual void addObjectActions( const PMControlPointList&,
                                  QList<PMObjectAction*>& );
   /** */
   virtual void objectActionCalled( const PMObjectAction*,
                                    const PMControlPointList&,
                                    const QList<PMVector*>&,
                                    const PMVector& );

   /**
    * Returns the spline points
    */
   QList<PMVector> points() const { return m_points; }
   /**
    * Sets the spline points
    */
   void setPoints( const QList<PMVector>& points );
   /**
    * Returns the radii
    */
   QList<double> radii() const { return m_radii; }
   /**
    * Sets the radii
    */
   void setRadii( const QList<double>& radii );
   /**
    * Returns the number of spline points
    */
   int numberOfPoints() const { return m_points.size(); }
   /**
    * Returns the spline type
    */
   SplineType splineType() const { return m_splineType; }
   /**
    * Sets the spline type
    */
   void setSplineType( SplineType t );
   /**
    * Returns the depth tolerance
    */
   double tolerance() const { return m_tolerance; }
   /**
    * Sets the depth tolerance
    */
   void setTolerance( double t );

   /**
    * Sets the number of steps around the y axis
    */
   static void setRSteps( int r );
   /**
    * Sets the number of subdivisions of one spline segment
    */
   static void setSSteps( int v );
   /**
    * Returns the number of steps around the y axis
    */
   static int rSteps() { return s_rSteps; }
   /**
    * Returns the number of subdivisions of one spline segment
    */
   static int sSteps() { return s_sSteps; }

protected:
   /** */
   virtual void createViewStructure();
   /** */
   virtual int viewStructureParameterKey() const { return s_parameterKey + globalDetailKey(); }

private:
   /**
    * Object action. Adds a spline point
    */
   void splitSegment( const PMControlPointList& cp,
							 const QList<PMVector*>& cpViewPosition,
                      const PMVector& clickPosition );
   /**
    * Object action. Removes a spline point
    */
   void joinSegments( const PMControlPointList& cp,
							 const QList<PMVector*>& cpViewPosition,
                      const PMVector& clickPosition );

   void stringToValues( const QString& str );
   QString valuesToString() const;

   /**
    * IDs for @ref PMMementoData
    */
   enum PMSphereSweepMementoID { PMSplineTypeID, PMToleranceID };
   /**
    * IDs for the object actions
    */
   enum PMSphereSweepActionID { PMSplitSegmentID, PMJoinSegmentsID };
   SplineType m_splineType;
   QList<PMVector> m_points;
   QList<double> m_radii;
   double m_tolerance;

   static int s_rSteps;
   static int s_sSteps;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;

   /**
    * Segment Structure for holding info for
    * creating view structure
    */
   struct Segment{
      QList<PMVector> points;
      QList<double> radii;
      QList<PMVector> direction;
   };

   QList<Segment> m_segments;
   int m_nextPoint;
   int m_nextLine;
   /**
    * Sets up the segments for linear spline
    * @param sSteps the number of subdivisions per segment
    */
   void setLinear( int sSteps );
   /**
    * Sets up segments for curved splines
    * @param cubic true for cubic_spline false for b_spline
    * @param sSteps the number of subdivisions per segment
    */
   void setCurved( bool cubic, int sSteps );
   /**
    * Returns a point on a Catmull rom spline
    * @param v Pointer to four control points
    * @param t position along spline ( 0.0 - 1.0 )
    */
   PMVector catmullRom( PMVector *v, double t );
   /**
    * Returns a point on a cubic b spline
    * @param v pointer to four control points
    * @param t position along spline ( 0.0 - 1.0 )
    */
   PMVector bSpline( PMVector *v, double t );
   /**
    * Creates a sphere
    * @param v poisition of sphere
    * @param r radius of sphere
    * @param rSteps the number of steps around the y axis
    */
   void createSphere( PMVector v, double r, int rSteps );

};

#endif
