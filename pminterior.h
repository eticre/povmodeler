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


#ifndef PMINTERIOR_H
#define PMINTERIOR_H



#include "pmtexturebase.h"
#include "pmcolor.h"

/**
 * Class for povray interiors
 */
class PMInterior : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMInterior
    */
   PMInterior( PMPart* part );
   /**
    * Copy constructor
    */
   PMInterior( const PMInterior& i );
   /**
    * Deletes the object
    */
   virtual ~PMInterior();

   /** */
   virtual PMObject* copy() const { return new PMInterior( *this ); }
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
    * Returns a new @ref PMInteriorEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pminterior" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );

   double ior() const { return m_ior; }
   double caustics() const { return m_caustics; }
   double dispersion() const { return m_dispersion; }
   int dispSamples() const { return m_dispSamples; }
   double fadeDistance() const { return m_fadeDistance; }
   double fadePower() const { return m_fadePower; }
   bool isIorEnabled() const { return m_enableIor; }
   bool isCausticsEnabled() const { return m_enableCaustics; }
   bool isDispersionEnabled() const { return m_enableDispersion; }
   bool isDispSamplesEnabled() const { return m_enableDispSamples; }
   bool isFadeDistanceEnabled() const { return m_enableFadeDistance; }
   bool isFadePowerEnabled() const { return m_enableFadePower; }

   void setIor( double c );
   void setCaustics( double c );
   void setDispersion ( double c );
   void setDispSamples ( int c );
   void setFadeDistance( double c );
   void setFadePower( double c );
   void enableIor( bool c );
   void enableCaustics( bool c );
   void enableDispersion( bool c );
   void enableDispSamples( bool c );
   void enableFadeDistance( bool c );
   void enableFadePower( bool c );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMInteriorMementoID { PMIorID, PMCausticsID, PMDispersionID,
                              PMDispSamplesID, PMFadeDistanceID, PMFadePowerID,
                              PMEnableIorID, PMEnableCausticsID,
                              PMEnableDispersionID, PMEnableDispSamplesID,
                              PMEnableFadeDistanceID, PMEnableFadePowerID };
   double m_ior;
   double m_caustics;
   double m_dispersion;
   int m_dispSamples;
   double m_fadeDistance;
   double m_fadePower;

   bool m_enableIor;
   bool m_enableCaustics;
   bool m_enableDispersion;
   bool m_enableDispSamples;
   bool m_enableFadeDistance;
   bool m_enableFadePower;

   static PMMetaObject* s_pMetaObject;
};


#endif
