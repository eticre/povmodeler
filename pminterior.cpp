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


#include "pminterior.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pminterioredit.h"



const double iorDefault = 1.0;
const double causticsDefault = 0.0;
const double dispersionDefault = 1.0;
const int dispSamplesDefault = 7;
const double fadeDistanceDefault = 0.0;
const double fadePowerDefault = 0.0;

PMDefinePropertyClass( PMInterior, PMInteriorProperty );

PMMetaObject* PMInterior::s_pMetaObject = nullptr;
PMObject* createNewInterior( PMPart* part )
{
   return new PMInterior( part );
}

PMInterior::PMInterior( PMPart* part )
      : Base( part )
{
   m_ior = iorDefault;
   m_caustics = causticsDefault;
   m_dispersion = dispersionDefault;
   m_dispSamples = dispSamplesDefault;
   m_fadeDistance = fadeDistanceDefault;
   m_fadePower = fadePowerDefault;
   m_enableIor = false;
   m_enableCaustics = false;
   m_enableDispersion = false;
   m_enableDispSamples = false;
   m_enableFadeDistance = false;
   m_enableFadePower = false;
}

PMInterior::PMInterior( const PMInterior& i )
      : Base( i )
{
   m_ior = i.m_ior;
   m_caustics = i.m_caustics;
   m_dispersion = i.m_dispersion;
   m_dispSamples = i.m_dispSamples;
   m_fadeDistance = i.m_fadeDistance;
   m_fadePower = i.m_fadePower;
   m_enableIor = i.m_enableIor;
   m_enableCaustics = i.m_enableCaustics;
   m_enableDispersion = i.m_enableDispersion;
   m_enableDispSamples = i.m_enableDispSamples;
   m_enableFadeDistance = i.m_enableFadeDistance;
   m_enableFadePower = i.m_enableFadePower;
}

PMInterior::~PMInterior()
{
}

PMMetaObject* PMInterior::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Interior", Base::metaObject(),
                                        createNewInterior );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "ior", &PMInterior::setIor, &PMInterior::ior ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "caustics", &PMInterior::setCaustics, &PMInterior::caustics ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "dispersion", &PMInterior::setDispersion, &PMInterior::dispersion ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "dispSamples", &PMInterior::setDispSamples, &PMInterior::dispSamples ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "fadeDistance", &PMInterior::setFadeDistance, &PMInterior::fadeDistance ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "fadePower", &PMInterior::setFadePower, &PMInterior::fadePower ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "iorEnabled", &PMInterior::enableIor, &PMInterior::isIorEnabled ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "causticsEnabled", &PMInterior::enableCaustics, &PMInterior::isCausticsEnabled ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "dispersionEnabled", &PMInterior::enableDispersion, &PMInterior::isDispersionEnabled ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "dispSamplesEnabled", &PMInterior::enableDispSamples, &PMInterior::isDispSamplesEnabled ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "fadeDistanceEnabled", &PMInterior::enableFadeDistance, &PMInterior::isFadeDistanceEnabled ) );
      s_pMetaObject->addProperty(
         new PMInteriorProperty( "fadePowerEnabled", &PMInterior::enableFadePower, &PMInterior::isFadePowerEnabled ) );
   }
   return s_pMetaObject;
}

void PMInterior::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

QString PMInterior::description() const
{
   return ( "interior" );
}

void PMInterior::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
   e.setAttribute( "enable_ior", m_enableIor );
   e.setAttribute( "enable_caustics", m_enableCaustics );
   e.setAttribute( "enable_dispersion", m_enableDispersion );
   e.setAttribute( "enable_disp_samples", m_enableDispSamples );
   e.setAttribute( "enable_fade_distance", m_enableFadeDistance );
   e.setAttribute( "enable_fade_power", m_enableFadePower );
   e.setAttribute( "ior", m_ior );
   e.setAttribute( "caustics", m_caustics );
   e.setAttribute( "dispersion", m_dispersion );
   e.setAttribute( "disp_samples", m_dispSamples );
   e.setAttribute( "fade_distance", m_fadeDistance );
   e.setAttribute( "fade_power", m_fadePower );
}

void PMInterior::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
   m_enableIor = h.boolAttribute( "enable_ior", false );
   m_enableCaustics = h.boolAttribute( "enable_caustics", false );
   m_enableDispersion = h.boolAttribute( "enable_dispersion", false );
   m_enableDispSamples = h.boolAttribute( "enable_disp_samples", false );
   m_enableFadeDistance = h.boolAttribute( "enable_fade_distance", false );
   m_enableFadePower = h.boolAttribute( "enable_fade_power", false );
   m_ior = h.doubleAttribute( "ior", iorDefault );
   m_caustics = h.doubleAttribute( "caustics", causticsDefault );
   m_dispersion = h.doubleAttribute( "dispersion", dispersionDefault );
   m_dispSamples = h.intAttribute( "disp_samples", dispSamplesDefault );
   m_fadeDistance = h.doubleAttribute( "fade_distance", fadeDistanceDefault );
   m_fadePower = h.doubleAttribute( "fade_power", fadePowerDefault );
}

void PMInterior::setIor( double c )
{
   if( c != m_ior )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMIorID, m_ior );
      m_ior = c;
   }
}

void PMInterior::setCaustics( double c )
{
   if( c != m_caustics )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCausticsID, m_caustics );
      m_caustics = c;
   }
}

void PMInterior::setDispersion( double c )
{
   if ( c != m_dispersion )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDispersionID, m_dispersion );
      m_dispersion = c;
   }
}

void PMInterior::setDispSamples( int c )
{
   if ( c != m_dispSamples )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDispSamplesID, m_dispSamples );
      m_dispSamples = c;
   }
}

void PMInterior::setFadeDistance( double c )
{
   if( c != m_fadeDistance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFadeDistanceID, m_fadeDistance );
      m_fadeDistance = c;
   }
}

void PMInterior::setFadePower( double c )
{
   if( c != m_fadePower )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFadePowerID, m_fadePower );
      m_fadePower = c;
   }
}

void PMInterior::enableIor( bool c )
{
   if( c != m_enableIor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableIorID, m_enableIor );
      m_enableIor = c;
   }
}

void PMInterior::enableCaustics( bool c )
{
   if( c != m_enableCaustics )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableCausticsID, m_enableCaustics );
      m_enableCaustics = c;
   }
}

void PMInterior::enableDispersion( bool c )
{
   if( c != m_enableDispersion )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableDispersionID, m_enableDispersion );
      m_enableDispersion = c;
   }
}

void PMInterior::enableDispSamples( bool c )
{
   if( c != m_enableDispSamples )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableDispSamplesID, m_enableDispSamples );
      m_enableDispSamples = c;
   }
}

void PMInterior::enableFadeDistance( bool c )
{
   if( c != m_enableFadeDistance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableFadeDistanceID,
                                                          m_enableFadeDistance );
      m_enableFadeDistance = c;
   }
}

void PMInterior::enableFadePower( bool c )
{
   if( c != m_enableFadePower )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableFadePowerID,
                                                          m_enableFadePower );
      m_enableFadePower = c;
   }
}

PMDialogEditBase* PMInterior::editWidget( QWidget* parent ) const
{
   return new PMInteriorEdit( parent );
}

void PMInterior::restoreMemento( PMMemento* s )
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
            case PMIorID:
               setIor( data->doubleData() );
               break;
            case PMCausticsID:
               setCaustics( data->doubleData() );
               break;
            case PMDispersionID:
               setDispersion( data->doubleData() );
               break;
            case PMDispSamplesID:
               setDispSamples( data->intData() );
               break;
            case PMFadeDistanceID:
               setFadeDistance( data->doubleData() );
               break;
           case PMFadePowerID:
               setFadePower( data->doubleData() );
               break;
            case PMEnableIorID:
               enableIor( data->boolData() );
               break;
            case PMEnableCausticsID:
               enableCaustics( data->boolData() );
               break;
            case PMEnableDispersionID:
               enableDispersion( data->boolData() );
               break;
            case PMEnableDispSamplesID:
               enableDispSamples( data->boolData() );
                break;
            case PMEnableFadeDistanceID:
               enableFadeDistance( data->boolData() );
               break;
            case PMEnableFadePowerID:
               enableFadePower( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMInterior::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
