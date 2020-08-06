/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmiomanager.h"
#include "pmdebug.h"
#include "pmpovray31format.h"
#include "pmpovray35format.h"

PMIOFormat::PMIOFormat()
{
}

PMIOFormat::~PMIOFormat()
{

}

PMIOManager::PMIOManager( PMPart* part )
{
   m_pPart = part;

   addFormat( new PMPovray35Format() );
   //addFormat( new PMPovray31Format() );
}

PMIOManager::~PMIOManager()
{
   while( !m_formats.isEmpty() )
      delete m_formats.takeFirst();
}

void PMIOManager::addFormat( PMIOFormat* format )
{
   if( !format )
      return;
   if( !m_formats.contains( format ) )
   {
      if( !m_dict.value( format->name() ) )
      {
         m_formats.append( format );
         m_dict.insert( format->name(), format );
      }
      else
         qCritical(  ) << "Format " << format->name() << "already registered" << Qt::endl;
   }
   else
      qCritical(  ) << "Format " << format->name() << "already registered" << Qt::endl;
}

void PMIOManager::removeFormat( const QString& name )
{
   PMIOFormat* pFormat = format( name );
   if( pFormat )
   {
      m_dict.remove( name );
      m_formats.removeAll( pFormat );
   }
}

PMIOFormat* PMIOManager::format( const QString& name ) const
{
   return m_dict.value( name );
}

PMIOFormat* PMIOManager::formatForMimeType( const QString& mime ) const
{
   foreach( PMIOFormat* pit, m_formats)
   {
      if( pit->mimeType() == mime )
         return pit;
   }
   return nullptr;
}
