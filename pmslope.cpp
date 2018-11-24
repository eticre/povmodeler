/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Luis Carvalho
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

#include "pmxmlhelper.h"
#include "pmvector.h"
#include "pmslope.h"
#include "pmslopeedit.h"
#include "pmmemento.h"



const double heightDefault = 0;
const double slopeDefault = 0;

PMDefinePropertyClass( PMSlope, PMSlopeProperty );

PMMetaObject* PMSlope::s_pMetaObject = 0;
PMObject* createNewSlope( PMPart* part )
{
   return new PMSlope( part );
}

PMSlope::PMSlope( PMPart* part )
      : Base( part )
{
   m_height = heightDefault;
   m_slope = slopeDefault;
}

PMSlope::PMSlope( const PMSlope& s )
      : Base( s )
{
   m_height = s.m_height;
   m_slope = s.m_slope;
}

PMSlope::~PMSlope()
{
}

QString PMSlope::description() const
{
   return ( "slope" );
}

void PMSlope::serialize( QDomElement& e, QDomDocument& /*doc*/ ) const
{
   e.setAttribute( "height", m_height );
   e.setAttribute( "slope", m_slope );
}

void PMSlope::readAttributes( const PMXMLHelper& h )
{
   m_height = h.doubleAttribute( "height", heightDefault );
   m_slope = h.doubleAttribute( "slope", slopeDefault );
}

PMMetaObject* PMSlope::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Slope", Base::metaObject(),
                                        createNewSlope );
      s_pMetaObject->addProperty(
         new PMSlopeProperty( "height", &PMSlope::setHeight, &PMSlope::height ) );
      s_pMetaObject->addProperty(
         new PMSlopeProperty( "slope", &PMSlope::setSlope, &PMSlope::slope ) );
   }
   return s_pMetaObject;
}

void PMSlope::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMSlope::setHeight( const double c )
{
   if( c != m_height )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHeightID, m_height );
      m_height = c;
   }
}

void PMSlope::setSlope( const double c )
{
   if( c != m_slope )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeID, m_slope );
      m_slope = c;
   }
}

PMDialogEditBase* PMSlope::editWidget( QWidget* parent ) const
{
   return new PMSlopeEdit( parent );
}

void PMSlope::restoreMemento( PMMemento* s )
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
            case PMSlopeID:
               setSlope( data->doubleData() );
               break;
            case PMHeightID:
               setHeight( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMSlope::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
