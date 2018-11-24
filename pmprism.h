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


#ifndef PMPRISM_H
#define PMPRISM_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include <QList>
#include <math.h>


/**
 * Class for povray prism objects.
 */

class PMPrism : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * The spline type
    */
   enum SplineType { LinearSpline, QuadraticSpline, CubicSpline, BezierSpline };
   /**
    * The sweep type
    */
   enum SweepType { LinearSweep, ConicSweep };
   /**
    * Creates an empty PMPrism
    */
   PMPrism( PMPart* part );
   /**
    * Copy constructor
    */
   PMPrism( const PMPrism& p );
   /**
    * deletes the PMPrism
    */
   virtual ~PMPrism();

   /** */
   virtual PMObject* copy() const { return new PMPrism( *this ); }
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
    * Returns a new @ref PMPrismEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmprism" ); }

   /** */
   virtual void createMemento();
   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual bool multipleSelectControlPoints() const { return true; }
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
   QList< QList<PMVector> > points() const { return m_points; }
   /**
    * Sets the spline points
    */
   void setPoints( const QList< QList<PMVector> >& points );
   /**
    * Returns the spline type
    */
   SplineType splineType() const { return m_splineType; }
   /**
    * Sets the spline type
    */
   void setSplineType( SplineType t );
   /**
    * Returns the sweep type
    */
   SweepType sweepType() const { return m_sweepType; }
   /**
    * Sets the sweep type
    */
   void setSweepType( SweepType t );
   /**
    * Returns the sturm flag
    */
   bool sturm() const { return m_sturm; }
   /**
    * Sets the sturm flag
    */
   void setSturm( bool s );
   /**
    * Returns the open flag
    */
   bool open() const { return m_open; }
   /**
    * Sets the open flag
    */
   void setOpen( bool o );
   /**
    * Returns the height 1
    */
   double height1() const { return m_height1; }
   /**
    * Returns the height 2
    */
   double height2() const { return m_height2; }
   /**
    * Sets the height 1
    */
   void setHeight1( double h );
   /**
    * Sets the height 2
    */
   void setHeight2( double h );

   /**
    * Sets the number of steps around the y axis
    */
   static void setSSteps( int s );
   /**
    * Returns the number of steps around the y axis
    */
   static int sSteps() { return s_sSteps; }
   /**
    * Returns the points for POV-Ray serialization (contains additional points)
    */
   QList<PMVector> expandedPoints( const QList<PMVector>& p ) const;

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
   enum PMPrismMementoID { PMSplineTypeID, PMSweepTypeID, PMSturmID, PMOpenID,
                           PMHeight1ID, PMHeight2ID };
   /**
    * IDs for the object actions
    */
   enum PMPrismActionID { PMSplitSegmentID, PMJoinSegmentsID };
   SplineType m_splineType;
   SweepType m_sweepType;
   QList< QList<PMVector> > m_points;
   double m_height1, m_height2;
   bool m_sturm;
   bool m_open;

   static int s_sSteps;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
