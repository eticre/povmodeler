/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
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


#include "pmquickcolor.h"

#include "pmxmlhelper.h"
#include "pmquickcoloredit.h"
#include "pmmemento.h"

#include <QLocale>

const PMColor colorDefault = PMColor( 1.0, 1.0, 1.0, 0.0, 0.0 );

PMDefinePropertyClass( PMQuickColor, PMQuickColorProperty );

PMMetaObject* PMQuickColor::s_pMetaObject = 0;
PMObject* createNewQuickColor( PMPart* part )
{
   return new PMQuickColor( part );
}

PMQuickColor::PMQuickColor( PMPart* part )
      : Base( part )
{
   m_color = colorDefault;
}

PMQuickColor::PMQuickColor( const PMQuickColor& c )
      : Base( c )
{
   m_color = c.m_color;
}

PMQuickColor::~PMQuickColor()
{
}

QString PMQuickColor::description() const
{
   return ( "quick color" );
}

void PMQuickColor::serialize( QDomElement& e, QDomDocument& /*doc*/ ) const
{
   e.setAttribute( "quickcolor", m_color.serializeXML() );
}

void PMQuickColor::readAttributes( const PMXMLHelper& h )
{
   m_color = h.colorAttribute( "quickcolor", colorDefault );
}

PMMetaObject* PMQuickColor::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "QuickColor", Base::metaObject(),
                                        createNewQuickColor );
      s_pMetaObject->addProperty(
         new PMQuickColorProperty( "color", &PMQuickColor::setColor, &PMQuickColor::color ) );
   }
   return s_pMetaObject;
}

void PMQuickColor::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMQuickColor::setColor( const PMColor& c )
{
   if( c != m_color )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMColorID, m_color );
         m_pMemento->setViewStructureChanged();
      }
      m_color = c;
   }
}

PMDialogEditBase* PMQuickColor::editWidget( QWidget* parent ) const
{
   return new PMQuickColorEdit( parent );
}

void PMQuickColor::restoreMemento( PMMemento* s )
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
               qCritical(  ) << "Wrong ID in PMQuickColor::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
