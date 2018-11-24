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


#ifndef PMFINISH_H
#define PMFINISH_H



#include "pmtexturebase.h"
#include "pmcolor.h"

/**
 * Class for povray finishs
 */
class PMFinish : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMFinish
    */
   PMFinish( PMPart* part );
   /**
    * Copy constructor
    */
   PMFinish( const PMFinish& f );
   /**
    * Deletes the object
    */
   virtual ~PMFinish();

   /** */
   virtual PMObject* copy() const { return new PMFinish( *this ); }
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
    * Returns a new @ref PMFinishEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmfinish" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );
   
   PMColor emissionColor() const { return m_emissionColor; }
   PMColor ambientColor() const { return m_ambientColor; }
   double phong() const { return m_phong; }
   double phongSize() const { return m_phongSize; }
   double diffuse() const { return m_diffuse; }
   double brilliance() const { return m_brilliance; }
   double crand() const { return m_crand; }
   bool conserveEnergy() const { return m_conserveEnergy; }
   double specular() const { return m_specular; }
   double roughness() const { return m_roughness; }
   double metallic() const { return m_metallic; }
   bool irid() const { return m_irid; }
   double iridAmount() const { return m_iridAmount; }
   double iridThickness() const { return m_iridThickness; }
   double iridTurbulence() const { return m_iridTurbulence; }
   PMColor reflectionColor() const { return m_reflectionColor; }
   PMColor reflectionMinColor() const { return m_reflectionMinColor;}
   bool reflectionFresnel() const { return m_reflectionFresnel; }
   double reflectionFalloff() const { return m_reflectionFalloff; }
   double reflectionExponent() const { return m_reflectionExponent; }
   double reflectionMetallic() const { return m_reflectionMetallic; }
   bool isEmissionEnabled() const { return m_enableEmission; }
   bool isAmbientEnabled() const { return m_enableAmbient; }
   bool isPhongEnabled() const { return m_enablePhong; }
   bool isPhongSizeEnabled() const { return m_enablePhongSize; }
   bool isDiffuseEnabled() const { return m_enableDiffuse; }
   bool isBrillianceEnabled() const { return m_enableBrilliance; }
   bool isCrandEnabled() const { return m_enableCrand; }
   bool isSpecularEnabled() const { return m_enableSpecular; }
   bool isRoughnessEnabled() const { return m_enableRoughness; }
   bool isMetallicEnabled() const { return m_enableMetallic; }
   bool isReflectionEnabled() const { return m_enableReflection; }
   bool isReflectionMinEnabled() const { return m_enableReflectionMin; }
   bool isRefFalloffEnabled() const { return m_enableRefFalloff; }
   bool isRefExponentEnabled() const { return m_enableRefExponent; }
   bool isRefMetallicEnabled() const { return m_enableRefMetallic; }

   //This is here for povrat31serialization (It won't compile otherwise)
   bool isExponentEnabled() const { return m_enableRefExponent; }

   void setEmissionColor( const PMColor& c );
   void setAmbientColor( const PMColor& c );
   void setPhong( double c );
   void setPhongSize( double c );
   void setDiffuse( double c );
   void setBrilliance( double c );
   void setCrand( double c );
   void setConserveEnergy( bool c );
   void setSpecular( double c );
   void setRoughness( double c );
   void setMetallic( double c );
   void setIrid( bool c );
   void setIridAmount( double c );
   void setIridThickness( double c );
   void setIridTurbulence( double c );
   void setReflectionColor( const PMColor& c );
   void setReflectionMinColor( const PMColor& c );
   void setReflectionFresnel( bool c );
   void setReflectionFalloff( double c );
   void setReflectionExponent( double c );
   void setReflectionMetallic( double c );
   void enableEmission( bool c );
   void enableAmbient( bool c );
   void enablePhong( bool c );
   void enablePhongSize( bool c );
   void enableDiffuse( bool c );
   void enableBrilliance( bool c );
   void enableCrand( bool c );
   void enableSpecular( bool c );
   void enableRoughness( bool c );
   void enableMetallic( bool c );
   void enableReflection( bool c );
   void enableReflectionMin( bool c );
   void enableRefFalloff( bool c );
   void enableRefExponent( bool c );
   void enableRefMetallic( bool c );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMFinishMementoID {PMEmissionColorID, PMAmbientColorID, PMPhongID, PMPhongSizeID,
                            PMMetallicID, PMDiffuseID, PMBrillianceID,
                            PMCrandID, PMConserveEnergyID, PMSpecularID,
                            PMRoughnessID,
                            PMIridID, PMIridAmountID, PMIridThicknessID,
                            PMReflectionColorID, PMReflectionMinColorID,
                            PMReflectionFresnelID, PMReflectionFalloffID,
                            PMReflectionExponentID, PMReflectionMetallicID,
                            PMIridTurbulenceID,PMEnableEmissionID, PMEnableAmbientID,
                            PMEnablePhongID, PMEnablePhongSizeID,
                            PMEnableMetallicID,
                            PMEnableDiffuseID, PMEnableBrillianceID,
                            PMEnableCrandID, PMEnableSpecularID,
                            PMEnableRoughnessID, PMEnableReflectionID,
                            PMEnableReflectionMinID, PMEnableRefFalloffID,
                            PMEnableRefExponentID, PMEnableRefMetallicID };
   
   PMColor m_emissionColor;
   PMColor m_ambientColor;
   double m_phong;
   double m_phongSize;
   double m_diffuse;
   double m_brilliance;
   double m_crand;
   bool m_conserveEnergy;
   double m_specular;
   double m_roughness;
   double m_metallic;
   bool m_irid;
   double m_iridAmount;
   double m_iridThickness;
   double m_iridTurbulence;
   PMColor m_reflectionColor;
   PMColor m_reflectionMinColor;
   bool m_reflectionFresnel;
   double m_reflectionFalloff;
   double m_reflectionExponent;
   double m_reflectionMetallic;

   bool m_enableEmission;
   bool m_enableAmbient;
   bool m_enableDiffuse;
   bool m_enableBrilliance;
   bool m_enableCrand;
   bool m_enablePhong;
   bool m_enablePhongSize;
   bool m_enableMetallic;
   bool m_enableSpecular;
   bool m_enableRoughness;
   bool m_enableReflection;
   bool m_enableReflectionMin;
   bool m_enableRefFalloff;
   bool m_enableRefExponent;
   bool m_enableRefMetallic;

   static PMMetaObject* s_pMetaObject;
};

#endif
