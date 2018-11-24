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

#include "pmsplinesegment.h"

PMVector PMSplineSegment::point( double t ) const
{
   double t2 = t * t;
   double t3 = t2 * t;

   return PMVector( m_a[0]*t3 + m_b[0]*t2 + m_c[0]*t + m_d[0],
                    m_a[1]*t3 + m_b[1]*t2 + m_c[1]*t + m_d[1] );
}
   
PMVector PMSplineSegment::gradient( double t ) const
{
   double t2 = t * t;

   return PMVector( 3*m_a[0]*t2 + 2*m_b[0]*t + m_c[0],
                    3*m_a[1]*t2 + 2*m_b[1]*t + m_c[1] );
}

void PMSplineSegment::calculateLinear( const PMVector& p1, const PMVector& p2 )
{
   m_a[0] =  0.0;
   m_b[0] =  0.0;
   m_c[0] = -1.0 * p1[0] + 1.0 * p2[0];
   m_d[0] =  1.0 * p1[0];
   
   m_a[1] = 0.0;
   m_b[1] =  0.0;
   m_c[1] = -1.0 * p1[1] + 1.0 * p2[1];
   m_d[1] =  1.0 * p1[1];
}

void PMSplineSegment::calculateQuadratic( const PMVector& p1,
                                          const PMVector& p2,
                                          const PMVector& p3 )
{
   m_a[0] =  0.0;
   m_b[0] =  0.5 * p1[0] - 1.0 * p2[0] + 0.5 * p3[0];
   m_c[0] = -0.5 * p1[0]               + 0.5 * p3[0];
   m_d[0] =                1.0 * p2[0];
   
   m_a[1] =  0.0;
   m_b[1] =  0.5 * p1[1] - 1.0 * p2[1] + 0.5 * p3[1];
   m_c[1] = -0.5 * p1[1]               + 0.5 * p3[1];
   m_d[1] =                1.0 * p2[1];
}

void PMSplineSegment::calculateCubic( const PMVector& p1, const PMVector& p2,
                                      const PMVector& p3, const PMVector& p4 )
{
   m_a[0] = -0.5 * p1[0] + 1.5 * p2[0] - 1.5 * p3[0] + 0.5 * p4[0];
   m_b[0] =        p1[0] - 2.5 * p2[0] + 2.0 * p3[0] - 0.5 * p4[0];
   m_c[0] = -0.5 * p1[0]               + 0.5 * p3[0];
   m_d[0] =                      p2[0];
   
   m_a[1] = -0.5 * p1[1] + 1.5 * p2[1] - 1.5 * p3[1] + 0.5 * p4[1];
   m_b[1] =        p1[1] - 2.5 * p2[1] + 2.0 * p3[1] - 0.5 * p4[1];
   m_c[1] = -0.5 * p1[1]               + 0.5 * p3[1];
   m_d[1] =                      p2[1];
}

void PMSplineSegment::calculateBezier( const PMVector& p1, const PMVector& p2,
                                       const PMVector& p3, const PMVector& p4 )
{
   m_a[0] = -      p1[0] + 3.0 * p2[0] - 3.0 * p3[0] + p4[0];
   m_b[0] =  3.0 * p1[0] - 6.0 * p2[0] + 3.0 * p3[0];
   m_c[0] = -3.0 * p1[0] + 3.0 * p2[0];
   m_d[0] =        p1[0];
   
   m_a[1] = -      p1[1] + 3.0 * p2[1] - 3.0 * p3[1] + p4[1];
   m_b[1] =  3.0 * p1[1] - 6.0 * p2[1] + 3.0 * p3[1];
   m_c[1] = -3.0 * p1[1] + 3.0 * p2[1];
   m_d[1] =        p1[1];
}

void PMSplineSegment::calculateQuadricBezier( const PMVector& p1,
                                              const PMVector& p2,
                                              const PMVector& p3 )
{
   m_a[0] = 0;
   m_b[0] =        p1[0] - 2.0 * p2[0] + p3[0];
   m_c[0] = -2.0 * p1[0] + 2.0 * p2[0];
   m_d[0] =        p1[0];
   
   m_a[1] = 0;
   m_b[1] =        p1[1] - 2.0 * p2[1] + p3[1];
   m_c[1] = -2.0 * p1[1] + 2.0 * p2[1];
   m_d[1] =        p1[1];
}
