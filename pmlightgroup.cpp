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


#include "pmlightgroup.h"


#include "pmxmlhelper.h"
#include "pmlightgroupedit.h"
#include "pmmemento.h"

PMDefinePropertyClass( PMLightGroup, PMLightGroupProperty );

PMMetaObject* PMLightGroup::s_pMetaObject = 0;
PMObject* createNewLightGroup( PMPart* part )
{
   return new PMLightGroup( part );
}

PMLightGroup::PMLightGroup( PMPart* part )
      : Base( part )
{
   m_globalLights = false;
}

PMLightGroup::PMLightGroup( const PMLightGroup& lg )
      : Base( lg )
{
   m_globalLights = lg.m_globalLights;
}

PMLightGroup::~PMLightGroup()
{
}

QString PMLightGroup::description() const
{
   return QString( "light group" );
}

void PMLightGroup::serialize( QDomElement& e, QDomDocument& doc ) const
{
   if( m_globalLights )
      e.setAttribute( "global_lights", "1" );
   else
      e.setAttribute( "global_lights", "0" );

   Base::serialize( e, doc );
}

void PMLightGroup::readAttributes( const PMXMLHelper& h )
{
   m_globalLights = h.boolAttribute( "global_lights", false );

   Base::readAttributes( h );
}

PMMetaObject* PMLightGroup::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "LightGroup", Base::metaObject(),
                                        createNewLightGroup );

      s_pMetaObject->addProperty( new PMLightGroupProperty( "globalLights",
         &PMLightGroup::setGlobalLights, &PMLightGroup::globalLights ) );

   }
   return s_pMetaObject;
}

void PMLightGroup::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMLightGroup::setGlobalLights( bool gl )
{
   if( gl != m_globalLights )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMGlobalLightsID, m_globalLights );
      m_globalLights = gl;
   }
}

PMDialogEditBase* PMLightGroup::editWidget( QWidget* parent ) const
{
   return new PMLightGroupEdit( parent );
}

void PMLightGroup::restoreMemento( PMMemento* s )
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
            case PMGlobalLightsID:
               setGlobalLights( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMCSG::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

