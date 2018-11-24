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


#include "pmmaterialmapedit.h"
#include "pmmaterialmap.h"

#include "pmxmlhelper.h"
#include "pmcompositeobject.h"
#include "pmmemento.h"
#include "pmenumproperty.h"



const PMMaterialMap::PMBitmapType bitmapTypeDefault = PMMaterialMap::BitmapSys;
const char *const bitmapFileDefault = 0;
const bool enableFilterAllDefault = false;
const bool enableTransmitAllDefault = false;
const double filterAllDefault = 0.0;
const double transmitAllDefault = 0.0;
const bool onceDefault = false;
const PMMaterialMap::PMMapType mapTypeDefault = PMMaterialMap::MapPlanar;
const PMMaterialMap::PMInterpolateType interpolateTypeDefault = PMMaterialMap::InterpolateNone;

PMDefinePropertyClass( PMMaterialMap, PMMaterialMapProperty );
PMDefineEnumPropertyClass( PMMaterialMap, PMMaterialMap::PMBitmapType,
                           PMBitmapTypeProperty );
PMDefineEnumPropertyClass( PMMaterialMap, PMMaterialMap::PMInterpolateType,
                           PMInterpolateTypeProperty );
PMDefineEnumPropertyClass( PMMaterialMap, PMMaterialMap::PMMapType,
                           PMMapTypeProperty );

PMMetaObject* PMMaterialMap::s_pMetaObject = 0;
PMObject* createNewMaterialMap( PMPart* part )
{
   return new PMMaterialMap( part );
}

PMMaterialMap::PMMaterialMap( PMPart* part )
      : Base( part )
{
   m_bitmapType = bitmapTypeDefault;
   m_bitmapFile = bitmapFileDefault;
   m_once = onceDefault;
   m_mapType = mapTypeDefault;
   m_interpolateType = interpolateTypeDefault;
}

PMMaterialMap::PMMaterialMap( const PMMaterialMap& m )
      : Base( m )
{
   m_bitmapType = m.m_bitmapType;
   m_bitmapFile = m.m_bitmapFile;
   m_once = m.m_once;
   m_mapType = m.m_mapType;
   m_interpolateType = m.m_interpolateType;
}

PMMaterialMap::~PMMaterialMap()
{
}

void PMMaterialMap::serialize( QDomElement& e, QDomDocument& doc ) const
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
   Base::serialize( e, doc );
}

void PMMaterialMap::readAttributes( const PMXMLHelper& h )
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

   Base::readAttributes( h );
}

PMMetaObject* PMMaterialMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "MaterialMap", Base::metaObject(),
                                        createNewMaterialMap );

      PMBitmapTypeProperty* bp = new PMBitmapTypeProperty(
         "bitmapType", &PMMaterialMap::setBitmapType, &PMMaterialMap::bitmapType );
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
         "interpolateType", &PMMaterialMap::setInterpolateType,
         &PMMaterialMap::interpolateType );
      ip->addEnumValue( "None", InterpolateNone );
      ip->addEnumValue( "Bilinear", InterpolateBilinear );
      ip->addEnumValue( "Normalized", InterpolateNormalized );
      s_pMetaObject->addProperty( ip );

      PMMapTypeProperty* mp = new PMMapTypeProperty(
         "mapType", &PMMaterialMap::setMapType, &PMMaterialMap::mapType );
      mp->addEnumValue( "Planar", MapPlanar );
      mp->addEnumValue( "Spherical", MapSpherical );
      mp->addEnumValue( "Cylindrical", MapCylindrical );
      mp->addEnumValue( "Toroidal", MapToroidal );
      s_pMetaObject->addProperty( mp );

      s_pMetaObject->addProperty(
         new PMMaterialMapProperty( "bitmapFile", &PMMaterialMap::setBitmapFileName,
                         &PMMaterialMap::bitmapFile ) );
      s_pMetaObject->addProperty(
         new PMMaterialMapProperty( "once", &PMMaterialMap::enableOnce, &PMMaterialMap::isOnceEnabled ) );
   }
   return s_pMetaObject;
}

void PMMaterialMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMMaterialMap::description() const
{
   return ( "material map" );
}

void PMMaterialMap::setBitmapType( PMBitmapType c )
{
   if( c != m_bitmapType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBitmapTypeID, m_bitmapType );
      m_bitmapType = c;
   }
}

void PMMaterialMap::setBitmapFileName( const QString& c )
{
   if( c != m_bitmapFile )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBitmapFileID, m_bitmapFile );
      m_bitmapFile = c;
   }
}

void PMMaterialMap::setMapType( PMMapType c )
{
   if( c != m_mapType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMapTypeID, m_mapType );
      m_mapType = c;
   }
}

void PMMaterialMap::setInterpolateType( PMInterpolateType c )
{
   if( c != m_interpolateType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMInterpolateID, m_interpolateType );
      m_interpolateType = c;
   }
}

void PMMaterialMap::enableOnce( bool c )
{
   if( c != m_once )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOnceID, m_once );
      m_once = c;
   }
}

PMDialogEditBase* PMMaterialMap::editWidget( QWidget* parent ) const
{
   return new PMMaterialMapEdit( parent );
}

void PMMaterialMap::restoreMemento( PMMemento* s )
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
            default:
               qCritical(  ) << "Wrong ID in PMMaterialMap::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
