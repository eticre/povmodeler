//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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


#ifndef PMBICUBICPATCH_H
#define PMBICUBICPATCH_H



#include "pmgraphicalobject.h"
#include "pmvector.h"

class PMViewStructure;

/**
 * Class for povray bicubic patches (bezier patches).
 */

class PMBicubicPatch : public PMGraphicalObject
{
   typedef PMGraphicalObject Base;
public:
   /**
    * Creates an empty PMBicubicPatch
    */
   PMBicubicPatch( PMPart* part );
   /**
    * Copy constructor
    */
   PMBicubicPatch( const PMBicubicPatch& p );
   /**
    * deletes the PMBicubicPatch
    */
   virtual ~PMBicubicPatch();

   /** */
   virtual PMObject* copy() const { return new PMBicubicPatch( *this ); }
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
    * Returns a new @ref PMBicubicPatchEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmbicubicpatch" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual bool multipleSelectControlPoints() const { return true; }

   /**
    * Sets the type. Type must be 0 or 1
    */
   void setPatchType( int type );
   /**
    * Returns the type
    */
   int patchType() const { return m_patchType; }
   /**
    * Sets the number of u steps
    */
   void setUSteps( int steps );
   /**
    * Returns the number of u steps
    */
   int uSteps() const { return m_numUSteps; }
   /**
    * Sets the number of v steps
    */
   void setVSteps( int steps );
   /**
    * Returns the number of v steps
    */
   int vSteps() const { return m_numVSteps; }
   /**
    * Sets the flatness
    */
   void setFlatness( double f );
   /**
    * Returns the flatness
    */
   double flatness() const { return m_flatness; }

   /**
    * Sets the ith patch control point
    */
   void setControlPoint( int i, const PMVector& p );
   /**
    * Returns the ith patch control point
    */
   PMVector controlPoint( int i ) const;

   /**
    * Sets the uv vectors flag
    */
   void enableUV( bool yes );
   /**
    * Returns the uv vectors flag
    */
   bool isUVEnabled() const { return m_uvEnabled; }
   /**
    * Sets the ith uv vector
    */
   void setUVVector( int i, const PMVector& v );
   /**
    * Returns the ith uv vector
    */
   PMVector uvVector( int i ) const;

protected:
   /** */
   virtual bool isDefault() { return false; }
   /** */
   virtual void createViewStructure();
   /** */
   virtual PMViewStructure* defaultViewStructure() const { return nullptr; }

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMBicubicPatchMementoID { PMTypeID, PMFlatnessID, PMUStepsID,
                                  PMVStepsID,
                                  PMCP0ID, PMCP1ID, PMCP2ID, PMCP3ID,
                                  PMCP4ID, PMCP5ID, PMCP6ID, PMCP7ID,
                                  PMCP8ID, PMCP9ID, PMCP10ID, PMCP11ID,
                                  PMCP12ID, PMCP13ID, PMCP14ID, PMCP15ID,
                                  PMUVEnabledID,
                                  PMUV0ID, PMUV1ID, PMUV2ID, PMUV3ID };
   /**
    * The patch type
    */
   int m_patchType;
   /**
    * Number of subdivisions
    */
   int m_numUSteps, m_numVSteps;
   /**
    * The control points
    */
   PMVector m_point[16];
   PMVector m_uvVectors[4];

   /**
    * Flatness of the patch
    */
   double m_flatness;
   int m_vsUSteps, m_vsVSteps;
   bool m_uvEnabled;

   static PMMetaObject* s_pMetaObject;
};

#endif
