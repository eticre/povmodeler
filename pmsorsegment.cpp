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

#include "pmsorsegment.h"
#include "pmmath.h"
#include "pmmatrix.h"
#include "pmdebug.h"
#include <math.h>

PMVector PMSorSegment::point( double t ) const
{
   t = m_t + t * m_s;
   double r2 = t * ( t * ( t * m_a + m_b ) + m_c ) + m_d;
   if( r2 < 0.0 )
      r2 = 0.0;

   return PMVector( sqrt( r2 ), t );
}
   
void PMSorSegment::calculateSor( const PMVector& p0, const PMVector& p1,
                                 const PMVector& p2, const PMVector& p3 )
{
   double b[4], h;
   PMMatrix m;
   
   m_t = p1[1];
   m_s = p2[1] - p1[1];
   
   if( approxZero( p2[1] - p0[1] ) || approxZero( p3[1] - p1[1] ) )
   {
      qCritical(  ) << "Incorrect points in PMSorSegment::calculateSor\n";
      m_a = m_b = m_c = m_d = 0.0;
      return;
   }

   // interpolate the points
   // see povray documentation

   b[0] = p1[0] * p1[0];
   b[1] = p2[0] * p2[0];
   b[2] = 2.0 * p1[0] * ( p2[0] - p0[0] ) / ( p2[1] - p0[1] );
   b[3] = 2.0 * p2[0] * ( p3[0] - p1[0] ) / ( p3[1] - p1[1] );

   h = p1[1];
   
   m[0][0] = h * h * h;
   m[0][1] = h * h;
   m[0][2] = h;
   m[0][3] = 1.0;

   m[2][0] = 3.0 * h * h;
   m[2][1] = 2.0 * h;
   m[2][2] = 1.0;
   m[2][3] = 0.0;

   h = p2[1];

   m[1][0] = h * h * h;
   m[1][1] = h * h;
   m[1][2] = h;
   m[1][3] = 1.0;

   m[3][0] = 3.0 * h * h;
   m[3][1] = 2.0 * h;
   m[3][2] = 1.0;
   m[3][3] = 0.0;

   // Calculate the coefficients
   // x = M^-1 * b;
   
   m = m.inverse();
   
   m_a = b[0] * m[0][0] + b[1] * m[0][1] + b[2] * m[0][2] + b[3] * m[0][3];
   m_b = b[0] * m[1][0] + b[1] * m[1][1] + b[2] * m[1][2] + b[3] * m[1][3];
   m_c = b[0] * m[2][0] + b[1] * m[2][1] + b[2] * m[2][2] + b[3] * m[2][3];
   m_d = b[0] * m[3][0] + b[1] * m[3][1] + b[2] * m[3][2] + b[3] * m[3][3];

   if( approxZero( m_a ) ) m_a = 0.0;
   if( approxZero( m_b ) ) m_b = 0.0;
   if( approxZero( m_c ) ) m_c = 0.0;
   if( approxZero( m_d ) ) m_d = 0.0;
}
