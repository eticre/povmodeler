//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2004 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/
#ifndef PMFACE_H
#define PMFACE_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "pmdebug.h"
#include "pmvector.h"

/**
 * face to display with index of points. Points must be in clockwise order.
 *
 * Face of a @ref PMViewStructure. Only the indices in a @ref PMPointArray
 * are stored.
 *
 * Optimized for OpenGL
 */
class PMFace
{
public:
   /**
    * Default constructor
    */
   PMFace()
   {
      m_points = 0;
      m_size = 0;
   }
   /**
    * Copy Constructor
    */
   PMFace( const PMFace& face );
   /**
    * Destructor
    */
   ~PMFace() { delete m_points; }
   /**
    * Creates a face with three points.
    */
   PMFace( const GLuint pt1, const GLuint pt2, const GLuint pt3, const PMVector& normal = PMVector() );
   /**
    * Creates a face with four points.
    */
   PMFace( const GLuint pt1, const GLuint pt2, const GLuint pt3, const GLuint pt4, const PMVector& normal = PMVector() );

   /**
    * Operator assignment
    */
   PMFace& operator=( const PMFace& face );
   /**
    * Operator equals
    */
   bool operator==( const PMFace& face ) const;
   /**
    * Operator unequals
    */
   bool operator!=( const PMFace& face ) const;

   /**
    * Resizes the number of points in the face.
    * @return True if successful
    */
   bool resize( const unsigned int size );
   /**
    * @return The number of points in the face.
    */
   unsigned int size() const { return m_size; }

   /**
    * Returns a reference to a point in the face
    */
   GLuint& operator[] ( int index );
   /**
    * Returns a const reference to a point in the face
    */
   const GLuint& operator[] ( int index ) const;

   /**
    * Calculates the normal for the face
    */
   void setNormal( const PMVector& normal ) { m_normal = normal; }
   /**
    * Returns the normal for the face
    */
   PMVector normal() const { return m_normal; }

private:
   /**
    * The face points (indices!)
    */
   GLuint* m_points;
   /**
    * The number of points in the face
    */
   unsigned int m_size;
   /**
    * The normal of the face, calculated automatically
    */
   PMVector m_normal;
   static GLuint s_dummy;
};

typedef QList< PMFace* >::Iterator PMFaceListIterator;

/**
 * A list of @ref PMFace objects.
 *
 * This class stores all faces of a @ref PMViewStructure. Only the indices
 * in a @ref PMPointArray are stored.
 */
typedef QVector< PMFace > PMFaceArray;

#endif
