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


#ifndef PMBUMPMAP_H
#define PMBUMPMAP_H



#include "pmnamedobject.h"
#include "pmpalettevalue.h"

/**
 * Class for povray bumpmaps.
 */

class PMBumpMap : public PMNamedObject
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
    * Creates a PMBumpMap
    */
   PMBumpMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMBumpMap( const PMBumpMap& m );
   /**
    * deletes the PMBumpMap
    */
   virtual ~PMBumpMap();

   /** */
   virtual PMObject* copy() const { return new PMBumpMap( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmbumpmap" ); }

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMBumpMapEdit
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
    * Returns true if use_index is enabled
    */
   bool isUseIndexEnabled() const { return m_useIndex; }
   /**
    * Returns the bump size
    */
   double bumpSize()  const { return m_bumpSize; }
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
    * Sets the bumpmap type
    */
   void setBitmapType( PMBitmapType c );
   /**
    * Sets the bitmap file name*/
   void setBitmapFileName( const QString& c );
   /**
    * Sets if use_index is enabled
    */
   void enableUseIndex( bool c );
   /**
    * Set the bump size
    */
   void setBumpSize( double c );
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
   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMBumpMapMementoID { PMBitmapTypeID, PMBitmapFileID,
                             PMOnceID, PMMapTypeID, PMInterpolateID,
                             PMUseIndexID, PMBumpSizeID };
   /**
    * BumpMap type
    */
   PMBitmapType m_bitmapType;
   QString m_bitmapFile;
   bool m_once;
   PMMapType m_mapType;
   PMInterpolateType m_interpolateType;
   bool m_useIndex;
   double m_bumpSize;

   static PMMetaObject* s_pMetaObject;
};

#endif
