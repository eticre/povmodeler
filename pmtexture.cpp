/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Luis Carvalho
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


#include "pmtexture.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmtextureedit.h"



PMDefinePropertyClass( PMTexture, PMTextureProperty );

PMMetaObject* PMTexture::s_pMetaObject = 0;
PMObject* createNewTexture( PMPart* part )
{
   return new PMTexture( part );
}

PMTexture::PMTexture( PMPart* part ) : Base( part )
{
   m_uvMapping = false;
}

PMTexture::PMTexture( const PMTexture& t ) : Base( t )
{
   m_uvMapping = t.m_uvMapping;
}

PMTexture::~PMTexture()
{
}

PMMetaObject* PMTexture::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Texture", Base::metaObject(),
                                        createNewTexture );
      s_pMetaObject->addProperty(
         new PMTextureProperty( "uvMapping", &PMTexture::setUVMapping, &PMTexture::uvMapping ) );
   }
   return s_pMetaObject;
}

void PMTexture::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMTexture::description() const
{
   return ( "texture" );
}

PMDialogEditBase* PMTexture::editWidget( QWidget* parent ) const
{
   return new PMTextureEdit( parent );
}

void PMTexture::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "uv_mapping", m_uvMapping );
   Base::serialize( e, doc );
}

void PMTexture::readAttributes( const PMXMLHelper& h )
{
   m_uvMapping = h.boolAttribute( "uv_mapping", false );
   Base::readAttributes( h );
}

void PMTexture::setUVMapping( bool m )
{
   if( m != m_uvMapping )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUVMappingID, m_uvMapping );
      m_uvMapping = m;
   }
}

void PMTexture::restoreMemento( PMMemento *s )
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
               qCritical(  ) << "Wrong ID in PMTexture::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
