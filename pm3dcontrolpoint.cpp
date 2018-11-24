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

#include "pm3dcontrolpoint.h"
#include "pmmath.h"
#include <math.h>

PM3DControlPoint::PM3DControlPoint( const PMVector& point, int id,
                                    const QString& description )
      : PMControlPoint( id, description )
{
   m_point = point;
}

void PM3DControlPoint::graphicalChangeStarted()
{
   m_originalPoint = m_point;
}

void PM3DControlPoint::graphicalChange( const PMVector& startPoint,
                                        const PMVector& /*viewNormal*/,
                                        const PMVector& endPoint )
{
   m_point = m_originalPoint + endPoint - startPoint;
}

void PM3DControlPoint::snapToGrid()
{
   int i;
   double d = moveGrid();
   if( !approxZero( d ) )
      for( i = 0; i < 3; i++ )
         m_point[i] = rint( m_point[i] / d ) * d;
   setChanged();
}
