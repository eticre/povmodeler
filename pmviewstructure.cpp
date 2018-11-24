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


#include "pmviewstructure.h"

/*
void PMViewStructure::render()
{
   glVertexPointer( 3, GL_DOUBLE, sizeof( PMVector ), points.data() );
   glDrawElements( GL_LINES, lines.size() * 2,
                   GL_UNSIGNED_INT, lines.data() );
}
*/

PMViewStructure::PMViewStructure()
{
   m_parameterKey = -1;
}

PMViewStructure::PMViewStructure( unsigned int n, unsigned int l, unsigned int f )
{
   m_points.resize( n );
   m_lines.resize( l );
   m_faces.resize( f );
   m_parameterKey = -1;
}

PMViewStructure::PMViewStructure( const PMViewStructure& vs )
{
   m_points = vs.m_points;
   m_lines = vs.m_lines;
   m_faces = vs.m_faces;
   m_parameterKey = vs.m_parameterKey;
}

PMViewStructure::PMViewStructure( const PMViewStructure* vs )
{
   m_points = vs->m_points;
   m_lines = vs->m_lines;
   m_faces = vs->m_faces;
   m_parameterKey = vs->m_parameterKey;
}

PMViewStructure& PMViewStructure::operator = ( const PMViewStructure& vs )
{
   m_lines = vs.m_lines;
   m_points = vs.m_points;
   m_faces = vs.m_faces;

   return *this;
}

bool PMViewStructure::operator == ( const PMViewStructure& vs ) const
{
   return ( ( m_lines.data() == vs.m_lines.data() )
            && ( m_points.data() == vs.m_points.data() )
            && ( m_faces == vs.m_faces ) );
}

bool PMViewStructure::operator != ( const PMViewStructure& vs ) const
{
   return ( ( m_lines.data() != vs.m_lines.data() )
            || ( m_points.data() != vs.m_points.data() )
            || !( m_faces == vs.m_faces ) );
}


PMBoundingBox::PMBoundingBox( const PMVector& min, const PMVector& max )
{
   m_bValid = true;
   m_min = min;
   m_max = max;
}

PMBoundingBox::PMBoundingBox()
{
   m_bValid = false;
   m_min = PMVector( 0.0, 0.0, 0.0 );
   m_max = PMVector( 0.0, 0.0, 0.0 );
}

void PMBoundingBox::mergeWith( const PMBoundingBox& box )
{
   if( m_bValid )
   {
      if( box.m_bValid )
      {
         if( box.m_min.x() < m_min.x() )
            m_min.setX( box.m_min.x() );
         if( box.m_min.y() < m_min.y() )
            m_min.setY( box.m_min.y() );
         if( box.m_min.z() < m_min.z() )
            m_min.setZ( box.m_min.z() );

         if( box.m_max.x() > m_max.x() )
            m_max.setX( box.m_max.x() );
         if( box.m_max.y() > m_max.y() )
            m_max.setY( box.m_max.y() );
         if( box.m_max.z() > m_max.z() )
            m_max.setZ( box.m_max.z() );
      }
   }
   else
   {
      if( box.m_bValid )
      {
         m_bValid = true;
         m_max = box.m_max;
         m_min = box.m_min;
      }
   }
}
