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


#ifndef PMVECTOR_H
#define PMVECTOR_H

#include <qstring.h>

class PMMatrix;

/**
 * Class for vectors with variable size
 */
class PMVector
{
public:
   /**
    * Creates a vector with size s. All values are set to 0
    */
   PMVector( int s );
   /**
    * Creates a vector with size 3. All values are set to 0
    */
   PMVector();
   /**
    * Creates a vector with coordinates [x,y]
    */
   PMVector( const double x, const double y );
   /**
    * Creates a vector with coordinates [x,y,z]
    */
   PMVector( const double x, const double y, const double z );
   /**
    * Creates a vector with coordinates [x,y,z,t]
    */
   PMVector( const double x, const double y, const double z, const double t );
   /**
    * Copy constructor. Creates a deep copy.
    */
   PMVector( const PMVector& v );
   /**
    * Deletes the vector
    */
   ~PMVector();

   /**
    * Returns the size of the vector
    */
   int size() const { return m_size; }
   /**
    * Resizes the vector. New elements are initialized with 0
    */
   void resize( int size );
   /**
    * Returns a reference to a coordinate, 0:x, 1:y, 2:z
    */
   double& operator[] ( int index );
   /**
    * Returns a reference to a coordinate, 0:x, 1:y, 2:z
    */
   const double& operator[] ( int index ) const;

   /**
    * Returns the x coordinate
    */
   double x() const { return (*this)[0]; }
   /**
    * Returns the y coordinate
    */
   double y() const { return (*this)[1]; }
   /**
    * Returns the z coordinate
    */
   double z() const { return (*this)[2]; }
   /**
    * Returns the t coordinate
    */
   double t() const { return (*this)[3]; }

   /**
    * Sets the x coordinate
    */
   void setX( const double d ) { (*this)[0] = d; }
   /**
    * Sets the y coordinate
    */
   void setY( const double d ) { (*this)[1] = d; }
   /**
    * Sets the z coordinate
    */
   void setZ( const double d ) { (*this)[2] = d; }
   /**
    * Sets the x coordinate
    */
   void setT( const double d ) { (*this)[3] = d; }

   /**
    * Transforms the point with the matrix m. Same as p = m * p
    *
    * size must be 3!
    */
   void transform( const PMMatrix& m );

   /**
    * Assigns p to the vector. Resizes the vector if necessary.
    */
   PMVector& operator= ( const PMVector& p );
   /**
    * Sets all values to d
    */
   PMVector& operator= ( const double d );
   
   /**
    * Multiplies all coordinates with d
    */
   PMVector& operator*= ( double d );
   /**
    * Divides all coordinates through d
    */
   PMVector& operator/= ( double d );
   /**
    * Adds d to all coordinates
    */
   PMVector& operator+= ( double d );
   /**
    * Subtracts d from all coordinates
    */
   PMVector& operator-= ( double d );

   /**
    * Multiplies the vectors (<x1*x2, y1*y2, z1*z2> ...)
    */
   PMVector& operator*= ( const PMVector& p );
   /**
    * Divides the vectors (<x1/x2, y1/y2, z1/z2> ...)
    */
   PMVector& operator/= ( const PMVector& p );

   /**
    * Adds p to the point.
    */
   PMVector& operator+= ( const PMVector& p );
   /**
    * Subtracts p from the point.
    */
   PMVector& operator-= ( const PMVector& p );

   /**
    * Returns a point with negated coordinates
    */
   friend PMVector operator- ( const PMVector& p );
   /**
    * Adds the two points
    */
   friend PMVector operator+ ( const PMVector& p1, const PMVector& p2 );
   /**
    * Subtracts p2 from p1
    */
   friend PMVector operator- ( const PMVector& p1, const PMVector& p2 );

   /**
    * Multiplies all coordinates with d
    */
   friend PMVector operator* ( const PMVector& p, const double d );
   /**
    * Divides all coordinates by d
    */
   friend PMVector operator/ ( const PMVector& p, const double d );
   /**
    * Adds d to all coordinates
    */
   friend PMVector operator+ ( const PMVector& p, const double d );
   /**
    * Subtracts d from all coordinates
    */
   friend PMVector operator- ( const PMVector& p, const double d );

   /**
    * Multiplies all coordinates with d
    */
   friend PMVector operator* ( const double d, const PMVector& p );
   /**
    * Multiplies the vectors (<x1*x2, y1*y2, z1*z2> ...)
    */
   friend PMVector operator* ( const PMVector& p1, const PMVector& p2 );
   /**
    * Divides the vectors (<x1/x2, y1/y2, z1/z2> ...)
    */
   friend PMVector operator/ ( const PMVector& p1, const PMVector& p2 );
   /**
    * Transforms the point p with the matrix m
    * @see transform
    */
   friend PMVector operator* ( const PMMatrix& m, const PMVector& p );

   /**
    * Returns true if the vectors have the same size and values
    */
   bool operator== ( const PMVector& p ) const;
   /**
    * Returns false if the vectors have the same size and values
    */
   bool operator!= ( const PMVector& p ) const;
   
   /**
    * Returns true if the vectors have the same size and values
    */
   bool approxEqual( const PMVector& p, double epsilon = 1e-6 ) const;

   /**
    * Returns the cross product of v1 and v2
    *
    * Size of v1 and v2 has to be 3!
    */
   static PMVector cross( const PMVector& v1, const PMVector& v2 );
   /**
    * Returns the dot product of v1 and v2
    */
   static double dot( const PMVector& v1, const PMVector& v2 );
   
   /**
    * Returns the angle between v1 and v2
    *
    * Size of v1 and v2 has to be 3!
    */
   static double angle( const PMVector& v1, const PMVector& v2 );

   /**
    * Returns the length of the vector
    */
   double abs() const;

   /**
    * Returns a normalized vector that is orthogonal to this vector
    */
   PMVector orthogonal() const;
   
   /**
    * Returns a string for serialization
    */
   QString serialize() const;
   /**
    * Returns a string for xml output
    */
   QString serializeXML() const;
   /**
    * loads the vector data from the xml string
    */
   bool loadXML( const QString& str );
private:
   void allocateMemory( int size );
   
   double* m_coord;
   static double s_dummy;
   int m_size;
};

#endif
