//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
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


#ifndef PMMEDIA_H
#define PMMEDIA_H



#include "pmtexturebase.h"
#include "pmcolor.h"

/**
 * Class for povray medias
 */
class PMMedia : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMMedia
    */
   PMMedia( PMPart* part );
   /**
    * Copy constructor
    */
   PMMedia( const PMMedia& m );
   /**
    * Deletes the object
    */
   virtual ~PMMedia();

   /** */
   virtual PMObject* copy() const { return new PMMedia( *this ); }
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
    * Returns a new @ref PMMediaEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmmedia" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );

   int method() const { return m_method; }
   int intervals() const { return m_intervals; }
   int samplesMin() const { return m_samplesMin; }
   int samplesMax() const { return m_samplesMax; }
   double confidence() const { return m_confidence; }
   double variance() const { return m_variance; }
   double ratio() const { return m_ratio; }
   int aaLevel() const { return m_aaLevel; }
   double aaThreshold() const { return m_aaThreshold; }
   PMColor absorption() const { return m_absorption; }
   PMColor emission() const { return m_emission; }
   int scatteringType() const { return m_scatteringType; }
   PMColor scatteringColor() const { return m_scatteringColor; }
   double scatteringEccentricity() const { return m_scatteringEccentricity; }
   double scatteringExtinction() const { return m_scatteringExtinction; }
   bool isAbsorptionEnabled() const { return m_enableAbsorption; }
   bool isEmissionEnabled() const { return m_enableEmission; }
   bool isScatteringEnabled() const { return m_enableScattering; }

   void setMethod( int c );
   void setIntervals( int c );
   void setSamplesMin( int c );
   void setSamplesMax( int c );
   void setAALevel( int c );
   void setConfidence( double c );
   void setVariance( double c );
   void setRatio( double c );
   void setAAThreshold( double c );
   void setAbsorption( const PMColor& c );
   void setEmission( const PMColor& c );
   void setScatteringType( int c );
   void setScatteringColor( const PMColor& c );
   void setScatteringEccentricity( double c );
   void setScatteringExtinction( double c );
   void enableAbsorption( bool c );
   void enableEmission( bool c );
   void enableScattering( bool c );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMMediaMementoID { PMMethodID, PMIntervalsID, PMSamplesMinID, PMSamplesMaxID,
                            PMConfidenceID, PMVarianceID, PMRatioID,
                            PMAALevelID, PMAAThresholdID, PMAbsorptionID, PMEmissionID,
                            PMScatteringTypeID, PMScatteringColorID,
                            PMScatteringEccentricityID, PMScatteringExtinctionID,
                            PMEnableAbsorptionID, PMEnableEmissionID,
                            PMEnableScatteringID };

   int m_method;
   int m_intervals;
   int m_samplesMin;
   int m_samplesMax;
   double m_confidence;
   double m_variance;
   double m_ratio;
   int m_aaLevel;
   double m_aaThreshold;
   PMColor m_absorption;
   PMColor m_emission;
   int m_scatteringType;
   PMColor m_scatteringColor;
   double m_scatteringEccentricity;
   double m_scatteringExtinction;

   bool m_enableAbsorption;
   bool m_enableEmission;
   bool m_enableScattering;

   static PMMetaObject* s_pMetaObject;
};


#endif
