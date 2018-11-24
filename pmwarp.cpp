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
#include "pmwarp.h"
#include "pmwarpedit.h"
#include "pmmemento.h"
#include "pmenumproperty.h"



const PMVector directionDefault = PMVector( 1.0, 0.0, 0.0 );
const PMVector offsetDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector flipDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector locationDefault = PMVector( 0.0, 0.0, 0.0 );
const double radiusDefault = 0;
const double strengthDefault = 0;
const double falloffDefault = 0;
const bool inverseDefault = false;
const PMVector repeatDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector turbulenceDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector valueVectorDefault = PMVector( 0.0, 0.0, 0.0 );
const int octavesDefault = 6;
const double omegaDefault = 0.5;
const double lambdaDefault = 2.0;
const PMVector orientationDefault = PMVector( 0.0, 0.0, 1.0 );
const double distExpDefault = 0.0;
const double majorRadiusDefault = 1.0;

PMDefinePropertyClass( PMWarp, PMWarpProperty );
PMDefineEnumPropertyClass( PMWarp, PMWarp::PMWarpType, PMWarpTypeProperty );

PMMetaObject* PMWarp::s_pMetaObject = 0;
PMObject* createNewWarp( PMPart* part )
{
   return new PMWarp( part );
}

PMWarp::PMWarp( PMPart* part )
      : Base( part )
{
   m_warpType = PMWarp::Repeat;
   m_direction = directionDefault;
   m_offset = offsetDefault;
   m_flip = flipDefault;
   m_location = locationDefault;
   m_radius = radiusDefault;
   m_strength = strengthDefault;
   m_falloff = falloffDefault;
   m_inverse = inverseDefault;
   m_repeat = repeatDefault;
   m_turbulence = turbulenceDefault;
   m_valueVector = valueVectorDefault;
   m_octaves = octavesDefault;
   m_omega = omegaDefault;
   m_lambda = lambdaDefault;
   m_orientation = orientationDefault;
   m_distExp = distExpDefault;
   m_majorRadius = majorRadiusDefault;
}

PMWarp::PMWarp( const PMWarp& w )
      : Base( w )
{
   m_warpType = w.m_warpType;
   m_direction = w.m_direction;
   m_offset = w.m_offset;
   m_flip = w.m_flip;
   m_location = w.m_location;
   m_radius = w.m_radius;
   m_strength = w.m_strength;
   m_falloff = w.m_falloff;
   m_inverse = w.m_inverse;
   m_repeat = w.m_repeat;
   m_turbulence = w.m_turbulence;
   m_valueVector = w.m_valueVector;
   m_octaves = w.m_octaves;
   m_omega = w.m_omega;
   m_lambda = w.m_lambda;
   m_orientation = w.m_orientation;
   m_distExp = w.m_distExp;
   m_majorRadius = w.m_majorRadius;
}

PMWarp::~PMWarp()
{
}

QString PMWarp::description() const
{
   return ( "warp" );
}

void PMWarp::serialize( QDomElement& e, QDomDocument& /*doc*/ ) const
{
   bool mapping = false;

   switch( m_warpType )
   {
      case PMWarp::Repeat:
         e.setAttribute( "warp_type", "repeat");
         e.setAttribute( "direction", m_direction.serializeXML() );
         e.setAttribute( "offset", m_offset.serializeXML() );
         e.setAttribute( "flip", m_flip.serializeXML() );
         break;
      case PMWarp::BlackHole:
         e.setAttribute( "warp_type", "black hole");
         e.setAttribute( "location", m_location.serializeXML() );
         e.setAttribute( "radius", m_radius );
         e.setAttribute( "strength", m_strength );
         e.setAttribute( "falloff", m_falloff );
         e.setAttribute( "inverse", m_inverse );
         e.setAttribute( "repeat", m_repeat.serializeXML() );
         e.setAttribute( "turbulence", m_turbulence.serializeXML() );
         break;
      case PMWarp::Turbulence:
         e.setAttribute( "warp_type", "turbulence");
         e.setAttribute( "turbulence", m_valueVector.serializeXML() );
         e.setAttribute( "octaves", m_octaves );
         e.setAttribute( "omega", m_omega );
         e.setAttribute( "lambda", m_lambda );
         break;
      case PMWarp::Cylindrical:
         mapping = true;
         e.setAttribute( "warp_type", "cylindrical" );
         break;
      case PMWarp::Spherical:
         mapping = true;
         e.setAttribute( "warp_type", "spherical" );
         break;
      case PMWarp::Toroidal:
         mapping = true;
         e.setAttribute( "warp_type", "toroidal" );
         e.setAttribute( "major_radius", m_majorRadius );
         break;
      case PMWarp::Planar:
         mapping = true;
         e.setAttribute( "warp_type", "planar" );
         break;
   }

   if ( mapping )
   {
      e.setAttribute( "orientation", m_orientation.serializeXML() );
      e.setAttribute( "dist_exp", m_distExp );
   }
}

void PMWarp::readAttributes( const PMXMLHelper& h )
{
   bool mapping = false;
   QString str = h.stringAttribute( "warp_type", "repeat" );

   if( str == "repeat" )
   {
      m_warpType = PMWarp::Repeat;
      m_direction = h.vectorAttribute( "direction", directionDefault );
      m_offset = h.vectorAttribute( "offset", offsetDefault );
      m_flip = h.vectorAttribute( "flip", flipDefault );
   }
   else if( str == "black hole" )
   {
      m_warpType = PMWarp::BlackHole;
      m_location = h.vectorAttribute( "location", locationDefault );
      m_radius = h.doubleAttribute( "radius", radiusDefault );
      m_strength = h.doubleAttribute( "strength", strengthDefault );
      m_falloff = h.doubleAttribute( "falloff", falloffDefault );
      m_inverse = h.boolAttribute( "inverse", inverseDefault );
      m_repeat = h.vectorAttribute( "repeat", repeatDefault );
      m_turbulence = h.vectorAttribute( "turbulence", turbulenceDefault );
   }
   else if( str == "turbulence" )
   {
      m_warpType = PMWarp::Turbulence;
      m_valueVector = h.vectorAttribute( "turbulence", valueVectorDefault );
      m_octaves = h.intAttribute( "octaves", octavesDefault );
      m_omega = h.doubleAttribute( "omega", omegaDefault );
      m_lambda = h.doubleAttribute( "lambda", lambdaDefault );
   }
   else if( str == "cylindrical" )
   {
      mapping = true;
      m_warpType = PMWarp::Cylindrical;
   }
   else if( str == "spherical" )
   {
      mapping = true;
      m_warpType = PMWarp::Spherical;
   }
   else if( str == "toroidal" )
   {
      mapping = true;
      m_warpType = PMWarp::Toroidal;
      m_majorRadius = h.doubleAttribute( "major_radius", majorRadiusDefault );
   }
   else if( str == "planar" )
   {
      mapping = true;
      m_warpType = PMWarp::Planar;
   }

   if( mapping )
   {
      m_orientation = h.vectorAttribute( "orientation", orientationDefault );
      m_distExp = h.doubleAttribute( "dist_exp", distExpDefault );
   }
}

PMMetaObject* PMWarp::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Warp", Base::metaObject(),
                                        createNewWarp );

      PMWarpTypeProperty* p = new PMWarpTypeProperty(
         "warpType", &PMWarp::setWarpType, &PMWarp::warpType );
      p->addEnumValue( "Repeat", Repeat );
      p->addEnumValue( "BlackHole", BlackHole );
      p->addEnumValue( "Turbulence", Turbulence );
      p->addEnumValue( "Cylindrical", Cylindrical );
      p->addEnumValue( "Spherical", Spherical );
      p->addEnumValue( "Toroidal", Toroidal );
      p->addEnumValue( "Planar", Planar );
      s_pMetaObject->addProperty( p );

      s_pMetaObject->addProperty(
         new PMWarpProperty( "direction", &PMWarp::setDirection, &PMWarp::direction ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "offset", &PMWarp::setOffset, &PMWarp::offset ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "flip", &PMWarp::setFlip, &PMWarp::flip ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "location", &PMWarp::setLocation, &PMWarp::location ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "radius", &PMWarp::setRadius, &PMWarp::radius ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "strength", &PMWarp::setStrength, &PMWarp::strength ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "falloff", &PMWarp::setFalloff, &PMWarp::falloff ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "inverse", &PMWarp::setInverse, &PMWarp::inverse ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "repeat", &PMWarp::setRepeat, &PMWarp::repeat ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "turbulence", &PMWarp::setTurbulence, &PMWarp::turbulence ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "valueVector", &PMWarp::setValueVector, &PMWarp::valueVector ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "octaves", &PMWarp::setOctaves, &PMWarp::octaves ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "omega", &PMWarp::setOmega, &PMWarp::omega ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "lambda", &PMWarp::setLambda, &PMWarp::lambda ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "orientation", &PMWarp::setOrientation, &PMWarp::orientation ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "dist_exp", &PMWarp::setDistExp, &PMWarp::distExp ) );
      s_pMetaObject->addProperty(
         new PMWarpProperty( "major_radius", &PMWarp::setMajorRadius, &PMWarp::majorRadius ) );
   }
   return s_pMetaObject;
}

void PMWarp::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMWarp::setWarpType( PMWarpType c )
{
   if( c != m_warpType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMWarpTypeID, m_warpType );
      m_warpType = c;
   }
}

void PMWarp::setDirection( const PMVector& c )
{
   if( c != m_direction )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDirectionID, m_direction );
      m_direction = c;
   }
}

void PMWarp::setOffset( const PMVector& c )
{
   if( c != m_offset )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOffsetID, m_offset );
      m_offset = c;
   }
}

void PMWarp::setFlip( const PMVector& c )
{
   if( c != m_flip )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFlipID, m_flip );
      m_flip = c;
   }
}

void PMWarp::setLocation( const PMVector& c )
{
   if( c != m_location )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLocationID, m_location );
      m_location = c;
   }
}

void PMWarp::setRadius( const double c )
{
   if( c != m_radius )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusID, m_radius );
      m_radius = c;
   }
}

void PMWarp::setStrength( const double c )
{
   if( c != m_strength )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMStrengthID, m_strength );
      m_strength = c;
   }
}

void PMWarp::setFalloff( const double c )
{
   if( c != m_falloff )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFalloffID, m_falloff );
      m_falloff = c;
   }
}

void PMWarp::setInverse( const bool c )
{
   if( c != m_inverse )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMInverseID, m_inverse );
      m_inverse = c;
   }
}

void PMWarp::setRepeat( const PMVector& c )
{
   if( c != m_repeat )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRepeatID, m_repeat );
      m_repeat = c;
   }
}

void PMWarp::setTurbulence( const PMVector& c )
{
   if( c != m_turbulence )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMTurbulenceID, m_turbulence );
      m_turbulence = c;
   }
}

void PMWarp::setValueVector( const PMVector& c )
{
   if( c != m_valueVector )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMValueVectorID, m_valueVector );
      m_valueVector = c;
   }
}

void PMWarp::setOctaves( const int c )
{
   if( c != m_octaves )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOctavesID, m_octaves );
      m_octaves = c;
   }
}

void PMWarp::setOmega( const double c )
{
   if( c != m_omega )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOmegaID, m_omega );
      m_omega = c;
   }
}

void PMWarp::setLambda( const double c )
{
   if( c != m_lambda )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLambdaID, m_lambda );
      m_lambda = c;
   }
}

void PMWarp::setOrientation( const PMVector& v )
{
   if ( v != m_orientation )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOrientationID, m_orientation );
      m_orientation = v;
   }
}

void PMWarp::setDistExp( const double c )
{
   if ( c != m_distExp )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDistExpID, m_distExp );
      m_distExp = c;
   }
}

void PMWarp::setMajorRadius( const double c )
{
   if ( c != m_majorRadius )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMajorRadiusID, m_majorRadius );
      m_majorRadius = c;
   }
}

PMDialogEditBase* PMWarp::editWidget( QWidget* parent ) const
{
   return new PMWarpEdit( parent );
}

void PMWarp::restoreMemento( PMMemento* s )
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
            case PMWarpTypeID:
               setWarpType( ( PMWarpType )data->intData() );
               break;
            case PMDirectionID:
               setDirection( data->vectorData() );
               break;
            case PMOffsetID:
               setOffset( data->vectorData() );
               break;
            case PMFlipID:
               setFlip( data->vectorData() );
               break;
            case PMLocationID:
               setLocation( data->vectorData() );
               break;
            case PMRadiusID:
               setRadius( data->doubleData() );
               break;
            case PMStrengthID:
               setStrength( data->doubleData() );
               break;
            case PMFalloffID:
               setFalloff( data->doubleData() );
               break;
            case PMInverseID:
               setInverse( data->boolData() );
               break;
            case PMRepeatID:
               setRepeat( data->vectorData() );
               break;
            case PMTurbulenceID:
               setTurbulence( data->vectorData() );
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
            case PMOrientationID:
               setOrientation( data->vectorData() );
               break;
            case PMDistExpID:
               setDistExp( data->doubleData() );
               break;
            case PMMajorRadiusID:
               setMajorRadius( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMWarp::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
