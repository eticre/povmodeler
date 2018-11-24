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


#ifndef PMSPLINESEGMENT_H
#define PMSPLINESEGMENT_H



#include "pmvector.h"
#include <QList>

/**
 * Helper class for splines
 *
 * Each instance of this class represents one spline segment. A point
 * on the segment is given by the equation
 *
 * fi(t) = A[i] * t^3 + B[i] * t^2 + C[i] * t + D[i]
 *
 * with t ranging from 0 to 1.
 */
class PMSplineSegment
{
public:
   /**
    * Standard constructor
    */
   PMSplineSegment()
   {
      m_a[0] = m_b[0] = m_c[0] = m_d[0] = 0.0;
      m_a[1] = m_b[1] = m_c[1] = m_d[1] = 0.0;
   }
   /**
    * Copy constructor
    */
   PMSplineSegment( const PMSplineSegment& s )
   {
      int i;
      for( i = 0; i < 2; i++ )
      {
         m_a[i] = s.m_a[i];
         m_b[i] = s.m_b[i];
         m_c[i] = s.m_c[i];
         m_d[i] = s.m_d[i];
      }
   }
   /**
    * Assigns s to the segment
    */
   PMSplineSegment& operator= ( const PMSplineSegment& s )
   {
      int i;
      for( i = 0; i < 2; i++ )
      {
         m_a[i] = s.m_a[i];
         m_b[i] = s.m_b[i];
         m_c[i] = s.m_c[i];
         m_d[i] = s.m_d[i];
      }
      return *this;
   }
   
   /**
    * Returns a 2D vector with the point on the spline segment
    */
   PMVector point( double t ) const;
   /**
    * Returns the gradient on the spline
    */
   PMVector gradient( double t ) const;
   
   /**
    * Calculates the spline parameters for the linear spline type
    */
   void calculateLinear( const PMVector& p1, const PMVector& p2 );
   /**
    * Calculates the spline parameters for the quadratic spline type
    */
   void calculateQuadratic( const PMVector& p1, const PMVector& p2,
                            const PMVector& p3 );
   /**
    * Calculates the spline parameters for the cubic spline type
    */
   void calculateCubic( const PMVector& p1, const PMVector& p2,
                        const PMVector& p3, const PMVector& p4 );
   /**
    * Calculates the spline parameters for the bezier spline type
    */
   void calculateBezier( const PMVector& p1, const PMVector& p2,
                         const PMVector& p3, const PMVector& p4 );
   /**
    * Calculates the spline parameters for the quadric bezier
    */
   void calculateQuadricBezier( const PMVector& p1, const PMVector& p2,
                                const PMVector& p3 );
   
private:
   double m_a[2], m_b[2], m_c[2], m_d[2];
};

typedef QList<PMSplineSegment> PMSegmentList;
typedef QList<PMSegmentList> PMSegmentListList;

#endif
