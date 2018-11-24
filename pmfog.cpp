/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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

#include "pmfog.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmfogedit.h"
#include "pmvector.h"



const int fogTypeDefault = 1;
const double distanceDefault = 0.0;
const PMColor colorDefault = PMColor( 0.0, 0.0, 0.0 );
const double lambdaDefault = 2.0;
const double fogOffsetDefault = 0.0;
const double fogAltDefault = 0.0;
const PMVector turbulenceDefault = PMVector( 0.0, 0.0, 0.0 );
const int octavesDefault = 6;
const double omegaDefault = 0.5;
const double turbDepthDefault = 0.5;
const PMVector upDefault = PMVector( 0.0, 1.0, 0.0 );

PMDefinePropertyClass( PMFog, PMFogProperty );

PMMetaObject* PMFog::s_pMetaObject = 0;
PMObject* createNewFog( PMPart* part )
{
   return new PMFog( part );
}

PMFog::PMFog( PMPart* part )
      : Base( part )
{
   m_fogType = fogTypeDefault;
   m_distance = distanceDefault;
   m_color = colorDefault;
   m_enableTurbulence = false;
   m_valueVector = turbulenceDefault;
   m_octaves = octavesDefault;
   m_omega = omegaDefault;
   m_depth = turbDepthDefault;
   m_fogOffset = fogOffsetDefault;
   m_fogAlt = fogAltDefault;
   m_up = upDefault;
   m_lambda = 0;
}

PMFog::PMFog( const PMFog& f )
      : Base( f )
{
   m_fogType = f.m_fogType;
   m_distance = f.m_distance;
   m_color = f.m_color;
   m_enableTurbulence = f.m_enableTurbulence;
   m_valueVector = f.m_valueVector;
   m_octaves = f.m_octaves;
   m_omega = f.m_omega;
   m_depth = f.m_depth;
   m_fogOffset = f.m_fogOffset;
   m_fogAlt = f.m_fogAlt;
   m_up = f.m_up;
   m_lambda = f.m_lambda;
}

PMFog::~PMFog()
{
}

PMMetaObject* PMFog::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Fog", Base::metaObject(),
                                        createNewFog );
      s_pMetaObject->addProperty(
         new PMFogProperty( "fogType", &PMFog::setFogType, &PMFog::fogType ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "distance", &PMFog::setDistance, &PMFog::distance ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "color", &PMFog::setColor, &PMFog::color ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "turbulenceEnabled", &PMFog::enableTurbulence, &PMFog::isTurbulenceEnabled ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "turbulence", &PMFog::setValueVector, &PMFog::valueVector ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "octaves", &PMFog::setOctaves, &PMFog::octaves ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "omega", &PMFog::setOmega, &PMFog::omega ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "lambda", &PMFog::setLambda, &PMFog::lambda ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "depth", &PMFog::setDepth, &PMFog::depth ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "offset", &PMFog::setFogOffset, &PMFog::fogOffset ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "altitude", &PMFog::setFogAlt, &PMFog::fogAlt ) );
      s_pMetaObject->addProperty(
         new PMFogProperty( "up", &PMFog::setUp, &PMFog::up ) );
   }
   return s_pMetaObject;
}

void PMFog::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMFog::description() const
{
   return ( "fog" );
}

void PMFog::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
   e.setAttribute( "fog_type", m_fogType );
   e.setAttribute( "distance", m_distance );
   e.setAttribute( "color", m_color.serializeXML() );
   e.setAttribute( "enable_turbulence", m_enableTurbulence );
   e.setAttribute( "value_vector", m_valueVector.serializeXML() );
   e.setAttribute( "octaves", m_octaves );
   e.setAttribute( "omega", m_omega );
   e.setAttribute( "lambda", m_lambda );
   e.setAttribute( "depth", m_depth );
   e.setAttribute( "fog_offset", m_fogOffset );
   e.setAttribute( "fog_alt", m_fogAlt );
   e.setAttribute( "up", m_up.serializeXML() );
}

void PMFog::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
   m_fogType = h.intAttribute( "fog_type", fogTypeDefault );
   m_distance = h.doubleAttribute( "distance", distanceDefault );
   m_color = h.colorAttribute( "color", colorDefault );
   m_enableTurbulence = h.boolAttribute( "enable_turbulence", false );
   m_valueVector = h.vectorAttribute( "value_vector", turbulenceDefault );
   m_octaves = h.intAttribute( "octaves", octavesDefault );
   m_omega = h.doubleAttribute( "omega", omegaDefault );
   m_lambda = h.doubleAttribute( "lambda", lambdaDefault );
   m_depth = h.doubleAttribute( "depth", turbDepthDefault );
   m_fogOffset = h.doubleAttribute( "fog_offset", fogOffsetDefault );
   m_fogAlt = h.doubleAttribute( "fog_alt", fogAltDefault );
   m_up = h.vectorAttribute( "up", upDefault );
}

void PMFog::setFogType( int c )
{
   if( c != m_fogType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFogTypeID, m_fogType );
      m_fogType = c;
   }
}

void PMFog::setDistance( double c )
{
   if( c != m_distance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDistanceID, m_distance );
      m_distance = c;
   }
}

void PMFog::setColor( const PMColor& c )
{
   if( c != m_color )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMColorID, m_color );
      m_color = c;
   }
}

void PMFog::enableTurbulence( bool c )
{
   if( c != m_enableTurbulence )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableTurbulenceID, m_enableTurbulence );
      m_enableTurbulence = c;
   }
}

void PMFog::setValueVector( const PMVector& c )
{
   if( c != m_valueVector )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMValueVectorID, m_valueVector );
      m_valueVector = c;
   }
}

void PMFog::setOctaves( int c )
{
   if( c != m_octaves )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOctavesID, m_octaves );
      m_octaves = c;
   }
}

void PMFog::setOmega( double c )
{
   if( c != m_omega )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOmegaID, m_omega );
      m_omega = c;
   }
}

void PMFog::setLambda( double c )
{
   if( c != m_lambda )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLambdaID, m_lambda );
      m_lambda = c;
   }
}

void PMFog::setDepth( double c )
{
   if( c != m_depth )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDepthID, m_depth );
      m_depth = c;
   }
}

void PMFog::setFogOffset( double c )
{
   if( c != m_fogOffset )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFogOffsetID, m_fogOffset );
      m_fogOffset = c;
   }
}

void PMFog::setFogAlt( double c )
{
   if( c != m_fogAlt )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFogAltID, m_fogAlt );
      m_fogAlt = c;
   }
}

void PMFog::setUp( const PMVector& c )
{
   if( c != m_up )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUpID, m_up );
      m_up = c;
   }
}

PMDialogEditBase* PMFog::editWidget( QWidget* parent ) const
{
   return new PMFogEdit( parent );
}

void PMFog::restoreMemento( PMMemento* s )
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
            case PMFogTypeID:
               setFogType( data->intData() );
               break;
            case PMDistanceID:
               setDistance( data->doubleData() );
               break;
            case PMColorID:
               setColor( data->colorData() );
               break;
            case PMEnableTurbulenceID:
               enableTurbulence( data->boolData() );
               break;
           case PMValueVectorID:
               setValueVector( data->vectorData() );
               break;
           case PMOctavesID:
               setOctaves( data->intData() );
               break;
            case PMOmegaID:
               setOmega( data->doubleData() );
               break;
            case PMLambdaID:
               setLambda( data->doubleData() );
               break;
            case PMDepthID:
               setDepth( data->doubleData() );
               break;
            case PMFogOffsetID:
               setFogOffset( data->doubleData() );
               break;
            case PMFogAltID:
               setFogAlt( data->doubleData() );
               break;
            case PMUpID:
               setUp( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMFog::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
