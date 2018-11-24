/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Passos Carvalho
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


#include "pmbumpmapedit.h"
#include "pmbumpmap.h"
#include "pmpalettevalue.h"
#include "pmpalettevaluememento.h"

#include "pmxmlhelper.h"
#include "pmcompositeobject.h"
#include "pmmemento.h"
#include "pmenumproperty.h"



const PMBumpMap::PMBitmapType bitmapTypeDefault = PMBumpMap::BitmapSys;
const char *const bitmapFileDefault = 0;
const bool enableFilterAllDefault = false;
const bool enableTransmitAllDefault = false;
const double filterAllDefault = 0.0;
const double transmitAllDefault = 0.0;
const bool onceDefault = false;
const PMBumpMap::PMMapType mapTypeDefault = PMBumpMap::MapPlanar;
const PMBumpMap::PMInterpolateType interpolateTypeDefault = PMBumpMap::InterpolateNone;
const bool useIndexDefault = false;
const double bumpSizeDefault = 0.0;

PMDefinePropertyClass( PMBumpMap, PMBumpMapProperty );
PMDefineEnumPropertyClass( PMBumpMap, PMBumpMap::PMBitmapType,
                           PMBitmapTypeProperty );
PMDefineEnumPropertyClass( PMBumpMap, PMBumpMap::PMInterpolateType,
                           PMInterpolateTypeProperty );
PMDefineEnumPropertyClass( PMBumpMap, PMBumpMap::PMMapType,
                           PMMapTypeProperty );

PMMetaObject* PMBumpMap::s_pMetaObject = 0;
PMObject* createNewBumpMap( PMPart* part )
{
   return new PMBumpMap( part );
}

PMBumpMap::PMBumpMap( PMPart* part )
      : Base( part )
{
   m_bitmapType = bitmapTypeDefault;
   m_bitmapFile = bitmapFileDefault;
   m_once = onceDefault;
   m_mapType = mapTypeDefault;
   m_interpolateType = interpolateTypeDefault;
   m_useIndex = useIndexDefault;
   m_bumpSize = bumpSizeDefault;
}

PMBumpMap::PMBumpMap( const PMBumpMap& m )
      : Base( m )
{
   m_bitmapType = m.m_bitmapType;
   m_bitmapFile = m.m_bitmapFile;
   m_once = m.m_once;
   m_mapType = m.m_mapType;
   m_interpolateType = m.m_interpolateType;
   m_useIndex = m.m_useIndex;
   m_bumpSize = m.m_bumpSize;
}

PMBumpMap::~PMBumpMap()
{
}

void PMBumpMap::serialize( QDomElement& e, QDomDocument& doc ) const
{
   switch( m_bitmapType )
   {
      case BitmapGif:
         e.setAttribute( "bitmap_type", "gif" );
         break;
      case BitmapTga:
         e.setAttribute( "bitmap_type", "tga" );
         break;
      case BitmapIff:
         e.setAttribute( "bitmap_type", "iff" );
         break;
      case BitmapPpm:
         e.setAttribute( "bitmap_type", "ppm" );
         break;
      case BitmapPgm:
         e.setAttribute( "bitmap_type", "pgm" );
         break;
      case BitmapPng:
         e.setAttribute( "bitmap_type", "png" );
         break;
      case BitmapJpeg:
         e.setAttribute( "bitmap_type", "jpeg" );
         break;
      case BitmapTiff:
         e.setAttribute( "bitmap_type", "tiff" );
         break;
      case BitmapSys:
         e.setAttribute( "bitmap_type", "sys" );
         break;
   }
   e.setAttribute( "file_name", m_bitmapFile );
   e.setAttribute( "once", m_once );
   switch( m_mapType )
   {
      case MapPlanar:
         e.setAttribute( "map_type", "planar" );
         break;
      case MapSpherical:
         e.setAttribute( "map_type", "spherical" );
         break;
      case MapCylindrical:
         e.setAttribute( "map_type", "cylindrical" );
         break;
      case MapToroidal:
         e.setAttribute( "map_type", "toroidal" );
         break;
   }
   switch( m_interpolateType )
   {
      case InterpolateNone:
         e.setAttribute( "interpolate", "none" );
         break;
      case InterpolateBilinear:
         e.setAttribute( "interpolate", "bilinear" );
         break;
      case InterpolateNormalized:
         e.setAttribute( "interpolate", "normalized" );
         break;
   }
   e.setAttribute( "use_index", m_useIndex );
   e.setAttribute( "bump_size", m_bumpSize );

   Base::serialize( e, doc );
}

void PMBumpMap::readAttributes( const PMXMLHelper& h )
{
   QString str;

   str = h.stringAttribute( "bitmap_type", "sys" );
   if( str == "gif" )
      m_bitmapType = BitmapGif;
   else if( str == "tga" )
      m_bitmapType = BitmapTga;
   else if( str == "iff" )
      m_bitmapType = BitmapIff;
   else if( str == "ppm" )
      m_bitmapType = BitmapPpm;
   else if( str == "pgm" )
      m_bitmapType = BitmapPgm;
   else if( str == "png" )
      m_bitmapType = BitmapPng;
   else if( str == "jpeg" )
      m_bitmapType = BitmapJpeg;
   else if( str == "tiff" )
      m_bitmapType = BitmapTiff;
   else if( str == "sys" )
      m_bitmapType = BitmapSys;

   m_bitmapFile = h.stringAttribute( "file_name", bitmapFileDefault );
   m_once = h.boolAttribute( "once", onceDefault );

   str = h.stringAttribute( "map_type", "planar" );
   if( str == "planar" )
      m_mapType = MapPlanar;
   else if( str == "spherical" )
      m_mapType = MapSpherical;
   else if( str == "cylindrical" )
      m_mapType = MapCylindrical;
   else if( str == "toroidal" )
      m_mapType = MapToroidal;

   str = h.stringAttribute( "interpolate", "none" );
   if( str == "none" )
      m_interpolateType = InterpolateNone;
   else if( str == "bilinear" )
      m_interpolateType = InterpolateBilinear;
   else if( str == "normalized" )
      m_interpolateType = InterpolateNormalized;

   m_useIndex = h.boolAttribute( "use_index", useIndexDefault );
   m_bumpSize = h.doubleAttribute( "bump_size", bumpSizeDefault );

   Base::readAttributes( h );
}

PMMetaObject* PMBumpMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "BumpMap", Base::metaObject(),
                                        createNewBumpMap );

      PMBitmapTypeProperty* bp = new PMBitmapTypeProperty(
         "bitmapType", &PMBumpMap::setBitmapType,
         &PMBumpMap::bitmapType );
      bp->addEnumValue( "Gif", BitmapGif );
      bp->addEnumValue( "Tga", BitmapTga );
      bp->addEnumValue( "Iff", BitmapIff );
      bp->addEnumValue( "Ppm", BitmapPpm );
      bp->addEnumValue( "Pgm", BitmapPgm );
      bp->addEnumValue( "Png", BitmapPng );
      bp->addEnumValue( "Jpeg", BitmapJpeg );
      bp->addEnumValue( "Tiff", BitmapTiff );
      bp->addEnumValue( "Sys", BitmapSys );
      s_pMetaObject->addProperty( bp );

      PMInterpolateTypeProperty* ip = new PMInterpolateTypeProperty(
         "interpolateType", &PMBumpMap::setInterpolateType,
         &PMBumpMap::interpolateType );
      ip->addEnumValue( "None", InterpolateNone );
      ip->addEnumValue( "Bilinear", InterpolateBilinear );
      ip->addEnumValue( "Normalized", InterpolateNormalized );
      s_pMetaObject->addProperty( ip );

      PMMapTypeProperty* mp = new PMMapTypeProperty(
         "mapType", &PMBumpMap::setMapType, &PMBumpMap::mapType );
      mp->addEnumValue( "Planar", MapPlanar );
      mp->addEnumValue( "Spherical", MapSpherical );
      mp->addEnumValue( "Cylindrical", MapCylindrical );
      mp->addEnumValue( "Toroidal", MapToroidal );
      s_pMetaObject->addProperty( mp );

      s_pMetaObject->addProperty(
         new PMBumpMapProperty( "bitmapFile", &PMBumpMap::setBitmapFileName,
                         &PMBumpMap::bitmapFile ) );
      s_pMetaObject->addProperty(
         new PMBumpMapProperty( "useIndex", &PMBumpMap::enableUseIndex,
                         &PMBumpMap::isUseIndexEnabled ) );
      s_pMetaObject->addProperty(
         new PMBumpMapProperty( "once", &PMBumpMap::enableOnce,
                         &PMBumpMap::isOnceEnabled ) );
   }
   return s_pMetaObject;
}

void PMBumpMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}


QString PMBumpMap::description() const
{
   return ( "bump map" );
}

void PMBumpMap::setBitmapType( PMBitmapType c )
{
   if( c != m_bitmapType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBitmapTypeID, m_bitmapType );
      m_bitmapType = c;
   }
}

void PMBumpMap::setBitmapFileName( const QString& c )
{
   if( c != m_bitmapFile )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBitmapFileID, m_bitmapFile );
      m_bitmapFile = c;
   }
}

void PMBumpMap::enableUseIndex( bool c )
{
   if( c != m_useIndex )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUseIndexID, m_useIndex );
      m_useIndex = c;
   }
}

void PMBumpMap::setMapType( PMMapType c )
{
   if( c != m_mapType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMapTypeID, m_mapType );
      m_mapType = c;
   }
}

void PMBumpMap::setInterpolateType( PMInterpolateType c )
{
   if( c != m_interpolateType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMInterpolateID, m_interpolateType );
      m_interpolateType = c;
   }
}

void PMBumpMap::enableOnce( bool c )
{
   if( c != m_once )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOnceID, m_once );
      m_once = c;
   }
}

void PMBumpMap::setBumpSize( double c )
{
   if( c != m_bumpSize )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBumpSizeID, m_bumpSize );
      m_bumpSize = c;
   }
}

PMDialogEditBase* PMBumpMap::editWidget( QWidget* parent ) const
{
   return new PMBumpMapEdit( parent );
}

void PMBumpMap::restoreMemento( PMMemento* s )
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
            case PMBitmapTypeID:
               setBitmapType( ( PMBitmapType )data->intData() );
               break;
            case PMBitmapFileID:
               setBitmapFileName( data->stringData() );
               break;
            case PMOnceID:
               enableOnce( data->boolData() );
               break;
            case PMMapTypeID:
               setMapType( ( PMMapType )data->intData() );
               break;
            case PMInterpolateID:
               setInterpolateType( ( PMInterpolateType )data->intData() );
               break;
            case PMUseIndexID:
               enableUseIndex( data->boolData() );
               break;
            case PMBumpSizeID:
               setBumpSize( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMBumpMap::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
