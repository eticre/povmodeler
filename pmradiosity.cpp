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


#include "pmradiosity.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmradiosityedit.h"



const double adcBailoutDefault = 0.01;
const double brightnessDefault = 1.0;
const int countDefault = 35;
const double errorBoundDefault = 1.8;
const double grayThresholdDefault = 0.0;
const double lowErrorFactorDefault = 0.5;
const double maxSampleDefault = -1.0;
const double minimumReuseDefault = 0.015;
const int nearestCountDefault = 5;
const double pretraceStartDefault = 0.08;
const double pretraceEndDefault = 0.04;
const int recursionLimitDefault = 2;

PMDefinePropertyClass( PMRadiosity, PMRadiosityProperty );
PMMetaObject* PMRadiosity::s_pMetaObject = 0;
PMObject* createNewRadiosity( PMPart* part )
{
   return new PMRadiosity( part );
}

PMRadiosity::PMRadiosity( PMPart* part ) : Base( part )
{
  m_adcBailout = adcBailoutDefault;
  m_alwaysSample = true;
  m_brightness = brightnessDefault;
  m_count = countDefault;
  m_errorBound = errorBoundDefault;
  m_grayThreshold = grayThresholdDefault;
  m_lowErrorFactor = lowErrorFactorDefault;
  m_maxSample = maxSampleDefault;
  m_media = false;
  m_minimumReuse = minimumReuseDefault;
  m_nearestCount = nearestCountDefault;
  m_normal = false;
  m_pretraceStart = pretraceStartDefault;
  m_pretraceEnd = pretraceEndDefault;
  m_recursionLimit = recursionLimitDefault;
  checked_in_rendermode = 0;
  radYN = false;
}

PMRadiosity::PMRadiosity( const PMRadiosity& r )
      : Base( r )
{
  m_adcBailout = r.m_adcBailout;
  m_alwaysSample = r.m_alwaysSample;
  m_brightness = r.m_brightness;
  m_count = r.m_count;
  m_errorBound = r.m_errorBound;
  m_grayThreshold = r.m_grayThreshold;
  m_lowErrorFactor = r.m_lowErrorFactor;
  m_maxSample = r.m_maxSample;
  m_media = r.m_media;
  m_minimumReuse = r.m_minimumReuse;
  m_nearestCount = r.m_nearestCount;
  m_normal = r.m_normal;
  m_pretraceStart = r.m_pretraceStart;
  m_pretraceEnd = r.m_pretraceEnd;
  m_recursionLimit = r.m_recursionLimit;
}

PMRadiosity::~PMRadiosity()
{
}

PMMetaObject* PMRadiosity::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Radiosity", Base::metaObject(),
                                        createNewRadiosity );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "adcBailout", &PMRadiosity::setAdcBailout, &PMRadiosity::adcBailout ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "alwaysSample", &PMRadiosity::setAlwaysSample, &PMRadiosity::alwaysSample ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "brightness", &PMRadiosity::setBrightness, &PMRadiosity::brightness ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "count", &PMRadiosity::setCount, &PMRadiosity::count ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "errorBound", &PMRadiosity::setErrorBound, &PMRadiosity::errorBound ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "grayThreshold", &PMRadiosity::setGrayThreshold, &PMRadiosity::grayThreshold ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "lowErrorFactor", &PMRadiosity::setLowErrorFactor, &PMRadiosity::lowErrorFactor ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "maxSample", &PMRadiosity::setMaxSample, &PMRadiosity::maxSample ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "media", &PMRadiosity::setMedia, &PMRadiosity::media ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "minimumReuse", &PMRadiosity::setMinimumReuse, &PMRadiosity::minimumReuse ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "nearestCount", &PMRadiosity::setNearestCount, &PMRadiosity::nearestCount ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "normal", &PMRadiosity::setNormal, &PMRadiosity::normal ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "pretraceStart", &PMRadiosity::setPretraceStart, &PMRadiosity::pretraceStart ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "pretraceEnd", &PMRadiosity::setPretraceEnd, &PMRadiosity::pretraceEnd ) );
      s_pMetaObject->addProperty(
         new PMRadiosityProperty( "recursionLimit", &PMRadiosity::setRecursionLimit, &PMRadiosity::recursionLimit ) );
   }
   return s_pMetaObject;
}

void PMRadiosity::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMRadiosity::description() const
{
   return ( "radiosity" );
}

void PMRadiosity::serialize( QDomElement& e, QDomDocument& ) const
{
   e.setAttribute( "adc_bailout", m_adcBailout );

   if ( m_alwaysSample )
      e.setAttribute( "always_sample", "1" );
   else
      e.setAttribute( "always_sample", "0" );

   e.setAttribute( "brightness", m_brightness );
   e.setAttribute( "count", m_count );
   e.setAttribute( "error_bound", m_errorBound );
   e.setAttribute( "gray_threshold", m_grayThreshold );
   e.setAttribute( "low_error_factor", m_lowErrorFactor );
   e.setAttribute( "max_sample", m_maxSample );

   if ( m_media )
      e.setAttribute( "media", "1" );
   else
      e.setAttribute( "media", "0" );

   e.setAttribute( "minimum_reuse", m_minimumReuse );
   e.setAttribute( "nearest_count", m_nearestCount );

   if ( m_normal )
      e.setAttribute( "normal", "1" );
   else
      e.setAttribute( "normal", "0" );

   e.setAttribute( "pretrace_start", m_pretraceStart );
   e.setAttribute( "pretrace_end", m_pretraceEnd );
   e.setAttribute( "recursion_limit", m_recursionLimit );
}

void PMRadiosity:: readAttributes( const PMXMLHelper& h )
{
   m_adcBailout = h.doubleAttribute( "adc_bailout", adcBailoutDefault );
   m_alwaysSample = h.boolAttribute( "always_sample", true );
   m_brightness = h.doubleAttribute( "brightness", brightnessDefault );
   m_count = h.intAttribute( "count", countDefault );
   m_errorBound = h.doubleAttribute( "error_bound", errorBoundDefault );
   m_grayThreshold = h.doubleAttribute( "gray_threshold", grayThresholdDefault );
   m_lowErrorFactor = h.doubleAttribute( "low_error_factor", lowErrorFactorDefault );
   m_maxSample = h.doubleAttribute( "max_sample", maxSampleDefault );
   m_media = h.boolAttribute( "media", false );
   m_minimumReuse = h.doubleAttribute( "minimum_reuse", minimumReuseDefault );
   m_nearestCount = h.intAttribute( "nearest_count", nearestCountDefault );
   m_normal = h.boolAttribute( "normal", false );
   m_pretraceStart = h.doubleAttribute( "pretrace_start", pretraceStartDefault );
   m_pretraceEnd = h.doubleAttribute( "pretrace_end", pretraceEndDefault );
   m_recursionLimit = h.intAttribute( "recursion_limit", recursionLimitDefault );
}

void PMRadiosity::setAdcBailout( double ab )
{
   if( ab != m_adcBailout )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAdcBailoutID, m_adcBailout );
      m_adcBailout = ab;
   }
}

void PMRadiosity::setAlwaysSample( bool as )
{
   if( as != m_alwaysSample )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAlwaysSampleID, m_alwaysSample );
      m_alwaysSample = as;
   }
}

void PMRadiosity::setBrightness( double b )
{
   if( b != m_brightness )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBrightnessID, m_brightness );
      m_brightness = b;
   }
}

void PMRadiosity::setCount( int c )
{
   if( c != m_count )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCountID, m_count );
      m_count = c;
   }
}

void PMRadiosity::setErrorBound( double eb )
{
   if( eb != m_errorBound )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMErrorBoundID, m_errorBound );
      m_errorBound = eb;
   }
}

void PMRadiosity::setGrayThreshold( double gt )
{
   if( gt != m_grayThreshold )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMGrayThresholdID, m_grayThreshold );
      m_grayThreshold = gt;
   }
}

void PMRadiosity::setLowErrorFactor( double lew )
{
   if( lew != m_lowErrorFactor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLowErrorFactorID, m_lowErrorFactor );
      m_lowErrorFactor = lew;
   }
}

void PMRadiosity::setMaxSample( double ms )
{
   if( ms != m_maxSample )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMaxSampleID, m_maxSample );
      m_maxSample = ms;
   }
}

void PMRadiosity::setMedia( bool m )
{
   if( m != m_media )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMediaID, m_media );
      m_media = m;
   }
}

void PMRadiosity::setMinimumReuse( double c )
{
   if( c != m_minimumReuse )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMinimumReuseID, m_minimumReuse );
      m_minimumReuse = c;
   }
}

void PMRadiosity::setNearestCount( int c )
{
   if( c != m_nearestCount )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNearestCountID, m_nearestCount );
      m_nearestCount = c;
   }
}

void PMRadiosity::setNormal( bool n )
{
   if( n != m_normal )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNormalID, m_normal );
      m_normal = n;
   }
}

void PMRadiosity::setPretraceStart( double ps )
{
   if ( ps < m_pretraceEnd )
   {
      qCritical(  ) << "Pretrace Start < Pretrace End in PMRadiosity::setPretraceStart\n";
      ps = m_pretraceEnd;
   }

   if( ps != m_pretraceStart )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPretraceStartID, m_pretraceStart );
      m_pretraceStart = ps;
   }
}

void PMRadiosity::setPretraceEnd( double pe )
{
   if ( pe > m_pretraceStart )
   {
      qCritical(  ) << "Pretrace End > Pretrace Start in PMRadiosity::setPretraceEnd\n";
      pe = m_pretraceStart;
   }

   if( pe != m_pretraceEnd )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPretraceEndID, m_pretraceEnd );
      m_pretraceEnd = pe;
   }
}

void PMRadiosity::setRecursionLimit( int c )
{
   if( c != m_recursionLimit )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRecursionLimitID, m_recursionLimit );
      m_recursionLimit = c;
   }
}

PMDialogEditBase* PMRadiosity::editWidget( QWidget* parent ) const
{
   return new PMRadiosityEdit( parent );
}

void PMRadiosity::restoreMemento( PMMemento* s )
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
            case PMAlwaysSampleID:
               setAlwaysSample( data->boolData() );
               break;
            case PMBrightnessID:
               setBrightness( data->doubleData() );
               break;
            case PMCountID:
               setCount( data->intData() );
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
            case PMMaxSampleID:
               setMaxSample( data->doubleData() );
               break;
            case PMMediaID:
               setMedia( data->boolData() );
               break;
            case PMMinimumReuseID:
               setMinimumReuse( data->doubleData() );
               break;
            case PMNearestCountID:
               setNearestCount( data->intData() );
               break;
            case PMNormalID:
               setNormal( data->boolData() );
               break;
            case PMPretraceStartID:
               setPretraceStart( data->doubleData() );
               break;
            case PMPretraceEndID:
               setPretraceEnd( data->doubleData() );
               break;
            case PMRecursionLimitID:
               setRecursionLimit( data->intData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMRadiosity::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMRadiosity::slot_radiosity_YN( bool yn )
{
    radYN = yn;
}
