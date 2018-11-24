/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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

#include "pm2dcontrolpoint.h"
#include "pmmath.h"
#include <math.h>

PM2DControlPoint::PM2DControlPoint( const PMVector& point,
                                    PM2DControlPoint::CPType type, int id,
                                    const QString& description )
      : PMControlPoint( id, description )
{
   m_point = point;
   m_type = type;
   m_thirdCoordinate = 0;
   m_scale = 1.0;
   m_pBasePoint = nullptr;
   m_pLatheLink = nullptr;
}

void PM2DControlPoint::graphicalChangeStarted()
{
	QList<PM2DControlPoint*>::iterator it;
	for( it = m_linkedPoints.begin(); it != m_linkedPoints.end(); ++it )
      if( !( *it )->selected() )
         ( *it )->graphicalChangeStarted();

   m_original2DPoint = m_point;
   m_originalPoint = to3D( m_point );
}

void PM2DControlPoint::graphicalChange( const PMVector& startPoint,
                                        const PMVector& /*viewNormal*/,
                                        const PMVector& endPoint )
{
   if( m_pBasePoint && m_pBasePoint->selected() )
      return;
   
   m_point = to2D( m_originalPoint + endPoint - startPoint );
   
   if( m_pLatheLink && m_pLatheLink->selected() )
   {
      PM2DControlPoint* ll = m_pLatheLink;
      PMVector op = ll->to2D( ll->m_originalPoint + endPoint - startPoint );
      
      m_point = to2D( m_originalPoint + endPoint - startPoint );
      if( ( m_point - m_original2DPoint ).abs() <
          ( op - ll->m_original2DPoint ).abs() )
         m_point = op;
   }

   QList<PM2DControlPoint*>::iterator it;
	for( it = m_linkedPoints.begin(); it != m_linkedPoints.end(); ++it )
   {
      ( *it )->m_point = m_point + ( *it )->m_original2DPoint
                         - m_original2DPoint;
      ( *it )->setChanged();
   }
}

void PM2DControlPoint::setBasePoint( PM2DControlPoint* p )
{
   if( p != m_pBasePoint )
   {
      if( m_pBasePoint )
         m_pBasePoint->removeLinkedPoint( this );
      m_pBasePoint = p;
      if( m_pBasePoint )
         m_pBasePoint->addLinkedPoint( this );
   }
}

void PM2DControlPoint::snapToGrid()
{
   int i;
   double d = moveGrid();
   bool diff = false;
   PMVector change( 2 );
   
   if( m_pBasePoint && m_pBasePoint->selected() )
   {
      m_point -= m_pBasePoint->m_point;
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
      m_point += m_pBasePoint->m_point;

   QList<PM2DControlPoint*>::iterator it;
	for( it = m_linkedPoints.begin(); it != m_linkedPoints.end(); ++it )
   {
      ( *it )->m_point += change;
      ( *it )->setChanged();
   }
   
   setChanged();
}

PMVector PM2DControlPoint::to2D( const PMVector& v ) const
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
   if( !approxZero( m_scale ) )
      result /= m_scale;
   return result;
}

PMVector PM2DControlPoint::to3D( const PMVector& v ) const
{
   PMVector vec( v * m_scale );
   PMVector result( 3 );
   switch( m_type )
   {
      case PM2DXY:
         result[0] = vec[0];
         result[1] = vec[1];
         result[2] = m_thirdCoordinate;
         break;
      case PM2DXZ:
         result[0] = vec[0];
         result[1] = m_thirdCoordinate;
         result[2] = vec[1];
         break;
      case PM2DYZ:
         result[0] = m_thirdCoordinate;
         result[1] = vec[0];
         result[2] = vec[1];
         break;
      case PM2DYX:
         result[1] = vec[0];
         result[0] = vec[1];
         result[2] = m_thirdCoordinate;
         break;
      case PM2DZX:
         result[2] = vec[0];
         result[0] = vec[1];
         result[1] = m_thirdCoordinate;
         break;
      case PM2DZY:
         result[2] = vec[0];
         result[1] = vec[1];
         result[0] = m_thirdCoordinate;
         break;
   }
   return result;
}

void PM2DControlPoint::addLinkedPoint( PM2DControlPoint* p )
{
   if( !m_linkedPoints.contains( p ) )
      m_linkedPoints.append( p );
}

void PM2DControlPoint::removeLinkedPoint( PM2DControlPoint* p )
{
   m_linkedPoints.removeAll( p );
}
