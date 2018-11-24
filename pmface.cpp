/*
**************************************************************************
                               description
                           --------------------
  copyright            : (C) 2004 by Leon Pennington
  email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/
#include <new>
#include "pmface.h"

GLuint PMFace::s_dummy = 0;

PMFace::PMFace( const PMFace& face )
{
   m_points = new( std::nothrow ) GLuint[ face.m_size ];
   for( unsigned i = 0; i < face.m_size; ++i )
      m_points[ i ] = face.m_points[ i ];
   m_size = face.m_size;
   m_normal = face.m_normal;
}

PMFace::PMFace( const GLuint pt1, const GLuint pt2, const GLuint pt3, const PMVector& normal )
{
   m_points = new( std::nothrow ) GLuint[ 3 ];
   m_points[ 0 ] = pt1;
   m_points[ 1 ] = pt2;
   m_points[ 2 ] = pt3;
   m_size = 3;
   m_normal = normal;
}

PMFace::PMFace( const GLuint pt1, const GLuint pt2, const GLuint pt3, const GLuint pt4, const PMVector& normal )
{
   m_points = new( std::nothrow ) GLuint[ 4 ];
   m_points[ 0 ] = pt1;
   m_points[ 1 ] = pt2;
   m_points[ 2 ] = pt3;
   m_points[ 3 ] = pt4;
   m_size = 4;
   m_normal = normal;
}

PMFace& PMFace::operator=( const PMFace& face )
{
   delete m_points;
   m_points = new( std::nothrow ) GLuint[ face.m_size ];
   for( unsigned i = 0; i < face.m_size; ++i )
      m_points[ i ] = face.m_points[ i ];
   m_size = face.m_size;
   m_normal = face.m_normal;
   return *this;
}

bool PMFace::operator==( const PMFace& face ) const
{
   if( m_size == face.m_size && m_normal == face.m_normal )
   {
      for( unsigned i = 0; i < m_size; ++i )
      {
         if( m_points[i] != face.m_points[i] )
            return false;
      }
      return true;
   }
   else
      return false;
}

bool PMFace::operator!=( const PMFace& face ) const
{
   return !( *this == face );
}

bool PMFace::resize( const unsigned int size )
{
   if( size != m_size )
   {
      GLuint* temp = new( std::nothrow ) GLuint[ size ];
      if( temp )
      {
         for( unsigned i = 0; i < m_size && i < size; ++i )
            temp[i] = m_points[i];

         for( unsigned i = m_size; i < size; ++i )
            temp[i] = 0;

         delete [] m_points;
         m_points = temp;
      }
      else
         return false;
   }
   return true;
}

GLuint& PMFace::operator[] ( int index )
{
   if( ( index >= 0 ) && ( index < ( signed ) m_size ) )
      return m_points[index];
   qCritical(  ) << "Bad index in PMFace operator []\n";
   return s_dummy;
}

const GLuint& PMFace::operator[] ( int index ) const
{
   if( ( index >= 0 ) && ( index < ( signed ) m_size ) )
      return m_points[index];
   qCritical(  ) << "Bad index in PMFace operator []\n";
   return s_dummy;
}
