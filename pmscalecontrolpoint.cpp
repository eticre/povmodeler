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

#include "pmscalecontrolpoint.h"
#include "pmmath.h"
#include "pmdebug.h"


#include <math.h>

const double precision = 0.001;

PMScaleControlPoint::PMScaleControlPoint( const PMVector& scale, int id )
      : PMControlPoint( id, ( "Scale" ) )
{
   m_scale = scale;
}

void PMScaleControlPoint::graphicalChangeStarted()
{
   m_originalScale = m_scale;
}

void PMScaleControlPoint::graphicalChange( const PMVector& startPoint,
                                           const PMVector& /*viewNormal*/,
                                           const PMVector& endPoint )
{
   int i;

   for( i = 0; i < 3; i++ )
      if( !approxZero( startPoint[i] ) )
         m_scale[i] = rint( m_originalScale[i] * endPoint[i] / startPoint[i]
                            / precision ) * precision;
}

void PMScaleControlPoint::snapToGrid()
{
   int i;
   double d = scaleGrid();
   if( !approxZero( d ) )
      for( i = 0; i < 3; i++ )
         m_scale[i] = rint( m_scale[i] / d ) * d;
   setChanged();
}
