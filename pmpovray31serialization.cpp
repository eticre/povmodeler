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

#include "pmpovray31serialization.h"
#include "pmoutputdevice.h"
#include "pmallobjects.h"
#include <QList>
#include <QTextStream>

const double c_defaultPatchFlatness = 0;

void PMPov31SerBicubicPatch( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
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

void PMPov31SerBlendMapModifiers( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMBlendMapModifiers* o = ( PMBlendMapModifiers* ) object;

   QString str;

   if( o->isFrequencyEnabled() )
   {
      str.setNum( o->frequency() );
      dev->writeLine( "frequency " + str );
   }

   if( o->isPhaseEnabled() )
   {
      str.setNum( o->phase() );
      dev->writeLine( "phase " + str );
   }

   if( o->isWaveFormEnabled() )
   {
      switch( o->waveFormType() )
      {
         case PMBlendMapModifiers::RampWave:
            dev->writeLine( "ramp_wave" );
            break;
         case PMBlendMapModifiers::TriangleWave:
            dev->writeLine( "triangle_wave" );
            break;
         case PMBlendMapModifiers::SineWave:
            dev->writeLine( "sine_wave" );
            break;
         case PMBlendMapModifiers::ScallopWave:
            dev->writeLine( "scallop_wave" );
            break;
         case PMBlendMapModifiers::CubicWave:
            dev->writeLine( "cubic_wave" );
            break;
         case PMBlendMapModifiers::PolyWave:
            str.setNum( o->waveFormExponent() );
            dev->writeLine( "poly_wave " + str );
            break;
      }
   }
}

void PMPov31SerBlob( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMBlob* o = ( PMBlob* ) object;

   dev->objectBegin( "blob" );

   dev->writeName( object->name() );
   dev->writeLine( QString( "threshold %1" ).arg( o->threshold() ) );

   dev->callSerialization( object, metaObject->superClass() );

   if( o->sturm() )
      dev->writeLine( "sturm" );
   if( o->hierarchy() )
      dev->writeLine( "hierarchy" );

   dev->objectEnd();
}

void PMPov31SerBlobCylinder( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMBlobCylinder* o = ( PMBlobCylinder* ) object;

   dev->objectBegin( "cylinder" );

   dev->writeName( object->name() );
   QString str1;
   str1.setNum( o->radius() );
   dev->writeLine( o->end1().serialize() + ", " + o->end2().serialize()
                  + ", " + str1 + ',' );
   dev->writeLine( QString( "strength %1" ).arg( o->strength() ) );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerBlobSphere( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMBlobSphere* o = ( PMBlobSphere* ) object;

   dev->objectBegin( "sphere" );
   dev->writeName( object->name() );
   dev->writeLine( o->centre().serialize() + QString( ", %1," ).arg( o->radius() ) );
   dev->writeLine( QString( "strength %1" ).arg( o->strength() ) );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerBoundedBy( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMBoundedBy* o = ( PMBoundedBy* ) object;

   dev->objectBegin( "bounded_by" );
   if( o->clippedBy() )
      dev->writeLine( "clipped_by" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerBox( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMBox* o = ( PMBox* ) object;

   dev->objectBegin( "box" );

   dev->writeName( object->name() );
   dev->writeLine( o->corner1().serialize() + ", " + o->corner2().serialize() );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerBumpMap( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMBumpMap* o = ( PMBumpMap* ) object;

   QString str1;

   dev->objectBegin( "bump_map" );

   switch( o->bitmapType() )
   {
      case PMBumpMap::BitmapGif:
         dev->writeLine( "gif" );
         break;
      case PMBumpMap::BitmapTga:
         dev->writeLine( "tga" );
         break;
      case PMBumpMap::BitmapIff:
         dev->writeLine( "iff" );
         break;
      case PMBumpMap::BitmapPpm:
         dev->writeLine( "ppm" );
         break;
      case PMBumpMap::BitmapPgm:
         dev->writeLine( "pgm" );
         break;
      case PMBumpMap::BitmapPng:
         dev->writeLine( "png" );
         break;
      case PMBumpMap::BitmapJpeg:
         dev->writeLine( "jpeg" );
         break;
      case PMBumpMap::BitmapTiff:
         dev->writeLine( "tiff" );
         break;
      case PMBumpMap::BitmapSys:
         dev->writeLine( "sys" );
         break;
   }

   dev->writeLine( "\"" + o->bitmapFile() + "\"" );

   if( o->isOnceEnabled() )
      dev->writeLine( "once" );

   switch( o->mapType() )
   {
      case PMBumpMap::MapPlanar:
         dev->writeLine( "map_type 0" );
         break;
      case PMBumpMap::MapSpherical:
         dev->writeLine( "map_type 1" );
         break;
      case PMBumpMap::MapCylindrical:
         dev->writeLine( "map_type 2" );
         break;
      case PMBumpMap::MapToroidal:
         dev->writeLine( "map_type 5" );
         break;
   }
   switch( o->interpolateType() )
   {
      case PMBumpMap::InterpolateBilinear:
         dev->writeLine( "interpolate 2" );
         break;
      case PMBumpMap::InterpolateNormalized:
         dev->writeLine( "interpolate 4" );
         break;
      default:
         break;
   }

   if( o->isUseIndexEnabled() )
      dev->writeLine( "use_index" );

   if( o->bumpSize() )
   {
      str1.setNum( o->bumpSize() );
      dev->writeLine( "bump_size " + str1 );
   }

   dev->objectEnd();
}

void PMPov31SerCamera( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMCamera* o = ( PMCamera* ) object;

   dev->objectBegin( "camera" );
   QString str;

   dev->writeName( object->name() );

   if( o->cameraType() == PMCamera::Cylinder )
   {
      str.setNum( o->cylinderType() );
      dev->writeLine( "cylinder " + str );
   }
   else
      dev->writeLine( o->cameraTypeToString( o->cameraType() ) );

   dev->writeLine( "location " + o->location().serialize() );
   dev->writeLine( "sky " + o->sky().serialize() );
   dev->writeLine( "direction " + o->direction().serialize() );
   dev->writeLine( "right " + o->right().serialize() );
   dev->writeLine( "up " + o->up().serialize() );
   dev->writeLine( "look_at " + o->lookAt().serialize() );

   //eticre angle in ortho is ok, set vista same in persp.
   if( /*( o->cameraType() != PMCamera::Orthographic ) &&*/
       ( o->cameraType() != PMCamera::Omnimax ) &&
       ( o->cameraType() != PMCamera::Panoramic ) && o->isAngleEnabled() )
   {
      str.setNum( o->angle() );
      dev->writeLine( "angle " + str );
   }
   if( o->isFocalBlurEnabled() && ( o->cameraType() == PMCamera::Perspective ) )
   {
      str.setNum( o->aperture() );
      dev->writeLine( "aperture " + str );
      str.setNum( o->blurSamples() );
      dev->writeLine( "blur_samples " + str );
      dev->writeLine( "focal_point " + o->focalPoint().serialize() );
      str.setNum( o->confidence() );
      dev->writeLine( "confidence " + str );
      str.setNum( o->variance() );
      dev->writeLine( "variance " + str );
   }

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerClippedBy( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMClippedBy* o = ( PMClippedBy* ) object;

   dev->objectBegin( "clipped_by" );
   if( o->boundedBy() )
      dev->writeLine( "bounded_by" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerComment( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMComment* o = ( PMComment* ) object;

   dev->writeComment( o->text() );
}

void PMPov31SerCompositeObject( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMCompositeObject* o = ( PMCompositeObject* ) object;

   PMObject* tmp;

   //fail no radiosity if not enabled because rad.is.child.of.globalSetting
   for( tmp = o->firstChild(); tmp; tmp = tmp->nextSibling() )
      if( tmp->exportPovray() )
         dev->serialize( tmp );

   //if( o->is_a_function )
}

void PMPov31SerCone( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMCone* o = ( PMCone* ) object;

   dev->objectBegin( "cone" );

   dev->writeName( object->name() );
   QString str1;
   str1.setNum( o->radius1() );
   dev->writeLine( o->end1().serialize() + ", " + str1 + ',' );
   str1.setNum( o->radius2() );
   dev->writeLine( o->end2().serialize() + ", " + str1 );
   if( o->open() )
      dev->writeLine( QString( "open" ) );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerCSG( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMCSG* o = ( PMCSG* ) object;

   switch( o->csgType() )
   {
      case PMCSG::CSGUnion:
         dev->objectBegin( "union" );
         break;
      case PMCSG::CSGIntersection:
         dev->objectBegin( "intersection" );
         break;
      case PMCSG::CSGDifference:
         dev->objectBegin( "difference" );
         break;
      case PMCSG::CSGMerge:
         dev->objectBegin( "merge" );
         break;
   }

   dev->writeName( object->name() );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerCylinder( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMCylinder* o = ( PMCylinder* ) object;

   dev->objectBegin( "cylinder" );

   dev->writeName( object->name() );
   QString str1;
   str1.setNum( o->radius() );
   dev->writeLine( o->end1().serialize() + ", " + o->end2().serialize()
                  + ", " + str1 );
   if( o->open() )
      dev->writeLine( QString( "open" ) );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerDeclare( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMDeclare* o = ( PMDeclare* ) object;

   if( o->firstChild() )
   {
      if( o->is_a_function )
          dev->declareBegin( o->id() + " = function{ " );
      else
          dev->declareBegin( o->id() + " = " );
      dev->callSerialization( object, metaObject->superClass() );
      if( o->is_a_function )
          dev->objectEnd();
   }
}

void PMPov31SerDensity( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   bool bObject = true;
   if( object->parent() )
      if( object->parent()->type() == "DensityMap" )
         bObject = false;

   if( bObject )
      dev->objectBegin( "density" );
   dev->callSerialization( object, metaObject->superClass() );
   if( bObject )
      dev->objectEnd();
}

void PMPov31SerDisc( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMDisc* o = ( PMDisc* ) object;

   dev->objectBegin( "disc" );

   dev->writeName( object->name() );
   QString str1, str2;
   str1.setNum( o->radius() );
   if( o->radius() != 0.0 )
   {
      str2.setNum( o->holeRadius() );
      dev->writeLine( o->center().serialize() + ',' + o->normal().serialize() + ", "  + str1 + ',' + str2 );
   }
   else
   {
      dev->writeLine( o->center().serialize() + ','  + o->normal().serialize() + ", "  + str1 );
   }
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerFinish( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
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
   if( o->isReflectionEnabled() )
   {
      dev->writeLine( "reflection " + o->reflectionColor().serialize() );
   }
   if( o->isExponentEnabled() )
   {
      str1.setNum( o->reflectionExponent() );
      dev->writeLine( "reflection_exponent " + str1 );
   }
   if( o->irid() )
   {
      str1.setNum( o->iridAmount() );
      dev->writeLine( "irid { " + str1 );
      str1.setNum( o->iridThickness() );
      dev->writeLine( "thickness " + str1 );
      str1.setNum( o->iridTurbulence() );
      dev->writeLine( "turbulence " + str1 + " } " );
   }
   if( o->isCrandEnabled() )
   {
      str1.setNum( o->crand() );
      dev->writeLine( "crand " + str1 );
   }
   dev->objectEnd();
}

const int c_defaultFogOctaves = 6;
const double c_defaultFogLambda = 2.0;
const double c_defaultFogOmega = 0.5;
const double c_defaultFogTurbDepth = 0.5;

void PMPov31SerFog( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMFog* o = ( PMFog* ) object;

   QString str;

   dev->objectBegin( "fog" );

   // Serialize the name of this object
   dev->writeName( object->name() );

   // Serialize a possible link
   if( o->linkedObject() )
   {
      if( o->linkedObject()->firstChild() )
         dev->writeLine( o->linkedObject()->id() );
      else
      {
         QString text;
         text = o->name();
         if( text.isEmpty() )
            text = o->description();

         dev->writeComment( QString( "No prototype for %1" ).arg( text ) );
      }
   }

   str.setNum( o->fogType() );
   dev->writeLine( "fog_type " + str );

   str.setNum( o->distance() );
   dev->writeLine( "distance " + str );
   dev->writeLine( ' ' + o->color().serialize() + ' ' );
   if( o->isTurbulenceEnabled() )
   {
      dev->writeLine( "turbulence " + o->valueVector().serialize() );
      if( o->octaves() != c_defaultFogOctaves )
      {
         str.setNum(o->octaves());
         dev->writeLine( "octaves " + str );
      }
      if( o->omega() != c_defaultFogOmega )
      {
         str.setNum(o->omega());
         dev->writeLine( "omega " + str );
      }
      if( o->lambda() != c_defaultFogLambda )
      {
         str.setNum(o->lambda());
         dev->writeLine( "lambda " + str );
      }
      if( o->depth() != c_defaultFogTurbDepth )
      {
         str.setNum(o->depth());
         dev->writeLine( "turb_depth " + str );
      }
   }
   if( o->fogType() == 2 )
   {
      // Serialize ground fog variables
      str.setNum( o->fogOffset() );
      dev->writeLine( "fog_offset " + str );
      str.setNum( o->fogAlt() );
      dev->writeLine( "fog_alt " + str );
      dev->writeLine( "up " + o->up().serialize() );
   }
   // Serialize the children of this object
   dev->callSerialization( object, object->metaObject()->superClass()->superClass() );
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

void PMPov31SerGlobalSettings( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
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
   if( o->isRadiosityEnabled() )
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
   dev->objectEnd();
}

void PMPov31SerGraphicalObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMGraphicalObject* o = ( PMGraphicalObject* ) object;

   dev->callSerialization( object, metaObject->superClass() );
   if( o->noShadow() )
      dev->writeLine( "no_shadow" );
}

void PMPov31SerHeightField( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMHeightField* o = ( PMHeightField* ) object;

   dev->objectBegin( "height_field" );

   dev->writeName( object->name() );

   dev->writeLine( o->typeToString( o->heightFieldType() ) + " \"" + o->fileName() + "\"" );
   if( o->waterLevel() > 0.0 )
      dev->writeLine( QString( "water_level %1" ).arg( o->waterLevel() ) );
   if( !o->hierarchy() )
      dev->writeLine( "hierarchy off" );
   if( o->smooth() )
      dev->writeLine( "smooth" );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerImageMap( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMImageMap* o = ( PMImageMap* ) object;

   typedef QList<PMPaletteValue> PMPaletteValueList;
   PMPaletteValueList values;
   PMPaletteValueList::ConstIterator tmpPalette;
   QString str1, str2;

   dev->objectBegin( "image_map" );

   switch( o->bitmapType() )
   {
      case PMImageMap::BitmapGif:
         dev->writeLine( "gif" );
         break;
      case PMImageMap::BitmapTga:
         dev->writeLine( "tga" );
         break;
      case PMImageMap::BitmapIff:
         dev->writeLine( "iff" );
         break;
      case PMImageMap::BitmapPpm:
         dev->writeLine( "ppm" );
         break;
      case PMImageMap::BitmapPgm:
         dev->writeLine( "pgm" );
         break;
      case PMImageMap::BitmapPng:
         dev->writeLine( "png" );
         break;
      case PMImageMap::BitmapJpeg:
         dev->writeLine( "jpeg" );
         break;
      case PMImageMap::BitmapTiff:
         dev->writeLine( "tiff" );
         break;
      case PMImageMap::BitmapSys:
         dev->writeLine( "sys" );
         break;
   }

   dev->writeLine( "\"" + o->bitmapFile() + "\"" );

   values = o->filters();
   for( tmpPalette = values.constBegin(); tmpPalette != values.constEnd(); ++tmpPalette )
   {
      str1.setNum( ( *tmpPalette ).index() );
      str2.setNum( ( *tmpPalette ).value() );
      dev->writeLine( "filter " + str1 + ", " + str2 );
   }

   values = o->transmits();
   for( tmpPalette = values.constBegin(); tmpPalette != values.constEnd(); ++tmpPalette )
   {
      str1.setNum( ( *tmpPalette ).index() );
      str2.setNum( ( *tmpPalette ).value() );
      dev->writeLine( "transmit " + str1 + ", " + str2 );
   }

   if( o->isFilterAllEnabled() )
   {
      str1.setNum( o->filterAll() );
      dev->writeLine( "filter all " + str1 );
   }

   if( o->isTransmitAllEnabled() )
   {
      str1.setNum( o->transmitAll() );
      dev->writeLine( "transmit all " + str1 );
   }

   if( o->isOnceEnabled() )
      dev->writeLine( "once" );

   switch( o->mapType() )
   {
      case PMImageMap::MapPlanar:
         dev->writeLine( "map_type 0" );
         break;
      case PMImageMap::MapSpherical:
         dev->writeLine( "map_type 1" );
         break;
      case PMImageMap::MapCylindrical:
         dev->writeLine( "map_type 2" );
         break;
      case PMImageMap::MapToroidal:
         dev->writeLine( "map_type 5" );
         break;
   }
   switch( o->interpolateType() )
   {
      case PMImageMap::InterpolateBilinear:
         dev->writeLine( "interpolate 2" );
         break;
      case PMImageMap::InterpolateNormalized:
         dev->writeLine( "interpolate 4" );
         break;
      default:
         break;
   }

   dev->objectEnd();
}

void PMPov31SerInterior( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
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
   if( o->isFadeDistanceEnabled() )
   {
      str1.setNum( o->fadeDistance() );
      dev->writeLine( "fade_distance " + str1 );
   }
   if( o->isFadeDistanceEnabled() )
   {
      str1.setNum( o->fadeDistance() );
      dev->writeLine( "fade_distance " + str1 );
   }
   dev->objectEnd();
}

void PMPov31SerJuliaFractal( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMJuliaFractal* o = ( PMJuliaFractal* ) object;

   dev->objectBegin( "julia_fractal" );

   dev->writeName( object->name() );
   dev->writeLine( o->juliaParameter().serialize() );
   dev->writeLine( o->algebraTypeToString( o->algebraType() ) );

   if( o->functionType() == PMJuliaFractal::FTpwr )
      dev->writeLine( QString( "pwr(%1, %2)" ).arg( o->exponent()[0] ).
                     arg( o->exponent()[1] ) );
   else
      dev->writeLine( o->functionTypeToString( o->functionType() ) );

   dev->writeLine( QString( "max_iteration %1" ).arg( o->maximumIterations() ) );
   dev->writeLine( QString( "precision %1" ).arg( o->precision() ) );
   dev->writeLine( QString( "slice %1, %2" ).arg( o->sliceNormal().serialize() )
                  .arg( o->sliceDistance() ) );


   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerLathe( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMLathe* o = ( PMLathe* ) object;

   dev->objectBegin( "lathe" );

   dev->writeName( object->name() );

   switch( o->splineType() )
   {
      case PMLathe::LinearSpline:
         dev->writeLine( "linear_spline" );
         break;
      case PMLathe::QuadraticSpline:
         dev->writeLine( "quadratic_spline" );
         break;
      case PMLathe::CubicSpline:
         dev->writeLine( "cubic_spline" );
         break;
      case PMLathe::BezierSpline:
         dev->writeLine( "bezier_spline" );
         break;
   }

   int num = o->points().count();
   dev->writeLine( QString( "%1," ).arg( num ) );

   bool first = true;
   QList<PMVector> points = o->points();
   QList<PMVector>::ConstIterator it = points.constBegin();
   for( ; it != points.constEnd(); ++it )
   {
      if( !first )
         dev->write( ", " );
      dev->write( ( *it ).serialize() );
      first = false;
   }
   dev->writeLine( "" );

   if( o->sturm() )
      dev->writeLine( "sturm" );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

const double c_defaultLightTightness = 10;
const int c_defaultLightAdaptive = 0;

void PMPov31SerLight( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
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

void PMPov31SerListPattern( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMListPattern* o = ( PMListPattern* ) object;

   QString str1;

   switch( o->listType() )
   {
      case PMListPattern::ListPatternBrick:
         dev->writeLine( "brick" );
         break;
      case PMListPattern::ListPatternChecker:
         dev->writeLine( "checker " );
         break;
      case PMListPattern::ListPatternHexagon:
         dev->writeLine( "hexagon " );
         break;
   }

   dev->callSerialization( object, metaObject->superClass() );

   if( o->listType() == PMListPattern::ListPatternBrick )
   {
      dev->writeLine( "brick_size " +
                      o->brickSize().serialize() );
      str1.setNum( o->mortar() );
      dev->writeLine( "mortar " + str1 );
   }
}

void PMPov31SerTextureList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->callSerialization( object, metaObject->superClass() );
}

void PMPov31SerPigmentList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->callSerialization( object, metaObject->superClass() );
}

void PMPov31SerColorList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->callSerialization( object, metaObject->superClass() );
}

void PMPov31SerDensityList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->callSerialization( object, metaObject->superClass() );
}

void PMPov31SerNormalList( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMNormalList* o = ( PMNormalList* ) object;

   QString str1;

   switch( o->listType() )
   {
      case PMNormalList::ListPatternBrick:
         dev->writeLine( "brick " );
         break;
      case PMNormalList::ListPatternChecker:
         dev->writeLine( "checker " );
         break;
      case PMNormalList::ListPatternHexagon:
         dev->writeLine( "hexagon " );
         break;
   }
   if( o->depth() )
   {
      str1.setNum( o->depth() );
      dev->writeLine( str1 );
   }
   if( o->listType() == PMNormalList::ListPatternBrick )
   {
      dev->writeLine( "brick_size " +
                      o->brickSize().serialize() );
      str1.setNum( o->mortar() );
      dev->writeLine( "mortar " + str1 );
   }
}

void PMPov31SerLooksLike( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "looks_like" );
   dev->writeName( object->name() );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerMaterial( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "material" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerMaterialMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMMaterialMap* o = ( PMMaterialMap* ) object;

   QString str1;

   dev->objectBegin( "material_map" );

   switch( o->bitmapType() )
   {
      case PMMaterialMap::BitmapGif:
         dev->writeLine( "gif" );
         break;
      case PMMaterialMap::BitmapTga:
         dev->writeLine( "tga" );
         break;
      case PMMaterialMap::BitmapIff:
         dev->writeLine( "iff" );
         break;
      case PMMaterialMap::BitmapPpm:
         dev->writeLine( "ppm" );
         break;
      case PMMaterialMap::BitmapPgm:
         dev->writeLine( "pgm" );
         break;
      case PMMaterialMap::BitmapPng:
         dev->writeLine( "png" );
         break;
      case PMMaterialMap::BitmapJpeg:
         dev->writeLine( "jpeg" );
         break;
      case PMMaterialMap::BitmapTiff:
         dev->writeLine( "tiff" );
         break;
      case PMMaterialMap::BitmapSys:
         dev->writeLine( "sys" );
         break;
   }

   dev->writeLine( "\"" + o->bitmapFile() + "\"" );

   if( o->isOnceEnabled() )
      dev->writeLine( "once" );

   switch( o->mapType() )
   {
      case PMMaterialMap::MapPlanar:
         dev->writeLine( "map_type 0" );
         break;
      case PMMaterialMap::MapSpherical:
         dev->writeLine( "map_type 1" );
         break;
      case PMMaterialMap::MapCylindrical:
         dev->writeLine( "map_type 2" );
         break;
      case PMMaterialMap::MapToroidal:
         dev->writeLine( "map_type 5" );
         break;
   }
   switch( o->interpolateType() )
   {
      case PMMaterialMap::InterpolateBilinear:
         dev->writeLine( "interpolate 2" );
         break;
      case PMMaterialMap::InterpolateNormalized:
         dev->writeLine( "interpolate 4" );
         break;
      default:
         break;
   }

   dev->callSerialization( object, metaObject->superClass() );

   dev->objectEnd();
}

const int c_defaultMediaIntervals = 10;
const int c_defaultMediaSamplesMin = 1;
const int c_defaultMediaSamplesMax = 1;
const double c_defaultMediaConfidence = 0.9;
const double c_defaultMediaVariance = 0.0078125;
const double c_defaultMediaRatio = 0.9;
const double c_defaultMediaScatteringEccentricity = 0;
const double c_defaultMediaScatteringExtinction = 1.0;

void PMPov31SerMedia( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMMedia* o = ( PMMedia* ) object;

   QString str1;
   QString str2;

   dev->objectBegin( "media" );
   dev->callSerialization( object, metaObject->superClass() );
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
      dev->writeLine( "samples " + str1 + ',' + str2 );
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

void PMPov31SerNamedObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->callSerialization( object, metaObject->superClass() );
}

void PMPov31SerNormal( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMNormal* o = ( PMNormal* ) object;

   QString str1;
   bool bObject = true;

   if( o->parent() )
      if( o->parent()->type() == "NormalMap" )
         bObject = false;

   if( bObject )
      dev->objectBegin( "normal" );
   dev->callSerialization( object, metaObject->superClass() );
   if( o->isBumpSizeEnabled() )
   {
      str1.setNum( o->bumpSize() );
      dev->writeLine( "bump_size " + str1 );
   }
   if( bObject )
      dev->objectEnd();
}

void PMPov31SerObjectLink( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMObjectLink* o = ( PMObjectLink* ) object;

   bool writeComment = true;
   if( o->linkedObject() )
   {
      if( o->linkedObject()->firstChild() )
      {
         dev->objectBegin( "object" );

         dev->writeName( object->name() );
         dev->writeLine( o->linkedObject()->id() );
         dev->callSerialization( object, metaObject->superClass() );
         writeComment = false;

         dev->objectEnd();
      }
   }
   if( writeComment )
   {
      QString text;
      text = o->name();
      if( text.isEmpty() )
         text = o->description();

      dev->writeComment( QString( "No prototype for %1" ).arg( text ) );
   }
}

const int c_defaultPatternOctaves = 6;
const double c_defaultPatternOmega = 0.5;
const double c_defaultPatternLambda = 2.0;

void PMPov31SerPattern( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMPattern* o = ( PMPattern* ) object;

   QString str;

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
      case PMPattern::PatternLeopard:
         dev->writeLine( "leopard" );
         break;
      case PMPattern::PatternMandel:
         str.setNum( o->maxIterations() );
         dev->writeLine( "mandel " + str );
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
      case PMPattern::PatternWaves:
         dev->writeLine( "waves" );
         break;
      case PMPattern::PatternWood:
         dev->writeLine( "wood" );
         break;
      case PMPattern::PatternWrinkles:
         dev->writeLine( "wrinkles" );
         break;
      default:
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
      case PMPattern::PatternDensity:
         str.setNum( o->densityInterpolate() );
         dev->writeLine( "interpolate " + str );
         break;
      case PMPattern::PatternQuilted:
         str.setNum( o->quiltControl0() );
         dev->writeLine( "control0 " + str );
         str.setNum( o->quiltControl1() );
         dev->writeLine( "control1 " + str );
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

void PMPov31SerPigment( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMPigment* o = ( PMPigment* ) object;

   bool bObject = true;
   if( o->parent() )
      if( o->parent()->type() == "PigmentMap" )
         bObject = false;

   if( bObject )
      dev->objectBegin( "pigment" );
   dev->callSerialization( object, metaObject->superClass() );
   if( bObject )
      dev->objectEnd();
}

void PMPov31SerPlane( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMPlane* o = ( PMPlane* ) object;

   dev->objectBegin( "plane" );

   dev->writeName( object->name() );
   QString str1;
   str1.setNum( o->distance() );
   dev->writeLine( o->normal().serialize() + ", "  + str1 );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerPolynom( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMPolynom* o = ( PMPolynom* ) object;
   PMVector coefficients = o->coefficients();

   if( o->polynomOrder() == 2 )
      dev->objectBegin( "quadric" );
   else if( o->polynomOrder() == 3 )
      dev->objectBegin( "cubic" );
   else if( o->polynomOrder() == 4 )
      dev->objectBegin( "quartic" );
   else
      dev->objectBegin( "poly" );

   dev->writeName( object->name() );

   if( o->polynomOrder() == 2 )
   {
      dev->writeLine( QString( "<%1, %2, %3>," ).arg( coefficients[0] )
                     .arg( coefficients[4] ).arg( coefficients[7] ) );
      dev->writeLine( QString( "<%1, %2, %3>," ).arg( coefficients[1] )
                     .arg( coefficients[2] ).arg( coefficients[5] ) );
      dev->writeLine( QString( "<%1, %2, %3>, %4" ).arg( coefficients[3] )
                     .arg( coefficients[6] ).arg( coefficients[8] )
                     .arg( coefficients[9] ) );
   }
   else
   {
      if( o->polynomOrder() > 4 )
         dev->writeLine( QString( "%1," ).arg( o->polynomOrder() ) );

      int size = coefficients.size();

      int i;
      QString hlp;

      dev->write( "<" );
      for( i = 0; i < size; i++ )
      {
         hlp.setNum( coefficients[i] );
         dev->write( hlp );

         if( i != ( size - 1 ) )
         {
            dev->write( ", " );
            if( ( ( i + 1 ) % 5 ) == 0  )
               dev->writeLine( "" );
         }
      }
      dev->writeLine( ">" );
      if( o->sturm() )
         dev->writeLine( "sturm" );
   }

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerPovrayMatrix( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMPovrayMatrix* o = ( PMPovrayMatrix* ) object;

   dev->writeLine( QString( "matrix < %1, %2, %3," ).arg( o->values()[0] )
                  .arg( o->values()[1] ).arg( o->values()[2] ) );
   dev->writeLine( QString( "         %1, %2, %3," ).arg( o->values()[3] )
                  .arg( o->values()[4] ).arg( o->values()[5] ) );
   dev->writeLine( QString( "         %1, %2, %3," ).arg( o->values()[6] )
                  .arg( o->values()[7] ).arg( o->values()[8] ) );
   dev->writeLine( QString( "         %1, %2, %3 >" ).arg( o->values()[9] )
                  .arg( o->values()[10] ).arg( o->values()[11] ) );
}

void PMPov31SerPrism( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMPrism* o = ( PMPrism* ) object;

   dev->objectBegin( "prism" );

   dev->writeName( object->name() );

   switch( o->splineType() )
   {
      case PMPrism::LinearSpline:
         dev->writeLine( "linear_spline" );
         break;
      case PMPrism::QuadraticSpline:
         dev->writeLine( "quadratic_spline" );
         break;
      case PMPrism::CubicSpline:
         dev->writeLine( "cubic_spline" );
         break;
      case PMPrism::BezierSpline:
         dev->writeLine( "bezier_spline" );
         break;
   }
   switch( o->sweepType() )
   {
      case PMPrism::LinearSweep:
         dev->writeLine( "linear_sweep" );
         break;
      case PMPrism::ConicSweep:
         dev->writeLine( "conic_sweep" );
         break;
   }
   dev->writeLine( QString( "%1, %2," ).arg( o->height1() ).arg( o->height2() ) );

   // count number of points
   QList< QList<PMVector> > points = o->points();
   QList< QList<PMVector> >::ConstIterator spit = points.constBegin();
   int lines = 0;
   for( ; spit != points.constEnd(); ++spit )
   {
      if( o->splineType() != PMPrism::BezierSpline )
         lines += ( *spit ).count() + 1;
      else
         lines += ( *spit ).count() / 3 * 4;
   }
   dev->writeLine( QString( "%1," ).arg( lines ) );

   for( spit = points.constBegin(); spit != points.constEnd(); ++spit )
   {
      bool first = true;

      QList<PMVector> fullPoints = o->expandedPoints( *spit );
      QList<PMVector>::ConstIterator it = fullPoints.constBegin();

      for( ; it != fullPoints.constEnd(); ++it )
      {
         if( !first )
            dev->write( ", " );
         dev->write( ( *it ).serialize() );
         first = false;
      }
      QList< QList<PMVector> >::ConstIterator spit2 = spit;
      spit2++;
      if( spit2 != points.constEnd() )
         dev->write( "," );
      dev->writeLine( "" );
   }

   if( o->open() )
      dev->writeLine( "open" );
   if( o->sturm() )
      dev->writeLine( "sturm" );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerQuickColor( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMQuickColor* o = ( PMQuickColor* ) object;

   dev->writeLine( "quick_color " + o->color().serialize() );
}

void PMPov31SerRainbow( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMRainbow* o = ( PMRainbow* ) object;

   QString str1;

   dev->objectBegin( "rainbow" );
   dev->callSerialization( object, metaObject->superClass() );

   if( o->isDirectionEnabled() )
      dev->writeLine( "direction " + o->direction().serialize() );
   if( o->isAngleEnabled() )
   {
      str1.setNum( o->angle() );
      dev->writeLine( "angle " + str1 );
   }
   if( o->isWidthEnabled() )
   {
      str1.setNum( o->width() );
      dev->writeLine( "width " + str1 );
   }
   if( o->isDistanceEnabled() )
   {
      str1.setNum( o->distance() );
      dev->writeLine( "distance " + str1 );
   }
   if( o->isJitterEnabled() )
   {
      str1.setNum( o->jitter() );
      dev->writeLine( "jitter " + str1 );
   }
   if( o->isUpEnabled() )
      dev->writeLine( "up " + o->up().serialize() );
   if( o->isArcAngleEnabled() )
   {
      str1.setNum( o->arcAngle() );
      dev->writeLine( "arc_angle " + str1 );
   }
   if( o->isFalloffAngleEnabled() )
   {
      str1.setNum( o->falloffAngle() );
      dev->writeLine( "falloff_angle " + str1 );
   }
   dev->objectEnd();
}

void PMPov31SerRaw( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMRaw* o = ( PMRaw* ) object;

   dev->writeLine( "//*PMRawBegin" );

   QString tmp = o->code();
   QTextStream str( &tmp, QIODevice::ReadOnly );
   while( !str.atEnd() )
      dev->writeLine( str.readLine() );

   dev->writeLine( "//*PMRawEnd" );
}

void PMPov31SerRotate( const PMObject* object, const PMMetaObject*, PMOutputDevice* dev )
{
   PMRotate* o = ( PMRotate* ) object;
   PMVector rotate = o->rotation();

   QString vector;
   QTextStream str( &vector, QIODevice::WriteOnly );
   int i;
   bool z[3];

   for( i = 0; i < 3; i++ )
      z[i] = approxZero( rotate[i] );
   if( !z[0] && z[1] && z[2] )
   {
      str << "x*";
      i = 0;
   }
   else if( z[0] && !z[1] && z[2] )
   {
      str << "y*";
      i = 1;
   }
   else if( z[0] && z[1] && !z[2] )
   {
      str << "z*";
      i = 2;
   }

   if( i < 3 )
   {
      if( rotate[i] > 0 )
         str << rotate[i];
      else
         str << "(" << rotate[i] << ")";
   }
   else
   {
      str << '<';
      for( i = 0; i < 3; i++ )
      {
         if( i > 0 )
            str << ", ";
         str << rotate[i];
      }
      str << '>';
   }

   dev->writeLine( "rotate " + vector );
}

void PMPov31SerScale( const PMObject* object, const PMMetaObject* , PMOutputDevice* dev )
{
   PMScale* o = ( PMScale* ) object;
   PMVector scale = o->scale();

   if( approx( scale[0], scale[1] ) &&
       approx( scale[1], scale[2] ) )
      dev->writeLine( QString( "scale %1" ).arg( scale[0] ) );
   else
      dev->writeLine( "scale " + scale.serialize() );
}

void PMPov31SerScene( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->callSerialization( object, metaObject->superClass() );
}

void PMPov31SerSkySphere( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "sky_sphere" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerSlope( const PMObject* object, const PMMetaObject* , PMOutputDevice* dev )
{
   PMSlope* o = ( PMSlope* ) object;

   QString str1,str2;

   str1.setNum(o->height());
   str2.setNum(o->slope());

   dev->writeLine( "<" + str1 + ", " + str2 + ">" );
}

void PMPov31SerSolidColor( const PMObject* object, const PMMetaObject* , PMOutputDevice* dev )
{
   PMSolidColor* o = ( PMSolidColor* ) object;

   dev->writeLine( o->color().serialize( true ) );
}

void PMPov31SerSolidObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMSolidObject* o = ( PMSolidObject* ) object;

   dev->callSerialization( object, metaObject->superClass() );
   switch( o->hollow() )
   {
      case PMTrue:
         dev->writeLine( "hollow" );
         break;
      case PMFalse:
         dev->writeLine( "hollow false" );
         break;
      case PMUnspecified:
         break;
   }
   if( o->inverse() )
      dev->writeLine( "inverse" );
}

void PMPov31SerSurfaceOfRevolution( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMSurfaceOfRevolution* o = ( PMSurfaceOfRevolution* ) object;

   dev->objectBegin( "sor" );

   dev->writeName( object->name() );

   int num = o->points().count();
   dev->writeLine( QString( "%1," ).arg( num ) );

   bool first = true;
   QList<PMVector> points = o->points();
   QList<PMVector>::ConstIterator it = points.constBegin();
   for( ; it != points.constEnd(); ++it )
   {
      if( !first )
         dev->write( ", " );
      dev->write( ( *it ).serialize() );
      first = false;
   }
   dev->writeLine( "" );

   if( o->open() )
      dev->writeLine( "open" );
   if( o->sturm() )
      dev->writeLine( "sturm" );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerSphere( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMSphere* o = ( PMSphere* ) object;

   dev->objectBegin( "sphere" );
   dev->writeName( object->name() );
   QString str;
   str.setNum( o->radius() );
   dev->writeLine( o->centre().serialize() + ", " + str );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerSuperquadricEllipsoid( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMSuperquadricEllipsoid* o = ( PMSuperquadricEllipsoid* ) object;

   dev->objectBegin( "superellipsoid" );

   dev->writeName( object->name() );
   dev->writeLine( QString( "<%1, %2>" ).arg( o->eastWestExponent() )
                  .arg( o->northSouthExponent() ) );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerText( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMText* o = ( PMText* ) object;

   dev->objectBegin( "text" );

   dev->writeName( object->name() );
   dev->writeLine( QString( "ttf \"" ) + o->font() + "\"" );
   dev->writeLine( PMOutputDevice::escapeAndQuoteString( o->text() ) );
   dev->writeLine( QString( "%1, " ).arg( o->thickness() )
                  + o->offset().serialize() );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerTexture( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMTexture* o = ( PMTexture* ) object;

   bool bObject = true;
   if( o->parent() )
      if( o->parent()->type() == "TextureMap" )
         bObject = false;

   if( bObject )
      dev->objectBegin( "texture" );
   dev->callSerialization( object, metaObject->superClass() );
   if( bObject )
      dev->objectEnd();
}

void PMPov31SerTextureBase( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMTextureBase* o = ( PMTextureBase* ) object;

   dev->writeName( object->name() );

   PMDeclare* linkedObject = o->linkedObject();

   if( linkedObject)
   {
      if( linkedObject->firstChild() )
         dev->writeLine( linkedObject->id() );
      else
      {
         QString text;
         text = o->name();
         if( text.isEmpty() )
            text = o->description();

         dev->writeComment( QString( "No prototype for %1" ).arg( text ) );
      }
   }

   dev->callSerialization( object, metaObject->superClass() );
}

void PMPov31SerTextureMapBase( const PMObject* object, const PMMetaObject* , PMOutputDevice* dev )
{
   PMTextureMapBase* o = ( PMTextureMapBase* ) object;

   QList<double> mapValues = o->mapValues();
   QList<double>::ConstIterator it = mapValues.constBegin();
   PMObject* c = o->firstChild();
   double value = 0.0;

   /* Take care of a possible link */
   if( o->linkedObject() )
   {
      if( o->linkedObject()->firstChild() )
         dev->writeLine( o->linkedObject()->id() );
      else
      {
         QString text;
         text = o->name();
         if( text.isEmpty() )
            text = o->description();

         dev->writeComment( QString( "No prototype for %1" ).arg( text ) );
      }
   }

   /* Serialize the map */
   for( ; c; c = c->nextSibling() )
   {
      if( c->type() == o->mapType() )
      {
         value = 1.0;
         if( it != mapValues.constEnd() )
            value = *it;
         dev->write( QString( "[ %1 " ).arg( value ) );
         dev->serialize( c );
         dev->writeLine( "]" );
         ++it;
      }
   }
}

void PMPov31SerTextureMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "texture_map" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerPigmentMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "pigment_map" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerColorMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "color_map" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerNormalMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "normal_map" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerSlopeMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "slope_map" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerDensityMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->objectBegin( "density_map" );
   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerTorus( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMTorus* o = ( PMTorus* ) object;

   dev->objectBegin( "torus" );
   dev->writeName( object->name() );
   QString strMinor;
   QString strMajor;
   strMinor.setNum( o->minorRadius() );
   strMajor.setNum( o->majorRadius() );

   dev->writeLine(strMajor + ", " + strMinor);
   if( o->sturm() )
      dev->writeLine( QString( "sturm" ) );

   dev->callSerialization( object, metaObject->superClass() );
   dev->objectEnd();
}

void PMPov31SerTranslate( const PMObject* object, const PMMetaObject* , PMOutputDevice* dev )
{
   PMTranslate* o = ( PMTranslate* ) object;

   QString vector;
   QTextStream str( &vector, QIODevice::WriteOnly );
   int i;
   bool z[3];
   PMVector move = o->translation();

   for( i = 0; i < 3; i++ )
      z[i] = approxZero( move[i] );
   if( !z[0] && z[1] && z[2] )
   {
      str << "x*";
      i = 0;
   }
   else if( z[0] && !z[1] && z[2] )
   {
      str << "y*";
      i = 1;
   }
   else if( z[0] && z[1] && !z[2] )
   {
      str << "z*";
      i = 2;
   }

   if( i < 3 )
   {
      if( move[i] > 0 )
         str << move[i];
      else
         str << "(" << move[i] << ")";
   }
   else
   {
      str << '<';
      for( i = 0; i < 3; i++ )
      {
         if( i > 0 )
            str << ", ";
         str << move[i];
      }
      str << '>';
   }

   dev->writeLine( "translate " + vector );
}

void PMPov31SerTriangle( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   PMTriangle* o = ( PMTriangle* ) object;

   if( o->isSmoothTriangle() )
   {
      dev->objectBegin( "smooth_triangle" );

      dev->writeName( object->name() );
      dev->writeLine( o->point( 0 ).serialize() + ", " + o->normal( 0 ).serialize() + ',' );
      dev->writeLine( o->point( 1 ).serialize() + ", " + o->normal( 1 ).serialize() + ',' );
      dev->writeLine( o->point( 2 ).serialize() + ", " + o->normal( 2 ).serialize() );

      dev->callSerialization( object, metaObject->superClass() );
      dev->objectEnd();
   }
   else
   {
      dev->objectBegin( "triangle" );

      dev->writeName( object->name() );
      dev->writeLine( o->point( 0 ).serialize() + ", " + o->point( 1 ).serialize()
                     + ", " + o->point( 2 ).serialize() );

      dev->callSerialization( object, metaObject->superClass() );
      dev->objectEnd();
   }
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

void PMPov31SerWarp( const PMObject* object, const PMMetaObject* , PMOutputDevice* dev )
{
   PMWarp* o = ( PMWarp* ) object;

   QString str1;

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
      default:
         break;
   }
   dev->objectEnd();
}

void PMPov31SerDetailObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev )
{
   dev->callSerialization( object, metaObject->superClass() );
}
