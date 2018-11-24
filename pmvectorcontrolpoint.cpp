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

#include "pmvectorcontrolpoint.h"
#include "pmmath.h"
#include <math.h>

PMVectorControlPoint::PMVectorControlPoint( PMControlPoint* base,
                                            const PMVector& v, int id,
                                            const QString& description,
                                            bool extraLine, bool normalize )
      : PMControlPoint( id, description )
{
   m_vector = v;
   m_pBasePoint = base;
   m_extraLine = extraLine;
   m_normalize = normalize;
}

PMVectorControlPoint::PMVectorControlPoint( const PMVector& base,
                                            const PMVector& v, int id,
                                            const QString& description,
                                            bool extraLine, bool normalize )
      : PMControlPoint( id, description )
{
   m_vector = v;
   m_constBasePoint = base;
   m_pBasePoint = nullptr;
   m_extraLine = extraLine;
   m_normalize = normalize;
}

PMVector PMVectorControlPoint::position() const
{
   return basePoint() + m_vector;
}

PMVector PMVectorControlPoint::basePoint() const
{
   if( m_pBasePoint )
      return m_pBasePoint->position();
   return m_constBasePoint;
}

void PMVectorControlPoint::graphicalChangeStarted()
{
   m_originalVector = m_vector;
}

void PMVectorControlPoint::graphicalChange( const PMVector& startPoint,
                                        const PMVector& /*viewNormal*/,
                                        const PMVector& endPoint )
{
   double d;
   
   m_vector = m_originalVector + endPoint - startPoint;
   if( m_normalize )
   {
      d = m_vector.abs();
      if( approxZero( d ) )
         m_vector = m_originalVector;
      else
         m_vector /= d;
   }
}

void PMVectorControlPoint::snapToGrid()
{
   int i;
   double d = moveGrid();
   if( !approxZero( d ) )
      for( i = 0; i < 3; i++ )
         m_vector[i] = rint( m_vector[i] / d ) * d;
   setChanged();
}
