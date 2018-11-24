//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Luis Carvalho
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


#ifndef PMRAINBOW_H
#define PMRAINBOW_H



#include "pmtexturebase.h"

/**
 * Class for povray rainbows
 */
class PMRainbow : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMRainbow
    */
   PMRainbow( PMPart* part );
   /**
    * Copy constructor
    */
   PMRainbow( const PMRainbow& r );
   /**
    * Deletes the object
    */
   virtual ~PMRainbow();

   /** */
   virtual PMObject* copy() const { return new PMRainbow( *this ); }
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
    * Returns a new @ref PMRainbowEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmrainbow" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );

   PMVector direction() const { return m_direction; }
   double angle() const { return m_angle; }
   double width() const { return m_width; }
   double distance() const { return m_distance; }
   double jitter() const { return m_jitter; }
   PMVector up() const { return m_up; }
   double arcAngle() const { return m_arcAngle; }
   double falloffAngle() const { return m_falloffAngle; }
   bool isDirectionEnabled() const { return m_enableDirection; }
   bool isAngleEnabled() const { return m_enableAngle; }
   bool isWidthEnabled() const { return m_enableWidth; }
   bool isDistanceEnabled() const { return m_enableDistance; }
   bool isJitterEnabled() const { return m_enableJitter; }
   bool isUpEnabled() const { return m_enableUp; }
   bool isArcAngleEnabled() const { return m_enableArcAngle; }
   bool isFalloffAngleEnabled() const { return m_enableFalloffAngle; }

   void setDirection( const PMVector& c );
   void setAngle( double c );
   void setWidth( double c );
   void setDistance( double c );
   void setJitter( double c );
   void setUp( const PMVector& c );
   void setArcAngle( double c );
   void setFalloffAngle( double c );
   void enableDirection( bool c );
   void enableAngle( bool c );
   void enableWidth( bool c );
   void enableDistance( bool c );
   void enableJitter( bool c );
   void enableUp( bool c );
   void enableArcAngle( bool c );
   void enableFalloffAngle( bool c );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMRainbowMementoID { PMDirectionID, PMAngleID, PMWidthID,
	                          PMDistanceID, PMJitterID, PMUpID, PMArcAngleID,
                             PMFalloffAngleID, PMEnableDirectionID,
                             PMEnableAngleID, PMEnableWidthID,
                             PMEnableDistanceID, PMEnableJitterID, PMEnableUpID,
                             PMEnableArcAngleID, PMEnableFalloffAngleID };
   PMVector m_direction;
   double m_angle;
   double m_width;
   double m_distance;
   double m_jitter;
   PMVector m_up;
   double m_arcAngle;
   double m_falloffAngle;

   bool m_enableDirection;
   bool m_enableAngle;
   bool m_enableWidth;
   bool m_enableDistance;
   bool m_enableJitter;
   bool m_enableUp;
   bool m_enableArcAngle;
   bool m_enableFalloffAngle;

   static PMMetaObject* s_pMetaObject;
};


#endif
