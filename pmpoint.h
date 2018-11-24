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


#ifndef PMPOINT_H
#define PMPOINT_H

#include <QVector>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class PMVector;
class PMMatrix;

/**
 * Class for 3d points.
 *
 * Used in @ref PMViewStructure.
 *
 * Optimized for rendering with opengl
 */

class PMPoint
{
public:
   /**
    * Creates a point with coordinates [0,0,0]
    */
   PMPoint();
   /**
    * Creates a point with coordinates [x,y,z]
    */
   PMPoint( GLdouble x, GLdouble y, GLdouble z );
   /**
    * Creates a point from a vector. The size of the vector has to be 3
    */
   PMPoint( const PMVector& v );
   /**
    * Copy constructor
    */
   PMPoint( const PMPoint& p );

   /**
    * Returns a reference to a coordinate, 0:x, 1:y, 2:z
    */
   GLdouble& operator[] ( int index ) { return m_coord[index];}
   /**
    * Returns a reference to a coordinate, 0:x, 1:y, 2:z
    */
   const GLdouble& operator[] ( int index ) const { return m_coord[index];}

   /**
    * Returns the x coordinate
    */
   GLdouble x() const { return m_coord[0]; }
   /**
    * Returns the y coordinate
    */
   GLdouble y() const { return m_coord[1]; }
   /**
    * Returns the z coordinate
    */
   GLdouble z() const { return m_coord[2]; }

   /**
    * Sets the x coordinate
    */
   void setX( const GLdouble newx ) { m_coord[0] = newx; }
   /**
    * Sets the y coordinate
    */
   void setY( const GLdouble newy ) { m_coord[1] = newy; }
   /**
    * Sets the z coordinate
    */
   void setZ( const GLdouble newz ) { m_coord[2] = newz; }

   /**
    * Transforms the point p with the matrix m
    * @see transform
    */
   friend PMPoint operator* ( const PMMatrix& m, const PMPoint& p );
   /**
    * Assigns c to the point
    */
   PMPoint& operator= ( const PMPoint& c );
   /**
    * Transforms the point with the matrix m. Same as p = m * p
    *
    * size must be 3!
    */
   void transform( const PMMatrix& m );
   
private:
   /**
    * The coords. THIS MEMBER HAS TO BE THE FIRST AND ONLY ONE
    * (for rendering with OpenGL)!
    */
   GLdouble m_coord[3];
};

/**
 * @ref QMemArray of PMPoints
 */
typedef QVector<PMPoint> PMPointArray;


#endif
