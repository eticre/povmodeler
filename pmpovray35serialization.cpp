/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmpovray35serialization.h"
#include "pmoutputdevice.h"

#include "pmisosurface.h"
#include "pmlight.h"
#include "pmprojectedthrough.h"
#include "pmglobalsettings.h"
#include "pmradiosity.h"
#include "pmglobalphotons.h"
#include "pmphotons.h"
#include "pminterior.h"
#include "pmlightgroup.h"
#include "pmpattern.h"
#include "pmnormal.h"
#include "pminteriortexture.h"
#include "pmwarp.h"
#include "pmspheresweep.h"
#include "pmfinish.h"
#include "pmmesh.h"
#include "pmmedia.h"
#include "pmgraphicalobject.h"
#include "pmpigment.h"
#include "pmtexture.h"
#include "pmbicubicpatch.h"
#include "pmtriangle.h"
#include <QList>

const PMIsoSurface::ContainedByType c_defaultIsoContainedBy = PMIsoSurface::Box;
const PMVector c_defaultIsoCorner1 = PMVector( -1, -1, -1 );
const PMVector c_defaultIsoCorner2 = PMVector( 1, 1, 1 );
const PMVector c_defaultIsoCenter = PMVector( 0, 0, 0 );
const double c_defaultIsoRadius = 1;

const double c_defaultIsoThreshold = 0.0;
const double c_defaultIsoAccuracy = 0.001;
const double c_defaultIsoMaxGradient = 1.1;
const bool c_defaultIsoEvaluate = false;
const double c_defaultIsoEvaluate0 = 5;
const double c_defaultIsoEvaluate1 = 1.2;
const double c_defaultIsoEvaluate2 = 0.95;
const double c_defaultIsoOpen = false;
const int c_defaultIsoMaxTrace = 1;
const bool c_defaultIsoAllIntersections = false;

void PMPov35SerIsoSurface( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMIsoSurface* o = ( PMIsoSurface* ) object;
   QString str;

   dev->objectBegin( "isosurface" );
   dev->writeName( object->name() );

   dev->writeLine( QString( "function { " ) + o->function() + " }" );

   if( o->containedBy() == PMIsoSurface::Box )
   {
      if( o->corner1() != c_defaultIsoCorner1 ||
          o->corner2() != c_defaultIsoCorner2 )
      {
         dev->writeLine( QString( "contained_by { box { " ) +
                         o->corner1().serialize() + ", " +
                         o->corner2().serialize() + " } }" );
      }
   }
   else
   {
      str.setNum( o->radius() );
      dev->writeLine( QString( "contained_by { sphere { " ) +
                      o->center().serialize() + ", " + str + " } }" );
   }

   if( !approx( o->threshold(), c_defaultIsoThreshold ) )
   {
      str.setNum( o->threshold() );
      dev->writeLine( "threshold " + str );
   }
   if( !approx( o->accuracy(), c_defaultIsoAccuracy ) )
   {
      str.setNum( o->accuracy() );
      dev->writeLine( "accuracy " + str );
   }
   if( !approx( o->maxGradient(), c_defaultIsoMaxGradient ) )
   {
      str.setNum( o->maxGradient() );
      dev->writeLine( "max_gradient " + str );
   }
   if( o->evaluate() )
   {
      str = QString( "%1, %2, %3" ).arg( o->evaluateValue( 0 ) )
            .arg( o->evaluateValue( 1 ) ).arg( o->evaluateValue( 2 ) );
      dev->writeLine( "evaluate " + str );
   }
   if( o->allIntersections() )
      dev->writeLine( "all_intersections" );
   else
   {
      str.setNum( o->maxTrace() );
      dev->writeLine( "max_trace " + str );
   }
   if( o->open() )
      dev->writeLine( "open" );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

const double c_defaultLightTightness = 10;
const int c_defaultLightAdaptive = 0;

void PMPov35SerLight( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMLight* o = ( PMLight* ) object;

   dev->objectBegin( QString( "light_source" ) );

   dev->writeName( object->name() );
   dev->writeLine( o->location().serialize() + ", " + o->color().serialize() );

   if( o->lightType() == PMLight::SpotLight )
      dev->writeLine( QString( "spotlight" ) );
   else if( o->lightType() == PMLight::CylinderLight )
      dev->writeLine( QString( "cylinder" ) );
   else if( o->lightType() == PMLight::ShadowlessLight )
      dev->writeLine( QString( "shadowless" ) );

   if( o->parallel() )
      dev->writeLine( QString( "parallel" ) );

   if( ( o->lightType() == PMLight::SpotLight ) ||
       ( o->lightType() == PMLight::CylinderLight ) )
   {
      dev->writeLine( QString( "radius %1" ).arg( o->radius() ) );
      dev->writeLine( QString( "falloff %1" ).arg( o->falloff() ) );
      if( o->tightness() != c_defaultLightTightness )
         dev->writeLine( QString( "tightness %1" ).arg( o->tightness() ) );
      dev->writeLine( QString( "point_at " ) + o->pointAt().serialize() );
   }

   if( o->isAreaLight() )
   {
      dev->writeLine( QString( "area_light " ) + o->axis1().serialize()
                     + QString( ", " ) + o->axis2().serialize()
                     + QString( ", %1, %2" ).arg( o->size1() ).arg( o->size2() ) );
      if( o->adaptive() != c_defaultLightAdaptive )
         dev->writeLine( QString( "adaptive %1" ).arg( o->adaptive() ) );
      if( o->jitter() )
         dev->writeLine( QString( "jitter" ) );
      if ( o->areaType() == PMLight::Circular )
         dev->writeLine( QString( "circular" ) );
      if ( o->orient() )
         dev->writeLine( QString( "orient" ) );
   }

   if( o->fading() )
   {
      dev->writeLine( QString( "fade_distance %1" ).arg( o->fadeDistance() ) );
      dev->writeLine( QString( "fade_power %1" ).arg( o->fadePower() ) );
   }

   if( !o->mediaInteraction() )
      dev->writeLine( QString( "media_interaction off" ) );
   if( !o->mediaAttenuation() )
      dev->writeLine( QString( "media_attenuation off" ) );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov35SerProjectedThrough( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "projected_through" );
   dev->writeName( object->name() );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

const double c_defaultGlobalSettingsAdcBailout = 1.0 / 255.0;
const PMColor c_defaultGlobalSettingsAmbientLight = PMColor( 1.0, 1.0, 1.0, 0.0, 0.0 );
const double c_defaultGlobalSettingsAssumedGamma = 0.0;
const bool c_defaultGlobalSettingsHfGray16 = false;
const PMColor c_defaultGlobalSettingsIridWaveLength = PMColor( 0.25, 0.18, 0.14, 0.0, 0.0 );
const int c_defaultGlobalSettingsMaxIntersections = 0; // ???
const int c_defaultGlobalSettingsMaxTraceLevel = 0;   // ???
const int c_defaultGlobalSettingsNumberWaves = 10;
const bool c_defaultGlobalSettingsRadiosity = false;
const double c_defaultGlobalSettingsBrightness = 1.0;
const int c_defaultGlobalSettingsCount = 35;
const double c_defaultGlobalSettingsDistanceMaximum = 0; // ???
const double c_defaultGlobalSettingsErrorBound = 1.8;
const double c_defaultGlobalSettingsGrayThreshold = 0.0;
const double c_defaultGlobalSettingsLowErrorFactor = 0.5;
const double c_defaultGlobalSettingsMinimumReuse = 0.015;
const int c_defaultGlobalSettingsNearestCount = 5;
const int c_defaultGlobalSettingsRecursionLimit = 2;

void PMPov35SerGlobalSettings( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMGlobalSettings* o = ( PMGlobalSettings* ) object;

   QString str1;

   dev->objectBegin( "global_settings" );

   if( o->adcBailout() != c_defaultGlobalSettingsAdcBailout )
   {
      str1.setNum( o->adcBailout() );
      dev->writeLine( "adc_bailout " + str1 );
   }
   if( o->ambientLight() != c_defaultGlobalSettingsAmbientLight )
      dev->writeLine( "ambient_light " + o->ambientLight().serialize() );
   if( o->assumedGamma() != c_defaultGlobalSettingsAssumedGamma )
   {
      str1.setNum( o->assumedGamma() );
      dev->writeLine( "assumed_gamma " + str1 );
   }
   if( o->hfGray16() != c_defaultGlobalSettingsHfGray16 )
   {
      if( o->hfGray16() )
         dev->writeLine( "hf_gray_16 on" );
      else
         dev->writeLine( "hf_gray_16 off" );
   }
   if( o->iridWaveLength() != c_defaultGlobalSettingsIridWaveLength )
      dev->writeLine( "irid_wavelength " + o->iridWaveLength().serialize() );
   if( o->maxTraceLevel() != c_defaultGlobalSettingsMaxTraceLevel )
   {
      str1.setNum( o->maxTraceLevel() );
      dev->writeLine( "max_trace_level " + str1 );
   }
   if( o->maxIntersections() != c_defaultGlobalSettingsMaxIntersections )
   {
      str1.setNum( o->maxIntersections() );
      dev->writeLine( "max_intersections " + str1 );
   }
   if( o->numberWaves() != c_defaultGlobalSettingsNumberWaves )
   {
      str1.setNum( o->numberWaves() );
      dev->writeLine( "number_of_waves " + str1 );
   }
   if ( o->noiseGenerator() == PMGlobalSettings::Original )
      dev->writeLine( QString( "noise_generator 1" ) );
   else if ( o->noiseGenerator() == PMGlobalSettings::RangeCorrected )
      dev->writeLine( QString( "noise_generator 2" ) );
   else
      dev->writeLine( QString( "noise_generator 3" ) );
   if( o->isRadiosityEnabled() )//only if old radiosity 3.1 enbled
   {
      dev->objectBegin( "radiosity" );
      if( o->brightness() != c_defaultGlobalSettingsBrightness )
      {
         str1.setNum( o->brightness() );
         dev->writeLine( "brightness " + str1 );
      }
      if( o->count() != c_defaultGlobalSettingsCount )
      {
         str1.setNum( o->count() );
         dev->writeLine( "count " + str1 );
      }
      if( o->distanceMaximum() != c_defaultGlobalSettingsDistanceMaximum )
      {
         str1.setNum( o->distanceMaximum() );
         dev->writeLine( "distance_maximum " + str1 );
      }
      if( o->errorBound() != c_defaultGlobalSettingsErrorBound )
      {
         str1.setNum( o->errorBound() );
         dev->writeLine( "error_bound " + str1 );
      }
      if( o->grayThreshold() != c_defaultGlobalSettingsGrayThreshold )
      {
         str1.setNum( o->grayThreshold() );
         dev->writeLine( "gray_threshold " + str1 );
      }
      if( o->lowErrorFactor() != c_defaultGlobalSettingsLowErrorFactor )
      {
         str1.setNum( o->lowErrorFactor() );
         dev->writeLine( "low_error_factor " + str1 );
      }
      if( o->minimumReuse() != c_defaultGlobalSettingsMinimumReuse )
      {
         str1.setNum( o->minimumReuse() );
         dev->writeLine( "minimuo->reuse() " + str1 );
      }
      if( o->nearestCount() != c_defaultGlobalSettingsNearestCount )
      {
         str1.setNum( o->nearestCount() );
         dev->writeLine( "nearest_count " + str1 );
      }
      if( o->recursionLimit() != c_defaultGlobalSettingsRecursionLimit )
      {
         str1.setNum( o->recursionLimit() );
         dev->writeLine( "recursion_limit " + str1 );
      }
      dev->objectEnd();
   }
   else/*eticre fail to serialize radiosity if is not enabled*/
      dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

const double c_defaultRadiosityAdcBailout = 0.01;
const double c_defaultRadiosityBrightness = 1.0;
const int c_defaultRadiosityCount = 35;
const double c_defaultRadiosityErrorBound = 1.8;
const double c_defaultRadiosityGrayThreshold = 0.0;
const double c_defaultRadiosityLowErrorFactor = 0.5;
const double c_defaultRadiosityMaxSample = -1.0;
const double c_defaultRadiosityMinimumReuse = 0.015;
const int c_defaultRadiosityNearestCount = 5;
const double c_defaultRadiosityPretraceStart = 0.08;
const double c_defaultRadiosityPretraceEnd = 0.04;
const int c_defaultRadiosityRecursionLimit = 2;

void PMPov35SerRadiosity( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{

    PMRadiosity* o = ( PMRadiosity* ) object;

   QString str1;

   dev->objectBegin( "radiosity" );

   if( o->adcBailout() != c_defaultRadiosityAdcBailout )
   {
      str1.setNum( o->adcBailout() );
      dev->writeLine( "adc_bailout " + str1 );
   }

   if( !o->alwaysSample() )
      dev->writeLine( "always_sample off" );

   if( o->brightness() != c_defaultRadiosityBrightness )
   {
      str1.setNum( o->brightness() );
      dev->writeLine( "brightness " + str1 );
   }

   if( o->count() != c_defaultRadiosityCount )
   {
      str1.setNum( o->count() );
      dev->writeLine( "count " + str1 );
   }

   if( o->errorBound() != c_defaultRadiosityErrorBound )
   {
      str1.setNum( o->errorBound() );
      dev->writeLine( "error_bound " + str1 );
   }

   if( o->grayThreshold() != c_defaultRadiosityGrayThreshold )
   {
      str1.setNum( o->grayThreshold() );
      dev->writeLine( "gray_threshold " + str1 );
   }

   if( o->lowErrorFactor() != c_defaultRadiosityLowErrorFactor )
   {
      str1.setNum( o->lowErrorFactor() );
      dev->writeLine( "low_error_factor " + str1 );
   }

   if( o->maxSample() != c_defaultRadiosityMaxSample )
   {
      str1.setNum( o->maxSample() );
      dev->writeLine( "max_sample " + str1 );
   }

   if( o->media() )
      dev->writeLine( "media on" );

   if( o->minimumReuse() != c_defaultRadiosityMinimumReuse )
   {
      str1.setNum( o->minimumReuse() );
      dev->writeLine( "minimum_reuse " + str1 );
   }
   if( o->nearestCount() != c_defaultRadiosityNearestCount )
   {
      str1.setNum( o->nearestCount() );
      dev->writeLine( "nearest_count " + str1 );
   }

   if( o->normal() )
      dev->writeLine( "normal on" );

   if( o->pretraceStart() != c_defaultRadiosityPretraceStart )
   {
      str1.setNum( o->pretraceStart() );
      dev->writeLine( "pretrace_start " + str1 );
   }

   if( o->pretraceEnd() != c_defaultRadiosityPretraceEnd )
   {
      str1.setNum( o->pretraceEnd() );
      dev->writeLine( "pretrace_end " + str1 );
   }

   if( o->recursionLimit() != c_defaultGlobalSettingsRecursionLimit )
   {
      str1.setNum( o->recursionLimit() );
      dev->writeLine( "recursion_limit " + str1 );
   }
   dev->objectEnd();
}

const int c_defaultGlobalPhotonsGatherMin = 20;
const int c_defaultGlobalPhotonsGatherMax = 100;
const int c_defaultGlobalPhotonsMediaMaxSteps = 0;
const double c_defaultGlobalPhotonsMediaFactor = 1.0;
const double c_defaultGlobalPhotonsJitter = 0.4;
const double c_defaultGlobalPhotonsAutostop = 0.0;
const double c_defaultGlobalPhotonsExpandIncrease = 0.2;
const int c_defaultGlobalPhotonsExpandMin = 40;
const double c_defaultGlobalPhotonsRadiusGather = 0.0;
const double c_defaultGlobalPhotonsRadiusGatherMulti = 1.0;
const double c_defaultGlobalPhotonsRadiusMedia = 0.0;
const double c_defaultGlobalPhotonsRadiusMediaMulti = 1.0;

void PMPov35SerGlobalPhotons( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMGlobalPhotons* o = ( PMGlobalPhotons* ) object;

   QString str1, str2;

   dev->objectBegin( "photons" );

   if ( o->numberType() == PMGlobalPhotons::Spacing )
   {
      str1.setNum( o->spacing() );
      dev->writeLine( "spacing " + str1 );
   }
   else
   {
      str1.setNum( o->count() );
      dev->writeLine( "count " + str1 );
   }

   if ( o->gatherMin() != c_defaultGlobalPhotonsGatherMin ||
        o->gatherMax() != c_defaultGlobalPhotonsGatherMax )
   {
      str1.setNum( o->gatherMin() );
      str2.setNum( o->gatherMax() );
      dev->writeLine( "gather " + str1 + ", " + str2 );
   }

   if ( o->mediaMaxSteps() != c_defaultGlobalPhotonsMediaMaxSteps )
   {
      str1.setNum( o->mediaMaxSteps() );
      if ( o->mediaFactor() != c_defaultGlobalPhotonsMediaFactor )
      {
         str2.setNum( o->mediaFactor() );
         dev->writeLine( "media " + str1 + ", " + str2 );
      }
      else
         dev->writeLine( "media " + str1 );
   }

   if ( o->jitter() != c_defaultGlobalPhotonsJitter )
   {
      str1.setNum( o->jitter() );
      dev->writeLine( "jitter " + str1 );
   }

   if ( !o->maxTraceLevelGlobal() )
   {
      str1.setNum( o->maxTraceLevel() );
      dev->writeLine( "max_trace_level " + str1 );
   }

   if ( !o->adcBailoutGlobal() )
   {
      str1.setNum( o->adcBailout() );
      dev->writeLine( "adc_bailout " + str1 );
   }

   if ( o->autostop() != c_defaultGlobalPhotonsAutostop )
   {
      str1.setNum( o->autostop() );
      dev->writeLine( "autostop " + str1 );
   }

   if ( o->expandIncrease() != c_defaultGlobalPhotonsExpandIncrease ||
        o->expandMin() != c_defaultGlobalPhotonsExpandMin )
   {
      str1.setNum( o->expandIncrease() );
      str2.setNum( o->expandMin() );
      dev->writeLine( "expand_thresholds " + str1 + ", " + str2 );
   }

   if ( o->radiusGather() != c_defaultGlobalPhotonsRadiusGather ||
        o->radiusGatherMulti() != c_defaultGlobalPhotonsRadiusGatherMulti ||
        o->radiusMedia() != c_defaultGlobalPhotonsRadiusMedia ||
        o->radiusMediaMulti() != c_defaultGlobalPhotonsRadiusMediaMulti )
   {
      QString str3, str4;
      str1.setNum( o->radiusGather() );
      str2.setNum( o->radiusGatherMulti() );
      str3.setNum( o->radiusMedia() );
      str4.setNum( o->radiusMediaMulti() );
      dev->writeLine( "radius " + str1 + ", " + str2 + ", " + str3 + ", " + str4 );
   }
   dev->objectEnd();
}

const double c_defaultPhotonsSpacingMulti = 1.0;

void PMPov35SerPhotons( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMPhotons* o = ( PMPhotons* ) object;

   QString str1;

   dev->objectBegin( "photons" );

   if( o->parent() && ( o->parent()->type() == "Light" ) )
   {
      if( o->refraction() )
         dev->writeLine( QString( "refraction on" ) );
      if( o->reflection() )
         dev->writeLine( QString( "reflection on" ) );
      if( o->areaLight() )
         dev->writeLine( QString( "area_light" ) );
   }
   else
   {
      if( o->target() )
      {
         if( o->spacingMulti() != c_defaultPhotonsSpacingMulti )
         {
            str1.setNum( o->spacingMulti() );
            dev->writeLine( "target " + str1 );
         }
         else
            dev->writeLine( QString( "target" ) );
      }
      if( o->refraction() )
         dev->writeLine( QString( "refraction on" ) );
      if( o->reflection() )
         dev->writeLine( QString( "reflection on" ) );
      if( !o->collect() )
         dev->writeLine( QString( "collect off" ) );
      if( o->passThrough() )
         dev->writeLine( QString( "pass_through" ) );
   }
   dev->objectEnd();
}

void PMPov35SerInterior( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMInterior* o = ( PMInterior* ) object;

   QString str1;

   dev->objectBegin( "interior" );
   dev->callSerialization( object, metaObject->superClass() );

   if( o->isIorEnabled() )
   {
      str1.setNum( o->ior() );
      dev->writeLine( "ior " + str1 );
   }
   if( o->isCausticsEnabled() )
   {
      str1.setNum( o->caustics() );
      dev->writeLine( "caustics " + str1 );
   }
   if ( o->isDispersionEnabled() )
   {
      str1.setNum( o->dispersion() );
      dev->writeLine( "dispersion " + str1 );
   }
   if ( o->isDispSamplesEnabled() )
   {
      str1.setNum( o->dispSamples() );
      dev->writeLine( "dispersion_samples " + str1 );
   }
   if( o->isFadeDistanceEnabled() )
   {
      str1.setNum( o->fadeDistance() );
      dev->writeLine( "fade_distance " + str1 );
   }
   if( o->isFadePowerEnabled() )
   {
      str1.setNum( o->fadePower() );
      dev->writeLine( "fade_power " + str1 );
   }
   dev->objectEnd();
}

void PMPov35SerLightGroup( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMLightGroup* o = ( PMLightGroup* ) object;

   dev->objectBegin( "light_group" );

   dev->writeName( object->name() );
   dev->callSerialization( object, metaObject->superClass() );

   if ( o->globalLights() )
      dev->writeLine( "global_lights on" );
   else
      dev->writeLine( "global_lights off" );

   dev->objectEnd();
}

const PMVector c_defaultPatternCrackleForm = PMVector( -1.0, 1.0, 0.0 );
const int c_defaultPatternCrackleMetric = 2;
const double c_defaultPatternCrackleOffset = 0.0;
const bool c_defaultPatternCrackleSolid = false;
const int c_defaultPatternFractalExponent = 2;
const int c_defaultPatternFractalExtType = 1;
const double c_defaultPatternFractalExtFactor = 1.0;
const int c_defaultPatternFractalIntType = 0;
const double c_defaultPatternFractalIntFactor = 1.0;
const double c_defaultPatternSlopeLoSlope = 0.0;
const double c_defaultPatternSlopeHiSlope = 1.0;
const double c_defaultPatternSlopeLoAlt = 0.0;
const double c_defaultPatternSlopeHiAlt = 1.0;
const int c_defaultPatternOctaves = 6;
const double c_defaultPatternOmega = 0.5;
const double c_defaultPatternLambda = 2.0;

void PMPov35SerPattern( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMPattern* o = ( PMPattern* ) object;

   QString str, str2;

   // pattern type
   switch( o->patternType() )
   {
      case PMPattern::PatternAgate:
         dev->writeLine( "agate" );
         break;
      case PMPattern::PatternAverage:
         dev->writeLine( "average" );
         break;
      case PMPattern::PatternBoxed:
         dev->writeLine( "boxed" );
         break;
      case PMPattern::PatternBozo:
         dev->writeLine( "bozo" );
         break;
      case PMPattern::PatternBumps:
         dev->writeLine( "bumps" );
         break;
      case PMPattern::PatternCells:
         dev->writeLine( "cells" );
         break;
      case PMPattern::PatternCrackle:
         dev->writeLine( "crackle" );
         break;
      case PMPattern::PatternCylindrical:
         dev->writeLine( "cylindrical" );
         break;
      case PMPattern::PatternDensity:
         dev->writeLine( "density_file df3 \"" + o->densityFile() + "\"");
         break;
      case PMPattern::PatternDents:
         dev->writeLine( "dents" );
         break;
      case PMPattern::PatternGradient:
         dev->writeLine( "gradient " + o->gradient().serialize() );
         break;
      case PMPattern::PatternGranite:
         dev->writeLine( "granite" );
         break;
      case PMPattern::PatternJulia:
         if( o->fractalMagnet() )
         {
            str.setNum( o->fractalMagnetType() );
            str = "magnet " + str + ' ';
         }
         else
            str = "";

         str2.setNum( o->maxIterations() );
         str = str + "julia " + o->juliaComplex().serialize() + ", " + str2;
         dev->writeLine( str );
         break;
      case PMPattern::PatternLeopard:
         dev->writeLine( "leopard" );
         break;
      case PMPattern::PatternMandel:
         if( o->fractalMagnet() )
         {
            str.setNum( o->fractalMagnetType() );
            str = "magnet " + str + ' ';
         }
         else
            str = "";

         str2.setNum( o->maxIterations() );
         dev->writeLine( str + "mandel " + str2 );
         break;
      case PMPattern::PatternMarble:
         dev->writeLine( "marble" );
         break;
      case PMPattern::PatternOnion:
         dev->writeLine( "onion" );
         break;
      case PMPattern::PatternPlanar:
         dev->writeLine( "planar" );
         break;
      case PMPattern::PatternQuilted:
         dev->writeLine( "quilted" );
         break;
      case PMPattern::PatternRadial:
         dev->writeLine( "radial" );
         break;
      case PMPattern::PatternRipples:
         dev->writeLine( "ripples" );
         break;
      case PMPattern::PatternSlope:
         dev->objectBegin( "slope" );
         // point_at
         if ( o->slopePointAtFlag() )
            dev->write( "point_at" + o->slopePointAt().serialize() );
         else
            dev->write( o->slopeDirection().serialize() );

         if ( o->slopeLoSlope() != c_defaultPatternSlopeLoSlope ||
               o->slopeHiSlope() != c_defaultPatternSlopeHiSlope )
         {
            str.setNum( o->slopeLoSlope() );
            str2.setNum( o->slopeHiSlope() );
            dev->writeLine( ", " + str + ", " + str2 );
         }
         else
            dev->writeLine( "" );

         if ( o->slopeAltFlag() )
         {
            dev->write( "altitude " + o->slopeAltitude().serialize() );
            if ( o->slopeLoAltitude() != c_defaultPatternSlopeLoAlt ||
                  o->slopeHiAltitude() != c_defaultPatternSlopeHiAlt )
            {
               str.setNum( o->slopeLoAltitude() );
               str2.setNum( o->slopeHiAltitude() );
               dev->writeLine( ", " + str + ", " + str2 );
            }
            else
               dev->writeLine( "" );
         }
         dev->objectEnd();
         break;
      case PMPattern::PatternSpherical:
         dev->writeLine( "spherical" );
         break;
      case PMPattern::PatternSpiral1:
         str.setNum( o->spiralNumberArms() );
         dev->writeLine( "spiral1 " + str );
         break;
      case PMPattern::PatternSpiral2:
         str.setNum( o->spiralNumberArms() );
         dev->writeLine( "spiral2 " + str );
         break;
      case PMPattern::PatternSpotted:
         dev->writeLine( "spotted" );
         break;
      case PMPattern::PatternTiling:
         str.setNum( o->tilingNumbers() );//eticre tiling
         dev->writeLine( "tiling " + str );
         break;
      case PMPattern::PatternWaves:
         dev->writeLine( "waves" );
         break;
      case PMPattern::PatternWood:
         dev->writeLine( "wood" );
         break;
      case PMPattern::PatternWrinkles:
         dev->writeLine( "wrinkles" );
         break;
      case PMPattern::PatternPigment:
         //normal {pigment_pattern{Pigment_1}, 1.0 }
         if( !o->linkedObject() )
         {
             str="";
         }
         else
         {
             str = o->linkedObject()->id();
             dev->writeLine( "pigment_pattern {" + str + "}" );
          }
         break;
   }
   // depth
   if( o->parent() )
   {
      if( o->depth() && o->parent()->type() == "Normal" )
      {
         str.setNum( o->depth() );
         dev->writeLine( str );
      }
   }
   // modifiers
   switch( o->patternType() )
   {
      case PMPattern::PatternAgate:
         str.setNum( o->agateTurbulence() );
         dev->writeLine( "agate_turb " + str );
         break;
      case PMPattern::PatternCrackle:
         if ( o->crackleForm() != c_defaultPatternCrackleForm )
            dev->writeLine( "form " + o->crackleForm().serialize() );
         if ( o->crackleMetric() != c_defaultPatternCrackleMetric )
         {
            str.setNum( o->crackleMetric() );
            dev->writeLine( "metric " + str );
         }
         if ( o->crackleOffset() != c_defaultPatternCrackleOffset )
         {
            str.setNum( o->crackleOffset() );
            dev->writeLine( "offset " + str );
         }
         if ( o->crackleSolid() )
            dev->writeLine( "solid" );
         break;
      case PMPattern::PatternDensity:
         str.setNum( o->densityInterpolate() );
         dev->writeLine( "interpolate " + str );
         break;
      case PMPattern::PatternJulia:
      case PMPattern::PatternMandel:
         if ( !o->fractalMagnet() && o->fractalExponent() != c_defaultPatternFractalExponent )
         {
            str.setNum( o->fractalExponent() );
            dev->writeLine( "exponent " + str );
         }
         if ( o->fractalExtType() != c_defaultPatternFractalExtType ||
               o->fractalExtFactor() != c_defaultPatternFractalExtFactor )
         {
            str.setNum( o->fractalExtType() );
            str2.setNum( o->fractalExtFactor() );
            dev->writeLine( "exterior " + str + ", " + str2 );
         }
         if ( o->fractalIntType() != c_defaultPatternFractalIntType ||
               o->fractalIntFactor() != c_defaultPatternFractalIntFactor )
         {
            str.setNum( o->fractalIntType() );
            str2.setNum( o->fractalIntFactor() );
            dev->writeLine( "interior " + str + ", " + str2 );
         }
         break;
      case PMPattern::PatternQuilted:
         str.setNum( o->quiltControl0() );
         dev->writeLine( "control0 " + str );
         str.setNum( o->quiltControl1() );
         dev->writeLine( "control1 " + str );
         break;
      case PMPattern::PatternBozo:
      case PMPattern::PatternBumps:
      case PMPattern::PatternGranite:
      case PMPattern::PatternWrinkles:
         switch( o->noiseGenerator() )
         {
            case PMPattern::Original:
               dev->writeLine( QString( "noise_generator 1" ) );
               break;
            case PMPattern::RangeCorrected:
               dev->writeLine( QString( "noise_generator 2" ) );
               break;
            case PMPattern::Perlin:
               dev->writeLine( QString( "noise_generator 3" ) );
               break;
            default:
            break;
         }
         break;
      default:
         break;
   }
   if( o->isTurbulenceEnabled() )
   {
      dev->writeLine( "turbulence " + o->valueVector().serialize() );
      if( o->octaves() != c_defaultPatternOctaves )
      {
         str.setNum( o->octaves() );
         dev->writeLine( "octaves " + str );
      }
      if( o->omega() != c_defaultPatternOmega )
      {
         str.setNum( o->omega() );
         dev->writeLine( "omega " + str );
      }
      if( o->lambda() != c_defaultPatternLambda )
      {
         str.setNum( o->lambda() );
         dev->writeLine( "lambda " + str );
      }
   }
}

const double c_defaultNormalAccuracy = 0.02;

void PMPov35SerNormal( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMNormal* o = ( PMNormal* ) object;

   QString str1;
   bool bObject = true;

   if( o->parent() )
   {
      if( o->parent()->type() == "NormalMap" )
         bObject = false;
   }

   if( bObject )
   {
      dev->objectBegin( "normal" );
      if ( o->uvMapping() )
         dev->writeLine( "uv_mapping" );
   }

   dev->callSerialization( object, metaObject->superClass() );

   if( o->isBumpSizeEnabled() )
   {
      str1.setNum( o->bumpSize() );
      dev->writeLine( "bump_size " + str1 );
   }

   if( o->accuracy() != c_defaultNormalAccuracy )
   {
      str1.setNum( o->accuracy() );
      dev->writeLine( "accuracy " + str1 );
   }

   if( bObject )
      dev->objectEnd();
}

void PMPov35SerInteriorTexture( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "interior_texture" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

const PMVector c_warpDirectionDefault = PMVector( 1.0, 0.0, 0.0 );
const PMVector c_warpOffsetDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector c_warpFlipDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector c_warpLocationDefault = PMVector( 0.0, 0.0, 0.0 );
const double c_warpRadiusDefault = 0;
const double c_warpStrengthDefault = 0;
const double c_warpFalloffDefault = 0;
const bool c_warpInverseDefault = false;
const PMVector c_warpRepeatDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector c_warpTurbulenceDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector c_warpValueVectorDefault = PMVector( 0.0, 0.0, 0.0 );
const int c_warpOctavesDefault = 6;
const double c_warpOmegaDefault = 0.5;
const double c_warpLambdaDefault = 2.0;
const PMVector c_warpOrientationDefault = PMVector( 0.0, 0.0, 1.0 );
const double c_warpDistExpDefault = 0.0;
const double c_warpMajorRadiusDefault = 1.0;

void PMPov35SerWarp( const PMObject* object, const PMMetaObject* , PMOutputDevice* dev )
{
   PMWarp* o = ( PMWarp* ) object;

   QString str1, str2;

   dev->objectBegin( "warp" );
   switch( o->warpType() )
   {
      case PMWarp::Repeat:
         dev->writeLine( "repeat" );
         dev->writeLine( o->direction().serialize() );
         dev->writeLine( "offset " + o->offset().serialize() );
         dev->writeLine( "flip " + o->flip().serialize() );
         break;
      case PMWarp::BlackHole:
         dev->writeLine( "black_hole" );
         dev->writeLine( o->location().serialize() );
         str1.setNum(o->radius());
         dev->writeLine( ", " + str1 );
         if( o->strength() != c_warpStrengthDefault )
         {
            str1.setNum( o->strength());
            dev->writeLine( "strength " + str1 );
         }
         if( o->falloff() != c_warpFalloffDefault )
         {
            str1.setNum( o->falloff());
            dev->writeLine( "falloff " + str1 );
         }
         if( o->inverse() != c_warpInverseDefault )
         {
            if( o->inverse() ) dev->writeLine( "inverse" );
         }
         if( o->repeat() != c_warpRepeatDefault )
         {
            dev->writeLine( "repeat " + o->repeat().serialize() );
         }
         if( o->turbulence() != c_warpTurbulenceDefault )
         {
            dev->writeLine( "turbulence " + o->turbulence().serialize() );
         }
         break;
      case PMWarp::Turbulence:
         dev->writeLine( "turbulence " + o->valueVector().serialize() );
         if( o->octaves() != c_warpOctavesDefault )
         {
            str1.setNum(o->octaves());
            dev->writeLine( "octaves " + str1 );
         }
         if( o->omega() != c_warpOmegaDefault )
         {
            str1.setNum( o->omega() );
            dev->writeLine( "omega " + str1 );
         }
         if( o->lambda() != c_warpLambdaDefault )
         {
            str1.setNum( o->lambda() );
            dev->writeLine( "lambda " + str1 );
         }
         break;
      case PMWarp::Cylindrical:
         dev->writeLine( "cylindrical orientation " + o->orientation().serialize() );// 07-04-2012 eticre add "orientation" word
         if ( o->distExp() != c_warpDistExpDefault )
         {
            str1.setNum( o->distExp() );
            dev->writeLine( "dist_exp " + str1 );
         }
         break;
      case PMWarp::Spherical:
         dev->writeLine( "spherical orientation " + o->orientation().serialize() );
         if ( o->distExp() != c_warpDistExpDefault )
         {
            str1.setNum( o->distExp() );
            dev->writeLine( "dist_exp " + str1 );
         }
         break;
      case PMWarp::Toroidal:
         dev->writeLine( "toroidal orientation " + o->orientation().serialize() );
         if ( o->distExp() != c_warpDistExpDefault )
         {
            str1.setNum( o->distExp() );
            dev->writeLine( "dist_exp " + str1 );
         }
         if ( o->majorRadius() != c_warpMajorRadiusDefault )
         {
            str1.setNum( o->majorRadius() );
            dev->writeLine( "major_radius " + str1 );
         }
         break;
      case PMWarp::Planar:
         str1 = "planar " + o->orientation().serialize();
         if ( o->distExp() != c_warpDistExpDefault )
         {
            str2.setNum( o->distExp() );
            dev->writeLine( str1 + ", " + str2 );
         }
         else
            dev->writeLine( str1 );
         break;
   }

   dev->objectEnd();
}

const double c_sphereSweepToleranceDefault = 1e-6;

void PMPov35SerSphereSweep( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMSphereSweep* o = ( PMSphereSweep* ) object;

   QString str1;
   int numSpheres;
   QList<PMVector> points;
   QList<double> radii;

   dev->objectBegin( "sphere_sweep" );

   switch( o->splineType() )
   {
      case PMSphereSweep::LinearSpline:
         dev->writeLine( QString( "linear_spline," ) );
         break;
      case PMSphereSweep::BSpline:
         dev->writeLine( QString( "b_spline," ) );
         break;
      case PMSphereSweep::CubicSpline:
         dev->writeLine( QString( "cubic_spline," ) );
         break;
   }

   numSpheres = o->numberOfPoints();
   str1.setNum( numSpheres );
   dev->writeLine( str1 + ',' );
   points = o->points();
   radii = o->radii();

   for ( int i = 0; i < numSpheres; ++i )
   {
      str1.setNum( radii[i] );
      dev->writeLine( points[i].serialize() + ',' + str1 );
   }

   if ( o->tolerance() != c_sphereSweepToleranceDefault )
   {
      str1.setNum( o->tolerance() );
      dev->writeLine( "tolerance " + str1 );
   }

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov35SerFinish( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMFinish* o = ( PMFinish* ) object;

   QString str1;

   dev->objectBegin( "finish" );

   dev->callSerialization( object, metaObject->superClass() );

   if( o->isEmissionEnabled() )
      dev->writeLine( "emission " + o->emissionColor().serialize() );
   
   if( o->isAmbientEnabled() )
      dev->writeLine( "ambient " + o->ambientColor().serialize() );

   if( o->isDiffuseEnabled() )
   {
      str1.setNum( o->diffuse() );
      dev->writeLine( "diffuse " + str1 );
   }

   if( o->isBrillianceEnabled() )
   {
      str1.setNum( o->brilliance() );
      dev->writeLine( "brilliance " + str1 );
   }

   if( o->isPhongEnabled() )
   {
      str1.setNum( o->phong() );
      dev->writeLine( "phong " + str1 );
   }

   if( o->isPhongSizeEnabled() )
   {
      str1.setNum( o->phongSize() );
      dev->writeLine( "phong_size " + str1 );
   }

   if( o->isMetallicEnabled() )
   {
      str1.setNum( o->metallic() );
      dev->writeLine( "metallic " + str1 );
   }

   if( o->isSpecularEnabled() )
   {
      str1.setNum( o->specular() );
      dev->writeLine( "specular " + str1 );
   }

   if( o->isRoughnessEnabled() )
   {
      str1.setNum( o->roughness() );
      dev->writeLine( "roughness " + str1 );
   }

   if( o->isCrandEnabled() )
   {
      str1.setNum( o->crand() );
      dev->writeLine( "crand " + str1 );
   }

   if( o->conserveEnergy() )
      dev->writeLine( "conserve_energy" );

   if( o->irid() )
   {
      str1.setNum( o->iridAmount() );
      dev->writeLine( "irid { " + str1 );
      str1.setNum( o->iridThickness() );
      dev->writeLine( "thickness " + str1 );
      str1.setNum( o->iridTurbulence() );
      dev->writeLine( "turbulence " + str1 + " } " );
   }

   if( o->isReflectionEnabled() )
   {
      dev->objectBegin( "reflection" );

      if ( o->isReflectionMinEnabled() )
      {
         dev->writeLine( o->reflectionMinColor().serialize() + ", " +
                         o->reflectionColor().serialize() );
      }
      else
         dev->writeLine( o->reflectionColor().serialize() );

      if ( o->reflectionFresnel() )
         dev->writeLine( "fresnel" );

      if ( o->isRefFalloffEnabled() )
      {
         str1.setNum( o->reflectionFalloff() );
         dev->writeLine( "falloff " + str1 );
      }

      if ( o->isRefExponentEnabled() )
      {
         str1.setNum( o->reflectionExponent() );
         dev->writeLine( "exponent " + str1 );
      }

      if ( o->isRefMetallicEnabled() )
      {
         str1.setNum( o->reflectionMetallic() );
         dev->writeLine( "metallic " + str1 );
      }

      dev->objectEnd();
   }

   dev->objectEnd();
}

void PMPov35SerMesh( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMMesh* o = ( PMMesh* ) object;

   dev->objectBegin( "mesh" );

   if( o->isInsideVectorEnabled() )
      dev->writeLine( "inside_vector " + o->insideVector().serialize() );

   dev->callSerialization( object, metaObject->superClass() );

   if( !o->hierarchy() )
      dev->writeLine( "hierarchy off" );

   dev->objectEnd();
}

const int c_defaultMediaMethod = 1;
const int c_defaultMediaIntervals = 10;
const int c_defaultMediaSamplesMin = 1;
const int c_defaultMediaSamplesMax = 1;
const double c_defaultMediaConfidence = 0.9;
const double c_defaultMediaVariance = 0.0078125;
const double c_defaultMediaRatio = 0.9;
const int c_defaultMediaAALevel = 4;
const double c_defaultMediaAAThreshold = 0.1;
const double c_defaultMediaScatteringEccentricity = 0;
const double c_defaultMediaScatteringExtinction = 1.0;

void PMPov35SerMedia( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMMedia* o = ( PMMedia* ) object;

   QString str1;
   QString str2;

   dev->objectBegin( "media" );
   dev->callSerialization( object, metaObject->superClass() );

   if( o->method() != c_defaultMediaMethod )
   {
      str1.setNum( o->method() );
      dev->writeLine( "method " + str1 );
   }
   if( o->intervals() != c_defaultMediaIntervals )
   {
      str1.setNum( o->intervals() );
      dev->writeLine( "intervals " + str1 );
   }
   if( o->samplesMin() != c_defaultMediaSamplesMin ||
       o->samplesMax() != c_defaultMediaSamplesMax )
   {
      str1.setNum( o->samplesMin() );
      str2.setNum( o->samplesMax() );
      if ( o->method() < 3 )
         dev->writeLine( "samples " + str1 + ',' + str2 );
      else
         dev->writeLine( "samples " + str1 );
   }
   if( o->confidence() != c_defaultMediaConfidence )
   {
      str1.setNum( o->confidence() );
      dev->writeLine( "confidence " + str1 );
   }
   if( o->variance() != c_defaultMediaVariance )
   {
      str1.setNum( o->variance() );
      dev->writeLine( "variance " + str1 );
   }
   if( o->ratio() != c_defaultMediaRatio )
   {
      str1.setNum( o->ratio() );
      dev->writeLine( "ratio " + str1 );
   }
   if ( o->method() == 3 )
   {
      if ( o->aaLevel() != c_defaultMediaAALevel )
      {
         str1.setNum( o->aaLevel() );
         dev->writeLine( "aa_level " + str1 );
      }
      if ( o->aaThreshold() != c_defaultMediaAAThreshold )
      {
         str1.setNum( o->aaThreshold() );
         dev->writeLine( "aa_threshold " + str1 );
      }
   }
   if( o->isAbsorptionEnabled() )
   {
      dev->writeLine( "absorption " + o->absorption().serialize() );
   }
   if( o->isEmissionEnabled() )
   {
      dev->writeLine( "emission " + o->emission().serialize() );
   }
   if( o->isScatteringEnabled() )
   {
      dev->objectBegin( "scattering" );
      str1.setNum( o->scatteringType() );
      dev->writeLine( str1 + ", " + o->scatteringColor().serialize() );
      if( o->scatteringType() == 5 && o->scatteringEccentricity()
          != c_defaultMediaScatteringEccentricity )
      {
         str1.setNum( o->scatteringEccentricity() );
         dev->writeLine( "eccentricity " + str1 );
      }
      if( o->scatteringExtinction() != c_defaultMediaScatteringExtinction )
      {
         str1.setNum( o->scatteringExtinction() );
         dev->writeLine( "extinction " + str1 );
      }
      dev->objectEnd();
   }
   dev->objectEnd();
}

void PMPov35SerGraphicalObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMGraphicalObject* o = ( PMGraphicalObject* ) object;

   dev->callSerialization( object, metaObject->superClass() );

   if( o->noShadow() )
      dev->writeLine( "no_shadow" );

   if( o->noImage() )
      dev->writeLine( "no_image" );

   if( o->noReflection() )
      dev->writeLine( "no_reflection" );

   if( o->doubleIlluminate() )
      dev->writeLine( "double_illuminate" );
}

void PMPov35SerPigment( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMPigment* o = ( PMPigment* ) object;

   bool bObject = true;
   if( o->parent() )
       if( o->parent()->type() == "PigmentMap" )
         bObject = false;

   if( bObject )
   {
      dev->objectBegin( "pigment" );
      if ( o->uvMapping() )
         dev->writeLine( "uv_mapping" );
   }
   dev->callSerialization( object, metaObject->superClass() );
   if( bObject )
      dev->objectEnd();
}

void PMPov35SerTexture( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMTexture* o = ( PMTexture* ) object;

   bool bObject = true;
   if( o->parent() )
      if( o->parent()->type() == "TextureMap" )
         bObject = false;

   if( bObject )
   {
      dev->objectBegin( "texture" );
      if ( o->uvMapping() )
         dev->writeLine( "uv_mapping" );
   }
   dev->callSerialization( object, metaObject->superClass() );
   if( bObject )
      dev->objectEnd();
}

const double c_defaultPatchFlatness = 0;

void PMPov35SerBicubicPatch( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMBicubicPatch* o = ( PMBicubicPatch* ) object;

   int u, v;
   QString str, line;
   dev->objectBegin( "bicubic_patch" );

   dev->writeName( object->name() );

   str.setNum( o->patchType() );
   dev->writeLine( "type " + str );
   if( !approx( o->flatness(), c_defaultPatchFlatness ) )
   {
      str.setNum( o->flatness() );
      dev->writeLine( "flatness " + str );
   }
   str.setNum( o->uSteps() );
   dev->writeLine( "u_steps " + str );
   str.setNum( o->vSteps() );
   dev->writeLine( "v_steps " + str );

   if( o->isUVEnabled() )
   {
      dev->writeLine( "uv_vectors " + o->uvVector( 0 ).serialize() +
                      ' ' + o->uvVector( 1 ).serialize() +
                      ' ' + o->uvVector( 2 ).serialize() +
                      ' ' + o->uvVector( 3 ).serialize() );
   }

   for( v = 0; v < 4; v++ )
   {
      line = o->controlPoint( v*4 ).serialize();
      for( u = 1; u < 4; u++ )
         line += QString( ", " ) + o->controlPoint( u+4*v ).serialize();
      if( v != 3 )
         line += ',';
      dev->writeLine( line );
   }

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov35SerTriangle( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMTriangle* o = ( PMTriangle* ) object;

   if( o->isSmoothTriangle() )
   {
      dev->objectBegin( "smooth_triangle" );

      dev->writeName( object->name() );
      dev->writeLine( o->point( 0 ).serialize() + ", " + o->normal( 0 ).serialize() + ',' );
      dev->writeLine( o->point( 1 ).serialize() + ", " + o->normal( 1 ).serialize() + ',' );
      dev->writeLine( o->point( 2 ).serialize() + ", " + o->normal( 2 ).serialize() );
   }
   else
   {
      dev->objectBegin( "triangle" );

      dev->writeName( object->name() );
      dev->writeLine( o->point( 0 ).serialize() + ", " + o->point( 1 ).serialize()
                     + ", " + o->point( 2 ).serialize() );
   }

   if( o->isUVEnabled() )
   {
      dev->writeLine( "uv_vectors " + o->uvVector( 0 ).serialize() +
                      ' ' + o->uvVector( 1 ).serialize() +
                      ' ' + o->uvVector( 2 ).serialize() );
   }

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}
