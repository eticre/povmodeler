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


#include "pmrainbow.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmrainbowedit.h"
#include "pmvector.h"



PMMetaObject* PMRainbow::s_pMetaObject = 0;
PMObject* createNewRainbow( PMPart* part )
{
   return new PMRainbow( part );
}

const PMVector directionDefault = PMVector( 0.0, 0.0, 0.0 );
const double angleDefault = 0.0;
const double widthDefault = 0.0;
const double distanceDefault = 0.0;
const double jitterDefault = 0.0;
const PMVector upDefault = PMVector( 0.0, 0.0, 0.0 );
const double arcAngleDefault = 0.0;
const double falloffAngleDefault = 0.0;

PMDefinePropertyClass( PMRainbow, PMRainbowProperty );

PMRainbow::PMRainbow( PMPart* part )
      : Base( part )
{
   m_direction = directionDefault;
   m_angle = angleDefault;
   m_width = widthDefault;
   m_distance = distanceDefault;
   m_jitter = jitterDefault;
   m_up = upDefault;
   m_arcAngle = arcAngleDefault;
   m_falloffAngle = falloffAngleDefault;
   m_enableDirection = false;
   m_enableAngle = false;
   m_enableWidth = false;
   m_enableDistance = false;
   m_enableJitter = false;
   m_enableUp = false;
   m_enableArcAngle = false;
   m_enableFalloffAngle = false;
}

PMRainbow::PMRainbow( const PMRainbow& r )
      : Base( r )
{
   m_direction = r.m_direction;
   m_angle = r.m_angle;
   m_width = r.m_width;
   m_distance = r.m_distance;
   m_jitter = r.m_jitter;
   m_up = r.m_up;
   m_arcAngle = r.m_arcAngle;
   m_falloffAngle = r.m_falloffAngle;
   m_enableDirection = r.m_enableDirection;
   m_enableAngle = r.m_enableAngle;
   m_enableWidth = r.m_enableWidth;
   m_enableDistance = r.m_enableDistance;
   m_enableJitter = r.m_enableJitter;
   m_enableUp = r.m_enableUp;
   m_enableArcAngle = r.m_enableArcAngle;
   m_enableFalloffAngle = r.m_enableFalloffAngle;
}

PMRainbow::~PMRainbow()
{
}

PMMetaObject* PMRainbow::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Rainbow", Base::metaObject(),
                                        createNewRainbow );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "direction", &PMRainbow::setDirection, &PMRainbow::direction ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "angle", &PMRainbow::setAngle, &PMRainbow::angle ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "width", &PMRainbow::setWidth, &PMRainbow::width ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "distance", &PMRainbow::setDistance, &PMRainbow::distance ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "jitter", &PMRainbow::setJitter, &PMRainbow::jitter ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "up", &PMRainbow::setUp, &PMRainbow::up ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "arcAngle", &PMRainbow::setArcAngle, &PMRainbow::arcAngle ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "falloffAngle", &PMRainbow::setFalloffAngle, &PMRainbow::falloffAngle ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "directionEnabled", &PMRainbow::enableDirection, &PMRainbow::isDirectionEnabled ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "angleEnabled", &PMRainbow::enableAngle, &PMRainbow::isAngleEnabled ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "widthEnabled", &PMRainbow::enableWidth, &PMRainbow::isWidthEnabled ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "distanceEnabled", &PMRainbow::enableDistance, &PMRainbow::isDistanceEnabled ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "jitterEnabled", &PMRainbow::enableJitter, &PMRainbow::isJitterEnabled ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "upEnabled", &PMRainbow::enableUp, &PMRainbow::isUpEnabled ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "arcAngleEnabled", &PMRainbow::enableArcAngle, &PMRainbow::isArcAngleEnabled ) );
      s_pMetaObject->addProperty(
         new PMRainbowProperty( "falloffAngleEnabled", &PMRainbow::enableFalloffAngle, &PMRainbow::isFalloffAngleEnabled ) );
   }
   return s_pMetaObject;
}

void PMRainbow::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMRainbow::description() const
{
   return ( "rainbow" );
}

void PMRainbow::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
   e.setAttribute( "enable_direction", m_enableDirection );
   e.setAttribute( "enable_angle", m_enableAngle );
   e.setAttribute( "enable_width", m_enableWidth );
   e.setAttribute( "enable_distance", m_enableDistance );
   e.setAttribute( "enable_jitter", m_enableJitter );
   e.setAttribute( "enable_up", m_enableUp );
   e.setAttribute( "enable_arc_angle", m_enableArcAngle );
   e.setAttribute( "enable_falloff_angle", m_enableFalloffAngle );
   e.setAttribute( "direction", m_direction.serializeXML() );
   e.setAttribute( "angle", m_angle );
   e.setAttribute( "width", m_width );
   e.setAttribute( "distance", m_distance );
   e.setAttribute( "jitter", m_jitter );
   e.setAttribute( "up", m_up.serializeXML() );
   e.setAttribute( "arc_angle", m_arcAngle );
   e.setAttribute( "falloff_angle", m_falloffAngle );
}

void PMRainbow::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
   m_enableDirection = h.boolAttribute( "enable_direction", false );
   m_enableAngle = h.boolAttribute( "enable_angle", false );
   m_enableWidth = h.boolAttribute( "enable_width", false );
   m_enableDistance = h.boolAttribute( "enable_distance", false );
   m_enableJitter = h.boolAttribute( "enable_jitter", false );
   m_enableUp = h.boolAttribute( "enable_up", false );
   m_enableArcAngle = h.boolAttribute( "enable_arc_angle", false );
   m_enableFalloffAngle = h.boolAttribute( "enable_falloff_angle", false );
   m_direction = h.vectorAttribute( "direction", directionDefault );
   m_angle = h.doubleAttribute( "angle", angleDefault );
   m_width = h.doubleAttribute( "width", widthDefault );
   m_distance = h.doubleAttribute( "distance", distanceDefault );
   m_jitter = h.doubleAttribute( "jitter", jitterDefault );
   m_up = h.vectorAttribute( "up", upDefault );
   m_arcAngle = h.doubleAttribute( "arc_angle", arcAngleDefault );
   m_falloffAngle = h.doubleAttribute( "falloff_angle", falloffAngleDefault );
}

void PMRainbow::setDirection( const PMVector& c )
{
   if( c != m_direction )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDirectionID, m_direction );
      m_direction = c;
   }
}

void PMRainbow::setAngle( double c )
{
   if( c != m_angle )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAngleID, m_angle );
      m_angle = c;
   }
}

void PMRainbow::setWidth( double c )
{
   if( c != m_width )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMWidthID, m_width );
      m_width = c;
   }
}

void PMRainbow::setDistance( double c )
{
   if( c != m_distance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDistanceID, m_distance );
      m_distance = c;
   }
}

void PMRainbow::setJitter( double c )
{
   if( c != m_jitter )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMJitterID, m_jitter );
      m_jitter = c;
   }
}

void PMRainbow::setUp( const PMVector& c )
{
   if( c != m_up )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUpID, m_up );
      m_up = c;
   }
}

void PMRainbow::setArcAngle( double c )
{
   if( c != m_arcAngle )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMArcAngleID, m_arcAngle );
      m_arcAngle = c;
   }
}

void PMRainbow::setFalloffAngle( double c )
{
   if( c != m_falloffAngle )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFalloffAngleID, m_falloffAngle );
      m_falloffAngle = c;
   }
}

void PMRainbow::enableDirection( bool c )
{
   if( c != m_enableDirection )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableDirectionID, m_enableDirection );
      m_enableDirection = c;
   }
}

void PMRainbow::enableAngle( bool c )
{
   if( c != m_enableAngle )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableAngleID, m_enableAngle );
      m_enableAngle = c;
   }
}

void PMRainbow::enableWidth( bool c )
{
   if( c != m_enableWidth )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableWidthID, m_enableWidth );
      m_enableWidth = c;
   }
}

void PMRainbow::enableDistance( bool c )
{
   if( c != m_enableDistance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableDistanceID, m_enableDistance );
      m_enableDistance = c;
   }
}

void PMRainbow::enableJitter( bool c )
{
   if( c != m_enableJitter )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableJitterID, m_enableJitter );
      m_enableJitter = c;
   }
}

void PMRainbow::enableUp( bool c )
{
   if( c != m_enableUp )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableUpID, m_enableUp );
      m_enableUp = c;
   }
}

void PMRainbow::enableArcAngle( bool c )
{
   if( c != m_enableArcAngle )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableArcAngleID, m_enableArcAngle );
      m_enableArcAngle = c;
   }
}

void PMRainbow::enableFalloffAngle( bool c )
{
   if( c != m_enableFalloffAngle )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableFalloffAngleID,
                                                         m_enableFalloffAngle );
      m_enableFalloffAngle = c;
   }
}

PMDialogEditBase* PMRainbow::editWidget( QWidget* parent ) const
{
   return new PMRainbowEdit( parent );
}

void PMRainbow::restoreMemento( PMMemento* s )
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
            case PMDirectionID:
               setDirection( data->vectorData() );
               break;
            case PMAngleID:
               setAngle( data->doubleData() );
               break;
            case PMWidthID:
               setWidth( data->doubleData() );
               break;
           case PMDistanceID:
               setDistance( data->doubleData() );
               break;
           case PMJitterID:
               setJitter( data->doubleData() );
               break;
            case PMUpID:
               setUp( data->vectorData() );
               break;
            case PMArcAngleID:
               setArcAngle( data->doubleData() );
               break;
            case PMFalloffAngleID:
               setFalloffAngle( data->doubleData() );
               break;
            case PMEnableDirectionID:
               enableDirection( data->boolData() );
               break;
            case PMEnableAngleID:
               enableAngle( data->boolData() );
               break;
            case PMEnableWidthID:
               enableWidth( data->boolData() );
               break;
            case PMEnableDistanceID:
               enableDistance( data->boolData() );
               break;
            case PMEnableJitterID:
               enableJitter( data->boolData() );
               break;
            case PMEnableUpID:
               enableUp( data->boolData() );
               break;
            case PMEnableArcAngleID:
               enableArcAngle( data->boolData() );
               break;
            case PMEnableFalloffAngleID:
               enableFalloffAngle( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMRainbow::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
