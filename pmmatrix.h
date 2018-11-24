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


#ifndef PMMATRIX_H
#define PMMATRIX_H



#include "pmmath.h"
#include "math.h"

#include <QString>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class PMVector;

/**
 * 4x4 matrix for transformations with homogenous coordinates
 *
 * Optimized for OpenGL.
 */
class PMMatrix
{
public:
   /**
    * Creates a zero matrix
    */
   PMMatrix();
   /**
    * Deletes the matrix
    */
   ~PMMatrix();
   /**
    * Assigns m to the matrix
    */
   PMMatrix& operator= ( const PMMatrix& m );
   /**
    * Creates an identity matrix
    */
   static PMMatrix identity();
   /**
    * Creates a matrix for translation.
    */
   static PMMatrix translation( double x, double y, double z );
   /**
    * Creates a matrix for rotations. Rotation first around the x, then y
    * and then around the z axis.
    *
    * x, y and z in rad
    */
   static PMMatrix rotation( double x, double y, double z );
   /**
    * Creates a matrix for rotation around the direction n with angle a.
    *
    * a in rad.
    */
   static PMMatrix rotation( const PMVector& n, double a );
   /**
    * Creates a viewing matrix.
    *
    * The viewpoint is specified by the parameter eye. The parameter
    * lookAt specifies any point along the line of sight. The up vector
    * indicate which direction is up.
    */
   static PMMatrix viewTransformation( const PMVector& eye,
                                       const PMVector& lookAt,
                                       const PMVector& up );
   /**
    * Backwards calculate the rotations from the matrix
    */
   void toRotation( double* x, double* y, double* z );
   
   /**
    * Creates a matrix for scale.
    */
   static PMMatrix scale( double x, double y, double z );
   /**
    * Returns the current gl modelview matrix
    */
   static PMMatrix modelviewMatrix();
   
   /**
    * Returns a pointer to the column (!) at position index.
    * That means matrix[i][j] is the element at column i and row j*/
   GLdouble* operator[] ( int index ) { return &( m_elements[index*4] ); }
   /**
    * Returns a pointer to the column (!) at position index.
    * That means matrix[i][j] is the element at column i and row j*/
   const GLdouble* operator[] ( int index ) const
      { return &(m_elements[index*4]); }
   /**
    * Returns true if an inverse matrix can be calculated
    */
   bool canBuildInverse() const { return ! approxZero( det() ); }
   /**
    * Returns the inverse matrix if possible, otherwise a identity matrix
    */
   PMMatrix inverse() const;
   /**
    * Returns the determinant of the matrix
    */
   double det() const;
   /**
    * Multiplies m to the matrix from the right side
    */
   PMMatrix& operator*= ( const PMMatrix& m );
   /**
    * Multiplies each element with d
    */
   PMMatrix& operator*= ( const double d );
   /**
    * Divides each element by d
    */
   PMMatrix& operator/= ( const double d );

   /**
    * Returns a matrix with negated elements
    */
   friend PMMatrix operator- ( const PMMatrix& m );
   /**
    * Multiplies m2 with m1 from the right side (m1*m2)*/
   friend PMMatrix operator* ( const PMMatrix& m1, const PMMatrix& m2 );
   /**
    * Multiplies d to all elements
    */
   friend PMMatrix operator* ( const PMMatrix& m, const double d );
   /**
    * Multiplies d to all elements
    */
   friend PMMatrix operator* ( const GLdouble d, const PMMatrix& m );
   /**
    * Divides all elements by d
    */
   friend PMMatrix operator/ ( const PMMatrix& m, const double d );

   /**
    * Only for tests
    */
   void testOutput();
   /**
    * Returns a pointer to the data. Can be used in glMultMatrixd
    */
   const GLdouble* data() const { return m_elements; }
   
   /**
    * Returns a string for xml output
    */
   QString serializeXML() const;
   /**
    * loads the vector data from the xml string
    */
   bool loadXML( const QString& str );
private:
   /**
    * Exchanges two rows
    */
   void exchangeRows( int r1, int r2 );
   /**
    * Finds a row with not zero element in column index. Begins to
    * search in row at position index. Used for det() and inverse()
    *
    * Returns -1 if all rows are zero in that column
    */
   int notNullElementRow( const int index ) const;
   
   GLdouble m_elements[16];
};

#endif
