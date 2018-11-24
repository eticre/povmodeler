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


#include "pmlight.h"

#include "pmxmlhelper.h"
#include "pmlightedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"
#include "pmmath.h"
#include "pmmatrix.h"
#include "pmenumproperty.h"



const PMVector locationDefault = PMVector( 0, 0, 0 );
const PMColor colorDefault = PMColor( 1.0, 1.0, 1.0 );
const double radiusDefault = 70.0;
const double falloffDefault = 70.0;
const double tightnessDefault = 10;
const PMVector pointAtDefault = PMVector( 0, 0, 1 );
const bool parallelDefault = false;
const PMVector areaAxis1Default = PMVector( 1, 0, 0 );
const PMVector areaAxis2Default = PMVector( 0, 1, 0 );
const int areaSize1Default = 3;
const int areaSize2Default = 3;
const int adaptiveDefault = 0;
const bool orientDefault = false;
const bool jitterDefault = false;
const int fadePowerDefault = 1;
const double fadeDistanceDefault = 10.0;

PMViewStructure* PMLight::s_pDefaultPointStructure = 0;
PMViewStructure* PMLight::s_pDefaultSpotStructure = 0;
PMViewStructure* PMLight::s_pDefaultCylindricalStructure = 0;
double PMLight::s_pointLightSize = 0.25;
int PMLight::s_nCylinderLines = 8;
int PMLight::s_nSpotLines = 8;
double PMLight::s_length = 1.0;

PMDefinePropertyClass( PMLight, PMLightProperty );
PMDefineEnumPropertyClass( PMLight, PMLight::PMLightType, PMTypeProperty );
PMDefineEnumPropertyClass( PMLight, PMLight::PMAreaType, PMAreaProperty );

PMMetaObject* PMLight::s_pMetaObject = 0;
PMObject* createNewLight( PMPart* part )
{
   return new PMLight( part );
}

PMLight::PMLight( PMPart* part )
      : Base( part )
{
   m_location = locationDefault;
   m_color = colorDefault;
   m_type = PointLight;
   m_radius = radiusDefault;
   m_falloff = falloffDefault;
   m_tightness = tightnessDefault;
   m_pointAt = pointAtDefault;
   m_parallel = parallelDefault;
   m_bAreaLight = false;
   m_areaType = Rectangular;
   m_areaAxis1 = areaAxis1Default;
   m_areaAxis2 = areaAxis2Default;
   m_areaSize1 = areaSize1Default;
   m_areaSize2 = areaSize2Default;
   m_adaptive = adaptiveDefault;
   m_orient = orientDefault;
   m_jitter = jitterDefault;
   m_bFading = false;
   m_fadeDistance = fadeDistanceDefault;
   m_fadePower = fadePowerDefault;
   m_bMediaInteraction = true;
   m_bMediaAttenuation = true;
}

PMLight::PMLight( const PMLight& l )
      : Base( l )
{
   m_location = l.m_location;
   m_color = l.m_color;
   m_type = l.m_type;
   m_radius = l.m_radius;
   m_falloff = l.m_falloff;
   m_tightness = l.m_tightness;
   m_pointAt = l.m_pointAt;
   m_parallel = l.m_parallel;
   m_bAreaLight = l.m_bAreaLight;
   m_areaType = l.m_areaType;
   m_areaAxis1 = l.m_areaAxis1;
   m_areaAxis2 = l.m_areaAxis2;
   m_areaSize1 = l.m_areaSize1;
   m_areaSize2 = l.m_areaSize2;
   m_adaptive = l.m_adaptive;
   m_orient = l.m_orient;
   m_jitter = l.m_jitter;
   m_bFading = l.m_bFading;
   m_fadeDistance = l.m_fadeDistance;
   m_fadePower = l.m_fadePower;
   m_bMediaInteraction = l.m_bMediaInteraction;
   m_bMediaAttenuation = l.m_bMediaAttenuation;
}

PMLight::~PMLight()
{
}

QString PMLight::description() const
{
   return ( "light" );
}

void PMLight::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "location", m_location.serializeXML() );

   if( m_color.getSrgb() )
       e.setAttribute( "colorsrgb", m_color.serializeXML() );
   else
       e.setAttribute( "color", m_color.serializeXML() );

   switch( m_type )
   {
      case SpotLight:
         e.setAttribute( "lighttype", "spotlight" );
         break;
      case CylinderLight:
         e.setAttribute( "lighttype", "cylinder" );
         break;
      case ShadowlessLight:
         e.setAttribute( "lighttype", "shadowless" );
         break;
      case PointLight:
         e.setAttribute( "lighttype", "point" );
         break;
   }

   if( ( m_type == SpotLight ) || ( m_type == CylinderLight ) )
   {
      e.setAttribute( "radius", m_radius );
      e.setAttribute( "falloff", m_falloff );
      e.setAttribute( "tightness", m_tightness );
      e.setAttribute( "point_at", m_pointAt.serializeXML() );
   }

   if ( m_parallel )
      e.setAttribute( "parallel", "1" );
   else
      e.setAttribute( "parallel", "0" );

   if( m_bAreaLight )
   {
      if ( m_areaType == Rectangular )
         e.setAttribute( "areatype", "rectangular" );
      else
         e.setAttribute( "areatype", "circular" );

      e.setAttribute( "area_light", "1" );
      e.setAttribute( "area_light_a", m_areaAxis1.serializeXML() );
      e.setAttribute( "area_light_b", m_areaAxis2.serializeXML() );
      e.setAttribute( "area_size_a", m_areaSize1 );
      e.setAttribute( "area_size_b", m_areaSize2 );
      e.setAttribute( "adaptive", m_adaptive );

      if( m_orient )
         e.setAttribute( "orient", "1" );
      else
         e.setAttribute( "orient", "0" );

      if( m_jitter )
         e.setAttribute( "jitter", "1" );
      else
         e.setAttribute( "jitter", "0" );
   }
   else
      e.setAttribute( "area_light", "0" );

   if( m_bFading )
   {
      e.setAttribute( "fading", "1" );
      e.setAttribute( "fade_distance" , m_fadeDistance );
      e.setAttribute( "fade_power", m_fadePower );
   }
   else
      e.setAttribute( "fading", "0" );

   if( m_bMediaInteraction )
      e.setAttribute( "media_interaction", "1" );
   else
      e.setAttribute( "media_interaction", "0" );

   if( m_bMediaAttenuation )
      e.setAttribute( "media_attenuation", "1" );
   else
      e.setAttribute( "media_attenuation", "0" );

   Base::serialize( e, doc );
}

void PMLight::readAttributes( const PMXMLHelper& h )
{
   QString str;

   m_location = h.vectorAttribute( "location", locationDefault );
   m_color = h.colorAttribute( "color", colorDefault );

   str = h.stringAttribute( "lighttype", "point" );
   if( str == "point" )
      m_type = PointLight;
   else if( str == "spotlight" )
      m_type = SpotLight;
   else if( str == "cylinder" )
      m_type = CylinderLight;
   else if( str == "shadowless" )
      m_type = ShadowlessLight;
   else
      m_type = PointLight;

   if( ( m_type == SpotLight ) || ( m_type == CylinderLight ) )
   {
      m_radius = h.doubleAttribute( "radius", radiusDefault );
      m_falloff = h.doubleAttribute( "falloff", falloffDefault );
      m_tightness = h.doubleAttribute( "tightness", tightnessDefault );
      m_pointAt = h.vectorAttribute( "point_at", pointAtDefault );
   }

   m_parallel = h.boolAttribute( "parallel", parallelDefault );

   m_bAreaLight = h.boolAttribute( "area_light", false );
   if( m_bAreaLight )
   {
      str = h.stringAttribute( "areatype", "rectangular" );
      if ( str == "circular" )
         m_areaType = Circular;
      else
         m_areaType = Rectangular;

      m_areaAxis1 = h.vectorAttribute( "area_light_a", areaAxis1Default );
      m_areaAxis2 = h.vectorAttribute( "area_light_b", areaAxis2Default );
      m_areaSize1 = h.intAttribute( "area_size_a", areaSize1Default );
      m_areaSize2 = h.intAttribute( "area_size_b", areaSize2Default );
      m_adaptive = h.intAttribute( "adaptive", adaptiveDefault );
      m_orient = h.boolAttribute( "orient", orientDefault );
      m_jitter = h.boolAttribute( "jitter", jitterDefault );
   }
   m_bFading = h.boolAttribute( "fading", false );
   if( m_bFading )
   {
      m_fadeDistance = h.doubleAttribute( "fade_distance", fadeDistanceDefault );
      m_fadePower = h.intAttribute( "fade_power", m_fadePower );
   }
   m_bMediaInteraction = h.boolAttribute( "media_interaction", true );
   m_bMediaAttenuation = h.boolAttribute( "media_attenuation", true );

   Base::readAttributes( h );
}

PMMetaObject* PMLight::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Light", Base::metaObject(),
                                        createNewLight );
      PMTypeProperty* p = new PMTypeProperty( "lightType", &PMLight::setLightType,
                                              &PMLight::lightType );
      p->addEnumValue( "PointLight", PointLight );
      p->addEnumValue( "SpotLight", SpotLight );
      p->addEnumValue( "CylinderLight", CylinderLight );
      p->addEnumValue( "ShadowlessLight", ShadowlessLight );
      s_pMetaObject->addProperty( p );

      s_pMetaObject->addProperty(
         new PMLightProperty( "location", &PMLight::setLocation, &PMLight::location ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "color", &PMLight::setColor, &PMLight::color ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "radius", &PMLight::setRadius, &PMLight::radius ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "falloff", &PMLight::setFalloff, &PMLight::falloff ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "tightness", &PMLight::setTightness, &PMLight::tightness ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "pointAt", &PMLight::setPointAt, &PMLight::pointAt ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "parallel", &PMLight::setParallel, &PMLight::parallel ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "areaLight", &PMLight::setAreaLight, &PMLight::isAreaLight ) );

      PMAreaProperty* p2 = new PMAreaProperty( "areaType", &PMLight::setAreaType,
                                               &PMLight::areaType );
      p2->addEnumValue( "Rectangular", Rectangular );
      p2->addEnumValue( "Circular", Circular );
      s_pMetaObject->addProperty( p2 );

      s_pMetaObject->addProperty(
         new PMLightProperty( "axis1", &PMLight::setAxis1, &PMLight::axis1 ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "axis2", &PMLight::setAxis2, &PMLight::axis2 ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "adaptive", &PMLight::setAdaptive, &PMLight::adaptive ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "orient", &PMLight::setOrient, &PMLight::orient ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "jitter", &PMLight::setJitter, &PMLight::jitter ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "fading", &PMLight::setFading, &PMLight::fading ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "fadeDistance", &PMLight::setFadeDistance, &PMLight::fadeDistance ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "fadePower", &PMLight::setFadePower, &PMLight::fadePower ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "mediaInteraction", &PMLight::setMediaInteraction,
                         &PMLight::mediaInteraction ) );
      s_pMetaObject->addProperty(
         new PMLightProperty( "mediaAttenuation", &PMLight::setMediaAttenuation,
                         &PMLight::mediaAttenuation ) );
   }
   return s_pMetaObject;
}

void PMLight::setLocation( const PMVector& p )
{
   if( p != m_location )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLocationID, m_location );
      m_location = p;
      m_location.resize( 3 );
      setViewStructureChanged();
   }
}

void PMLight::setColor( const PMColor& c )
{
   if( c != m_color )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMColorID, m_color );
      m_color = c;
   }
}

void PMLight::setLightType( PMLightType t )
{
   if( t != m_type )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMTypeID, m_type );
      m_type = t;
      setViewStructureChanged();
   }
}

void PMLight::setRadius( double r )
{
   if( !approx( r, m_radius ) )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusID, m_radius );
      m_radius = r;
      setViewStructureChanged();
   }
}

void PMLight::setFalloff( double f )
{
   if( !approx( f, m_falloff ) )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFalloffID, m_falloff );
      m_falloff = f;
      setViewStructureChanged();
   }
}

void PMLight::setTightness( double t )
{
   if( !approx( t, m_tightness ) )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMTightnessID, m_tightness );
      m_tightness = t;
   }
}

void PMLight::setPointAt( const PMVector& v )
{
   if( !m_pointAt.approxEqual( v ) )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPointAtID, m_pointAt );
      m_pointAt = v;
      setViewStructureChanged();
   }
}

void PMLight::setParallel( bool p )
{
   if ( p != m_parallel )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMParallelID, m_parallel );
      m_parallel = p;
   }
}

void PMLight::setAreaLight( bool yes )
{
   if( yes != m_bAreaLight )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAreaLightID, m_bAreaLight );
      m_bAreaLight = yes;
      setViewStructureChanged();
   }
}

void PMLight::setAreaType( PMAreaType at )
{
   if ( at != m_areaType )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAreaTypeID, m_areaType );
      m_areaType = at;
      setViewStructureChanged();
   }
}

void PMLight::setAxis1( const PMVector& v )
{
   if( !m_areaAxis1.approxEqual( v ) )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAreaAxis1ID, m_areaAxis1 );
      m_areaAxis1 = v;
      setViewStructureChanged();
   }
}

void PMLight::setAxis2( const PMVector& v )
{
   if( !m_areaAxis2.approxEqual( v ) )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAreaAxis2ID, m_areaAxis2 );
      m_areaAxis2 = v;
      setViewStructureChanged();
   }
}

void PMLight::setSize1( int s )
{
   if( s != m_areaSize1 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAreaSize1ID, m_areaSize1 );
      m_areaSize1 = s;
      setViewStructureChanged();
   }
}

void PMLight::setSize2( int s )
{
   if( s != m_areaSize2 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAreaSize2ID, m_areaSize2 );
      m_areaSize2 = s;
      setViewStructureChanged();
   }
}

void PMLight::setAdaptive( int a )
{
   if( a != m_adaptive )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAdaptiveID, m_adaptive );
      m_adaptive = a;
   }
}

void PMLight::setOrient( bool o )
{
   if( o != m_orient )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOrientID, m_orient );
      m_orient = o;
      setViewStructureChanged();
   }
}

void PMLight::setJitter( bool j )
{
   if( j != m_jitter )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMJitterID, m_jitter );
      m_jitter = j;
   }
}

void PMLight::setFading( bool y )
{
   if( y != m_bFading )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFadingID, m_bFading );
      m_bFading = y;
   }
}

void PMLight::setFadeDistance( double d )
{
   if( !approx( d, m_fadeDistance ) )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFadeDistanceID, m_fadeDistance );
      m_fadeDistance = d;
   }
}

void PMLight::setFadePower( int p )
{
   if( p != m_fadePower )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFadePowerID, m_fadePower );
      m_fadePower = p;
   }
}

void PMLight::setMediaInteraction( bool y )
{
   if( y != m_bMediaInteraction )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMInteractionID, m_bMediaInteraction );
      m_bMediaInteraction = y;
   }
}

void PMLight::setMediaAttenuation( bool y )
{
   if( y != m_bMediaAttenuation )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAttenuationID, m_bMediaAttenuation );
      m_bMediaAttenuation = y;
   }
}

PMDialogEditBase* PMLight::editWidget( QWidget* parent ) const
{
   return new PMLightEdit( parent );
}

void PMLight::restoreMemento( PMMemento* s )
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
            case PMLocationID:
               setLocation( data->vectorData() );
               break;
            case PMColorID:
               setColor( data->colorData() );
               break;
            case PMTypeID:
               setLightType( ( PMLightType ) ( data->intData() ) );
               break;
            case PMRadiusID:
               setRadius( data->doubleData() );
               break;
            case PMFalloffID:
               setFalloff( data->doubleData() );
               break;
            case PMTightnessID:
               setTightness( data->doubleData() );
               break;
            case PMPointAtID:
               setPointAt( data->vectorData() );
               break;
            case PMParallelID:
               setParallel( data->boolData() );
               break;
            case PMAreaLightID:
               setAreaLight( data->boolData() );
               break;
            case PMAreaTypeID:
               setAreaType( ( PMAreaType ) ( data->intData() ) );
               break;
            case PMAreaAxis1ID:
               setAxis1( data->vectorData() );
               break;
            case PMAreaAxis2ID:
               setAxis2( data->vectorData() );
               break;
            case PMAreaSize1ID:
               setSize1( data->intData() );
               break;
            case PMAreaSize2ID:
               setSize2( data->intData() );
               break;
            case PMAdaptiveID:
               setAdaptive( data->intData() );
               break;
            case PMOrientID:
               setOrient( data->boolData() );
               break;
            case PMJitterID:
               setJitter( data->boolData() );
               break;
            case PMFadingID:
               setFading( data->boolData() );
               break;
            case PMFadeDistanceID:
               setFadeDistance( data->doubleData() );
               break;
            case PMFadePowerID:
               setFadePower( data->intData() );
               break;
            case PMInteractionID:
               setMediaInteraction( data->boolData() );
               break;
            case PMAttenuationID:
               setMediaAttenuation( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMLight::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMLight::createViewStructure()
{
   if( ( m_type == PointLight ) || ( m_type == ShadowlessLight ) )
   {
      if( !m_pViewStructure )
      {
         m_pViewStructure = new PMViewStructure( defaultPointStructure() );
         m_pViewStructure->points().detach();
      }
      else
      {
         m_pViewStructure->points().resize(
            defaultPointStructure()->points().size() );
         m_pViewStructure->lines() = defaultPointStructure()->lines();
      }

      PMPointArray& points = m_pViewStructure->points();

      int i;
      double c = s_pointLightSize / sqrt( 3.0 );
      for( i = 0; i < 14; i++ )
         points[i] = PMPoint( m_location );

      points[0][0] += s_pointLightSize;
      points[1][0] -= s_pointLightSize;
      points[2][1] += s_pointLightSize;
      points[3][1] -= s_pointLightSize;
      points[4][2] += s_pointLightSize;
      points[5][2] -= s_pointLightSize;

      for( i = 0; i < 4; i++ )
      {
         points[6+2*i][0] += c;
         points[6+2*i][1] += ( i & 1 ? c : -c );
         points[6+2*i][2] += ( i & 2 ? c : -c );
         points[7+2*i][0] -= c;
         points[7+2*i][1] -= ( i & 1 ? c : -c );
         points[7+2*i][2] -= ( i & 2 ? c : -c );
      }
   }
   else if( m_type == SpotLight )
   {
      if( !m_pViewStructure )
      {
         m_pViewStructure = new PMViewStructure( defaultSpotStructure() );
         m_pViewStructure->points().detach();
      }
      else
      {
         m_pViewStructure->points().resize(
            defaultSpotStructure()->points().size() );
         m_pViewStructure->lines() = defaultSpotStructure()->lines();
      }

      PMPointArray& points = m_pViewStructure->points();

      points[0] = PMPoint( m_location );

      PMVector pointAtVector = m_pointAt - m_location;
      double pl = pointAtVector.abs();
      if( approxZero( pl ) )
         pointAtVector = PMVector( 0.0, 0.0, 1.0 );
      else
         pointAtVector /= pl;
      PMVector endPoint = pointAtVector.orthogonal();
      PMMatrix rotation = PMMatrix::rotation( pointAtVector,
                                              2 * M_PI / s_nSpotLines );
      double length, r1, r2, a1, a2;
      length = s_length;
      a1 = m_radius;
      a2 = m_falloff;
      if( a1 < 0 ) a1 = 0;
      if( a2 < 0 ) a2 = 0;
      if( a1 > a2 ) a1 = a2;
      if( a1 >= 89.9 ) a1 = 89.9;
      if( a2 >= 89.9 ) a2 = 89.9;
      a1 *= M_PI / 180;
      a2 *= M_PI / 180;
      r1 = tan( a1 ) * length;
      r2 = tan( a2 ) * length;

      if( r2 > length )
      {
         double d = length / r2;
         r1 *= d;
         r2 *= d;
         length *= d;
      }

      endPoint *= r2;
      double r;
      if( approxZero( r2 ) )
         r = 1;
      else
         r = r1 / r2;

      PMVector circleCenter = m_location + length * pointAtVector;
      points[1] = PMPoint( circleCenter + endPoint );
      points[s_nSpotLines + 1] = PMPoint( circleCenter + endPoint * r );

      int i;
      for( i = 2; i < ( s_nSpotLines + 1 ); i++ )
      {
         endPoint = rotation * endPoint;
         points[i] = PMPoint( circleCenter + endPoint );
         points[s_nSpotLines + i] = PMPoint( circleCenter + endPoint * r );
      }
      points[s_nSpotLines*2+1] = m_pointAt;
   }
   else if( m_type == CylinderLight )
   {
      if( !m_pViewStructure )
      {
         m_pViewStructure = new PMViewStructure( defaultCylindricalStructure() );
         m_pViewStructure->points().detach();
      }
      else
      {
         m_pViewStructure->points().resize(
            defaultCylindricalStructure()->points().size() );
         m_pViewStructure->lines() = defaultCylindricalStructure()->lines();
      }

      PMPointArray& points = m_pViewStructure->points();

      points[s_nCylinderLines*4] = PMPoint( m_location );
      points[s_nCylinderLines*4+1] = PMPoint( m_pointAt );

      PMVector pointAtVector = m_pointAt - m_location;
      double pl = pointAtVector.abs();
      if( approxZero( pl ) )
         pointAtVector = PMVector( 0.0, 0.0, 1.0 );
      else
         pointAtVector /= pl;
      PMVector endPoint = pointAtVector.orthogonal();
      PMMatrix rotation = PMMatrix::rotation( pointAtVector,
                                              2 * M_PI / s_nCylinderLines );
      double r1, r2;
      r1 = m_radius / 100;
      r2 = m_falloff / 100;
      if( r1 < 0 ) r1 = 0;
      if( r2 < 0 ) r2 = 0;
      if( r1 > r2 ) r1 = r2;

      endPoint *= r2;
      double r;
      if( approxZero( r2 ) )
         r = 1;
      else
         r = r1 / r2;

      PMVector circleCenter = m_location + s_length * pointAtVector;
      points[0] = PMPoint( circleCenter + endPoint );
      points[s_nCylinderLines] = PMPoint( m_location + endPoint );
      points[2*s_nCylinderLines] = PMPoint( circleCenter + endPoint * r );
      points[3*s_nCylinderLines] = PMPoint( m_location + endPoint * r );

      int i;
      for( i = 1; i < s_nCylinderLines; i++ )
      {
         endPoint = rotation * endPoint;
         points[i] = PMPoint( circleCenter + endPoint );
         points[s_nCylinderLines + i] = PMPoint( m_location + endPoint );
         points[2*s_nCylinderLines + i] = PMPoint( circleCenter + endPoint * r );
         points[3*s_nCylinderLines + i] = PMPoint( m_location + endPoint * r );
      }
   }

   if( m_bAreaLight )
   {
      int s1, s2;

      s1 = m_areaSize1;
      s2 = m_areaSize2;
      if( s1 < 1 ) s1 = 1;
      if( s2 < 1 ) s2 = 1;

      if( ( s1 > 1 ) || ( s2 > 1 ) )
      {
         int x, y, h;
         int ps, ls;
         PMVector bp;

         PMPointArray& points = m_pViewStructure->points();
         PMLineArray& lines = m_pViewStructure->lines();
         points.detach();
         lines.detach();

         ps = points.size();
         ls = lines.size();
         points.resize( ps + s1*s2 );
         lines.resize( ls + s1*(s2-1) + s2*(s1-1) );

         if( s1 == 1 )
         {
            bp = m_location - m_areaAxis2/2;
            for( y = 0; y < s2; y++ )
               points[ps + y] = PMPoint( bp + m_areaAxis2
                                         * ( (double)y/(double)(s2-1) ) );
            for( y = 0; y < ( s2-1 ); y++ )
               lines[ls+y] = PMLine( ps + y, ps + y+1 );
         }
         else if( s2 == 1 )
         {
            bp = m_location - m_areaAxis1/2;
            for( x = 0; x < s1; x++ )
               points[ps + x] = PMPoint( bp + m_areaAxis1
                                         * ( (double)x/(double)(s1-1) ) );
            for( x = 0; x < ( s1-1 ); x++ )
               lines[ls+x] = PMLine( ps + x, ps + x+1 );
         }
         else
         {
            bp = m_location - m_areaAxis1/2 - m_areaAxis2/2;

            if ( m_areaType == Rectangular || s1 < 2 || s2 < 2 )
            {
               for( x = 0; x < s1; x++ )
                  for( y = 0; y < s2; y++ )
                     points[ps + y*s1 + x] =
                        PMPoint( bp + m_areaAxis1 * ( (double)x/(double)(s1-1) )
                                 + m_areaAxis2 * ( (double)y/(double)(s2-1) ) );
            }
            else
            {
               double stepX = ( 2.0 / (double)(s1-1) );
               double stepY = ( 2.0 / (double)(s2-1) );
               double doubleX, doubleY, xSqr, scaleFactor;

               for ( x = 0; x < s1; ++x )
               {
                  doubleX = ( (double)x * stepX ) - 1.0;
                  xSqr = doubleX * doubleX;
                  for ( y = 0; y < s2; ++y )
                  {
                     doubleY = ( (double)y * stepY ) - 1.0;

                     if ( doubleX == 0.0 && doubleY == 0.0 )
                        scaleFactor = 1.0;
                     else
                     {
                        if ( fabs( doubleX ) > fabs( doubleY ) )
                           scaleFactor = fabs( doubleX );
                        else
                           scaleFactor = fabs( doubleY );
                        scaleFactor /= sqrt( xSqr + doubleY * doubleY );
                     }

                     points[ps + y*s1 + x] =
                        PMPoint( bp + m_areaAxis1 *
                                 ( ( ( doubleX * scaleFactor ) / 2.0 ) + 0.5 )
                                 + m_areaAxis2 *
                                 ( ( ( doubleY * scaleFactor ) / 2.0 ) + 0.5 ) );
                  }
               }
            }

            for( x = 0; x < s1; x++ )
            {
               for( y = 0; y < (s2-1); y++ )
               {
                  h = ps + x + s1*y;
                  lines[ls + x*(s2-1) + y] = PMLine( h, h+s1 );
               }
            }

            ls += s1*(s2-1);
            for( y = 0; y < s2; y++ )
            {
               for( x = 0; x < (s1-1); x++ )
               {
                  h = ps + x + s1*y;
                  lines[ls + y*(s1-1) + x] = PMLine( h, h+1 );
               }
            }
         }
      }
   }
}

PMViewStructure* PMLight::defaultPointStructure() const
{
   if( !s_pDefaultPointStructure )
   {
      s_pDefaultPointStructure = new PMViewStructure( 14, 7 );
//      PMPointArray& points = s_pDefaultPointStructure->points();
      PMLineArray& lines = s_pDefaultPointStructure->lines();

      lines[0] = PMLine( 0, 1 );
      lines[1] = PMLine( 2, 3 );
      lines[2] = PMLine( 4, 5 );
      lines[3] = PMLine( 6, 7 );
      lines[4] = PMLine( 8, 9 );
      lines[5] = PMLine( 10, 11 );
      lines[6] = PMLine( 12, 13 );
   }
   return s_pDefaultPointStructure;
}

PMViewStructure* PMLight::defaultSpotStructure() const
{
   if( !s_pDefaultSpotStructure )
   {
      s_pDefaultSpotStructure = new PMViewStructure( s_nSpotLines * 2 + 2, s_nSpotLines * 3 + 1 );
//      PMPointArray& points = s_pDefaultSpotStructure->points();
      PMLineArray& lines = s_pDefaultSpotStructure->lines();

      int i;
      for( i = 0; i < s_nSpotLines; i++ )
      {
         lines[i] = PMLine( 0, i+1 );
         lines[s_nSpotLines + i] = PMLine( i+1, i+2 );
         lines[2*s_nSpotLines + i] = PMLine( s_nSpotLines + i+1,
                                             s_nSpotLines + i+2 );
      }
      // fix for the last line
      lines[2*s_nSpotLines - 1] = PMLine( 1, s_nSpotLines );
      lines[3*s_nSpotLines - 1] = PMLine( s_nSpotLines + 1, s_nSpotLines*2 );
      lines[3*s_nSpotLines] = PMLine( 0, s_nSpotLines*2 + 1 );
   }
   return s_pDefaultSpotStructure;
}

PMViewStructure* PMLight::defaultCylindricalStructure() const
{
   if( !s_pDefaultCylindricalStructure )
   {
      s_pDefaultCylindricalStructure = new PMViewStructure( s_nCylinderLines * 4 + 2, s_nCylinderLines * 5 + 1 );
//      PMPointArray& points = s_pDefaultCylindricalStructure->points();
      PMLineArray& lines = s_pDefaultCylindricalStructure->lines();

      int i;
      for( i = 0; i < s_nCylinderLines; i++ )
      {
         lines[i] = PMLine( i, i+1 );
         lines[s_nCylinderLines + i] = PMLine( i + s_nCylinderLines,
                                               i + s_nCylinderLines + 1 );
         lines[2*s_nCylinderLines + i] = PMLine( i + 2*s_nCylinderLines,
                                                 i + 2*s_nCylinderLines + 1 );
         lines[3*s_nCylinderLines + i] = PMLine( i + 3*s_nCylinderLines,
                                                 i + 3*s_nCylinderLines + 1 );
         lines[4*s_nCylinderLines + i] = PMLine( i, i + s_nCylinderLines );
      }
      // fix for some lines
      lines[s_nCylinderLines-1] = PMLine( 0, s_nCylinderLines - 1 );
      lines[2*s_nCylinderLines-1] = PMLine( s_nCylinderLines,
                                            2*s_nCylinderLines - 1 );
      lines[3*s_nCylinderLines-1] = PMLine( 2*s_nCylinderLines,
                                            3*s_nCylinderLines - 1 );
      lines[4*s_nCylinderLines-1] = PMLine( 3*s_nCylinderLines,
                                            4*s_nCylinderLines - 1 );
      lines[5*s_nCylinderLines] = PMLine( 4*s_nCylinderLines,
                                          4*s_nCylinderLines + 1 );
   }
   return s_pDefaultCylindricalStructure;
}

void PMLight::controlPoints( PMControlPointList& list )
{
   list.append( new PM3DControlPoint( m_location, PMLocationID, ( "Location" ) ) );
   if( ( m_type == SpotLight ) || ( m_type == CylinderLight ) )
      list.append( new PM3DControlPoint( m_pointAt, PMPointAtID, ( "Point at" ) ) );
}

void PMLight::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it;

   for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMLocationID:
               setLocation( ( ( PM3DControlPoint* ) *it )->point() );
               break;
            case PMPointAtID:
               setPointAt( ( ( PM3DControlPoint* ) *it )->point() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMLight::controlPointsChanged\n";
               break;
         }
      }
   }
}

void PMLight::cleanUp() const
{
   if( s_pDefaultPointStructure )
      delete s_pDefaultPointStructure;
   s_pDefaultPointStructure = 0;
   if( s_pDefaultSpotStructure )
      delete s_pDefaultSpotStructure;
   s_pDefaultSpotStructure = 0;
   if( s_pDefaultCylindricalStructure )
      delete s_pDefaultCylindricalStructure;
   s_pDefaultCylindricalStructure = 0;
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}
