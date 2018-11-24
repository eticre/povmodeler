/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmpalettevalue.h"

PMPaletteValue::PMPaletteValue()
{
   m_index = 0;
   m_value = 0.0;
}

PMPaletteValue::PMPaletteValue( int idx, double value )
{
   m_index = idx;
   m_value = value;
}

void PMPaletteValue::setIndex( int idx )
{
   m_index = idx;
}

void PMPaletteValue::setValue( double value )
{
   m_value = value;
}

void PMPaletteValue::serialize( QDomElement& e, QDomDocument& /* doc */ ) const
{
   e.setAttribute( "index", m_index );
   e.setAttribute( "value", m_value );
}

void PMPaletteValue::readAttributes( const QDomElement& h )
{
   QString str;
   bool ok;

   str = h.attribute( "index" );
   if( str.isNull() )
      m_index = 0;
   else
   {
      m_index = str.toInt( &ok );
      if( !ok ) m_index = 0;
   }

   str = h.attribute( "value" );
   if( str.isNull() )
      m_value = 0;
   else
   {
      m_value = str.toDouble( &ok );
      if( !ok ) m_value = 0;
   }
}

bool PMPaletteValue::operator==( const PMPaletteValue& p ) const
{
   return ( m_index == p.m_index && m_value == p.m_value );
}

bool PMPaletteValue::operator!=( const PMPaletteValue& p ) const
{
   return ( m_index != p.m_index || m_value != p.m_value );
}
