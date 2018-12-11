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


#include "pmpigment.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmpigmentedit.h"



PMDefinePropertyClass( PMPigment, PMPigmentProperty );

PMMetaObject* PMPigment::s_pMetaObject = nullptr;
PMObject* createNewPigment( PMPart* part )
{
   return new PMPigment( part );
}

PMPigment::PMPigment( PMPart* part ) : Base( part )
{
   m_uvMapping = false;
}

PMPigment::PMPigment( const PMPigment& p ) : Base( p )
{
   m_uvMapping = p.m_uvMapping;
}

PMPigment::~PMPigment()
{
}

PMMetaObject* PMPigment::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Pigment", Base::metaObject(),
                                        createNewPigment );
      s_pMetaObject->addProperty(
         new PMPigmentProperty( "uvMapping", &PMPigment::setUVMapping, &PMPigment::uvMapping ) );
   }
   return s_pMetaObject;
}

void PMPigment::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMPigment::description() const
{
   return ( "pigment" );
}

PMDialogEditBase* PMPigment::editWidget( QWidget* parent ) const
{
   return new PMPigmentEdit( parent );
}

void PMPigment::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "uv_mapping", m_uvMapping );
   Base::serialize( e, doc );
}

void PMPigment::readAttributes( const PMXMLHelper& h )
{
   m_uvMapping = h.boolAttribute( "uv_mapping", false );
   Base::readAttributes( h );
}

void PMPigment::setUVMapping( bool m )
{
   if( m != m_uvMapping )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUVMappingID, m_uvMapping );
      m_uvMapping = m;
   }
}

void PMPigment::restoreMemento( PMMemento *s )
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
            case PMUVMappingID:
               setUVMapping( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMPigment::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
