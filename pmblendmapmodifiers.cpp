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

#include "pmblendmapmodifiers.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmblendmapmodifiersedit.h"
#include "pmvector.h"
#include "pmenumproperty.h"



PMDefinePropertyClass( PMBlendMapModifiers, PMBlendMapModifiersProperty );
PMDefineEnumPropertyClass( PMBlendMapModifiers,
                           PMBlendMapModifiers::PMWaveFormType,
                           PMWaveFormProperty );

PMMetaObject* PMBlendMapModifiers::s_pMetaObject = 0;
PMObject* createBlendMapModifiers( PMPart* part )
{
   return new PMBlendMapModifiers( part );
}

const double frequencyDefault = 1.0;
const double phaseDefault = 0.0;
const PMBlendMapModifiers::PMWaveFormType waveFormTypeDefault = PMBlendMapModifiers::RampWave;
const double waveFormExponentDefault = 1.0;


PMBlendMapModifiers::PMBlendMapModifiers( PMPart* part )
      : Base( part )
{
   m_enableFrequency = false;
   m_frequency = frequencyDefault;
   m_enablePhase = false;
   m_phase = phaseDefault;
   m_enableWaveForm = false;
   m_waveFormType = waveFormTypeDefault;
   m_waveFormExponent = waveFormExponentDefault;
}

PMBlendMapModifiers::~PMBlendMapModifiers()
{
}

PMMetaObject* PMBlendMapModifiers::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "BlendMapModifiers", Base::metaObject(),
                                        createBlendMapModifiers );
      s_pMetaObject->addProperty(
         new PMBlendMapModifiersProperty( "frequencyEnabled", &PMBlendMapModifiers::enableFrequency,
                         &PMBlendMapModifiers::isFrequencyEnabled ) );
      s_pMetaObject->addProperty(
         new PMBlendMapModifiersProperty( "frequency", &PMBlendMapModifiers::setFrequency,
                         &PMBlendMapModifiers::frequency ) );
      s_pMetaObject->addProperty(
         new PMBlendMapModifiersProperty( "phaseEnabled", &PMBlendMapModifiers::enablePhase,
                         &PMBlendMapModifiers::isPhaseEnabled ) );
      s_pMetaObject->addProperty(
         new PMBlendMapModifiersProperty( "phase", &PMBlendMapModifiers::setPhase,
                         &PMBlendMapModifiers::phase ) );
      s_pMetaObject->addProperty(
         new PMBlendMapModifiersProperty( "waveFormEnabled", &PMBlendMapModifiers::enableWaveForm,
                         &PMBlendMapModifiers::isWaveFormEnabled ) );
      s_pMetaObject->addProperty(
         new PMBlendMapModifiersProperty( "waveFormExponent", &PMBlendMapModifiers::setWaveFormExponent,
                         &PMBlendMapModifiers::waveFormExponent ) );

      PMWaveFormProperty* p = new PMWaveFormProperty(
         "waveForm", &PMBlendMapModifiers::setWaveFormType,
         &PMBlendMapModifiers::waveFormType );
      p->addEnumValue( QString( "RampWave" ), RampWave );
      p->addEnumValue( QString( "TriangleWave" ), TriangleWave );
      p->addEnumValue( QString( "SineWave" ), SineWave );
      p->addEnumValue( QString( "ScallopWave" ), ScallopWave );
      p->addEnumValue( QString( "CubicWave" ), CubicWave );
      p->addEnumValue( QString( "PolyWave" ), PolyWave );
      s_pMetaObject->addProperty( p );
   }
   return s_pMetaObject;
}

void PMBlendMapModifiers::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMBlendMapModifiers::description() const
{
   return ( "blend map modifiers" );
}

void PMBlendMapModifiers::serialize( QDomElement& e, QDomDocument& ) const
{
   e.setAttribute( "frequency", m_frequency );
   e.setAttribute( "phase", m_phase );
   e.setAttribute( "enable_frequency", m_enableFrequency );
   e.setAttribute( "enable_phase", m_enablePhase );
   e.setAttribute( "wave_exponent", m_waveFormExponent );
   e.setAttribute( "enable_wave", m_enableWaveForm );
   switch( m_waveFormType )
   {
      case RampWave:
         e.setAttribute( "waveform_type", "ramp" );
         break;
      case TriangleWave:
         e.setAttribute( "waveform_type", "triangle" );
         break;
      case SineWave:
         e.setAttribute( "waveform_type", "sine" );
         break;
      case ScallopWave:
         e.setAttribute( "waveform_type", "scallop" );
         break;
      case CubicWave:
         e.setAttribute( "waveform_type", "cubic" );
         break;
      case PolyWave:
         e.setAttribute( "waveform_type", "poly" );
         break;
   }
}

void PMBlendMapModifiers::readAttributes( const PMXMLHelper& h )
{
   QString str = h.stringAttribute( "waveform_type", "ramp" );

   if( str == "ramp" )
      m_waveFormType = RampWave;
   else if( str == "triangle" )
      m_waveFormType = TriangleWave;
   else if( str == "sine" )
      m_waveFormType = SineWave;
   else if( str == "scallop" )
      m_waveFormType = ScallopWave;
   else if( str == "cubic" )
      m_waveFormType = CubicWave;
   else if( str == "poly" )
      m_waveFormType = PolyWave;
   m_frequency = h.doubleAttribute( "frequency", frequencyDefault );
   m_enableFrequency = h.boolAttribute( "enable_frequency", false );
   m_phase = h.doubleAttribute( "phase", phaseDefault );
   m_enablePhase = h.boolAttribute( "enable_phase", false );
   m_enableWaveForm = h.boolAttribute( "enable_wave", false );
   m_waveFormExponent = h.doubleAttribute( "wave_exponent", waveFormExponentDefault );
}

void PMBlendMapModifiers::setFrequency( double c )
{
   if( c != m_frequency )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFrequencyID, m_frequency );
      m_frequency = c;
   }
}

void PMBlendMapModifiers::enableFrequency( bool c )
{
   if( c != m_enableFrequency )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableFrequencyID, m_enableFrequency );
      m_enableFrequency = c;
   }
}

void PMBlendMapModifiers::setPhase( double c )
{
   if( c != m_phase )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPhaseID, m_phase );
      m_phase = c;
   }
}

void PMBlendMapModifiers::enablePhase( bool c )
{
   if( c != m_enablePhase )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnablePhaseID, m_enablePhase );
      m_enablePhase = c;
   }
}

void PMBlendMapModifiers::enableWaveForm( bool c )
{
   if( c != m_enableWaveForm )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableWaveFormID, m_enableWaveForm );
      m_enableWaveForm = c;
   }
}

void PMBlendMapModifiers::setWaveFormExponent( double c )
{
   if( c != m_waveFormExponent )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMWaveFormExponentID, m_waveFormExponent );
      m_waveFormExponent = c;
   }
}

void PMBlendMapModifiers::setWaveFormType( PMWaveFormType c )
{
   if( c != m_waveFormType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMWaveFormTypeID, m_waveFormType );
      m_waveFormType = c;
   }
}

PMDialogEditBase* PMBlendMapModifiers::editWidget( QWidget* parent ) const
{
   return new PMBlendMapModifiersEdit( parent );
}

void PMBlendMapModifiers::restoreMemento( PMMemento* s )
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
            case PMFrequencyID:
               setFrequency( data->doubleData() );
               break;
            case PMEnableFrequencyID:
               enableFrequency( data->boolData() );
               break;
            case PMPhaseID:
               setPhase( data->doubleData() );
               break;
            case PMEnablePhaseID:
               enablePhase( data->boolData() );
               break;
            case PMWaveFormTypeID:
               setWaveFormType( ( PMWaveFormType )data->intData() );
               break;
            case PMEnableWaveFormID:
               enableWaveForm( data->boolData() );
               break;
            case PMWaveFormExponentID:
               setWaveFormExponent( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMBlendMapModifiers::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
