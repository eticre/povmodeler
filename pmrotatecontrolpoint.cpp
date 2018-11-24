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

#include "pmrotatecontrolpoint.h"
#include "pmmath.h"
#include "pmdebug.h"


#include <math.h>

const double precision = 0.1;

PMRotateControlPoint::PMRotateControlPoint( const PMVector& rot, int id )
      : PMControlPoint( id, ( "Rotation" ) )
{
   m_rotation = rot;
}

void PMRotateControlPoint::graphicalChangeStarted()
{
   m_originalRotation = m_rotation;
   m_originalTransformation =
      PMMatrix::rotation( deg2Rad( m_rotation.x() ),
                          deg2Rad( m_rotation.y() ),
                          deg2Rad( m_rotation.z() ) );
}

void PMRotateControlPoint::graphicalChange( const PMVector& startPoint,
                                            const PMVector& viewNormal,
                                            const PMVector& endPoint )
{
   double a, x, y, z;
   
   a = PMVector::angle( startPoint, endPoint );
   if( !approxZero( a ) )
   {
      PMMatrix m;
      
      if( !approx( a, M_PI ) )
      {
         PMVector n = PMVector::cross( startPoint, endPoint );
         m = PMMatrix::rotation( n, a )
            * m_originalTransformation;
      }
      else
         m = PMMatrix::rotation( viewNormal, M_PI )
            * m_originalTransformation;
         
      m.toRotation( &x, &y, &z );
      m_rotation[0] = rint( rad2Deg( x ) / precision ) * precision;
      m_rotation[1] = rint( rad2Deg( y ) / precision ) * precision;
      m_rotation[2] = rint( rad2Deg( z ) / precision ) * precision;
   }
}

void PMRotateControlPoint::snapToGrid()
{
   int i;
   double d = rotateGrid();
   if( !approxZero( d ) )
      for( i = 0; i < 3; i++ )
         m_rotation[i] = rint( m_rotation[i] / d ) * d;
   setChanged();
}
