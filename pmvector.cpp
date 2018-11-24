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


#include "pmvector.h"
#include "pmmath.h"
#include "pmmatrix.h"
#include "pmdebug.h"

#include <qtextstream.h>
#include <stdio.h>
#include <stdlib.h>

double PMVector::s_dummy = 0;

double& PMVector::operator[] ( int index )
{
   if( ( index >= 0 ) && ( index < ( signed ) m_size ) )
      return m_coord[index];
   qCritical(  ) << "Bad index in PMVector operator []\n";
   return s_dummy;
}

const double& PMVector::operator[] ( int index ) const
{
   if( ( index >= 0 ) && ( index < ( signed ) m_size ) )
      return m_coord[index];
   qCritical(  ) << "Bad index in PMVector operator []\n";
   return s_dummy;
}

void PMVector::allocateMemory( int size )
{
   m_size = size;
   
   if( m_size == 0 )
      m_coord = nullptr;
   else
      m_coord = ( double* ) malloc( sizeof( double ) * m_size );
}

PMVector::PMVector( const PMVector& v )
{
   int i;
   allocateMemory( v.m_size );

   for( i = 0; i < m_size; i++ )
      m_coord[i] = v.m_coord[i];
}

PMVector::PMVector()
{
   int i;
   
   allocateMemory( 3 );

   for( i = 0; i < 3; i++ )
      m_coord[i] = 0.0;
}

PMVector::PMVector( int s )
{
   int i;

   allocateMemory( s );
   
   for( i = 0; i < s; i++ )
      m_coord[i] = 0.0;
}

PMVector::PMVector( const double x, const double y )
{
   allocateMemory( 2 );

   m_coord[0] = x;
   m_coord[1] = y;
}

PMVector::PMVector( const double x, const double y, const double z )
{
   allocateMemory( 3 );

   m_coord[0] = x;
   m_coord[1] = y;
   m_coord[2] = z;
}

PMVector::PMVector( const double x, const double y, const double z,
                    const double t )
{
   allocateMemory( 4 );

   m_coord[0] = x;
   m_coord[1] = y;
   m_coord[2] = z;
   m_coord[3] = t;
}

PMVector::~PMVector()
{
   if( m_coord )
      free( m_coord );
}


void PMVector::resize( int s )
{
   int i;

   if( s != m_size )
   {
      m_coord = ( double* ) realloc( m_coord, sizeof( double ) * s );

      for( i = m_size; i < s; i++ )
         m_coord[i] = 0.0;

      if( m_coord )
         m_size = s;
      else
         m_size = 0; // possibly out of memory
   }
}

void PMVector::transform( const PMMatrix& m )
{
   (*this) = m * (*this);
}

PMVector& PMVector::operator= ( const PMVector& p )
{
   int i;
   
   resize( p.m_size );
   for( i = 0; i < m_size; i++ )
      m_coord[i] = p[i];
   return *this;
}

PMVector& PMVector::operator= ( const double d )
{
   int i;

   for( i = 0; i < m_size; i++ )
      m_coord[i] = d;
   return *this;
}

PMVector& PMVector::operator*= ( const PMVector& p )
{
   int i;

   if( m_size != p.m_size )
      resize( p.m_size );

   for( i = 0; i < m_size; i++ )
      m_coord[i] *= p[i];

   return *this;
}

PMVector& PMVector::operator/= ( const PMVector& p )
{
   int i;

   if( m_size > p.m_size )
   {
      qCritical(  ) << "Vector p is too small in PMVector& PMVector::operator/= ( const PMVector& p )\n";
   }
   else
   {
      for( i = 0; i < m_size; i++ )
      {
         if( approxZero( p[i] ) )
            qCritical(  ) << "Division by zero in PMVector::operator/= " << "\n";
         else
            m_coord[i] *= p[i];
      }
   }

   return *this;
}

PMVector& PMVector::operator*= ( double d )
{
   int i;

   for( i = 0; i < m_size; i++ )
      m_coord[i] *= d;
   
   return *this;
}

PMVector& PMVector::operator/= ( double d )
{
   if( approxZero( d ) )
   {
      qCritical(  ) << "Division by zero in PMVector::operator/= " << "\n";
   }
   else
   {
      int i;
      
      for( i = 0; i < m_size; i++ )
         m_coord[i] /= d;
   }
   
   return *this;
}

PMVector& PMVector::operator+= ( double d )
{
   int i;

   for( i = 0; i < m_size; i++ )
      m_coord[i] += d;
   
   return *this;
}

PMVector& PMVector::operator-= ( double d )
{
   int i;

   for( i = 0; i < m_size; i++ )
      m_coord[i] -= d;
   
   return *this;
}

PMVector& PMVector::operator+= ( const PMVector& p )
{
   int i;

   if( m_size < p.m_size )
      resize( p.m_size );

   for( i = 0; i < p.m_size; i++ )
      m_coord[i] += p[i];

   return *this;
}

PMVector& PMVector::operator-= ( const PMVector& p )
{
   int i;

   if( m_size < p.m_size )
      resize( p.m_size );

   for( i = 0; i < m_size; i++ )
      m_coord[i] -= p[i];

   return *this;
}

PMVector operator- ( const PMVector& p )
{
   PMVector result( p.m_size );
   int i;

   for( i = 0; i < p.m_size; i++ )
      result[i] = -p[i];
   
   return result;
}

PMVector operator+ ( const PMVector& p1, const PMVector& p2 )
{
   PMVector result( p1 );
   result += p2;
   
   return result;
}

PMVector operator- ( const PMVector& p1, const PMVector& p2 )
{
   PMVector result( p1 );
   result -= p2;
   
   return result;
}

PMVector operator* ( const PMVector& p, const double d )
{
   PMVector result( p.m_size );
   int i;

   for( i = 0; i < p.m_size; i++ )
      result[i] = p[i] * d;
   
   return result;
}

PMVector operator/ ( const PMVector& p, const double d )
{
   PMVector result( p.m_size );
   int i;

   if( approxZero( d ) )
      qCritical(  ) << "Division by zero in PMVector::operator/ ( PMVector& p, double d ) " << "\n";
   else
      for( i = 0; i < p.m_size; i++ )
         result[i] = p[i] / d;
   
   return result;
}

PMVector operator+ ( const PMVector& p, const double d )
{
   PMVector result( p.m_size );
   int i;

   for( i = 0; i < p.m_size; i++ )
      result[i] = p[i] + d;
   
   return result;
}

PMVector operator- ( const PMVector& p, const double d )
{
   PMVector result( p.m_size );
   int i;

   for( i = 0; i < p.m_size; i++ )
      result[i] = p[i] - d;
   
   return result;
}

PMVector operator* ( const double d, const PMVector& p )
{
   PMVector result( p.m_size );
   int i;

   for( i = 0; i < p.m_size; i++ )
      result[i] = p[i] * d;
   
   return result;
}

PMVector operator* ( const PMMatrix& m, const PMVector& p )
{
   PMVector result( 3 );
   int c, i;
   // for homogenous coordinates
   double u;

   if( p.m_size != 3 )
      qCritical(  ) << "Vector has not size 3 in PMVector operator* ( const PMVector& p, const PMMatrix& m ) \n";
   else
   {
      for( c=0; c<3; c++ )
      {
         result[c] = 0.0;
         for( i=0; i<4; i++ )
            result[c] += m[i][c] * ( i<3 ? p[i] : 1.0 );
      }
      
      u = 0.0;
      for( i=0; i<4; i++ )
         u += m[i][3] * ( i<3 ? p[i] : 1.0 );
      if( !approxZero( u ) )
         for( i=0; i<3; i++ )
            result[i] /= u;
   }

   return result;
}

PMVector operator* ( const PMVector& p1, const PMVector& p2 )
{
   PMVector result( p1 );
   result *= p2;
   
   return result;
}

PMVector operator/ ( const PMVector& p1, const PMVector& p2 )
{
   PMVector result( p1 );
   result /= p2;
   
   return result;
}

bool PMVector::operator== ( const PMVector& p ) const
{
   int i;
   if( m_size != p.m_size )
      return false;
   if( m_size == 0 )
      return true;

   for( i = 0; i < m_size; i++ )
      if( p.m_coord[i] != m_coord[i] )
         return false;
   return true;
}

bool PMVector::approxEqual( const PMVector& p, double epsilon ) const
{
   int i;
   if( m_size != p.m_size )
      return false;
   if( m_size == 0 )
      return true;

   for( i = 0; i < m_size; i++ )
      if( ! approx( p.m_coord[i], m_coord[i], epsilon ) )
         return false;
   return true;
}

bool PMVector::operator!= ( const PMVector& p ) const
{
   return !( *this == p );
}


PMVector PMVector::cross( const PMVector& v1, const PMVector& v2 )
{
   PMVector result;
   if( ( v1.size() == 3 ) && ( v2.size() == 3 ) )
   {
      result[0] = v1[1]*v2[2] - v1[2]*v2[1];
      result[1] = v1[2]*v2[0] - v1[0]*v2[2];
      result[2] = v1[0]*v2[1] - v1[1]*v2[0];
   }
   else
      qCritical(  ) << "Wrong sizes in PMVector::cross()\n";
   
   return result;
}

double PMVector::dot( const PMVector& v1, const PMVector& v2 )
{
   double result = 0.0;
   int i;
   
   if( v1.size() == v2.size() )
   {
      for( i = 0; i < v1.size(); i++ )
         result += v1[i] * v2[i];
   }
   else
      qCritical(  ) << "Wrong sizes in PMVector::dot()\n";
   
   return result;
}

double PMVector::angle( const PMVector& v1, const PMVector& v2 )
{
   PMVector cr;
   double s, c, n;
   double a = 0;
   int i;
   
   if( ( v1.size() == 3 ) && ( v2.size() == 3 ) )
   {
      n = v1.abs() * v2.abs();

      if( approxZero( n ) )
         a = 0;
      else
      {
         cr = cross( v1, v2 );
         s = cr.abs() / n;

         c = 0;
         for( i = 0; i < 3; i++ )
            c += v1[i] * v2[i];

         c = c / n;

         a = pmatan( s, c );
      }
   }      
   else
      qCritical(  ) << "Wrong sizes in PMVector::angle()\n";

   return a;
}

double PMVector::abs() const
{
   int i;
   double a = 0.0;

   for( i = 0; i < m_size; i++ )
      a += m_coord[i] * m_coord[i];
   
   return sqrt( a );
}

PMVector PMVector::orthogonal() const
{
   PMVector result;
   double l, rl;

   l = abs();
   if( approxZero( l ) )
   {
      qCritical(  ) << "Can't calculate an orthogonal vector from a null vector\n";
      return PMVector( 1, 0, 0 );
   }
   
   result = cross( (*this) / l, PMVector( 0, 0, 1 ) );
   rl = result.abs();
   if( rl < 0.001 )
   {
      result = cross( (*this) / l, PMVector( 1, 0, 0 ) );
      rl = result.abs();
   }
   return result / rl;
}

QString PMVector::serialize() const
{
   QString result;
   QTextStream str( &result, QIODevice::WriteOnly );
   int i;

   if( m_size > 0 )
   {
      str << '<';
      for( i = 0; i < m_size; i++ )
      {
         if( i > 0 )
            str << ", ";
         str << m_coord[i];
      }
      str << '>';
   }
   else
      qCritical(  ) << "Can't serialize a vector with size 0\n";
   
   return result;
}

QString PMVector::serializeXML() const
{
   QString result;
   QTextStream str( &result, QIODevice::WriteOnly );
   int i;

   if( m_size > 0 )
   {
      for( i = 0; i < m_size; i++ )
      {
         if( i > 0 )
            str << ' ';
         str << m_coord[i];
      }
   }
   else
      qCritical(  ) << "Can't serialize a vector with size 0\n";
   
   return result;
}

bool PMVector::loadXML( const QString& str )
{
   int i;
   int size = str.count( ' ' ) + 1;
   QString tmp( str );
   QTextStream s( &tmp, QIODevice::ReadOnly );
   QString val;
   bool ok;
   
   resize( size );
   for( i = 0; i < size; i++ )
   {
      s >> val;
      m_coord[i] = val.toDouble( &ok );
      if( !ok )
         return false;
   }
   return true;
}
