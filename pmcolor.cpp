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


#include "pmcolor.h"
#include "pmvector.h"
#include "pmmath.h"

#include "pmdebug.h"

#include <qtextstream.h>
#include <qstring.h>


PMColor::PMColor()
{
   useSrgb = false;
   int i;
   for( i = 0; i < 6; i++ )
      m_colorValue[i] = 0;
}

PMColor::PMColor( const double red, const double green, const double blue,
         const double filter, const double transmit )
{
   useSrgb = false;
   m_colorValue[0] = red;
   m_colorValue[1] = green;
   m_colorValue[2] = blue;
   m_colorValue[3] = filter;
   m_colorValue[4] = transmit;
   m_colorValue[5] = 0;
}

PMColor::PMColor( const double red, const double green, const double blue )
{
   useSrgb = false;
   m_colorValue[0] = red;
   m_colorValue[1] = green;
   m_colorValue[2] = blue;
   m_colorValue[3] = 0;
   m_colorValue[4] = 0;
   m_colorValue[5] = 0;
}

PMColor::PMColor( const PMVector& v )
{  //eticre todo verify srgb from vector
   int i;
   if( v.size() != 5 )
   {
      qCritical(  ) << "Vector has wrong size in PMColor::PMColor( const PMVector& v )\n";
      for( i = 0; i < 5; i++ )
         m_colorValue[i] = 0;
   }
   else
   {
      for( i = 0; i < 5; i++ )
         m_colorValue[i] = v[i];
   }
}

PMColor::PMColor( const QColor& c )
{
   useSrgb = false;
   m_colorValue[0] = c.red() / 255.0;
   m_colorValue[1] = c.green() / 255.0;
   m_colorValue[2] = c.blue() / 255.0;
   m_colorValue[3] = 0.0;
   m_colorValue[4] = 0.0;
}

QColor PMColor::toQColor() const
{
   double r, g, b, max = 0;
   r = red();
   g = green();
   b = blue();

   if( r < 0 ) r = 0;
   if( g < 0 ) g = 0;
   if( b < 0 ) b = 0;
   
   if( r > max )
      max = r;
   if( g > max )
      max = g;
   if( b > max )
      max = b;
   if( max > 1 )
   {
      r /= max;
      g /= max;
      b /= max;
   }
   return QColor( ( int ) ( r * 255 + 0.5 ), ( int ) ( g * 255 + 0.5 ),
                  ( int ) ( b * 255 + 0.5 ) );
}

QString PMColor::serialize( bool addColorKeyword ) const
{
   QString result;
   QTextStream str( &result, QIODevice::WriteOnly );
   QString rgb_select;

   if ( useSrgb )
       { rgb_select = "srgb"; }
   else
       { rgb_select = "rgb"; }


   if( addColorKeyword )
      str << "color ";

   if( approxZero( m_colorValue[4] ) )
   {
      if( approxZero( m_colorValue[3] ) )
      {
         // rgb
         rgb_select = rgb_select + "<";
         str << rgb_select << m_colorValue[0] << ", " << m_colorValue[1] << ", "
             << m_colorValue[2] << '>';
      }
      else
      {
         // rgbf
         rgb_select = rgb_select + "f <";
         str << rgb_select << m_colorValue[0] << ", " << m_colorValue[1] << ", "
             << m_colorValue[2] << ", " << m_colorValue[3] << '>';
      }
   }
   else
   {
      if( approxZero( m_colorValue[3] ) )
      {
         // rgbt
         rgb_select = rgb_select + "t <";
         str << rgb_select << m_colorValue[0] << ", " << m_colorValue[1] << ", "
             << m_colorValue[2] << ", " << m_colorValue[4] << '>';
      }
      else
      {
         // rgbft
         rgb_select = rgb_select + "ft <";
         str << rgb_select << m_colorValue[0] << ", " << m_colorValue[1] << ", "
             << m_colorValue[2] << ", " << m_colorValue[3]
             << ", " << m_colorValue[4] << '>';
      }
   }
   
   return result;
}

QString PMColor::serializeXML() const
{
   QString result;
   QTextStream str( &result, QIODevice::WriteOnly );
   int i;

   for( i = 0; i < 6; i++ )
   {
      if( i != 0 )
         str << ' ';
      str << m_colorValue[i];
   }
   
   return result;
}

bool PMColor::operator!= ( const PMColor& c ) const
{
   return !( *this == c );
}

bool PMColor::operator== ( const PMColor& c ) const
{
   unsigned int i;

   for( i = 0; i < 5; i++ )
      if( c.m_colorValue[i] != m_colorValue[i] )
         return false;
   //eticre srgb
   if( c.useSrgb != useSrgb )
       return false;

   return true;
}

bool PMColor::loadXML( const QString& str )
{
   QString tmp( str );
   QTextStream s( &tmp, QIODevice::ReadOnly );
   QString val;
   bool ok;
   int i;
   
   for( i = 0; i < 5; i++ )
   {
      s >> val;
      m_colorValue[i] = val.toDouble( &ok );

      if( !ok )
         val.replace(",", ".");

      m_colorValue[i] = val.toDouble( &ok );

      if( !ok )
         return false;
   }   
   return true;
}
