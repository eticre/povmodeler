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


#include "pmglobalphotons.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmenumproperty.h"
#include "pmglobalphotonsedit.h"



const double spacingDefault = 0.01;
const int countDefault = 20000;
const int gatherMinDefault = 20;
const int gatherMaxDefault = 100;
const int mediaMaxStepsDefault = 0;
const double mediaFactorDefault = 1.0;
const double jitterDefault = 0.4;
const int maxTraceLevelDefault = 0;
const double adcBailoutDefault = 0.01;
const double autostopDefault = 0.0;
const double expandIncreaseDefault = 0.2;
const int expandMinDefault = 40;
const double radiusGatherDefault = 0.0;
const double radiusGatherMultiDefault = 1.0;
const double radiusMediaDefault = 0.0;
const double radiusMediaMultiDefault = 1.0;

PMDefinePropertyClass( PMGlobalPhotons, PMGlobalPhotonsProperty );
PMDefineEnumPropertyClass( PMGlobalPhotons, PMGlobalPhotons::PMNumberType, PMNumberProperty );

PMMetaObject* PMGlobalPhotons::s_pMetaObject = 0;
PMObject* createNewGlobalPhotons( PMPart* part )
{
   return new PMGlobalPhotons( part );
}

PMGlobalPhotons::PMGlobalPhotons( PMPart* part ) : Base( part )
{
   m_numberType = Spacing;
   m_spacing = spacingDefault;
   m_count = countDefault;
   m_gatherMin = gatherMinDefault;
   m_gatherMax = gatherMaxDefault;
   m_mediaMaxSteps = mediaMaxStepsDefault;
   m_mediaFactor = mediaFactorDefault;
   m_jitter = jitterDefault;
   m_maxTraceLevelGlobal = true;
   m_maxTraceLevel = maxTraceLevelDefault;
   m_adcBailoutGlobal = true;
   m_adcBailout = adcBailoutDefault;
   m_autostop = autostopDefault;
   m_expandIncrease = expandIncreaseDefault;
   m_expandMin = expandMinDefault;
   m_radiusGather = radiusGatherDefault;
   m_radiusGatherMulti = radiusGatherMultiDefault;
   m_radiusMedia = radiusMediaDefault;
   m_radiusMediaMulti = radiusMediaMultiDefault;
}

PMGlobalPhotons::PMGlobalPhotons( const PMGlobalPhotons& p )
      : Base( p )
{
   m_numberType = p.m_numberType;
   m_spacing = p.m_spacing;
   m_count = p.m_count;
   m_gatherMin = p.m_gatherMin;
   m_gatherMax = p.m_gatherMax;
   m_mediaMaxSteps = p.m_mediaMaxSteps;
   m_mediaFactor = p.m_mediaFactor;
   m_jitter = p.m_jitter;
   m_maxTraceLevelGlobal = p.m_maxTraceLevelGlobal;
   m_maxTraceLevel = p.m_maxTraceLevel;
   m_adcBailoutGlobal = p.m_adcBailoutGlobal;
   m_adcBailout = p.m_adcBailout;
   m_autostop = p.m_autostop;
   m_expandIncrease = p.m_expandIncrease;
   m_expandMin = p.m_expandMin;
   m_radiusGather = p.m_radiusGather;
   m_radiusGatherMulti = p.m_radiusGatherMulti;
   m_radiusMedia = p.m_radiusMedia;
   m_radiusMediaMulti = p.m_radiusMediaMulti;
}

PMGlobalPhotons::~PMGlobalPhotons()
{
}

PMMetaObject* PMGlobalPhotons::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "GlobalPhotons", Base::metaObject(),
                                        createNewGlobalPhotons );

      PMNumberProperty* p1 = new PMNumberProperty( "numberType",
         &PMGlobalPhotons::setNumberType, &PMGlobalPhotons::numberType );
      p1->addEnumValue( "Spacing", Spacing );
      p1->addEnumValue( "Count", Count );
      s_pMetaObject->addProperty( p1 );

      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "spacing",
         &PMGlobalPhotons::setSpacing, &PMGlobalPhotons::spacing ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "count",
         &PMGlobalPhotons::setCount, &PMGlobalPhotons::count ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "gatherMin",
         &PMGlobalPhotons::setGatherMin, &PMGlobalPhotons::gatherMin ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "gatherMax",
         &PMGlobalPhotons::setGatherMax, &PMGlobalPhotons::gatherMax ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "mediaMaxSteps",
         &PMGlobalPhotons::setMediaMaxSteps, &PMGlobalPhotons::mediaMaxSteps ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "mediaFactor",
         &PMGlobalPhotons::setMediaFactor, &PMGlobalPhotons::mediaFactor ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "jitter",
         &PMGlobalPhotons::setJitter, &PMGlobalPhotons::jitter ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "maxTraceLevelGlobal",
         &PMGlobalPhotons::setMaxTraceLevelGlobal, &PMGlobalPhotons::maxTraceLevelGlobal ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "maxTraceLevel",
         &PMGlobalPhotons::setMaxTraceLevel, &PMGlobalPhotons::maxTraceLevel ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "adcBailoutGlobal",
         &PMGlobalPhotons::setAdcBailoutGlobal, &PMGlobalPhotons::adcBailoutGlobal ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "adcBailout",
         &PMGlobalPhotons::setAdcBailout, &PMGlobalPhotons::adcBailout ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "autostop",
         &PMGlobalPhotons::setAutostop, &PMGlobalPhotons::autostop ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "expandIncrease",
         &PMGlobalPhotons::setExpandIncrease, &PMGlobalPhotons::expandIncrease ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "expandMin",
         &PMGlobalPhotons::setExpandMin, &PMGlobalPhotons::expandMin ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "radiusGather",
         &PMGlobalPhotons::setRadiusGather, &PMGlobalPhotons::radiusGather ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "radiusGatherMulti",
         &PMGlobalPhotons::setRadiusGatherMulti, &PMGlobalPhotons::radiusGatherMulti ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "radiusMedia",
         &PMGlobalPhotons::setRadiusMedia, &PMGlobalPhotons::radiusMedia ) );
      s_pMetaObject->addProperty( new PMGlobalPhotonsProperty( "radiusMediaMulti",
         &PMGlobalPhotons::setRadiusMediaMulti, &PMGlobalPhotons::radiusMediaMulti ) );
   }
   return s_pMetaObject;
}

void PMGlobalPhotons::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMGlobalPhotons::description() const
{
   return ( "global photons" );
}

void PMGlobalPhotons::serialize( QDomElement& e, QDomDocument& ) const
{
   if( m_numberType == Spacing )
   {
       e.setAttribute( "number_type", "spacing" );
       e.setAttribute( "spacing", m_spacing );
   }
   else
   {
      e.setAttribute( "number_type", "count" );
      e.setAttribute( "count", m_count );
   }

   e.setAttribute( "gather_min", m_gatherMin );
   e.setAttribute( "gather_max", m_gatherMax );
   e.setAttribute( "media_max_steps", m_mediaMaxSteps );
   e.setAttribute( "media_factor", m_mediaFactor );
   e.setAttribute( "jitter", m_jitter );
   e.setAttribute( "max_trace_level_global", m_maxTraceLevelGlobal );
   e.setAttribute( "max_trace_level", m_maxTraceLevel );
   e.setAttribute( "adc_bailout_global", m_adcBailoutGlobal );
   e.setAttribute( "adc_bailout", m_adcBailout );
   e.setAttribute( "autostop", m_autostop );
   e.setAttribute( "expand_increase", m_expandIncrease );
   e.setAttribute( "expand_min", m_expandMin );
   e.setAttribute( "radius_gather", m_radiusGather );
   e.setAttribute( "radius_gather_multi", m_radiusGatherMulti );
   e.setAttribute( "radius_media", m_radiusMedia );
   e.setAttribute( "radius_media_multi", m_radiusMediaMulti );
}

void PMGlobalPhotons::readAttributes( const PMXMLHelper& h )
{
   QString str;

   str = h.stringAttribute( "number_type", "spacing" );
   if( str == "count" )
      m_numberType = Count;
   else
      m_numberType = Spacing;

   m_spacing = h.doubleAttribute( "spacing", spacingDefault );
   m_count = h.intAttribute( "count", countDefault );
   m_gatherMin = h.intAttribute( "gather_min", gatherMinDefault );
   m_gatherMax = h.intAttribute( "gather_max", gatherMaxDefault );
   m_mediaMaxSteps = h.intAttribute( "media_max_steps", mediaMaxStepsDefault );
   m_mediaFactor = h.doubleAttribute( "media_factor", mediaFactorDefault );
   m_jitter = h.doubleAttribute( "jitter", jitterDefault );
   m_maxTraceLevelGlobal = h.boolAttribute( "max_trace_level_global", true );
   m_maxTraceLevel = h.intAttribute( "max_trace_level", maxTraceLevelDefault );
   m_adcBailoutGlobal = h.boolAttribute( "adc_bailout_global", true );
   m_adcBailout = h.doubleAttribute( "adc_bailout", adcBailoutDefault );
   m_autostop = h.doubleAttribute( "autostop", autostopDefault );
   m_expandIncrease = h.doubleAttribute( "expand_increase", expandIncreaseDefault );
   m_expandMin = h.intAttribute( "expand_min", expandMinDefault );
   m_radiusGather = h.doubleAttribute( "radius_gather", radiusGatherDefault );
   m_radiusGatherMulti = h.doubleAttribute( "radius_gather_multi",
                                            radiusGatherMultiDefault );

   m_radiusMedia = h.doubleAttribute( "radius_media", radiusMediaDefault );
   m_radiusMediaMulti = h.doubleAttribute( "radius_media_multi",
                                           radiusMediaMultiDefault );
}

void PMGlobalPhotons::setNumberType( PMNumberType nt )
{
   if( nt != m_numberType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNumberTypeID, m_numberType );
      m_numberType = nt;
   }
}

void PMGlobalPhotons::setSpacing( double s )
{
   if( s != m_spacing )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSpacingID, m_spacing );
      m_spacing = s;
   }
}

void PMGlobalPhotons::setCount( int c )
{
   if( c != m_count )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCountID, m_count );
      m_count = c;
   }
}

void PMGlobalPhotons::setGatherMin( int gm )
{
   if( gm > m_gatherMax )
   {
      qCritical(  ) << "Gather Minimum > Gather Maximum in PMGlobalPhotons::setGatherMin\n";
      gm = m_gatherMax;
   }

   if( gm != m_gatherMin )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMGatherMinID, m_gatherMin );
      m_gatherMin = gm;
   }
}

void PMGlobalPhotons::setGatherMax( int gm )
{
   if( gm < m_gatherMin )
   {
      qCritical(  ) << "Gather Maximum < Gather Minimum in PMGlobalPhotons::setGatherMax\n";
      gm = m_gatherMin;
   }

   if( gm != m_gatherMax )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMGatherMaxID, m_gatherMax );
      m_gatherMax = gm;
   }
}

void PMGlobalPhotons::setMediaMaxSteps( int mms )
{
   if( mms != m_mediaMaxSteps )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMediaMaxStepsID, m_mediaMaxSteps );
      m_mediaMaxSteps = mms;
   }
}

void PMGlobalPhotons::setMediaFactor( double mf )
{
   if( mf != m_mediaFactor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMediaFactorID, m_mediaFactor );
      m_mediaFactor = mf;
   }
}

void PMGlobalPhotons::setJitter( double j )
{
   if( j != m_jitter )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMJitterID, m_jitter );
      m_jitter = j;
   }
}

void PMGlobalPhotons::setMaxTraceLevelGlobal( bool mtlg )
{
   if( mtlg != m_maxTraceLevelGlobal )
   {
      if( m_pMemento )
         m_pMemento->addData(
            s_pMetaObject, PMMaxTraceLevelGlobalID, m_maxTraceLevelGlobal );
      m_maxTraceLevelGlobal = mtlg;
   }
}

void PMGlobalPhotons::setMaxTraceLevel( int mtl )
{
   if( mtl != m_maxTraceLevel )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMaxTraceLevelID, m_maxTraceLevel );
      m_maxTraceLevel = mtl;
   }
}

void PMGlobalPhotons::setAdcBailoutGlobal( bool abg )
{
   if( abg != m_adcBailoutGlobal )
   {
      if( m_pMemento )
         m_pMemento->addData(
            s_pMetaObject, PMAdcBailoutGlobalID, m_adcBailoutGlobal );
      m_adcBailoutGlobal = abg;
   }
}

void PMGlobalPhotons::setAdcBailout( double ab )
{
   if( ab != m_adcBailout )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAdcBailoutID, m_adcBailout );
      m_adcBailout = ab;
   }
}

void PMGlobalPhotons::setAutostop( double a )
{
   if( a != m_autostop )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAutostopID, m_autostop );
      m_autostop = a;
   }
}

void PMGlobalPhotons::setExpandIncrease( double ei )
{
   if( ei != m_expandIncrease )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMExpandIncreaseID, m_expandIncrease );
      m_expandIncrease = ei;
   }
}

void PMGlobalPhotons::setExpandMin( int em )
{
   if(em != m_expandMin )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMExpandMinID, m_expandMin );
      m_expandMin = em;
   }
}

void PMGlobalPhotons::setRadiusGather( double rg )
{
   if ( rg != m_radiusGather )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusGatherID, m_radiusGather );
      m_radiusGather = rg;
   }
}

void PMGlobalPhotons::setRadiusGatherMulti( double rgm )
{
   if ( rgm != m_radiusGatherMulti )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusGatherID, m_radiusGatherMulti );
      m_radiusGatherMulti = rgm;
   }
}

void PMGlobalPhotons::setRadiusMedia( double rm )
{
   if ( rm != m_radiusMedia )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusGatherID, m_radiusMedia );
      m_radiusMedia = rm;
   }
}

void PMGlobalPhotons::setRadiusMediaMulti( double rmm )
{
   if ( rmm != m_radiusMediaMulti )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusGatherID, m_radiusMediaMulti );
      m_radiusMediaMulti = rmm;
   }
}

PMDialogEditBase* PMGlobalPhotons::editWidget( QWidget* parent ) const
{
   return new PMGlobalPhotonsEdit( parent );
}

void PMGlobalPhotons::restoreMemento( PMMemento* s )
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
            case PMNumberTypeID:
               setNumberType( ( PMNumberType ) ( data->intData() ) );
               break;
            case PMSpacingID:
               setSpacing( data->doubleData() );
               break;
            case PMCountID:
               setCount( data->intData() );
               break;
            case PMGatherMinID:
               setGatherMin( data->intData() );
               break;
            case PMGatherMaxID:
               setGatherMax( data->intData() );
               break;
            case PMMediaMaxStepsID:
               setMediaMaxSteps( data->intData() );
               break;
            case PMMediaFactorID:
               setMediaFactor( data->doubleData() );
               break;
            case PMJitterID:
               setJitter( data->doubleData() );
               break;
            case PMMaxTraceLevelGlobalID:
               setMaxTraceLevelGlobal( data->boolData() );
               break;
            case PMMaxTraceLevelID:
               setMaxTraceLevel( data->intData() );
               break;
            case PMAdcBailoutGlobalID:
               setAdcBailoutGlobal( data->boolData() );
               break;
            case PMAdcBailoutID:
               setAdcBailout( data->doubleData() );
               break;
            case PMAutostopID:
               setAutostop( data->doubleData() );
               break;
            case PMExpandIncreaseID:
               setExpandIncrease( data->doubleData() );
               break;
            case PMExpandMinID:
               setExpandMin( data->intData() );
               break;
            case PMRadiusGatherID:
               setRadiusGather( data->doubleData() );
               break;
            case PMRadiusGatherMultiID:
               setRadiusGatherMulti( data->doubleData() );
               break;
            case PMRadiusMediaID:
               setRadiusMedia( data->doubleData() );
               break;
            case PMRadiusMediaMultiID:
               setRadiusMediaMulti( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMRadiosity::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
