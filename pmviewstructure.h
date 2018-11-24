//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMVIEWSTRUCTURE_H
#define PMVIEWSTRUCTURE_H

#include "pmface.h"
#include "pmpoint.h"
#include "pmline.h"
#include "pmmatrix.h"
#include "pmobject.h"
#include "pmvector.h"

/**
 * Represents the view structure (points, lines, faces) of an object
 *
 * Faces are not implemented yet but are needed later to calculate the view
 * structure of csg objects
 */

class PMViewStructure
{
   friend class PMObject;
public:
   /**
    * Creates an empty view structure
    */
   PMViewStructure();
   /**
    * Creates a view structure with n points, l lines and f faces.
    */
   PMViewStructure( unsigned int n, unsigned int l, unsigned int f = 0 );
   /**
    * Creates a copy of the view structure. m_points, m_lines and m_faces are shared
    */
   PMViewStructure( const PMViewStructure& vs );
   /**
    * Creates a copy of the view structure. m_points, m_lines anf m_faces are shared
    */
   PMViewStructure( const PMViewStructure* vs );

   /**
    * Returns a reference to the array of points
    */
   PMPointArray& points() { return m_points; }
   /**
    * Returns a reference to the array of lines
    */
   PMLineArray& lines() { return m_lines; }
   /**
    * Returns a refrence to the array of faces
    */
   PMFaceArray& faces() { return m_faces; }
   /**
    * Returns the parameter key
    */
   int parameterKey() const { return m_parameterKey; }
   /**
    * Sets the parameter key
    */
   void setParameterKey( int k ) { m_parameterKey = k; }

   /**
    * Assigns the view structure to this view structure.
    * The points and lines are shared
    */
   PMViewStructure& operator = ( const PMViewStructure& vs );
   /**
    * Returns true if the view structures share the same points and lines
    */
   bool operator == ( const PMViewStructure& vs ) const;
   /**
    * Returns false if the view structures share the same points and lines
    */
   bool operator != ( const PMViewStructure& vs ) const;
protected:
   /**
    * Not transformed points, can be shared between PMObjects
    * of the same type. m_points.data() can be used as vertex array.
    *
    * Optimized for fast rendering.
    */
   PMPointArray m_points;
   /**
    * Lines to display. m_lines.data() can be used by glDrawElements.
    *
    * Optimized for fast rendering.
    */
   PMLineArray m_lines;
   /**
    * Faces to display.
    */
   PMFaceArray m_faces;
   /**
    * View structure parameter key.
    *
    * Each class can have parameters that modifies the number of lines and
    * points of a view structure (detail level).
    *
    * The framework determines if the view structure is up to date by
    * comparing the key with the parameter key of the corresponding class.
    */
   int m_parameterKey;
};

/**
 * Class for bounding boxes of PMObjects
 */
class PMBoundingBox
{
public:
   /**
    * Creates a bounding box with min and max vectors
    */
   PMBoundingBox( const PMVector& min, const PMVector& max );
   /**
    * Creates an invalid bounding box. @ref PMObject::boundingBox() returns
    * an invalid bounding box, if the object has none.
    */
   PMBoundingBox();

   /**
    * Returns the minimum coordinates
    */
   PMVector min() const { return m_min; }
   /**
    * Returns the maximum coordinates
    */
   PMVector max() const { return m_max; }
   /**
    * Returns the minumum x coordinate
    */
   double minX() const { return m_min.x(); }
   /**
    * Returns the minumum y coordinate
    */
   double minY() const { return m_min.y(); }
   /**
    * Returns the minumum z coordinate
    */
   double minZ() const { return m_min.z(); }
   /**
    * Returns the maximum x coordinate
    */
   double maxX() const { return m_max.x(); }
   /**
    * Returns the maximum y coordinate
    */
   double maxY() const { return m_max.y(); }
   /**
    * Returns the maximum z coordinate
    */
   double maxZ() const { return m_max.z(); }
   /**
    * Sets the minimum coordinates
    */
   void setMin( const PMVector& min ) { m_min = min; }
   /**
    * Sets the maximum coordinates
    */
   void setMax( const PMVector& max ) { m_max = max; }
   /**
    * Sets the minimum x coordinate
    */
   void setMinX( const double c ) { m_min.setX( c ); }
   /**
    * Sets the minimum y coordinate
    */
   void setMinY( const double c ) { m_min.setY( c ); }
   /**
    * Sets the minimum z coordinate
    */
   void setMinZ( const double c ) { m_min.setZ( c ); }
   /**
    * Sets the maximum x coordinate
    */
   void setMaxX( const double c ) { m_max.setX( c ); }
   /**
    * Sets the maximum y coordinate
    */
   void setMaxY( const double c ) { m_max.setY( c ); }
   /**
    * Sets the maximum z coordinate
    */
   void setMaxZ( const double c ) { m_max.setZ( c ); }

   /**
    * Returns true, if the bounding box is valid
    */
   bool isValid() const { return m_bValid; }
   /**
    * Sets the valid flag to v
    */
   void setValid( bool v ) { m_bValid = v; }

   /**
    * Merges the two bounding boxes
    */
   void mergeWith( const PMBoundingBox& box );

private:
   bool m_bValid;
   PMVector m_min, m_max;
};

#endif
