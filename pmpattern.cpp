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


#include "pmpatternedit.h"
#include "pmpattern.h"
#include "pmparser.h"

#include "pmxmlhelper.h"
#include "pmlistpatternedit.h"
#include "pmcompositeobject.h"
#include "pmmemento.h"
#include "pmenumproperty.h"



const PMPattern::PMPatternType patternTypeDefault = PMPattern::PatternAgate;
const double agateTurbulenceDefault = 0.5;
const PMVector crackleFormDefault = PMVector( -1.0, 1.0, 0.0 );
const int crackleMetricDefault = 2;
const double crackleOffsetDefault = 0.0;
const bool crackleSolidDefault = false;
const QString densityFileDefault = QString( "" );
const int densityInterpolateDefault = 0;
const PMVector gradientDefault = PMVector( 1.0, 1.0, 1.0 );
const PMVector juliaComplexDefault = PMVector( 0.353, 0.288 );
const bool fractalMagnetDefault = false;
const int fractalMagnetTypeDefault = 1;
const int maxIterationsDefault = 10;
const int fractalExponentDefault = 2;
const int fractalExtTypeDefault = 1;
const double fractalExtFactorDefault = 1.0;
const int fractalIntTypeDefault = 0;
const double fractalIntFactorDefault = 1.0;
const double quiltControl0Default = 1.0;
const double quiltControl1Default = 1.0;
const PMVector slopeDirectionDefault = PMVector( 0.0, -1.0, 0.0 );
const PMVector slopePointAtDefault = PMVector( 0.0, 0.0, 0.0 );
const bool slopePointAtFlagDefault = false;
const double slopeLoSlopeDefault = 0.0;
const double slopeHiSlopeDefault = 1.0;
const bool slopeAltFlagDefault = false;
const PMVector slopeAltitudeDefault = PMVector( 0.0, 1.0, 0.0 );
const double slopeLoAltDefault = 0.0;
const double slopeHiAltDefault = 1.0;
const int spiralNumberArmsDefault = 0;
const int tilingNumbersDefault = 1;
//const QString pigmentPatternDefault = "";
const PMPattern::PMNoiseType noiseGeneratorDefault = PMPattern::GlobalSetting;
const QString noiseGeneratorDefaultText = QString( "global_setting" );
const bool turbulenceDefault = false;
const PMVector valueVectorDefault = PMVector( 0.0, 0.0, 0.0 );
const int octavesDefault = 6;
const double omegaDefault = 0.5;
const double lambdaDefault = 2.0;
const double depthDefault = 0.0;

PMDefinePropertyClass( PMPattern, PMPatternProperty );
PMDefineEnumPropertyClass( PMPattern, PMPattern::PMPatternType,
                           PMPatternTypeProperty );
PMDefineEnumPropertyClass( PMPattern, PMPattern::PMNoiseType,
                           PMNoiseProperty );

PMMetaObject* PMPattern::s_pMetaObject = 0;
PMObject* createNewPattern( PMPart* part )
{
   return new PMPattern( part );
}

PMPattern::PMPattern( PMPart* part )
      : Base( part )
{
   //eticre try ppattern
   m_pLinkedObject = 0;

   m_patternType = patternTypeDefault;

   m_agateTurbulence = agateTurbulenceDefault;

   m_crackleForm = crackleFormDefault;
   m_crackleMetric = crackleMetricDefault;
   m_crackleOffset = crackleOffsetDefault;
   m_crackleSolid = crackleSolidDefault;

   m_densityFile = densityFileDefault;
   m_densityInterpolate = densityInterpolateDefault;

   m_gradient = gradientDefault;

   m_juliaComplex = juliaComplexDefault;
   m_fractalMagnet = fractalMagnetDefault;
   m_fractalMagnetType = fractalMagnetTypeDefault;
   m_maxIterations = maxIterationsDefault;
   m_fractalExponent = fractalExponentDefault;
   m_fractalExtType = fractalExtTypeDefault;
   m_fractalExtFactor = fractalExtFactorDefault;
   m_fractalIntType = fractalIntTypeDefault;
   m_fractalIntFactor = fractalIntFactorDefault;

   m_quiltControl0 = quiltControl0Default;
   m_quiltControl1 = quiltControl1Default;

   m_slopeDirection = slopeDirectionDefault;
   m_slopePointAt = slopePointAtDefault;
   m_slopePointAtFlag = slopePointAtFlagDefault;
   m_slopeLoSlope = slopeLoSlopeDefault;
   m_slopeHiSlope = slopeHiSlopeDefault;
   m_slopeAltFlag = slopeAltFlagDefault;
   m_slopeAltitude = slopeAltitudeDefault;
   m_slopeLoAlt = slopeLoAltDefault;
   m_slopeHiAlt = slopeHiAltDefault;

   m_spiralNumberArms = spiralNumberArmsDefault;

   m_tilingNumbers = tilingNumbersDefault;

   m_noiseGenerator = noiseGeneratorDefault;

   m_enableTurbulence = turbulenceDefault;
   m_valueVector = valueVectorDefault;
   m_octaves = octavesDefault;
   m_omega = omegaDefault;
   m_lambda = lambdaDefault;

   m_depth = depthDefault;
}

PMPattern::PMPattern( const PMPattern& p )
      : Base( p )
{
    //eticre try ppattern
   m_pLinkedObject = 0;

   m_patternType = p.m_patternType;

   m_agateTurbulence = p.m_agateTurbulence;

   m_crackleForm = p.m_crackleForm;
   m_crackleMetric = p.m_crackleMetric;
   m_crackleOffset = p.m_crackleOffset;
   m_crackleSolid = p.m_crackleSolid;

   m_densityFile = p.m_densityFile;
   m_densityInterpolate = p.m_densityInterpolate;

   m_gradient = p.m_gradient;

   m_tilingNumbers = p.m_tilingNumbers;

   m_juliaComplex = p.m_juliaComplex;
   m_fractalMagnet = p.m_fractalMagnet;
   m_fractalMagnetType = p.m_fractalMagnetType;
   m_maxIterations = p.m_maxIterations;
   m_fractalExponent = p.m_fractalExponent;
   m_fractalExtType = p.m_fractalExtType;
   m_fractalExtFactor = p.m_fractalExtFactor;
   m_fractalIntType = p.m_fractalIntType;
   m_fractalIntFactor = p.m_fractalIntFactor;

   m_quiltControl0 = p.m_quiltControl0;
   m_quiltControl1 = p.m_quiltControl1;

   m_slopeDirection = p.m_slopeDirection;
   m_slopePointAt = p.m_slopePointAt;
   m_slopePointAtFlag = p.m_slopePointAtFlag;
   m_slopeLoSlope = p.m_slopeLoSlope;
   m_slopeHiSlope = p.m_slopeHiSlope;
   m_slopeAltFlag = p.m_slopeAltFlag;
   m_slopeAltitude = p.m_slopeAltitude;
   m_slopeLoAlt = p.m_slopeLoAlt;
   m_slopeHiAlt = p.m_slopeHiAlt;

   m_spiralNumberArms = p.m_spiralNumberArms;

   m_noiseGenerator = p.m_noiseGenerator;

   m_enableTurbulence = p.m_enableTurbulence;
   m_valueVector = p.m_valueVector;
   m_octaves = p.m_octaves;
   m_omega = p.m_omega;
   m_lambda = p.m_lambda;

   m_depth = p.m_depth;
}

PMPattern::~PMPattern()
{
}

void PMPattern::serialize( QDomElement& e, QDomDocument& ) const
{
   switch( m_patternType )
   {
      case PatternAgate:
         e.setAttribute( "patterntype", "agate" );
         e.setAttribute( "agateturbulence", m_agateTurbulence );
         break;
      case PatternAverage:
         e.setAttribute( "patterntype", "average" );
         break;
      case PatternBoxed:
         e.setAttribute( "patterntype", "boxed" );
         break;
      case PatternBozo:
         e.setAttribute( "patterntype", "bozo" );
         break;
      case PatternBumps:
         e.setAttribute( "patterntype", "bumps" );
         break;
      case PatternCells:
         e.setAttribute( "patterntype", "cells" );
         break;
      case PatternCrackle:
         e.setAttribute( "patterntype", "crackle" );
         break;
      case PatternCylindrical:
         e.setAttribute( "patterntype", "cylindrical" );
         break;
      case PatternDensity:
         e.setAttribute( "patterntype", "density" );
         break;
      case PatternDents:
         e.setAttribute( "patterntype", "dents" );
         break;
      case PatternGradient:
         e.setAttribute( "patterntype", "gradient" );
         break;
      case PatternGranite:
         e.setAttribute( "patterntype", "granite" );
         break;
      case PatternJulia:
         e.setAttribute( "patterntype", "julia" );
         break;
      case PatternLeopard:
         e.setAttribute( "patterntype", "leopard" );
         break;
      case PatternMandel:
         e.setAttribute( "patterntype", "mandel" );
         break;
      case PatternMarble:
         e.setAttribute( "patterntype", "marble" );
         break;
      case PatternOnion:
         e.setAttribute( "patterntype", "onion" );
         break;
      case PatternPlanar:
         e.setAttribute( "patterntype", "planar" );
         break;
      case PatternQuilted:
         e.setAttribute( "patterntype", "quilted" );
         break;
      case PatternRadial:
         e.setAttribute( "patterntype", "radial" );
         break;
      case PatternRipples:
         e.setAttribute( "patterntype", "ripples" );
         break;
      case PatternSlope:
         e.setAttribute( "patterntype", "slope" );
         break;
      case PatternSpherical:
         e.setAttribute( "patterntype", "spherical" );
         break;
      case PatternSpiral1:
         e.setAttribute( "patterntype", "spiral1" );
         break;
      case PatternSpiral2:
         e.setAttribute( "patterntype", "spiral2" );
         break;
      case PatternSpotted:
         e.setAttribute( "patterntype", "spotted" );
         break;
      case PatternTiling:
         e.setAttribute( "patterntype", "tiling" );
         e.setAttribute( "tiling", m_tilingNumbers );
         break;
      case PatternPigment:
         e.setAttribute( "patterntype" , "pigment_pattern" );
         if( m_pLinkedObject )
            e.setAttribute( "prototype", m_pLinkedObject->id() );
         break;
      case PatternWaves:
         e.setAttribute( "patterntype", "waves" );
         break;
      case PatternWood:
         e.setAttribute( "patterntype", "wood" );
         break;
      case PatternWrinkles:
         e.setAttribute( "patterntype", "wrinkles" );
         break;
   }


   e.setAttribute( "crackleform", m_crackleForm.serializeXML() );
   e.setAttribute( "cracklemetric", m_crackleMetric );
   e.setAttribute( "crackleoffset", m_crackleOffset );
   e.setAttribute( "cracklesolid", m_crackleSolid );

   e.setAttribute( "densityinterpolate", m_densityInterpolate );
   e.setAttribute( "densityfile", m_densityFile );

   e.setAttribute( "gradient", m_gradient.serializeXML() );

   e.setAttribute( "juliacomplex", m_juliaComplex.serializeXML() );
   e.setAttribute( "fractalmagnet", m_fractalMagnet );
   e.setAttribute( "fractalmagnettype", m_fractalMagnetType );
   e.setAttribute( "maxiterations", m_maxIterations );
   e.setAttribute( "fractalexponent", m_fractalExponent );
   e.setAttribute( "fractalexttype", m_fractalExtType );
   e.setAttribute( "fractalextfactor", m_fractalExtFactor );
   e.setAttribute( "fractalinttype", m_fractalIntType );
   e.setAttribute( "fractalintfactor", m_fractalIntFactor );

   e.setAttribute( "quiltcontrol0", m_quiltControl0 );
   e.setAttribute( "quiltcontrol1", m_quiltControl1 );

   e.setAttribute( "slopedirection", m_slopeDirection.serializeXML() );
   e.setAttribute( "slopepointatflag", m_slopePointAtFlag );
   e.setAttribute( "slopepointat", m_slopePointAt.serializeXML() );
   e.setAttribute( "slopeloslope", m_slopeLoSlope );
   e.setAttribute( "slopehislope", m_slopeHiSlope );
   e.setAttribute( "slopealtflag", m_slopeAltFlag );
   e.setAttribute( "slopealtitude", m_slopeAltitude.serializeXML() );
   e.setAttribute( "slopeloalt", m_slopeLoAlt );
   e.setAttribute( "slopehialt", m_slopeHiAlt );

   e.setAttribute( "spiralnumberarms", m_spiralNumberArms );
   switch ( m_noiseGenerator )
   {
      case GlobalSetting:
         e.setAttribute( "noise_generator", "global_setting" );
         break;
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

   e.setAttribute( "enable_turbulence", m_enableTurbulence );
   e.setAttribute( "turbulence", m_valueVector.serializeXML() );
   e.setAttribute( "octaves", m_octaves );
   e.setAttribute( "omega", m_omega );
   e.setAttribute( "lambda", m_lambda );

   e.setAttribute( "depth", m_depth );
}

void PMPattern::readAttributes( const PMXMLHelper& h )
{
   QString str = h.stringAttribute( "patterntype", /*default=*/ "agate" );

   if( str == "agate" )
      m_patternType = PatternAgate;
   else if( str == "average" )
      m_patternType = PatternAverage;
   else if( str == "boxed" )
      m_patternType = PatternBoxed;
   else if( str == "bozo" )
      m_patternType = PatternBozo;
   else if( str == "bumps" )
      m_patternType = PatternBumps;
   else if( str == "cells" )
      m_patternType = PatternCells;
   else if( str == "crackle" )
      m_patternType = PatternCrackle;
   else if( str == "cylindrical" )
      m_patternType = PatternCylindrical;
   else if( str == "density" )
      m_patternType = PatternDensity;
   else if( str == "dents" )
      m_patternType = PatternDents;
   else if( str == "gradient" )
      m_patternType = PatternGradient;
   else if( str == "granite" )
      m_patternType = PatternGranite;
   else if( str == "julia" )
      m_patternType = PatternJulia;
   else if( str == "leopard" )
      m_patternType = PatternLeopard;
   else if( str == "mandel" )
      m_patternType = PatternMandel;
   else if( str == "marble" )
      m_patternType = PatternMarble;
   else if( str == "onion" )
      m_patternType = PatternOnion;
   else if( str == "planar" )
      m_patternType = PatternPlanar;
   else if( str == "quilted" )
      m_patternType = PatternQuilted;
   else if( str == "radial" )
      m_patternType = PatternRadial;
   else if( str == "ripples" )
      m_patternType = PatternRipples;
   else if( str == "slope" )
      m_patternType = PatternSlope;
   else if( str == "spherical" )
      m_patternType = PatternSpherical;
   else if( str == "spiral1" )
      m_patternType = PatternSpiral1;
   else if( str == "spiral2" )
      m_patternType = PatternSpiral2;
   else if( str == "spotted" )
      m_patternType = PatternSpotted;
   else if( str == "tiling" )
      m_patternType = PatternTiling;
   else if( str == "pigment_pattern" )
      m_patternType = PatternPigment;
   else if( str == "waves" )
      m_patternType = PatternWaves;
   else if( str == "wood" )
      m_patternType = PatternWood;
   else if( str == "wrinkles" )
      m_patternType = PatternWrinkles;

   if( str == "pigment_pattern" )
   {
       QString id = h.stringAttribute( "prototype", "" );
       if( id != "" )
       {
           m_pLinkedObject = h.parser()->checkLink( id );
           m_pLinkedObject->addLinkedObject( this );
       }
   }

   //read attribute - if not - return default
   m_agateTurbulence = h.doubleAttribute( "agateturbulence", agateTurbulenceDefault );
   m_tilingNumbers = h.intAttribute( "tiling", tilingNumbersDefault );

   m_crackleForm = h.vectorAttribute( "crackleform", crackleFormDefault );
   m_crackleMetric = h.intAttribute( "cracklemetric", crackleMetricDefault );
   m_crackleOffset = h.doubleAttribute( "crackleoffset", crackleOffsetDefault );
   m_crackleSolid = h.boolAttribute( "cracklesolid", crackleSolidDefault );

   m_densityInterpolate = h.intAttribute( "densityinterpolate", densityInterpolateDefault );
   m_densityFile = h.stringAttribute( "densityfile", densityFileDefault );

   m_gradient = h.vectorAttribute( "gradient", gradientDefault );

   m_juliaComplex = h.vectorAttribute( "juliacomplex", juliaComplexDefault );
   m_fractalMagnet = h.boolAttribute( "fractalmagnet", fractalMagnetDefault );
   m_fractalMagnetType = h.intAttribute( "fractalmagnettype", fractalMagnetTypeDefault );
   m_maxIterations = h.intAttribute( "maxiterations", maxIterationsDefault );
   m_fractalExponent = h.intAttribute( "fractalexponent", fractalExponentDefault );
   m_fractalExtType = h.intAttribute( "fractalexttype", fractalExtTypeDefault );
   m_fractalExtFactor = h.doubleAttribute( "fractalextfactor", fractalExtFactorDefault );
   m_fractalIntType = h.intAttribute( "fractalinttype", fractalIntTypeDefault );
   m_fractalIntFactor = h.doubleAttribute( "fractalintfactor", fractalIntFactorDefault );

   m_quiltControl0 = h.doubleAttribute( "quiltcontrol0", quiltControl0Default );
   m_quiltControl1 = h.doubleAttribute( "quiltcontrol1", quiltControl1Default );

   m_slopeDirection = h.vectorAttribute( "slopedirection", slopeDirectionDefault );
   m_slopePointAt = h.vectorAttribute( "slopepointat", slopePointAtDefault );
   m_slopePointAtFlag = h.boolAttribute( "slopepointatflag", slopePointAtFlagDefault );
   m_slopeLoSlope = h.doubleAttribute( "slopeloslope", slopeLoSlopeDefault );
   m_slopeHiSlope = h.doubleAttribute( "slopehislope", slopeHiSlopeDefault );
   m_slopeAltFlag = h.boolAttribute( "slopealtflag", slopeAltFlagDefault );
   m_slopeAltitude = h.vectorAttribute( "slopealtitude", slopeAltitudeDefault );
   m_slopeLoAlt = h.doubleAttribute( "slopeloalt", slopeLoAltDefault );
   m_slopeHiAlt = h.doubleAttribute( "slopehialt", slopeHiAltDefault );

   m_spiralNumberArms = h.intAttribute( "spiralnumberarms", spiralNumberArmsDefault );

   str = h.stringAttribute( "noise_generator", noiseGeneratorDefaultText );
   if ( str == "original" )
      m_noiseGenerator = Original;
   else if ( str == "range_corrected" )
      m_noiseGenerator = RangeCorrected;
   else if ( str == "perlin" )
      m_noiseGenerator = Perlin;
   else
      m_noiseGenerator = GlobalSetting;

   m_enableTurbulence = h.boolAttribute( "enable_turbulence" , false );
   m_valueVector = h.vectorAttribute( "turbulence", valueVectorDefault );
   m_octaves = h.intAttribute( "octaves", octavesDefault );
   m_omega = h.doubleAttribute( "omega", omegaDefault );
   m_lambda = h.doubleAttribute( "lambda", lambdaDefault );

   m_depth = h.doubleAttribute( "depth", depthDefault );

   Base::readAttributes( h );
}

PMMetaObject* PMPattern::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Pattern", Base::metaObject(),
                                        createNewPattern );
      s_pMetaObject->addProperty(
         new PMPatternProperty( "agateTurbulence", &PMPattern::setAgateTurbulence, &PMPattern::agateTurbulence ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "densityFile", &PMPattern::setDensityFile, &PMPattern::densityFile ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "densityInterpolate", &PMPattern::setDensityInterpolate, &PMPattern::densityInterpolate ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "gradient", &PMPattern::setGradient, &PMPattern::gradient ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "maxIterations", &PMPattern::setMaxIterations, &PMPattern::maxIterations ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "quiltControl0", &PMPattern::setQuiltControl0, &PMPattern::quiltControl0 ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "quiltControl1", &PMPattern::setQuiltControl1, &PMPattern::quiltControl1 ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "spiralNumberArms", &PMPattern::setSpiralNumberArms, &PMPattern::spiralNumberArms ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "tilingNumbers", &PMPattern::setTilingNumbers, &PMPattern::tilingNumbers ) );//eticre-tiling

      //s_pMetaObject->addProperty(
      //   new PMPatternProperty( "pigmentPattern", &PMPattern::setLinkedObject, &PMPattern::linkedObject ) );//eticre-ppattern

      s_pMetaObject->addProperty(
         new PMPatternProperty( "turbulence", &PMPattern::enableTurbulence, &PMPattern::isTurbulenceEnabled ) );
      s_pMetaObject->addProperty(
         new PMPatternProperty( "valueVector", &PMPattern::setValueVector, &PMPattern::valueVector ) );
      s_pMetaObject->addProperty(
         new PMPatternProperty( "octaves", &PMPattern::setOctaves, &PMPattern::octaves ) );
      s_pMetaObject->addProperty(
         new PMPatternProperty( "omega", &PMPattern::setOmega, &PMPattern::omega ) );
      s_pMetaObject->addProperty(
         new PMPatternProperty( "lambda", &PMPattern::setLambda, &PMPattern::lambda ) );

      s_pMetaObject->addProperty(
         new PMPatternProperty( "depth", &PMPattern::setDepth, &PMPattern::depth ) );

      PMPatternTypeProperty* p = new PMPatternTypeProperty(
         "patternType", &PMPattern::setPatternType, &PMPattern::patternType );
      p->addEnumValue( "Agate", PatternAgate );
      p->addEnumValue( "Average", PatternAverage );
      p->addEnumValue( "Boxed", PatternBoxed );
      p->addEnumValue( "Bozo", PatternBozo );
      p->addEnumValue( "Bumps", PatternBumps );
      p->addEnumValue( "Cells", PatternCells );
      p->addEnumValue( "Crackle", PatternCrackle );
      p->addEnumValue( "Cylindrical", PatternCylindrical );
      p->addEnumValue( "Density", PatternDensity );
      p->addEnumValue( "Dents", PatternDents );
      p->addEnumValue( "Gradient", PatternGradient );
      p->addEnumValue( "Granite", PatternGranite );
      p->addEnumValue( "Julia", PatternJulia );
      p->addEnumValue( "Leopard", PatternLeopard );
      p->addEnumValue( "Mandel", PatternMandel );
      p->addEnumValue( "Marble", PatternMarble );
      p->addEnumValue( "Onion", PatternOnion );
      p->addEnumValue( "Planar", PatternPlanar );
      p->addEnumValue( "Quilted", PatternQuilted );
      p->addEnumValue( "Radial", PatternRadial );
      p->addEnumValue( "Ripples", PatternRipples );
      p->addEnumValue( "Slope", PatternSlope );
      p->addEnumValue( "Spherical", PatternSpherical );
      p->addEnumValue( "Spiral1", PatternSpiral1 );
      p->addEnumValue( "Spiral2", PatternSpiral2 );
      p->addEnumValue( "Spotted", PatternSpotted );
      p->addEnumValue( "Waves", PatternWaves );
      p->addEnumValue( "Wood", PatternWood );
      p->addEnumValue( "Wrinkles", PatternWrinkles );
      p->addEnumValue( "Tiling", PatternTiling );//eticre-tiling-29
      p->addEnumValue( "Pigment_Pattern", PatternPigment );
      s_pMetaObject->addProperty( p );

      PMNoiseProperty* p2 = new PMNoiseProperty(
         "noiseGenerator", &PMPattern::setNoiseGenerator, &PMPattern::noiseGenerator );
      p2->addEnumValue( "GlobalSetting", GlobalSetting );
      p2->addEnumValue( "Original", Original );
      p2->addEnumValue( "RangeCorrected", RangeCorrected );
      p2->addEnumValue( "Perlin", Perlin );
      s_pMetaObject->addProperty( p2 );
   }
   return s_pMetaObject;
}

void PMPattern::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMPattern::description() const
{
   return ( "pattern" );
}

void PMPattern::setPatternType( PMPatternType c )
{
   if( c != m_patternType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPatternTypeID, m_patternType );
      m_patternType = c;
   }
}

void PMPattern::setAgateTurbulence( double c )
{
   if( c != m_agateTurbulence )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAgateTurbulenceID, m_agateTurbulence );
      m_agateTurbulence = c;
   }
}

void PMPattern::setCrackleForm( const PMVector& v )
{
   if ( v != m_crackleForm )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCrackleFormID, m_crackleForm );
      m_crackleForm = v;
   }
}

void PMPattern::setCrackleMetric( int c )
{
   if ( c < 1 )
   {
      qCritical(  ) << "new metric is < 1 in PMPattern::setCrackleMetric\n";
      c = 1;
   }

   if ( c != m_crackleMetric )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCrackleMetricID, m_crackleMetric );
      m_crackleMetric = c;
   }
}

void PMPattern::setCrackleOffset( double c )
{
   if ( c != m_crackleOffset )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCrackleOffsetID, m_crackleOffset );
      m_crackleOffset = c;
   }
}

void PMPattern::setCrackleSolid( bool c )
{
   if ( c != m_crackleSolid )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCrackleSolidID, m_crackleSolid );
      m_crackleSolid = c;
   }
}

void PMPattern::setDensityInterpolate( int c )
{
   if( c != m_densityInterpolate )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDensityInterpolateID, m_densityInterpolate );
      m_densityInterpolate = c;
   }
}

void PMPattern::setDensityFile( const QString& s )
{
   if( s != m_densityFile )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDensityFileID, m_densityFile );
      m_densityFile = s;
   }
}

void PMPattern::setGradient( const PMVector& v )
{
   if( v != m_gradient )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMGradientID, m_gradient );
      m_gradient = v;
   }
}

void PMPattern::setJuliaComplex( const PMVector& v )
{
   if ( v != m_juliaComplex )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMJuliaComplexID, m_juliaComplex );
      m_juliaComplex = v;
   }
}

void PMPattern::setFractalMagnet( bool c )
{
   if ( c != m_fractalMagnet )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFractalMagnetID, m_fractalMagnet );
      m_fractalMagnet = c;
   }
}

void PMPattern::setFractalMagnetType( int c )
{
   if ( c < 1 )
   {
      qCritical(  ) << "Magnet type < 1 in PMPattern::setFractalMagnetType\n";
      c = 1;
   }

   if ( c > 2 )
   {
      qCritical(  ) << "Magnet type > 2 in PMPattern::setFractalMagnetType\n";
      c = 2;
   }

   if ( c != m_fractalMagnetType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFractalMagnetTypeID, m_fractalMagnetType );
      m_fractalMagnetType = c;
   }
}

void PMPattern::setMaxIterations( int c )
{
   if( c != m_maxIterations )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMaxIterationsID, m_maxIterations );
      m_maxIterations = c;
   }
}

void PMPattern::setFractalExponent( int c )
{
   if ( c < 2 )
   {
      qCritical(  ) << "Exponent < 2 in PMPattern::setFractalExponent\n";
      c = 2;
   }

   if ( c > 33 )
   {
      qCritical(  ) << "Exponent > 33 in PMPattern::setFractalExponent\n";
      c = 33;
   }

   if ( c != m_fractalExponent )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFractalExponentID, m_fractalExponent );
      m_fractalExponent = c;
   }
}

void PMPattern::setFractalExtType( int c )
{
   if ( c < 0 )
   {
      qCritical(  ) << "Exterior Type < 0 in PMPattern::setFractalExtType\n";
      c = 0;
   }

   if ( c > 6 )
   {
      qCritical(  ) << "Exterior Type > 6 in PMPattern::setFractalExtType\n";
      c = 6;
   }

   if ( c != m_fractalExtType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFractalExtTypeID, m_fractalExtType );
      m_fractalExtType = c;
   }
}

void PMPattern::setFractalExtFactor( double c )
{
   if ( c != m_fractalExtFactor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFractalExtFactorID, m_fractalExtFactor );
      m_fractalExtFactor = c;
   }
}

void PMPattern::setFractalIntType( int c )
{
   if ( c < 0 )
   {
      qCritical(  ) << "Interior Type < 0 in PMPattern::setFractalIntType\n";
      c = 0;
   }

   if ( c > 6 )
   {
      qCritical(  ) << "Interior Type > 6 in PMPattern::setFractalIntType\n";
      c = 6;
   }

   if ( c != m_fractalIntType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFractalIntTypeID, m_fractalIntType );
      m_fractalIntType = c;
   }
}

void PMPattern::setFractalIntFactor( double c )
{
   if ( c != m_fractalIntFactor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFractalIntFactorID, m_fractalIntFactor );
      m_fractalIntFactor = c;
   }
}

void PMPattern::setQuiltControl0( double c )
{
   if( c != m_quiltControl0 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMQuiltControl0ID, m_quiltControl0 );
      m_quiltControl0 = c;
   }
}

void PMPattern::setQuiltControl1( double c )
{
   if( c != m_quiltControl1 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMQuiltControl1ID, m_quiltControl1 );
      m_quiltControl1 = c;
   }
}

void PMPattern::setSlopeDirection( const PMVector& v )
{
   if ( v != m_slopeDirection )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeDirectionID, m_slopeDirection );
      m_slopeDirection = v;
   }
}

void PMPattern::setSlopePointAt( const PMVector& v )
{
   if ( v != m_slopePointAt )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopePointAtID, m_slopePointAt );
      m_slopePointAt = v;
   }
}

void PMPattern::setSlopePointAtFlag( bool c )
{
   if ( c != m_slopePointAtFlag )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopePointAtFlagID, m_slopePointAtFlag );
      m_slopePointAtFlag = c;
   }
}

void PMPattern::setSlopeLoSlope( double c )
{
   if ( c < 0.0 )
   {
      qCritical(  ) << "Low slope < 0.0 in PMPattern::setSlopeLoSlope\n";
      c = 0.0;
   }

   if ( c > 1.0 )
   {
      qCritical(  ) << "Low slope > 1.0 in PMPattern::setSlopeLoSlope\n";
      c = 1.0;
   }

   if ( c != m_slopeLoSlope )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeLoSlopeID, m_slopeLoSlope );
      m_slopeLoSlope = c;
   }
}

void PMPattern::setSlopeHiSlope( double c )
{
   if ( c < 0.0 )
   {
      qCritical(  ) << "High slope < 0.0 in PMPattern::setSlopeHiSlope\n";
      c = 0.0;
   }

   if ( c > 1.0 )
   {
      qCritical(  ) << "High slope > 1.0 in PMPattern::setSlopeHiSlope\n";
      c = 1.0;
   }

   if ( c != m_slopeHiSlope )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeHiSlopeID, m_slopeHiSlope );
      m_slopeHiSlope = c;
   }
}

void PMPattern::setSlopeAltFlag( bool c )
{
   if ( c != m_slopeAltFlag )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeAltFlagID, m_slopeAltFlag );
      m_slopeAltFlag = c;
   }
}

void PMPattern::setSlopeAltitude( const PMVector& v )
{
   if ( v != m_slopeAltitude )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeAltitudeID, m_slopeAltitude );
      m_slopeAltitude = v;
   }
}

void PMPattern::setSlopeLoAlt( double c )
{
   if ( c != m_slopeLoAlt )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeLoAltID, m_slopeLoAlt );
      m_slopeLoAlt = c;
   }
}

void PMPattern::setSlopeHiAlt( double c )
{
   if ( c != m_slopeHiAlt )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSlopeHiAltID, m_slopeHiAlt );
      m_slopeHiAlt = c;
   }
}

void PMPattern::setSpiralNumberArms( int c )
{
   if( c != m_spiralNumberArms )
   {
      if(m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSpiralNumberArmsID, m_spiralNumberArms );
      m_spiralNumberArms = c;
   }
}

void PMPattern::setTilingNumbers( int c )
{
   if( c != m_tilingNumbers )
   {
      if(m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMTilingNumbersID, m_tilingNumbers );
      m_tilingNumbers = c;
   }
}

void PMPattern::setNoiseGenerator( PMNoiseType c )
{
   if( c != m_noiseGenerator )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNoiseGeneratorID, m_noiseGenerator );
      m_noiseGenerator = c;
   }
}

void PMPattern::enableTurbulence( bool c )
{
   if( c != m_enableTurbulence )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableTurbulenceID, m_enableTurbulence );
      m_enableTurbulence = c;
   }
}

void PMPattern::setValueVector( const PMVector& c )
{
   if( c != m_valueVector )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMValueVectorID, m_valueVector );
      m_valueVector = c;
   }
}

void PMPattern::setOctaves( const int c )
{
   if( c != m_octaves )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOctavesID, m_octaves );
      m_octaves = c;
   }
}

void PMPattern::setOmega( const double c )
{
   if( c != m_omega )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOmegaID, m_omega );
      m_omega = c;
   }
}

void PMPattern::setLambda( const double c )
{
   if( c != m_lambda )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLambdaID, m_lambda );
      m_lambda = c;
   }
}

void PMPattern::setDepth( const double c )
{
   if( c != m_depth )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDepthID, m_depth );
      m_depth = c;
   }
}

PMDialogEditBase* PMPattern::editWidget( QWidget* parent ) const
{
   return new PMPatternEdit( parent );
}

void PMPattern::restoreMemento( PMMemento* s )
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
            case PMPatternTypeID:
               setPatternType( ( PMPatternType )data->intData() );
               break;

            case PMAgateTurbulenceID:
               setAgateTurbulence( data->doubleData() );
               break;

            case PMCrackleFormID:
               setCrackleForm( data->vectorData() );
               break;
            case PMCrackleMetricID:
               setCrackleMetric( data->intData() );
               break;
            case PMCrackleOffsetID:
               setCrackleOffset( data->doubleData() );
               break;
            case PMCrackleSolidID:
               setCrackleSolid( data->boolData() );
               break;

            case PMDensityInterpolateID:
               setDensityInterpolate( data->intData() );
               break;
            case PMDensityFileID:
               setDensityFile( data->stringData() );
               break;

            case PMGradientID:
               setGradient( data->vectorData() );
               break;

            case PMJuliaComplexID:
               setJuliaComplex( data->vectorData() );
               break;
            case PMFractalMagnetID:
               setFractalMagnet( data->boolData() );
               break;
            case PMFractalMagnetTypeID:
               setFractalMagnetType( data->intData() );
               break;
            case PMMaxIterationsID:
               setMaxIterations( data->intData() );
               break;
            case PMFractalExponentID:
               setFractalExponent( data->intData() );
               break;
            case PMFractalExtTypeID:
               setFractalExtType( data->intData() );
               break;
            case PMFractalExtFactorID:
               setFractalExtFactor( data->doubleData() );
               break;
            case PMFractalIntTypeID:
               setFractalIntType( data->intData() );
               break;
            case PMFractalIntFactorID:
               setFractalIntFactor( data->doubleData() );
               break;

            case PMQuiltControl0ID:
               setQuiltControl0( data->doubleData() );
               break;
            case PMQuiltControl1ID:
               setQuiltControl1( data->doubleData() );
               break;

            case PMSlopeDirectionID:
               setSlopeDirection( data->vectorData() );
               break;
            case PMSlopePointAtID:
               setSlopePointAt( data->vectorData() );
               break;
            case PMSlopeLoSlopeID:
               setSlopeLoSlope( data->doubleData() );
               break;
            case PMSlopeHiSlopeID:
               setSlopeHiSlope( data->doubleData() );
               break;
            case PMSlopeAltFlagID:
               setSlopeAltFlag( data->boolData() );
               break;
            case PMSlopeAltitudeID:
               setSlopeAltitude( data->boolData() );
               break;
            case PMSlopeLoAltID:
               setSlopeLoAlt( data->intData() );
               break;
            case PMSlopeHiAltID:
               setSlopeHiAlt( data->intData() );
               break;

            case PMSpiralNumberArmsID:
               setSpiralNumberArms( data->intData() );
               break;

            case PMNoiseGeneratorID:
               setNoiseGenerator( ( PMNoiseType ) ( data->intData() ) );
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
            case PMTilingNumbersID:
               setTilingNumbers( data->intData() );
               break;
            case PMPigmentPatternID:
               setPigmentPattern( ( PMDeclare* ) data->objectData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMPattern::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMPattern::setPigmentPattern(PMDeclare* o )
{
   if( o )
   {
       if( o->declareType() == "Pigment" )
      {
         if( m_pLinkedObject != o )
         {
            if( m_pMemento )
            {
               m_pMemento->addData( s_pMetaObject, PMPigmentPatternID,
                                    m_pLinkedObject );
               m_pMemento->setViewStructureChanged();
            }

            if( m_pLinkedObject )
            {
               m_pLinkedObject->removeLinkedObject( this );
               if( m_pMemento )
                  m_pMemento->addChangedObject( m_pLinkedObject, PMCData );
            }
            m_pLinkedObject = o;
            m_pLinkedObject->addLinkedObject( this );
            if( m_pMemento )
               m_pMemento->addChangedObject( m_pLinkedObject, PMCData );
         }
         //return true;
      }
   }
   else
   {
      if( m_pLinkedObject != 0 )
      {
         if( m_pMemento )
         {
            m_pMemento->addData( s_pMetaObject, PMPigmentPatternID,
                                 m_pLinkedObject );
            m_pMemento->addChangedObject( m_pLinkedObject, PMCData );
         }
         m_pLinkedObject->removeLinkedObject( this );
         m_pLinkedObject = 0;
      }
      //return true;
   }
   //return false;
}

