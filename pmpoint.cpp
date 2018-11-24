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


#include "pmpoint.h"
#include "pmvector.h"
#include "pmmatrix.h"
#include "pmmath.h"


PMPoint::PMPoint()
{
   m_coord[0] = 0;
   m_coord[1] = 0;
   m_coord[2] = 0;
}

PMPoint::PMPoint( GLdouble x, GLdouble y, GLdouble z )
{
   m_coord[0] = x;
   m_coord[1] = y;
   m_coord[2] = z;
}

PMPoint::PMPoint( const PMVector& v )
{
   if( v.size() == 3 )
   {
      m_coord[0] = v[0];
      m_coord[1] = v[1];
      m_coord[2] = v[2];
   }
   else
   {
      m_coord[0] = 0;
      m_coord[1] = 0;
      m_coord[2] = 0;
   }
}

PMPoint::PMPoint( const PMPoint& p )
{
   m_coord[0] = p.m_coord[0];
   m_coord[1] = p.m_coord[1];
   m_coord[2] = p.m_coord[2];
}

PMPoint& PMPoint::operator= ( const PMPoint& p )
{
   m_coord[0] = p.m_coord[0];
   m_coord[1] = p.m_coord[1];
   m_coord[2] = p.m_coord[2];

   return *this;
}

void PMPoint::transform( const PMMatrix& m )
{
   (*this) = m * (*this);
}

PMPoint operator* ( const PMMatrix& m, const PMPoint& p )
{
   PMPoint result;
   int c, i;
   // for homogenous coordinates
   double u;

   for( c = 0; c < 3; c++ )
   {
      result.m_coord[c] = 0.0;
      for( i = 0; i < 4; i++ )
         result.m_coord[c] += m[i][c] * ( i<3 ? p[i] : 1.0 );
   }
      
   u = 0.0;
   for( i = 0; i < 4; i++ )
      u += m[i][3] * ( i<3 ? p[i] : 1.0 );
   if( !approxZero( u ) )
      for( i = 0; i < 3; i++ )
         result.m_coord[i] /= u;

   return result;
}
