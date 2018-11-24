//-*-C++-*-
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


#ifndef PMIMAGEMAP_H
#define PMIMAGEMAP_H



#include "pmnamedobject.h"
#include "pmpalettevalue.h"
#include <QList>

/**
 * Class for povray imagemaps.
 */

class PMImageMap : public PMNamedObject
{
   typedef PMNamedObject Base;
public:
   /**
    * The bitmap type
    */
   enum PMBitmapType { BitmapGif, BitmapTga, BitmapIff, BitmapPpm,
	               BitmapPgm, BitmapPng, BitmapJpeg, BitmapTiff,
		       BitmapSys };
   /**
    * The interpolate method
    */
   enum PMInterpolateType { InterpolateNone, InterpolateBilinear,
	                    InterpolateNormalized };
   /**
    * The mapping method
    */
   enum PMMapType { MapPlanar, MapSpherical, MapCylindrical,
	            MapToroidal };

   /**
    * Creates a PMImageMap
    */
   PMImageMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMImageMap( const PMImageMap& im );
   /**
    * deletes the PMImageMap
    */
   virtual ~PMImageMap();

   /** */
   virtual PMObject* copy() const { return new PMImageMap( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmimagemap" ); }

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMImageMapEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;

   /**
    * Gets the bitmap type
    */
   PMBitmapType bitmapType() const { return m_bitmapType; }
   /**
    * Gets the bitmap file name
    */
   QString bitmapFile() const { return m_bitmapFile; }
   /**
    * Returns true if filter all is enabled
    */
   bool isFilterAllEnabled() const { return m_enableFilterAll; }
   /**
    * Gets the value of filter for all colors
    */
   double filterAll() const { return m_filterAll; }
   /**
    * Returns true if transmit all is enabled
    */
   bool isTransmitAllEnabled() const { return m_enableTransmitAll; }
   /**
    * Gets the value of transmit all for all colors
    */
   double transmitAll() const { return m_transmitAll; }
   /**
    * Returns true if once is enabled
    */
   bool isOnceEnabled() const { return m_once; }
   /**
    * Gets the bitmap file type
    */
   PMMapType mapType() const { return m_mapType; }
   /**
    * Gets the interpolate method type
    */
   PMInterpolateType interpolateType() const { return m_interpolateType; }
   /**
    * Gets the list of indexed filters
    */
   QList<PMPaletteValue> filters() const { return m_filters; }
   /**
    * Gets the list of indexed transmits
    */
   QList<PMPaletteValue> transmits() const { return m_transmits; }

   /**
    * Sets the imagemap type
    */
   void setBitmapType( PMBitmapType c );
   /**
    * Sets the bitmap file name*/
   void setBitmapFileName( const QString& c );
   /**
    * Enables/Disables Filter All
    */
   void enableFilterAll( bool c );
   /**
    * Sets the filter all value
    */
   void setFilterAll( double c );
   /**
    * Enables/Disables Transmit All
    */
   void enableTransmitAll( bool c );
   /**
    * Sets the transmit all value
    */
   void setTransmitAll( double c );
   /**
    * Sets if the bitmap should be mapped once
    */
   void enableOnce( bool c );
   /**
    * Sets the mapping method
    */
   void setMapType( const PMMapType c );
   /**
    * Sets the interpolating method
    */
   void setInterpolateType( PMInterpolateType c );
   /**
    * Set the list of indexed filters
    */
   void setFilters( const QList<PMPaletteValue>& filters );
   /**
    * Set the list of indexed transmits
    */
   void setTransmits( const QList<PMPaletteValue>& transmits );
   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMImageMapMementoID { PMBitmapTypeID, PMBitmapFileID,
                              PMEnableFilterAllID,
			      PMEnableTransmitAllID,
                              PMFilterAllID, PMTransmitAllID,
                              PMOnceID, PMMapTypeID,
                              PMInterpolateID };
   /**
    * ImageMap type
    */
   PMBitmapType m_bitmapType;
   QString m_bitmapFile;
   bool m_enableFilterAll;
   float m_filterAll;
   bool m_enableTransmitAll;
   float m_transmitAll;
   bool m_once;
   PMMapType m_mapType;
   PMInterpolateType m_interpolateType;
   QList<PMPaletteValue> m_filters;
   QList<PMPaletteValue> m_transmits;

   static PMMetaObject* s_pMetaObject;
};

#endif
