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


#ifndef PMRADIOSITY_H
#define PMRADIOSITY_H



#include "pmobject.h"

/**
 * Class for radiosity settings.
 */

class PMRadiosity : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates a PMRadiosity
    */
   PMRadiosity( PMPart* part );
   /**
    * Copy constructor
    */
   PMRadiosity( const PMRadiosity& r );
   /**
    * deletes the PMRadiosity
    */
   virtual ~PMRadiosity();

   /** */
   virtual PMObject* copy() const { return new PMRadiosity( *this ); }
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
    * Returns a new @ref PMRadiosityEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmradiosity" ); }

   /**
    * Returns the adc bailout
    */
   double adcBailout() const { return m_adcBailout; }
   /**
    * Sets the adc bailout
    */
   void setAdcBailout( double ab );

   /**
    * Returns the always sample flag
    */
   bool alwaysSample() const { return m_alwaysSample; }
   /**
    * Sets the always sample flag
    */
   void setAlwaysSample( bool as );

   /**
    * Returns brightness
    */
   double brightness() const { return m_brightness; }
   /**
    * Sets the brightness
    */
   void setBrightness( double b );

   /**
    * Returns count
    */
   int count() const { return m_count; }
   /**
    * Sets the count
    */
   void setCount( int c );

   /**
    * Returns error boundary
    */
   double errorBound() const { return m_errorBound; }
   /**
    * Sets the error boundary
    */
   void setErrorBound( double eb );

   /**
    * Returns gray threshold
    */
   double grayThreshold() const { return m_grayThreshold; }
   /**
    * Sets the gray threshold
    */
   void setGrayThreshold( double gt );

   /**
    * Returns low error factor
    */
   double lowErrorFactor() const { return m_lowErrorFactor; }
   /**
    * Sets the low error factor
    */
   void setLowErrorFactor( double lew );

   /**
    * Returns the maximum sample
    */
   double maxSample() const { return m_maxSample; }
   /**
    * Sets the maximum sample
    */
   void setMaxSample( double ms );

   /**
    * Returns the media flag
    */
   bool media() const { return m_media; }
   /**
    * Sets the media flag
    */
   void setMedia( bool m );

   /**
    * Returns minimum reuse
    */
   double minimumReuse() const { return m_minimumReuse; }
   /**
    * Sets the minimum reuse
    */
   void setMinimumReuse( double mr );

   /**
    * Returns nearest count
    */
   int nearestCount() const { return m_nearestCount; }
   /**
    * Sets the nearest count
    */
   void setNearestCount( int nc );

   /**
    * Returns the normal flag
    */
   bool normal() const { return m_normal; }
   /**
    * Sets the normal flag
    */
   void setNormal( bool n );

   /**
    * Returns the pretrace start
    */
   double pretraceStart() const { return m_pretraceStart; }
   /**
    * Sets the pretrace start
    */
   void setPretraceStart( double ps );

   /**
    * Returns the pretrace end
    */
   double pretraceEnd() const { return m_pretraceEnd; }
   /**
    * Sets the pretrace end
    */
   void setPretraceEnd( double pe );

   /**
    * Returns recursion limit
    */
   int recursionLimit() const { return m_recursionLimit; }
   /**
    * Sets the recursion limit
    */
   void setRecursionLimit( int rl );

   /** */
   virtual void restoreMemento( PMMemento* s );

public slots:
   /**
    * radYN
    */
   void slot_radiosity_YN( bool yn );


private:

   bool radYN;

   /**
    * IDs for @ref PMMementoData
    */
   enum PMRadiosityMementoID { PMAdcBailoutID, PMAlwaysSampleID, PMBrightnessID,
                               PMCountID, PMErrorBoundID, PMGrayThresholdID,
                               PMLowErrorFactorID, PMMaxSampleID, PMMediaID,
                               PMMinimumReuseID, PMNearestCountID, PMNormalID,
                               PMPretraceStartID, PMPretraceEndID, PMRecursionLimitID };

   double m_adcBailout;
   bool m_alwaysSample;
   double m_brightness;
   int m_count;
   double m_errorBound;
   double m_grayThreshold;
   double m_lowErrorFactor;
   double m_maxSample;
   bool m_media;
   double m_minimumReuse;
   int m_nearestCount;
   bool m_normal;
   double m_pretraceStart;
   double m_pretraceEnd;
   int m_recursionLimit;
   bool checked_in_rendermode;

   static PMMetaObject* s_pMetaObject;
};

#endif
