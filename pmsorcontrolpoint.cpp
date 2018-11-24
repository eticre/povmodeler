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

#include "pmsorcontrolpoint.h"
#include "pmmath.h"
#include <math.h>

PMSorControlPoint::PMSorControlPoint( PMSorControlPoint* prev,
                                      const PMVector& point,
                                      PMSorControlPoint::CPType type, int id,
                                      const QString& description )
      : PMControlPoint( id, description )
{
   m_point = point;
   m_type = type;
   m_pPrev = prev;
   if( m_pPrev )
      m_pPrev->m_pNext = this;
   m_pNext = 0;
   m_pSorLink = 0;
}

void PMSorControlPoint::graphicalChangeStarted()
{
   if( m_pPrev && !m_pPrev->m_pPrev && !m_pPrev->selected() )
      m_pPrev->graphicalChangeStarted();
   if( m_pNext && !m_pNext->m_pNext && !m_pNext->selected() )
      m_pNext->graphicalChangeStarted();

   m_original2DPoint = m_point;
   m_originalPoint = to3D( m_point );
}

void PMSorControlPoint::graphicalChange( const PMVector& startPoint,
                                         const PMVector& /*viewNormal*/,
                                         const PMVector& endPoint )
{
   if(
      ( !m_pPrev && m_pNext->selected() )
      ||
      ( !m_pNext && m_pPrev->selected() )
     )
      return;
   
   m_point = to2D( m_originalPoint + endPoint - startPoint );
   
   if( m_pSorLink && m_pSorLink->selected() )
   {
      PMSorControlPoint* ll = m_pSorLink;
      PMVector op = ll->to2D( ll->m_originalPoint + endPoint - startPoint );

      if( ( m_point - m_original2DPoint ).abs() <
          ( op - ll->m_original2DPoint ).abs() )
         m_point = op;
   }
   
   if( m_pPrev && m_pNext )
   {
      if( m_pPrev->m_pPrev )
         if( ( m_point[1] - m_pPrev->m_point[1] ) < c_sorTolerance )
            m_point[1] = m_pPrev->m_point[1] + c_sorTolerance;
      if( m_pNext->m_pNext )
         if( ( m_pNext->m_point[1] - m_point[1] ) < c_sorTolerance )
            m_point[1] = m_pNext->m_point[1] - c_sorTolerance;
   }
   if( m_point[0] < 0.0 )
      m_point[0] = 0.0;

   if( m_pPrev && !m_pPrev->m_pPrev )
   {
      m_pPrev->m_point = m_point + m_pPrev->m_original2DPoint
                         - m_original2DPoint;
      m_pPrev->setChanged();
   }
   if( m_pNext && !m_pNext->m_pNext )
   {
      m_pNext->m_point = m_point + m_pNext->m_original2DPoint
                         - m_original2DPoint;
      m_pNext->setChanged();
   }
}

void PMSorControlPoint::snapToGrid()
{
   int i;
   double d = moveGrid();
   bool diff = false;
   PMVector change( 2 );
   PMSorControlPoint* basePoint = 0;
   PMSorControlPoint* linkedPoint = 0;
   if( !m_pPrev )
      basePoint = m_pNext;
   if( !m_pNext )
      basePoint = m_pPrev;
   if( m_pPrev && !m_pPrev->m_pPrev )
      linkedPoint = m_pPrev;
   if( m_pNext && !m_pNext->m_pNext )
      linkedPoint = m_pNext;
   
   if( basePoint && basePoint->selected() )
   {
      m_point -= basePoint->m_point;
      diff = true;
   }

   if( !approxZero( d ) )
   {
      for( i = 0; i < 2; i++ )
      {
         change[i] = -m_point[i];
         m_point[i] = rint( m_point[i] / d ) * d;
         change[i] += m_point[i];
      }
   }

   if( diff )
      m_point += basePoint->m_point;

   if( linkedPoint )
   {
      linkedPoint->m_point += change;
      linkedPoint->setChanged();
   }
   
   setChanged();
}

PMVector PMSorControlPoint::to2D( const PMVector& v ) const
{
   PMVector result( 2 );
   switch( m_type )
   {
      case PM2DXY:
         result[0] = v[0];
         result[1] = v[1];
         break;
      case PM2DXZ:
         result[0] = v[0];
         result[1] = v[2];
         break;
      case PM2DYZ:
         result[0] = v[1];
         result[1] = v[2];
         break;
      case PM2DYX:
         result[0] = v[1];
         result[1] = v[0];
         break;
      case PM2DZX:
         result[0] = v[2];
         result[1] = v[0];
         break;
      case PM2DZY:
         result[0] = v[2];
         result[1] = v[1];
         break;
   }
   return result;
}

PMVector PMSorControlPoint::to3D( const PMVector& vec ) const
{
   PMVector result( 3 );
   switch( m_type )
   {
      case PM2DXY:
         result[0] = vec[0];
         result[1] = vec[1];
         result[2] = 0.0;
         break;
      case PM2DXZ:
         result[0] = vec[0];
         result[1] = 0.0;
         result[2] = vec[1];
         break;
      case PM2DYZ:
         result[0] = 0.0;
         result[1] = vec[0];
         result[2] = vec[1];
         break;
      case PM2DYX:
         result[1] = vec[0];
         result[0] = vec[1];
         result[2] = 0.0;
         break;
      case PM2DZX:
         result[2] = vec[0];
         result[0] = vec[1];
         result[1] = 0.0;
         break;
      case PM2DZY:
         result[2] = vec[0];
         result[1] = vec[1];
         result[0] = 0.0;
         break;
   }
   return result;
}

bool PMSorControlPoint::hasExtraLine() const
{
   return( !m_pPrev || !m_pNext );
}

PMVector PMSorControlPoint::extraLineStart() const
{
   return position();
}

PMVector PMSorControlPoint::extraLineEnd() const
{
   if( !m_pPrev && m_pNext )
      return m_pNext->position();
   if( m_pPrev && !m_pNext )
      return m_pPrev->position();
   return PMVector( 0, 0, 0 );
}
