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


#include "pmimagemapedit.h"
#include "pmimagemap.h"
#include "pmpalettevalue.h"
#include "pmpalettevaluememento.h"

#include "pmxmlhelper.h"
#include "pmcompositeobject.h"
#include "pmmemento.h"
#include "pmenumproperty.h"


#include <QList>

const PMImageMap::PMBitmapType bitmapTypeDefault = PMImageMap::BitmapSys;
const char *const bitmapFileDefault = 0;
const bool enableFilterAllDefault = false;
const bool enableTransmitAllDefault = false;
const double filterAllDefault = 0.0;
const double transmitAllDefault = 0.0;
const bool onceDefault = false;
const PMImageMap::PMMapType mapTypeDefault = PMImageMap::MapPlanar;
const PMImageMap::PMInterpolateType interpolateTypeDefault = PMImageMap::InterpolateNone;

PMDefinePropertyClass( PMImageMap, PMImageMapProperty );
PMDefineEnumPropertyClass( PMImageMap, PMImageMap::PMBitmapType,
                           PMBitmapTypeProperty );
PMDefineEnumPropertyClass( PMImageMap, PMImageMap::PMInterpolateType,
                           PMInterpolateTypeProperty );
PMDefineEnumPropertyClass( PMImageMap, PMImageMap::PMMapType,
                           PMMapTypeProperty );

PMMetaObject* PMImageMap::s_pMetaObject = 0;
PMObject* createNewImageMap( PMPart* part )
{
   return new PMImageMap( part );
}

PMImageMap::PMImageMap( PMPart* part )
      : Base( part )
{
   m_bitmapType = bitmapTypeDefault;
   m_bitmapFile = bitmapFileDefault;
   m_enableFilterAll = enableFilterAllDefault;
   m_filterAll = filterAllDefault;
   m_enableTransmitAll = enableTransmitAllDefault;
   m_transmitAll = transmitAllDefault;
   m_once = onceDefault;
   m_mapType = mapTypeDefault;
   m_interpolateType = interpolateTypeDefault;
}

PMImageMap::PMImageMap( const PMImageMap& m )
      : Base( m )
{
   m_bitmapType = m.m_bitmapType;
   m_bitmapFile = m.m_bitmapFile;
   m_enableFilterAll = m.m_enableFilterAll;
   m_filterAll = m.m_filterAll;
   m_enableTransmitAll = m.m_enableTransmitAll;
   m_transmitAll = m.m_transmitAll;
   m_once = m.m_once;
   m_mapType = m.m_mapType;
   m_interpolateType = m.m_interpolateType;
   m_filters = m.m_filters;
   m_transmits = m.m_transmits;
}

PMImageMap::~PMImageMap()
{
}

void PMImageMap::serialize( QDomElement& e, QDomDocument& doc ) const
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
   e.setAttribute( "enable_filter_all", m_enableFilterAll );
   e.setAttribute( "filter_all", m_filterAll );
   e.setAttribute( "enable_transmit_all", m_enableTransmitAll );
   e.setAttribute( "transmit_all", m_transmitAll );
   e.setAttribute( "once", m_once );

   QDomElement extra_data = doc.createElement( "extra_data" );

   QDomElement data = doc.createElement( "indexed_filters" );
   QDomElement p;

   QList<PMPaletteValue>::ConstIterator it;
   for( it = m_filters.begin(); it != m_filters.end(); ++it )
   {
      p = doc.createElement( "index_filter" );
      ( *it ).serialize( p, doc );
      data.appendChild( p );
   }
   extra_data.appendChild( data );

   data = doc.createElement( "indexed_transmits" );
   for( it = m_transmits.begin(); it != m_transmits.end(); ++it )
   {
      p = doc.createElement( "index_transmit" );
      ( *it ).serialize( p, doc );
      data.appendChild( p );
   }
   extra_data.appendChild( data );

   e.appendChild( extra_data );

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

void PMImageMap::readAttributes( const PMXMLHelper& h )
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
   m_enableFilterAll = h.boolAttribute( "enable_filter_all", enableFilterAllDefault );
   m_filterAll = h.doubleAttribute( "filter_all", filterAllDefault );
   m_enableTransmitAll = h.boolAttribute( "enable_transmit_all", enableTransmitAllDefault );
   m_transmitAll = h.doubleAttribute( "transmit_all", transmitAllDefault );
   m_once = h.boolAttribute( "once", onceDefault );

   QDomElement e = h.extraData();
   if( !e.isNull() )
   {
      QDomNode c = e.firstChild();
      while( !c.isNull() )
      {
         if( c.isElement() )
         {
            QDomElement ce = c.toElement();
            if( ce.tagName() == "indexed_filters" )
            {
               m_filters.clear();
               QDomNode cd = ce.firstChild();
               while( !cd.isNull() )
               {
                  if( cd.isElement() )
                  {
                     PMPaletteValue pv;
                     pv.readAttributes( cd.toElement() );
                     m_filters.append( pv );
                  }
                  cd = cd.nextSibling();
               }
            }
            if( ce.tagName() == "indexed_transmits" )
            {
               m_transmits.clear();
               QDomNode cd = ce.firstChild();
               while( !cd.isNull() )
               {
                  if( cd.isElement() )
                  {
                     PMPaletteValue pv;
                     pv.readAttributes( cd.toElement() );
                     m_transmits.append( pv );
                  }
                  cd = cd.nextSibling();
               }
            }
         }
         c = c.nextSibling();
      }
   }

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

PMMetaObject* PMImageMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "ImageMap", Base::metaObject(),
                                        createNewImageMap );

      PMBitmapTypeProperty* bp = new PMBitmapTypeProperty(
         "bitmapType", &PMImageMap::setBitmapType, &PMImageMap::bitmapType );
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
         "interpolateType", &PMImageMap::setInterpolateType,
         &PMImageMap::interpolateType );
      ip->addEnumValue( "None", InterpolateNone );
      ip->addEnumValue( "Bilinear", InterpolateBilinear );
      ip->addEnumValue( "Normalized", InterpolateNormalized );
      s_pMetaObject->addProperty( ip );

      PMMapTypeProperty* mp = new PMMapTypeProperty(
         "mapType", &PMImageMap::setMapType, &PMImageMap::mapType );
      mp->addEnumValue( "Planar", MapPlanar );
      mp->addEnumValue( "Spherical", MapSpherical );
      mp->addEnumValue( "Cylindrical", MapCylindrical );
      mp->addEnumValue( "Toroidal", MapToroidal );
      s_pMetaObject->addProperty( mp );

      s_pMetaObject->addProperty(
         new PMImageMapProperty( "bitmapFile", &PMImageMap::setBitmapFileName,
                         &PMImageMap::bitmapFile ) );
      s_pMetaObject->addProperty(
         new PMImageMapProperty( "filterAllEnabled", &PMImageMap::enableFilterAll,
                         &PMImageMap::isFilterAllEnabled ) );
      s_pMetaObject->addProperty(
         new PMImageMapProperty( "filterAll", &PMImageMap::setFilterAll,
                         &PMImageMap::filterAll ) );
      s_pMetaObject->addProperty(
         new PMImageMapProperty( "transmitAllEnabled", &PMImageMap::enableTransmitAll,
                         &PMImageMap::isTransmitAllEnabled ) );
      s_pMetaObject->addProperty(
         new PMImageMapProperty( "transmitAll", &PMImageMap::setTransmitAll,
                         &PMImageMap::transmitAll ) );
      s_pMetaObject->addProperty(
         new PMImageMapProperty( "once", &PMImageMap::enableOnce, &PMImageMap::isOnceEnabled ) );

      // TODO: filters and transmits properties
   }
   return s_pMetaObject;
}

void PMImageMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMImageMap::description() const
{
   return ( "imagemap" );
}

void PMImageMap::setBitmapType( PMBitmapType c )
{
   if( c != m_bitmapType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBitmapTypeID, m_bitmapType );
      m_bitmapType = c;
   }
}

void PMImageMap::setBitmapFileName( const QString& c )
{
   if( c != m_bitmapFile )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBitmapFileID, m_bitmapFile );
      m_bitmapFile = c;
   }
}

void PMImageMap::enableFilterAll( bool c )
{
   if( c != m_enableFilterAll )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableFilterAllID, m_enableFilterAll );
      m_enableFilterAll = c;
   }
}

void PMImageMap::enableTransmitAll( bool c )
{
   if( c != m_enableTransmitAll )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableTransmitAllID, m_enableTransmitAll );
      m_enableTransmitAll = c;
   }
}

void PMImageMap::setFilterAll( double c )
{
   if( c != m_filterAll )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFilterAllID, m_filterAll );
      m_filterAll = c;
   }
}

void PMImageMap::setTransmitAll( double c )
{
   if( c != m_transmitAll )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFilterAllID, m_transmitAll );
      m_transmitAll = c;
   }
}

void PMImageMap::enableOnce( bool c )
{
   if( c != m_once )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOnceID, m_once );
      m_once = c;
   }
}

void PMImageMap::setMapType( PMMapType c )
{
   if( c != m_mapType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMapTypeID, m_mapType );
      m_mapType = c;
   }
}

void PMImageMap::setInterpolateType( PMInterpolateType c )
{
   if( c != m_interpolateType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMInterpolateID, m_interpolateType );
      m_interpolateType = c;
   }
}

void PMImageMap::setFilters( const QList<PMPaletteValue>& c )
{
   if( m_filters != c )
   {
      if( m_pMemento )
         ( ( PMPaletteValueMemento* ) m_pMemento )->setFilterPaletteValues( m_filters );
      m_filters = c;
   }
}

void PMImageMap::setTransmits( const QList<PMPaletteValue>& c )
{
   if( m_transmits != c )
   {
      if( m_pMemento )
         ( ( PMPaletteValueMemento* ) m_pMemento )->setTransmitPaletteValues( m_transmits );
      m_transmits = c;
   }
}

PMDialogEditBase* PMImageMap::editWidget( QWidget* parent ) const
{
   return new PMImageMapEdit( parent );
}

void PMImageMap::restoreMemento( PMMemento* s )
{
   PMPaletteValueMemento* m = ( PMPaletteValueMemento* ) s;
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
            case PMEnableFilterAllID:
               enableFilterAll( data->boolData() );
               break;
            case PMEnableTransmitAllID:
               enableTransmitAll( data->boolData() );
               break;
            case PMFilterAllID:
               setFilterAll( data->doubleData() );
               break;
            case PMTransmitAllID:
               setTransmitAll( data->doubleData() );
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
               qCritical(  ) << "Wrong ID in PMImageMap::restoreMemento\n";
               break;
         }
      }
   }

   if( m->filterPaletteValuesSaved() )
      setFilters( m->filterPaletteValues() );

   if( m->transmitPaletteValuesSaved() )
      setTransmits( m->transmitPaletteValues() );

   Base::restoreMemento( s );
}

