/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
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


#include "pmmedia.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmmediaedit.h"
#include "pmcolor.h"



const int methodDefault = 1;
const int intervalsDefault = 10;
const int samplesMinDefault = 1;
const int samplesMaxDefault = 1;
const double confidenceDefault = 0.9;
const double varianceDefault = 0.0078125;
const double ratioDefault = 0.9;
const int aaLevelDefault = 4;
const double aaThresholdDefault = 0.1;
const PMColor absorptionDefault = PMColor( 0.0, 0.0, 0.0 );
const PMColor emissionDefault = PMColor( 0.0, 0.0, 0.0 );
const int scatteringTypeDefault = 0;
const PMColor scatteringColorDefault = PMColor( 0.0, 0.0, 0.0 );
const double scatteringEccentricityDefault = 0;
const double scatteringExtinctionDefault = 1.0;

PMDefinePropertyClass( PMMedia, PMMediaProperty );
PMMetaObject* PMMedia::s_pMetaObject = 0;
PMObject* createNewMedia( PMPart* part )
{
   return new PMMedia( part );
}

PMMedia::PMMedia( PMPart* part )
      : Base( part )
{
   m_method = methodDefault;
   m_intervals = intervalsDefault;
   m_samplesMin = samplesMinDefault;
   m_samplesMax = samplesMaxDefault;
   m_confidence = confidenceDefault;
   m_variance = varianceDefault;
   m_ratio = ratioDefault;
   m_aaLevel = aaLevelDefault;
   m_aaThreshold = aaThresholdDefault;
   m_absorption = absorptionDefault;
   m_emission = emissionDefault;
   m_scatteringType = scatteringTypeDefault;
   m_scatteringColor = scatteringColorDefault;
   m_scatteringEccentricity = scatteringEccentricityDefault;
   m_scatteringExtinction = scatteringExtinctionDefault;
   m_enableAbsorption = false;
   m_enableEmission = false;
   m_enableScattering = false;
}

PMMedia::PMMedia( const PMMedia& m )
      : Base( m )
{
   m_method = m.m_method;
   m_intervals = m.m_intervals;
   m_samplesMin = m.m_samplesMin;
   m_samplesMax = m.m_samplesMax;
   m_aaLevel = m.m_aaLevel;
   m_confidence = m.m_confidence;
   m_variance = m.m_variance;
   m_ratio = m.m_ratio;
   m_aaThreshold = m.m_aaThreshold;
   m_absorption = m.m_absorption;
   m_emission = m.m_emission;
   m_scatteringType = m.m_scatteringType;
   m_scatteringColor = m.m_scatteringColor;
   m_scatteringEccentricity = m.m_scatteringEccentricity;
   m_scatteringExtinction = m.m_scatteringExtinction;
   m_enableAbsorption = m.m_enableAbsorption;
   m_enableEmission = m.m_enableEmission;
   m_enableScattering = m.m_enableScattering;
}

PMMedia::~PMMedia()
{
}

PMMetaObject* PMMedia::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Media", Base::metaObject(),
                                        createNewMedia );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "method", &PMMedia::setMethod, &PMMedia::method ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "intervals", &PMMedia::setIntervals, &PMMedia::intervals ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "samplesMin", &PMMedia::setSamplesMin, &PMMedia::samplesMin ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "samplesMax", &PMMedia::setSamplesMax, &PMMedia::samplesMax ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "aaLevel", &PMMedia::setAALevel, &PMMedia::aaLevel ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "confidence", &PMMedia::setConfidence, &PMMedia::confidence ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "variance", &PMMedia::setVariance, &PMMedia::variance ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "ratio", &PMMedia::setRatio, &PMMedia::ratio ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "aaThreshold", &PMMedia::setAAThreshold, &PMMedia::aaThreshold ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "absorption", &PMMedia::setAbsorption, &PMMedia::absorption ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "emission", &PMMedia::setEmission, &PMMedia::emission ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "scatteringType", &PMMedia::setScatteringType, &PMMedia::scatteringType ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "scatteringColor", &PMMedia::setScatteringColor, &PMMedia::scatteringColor ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "scatteringExtinction", &PMMedia::setScatteringExtinction, &PMMedia::scatteringExtinction ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "scatteringEccentricity", &PMMedia::setScatteringEccentricity, &PMMedia::scatteringEccentricity ) );

      s_pMetaObject->addProperty(
         new PMMediaProperty( "absorptionEnabled", &PMMedia::enableAbsorption, &PMMedia::isAbsorptionEnabled ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "emissionEnabled", &PMMedia::enableEmission, &PMMedia::isEmissionEnabled ) );
      s_pMetaObject->addProperty(
         new PMMediaProperty( "scatteringEnabled", &PMMedia::enableScattering, &PMMedia::isScatteringEnabled ) );
   }
   return s_pMetaObject;
}

void PMMedia::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMMedia::description() const
{
   return ( "media" );
}

void PMMedia::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
   e.setAttribute( "enable_absorption", m_enableAbsorption );
   e.setAttribute( "enable_emission", m_enableEmission );
   e.setAttribute( "enable_scattering", m_enableScattering );
   e.setAttribute( "method", m_method );
   e.setAttribute( "intervals", m_intervals );
   e.setAttribute( "samples_min", m_samplesMin );
   e.setAttribute( "samples_max", m_samplesMax );
   e.setAttribute( "aa_level", m_aaLevel );
   e.setAttribute( "confidence", m_confidence );
   e.setAttribute( "variance", m_variance );
   e.setAttribute( "ratio", m_ratio );
   e.setAttribute( "aa_threshold", m_aaThreshold );
   e.setAttribute( "absorption", m_absorption.serializeXML() );
   e.setAttribute( "emission", m_emission.serializeXML() );
   e.setAttribute( "scattering_type", m_scatteringType );
   e.setAttribute( "scattering_color", m_scatteringColor.serializeXML() );
   e.setAttribute( "scattering_eccentricity", m_scatteringEccentricity );
   e.setAttribute( "scattering_extinction", m_scatteringExtinction );
}

void PMMedia::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
   m_enableAbsorption = h.boolAttribute( "enable_absorption", false );
   m_enableEmission = h.boolAttribute( "enable_emission", false );
   m_enableScattering = h.boolAttribute( "enable_scattering", false );
   m_method = h.intAttribute( "method", methodDefault );
   m_intervals = h.intAttribute( "intervals", intervalsDefault );
   m_samplesMin = h.intAttribute( "samples_min", samplesMinDefault );
   m_samplesMax = h.intAttribute( "samples_max", samplesMaxDefault );
   m_aaLevel = h.intAttribute( "aa_level", aaLevelDefault );
   m_confidence = h.doubleAttribute( "confidence", confidenceDefault );
   m_variance = h.doubleAttribute( "variance", varianceDefault );
   m_ratio = h.doubleAttribute( "ratio", ratioDefault );
   m_aaThreshold = h.doubleAttribute( "aa_threshold", aaThresholdDefault );
   m_absorption = h.colorAttribute( "absorption", absorptionDefault );
   m_emission = h.colorAttribute( "emission", emissionDefault );
   m_scatteringType = h.intAttribute( "scattering_type", scatteringTypeDefault );
   m_scatteringColor = h.colorAttribute( "scattering_color", scatteringColorDefault );
   m_scatteringEccentricity = h.doubleAttribute( "scattering_eccentricity",
                                                       scatteringEccentricityDefault );
   m_scatteringExtinction = h.doubleAttribute( "scattering_extinction",
                                                       scatteringExtinctionDefault );
}

void PMMedia::setMethod( int c )
{
   if ( c < 1 )
   {
      qCritical(  ) << "method is < 1 in PMMedia::setMethod\n";
      c = 1;
   }

   if ( c > 3 )
   {
      qCritical(  ) << "method is > 3 in PMMedia::setMethod\n";
      c = 3;
   }

   if ( c != m_method )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMethodID, m_method );
      m_method = c;
   }
}

void PMMedia::setIntervals( int c )
{
   if( c != m_intervals )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMIntervalsID, m_intervals );
      m_intervals = c;
   }
}

void PMMedia::setSamplesMin( int c )
{
   if( c != m_samplesMin )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSamplesMinID, m_samplesMin );
      m_samplesMin = c;
   }
}

void PMMedia::setSamplesMax( int c )
{
   if( c != m_samplesMax )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSamplesMaxID, m_samplesMax );
      m_samplesMax = c;
   }
}

void PMMedia::setAALevel( int c )
{
   if ( c != m_aaLevel )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAALevelID, m_aaLevel );
      m_aaLevel = c;
   }
}

void PMMedia::setConfidence( double c )
{
   if( c != m_confidence )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMConfidenceID, m_confidence );
      m_confidence = c;
   }
}

void PMMedia::setVariance( double c )
{
   if( c != m_variance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMVarianceID, m_variance );
      m_variance = c;
   }
}

void PMMedia::setRatio( double c )
{
   if( c != m_ratio )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRatioID, m_ratio );
      m_ratio = c;
   }
}

void PMMedia::setAAThreshold( double c )
{
   if ( c != m_aaThreshold )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAAThresholdID, m_aaThreshold );
      m_aaThreshold = c;
   }
}

void PMMedia::setAbsorption( const PMColor& c )
{
   if( c != m_absorption )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAbsorptionID, m_absorption );
      m_absorption = c;
   }
}

void PMMedia::setEmission( const PMColor& c )
{
   if( c != m_emission )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEmissionID, m_emission );
      m_emission = c;
   }
}

void PMMedia::setScatteringType( int c )
{
   if( c != m_scatteringType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMScatteringTypeID, m_scatteringType );
      m_scatteringType = c;
   }
}

void PMMedia::setScatteringColor( const PMColor& c )
{
   if( c != m_scatteringColor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMScatteringColorID, m_scatteringColor );
      m_scatteringColor = c;
   }
}

void PMMedia::setScatteringEccentricity( double c )
{
   if( c != m_scatteringEccentricity )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMScatteringEccentricityID, m_scatteringEccentricity );
      m_scatteringEccentricity = c;
   }
}

void PMMedia::setScatteringExtinction( double c )
{
   if( c != m_scatteringExtinction )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMScatteringExtinctionID, m_scatteringExtinction );
      m_scatteringExtinction = c;
   }
}

void PMMedia::enableAbsorption( bool c )
{
   if( c != m_enableAbsorption )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableAbsorptionID, m_enableAbsorption );
      m_enableAbsorption = c;
   }
}

void PMMedia::enableEmission( bool c )
{
   if( c != m_enableEmission )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableEmissionID, m_enableEmission );
      m_enableEmission = c;
   }
}

void PMMedia::enableScattering( bool c )
{
   if( c != m_enableScattering )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableScatteringID, m_enableScattering );
      m_enableScattering = c;
   }
}

PMDialogEditBase* PMMedia::editWidget( QWidget* parent ) const
{
   return new PMMediaEdit( parent );
}

void PMMedia::restoreMemento( PMMemento* s )
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
            case PMEnableAbsorptionID:
               enableAbsorption( data->boolData() );
               break;
            case PMEnableEmissionID:
               enableEmission( data->boolData() );
               break;
            case PMEnableScatteringID:
               enableScattering( data->boolData() );
               break;
            case PMMethodID:
               setMethod( data->intData() );
               break;
            case PMIntervalsID:
               setIntervals( data->intData() );
               break;
            case PMSamplesMinID:
               setSamplesMin( data->intData() );
               break;
            case PMSamplesMaxID:
               setSamplesMax( data->intData() );
               break;
            case PMAALevelID:
               setAALevel( data->intData() );
               break;
            case PMConfidenceID:
               setConfidence( data->doubleData() );
               break;
            case PMVarianceID:
               setVariance( data->doubleData() );
               break;
            case PMRatioID:
               setRatio( data->doubleData() );
               break;
            case PMAAThresholdID:
               setAAThreshold( data->doubleData() );
               break;
            case PMAbsorptionID:
               setAbsorption( data->colorData() );
               break;
            case PMEmissionID:
               setEmission( data->colorData() );
               break;
            case PMScatteringTypeID:
               setScatteringType( data->intData() );
               break;
            case PMScatteringColorID:
               setScatteringColor( data->colorData() );
               break;
            case PMScatteringEccentricityID:
               setScatteringEccentricity( data->doubleData() );
               break;
            case PMScatteringExtinctionID:
               setScatteringExtinction( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMMedia::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
