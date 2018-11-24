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


#ifndef PMLATHE_H
#define PMLATHE_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include <QList>
#include <math.h>


/**
 * Class for povray lathe objects.
 */

class PMLathe : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * The spline type
    */
   enum SplineType { LinearSpline, QuadraticSpline, CubicSpline, BezierSpline };
   /**
    * Creates an empty PMLathe
    */
   PMLathe( PMPart* part );
   /**
    * Copy constructor
    */
   PMLathe( const PMLathe& l );
   /**
    * deletes the PMLathe
    */
   virtual ~PMLathe();

   /** */
   virtual PMObject* copy() const { return new PMLathe( *this ); }
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
    * Returns a new @ref PMLatheEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmlathe" ); }

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
   virtual bool multipleSelectControlPoints() const { return true; }
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
    * Returns the sturm flag
    */
   bool sturm() const { return m_sturm; }
   /**
    * Sets the sturm flag
    */
   void setSturm( bool s );

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
   enum PMLatheMementoID { PMSplineTypeID, PMSturmID };
   /**
    * IDs for the object actions
    */
   enum PMLatheActionID { PMSplitSegmentID, PMJoinSegmentsID };
   SplineType m_splineType;
   QList<PMVector> m_points;
   bool m_sturm;

   static int s_rSteps;
   static int s_sSteps;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
