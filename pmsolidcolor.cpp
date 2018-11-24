/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#include "pmsolidcolor.h"

#include "pmxmlhelper.h"
#include "pmsolidcoloredit.h"
#include "pmmemento.h"



const PMColor colorDefault = PMColor( 0.0, 0.0, 0.0, 0.0, 0.0 );
const bool srgbDefault = true;

PMDefinePropertyClass( PMSolidColor, PMSolidColorProperty );

PMMetaObject* PMSolidColor::s_pMetaObject = nullptr;
PMObject* createNewSolidColor( PMPart* part )
{
   return new PMSolidColor( part );
}

PMSolidColor::PMSolidColor( PMPart* part )
      : Base( part )
{
   m_color = colorDefault;
   m_color.setSrgb( srgbDefault );
}

PMSolidColor::PMSolidColor( const PMSolidColor& c )
      : Base( c )
{
   m_color = c.m_color;
}

PMSolidColor::~PMSolidColor()
{
}

QString PMSolidColor::description() const
{
   return ( "solid color" );
}

void PMSolidColor::serialize( QDomElement& e, QDomDocument& /*doc*/ ) const
{

    if( m_color.getSrgb() )
        e.setAttribute( "colorsrgb", m_color.serializeXML() );
    else
        e.setAttribute( "color", m_color.serializeXML() );

}

void PMSolidColor::readAttributes( const PMXMLHelper& h )
{
   m_color = h.colorAttribute( "color", colorDefault );
}

PMMetaObject* PMSolidColor::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "SolidColor", Base::metaObject(),
                                        createNewSolidColor );
      s_pMetaObject->addProperty(
         new PMSolidColorProperty( "color", &PMSolidColor::setColor, &PMSolidColor::color ) );
   }
   return s_pMetaObject;
}

void PMSolidColor::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMSolidColor::setColor( const PMColor& c )
{
   if( c != m_color )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMColorID, m_color );
      m_color = c;
   }
}

PMDialogEditBase* PMSolidColor::editWidget( QWidget* parent ) const
{
   return new PMSolidColorEdit( parent );
}

void PMSolidColor::restoreMemento( PMMemento* s )
{
	const QList<PMMementoData*>& changes = s->changes();
	QList<PMMementoData*>::const_iterator it;
   PMMementoData* data;
	for( it = changes.begin(); it != changes.end(); ++it )
	{
		data = *it;
      if( data->objectType() == s_pMetaObject )
      {
         switch( data->valueID() )
         {
            case PMColorID:
               setColor( data->colorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMSolidColor::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
