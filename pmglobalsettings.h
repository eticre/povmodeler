//-*-C++-*-
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


#ifndef PMGLOBALSETTINGS_H
#define PMGLOBALSETTINGS_H



#include "pmcompositeobject.h"
#include "pmcolor.h"

/**
 * Class for global settings.
 */

class PMGlobalSettings : public PMCompositeObject
{
   typedef PMCompositeObject Base;
public:
   enum PMNoiseType { Original = 0, RangeCorrected = 1, Perlin = 2 };

   /**
    * Creates a PMGlobalSettings
    */
   PMGlobalSettings( PMPart* part );
   /**
    * Copy constructor
    */
   PMGlobalSettings( const PMGlobalSettings& s );
   /**
    * deletes the PMGlobalSettings
    */
   virtual ~PMGlobalSettings();

   /** */
   virtual PMObject* copy() const { return new PMGlobalSettings( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMGlobalSettingsEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmglobalsettings" ); }

   /**
    * Returns the adc bailout
    */
   double adcBailout() const { return m_adcBailout; }
   /**
    * Sets the adc bailout
    */
   void setAdcBailout( double c );
   /**
    * Returns the ambient color
    */
   PMColor ambientLight() const { return m_ambientLight; }
   /**
    * Sets the ambient color
    */
   void setAmbientLight( const PMColor& c );
   /**
    * Returns the assumed gamma
    */
   double assumedGamma() const { return m_assumedGamma; }
   /**
    * Sets the assumed gamma
    */
   void setAssumedGamma( double c );
   /**
    * Returns the hf gray 16
    */
   bool hfGray16() const { return m_hfGray16; }
   /**
    * Sets the hf gray 16
    */
   void setHfGray16( bool c );
   /**
    * Returns the iridiscence wavelength
    */
   PMColor iridWaveLength() const { return m_iridWaveLength; }
   /**
    * Sets the iridiscence wavelength
    */
   void setIridWaveLength( const PMColor& c );
   /**
    * Returns the maximum number of intersections
    */
   int maxIntersections() const { return m_maxIntersections; }
   /**
    * Sets the maximum number of intersections
    */
   void setMaxIntersections( int c );
   /**
    * Returns the maximum trace level
    */
   int maxTraceLevel() const { return m_maxTraceLevel; }
   /**
    * Sets the maximum trace level
    */
   void setMaxTraceLevel( int c );
   /**
    * Returns the number of waves
    */
   int numberWaves() const { return m_numberWaves; }
   /**
    * Sets the number of waves
    */
   void setNumberWaves( int c );
   /**
    * Returns the current noise generator
    */
   PMNoiseType noiseGenerator() const { return m_noiseGenerator; }
   /**
    * Sets the noise generator
    */
   void setNoiseGenerator( PMNoiseType c );
   /**
    * Returns true if radiosity is enabled
    */
   bool isRadiosityEnabled() const { return m_radiosityEnabled; }
   /**
    * Enables/Disables radiosity
    */
   void enableRadiosity( bool c );
   /**
    * Returns brightness
    */
   double brightness() const { return m_brightness; }
   /**
    * Sets the brightness
    */
   void setBrightness( double c );
   /**
    * Returns count
    */
   int count() const { return m_count; }
   /**
    * Sets the count
    */
   void setCount( int c );
   /**
    * Returns maximum distance
    */
   double distanceMaximum() const { return m_distanceMaximum; }
   /**
    * Sets the maximum distance
    */
   void setDistanceMaximum( double c );
   /**
    * Returns error boundary
    */
   double errorBound() const { return m_errorBound; }
   /**
    * Sets the error boundary
    */
   void setErrorBound( double c );
   /**
    * Returns gray threshold
    */
   double grayThreshold() const { return m_grayThreshold; }
   /**
    * Sets the gray threshold
    */
   void setGrayThreshold( double c );
   /**
    * Returns low error factor
    */
   double lowErrorFactor() const { return m_lowErrorFactor; }
   /**
    * Sets the low error factor
    */
   void setLowErrorFactor( double c );
   /**
    * Returns minimum reuse
    */
   double minimumReuse() const { return m_minimumReuse; }
   /**
    * Sets the minimum reuse
    */
   void setMinimumReuse( double c );
   /**
    * Returns nearest count
    */
   int nearestCount() const { return m_nearestCount; }
   /**
    * Sets the nearest count
    */
   void setNearestCount( int c );
   /**
    * Returns recursion limit
    */
   int recursionLimit() const { return m_recursionLimit; }
   /**
    * Sets the recursion limit
    */
   void setRecursionLimit( int c );

   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMGlobalSettingsMementoID { PMAdcBailoutID , PMAmbientLightID, PMAssumedGammaID,
                                    PMHfGray16ID, PMIridWaveLengthID, PMMaxIntersectionsID,
                                    PMMaxTraceLevelID, PMNumberWavesID, PMNoiseGeneratorID,
                                    PMRadiosityEnabledID, PMBrightnessID, PMCountID,
                                    PMDistanceMaximumID, PMErrorBoundID, PMGrayThresholdID,
                                    PMLowErrorFactorID, PMMinimumReuseID, PMNearestCountID,
                                    PMRecursionLimitID };

   double      m_adcBailout;
   PMColor     m_ambientLight;
   double      m_assumedGamma;
   bool        m_hfGray16;
   PMColor     m_iridWaveLength;
   int         m_maxIntersections;
   int         m_maxTraceLevel;
   int         m_numberWaves;
   PMNoiseType m_noiseGenerator;
   bool        m_radiosityEnabled;
   double      m_brightness;
   int         m_count;
   double      m_distanceMaximum;
   double      m_errorBound;
   double      m_grayThreshold;
   double      m_lowErrorFactor;
   double      m_minimumReuse;
   int         m_nearestCount;
   int         m_recursionLimit;

   static PMMetaObject* s_pMetaObject;
};

#endif
