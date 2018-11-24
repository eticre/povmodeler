/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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

#include "pmplanenormalcontrolpoint.h"
#include "pmdistancecontrolpoint.h"
#include "pmmath.h"
#include <math.h>

PMPlaneNormalControlPoint::PMPlaneNormalControlPoint( PMDistanceControlPoint* d,
                                const PMVector& normal, int id,
                                const QString& description )
      : PMControlPoint( id, description )
{
   m_normal = normal;
   m_pDistancePoint = d;
}

PMVector PMPlaneNormalControlPoint::position() const
{
   return m_normal * ( m_pDistancePoint->distance() + 1.0 );
}

PMVector PMPlaneNormalControlPoint::extraLineStart() const
{
   return m_normal * m_pDistancePoint->distance();
}

PMVector PMPlaneNormalControlPoint::extraLineEnd() const
{
   return position();
}

void PMPlaneNormalControlPoint::graphicalChangeStarted()
{
   m_originalNormal = m_normal;
   m_originalDistance = m_pDistancePoint->distance();
}

void PMPlaneNormalControlPoint::graphicalChange( const PMVector& startPoint,
                                                 const PMVector& /*viewNormal*/,
                                                 const PMVector& endPoint )
{
   PMVector p = m_originalNormal * ( m_originalDistance + 1 )
                + endPoint - startPoint;
   double pabs = p.abs();
   
   if( !approxZero( pabs ) )
   {
      PMVector np = p / pabs;
      double nd = m_originalNormal.abs() * m_originalDistance;
      PMVector normal = p - np * fabs( nd );
      double nl = normal.abs();
      
      if( !approxZero( nl ) )
      {
         if( ( m_originalDistance * ( pabs - fabs( nd ) ) ) < 0 )
            nd = -nd;
         nd /= nl;
         m_pDistancePoint->setDistance( nd );
         m_pDistancePoint->setDirection( normal );
         m_normal = normal;
      }
   }
}

double PMPlaneNormalControlPoint::distance() const
{
   return m_pDistancePoint->distance();
}
   
void PMPlaneNormalControlPoint::snapToGrid()
{
   int i;
   double d = moveGrid();
   if( !approxZero( d ) )
      for( i = 0; i < 3; i++ )
         m_normal[i] = rint( m_normal[i] / d ) * d;
   setChanged();
}
