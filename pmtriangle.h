//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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


#ifndef PMTRIANGLE_H
#define PMTRIANGLE_H



#include "pmgraphicalobject.h"
#include "pmvector.h"

class PMViewStructure;

/**
 * Class for povray triangles.
 */

class PMTriangle : public PMGraphicalObject
{
   typedef PMGraphicalObject Base;
public:
   /**
    * Creates an empty PMTriangle
    */
   PMTriangle( PMPart* part );
   /**
    * Copy constructor
    */
   PMTriangle( const PMTriangle& t );
   /**
    * deletes the PMTriangle
    */
   virtual ~PMTriangle();

   /** */
   virtual PMObject* copy() const { return new PMTriangle( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMTriangleEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmtriangle" ); }

   /**
    * Returns true if the triangle is a smooth triangle
    */
   bool isSmoothTriangle() const { return m_smooth; }
   /**
    * Enables/disables the normal vectors
    */
   void setSmoothTriangle( bool on );

   /**
    * Returns the point with index i
    */
   PMVector point( int i ) const;
   /**
    * Sets the point with index i
    */
   void setPoint( int i, const PMVector& p );
   /**
    * Returns the normal vector with index i
    */
   PMVector normal( int i ) const;
   /**
    * Sets the mormal vector with index i
    */
   void setNormal( int i, const PMVector& n );

   /**
    * Returns the uv vector with index i
    */
   PMVector uvVector( int i ) const;
   /**
    * Sets the uv vector with index i
    */
   void setUVVector( int i, const PMVector& v );
   /**
    * Return true if triangle has uv vectors
    */
   bool isUVEnabled() const { return m_uvEnabled; }
   /**
    * Enables/disable the uv vectors
    */
   void enableUV( bool yes );

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

protected:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMTriangleMementoID { PMPoint0ID, PMPoint1ID, PMPoint2ID,
                              PMNormal0ID, PMNormal1ID, PMNormal2ID,
                              PMSmoothID,
                              PMUVVector0ID, PMUVVector1ID, PMUVVector2ID,
                              PMUVEnabledID };
   PMVector m_point[3];
   PMVector m_normal[3];
   PMVector m_uvVector[3];
   bool m_smooth;
   bool m_uvEnabled;

   /**
    * The default view structure. It can be shared between triangles
    */
   static PMViewStructure* s_pDefaultViewStructure;

   static PMMetaObject* s_pMetaObject;
};

#endif
