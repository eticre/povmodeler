/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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

#include "pmenumproperty.h"

PMEnumProperty::PMEnumProperty( const char* name, bool readOnly,
                                bool writeOnly )
      : PMPropertyBase( name, PMVariant::String, readOnly, writeOnly )
{
}

void PMEnumProperty::addEnumValue( const QString& str, int value )
{
   m_valueMap[value] = str;
   m_stringMap[str] = value;
}

QStringList PMEnumProperty::enumValues() const
{
   QStringList l;
   PMEnumStringValueMap::const_iterator it;
   for( it = m_stringMap.begin(); it != m_stringMap.end(); ++it )
      l.append( it.key() );
   return l;
}

bool PMEnumProperty::setProtected( PMObject* obj, const PMVariant& v )
{
   PMEnumStringValueMap::iterator it = m_stringMap.find( v.stringData() );
   if( it == m_stringMap.end() )
      return false;
   setEnum( obj, it.value() );
   return true;
}

PMVariant PMEnumProperty::getProtected( const PMObject* obj )
{
   int v = getEnum( obj );
   PMEnumValueStringMap::const_iterator it = m_valueMap.constFind( v );
   if( it == m_valueMap.constEnd() )
      return PMVariant( QString() );
   return it.value();
}
