//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMLIGHT_H
#define PMLIGHT_H



#include "pmnamedobject.h"
#include "pmvector.h"
#include "pmcolor.h"

class PMViewStructure;

/**
 * Class for povray light sources.
 */

class PMLight : public PMNamedObject
{
   typedef PMNamedObject Base;
public:
   enum PMLightType { PointLight=0, SpotLight=1, CylinderLight=2, ShadowlessLight=3 };
   enum PMAreaType { Rectangular=0, Circular=1 };
   /**
    * Creates an empty PMLight
    */
   PMLight( PMPart* part );
   /**
    * Copy constructor
    */
   PMLight( const PMLight& l );
   /**
    * deletes the PMLight
    */
   virtual ~PMLight();

   /** */
   virtual PMObject* copy() const { return new PMLight( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMLightEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmlight" ); }

   /**
    * Returns the type
    */
   PMLightType lightType() const { return m_type; }
   /**
    * Sets the type
    */
   void setLightType( PMLightType t );

   /**
    * Returns the location
    */
   PMVector location() const { return m_location; }
   /**
    * Sets the location
    */
   void setLocation( const PMVector& p );

   /**
    * Returns the color
    */
   PMColor color() const { return m_color; }
   /**
    * Sets the color
    */
   void setColor( const PMColor& c );

   /**
    * Returns the radius for cylinder and spot lights
    */
   double radius() const { return m_radius; }
   /**
    * Sets the radius for cylinder and spot lights
    */
   void setRadius( double r );

   /**
    * Returns the falloff for cylinder and spot lights
    */
   double falloff() const { return m_falloff; }
   /**
    * Sets the falloff for cylinder and spot lights
    */
   void setFalloff( double f );

   /**
    * Returns the tightness for cylinder and spot lights
    */
   double tightness() const { return m_tightness; }
   /**
    * Sets the tightness for cylinder and spot lights
    */
   void setTightness( double r );

   /**
    * Returns the pointAt point for cylinder and spot lights
    */
   PMVector pointAt() const { return m_pointAt; }
   /**
    * Sets the pointAt for cylinder and spot lights
    */
   void setPointAt( const PMVector& p );

   /**
    * Return true if the light is a parallel light
    */
   bool parallel() const { return m_parallel; }
   /**
    * Sets the parallel light flag
    */
   void setParallel( bool p );

   /**
    * Returns true if the light is a area light
    */
   bool isAreaLight() const { return m_bAreaLight; }
   /**
    * Sets the area light flag
    */
   void setAreaLight( bool yes );

   /**
    * Returns the area light type
    */
   PMAreaType areaType() const { return m_areaType; }
   /**
    * Sets the area light type
    */
   void setAreaType( PMAreaType at );

   /**
    * Returns the axis1 for area lights
    */
   PMVector axis1() const { return m_areaAxis1; }
   /**
    * Sets the axis1 for area lights
    */
   void setAxis1( const PMVector& v );

   /**
    * Returns the axis2 for area lights
    */
   PMVector axis2() const { return m_areaAxis2; }
   /**
    * Sets the axis2 for area lights
    */
   void setAxis2( const PMVector& v );

   /**
    * Returns the size1 for area lights
    */
   int size1() const { return m_areaSize1; }
   /**
    * Sets the size1 for area lights
    */
   void setSize1( int s );

   /**
    * Returns the size2 for area lights
    */
   int size2() const { return m_areaSize2; }
   /**
    * Sets the size2 for area lights
    */
   void setSize2( int s );

   /**
    * Returns the adaptive parameter for area lights
    */
   int adaptive() const { return m_adaptive; }
   /**
    * Sets the adaptive parameter for area lights
    */
   void setAdaptive( int s );

   /**
    * Returns if the area light is orientated
    */
   bool orient() const { return m_orient; }
   /**
    * Sets the orient flag
    */
   void setOrient( bool o );

   /**
    * Returns the jitter parameter for area lights
    */
   bool jitter() const { return m_jitter; }
   /**
    * Sets the jitter parameter for area lights
    */
   void setJitter( bool j );

   /**
    * Returns true if light fading is enabled
    */
   bool fading() const { return m_bFading; }
   /**
    * Enables/Disables light fading
    */
   void setFading( bool yes );

   /**
    * Returns the fading distance
    */
   double fadeDistance() const { return m_fadeDistance; }
   /**
    * Sets the fading distance
    */
   void setFadeDistance( double d );

   /**
    * Returns the fading power
    */
   int fadePower() const { return m_fadePower; }
   /**
    * Sets the fading power
    */
   void setFadePower( int p );

   /**
    * Returns the media interaction flag
    */
   bool mediaInteraction() const { return m_bMediaInteraction; }
   /**
    * Sets the media interaction flag
    */
   void setMediaInteraction( bool yes );

   /**
    * Returns the media attenuation flag
    */
   bool mediaAttenuation() const { return m_bMediaAttenuation; }
   /**
    * Sets the media attenuation flag
    */
   void setMediaAttenuation( bool yes );

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual void cleanUp() const;

protected:
   /** */
   virtual bool isDefault() { return false; }
   /** */
   virtual void createViewStructure();
   /** */
   virtual PMViewStructure* defaultViewStructure() const { return 0; }

private:
   /**
    * Creates and returns the default view structure for point lights
    */
   PMViewStructure* defaultPointStructure() const;
   /**
    * Creates and returns the default view structure for spot lights
    */
   PMViewStructure* defaultSpotStructure() const;
   /**
    * Creates and returns the default view structure for cylindrical lights
    */
   PMViewStructure* defaultCylindricalStructure() const;

   /**
    * IDs for @ref PMMementoData
    */
   enum PMLightMementoID
   { PMLocationID, PMColorID, PMRadiusID, PMFalloffID, PMTightnessID,
     PMPointAtID, PMParallelID, PMAreaLightID, PMAreaTypeID, PMAreaAxis1ID,
     PMAreaAxis2ID, PMAreaSize1ID, PMAreaSize2ID, PMAdaptiveID, PMOrientID,
     PMJitterID, PMTypeID, PMFadingID, PMFadeDistanceID, PMFadePowerID,
     PMInteractionID, PMAttenuationID };

   PMLightType m_type;
   PMVector m_location;
   PMColor m_color;
   double m_radius;
   double m_falloff;
   double m_tightness;
   PMVector m_pointAt;
   bool m_parallel;
   bool m_bAreaLight;
   PMAreaType m_areaType;
   PMVector m_areaAxis1, m_areaAxis2;
   int m_areaSize1, m_areaSize2;
   int m_adaptive;
   bool m_orient;
   bool m_jitter;
   bool m_bFading;
   double m_fadeDistance;
   int m_fadePower;
   bool m_bMediaInteraction;
   bool m_bMediaAttenuation;

   /**
    * The default view structure for point lights
    */
   static PMViewStructure* s_pDefaultPointStructure;
   /**
    * The default view structure for spot lights
    */
   static PMViewStructure* s_pDefaultSpotStructure;
   /**
    * The default view structure for cylindrical lights
    */
   static PMViewStructure* s_pDefaultCylindricalStructure;

   /**
    * The size of the point light for 3d views
    */
   static double s_pointLightSize;
   /**
    * Number of lines around the cylinder
    */
   static int s_nCylinderLines;
   /**
    * Number of lines around the spot
    */
   static int s_nSpotLines;
   /**
    * Length of the spot and cylinder
    */
   static double s_length;

   static PMMetaObject* s_pMetaObject;
};

#endif
