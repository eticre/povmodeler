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


#include "pmfinish.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmfinishedit.h"



const PMColor emissionColorDefault = PMColor( 0.0, 0.0, 0.0, 0.0, 0.0 );
const PMColor ambientColorDefault = PMColor( 0.0, 0.0, 0.0, 0.0, 0.0 );
const double diffuseDefault = 0.6;
const double brillianceDefault = 1.0;
const double crandDefault = 0.0;
const double phongDefault = 0.0;
const double phongSizeDefault = 40.0;
const double metallicDefault = 1.0;
const double specularDefault = 0.0;
const double roughnessDefault = 0.05;
const double iridAmountDefault = 0.0;
const double iridThicknessDefault = 0.0;
const double iridTurbulenceDefault = 0.0;
const PMColor reflectionColorDefault = PMColor( 0.0, 0.0, 0.0, 0.0, 0.0 );
const double reflectionFalloffDefault = 0.0;
const double reflectionExponentDefault = 1.0;
const double reflectionMetallicDefault = 1.0;

PMDefinePropertyClass( PMFinish, PMFinishProperty );

PMMetaObject* PMFinish::s_pMetaObject = 0;
PMObject* createNewFinish( PMPart* part )
{
   return new PMFinish( part );
}

PMFinish::PMFinish( PMPart* part )
      : Base( part )
{
  m_emissionColor = emissionColorDefault;
  m_ambientColor = ambientColorDefault;
  m_diffuse = diffuseDefault;
  m_brilliance = brillianceDefault;
  m_crand = crandDefault;
  m_conserveEnergy = false;
  m_phong = phongDefault;
  m_phongSize = phongSizeDefault;
  m_metallic = metallicDefault;
  m_specular = specularDefault;
  m_roughness = roughnessDefault;
  m_iridAmount = iridAmountDefault;
  m_iridThickness = iridThicknessDefault;
  m_iridTurbulence = iridTurbulenceDefault;
  m_reflectionColor = reflectionColorDefault;
  m_reflectionMinColor = reflectionColorDefault;
  m_reflectionFresnel = false;
  m_reflectionFalloff = reflectionFalloffDefault;
  m_reflectionExponent = reflectionExponentDefault;
  m_reflectionMetallic = reflectionMetallicDefault;
  m_enableEmission = false;
  m_enableAmbient = false;
  m_enableDiffuse = false;
  m_enableBrilliance = false;
  m_enableCrand = false;
  m_enablePhong = false;
  m_enablePhongSize = false;
  m_enableMetallic = false;
  m_enableSpecular = false;
  m_enableRoughness = false;
  m_enableReflection = false;
  m_enableReflectionMin = false;
  m_enableRefFalloff = false;
  m_enableRefExponent = false;
  m_enableRefMetallic = false;
  m_irid = false;
}

PMFinish::PMFinish( const PMFinish& f )
      : Base( f )
{
  m_emissionColor = f.m_emissionColor;
  m_ambientColor = f.m_ambientColor;
  m_diffuse = f.m_diffuse;
  m_brilliance = f.m_brilliance;
  m_crand = f.m_crand;
  m_conserveEnergy = f.m_conserveEnergy;
  m_phong = f.m_phong;
  m_phongSize = f.m_phongSize;
  m_metallic = f.m_metallic;
  m_specular = f.m_specular;
  m_roughness = f.m_roughness;
  m_iridAmount = f.m_iridAmount;
  m_iridThickness = f.m_iridThickness;
  m_iridTurbulence = f.m_iridTurbulence;
  m_reflectionColor = f.m_reflectionColor;
  m_reflectionMinColor = f.m_reflectionMinColor;
  m_reflectionFresnel = f.m_reflectionFresnel;
  m_reflectionFalloff = f.m_reflectionFalloff;
  m_reflectionExponent = f.m_reflectionExponent;
  m_reflectionMetallic = f.m_reflectionMetallic;
  m_enableEmission = f.m_enableEmission;
  m_enableAmbient = f.m_enableAmbient;
  m_enableDiffuse = f.m_enableDiffuse;
  m_enableBrilliance = f.m_enableBrilliance;
  m_enableCrand = f.m_enableCrand;
  m_enablePhong = f.m_enablePhong;
  m_enablePhongSize = f.m_enablePhongSize;
  m_enableMetallic = f.m_enableMetallic;
  m_enableSpecular = f.m_enableSpecular;
  m_enableRoughness = f.m_enableRoughness;
  m_enableReflection = f.m_enableReflection;
  m_enableReflectionMin = f.m_enableReflectionMin;
  m_enableRefFalloff = f.m_enableRefFalloff;
  m_enableRefExponent = f.m_enableRefExponent;
  m_enableRefMetallic = f.m_enableRefMetallic;
  m_irid = f.m_irid;
}

PMFinish::~PMFinish()
{
}

PMMetaObject* PMFinish::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Finish", Base::metaObject(),
                                        createNewFinish );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "emissionColor", &PMFinish::setEmissionColor, &PMFinish::emissionColor ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "ambientColor", &PMFinish::setAmbientColor, &PMFinish::ambientColor ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "phong", &PMFinish::setPhong, &PMFinish::phong ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "diffuse", &PMFinish::setDiffuse, &PMFinish::diffuse ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "brilliance", &PMFinish::setBrilliance, &PMFinish::brilliance ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "crand", &PMFinish::setCrand, &PMFinish::crand ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "conserveEnergy", &PMFinish::setConserveEnergy, &PMFinish::conserveEnergy ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "specular", &PMFinish::setSpecular, &PMFinish::specular ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "roughness", &PMFinish::setRoughness, &PMFinish::roughness ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "metallic", &PMFinish::setMetallic, &PMFinish::metallic ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "irid", &PMFinish::setIrid, &PMFinish::irid ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "iridAmount", &PMFinish::setIridAmount, &PMFinish::iridAmount ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "iridThickness", &PMFinish::setIridThickness, &PMFinish::iridThickness ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionColor", &PMFinish::setReflectionColor, &PMFinish::reflectionColor ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionMinColor", &PMFinish::setReflectionMinColor, &PMFinish::reflectionMinColor ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionFresnel", &PMFinish::setReflectionFresnel, &PMFinish::reflectionFresnel ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionFalloff", &PMFinish::setReflectionFalloff, &PMFinish::reflectionFalloff ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionExponent", &PMFinish::setReflectionExponent, &PMFinish::reflectionExponent ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionMetallic", &PMFinish::setReflectionMetallic, &PMFinish::reflectionMetallic ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "emissionEnabled", &PMFinish::enableEmission, &PMFinish::isEmissionEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "ambientEnabled", &PMFinish::enableAmbient, &PMFinish::isAmbientEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "phongEnabled", &PMFinish::enablePhong, &PMFinish::isPhongEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "phongSizeEnabled", &PMFinish::enablePhongSize, &PMFinish::isPhongSizeEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "diffuseEnabled", &PMFinish::enableDiffuse, &PMFinish::isDiffuseEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "brillianceEnabled", &PMFinish::enableBrilliance, &PMFinish::isBrillianceEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "crandEnabled", &PMFinish::enableCrand, &PMFinish::isCrandEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "specularEnabled", &PMFinish::enableSpecular, &PMFinish::isSpecularEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "roughnessEnabled", &PMFinish::enableRoughness, &PMFinish::isRoughnessEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "metallicEnabled", &PMFinish::enableMetallic, &PMFinish::isMetallicEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionEnabled", &PMFinish::enableReflection, &PMFinish::isReflectionEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "reflectionMinEnabled", &PMFinish::enableReflectionMin, &PMFinish::isReflectionMinEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "refFalloffEnabled", &PMFinish::enableRefFalloff, &PMFinish::isRefFalloffEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "refExponentEnabled", &PMFinish::enableRefExponent, &PMFinish::isRefExponentEnabled ) );
      s_pMetaObject->addProperty(
         new PMFinishProperty( "refMetallicEnabled", &PMFinish::enableRefMetallic, &PMFinish::isRefMetallicEnabled ) );
   }
   return s_pMetaObject;
}

void PMFinish::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMFinish::description() const
{
   return ( "finish" );
}

void PMFinish::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
   e.setAttribute( "enable_emission", m_enableEmission );
   e.setAttribute( "enable_ambient", m_enableAmbient );
   e.setAttribute( "enable_diffuse", m_enableDiffuse );
   e.setAttribute( "enable_brilliance", m_enableBrilliance );
   e.setAttribute( "enable_crand", m_enableCrand );
   e.setAttribute( "enable_phong", m_enablePhong );
   e.setAttribute( "enable_phong_size", m_enablePhongSize );
   e.setAttribute( "enable_metallic", m_enableMetallic );
   e.setAttribute( "enable_specular", m_enableSpecular );
   e.setAttribute( "enable_roughness", m_enableRoughness );
   e.setAttribute( "enable_reflection", m_enableReflection );
   e.setAttribute( "enable_reflection_min", m_enableReflectionMin );
   e.setAttribute( "enable_reflection_falloff", m_enableRefFalloff );
   e.setAttribute( "enable_reflection_exponent", m_enableRefExponent );
   e.setAttribute( "enable_reflection_metallic", m_enableRefMetallic );
   e.setAttribute( "emission", m_emissionColor.serializeXML() );
   e.setAttribute( "ambient", m_ambientColor.serializeXML() );
   e.setAttribute( "diffuse", m_diffuse );
   e.setAttribute( "brilliance", m_brilliance );
   e.setAttribute( "crand", m_crand );
   e.setAttribute( "conserve_energy", m_conserveEnergy );
   e.setAttribute( "phong", m_phong );
   e.setAttribute( "phongsize", m_phongSize );
   e.setAttribute( "metallic", m_metallic );
   e.setAttribute( "specular", m_specular );
   e.setAttribute( "roughness", m_roughness );
   e.setAttribute( "irid", m_irid );
   e.setAttribute( "irid_amount", m_iridAmount );
   e.setAttribute( "irid_thickness", m_iridThickness );
   e.setAttribute( "irid_turbulence", m_iridTurbulence );
   e.setAttribute( "reflection", m_reflectionColor.serializeXML() );
   e.setAttribute( "reflection_min", m_reflectionMinColor.serializeXML() );
   e.setAttribute( "reflection_fresnel", m_reflectionFresnel );
   e.setAttribute( "reflection_falloff", m_reflectionFalloff );
   e.setAttribute( "reflection_exponent", m_reflectionExponent );
   e.setAttribute( "reflection_metallic", m_reflectionMetallic );
}

void PMFinish::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
   m_enableEmission = h.boolAttribute( "enable_emission", false );
   m_enableAmbient = h.boolAttribute( "enable_ambient", false );
   m_enableDiffuse = h.boolAttribute( "enable_diffuse", false );
   m_enableBrilliance = h.boolAttribute( "enable_brilliance", false );
   m_enableCrand = h.boolAttribute( "enable_crand", false );
   m_enablePhong = h.boolAttribute( "enable_phong", false );
   m_enablePhongSize = h.boolAttribute( "enable_phong_size", false );
   m_enableMetallic = h.boolAttribute( "enable_metallic", false );
   m_enableSpecular = h.boolAttribute( "enable_specular", false );
   m_enableRoughness = h.boolAttribute( "enable_roughness", false );
   m_enableReflection = h.boolAttribute( "enable_reflection", false );
   m_enableReflectionMin = h.boolAttribute( "enable_reflection_min", false );
   m_enableRefFalloff = h.boolAttribute( "enable_reflection_falloff", false );
   m_enableRefExponent = h.boolAttribute( "enable_reflection_exponent", false );
   m_enableRefMetallic = h.boolAttribute( "enable_reflection_metallic", false );
   m_irid = h.boolAttribute( "irid", false );
   m_emissionColor = h.colorAttribute( "emission", emissionColorDefault );
   m_ambientColor = h.colorAttribute( "ambient", ambientColorDefault );
   m_diffuse = h.doubleAttribute( "diffuse", diffuseDefault );
   m_brilliance = h.doubleAttribute( "brilliance", crandDefault );
   m_crand = h.doubleAttribute( "crand", crandDefault );
   m_conserveEnergy = h.boolAttribute( "conserve_energy", false );
   m_phong = h.doubleAttribute( "phong", phongDefault );
   m_phongSize = h.doubleAttribute( "phongsize", phongSizeDefault );
   m_metallic = h.doubleAttribute( "metallic", metallicDefault );
   m_specular = h.doubleAttribute( "specular", specularDefault );
   m_roughness = h.doubleAttribute( "roughness", roughnessDefault );
   m_iridAmount = h.doubleAttribute( "irid_amount", iridAmountDefault );
   m_iridThickness = h.doubleAttribute( "irid_thickness", iridThicknessDefault );
   m_iridTurbulence = h.doubleAttribute( "irid_turbulence", iridTurbulenceDefault );
   m_reflectionColor = h.colorAttribute( "reflection", reflectionColorDefault );
   m_reflectionMinColor = h.colorAttribute( "reflection_min", reflectionColorDefault );
   m_reflectionFresnel = h.boolAttribute( "reflection_fresnel", false );
   m_reflectionFalloff = h.doubleAttribute( "reflection_falloff", reflectionFalloffDefault );
   m_reflectionExponent = h.doubleAttribute( "reflection_exponent", reflectionExponentDefault );
   m_reflectionMetallic = h.doubleAttribute( "reflection_metallic", reflectionMetallicDefault );
}

void PMFinish::setPhong( double c )
{
   if( c != m_phong )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPhongID, m_phong );
      m_phong = c;
   }
}

void PMFinish::setPhongSize( double c )
{
   if( c != m_phongSize )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPhongSizeID, m_phongSize );
      m_phongSize = c;
   }
}

void PMFinish::setMetallic( double c )
{
   if( c != m_metallic )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMetallicID, m_metallic );
      m_metallic = c;
   }
}

void PMFinish::setEmissionColor( const PMColor& c )
{
   if( c != m_emissionColor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEmissionColorID, m_emissionColor );
      m_emissionColor = c;
   }
}

void PMFinish::setAmbientColor( const PMColor& c )
{
   if( c != m_ambientColor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAmbientColorID, m_ambientColor );
      m_ambientColor = c;
   }
}

void PMFinish::setDiffuse( double c )
{
   if( c != m_diffuse )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDiffuseID, m_diffuse );
      m_diffuse = c;
   }
}

void PMFinish::setBrilliance( double c )
{
   if( c != m_brilliance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBrillianceID, m_brilliance );
      m_brilliance = c;
   }
}

void PMFinish::setCrand( double c )
{
   if( c != m_crand )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCrandID, m_crand );
      m_crand = c;
   }
}

void PMFinish::setConserveEnergy( bool c )
{
   if( c != m_conserveEnergy )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMConserveEnergyID, m_conserveEnergy );
      m_conserveEnergy = c;
   }
}

void PMFinish::setSpecular( double c )
{
   if( c != m_specular )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSpecularID, m_specular );
      m_specular = c;
   }
}

void PMFinish::setRoughness( double c )
{
   if( c != m_roughness )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRoughnessID, m_roughness );
      m_roughness = c;
   }
}

void PMFinish::setIrid( bool c )
{
   if( c != m_irid )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMIridID, m_irid );
      m_irid = c;
   }
}

void PMFinish::setReflectionColor( const PMColor& c )
{
   if( c != m_reflectionColor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMReflectionColorID, m_reflectionColor );
      m_reflectionColor = c;
   }
}

void PMFinish::setReflectionMinColor( const PMColor& c )
{
   if( c != m_reflectionMinColor )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMReflectionMinColorID, m_reflectionMinColor );
      m_reflectionMinColor = c;
   }
}

void PMFinish::setReflectionFresnel( bool c )
{
   if( c != m_reflectionFresnel )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMReflectionFresnelID, m_reflectionFresnel );
      m_reflectionFresnel = c;
   }
}

void PMFinish::setReflectionFalloff( double c )
{
   if( c != m_reflectionFalloff )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMReflectionFalloffID, m_reflectionFalloff );
      m_reflectionFalloff = c;
   }
}

void PMFinish::setReflectionExponent( double c )
{
   if( c != m_reflectionExponent )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMReflectionExponentID, m_reflectionExponent );
      m_reflectionExponent = c;
   }
}

void PMFinish::setReflectionMetallic( double c )
{
   if( c != m_reflectionMetallic )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMReflectionMetallicID, m_reflectionMetallic );
      m_reflectionMetallic = c;
   }
}

void PMFinish::enableEmission( bool c )
{
   if( c != m_enableEmission )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableEmissionID, m_enableEmission );
      m_enableEmission = c;
   }
}

void PMFinish::enableAmbient( bool c )
{
   if( c != m_enableAmbient )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableAmbientID, m_enableAmbient );
      m_enableAmbient = c;
   }
}

void PMFinish::enableDiffuse( bool c )
{
   if( c != m_enableDiffuse )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableDiffuseID, m_enableDiffuse );
      m_enableDiffuse = c;
   }
}

void PMFinish::enablePhong( bool c )
{
   if( c != m_enablePhong )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnablePhongID, m_enablePhong );
      m_enablePhong = c;
   }
}

void PMFinish::enablePhongSize( bool c )
{
   if( c != m_enablePhongSize )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnablePhongSizeID, m_enablePhongSize );
      m_enablePhongSize = c;
   }
}

void PMFinish::enableBrilliance( bool c )
{
   if( c != m_enableBrilliance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableBrillianceID, m_enableBrilliance );
      m_enableBrilliance = c;
   }
}

void PMFinish::enableCrand( bool c )
{
   if( c != m_enableCrand )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableCrandID, m_enableCrand );
      m_enableCrand = c;
   }
}

void PMFinish::enableSpecular( bool c )
{
   if( c != m_enableSpecular )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableSpecularID, m_enableSpecular );
      m_enableSpecular = c;
   }
}

void PMFinish::enableRoughness( bool c )
{
   if( c != m_enableRoughness )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableRoughnessID, m_enableRoughness );
      m_enableRoughness = c;
   }
}

void PMFinish::enableMetallic( bool c )
{
   if( c != m_enableMetallic )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableMetallicID, m_enableMetallic );
      m_enableMetallic = c;
   }
}

void PMFinish::enableReflection( bool c )
{
   if( c != m_enableReflection )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableReflectionID, m_enableReflection );
      m_enableReflection = c;
   }
}

void PMFinish::enableReflectionMin( bool c )
{
   if( c != m_enableReflectionMin )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableReflectionMinID, m_enableReflectionMin );
      m_enableReflectionMin = c;
   }
}

void PMFinish::enableRefFalloff( bool c )
{
   if( c != m_enableRefFalloff )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableRefFalloffID, m_enableRefFalloff );
      m_enableRefFalloff = c;
   }
}

void PMFinish::enableRefExponent( bool c )
{
   if( c != m_enableRefExponent )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableRefExponentID, m_enableRefExponent );
      m_enableRefExponent = c;
   }
}

void PMFinish::enableRefMetallic( bool c )
{
   if( c != m_enableRefMetallic )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableRefMetallicID, m_enableRefMetallic );
      m_enableRefMetallic = c;
   }
}

void PMFinish::setIridAmount( double c )
{
   if( c != m_iridAmount )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMIridAmountID, m_iridAmount );
      m_iridAmount = c;
   }
}

void PMFinish::setIridThickness( double c )
{
   if( c != m_iridThickness )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMIridThicknessID, m_iridThickness );
      m_iridThickness = c;
   }
}

void PMFinish::setIridTurbulence( double c )
{
   if( c != m_iridTurbulence )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMIridTurbulenceID, m_iridTurbulence );
      m_iridTurbulence = c;
   }
}

PMDialogEditBase* PMFinish::editWidget( QWidget* parent ) const
{
   return new PMFinishEdit( parent );
}

void PMFinish::restoreMemento( PMMemento* s )
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
            case PMEmissionColorID:
               setEmissionColor( data->colorData() );
               break;
            case PMAmbientColorID:
               setAmbientColor( data->colorData() );
               break;
            case PMDiffuseID:
               setDiffuse( data->doubleData() );
               break;
            case PMBrillianceID:
               setBrilliance( data->doubleData() );
               break;
            case PMCrandID:
               setCrand( data->doubleData() );
               break;
            case PMConserveEnergyID:
               setConserveEnergy( data->boolData() );
            case PMPhongID:
               setPhong( data->doubleData() );
               break;
            case PMPhongSizeID:
               setPhongSize( data->doubleData() );
               break;
            case PMMetallicID:
               setMetallic( data->doubleData() );
               break;
            case PMSpecularID:
               setSpecular( data->doubleData() );
               break;
            case PMRoughnessID:
               setRoughness( data->doubleData() );
               break;
            case PMIridID:
               setIrid( data->boolData() );
               break;
            case PMIridAmountID:
               setIridAmount( data->doubleData() );
               break;
            case PMIridThicknessID:
               setIridThickness( data->doubleData() );
               break;
            case PMIridTurbulenceID:
               setIridTurbulence( data->doubleData() );
               break;
            case PMReflectionColorID:
               setReflectionColor( data->colorData() );
               break;
            case PMReflectionMinColorID:
               setReflectionMinColor( data->colorData() );
               break;
            case PMReflectionFresnelID:
               setReflectionFresnel( data->boolData() );
               break;
            case PMReflectionFalloffID:
               setReflectionFalloff( data->doubleData() );
               break;
            case PMReflectionExponentID:
               setReflectionExponent( data->doubleData() );
               break;
            case PMReflectionMetallicID:
               setReflectionMetallic( data->doubleData() );
               break;
           case PMEnableEmissionID:
               enableEmission( data->boolData() );
               break;
            case PMEnableAmbientID:
               enableAmbient( data->boolData() );
               break;
            case PMEnablePhongID:
               enablePhong( data->boolData() );
               break;
            case PMEnablePhongSizeID:
               enablePhongSize( data->boolData() );
               break;
            case PMEnableDiffuseID:
               enableDiffuse( data->boolData() );
               break;
            case PMEnableBrillianceID:
               enableBrilliance( data->boolData() );
               break;
            case PMEnableCrandID:
               enableCrand( data->boolData() );
               break;
            case PMEnableSpecularID:
               enableSpecular( data->boolData() );
               break;
            case PMEnableRoughnessID:
               enableRoughness( data->boolData() );
               break;
            case PMEnableMetallicID:
               enableMetallic( data->boolData() );
               break;
            case PMEnableReflectionID:
               enableReflection( data->boolData() );
               break;
            case PMEnableReflectionMinID:
               enableReflectionMin( data->boolData() );
               break;
            case PMEnableRefFalloffID:
               enableRefFalloff( data->boolData() );
               break;
            case PMEnableRefExponentID:
               enableRefExponent( data->boolData() );
               break;
            case PMEnableRefMetallicID:
               enableRefMetallic( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMFinish::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
