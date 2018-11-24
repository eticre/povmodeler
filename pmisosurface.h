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


#ifndef PMISOSURFACE_H
#define PMISOSURFACE_H



#include "pmsolidobject.h"
#include "pmvector.h"

class PMViewStructure;

/**
 * Class for povray boxes.
 */

class PMIsoSurface : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Enum for the "contained_by" statement
    */
   enum ContainedByType { Box, Sphere };
   /**
    * Creates an empty PMIsoSurface
    */
   PMIsoSurface( PMPart* part );
   /**
    * Copy constructor
    */
   PMIsoSurface( const PMIsoSurface& b );
   /**
    * deletes the PMIsoSurface
    */
   virtual ~PMIsoSurface();

   /** */
   virtual PMObject* copy() const { return new PMIsoSurface( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMIsoSurfaceEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmisosurface" ); }


   /**
    * Sets the isosurface function
    */
   void setFunction( const QString& f );
   /**
    * Returns the isosurface function
    */
   QString function() const { return m_function; }
   /**
    * Sets the type of the contained_by object
    */
   void setContainedBy( ContainedByType type );
   /**
    * Returns the type of the contained_by object
    */
   ContainedByType containedBy() const { return m_containedBy; }
   /**
    * Sets the first corner of a contained_by box
    */
   void setCorner1( const PMVector& v );
   /**
    * Returns the first corner of a contained_by box
    */
   PMVector corner1() const { return m_corner1; }
   /**
    * Sets the second corner of a contained_by box
    */
   void setCorner2( const PMVector& v );
   /**
    * Returns the second corner of a contained_by box
    */
   PMVector corner2() const { return m_corner2; }
   /**
    * Sets the center of a contained_by sphere
    */
   void setCenter( const PMVector& v );
   /**
    * Returns the center of a contained_by sphere
    */
   PMVector center() const { return m_center; }
   /**
    * Sets the radius of a contained_by sphere
    */
   void setRadius( double r );
   /**
    * Returns the radius of a contained_by sphere
    */
   double radius() const { return m_radius; }
   /**
    * Sets the threshold
    */
   void setThreshold( double d );
   /**
    * Returns the Threshold
    */
   double threshold() const { return m_threshold; }
   /**
    * Sets the accuracy
    */
   void setAccuracy( double d );
   /**
    * Returns the accuracy
    */
   double accuracy() const { return m_accuracy; }
   /**
    * Sets the maximal gradient
    */
   void setMaxGradient( double d );
   /**
    * Returns the maximal Gradient
    */
   double maxGradient() const { return m_maxGradient; }
   /**
    * Enables/disables the evaluate statement
    */
   void setEvaluate( bool yes );
   /**
    * Returns the evaluate flag
    */
   bool evaluate() const { return m_bEvaluate; }
   /**
    * Sets the i-th evaluate value, index is in [0..2]
    */
   void setEvaluateValue( int index, double v );
   /**
    * Returns the i-th evaluate value
    */
   double evaluateValue( int index ) const { return m_evaluate[index]; }
   /**
    * Sets the open flag
    */
   void setOpen( bool yes );
   /**
    * Returns the open flag
    */
   bool open() const { return m_bOpen; }
   /**
    * Sets the maximal number of intersections
    */
   void setMaxTrace( int i );
   /**
    * Returns the maximal number of intersections
    */
   int maxTrace() const { return m_maxTrace; }
   /**
    * Sets the all intersections flag
    */
   void setAllIntersections( bool yes );
   /**
    * Returns the all intersections flag
    */
   bool allIntersections() const { return m_bAllIntersections; }


   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void cleanUp() const;

protected:
   /** */
   //virtual bool isDefault();
   /** */
   //virtual void createViewStructure();
   /** */
   //virtual PMViewStructure* defaultViewStructure() const;

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMIsoSurfaceMementoID { FunctionID, ContainedByID, Corner1ID, Corner2ID,
                                CenterID, RadiusID, ThresholdID, AccuracyID,
                                MaxGradientID, EvaluateID,
                                Evaluate0ID, Evaluate1ID, Evaluate2ID,
                                OpenID, MaxTraceID, AllIntersectionsID };

   QString m_function;
   ContainedByType m_containedBy;
   PMVector m_corner1, m_corner2, m_center;
   double m_radius, m_threshold, m_accuracy, m_maxGradient;
   bool m_bEvaluate;
   double m_evaluate[3];
   bool m_bOpen;
   int m_maxTrace;
   bool m_bAllIntersections;

   /**
    * The default view structure. It can be shared between boxes
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static PMMetaObject* s_pMetaObject;
};

#endif
