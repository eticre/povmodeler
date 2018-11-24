/*
**************************************************************************
                                 description
                           --------------------
   copyright            : (C) 2003 by Leon Pennington
   email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmphotons.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmphotonsedit.h"



const double spacingMultiDefault = 1.0;

PMDefinePropertyClass( PMPhotons, PMPhotonsProperty );

PMMetaObject* PMPhotons::s_pMetaObject = 0;
PMObject* createNewPhotons( PMPart* part )
{
   return new PMPhotons( part );
}

PMPhotons::PMPhotons( PMPart* part ) : Base( part )
{
   m_target = true;
   m_spacingMulti = spacingMultiDefault;
   m_refraction = false;
   m_reflection = false;
   m_collect = true;
   m_passThrough = false;
   m_areaLight = false;
}

PMPhotons::PMPhotons( const PMPhotons& p )
      : Base( p )
{
   m_target = p.m_target;
   m_spacingMulti = p.m_spacingMulti;
   m_refraction = p.m_refraction;
   m_reflection = p.m_reflection;
   m_collect = p.m_collect;
   m_passThrough = p.m_passThrough;
   m_areaLight = p.m_areaLight;
}

PMPhotons::~PMPhotons()
{
}

PMMetaObject* PMPhotons::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Photons", Base::metaObject(),
                                       createNewPhotons );

      s_pMetaObject->addProperty( new PMPhotonsProperty( "target",
         &PMPhotons::setTarget, &PMPhotons::target ) );
      s_pMetaObject->addProperty( new PMPhotonsProperty( "spacingMulti",
         &PMPhotons::setSpacingMulti, &PMPhotons::spacingMulti ) );
      s_pMetaObject->addProperty( new PMPhotonsProperty( "refraction",
         &PMPhotons::setRefraction, &PMPhotons::refraction ) );
      s_pMetaObject->addProperty( new PMPhotonsProperty( "reflection",
         &PMPhotons::setReflection, &PMPhotons::reflection ) );
      s_pMetaObject->addProperty( new PMPhotonsProperty( "collect",
         &PMPhotons::setCollect, &PMPhotons::collect ) );
      s_pMetaObject->addProperty( new PMPhotonsProperty( "passThrough",
         &PMPhotons::setPassThrough, &PMPhotons::passThrough ) );
      s_pMetaObject->addProperty( new PMPhotonsProperty( "areaLight",
         &PMPhotons::setAreaLight, &PMPhotons::areaLight ) );
   }
   return s_pMetaObject;
}

void PMPhotons::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMPhotons::description() const
{
   return ( "photons" );
}

void PMPhotons::serialize( QDomElement& e, QDomDocument& ) const
{
   e.setAttribute( "target", m_target );
   e.setAttribute( "spacing_multi", m_spacingMulti );
   e.setAttribute( "refraction", m_refraction );
   e.setAttribute( "reflection", m_reflection );
   e.setAttribute( "collect", m_collect );
   e.setAttribute( "pass_through", m_passThrough );
   e.setAttribute( "area_light", m_areaLight );
}

void PMPhotons::readAttributes( const PMXMLHelper& h )
{
   m_target = h.boolAttribute( "target", true );
   m_spacingMulti = h.doubleAttribute( "spacing_multi", spacingMultiDefault );
   m_refraction = h.boolAttribute( "refraction", false );
   m_reflection = h.boolAttribute( "reflection", false );
   m_collect = h.boolAttribute( "collect", true );
   m_passThrough = h.boolAttribute( "pass_through", false );
   m_areaLight = h.boolAttribute( "area_light", false );
}

void PMPhotons::setTarget( bool t )
{
   if( t != m_target )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMTargetID, m_target );
      m_target = t;
   }
}

void PMPhotons::setSpacingMulti( double sm )
{
   if ( sm != m_spacingMulti )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSpacingMultiID, m_spacingMulti );
      m_spacingMulti = sm;
   }
}

void PMPhotons::setRefraction( bool r )
{
   if ( r != m_refraction )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRefractionID, m_refraction );
      m_refraction = r;
   }
}

void PMPhotons::setReflection( bool r )
{
   if ( r != m_reflection )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMReflectionID, m_reflection);
      m_reflection = r;
   }
}

void PMPhotons::setCollect( bool c )
{
   if ( c != m_collect )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCollectID, m_collect );
      m_collect = c;
   }
}

void PMPhotons::setPassThrough( bool pt )
{
   if ( pt != m_passThrough )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPassThroughID, m_passThrough );
      m_passThrough = pt;
   }
}

void PMPhotons::setAreaLight( bool al )
{
   if ( al != m_areaLight )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAreaLightID, m_areaLight );
      m_areaLight = al;
   }
}

PMDialogEditBase* PMPhotons::editWidget( QWidget* parent ) const
{
   return new PMPhotonsEdit( parent );
}

void PMPhotons::restoreMemento( PMMemento* s )
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
            case PMTargetID:
               setTarget( data->boolData() );
               break;
            case PMSpacingMultiID:
               setSpacingMulti( data->doubleData() );
               break;
            case PMRefractionID:
               setRefraction( data->boolData() );
               break;
            case PMReflectionID:
               setReflection( data->boolData() );
               break;
            case PMCollectID:
               setCollect( data->boolData() );
               break;
            case PMPassThroughID:
               setPassThrough( data->boolData() );
               break;
            case PMAreaLightID:
               setAreaLight( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMRadiosity::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
