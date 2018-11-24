//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMGLOBALPHOTONS_H
#define PMGLOBALPHOTONS_H



#include "pmobject.h"

/**
 * Class for GlobalPhotons settings.
 */

class PMGlobalPhotons : public PMObject
{
   typedef PMObject Base;
public:
   enum PMNumberType { Spacing=0, Count=1 };

   /**
    * Creates a PMGlobalPhotons
    */
   PMGlobalPhotons( PMPart* part );
   /**
    * Copy constructor
    */
   PMGlobalPhotons( const PMGlobalPhotons& p );
   /**
    * deletes the PMGlobalPhotons
    */
   virtual ~PMGlobalPhotons();

   /** */
   virtual PMObject* copy() const { return new PMGlobalPhotons( *this ); }
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
    * Returns a new @ref PMGlobalPhotonsEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmglobalphotons" ); }

   /**
    * Returns the number type
    */
   PMNumberType numberType() const { return m_numberType; }
   /**
    * Sets the number type
    */
   void setNumberType( PMNumberType nt );

   /**
    * Returns the photons spacing value
    */
   double spacing() const { return m_spacing; }
   /**
    * Sets the photons spacing value
    */
   void setSpacing( double s );

   /**
    * Returns the number of photons to shoot
    */
   int count() const { return m_count; }
   /**
    * Sets the number of photons to shoot
    */
   void setCount( int c );

   /**
    * Returns the minimum gather
    */
   int gatherMin() const { return m_gatherMin; }
   /**
    * Sets the minimum gather
    */
   void setGatherMin( int gm );

   /**
    * Returns the maximum gather
    */
   int gatherMax() const { return m_gatherMax; }
   /**
    * Sets the maximum gather
    */
   void setGatherMax( int gm );

   /**
    * Returns the media maximum steps
    */
   int mediaMaxSteps() const { return m_mediaMaxSteps; }
   /**
    * Sets the media maximum steps
    */
   void setMediaMaxSteps( int mms );

   /**
    * Returns the media factor
    */
   double mediaFactor() const { return m_mediaFactor; }
   /**
    * Sets the media factor
    */
   void setMediaFactor( double mf );

   /**
    * Returns jitter amount
    */
   double jitter() const { return m_jitter; }
   /**
    * Sets the jitter amount
    */
   void setJitter( double j );

   /**
    * Returns the maximum trace level global flag
    */
   bool maxTraceLevelGlobal() const { return m_maxTraceLevelGlobal; }
   /**
    * Sets the maximum trace level global flag
    */
   void setMaxTraceLevelGlobal( bool mtlg );

   /**
    * Returns the maximum trace level
    */
   int maxTraceLevel() const { return m_maxTraceLevel; }
   /**
    * Sets the maximum trace level
    */
   void setMaxTraceLevel( int mtl );

   /**
    * Returns the adc bailout global flag
    */
   bool adcBailoutGlobal() const { return m_adcBailoutGlobal; }
   /**
    * Sets the adc bailout global flag
    */
   void setAdcBailoutGlobal( bool abg );

   /**
    * Returns the adc bailout
    */
   double adcBailout() const { return m_adcBailout; }
   /**
    * Sets the adc bailout
    */
   void setAdcBailout( double ab );

   /**
    * Returns the autostop fraction
    */
   double autostop() const { return m_autostop; }
   /**
    * Sets the autostop fraction
    */
   void setAutostop( double a );

   /**
    * Returns the expand threshold percent increase
    */
   double expandIncrease() const { return m_expandIncrease; }
   /**
    * Sets the expand threshold percent increase
    */
   void setExpandIncrease( double ei );

   /**
    * Returns the expand threshold minimum
    */
   int expandMin() const { return m_expandMin; }
   /**
    * Sets the expand threshold minimum
    */
   void setExpandMin( int em );

   /**
    * Retuens the gather radius
    */
   double radiusGather() const { return m_radiusGather; }
   /**
    * Sets the gather radius
    */
   void setRadiusGather( double rg );

   /**
    * Returns the gather radius multiplier
    */
   double radiusGatherMulti() const { return m_radiusGatherMulti; }
   /**
    * Sets the radius gather multiplier
    */
   void setRadiusGatherMulti( double rgm );

   /**
    * Returns the media gather radius
    */
   double radiusMedia() const { return m_radiusMedia; }
   /**
    * Sets the media gather radius
    */
   void setRadiusMedia( double rm );

   /**
    * Returns the media gather radius multiplier
    */
   double radiusMediaMulti() const { return m_radiusMediaMulti; }
   /**
    * Sets the media gather radius multiplier
    */
   void setRadiusMediaMulti( double rmm );

   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMGlobalPhotonsMementoID
   { PMNumberTypeID, PMSpacingID, PMCountID, PMGatherMinID, PMGatherMaxID,
     PMMediaMaxStepsID, PMMediaFactorID, PMJitterID, PMMaxTraceLevelGlobalID,
     PMMaxTraceLevelID, PMAdcBailoutGlobalID, PMAdcBailoutID, PMAutostopID,
     PMExpandIncreaseID, PMExpandMinID, PMRadiusGatherID, PMRadiusGatherMultiID,
     PMRadiusMediaID, PMRadiusMediaMultiID };

   PMNumberType m_numberType;
   double m_spacing;
   int m_count;
   int m_gatherMin;
   int m_gatherMax;
   int m_mediaMaxSteps;
   double m_mediaFactor;
   double m_jitter;
   bool m_maxTraceLevelGlobal;
   int m_maxTraceLevel;
   bool m_adcBailoutGlobal;
   double m_adcBailout;
   double m_autostop;
   double m_expandIncrease;
   int m_expandMin;
   double m_radiusGather;
   double m_radiusGatherMulti;
   double m_radiusMedia;
   double m_radiusMediaMulti;
   
   static PMMetaObject* s_pMetaObject;
};

#endif
