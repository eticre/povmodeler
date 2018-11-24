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


#include "pmglobalsettings.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmglobalsettingsedit.h"
#include "pmenumproperty.h"

const double adcBailoutDefault = 1.0 / 255.0;
const PMColor ambientLightDefault = PMColor( 1.0, 1.0, 1.0, 0.0, 0.0 );
const double assumedGammaDefault = 1;
const bool hfGray16Default = false;
const PMColor iridWaveLengthDefault = PMColor( 0.25, 0.18, 0.14, 0.0, 0.0 );
const int maxIntersectionsDefault = 0; // ???
const int maxTraceLevelDefault = 0;   // ???
const int numberWavesDefault = 10;
const bool radiosityDefault = false;
const double brightnessDefault = 1.0;
const int countDefault = 35;
const double distanceMaximumDefault = 0; // ???
const double errorBoundDefault = 1.8;
const double grayThresholdDefault = 0.0;
const double lowErrorFactorDefault = 0.5;
const double minimumReuseDefault = 0.015;
const int nearestCountDefault = 5;
const int recursionLimitDefault = 2;

PMDefinePropertyClass( PMGlobalSettings, PMGlobalSettingsProperty );
PMDefineEnumPropertyClass( PMGlobalSettings, PMGlobalSettings::PMNoiseType, PMNoiseProperty );

PMMetaObject* PMGlobalSettings::s_pMetaObject = nullptr;
PMObject* createNewGlobalSettings( PMPart* part )
{
   return new PMGlobalSettings( part );
}

PMGlobalSettings::PMGlobalSettings( PMPart* part )
      : Base( part )
{
  m_adcBailout = adcBailoutDefault;
  m_ambientLight = ambientLightDefault;
  m_assumedGamma = assumedGammaDefault;
  m_hfGray16 = hfGray16Default;
  m_iridWaveLength = iridWaveLengthDefault;
  m_maxIntersections = maxIntersectionsDefault;
  m_maxTraceLevel = maxTraceLevelDefault;
  m_numberWaves = numberWavesDefault;
  m_noiseGenerator = RangeCorrected;
  m_radiosityEnabled = radiosityDefault;
  m_brightness = brightnessDefault;
  m_count = countDefault;
  m_distanceMaximum = distanceMaximumDefault;
  m_errorBound = errorBoundDefault;
  m_grayThreshold = grayThresholdDefault;
  m_lowErrorFactor = lowErrorFactorDefault;
  m_minimumReuse = minimumReuseDefault;
  m_nearestCount = nearestCountDefault;
  m_recursionLimit = recursionLimitDefault;
}

PMGlobalSettings::PMGlobalSettings( const PMGlobalSettings& s )
      : Base( s )
{
  m_adcBailout = s.m_adcBailout;
  m_ambientLight = s.m_ambientLight;
  m_assumedGamma = s.m_assumedGamma;
  m_hfGray16 = s.m_hfGray16;
  m_iridWaveLength = s.m_iridWaveLength;
  m_maxIntersections = s.m_maxIntersections;
  m_maxTraceLevel = s.m_maxTraceLevel;
  m_numberWaves = s.m_numberWaves;
  m_noiseGenerator = s.m_noiseGenerator;
  m_radiosityEnabled = s.m_radiosityEnabled;
  m_brightness = s.m_brightness;
  m_count = s.m_count;
  m_distanceMaximum = s.m_distanceMaximum;
  m_errorBound = s.m_errorBound;
  m_grayThreshold = s.m_grayThreshold;
  m_lowErrorFactor = s.m_lowErrorFactor;
  m_minimumReuse = s.m_minimumReuse;
  m_nearestCount = s.m_nearestCount;
  m_recursionLimit = s.m_recursionLimit;
}

PMGlobalSettings::~PMGlobalSettings()
{
}

PMMetaObject* PMGlobalSettings::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "GlobalSettings", Base::metaObject(),
                                        createNewGlobalSettings );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "adcBailout", &PMGlobalSettings::setAdcBailout, &PMGlobalSettings::adcBailout ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "ambientLight", &PMGlobalSettings::setAmbientLight, &PMGlobalSettings::ambientLight ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "assumedGamma", &PMGlobalSettings::setAssumedGamma, &PMGlobalSettings::assumedGamma ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "hfGray16", &PMGlobalSettings::setHfGray16, &PMGlobalSettings::hfGray16 ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "iridWaveLength", &PMGlobalSettings::setIridWaveLength, &PMGlobalSettings::iridWaveLength ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "maxIntersections", &PMGlobalSettings::setMaxIntersections, &PMGlobalSettings::maxIntersections ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "maxTraceLevel", &PMGlobalSettings::setMaxTraceLevel, &PMGlobalSettings::maxTraceLevel ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "numberWaves", &PMGlobalSettings::setNumberWaves, &PMGlobalSettings::numberWaves ) );

      PMNoiseProperty* p = new PMNoiseProperty( "noiseGenerator", &PMGlobalSettings::setNoiseGenerator,
                                              &PMGlobalSettings::noiseGenerator );
      p->addEnumValue( "Original", Original );
      p->addEnumValue( "RangeCorrected", RangeCorrected );
      p->addEnumValue( "Perlin", Perlin );
      s_pMetaObject->addProperty( p );

      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "radiosity", &PMGlobalSettings::enableRadiosity, &PMGlobalSettings::isRadiosityEnabled ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "brightness", &PMGlobalSettings::setBrightness, &PMGlobalSettings::brightness ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "count", &PMGlobalSettings::setCount, &PMGlobalSettings::count ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "distanceMaximum", &PMGlobalSettings::setDistanceMaximum, &PMGlobalSettings::distanceMaximum ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "errorBound", &PMGlobalSettings::setErrorBound, &PMGlobalSettings::errorBound ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "grayThreshold", &PMGlobalSettings::setGrayThreshold, &PMGlobalSettings::grayThreshold ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "lowErrorFactor", &PMGlobalSettings::setLowErrorFactor, &PMGlobalSettings::lowErrorFactor ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "minimumReuse", &PMGlobalSettings::setMinimumReuse, &PMGlobalSettings::minimumReuse ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "nearestCount", &PMGlobalSettings::setNearestCount, &PMGlobalSettings::nearestCount ) );
      s_pMetaObject->addProperty(
         new PMGlobalSettingsProperty( "recursionLimit", &PMGlobalSettings::setRecursionLimit, &PMGlobalSettings::recursionLimit ) );
   }
   return s_pMetaObject;
}

void PMGlobalSettings::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMGlobalSettings::description() const
{
   return ( "global settings" );
}

void PMGlobalSettings::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "adc_bailout", m_adcBailout );
   e.setAttribute( "ambient_light", m_ambientLight.serializeXML() );
   e.setAttribute( "assumed_gamma", m_assumedGamma );
   e.setAttribute( "hf_gray_16", m_hfGray16 );
   e.setAttribute( "irid_wavelength", m_iridWaveLength.serializeXML() );
   e.setAttribute( "max_intersections", m_maxIntersections );
   e.setAttribute( "max_trace_level", m_maxTraceLevel );
   e.setAttribute( "number_of_waves", m_numberWaves );
   e.setAttribute( "radiosity", m_radiosityEnabled );
   switch ( m_noiseGenerator )
   {
      case Original:
         e.setAttribute( "noise_generator", "original" );
         break;
      case RangeCorrected:
         e.setAttribute( "noise_generator", "range_corrected" );
         break;
      case Perlin:
         e.setAttribute( "noise_generator", "perlin" );
         break;
   }
   e.setAttribute( "brightness", m_brightness );
   e.setAttribute( "count", m_count );
   e.setAttribute( "distance_maximum", m_distanceMaximum );
   e.setAttribute( "error_bound", m_errorBound );
   e.setAttribute( "gray_threshold", m_grayThreshold );
   e.setAttribute( "low_error_factor", m_lowErrorFactor );
   e.setAttribute( "minimum_reuse", m_minimumReuse );
   e.setAttribute( "nearest_count", m_nearestCount );
   e.setAttribute( "recursion_limit", m_recursionLimit );

   Base::serialize( e, doc );
}

void PMGlobalSettings::readAttributes( const PMXMLHelper& h )
{
   QString str;

   m_adcBailout = h.doubleAttribute( "adc_bailout", adcBailoutDefault );
   m_ambientLight = h.colorAttribute( "ambient_light", ambientLightDefault );
   m_assumedGamma = h.doubleAttribute( "assumed_gamma", assumedGammaDefault );
   m_hfGray16 = h.boolAttribute( "hf_gray_16", hfGray16Default );
   m_iridWaveLength = h.colorAttribute( "irid_wavelength", iridWaveLengthDefault );
   m_maxIntersections = h.intAttribute( "max_intersections", maxIntersectionsDefault );
   m_maxTraceLevel = h.intAttribute( "max_trace_level", maxTraceLevelDefault );
   m_numberWaves = h.intAttribute( "number_of_waves", numberWavesDefault );
   str = h.stringAttribute( "noise_generator", "range_corrected" );
   if ( str == "original" )
      m_noiseGenerator = Original;
   else if ( str == "perlin" )
      m_noiseGenerator = Perlin;
   else
      m_noiseGenerator = RangeCorrected;
   m_radiosityEnabled = h.boolAttribute( "radiosity", radiosityDefault );
   m_brightness = h.doubleAttribute( "brightness", brightnessDefault );
   m_count = h.intAttribute( "count", countDefault );
   m_distanceMaximum = h.doubleAttribute( "distance_maximum", distanceMaximumDefault );
   m_errorBound = h.doubleAttribute( "error_bound", errorBoundDefault );
   m_grayThreshold = h.doubleAttribute( "gray_threshold", grayThresholdDefault );
   m_lowErrorFactor = h.doubleAttribute( "low_error_factor", lowErrorFactorDefault );
   m_minimumReuse = h.doubleAttribute( "minimum_reuse", minimumReuseDefault );
   m_nearestCount = h.intAttribute( "nearest_count", nearestCountDefault );
   m_recursionLimit = h.intAttribute( "recursion_limit", recursionLimitDefault );

   Base::readAttributes( h );
}

void PMGlobalSettings::setAdcBailout( double c )
{
   if( c != m_adcBailout )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAdcBailoutID, m_adcBailout );
      m_adcBailout = c;
   }
}

void PMGlobalSettings::setAmbientLight( const PMColor& c )
{
   if( c != m_ambientLight )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAmbientLightID, m_ambientLight );
      m_ambientLight = c;
   }
}

void PMGlobalSettings::setAssumedGamma( double c )
{
   if( c != m_assumedGamma )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAssumedGammaID, m_assumedGamma );
      m_assumedGamma = c;
   }
}

void PMGlobalSettings::setIridWaveLength( const PMColor& c )
{
   if( c != m_iridWaveLength )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMIridWaveLengthID, m_iridWaveLength );
      m_iridWaveLength = c;
   }
}

void PMGlobalSettings::setHfGray16( bool c )
{
   if( c != m_hfGray16 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHfGray16ID, m_hfGray16 );
      m_hfGray16 = c;
   }
}

void PMGlobalSettings::setMaxIntersections( int c )
{
   if( c != m_maxIntersections )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMaxIntersectionsID, m_maxIntersections );
      m_maxIntersections = c;
   }
}

void PMGlobalSettings::setMaxTraceLevel( int c )
{
   if( c != m_maxTraceLevel )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMaxTraceLevelID, m_maxTraceLevel );
      m_maxTraceLevel = c;
   }
}

void PMGlobalSettings::setNumberWaves( int c )
{
   if( c != m_numberWaves )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNumberWavesID, m_numberWaves );
      m_numberWaves = c;
   }
}

void PMGlobalSettings::setNoiseGenerator( PMNoiseType c )
{
   if( c != m_noiseGenerator )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNoiseGeneratorID, m_noiseGenerator );
      m_noiseGenerator = c;
   }
}

void PMGlobalSettings::enableRadiosity( bool c )
{
   if( c != m_radiosityEnabled )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiosityEnabledID, m_radiosityEnabled );
      m_radiosityEnabled = c;
   }
}

void PMGlobalSettings::setBrightness( double c )
{
   if( c != m_brightness )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBrightnessID, m_brightness );
      m_brightness = c;
   }
}

void PMGlobalSettings::setCount( int c )
{
   if( c != m_count )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCountID, m_count );
      m_count = c;
   }
}

void PMGlobalSettings::setDistanceMaximum( double c )
{
   if( c != m_distanceMaximum )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDistanceMaximumID, m_distanceMaximum );
      m_distanceMaximum = c;
   }
}

void PMGlobalSettings::setErrorBound( double c )
{
   if( c != m_errorBound )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMErrorBoundID, m_errorBound );
      m_errorBound = c;
   }
}

void PMGlobalSettings::setGrayThreshold( double c )
{
   if( c != m_grayThreshold )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMGrayThresholdID, m_grayThreshold );
      m_grayThreshold = c;
   }
}

void PMGlobalSettings::setLowErrorFactor( double c )
{
   if( c != m_lowErrorFactor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLowErrorFactorID, m_lowErrorFactor );
      m_lowErrorFactor = c;
   }
}

void PMGlobalSettings::setMinimumReuse( double c )
{
   if( c != m_minimumReuse )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMinimumReuseID, m_minimumReuse );
      m_minimumReuse = c;
   }
}

void PMGlobalSettings::setNearestCount( int c )
{
   if( c != m_nearestCount )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNearestCountID, m_nearestCount );
      m_nearestCount = c;
   }
}

void PMGlobalSettings::setRecursionLimit( int c )
{
   if( c != m_recursionLimit )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRecursionLimitID, m_recursionLimit );
      m_recursionLimit = c;
   }
}

PMDialogEditBase* PMGlobalSettings::editWidget( QWidget* parent ) const
{
   return new PMGlobalSettingsEdit( parent );
}

void PMGlobalSettings::restoreMemento( PMMemento* s )
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
            case PMAdcBailoutID:
               setAdcBailout( data->doubleData() );
               break;
            //xxko case PMEmissionLightID:
            //xxko   setEmissionLight( data->colorData() );
             //xxko  break;
            case PMAmbientLightID:
               setAmbientLight( data->colorData() );
               break;
            case PMAssumedGammaID:
               setAssumedGamma( data->doubleData() );
               break;
            case PMHfGray16ID:
               setHfGray16( data->boolData() );
               break;
            case PMIridWaveLengthID:
               setIridWaveLength( data->colorData() );
               break;
           case PMMaxIntersectionsID:
               setMaxIntersections( data->intData() );
               break;
            case PMMaxTraceLevelID:
               setMaxTraceLevel( data->intData() );
               break;
            case PMNumberWavesID:
               setNumberWaves( data->intData() );
               break;
            case PMNoiseGeneratorID:
               setNoiseGenerator( ( PMNoiseType ) ( data->intData() ) );
               break;
            case PMRadiosityEnabledID:
               enableRadiosity( data->boolData() );
               break;
            case PMBrightnessID:
               setBrightness( data->doubleData() );
               break;
            case PMCountID:
               setCount( data->intData() );
               break;
            case PMDistanceMaximumID:
               setDistanceMaximum( data->doubleData() );
               break;
            case PMErrorBoundID:
               setErrorBound( data->doubleData() );
               break;
            case PMGrayThresholdID:
               setGrayThreshold( data->doubleData() );
               break;
            case PMLowErrorFactorID:
               setLowErrorFactor( data->doubleData() );
               break;
            case PMMinimumReuseID:
               setMinimumReuse( data->doubleData() );
               break;
            case PMNearestCountID:
               setNearestCount( data->intData() );
               break;
            case PMRecursionLimitID:
               setRecursionLimit( data->intData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMGlobalSettings::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
