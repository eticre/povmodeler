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


#include <math.h>

#include "pmmatrix.h"
#include "pmvector.h"
#include "pmdebug.h"

#include <qtextstream.h>

PMMatrix::PMMatrix()
{
   int i;

   for( i = 0; i < 16; i++ )
      m_elements[i] = 0;
}

PMMatrix::~PMMatrix()
{
}

PMMatrix& PMMatrix::operator= ( const PMMatrix& m )
{
   int i;
   for( i=0; i<16; i++ )
      m_elements[i] = m.m_elements[i];

   return *this;
}

PMMatrix PMMatrix::identity()
{
   PMMatrix newMatrix;
   int i;

   for( i=0; i<4; i++ )
      newMatrix[i][i] = 1.0;
   
   return newMatrix;
}

PMMatrix PMMatrix::translation( double x, double y, double z )
{
   PMMatrix newMatrix;
   newMatrix[3][0] = x;
   newMatrix[3][1] = y;
   newMatrix[3][2] = z;
   newMatrix[0][0] = 1;
   newMatrix[1][1] = 1;
   newMatrix[2][2] = 1;
   newMatrix[3][3] = 1;

   return newMatrix;
}

PMMatrix PMMatrix::scale( double x, double y, double z )
{
   PMMatrix newMatrix;
   newMatrix[0][0] = x;
   newMatrix[1][1] = y;
   newMatrix[2][2] = z;
   newMatrix[3][3] = 1;

   return newMatrix;
}

PMMatrix PMMatrix::rotation( double x, double y, double z )
{
   PMMatrix newMatrix;
   double sinx, siny, sinz, cosx, cosy, cosz;
   sinx = sin( x );
   siny = sin( y );
   sinz = sin( z );
   cosx = cos( x );
   cosy = cos( y );
   cosz = cos( z );
   
   newMatrix[0][0] = cosz*cosy;
   newMatrix[1][0] = -sinz*cosx + cosz*siny*sinx;
   newMatrix[2][0] = sinz*sinx + cosz*siny*cosx;
   newMatrix[0][1] = sinz*cosy;
   newMatrix[1][1] = cosz*cosx + sinz*siny*sinx;
   newMatrix[2][1] = -cosz*sinx + sinz*siny*cosx;
   newMatrix[0][2] = -siny;
   newMatrix[1][2] = cosy*sinx;
   newMatrix[2][2] = cosy*cosx;
   newMatrix[3][3] = 1;

   return newMatrix;
}

PMMatrix PMMatrix::rotation( const PMVector& n, double a )
{
   PMMatrix result( PMMatrix::identity() );
   double rx, ry;

   if( n.size() == 3 )
   {
      rx = pmatan( n.y(), n.z() );
      ry = - pmatan( n.x(), sqrt( n.y() * n.y() + n.z() * n.z() ) );

      result = rotation( -rx, 0.0, 0.0 ) * rotation( 0.0, -ry, 0.0 )
         * rotation( rx, ry, a );
         
   }
   else
      qCritical(  ) << "Wrong size in PMMatrix::rotation()\n";
   
   return result;
}

PMMatrix PMMatrix::viewTransformation( const PMVector& eye,
                                       const PMVector& lookAt,
                                       const PMVector& up )
{
   PMMatrix result;
   PMVector x, y, z;
   GLdouble len;
   int i;

   // create rotation matrix
   z = eye - lookAt;
   len = z.abs();
   if( !approxZero( len ) )
      z /= len;
   
   y = up;
   x = PMVector::cross( y, z );
   y = PMVector::cross( z, x );

   // normalize vectors
   len = x.abs();
   if( !approxZero( len ) )
      x /= len;

   len = y.abs();
   if( !approxZero( len ) )
      y /= len;

   for( i = 0; i < 3; i++ )
   {
      result[i][0] = x[i];
      result[i][1] = y[i];
      result[i][2] = z[i];
      result[3][i] = 0.0;
      result[i][3] = 0.0;
   }
   result[3][3] = 1.0;

   // Translate eye to origin
   return result * translation( -eye[0], -eye[1], -eye[2] ); 
}

void PMMatrix::toRotation( double* x, double* y, double* z )
{
   PMMatrix& m = *this;
   
   if( !approx( fabs( m[0][2] ), 1.0 ) )
   {
      double cosy;
      // | m[0][2] | != 1
      // sin(y) != 1.0, cos(y) != 0.0
      *y = asin( - m[0][2] );
      cosy = cos( *y );

      // sign of cosy is important!
      *x = pmatan( m[1][2] / cosy, m[2][2] / cosy );
      *z = pmatan( m[0][1] / cosy, m[0][0] / cosy );
   }
   else if( m[0][2] < 0 )
   {
      // m[0][2] == -1
      // sin(y) == 1, cos(y) == 0
      // z and x are dependent of each other, assume z = 0
      
      double zminusx = pmatan( m[2][1], m[1][1] );

      *y = M_PI_2;
      *z = 0.0;
      *x = - zminusx;
   }
   else
   {
      // m[0][2] == 1
      // sin(y) == -1, cos(y) == 0
      // z and x are dependent of each other, assume z = 0
      
      double zplusx = pmatan( -m[2][1], m[1][1] );

      *y = -M_PI_2;
      *z = 0.0;
      *x = zplusx;
   }
}

PMMatrix PMMatrix::modelviewMatrix()
{
   PMMatrix result;
   glGetDoublev( GL_MODELVIEW_MATRIX, result.m_elements );
   return result;
}

double PMMatrix::det() const
{
   PMMatrix tmp( *this );
   double result = 1.0, help;
   int i, k, e, row;

   // make a upper triangular matrix
   for( i=0; i<4; i++ )
   {
      row = tmp.notNullElementRow( i );
      if( row == -1 )
         return 0;
      if( row != i )
      {
         tmp.exchangeRows( i, row );
         result = -result;
      }

      result *= tmp[i][i];
      for( k=i+1; k<4; k++ )
      {
         help = tmp[i][k];
         for( e=0; e<4; e++ )
            tmp[e][k] -= tmp[e][i] * help/tmp[i][i];
      }
   }
   return result;
}

PMMatrix PMMatrix::inverse() const
{
   PMMatrix result( identity() );
   PMMatrix tmp( *this );
   int i, k, e, row;
   double a;
   
   // uses the Gauss algorithm
   // row operations to make tmp a identity matrix
   // result matrix is then the inverse
   for( i=0; i<4; i++ )
   {
      row = tmp.notNullElementRow( i );
      if( row == -1 )
         return identity();
      if( row != i )
      {
         tmp.exchangeRows( i, row );
         result.exchangeRows( i, row );
      }
      // tmp[i][i] != 0

      a = tmp[i][i];
      for( e=0; e<4; e++ )
      {
         result[e][i] /= a;
         tmp[e][i] /= a;
      }
      // tmp[i][i] == 1

      for( k=0; k<4; k++ )
      {
         if( k != i )
         {
            a = tmp[i][k];
            for( e=0; e<4; e++ )
            {
               result[e][k] -= result[e][i] * a;
               tmp[e][k] -= tmp[e][i] * a;
            }
         }
      }
      // tmp[!=i][i] == 0.0
   }
   return result;
}

void PMMatrix::exchangeRows( int r1, int r2 )
{
   GLdouble help;
   int i;

   for( i=0; i<4; i++ )
   {
      help = (*this)[i][r1];
      (*this)[i][r1] = (*this)[i][r2];
      (*this)[i][r2] = help;
   }
}

int PMMatrix::notNullElementRow( const int index ) const
{
   int i, result = -1;
   GLdouble max = 0.0, v;
   
   // choose the row with abs() = max
   for( i=index; i<4; i++ )
   {
      v = fabs((*this)[index][i]);
      if( v > max )
      {
         result = i;
         max = v;
      }
   }
   return result;
}

PMMatrix& PMMatrix::operator*= ( const double d )
{
   int i;
   for( i=0; i<16; i++ )
      m_elements[i] *= d;
   return *this;
}

PMMatrix& PMMatrix::operator/= ( const double d )
{
   int i;
   if( approxZero( 0 ) )
      qCritical(  ) << "Division by zero in PMMatrix::operator/=" << "\n";
   else
      for( i=0; i<16; i++ )
         m_elements[i] /= d;
   return *this;
}

PMMatrix& PMMatrix::operator*= ( const PMMatrix& m )
{
   *this = *this * m;
   return *this;
}

PMMatrix operator- ( const PMMatrix& m )
{
   PMMatrix result;
   int r,c;
   
   for( r=0; r<4; r++ )
      for( c=0; c<4; c++ )
         result[c][r] = -m[c][r];
   return result;
}

PMMatrix operator* ( const PMMatrix& m1, const PMMatrix& m2 )
{
   PMMatrix result;
   int r, c, i;

   for( r=0; r<4; r++ )
      for( c=0; c<4; c++ )
         for( i=0; i<4; i++ )
            result[c][r] += m1[i][r] * m2[c][i];
   return result;
}

PMMatrix operator* ( const PMMatrix& m1, const double d )
{
   PMMatrix result( m1 );
   result *= d;
   return result;
}

PMMatrix operator/ ( const PMMatrix& m1, const double d )
{
   PMMatrix result( m1 );
   result /= d ;
   return result;
}

PMMatrix operator* ( const double d, const PMMatrix& m1 )
{
   PMMatrix result( m1 );
   result *= d;
   return result;
}

#include <stdio.h>
void PMMatrix::testOutput()
{
   int r, c;

   printf( "\n" );
   for( r=0; r<4; r++ )
   {
      for( c=0; c<4; c++ )
         printf( "% 20.18f ", (*this)[c][r] );
      printf( "\n" );
   }     
}

QString PMMatrix::serializeXML() const
{
   QString result;
   QTextStream str( &result, QIODevice::WriteOnly );
   int i;

   for( i = 0; i < 16; i++ )
   {
      if( i > 0 )
         str << ' ';
      str << m_elements[i];
   }
   
   return result;
}

bool PMMatrix::loadXML( const QString& str )
{
   int i;
   QString tmp( str );
   QTextStream s( &tmp, QIODevice::ReadOnly );
   QString val;
   bool ok;
   
   for( i = 0; i < 16; i++ )
   {
      s >> val;
      m_elements[i] = val.toDouble( &ok );
      if( !ok )
         return false;
   }
   return true;
}
