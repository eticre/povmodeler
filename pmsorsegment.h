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


#ifndef PMSORSEGMENT_H
#define PMSORSEGMENT_H



#include "pmvector.h"

/**
 * Helper class for sors
 *
 * Each instance of this class represents one sor segment. A point
 * on the segment is given by the equation
 *
 * fi(t) = A[i] * t^3 + B[i] * t^2 + C[i] * t + D[i]
 *
 * with t ranging from 0 to 1.
 */
class PMSorSegment
{
public:
   /**
    * Standard constructor
    */
   PMSorSegment()
   {
      m_a = m_b = m_c = m_d = 0.0;
   }
   /**
    * Constructor that calculates the segment
    */
   PMSorSegment( const PMVector& p0, const PMVector& p1,
                 const PMVector& p2, const PMVector& p3 )
   {
      calculateSor( p0, p1, p2, p3 );
   }
   
   /**
    * Returns a 2D vector with the point on the sor segment
    */
   PMVector point( double t ) const;
   /**
    * Returns the gradient on the sor
    */
   //PMVector gradient( double t ) const;
   
   /**
    * Calculates the sor parameters
    */
   void calculateSor( const PMVector& p0, const PMVector& p1,
                      const PMVector& p2, const PMVector& p3 );
private:
   double m_a, m_b, m_c, m_d;
   double m_t, m_s;
};

#endif
