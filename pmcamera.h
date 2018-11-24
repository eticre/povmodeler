//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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

#ifndef PMCAMERA_H
#define PMCAMERA_H



#include "pmnamedobject.h"
#include "pmvector.h"

class PMViewStructure;

/**
 * Class for povray cameras
 */

class PMCamera : public PMNamedObject
{
   typedef PMNamedObject Base;
public:
   /**
    * Type of the camera
    */
   enum CameraType { Perspective, Orthographic, FishEye, UltraWideAngle,
                     Omnimax, Panoramic, Cylinder };
   /**
    * Creates a default povray camera
    */
   PMCamera( PMPart* part );
   /**
    * Copy constructor
    */
   PMCamera( const PMCamera& c );
   /**
    * deletes the camera
    */
   virtual ~PMCamera();

   /** */
   virtual PMObject* copy() const { return new PMCamera( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMCameraEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmcamera" ); }

   /**
    * Returns the location
    */
   PMVector location() const { return m_location; }
   /**
    * Sets the location
    */
   void setLocation( const PMVector& p );

   /**
    * Returns the look_at point
    */
   PMVector lookAt() const { return m_lookAt; }
   /**
    * Sets the look_at point
    */
   void setLookAt( const PMVector& p );

   /**
    * Returns the up vector
    */
   PMVector up() const { return m_up; }
   /**
    * Sets the up vector
    */
   void setUp( const PMVector& v );

   /**
    * Returns the right vector
    */
   PMVector right() const { return m_right; }
   /**
    * Sets the right vector
    */
   void setRight( const PMVector& v );

   /**
    * Returns the direction vector
    */
   PMVector direction() const { return m_direction; }
   /**
    * Sets the direction vector
    */
   void setDirection( const PMVector& v );

   /**
    * Returns the sky vector
    */
   PMVector sky() const { return m_sky; }
   /**
    * Sets the sky vector
    */
   void setSky( const PMVector& v );

   /**
    * Returns the aspect ratio
    */
   double aspect() const;

   /**
    * Returns true if angle is enabled
    */
   bool isAngleEnabled() const { return m_angleEnabled; }
   /**
    * Enables/Disables the use of angle
    */
   void enableAngle( bool yes );

   /**
    * Returns the angle
    */
   double angle() const { return m_angle; }
   /**
    * Sets the angle
    */
   void setAngle( double a );

   /**
    * Returns the camera type
    */
   CameraType cameraType() const { return m_cameraType; }
   /**
    * Sets the camera type
    */
   void setCameraType( CameraType t );

   /**
    * Returns the cylinder type
    */
   int cylinderType() const { return m_cylinderType; }
   /**
    * Sets the cylinder type. Valid values are 1-4
    */
   void setCylinderType( int t );

   /**
    * Returns true if focal blur is enabled
    */
   bool isFocalBlurEnabled() const { return m_focalBlurEnabled; }
   /**
    * Enables/Disables the focal blur
    */
   void enableFocalBlur( bool yes );
   /**
    * Returns the aperture
    */
   double aperture() const { return m_aperture; }
   /**
    * Sets the aperture. Has to be greater or equal 0
    */
   void setAperture( double d );
   /**
    * Returns the number of blur samples
    */
   int blurSamples() const { return m_blurSamples; }
   /**
    * Sets the number of blur samples
    */
   void setBlurSamples( int s );
   /**
    * Returns the focal point
    */
   PMVector focalPoint() const { return m_focalPoint; }
   /**
    * Sets the focal point
    */
   void setFocalPoint( const PMVector& v );
   /**
    * Returns the confidence
    */
   double confidence() const { return m_confidence;}
   /**
    * Sets the confidence
    */
   void setConfidence( double c );
   /**
    * Returns the variance
    */
   double variance() const { return m_variance; }
   /**
    * Sets the variance
    */
   void setVariance( double v );

   /**
    * Returns the export flag
    */
   virtual bool exportPovray() const { return m_export; }
   /**
    * Sets the export flag
    */
   void setExportPovray( bool ex );

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );

   /**
    * Calculates the new right, up and direction vector with the
    * look_at vector.
    */
   void calculateLookAtAngle( PMVector& right, PMVector& up, PMVector& direction );

   /**
    * Converts the camera type to a string
    */
   static QString cameraTypeToString( CameraType t );
   /**
    * Converts a string to the camera type
    */
   static CameraType stringToCameraType( const QString& s );
   /** */
   virtual void cleanUp() const;

protected:
   /** */
   virtual bool isDefault() { return false; }
   /** */
   virtual void createViewStructure();
   /** */
   virtual PMViewStructure* defaultViewStructure() const;

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMCameraMementoID { PMLocationID, PMLookAtID, PMDirectionID,
                            PMUpID, PMRightID, PMAngleID, PMSkyID,
                            PMCameraTypeID, PMCylinderTypeID,
                            PMFocalBlurID, PMBlurSamplesID, PMFocalPointID,
                            PMConfidenceID, PMVarianceID, PMApertureID,
                            PMAngleEnabledID, PMExportID };
   PMVector m_location, m_lookAt, m_up, m_right, m_direction, m_sky;
   bool m_angleEnabled;
   double m_angle;
   CameraType m_cameraType;
   int m_cylinderType;
   bool m_focalBlurEnabled;
   double m_aperture;
   int m_blurSamples;
   PMVector m_focalPoint;
   double m_confidence, m_variance;
   bool m_export;

   /**
    * The default view structure. It can be shared between cameras
    */
   static PMViewStructure* s_pDefaultViewStructure;

   static PMMetaObject* s_pMetaObject;
};

#endif
