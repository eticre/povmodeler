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

#include "pmdistancecontrolpoint.h"
#include "pmmath.h"
#include "pmdebug.h"
#include <math.h>


PMDistanceControlPoint::PMDistanceControlPoint( PMControlPoint* base,
                          const PMVector& direction, double distance,
                          int id, const QString& description,
                          bool showExtraLine )
      : PMControlPoint( id, description )
{
   m_distance = distance;
   m_pBasePoint = base;
   m_direction = direction;
   m_directionLength = direction.abs();
   m_extraLine = showExtraLine;
}

PMDistanceControlPoint::PMDistanceControlPoint( const PMVector& base,
                          const PMVector& direction, double distance,
                          int id, const QString& description,
                          bool showExtraLine )
      : PMControlPoint( id, description )
{
   m_distance = distance;
   m_constBasePoint = base;
   m_pBasePoint = 0;
   m_direction = direction;
   m_directionLength = direction.abs();
   m_extraLine = showExtraLine;
}

PMVector PMDistanceControlPoint::position() const
{
   if( m_pBasePoint )
      return m_pBasePoint->position() + m_direction * m_distance;
   return m_constBasePoint + m_direction * m_distance;
}

PMVector PMDistanceControlPoint::basePoint() const
{
   if( m_pBasePoint )
      return m_pBasePoint->position();
   return m_constBasePoint;
}

void PMDistanceControlPoint::setDirection( const PMVector& direction )
{
   m_direction = direction;
   m_directionLength = direction.abs();
}

void PMDistanceControlPoint::graphicalChangeStarted()
{
   m_originalDistance = m_distance;
}

void PMDistanceControlPoint::graphicalChange( const PMVector& startPoint,
                                        const PMVector& /*viewNormal*/,
                                        const PMVector& endPoint )
{
   if( !approxZero( m_directionLength ) )
      m_distance = m_originalDistance +
                   PMVector::dot( endPoint - startPoint, m_direction )
                   / ( m_directionLength * m_directionLength );
}

void PMDistanceControlPoint::snapToGrid()
{
   double d = moveGrid();
   if( !approxZero( d ) )
      m_distance = rint( m_distance / d ) * d;
   setChanged();
}
