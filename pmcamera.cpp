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


#include "pmcamera.h"

#include "pmxmlhelper.h"
#include "pmcameraedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"
#include "pmvectorcontrolpoint.h"
#include "pmenumproperty.h"



PMDefinePropertyClass( PMCamera, PMCameraProperty );
PMDefineEnumPropertyClass( PMCamera, PMCamera::CameraType, PMCameraTypeProperty );

PMViewStructure* PMCamera::s_pDefaultViewStructure = nullptr;
PMMetaObject* PMCamera::s_pMetaObject = nullptr;
PMObject* createNewCamera( PMPart* part )
{
   return new PMCamera( part );
}

const PMVector locationDefault = PMVector( 0.0, 0.0, 0.0 );
const PMVector lookAtDefault = PMVector( 0.0, 0.0, 1.0 );
const PMVector directionDefault = PMVector( 0.0, 0.0, 1.0 );
const PMVector upDefault = PMVector( 0.0, 1.0, 0.0 );
const PMVector rightDefault = PMVector( 4.0/3.0, 0.0, 0.0 );
const PMVector skyDefault = PMVector( 0.0, 1.0, 0.0 );
//const double aspectDefault = 1.0;
const bool angleEnabledDefault = false;
const double angleDefault = 90.0;
const PMCamera::CameraType cameraTypeDefault = PMCamera::Perspective;
const int cylinderTypeDefault = 1;
const bool focalBlurDefault = false;
const double apertureDefault = 0.4;
const int blurSamplesDefault = 10;
const PMVector focalPointDefault = PMVector( 0.0, 0.0, 0.0 );
const double confidenceDefault = 0.9;
const double varianceDefault = 0.008;
const bool c_defaultExport = true;

PMCamera::PMCamera( PMPart* part )
      : Base( part )
{
   m_location = locationDefault;
   m_lookAt = lookAtDefault;
   m_direction = directionDefault;

   m_up = upDefault;
   m_right = rightDefault;

   m_sky = skyDefault;
   m_angle = angleDefault;
   m_angleEnabled = angleEnabledDefault;

   m_cameraType = cameraTypeDefault;
   m_cylinderType = cylinderTypeDefault;

   m_focalBlurEnabled = focalBlurDefault;
   m_aperture = apertureDefault;
   m_blurSamples = blurSamplesDefault;
   m_focalPoint = focalPointDefault;
   m_confidence = confidenceDefault;
   m_variance = varianceDefault;
   m_export = c_defaultExport;
}

PMCamera::PMCamera( const PMCamera& c )
      : Base( c )
{
   m_location = c.m_location;
   m_lookAt = c.m_lookAt;
   m_direction = c.m_direction;

   m_up = c.m_up;
   m_right = c.m_right;

   m_sky = c.m_sky;
   m_angle = c.m_angle;
   m_angleEnabled = c.m_angleEnabled;

   m_cameraType = c.m_cameraType;
   m_cylinderType = c.m_cylinderType;

   m_focalBlurEnabled = c.m_focalBlurEnabled;
   m_aperture = c.m_aperture;
   m_blurSamples = c.m_blurSamples;
   m_focalPoint = c.m_focalPoint;
   m_confidence = c.m_confidence;
   m_variance = c.m_variance;
   m_export = c.m_export;
}

PMCamera::~PMCamera()
{
}

QString PMCamera::description() const
{
   return ( "camera" );
}

void PMCamera::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "camera_type", cameraTypeToString( m_cameraType ) );
   e.setAttribute( "cylinder_type", m_cylinderType );
   e.setAttribute( "location", m_location.serializeXML() );
   e.setAttribute( "sky", m_sky.serializeXML() );
   e.setAttribute( "direction", m_direction.serializeXML() );
   e.setAttribute( "right", m_right.serializeXML() );
   e.setAttribute( "up", m_up.serializeXML() );
   e.setAttribute( "look_at", m_lookAt.serializeXML() );
   e.setAttribute( "angle_enabled", m_angleEnabled );
   e.setAttribute( "angle", m_angle );
   e.setAttribute( "focal_blur", m_focalBlurEnabled );
   e.setAttribute( "aperture", m_aperture );
   e.setAttribute( "blur_samples", m_blurSamples );
   e.setAttribute( "focal_point", m_focalPoint.serializeXML() );
   e.setAttribute( "confidence", m_confidence );
   e.setAttribute( "variance", m_variance );
   e.setAttribute( "export", m_export );

   Base::serialize( e, doc );
}

void PMCamera::readAttributes( const PMXMLHelper& h )
{
   m_cameraType = stringToCameraType( h.stringAttribute( "camera_type",
                                                         "perspective" ) );
   m_cylinderType = h.intAttribute( "cylinder_type", cylinderTypeDefault );
   m_location = h.vectorAttribute( "location", locationDefault );
   m_sky = h.vectorAttribute( "sky", skyDefault );
   m_direction = h.vectorAttribute( "direction", directionDefault );
   m_right = h.vectorAttribute( "right", rightDefault );
   m_up = h.vectorAttribute( "up", upDefault );
   m_lookAt = h.vectorAttribute( "look_at", lookAtDefault );
   m_angleEnabled = h.boolAttribute( "angle_enabled", angleEnabledDefault );
   m_angle = h.doubleAttribute( "angle", angleDefault );
   m_focalBlurEnabled = h.boolAttribute( "focal_blur", focalBlurDefault );
   m_aperture = h.doubleAttribute( "aperture", apertureDefault );
   m_blurSamples = h.intAttribute( "blur_samples", blurSamplesDefault );
   m_focalPoint = h.vectorAttribute( "focal_point", focalPointDefault );
   m_confidence = h.doubleAttribute( "confidence", confidenceDefault );
   m_variance = h.doubleAttribute( "variance", varianceDefault );
   m_export = h.boolAttribute( "export", c_defaultExport );
   Base::readAttributes( h );
}

PMMetaObject* PMCamera::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Camera", Base::metaObject(),
                                        createNewCamera );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "location", &PMCamera::setLocation,
                         &PMCamera::location ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "lookAt", &PMCamera::setLookAt,
                         &PMCamera::lookAt ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "up", &PMCamera::setUp,
                         &PMCamera::up ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "right", &PMCamera::setRight,
                         &PMCamera::right ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "direction", &PMCamera::setDirection,
                         &PMCamera::direction ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "sky", &PMCamera::setSky,
                         &PMCamera::sky ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "aspect", 0, &PMCamera::aspect ) );

      s_pMetaObject->addProperty(
         new PMCameraProperty( "angleEnabled", &PMCamera::enableAngle,
                         &PMCamera::isAngleEnabled ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "angle", &PMCamera::setAngle,
                         &PMCamera::angle ) );

      PMCameraTypeProperty* p = new PMCameraTypeProperty(
         "cameraType", &PMCamera::setCameraType,
         &PMCamera::cameraType );
      p->addEnumValue( "Perspective", Perspective );
      p->addEnumValue( "Orthographic", Orthographic );
      p->addEnumValue( "FishEye", FishEye );
      p->addEnumValue( "UltraWideAngle", UltraWideAngle );
      p->addEnumValue( "Omnimax", Omnimax );
      p->addEnumValue( "Panoramic", Panoramic );
      p->addEnumValue( "Cylinder", Cylinder );
      s_pMetaObject->addProperty( p );

      s_pMetaObject->addProperty(
         new PMCameraProperty( "cylinderType", &PMCamera::setCylinderType,
                         &PMCamera::cylinderType ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "focalBlurEnabled", &PMCamera::enableFocalBlur,
                         &PMCamera::isFocalBlurEnabled ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "aperture", &PMCamera::setAperture,
                         &PMCamera::aperture ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "blurSamples", &PMCamera::setBlurSamples,
                         &PMCamera::blurSamples ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "focalPoint", &PMCamera::setFocalPoint,
                         &PMCamera::focalPoint ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "confidence", &PMCamera::setConfidence,
                         &PMCamera::confidence ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "variance", &PMCamera::setVariance,
                         &PMCamera::variance ) );
      s_pMetaObject->addProperty(
         new PMCameraProperty( "export", &PMCamera::setExportPovray,
                         &PMCamera::exportPovray ) );
   }
   return s_pMetaObject;
}

void PMCamera::setLocation( const PMVector& p )
{
   if( p != m_location )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLocationID, m_location );
      m_location = p;
      m_location.resize( 3 );
      setViewStructureChanged();
   }
}

void PMCamera::setLookAt( const PMVector& p )
{
   if( p != m_lookAt )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLookAtID, m_lookAt );
      m_lookAt = p;
      m_lookAt.resize( 3 );
      setViewStructureChanged();
   }
}

void PMCamera::setUp( const PMVector& v )
{
   if( v != m_up )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUpID, m_up );
      m_up = v;
      m_up.resize( 3 );
      setViewStructureChanged();
   }
}

void PMCamera::setRight( const PMVector& v )
{
   if( v != m_right )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRightID, m_right );
      m_right = v;
      m_right.resize( 3 );
      setViewStructureChanged();
   }
}

void PMCamera::setDirection( const PMVector& v )
{
   if( v != m_direction )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDirectionID, m_direction );
      m_direction = v;
      m_direction.resize( 3 );
      setViewStructureChanged();
   }
}

void PMCamera::setSky( const PMVector& v )
{
   if( v != m_sky )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSkyID, m_sky );
      m_sky = v;
      m_sky.resize( 3 );
      setViewStructureChanged();
   }
}

double PMCamera::aspect() const
{
   double d = m_up.abs();
   if( approxZero( d ) )
      return 1.0;
   return m_right.abs() / d;
}

void PMCamera::enableAngle( bool yes )
{
   if( yes != m_angleEnabled )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAngleEnabledID, m_angleEnabled );
      m_angleEnabled = yes;
      setViewStructureChanged();
   }
}

void PMCamera::setAngle( double a )
{
   if( a != m_angle )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAngleID, m_angle );
      m_angle = a;
      setViewStructureChanged();
   }
}

void PMCamera::setCameraType( PMCamera::CameraType t )
{
   if( t != m_cameraType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCameraTypeID, m_cameraType );
      m_cameraType = t;
      setViewStructureChanged();
   }
}

void PMCamera::setCylinderType( int t )
{
   if( ( t < 1 ) || ( t > 4 ) )
      qCritical(  ) << "Invalid type in PMCylinder::setCylinderType\n";
   else if( t != m_cylinderType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCylinderTypeID, m_cylinderType );
      m_cylinderType = t;
      setViewStructureChanged();
   }
}

void PMCamera::enableFocalBlur( bool yes )
{
   if( yes != m_focalBlurEnabled )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFocalBlurID, m_focalBlurEnabled );
      m_focalBlurEnabled = yes;
   }
}

void PMCamera::setAperture( double a )
{
   if( a < 0 )
      qCritical(  ) << "Aperture < 0 in PMCylinder::setAperture\n";
   else if( a != m_aperture )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMApertureID, m_aperture );
      m_aperture = a;
   }
}

void PMCamera::setBlurSamples( int s )
{
   if( s < 0 )
      qCritical(  ) << "Samples < 0 in PMCylinder::setBlutSamples\n";
   else if( s != m_blurSamples )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBlurSamplesID, m_blurSamples );
      m_blurSamples = s;
   }
}

void PMCamera::setFocalPoint( const PMVector& v )
{
   if( v != m_focalPoint )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFocalPointID, m_focalPoint );
      m_focalPoint = v;
   }
}

void PMCamera::setConfidence( double c )
{
   if( ( c < 0.0 ) || ( c > 1.0 ) )
      qCritical(  ) << "Confidence not in [0.0 1.0] in PMCylinder::setConfidence\n";
   else if( c != m_confidence )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMConfidenceID, m_confidence );
      m_confidence = c;
   }
}

void PMCamera::setVariance( double v )
{
   if( v < 0 )
      qCritical(  ) << "Variance < 0 in PMCylinder::setVariance\n";
   else if( v != m_variance )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMVarianceID, m_variance );
      m_variance = v;
   }
}

void PMCamera::setExportPovray( bool ex )
{
   if( m_export != ex )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMExportID, m_export );
      m_export = ex;
   }
}

PMDialogEditBase* PMCamera::editWidget( QWidget* parent ) const
{
   return new PMCameraEdit( parent );
}

void PMCamera::restoreMemento( PMMemento* s )
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
            case PMLocationID:
               setLocation( data->vectorData() );
               break;
            case PMLookAtID:
               setLookAt( data->vectorData() );
               break;
            case PMUpID:
               setUp( data->vectorData() );
               break;
            case PMRightID:
               setRight( data->vectorData() );
               break;
            case PMDirectionID:
               setDirection( data->vectorData() );
               break;
            case PMSkyID:
               setSky( data->vectorData() );
               break;
            case PMAngleEnabledID:
               enableAngle( data->boolData() );
               break;
            case PMAngleID:
               setAngle( data->doubleData() );
               break;
            case PMCameraTypeID:
               setCameraType( ( CameraType ) data->intData() );
               break;
            case PMCylinderTypeID:
               setCylinderType( data->intData() );
               break;
            case PMFocalBlurID:
               enableFocalBlur( data->boolData() );
               break;
            case PMBlurSamplesID:
               setBlurSamples( data->intData() );
               break;
            case PMFocalPointID:
               setFocalPoint( data->vectorData() );
               break;
            case PMConfidenceID:
               setConfidence( data->doubleData() );
               break;
            case PMVarianceID:
               setVariance( data->doubleData() );
               break;
            case PMApertureID:
               setAperture( data->doubleData() );
               break;
            case PMExportID:
               setExportPovray( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMCamera::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMCamera::createViewStructure()
{
   PMVector newUp, newRight, newDirection, tmp;
   double upLen, rightLen;

   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
   }

   PMPointArray& points = m_pViewStructure->points();
   calculateLookAtAngle( newRight, newUp, newDirection );

   points[0] = m_lookAt;
   points[1] = m_location;

   upLen = newUp.abs();
   rightLen = newRight.abs();
   if( rightLen > upLen )
   {
      newUp /= rightLen;
      newRight /= rightLen;
      newDirection /= rightLen;
   }
   else
   {
      newUp /= upLen;
      newRight /= upLen;
      newDirection /= upLen;
   }

   newRight /= 2.0;
   newUp /= 2.0;
   tmp = m_location + newDirection;
   points[2] = tmp - newRight + newUp;
   points[3] = tmp - newRight - newUp;
   points[4] = tmp + newRight - newUp;
   points[5] = tmp + newRight + newUp;
//   points[6] = m_location + m_sky;
}

PMViewStructure* PMCamera::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure )
   {
      s_pDefaultViewStructure = new PMViewStructure( 6, 9 );
      PMLineArray& lines = s_pDefaultViewStructure->lines();

      lines[0] = PMLine( 0, 1 );
      lines[1] = PMLine( 1, 2 );
      lines[2] = PMLine( 1, 3 );
      lines[3] = PMLine( 1, 4 );
      lines[4] = PMLine( 1, 5 );
      lines[5] = PMLine( 2, 3 );
      lines[6] = PMLine( 2, 5 );
      lines[7] = PMLine( 3, 4 );
      lines[8] = PMLine( 4, 5 );
//      lines[9] = PMLine( 1, 6 );
   }
   return s_pDefaultViewStructure;
}

void PMCamera::controlPoints( PMControlPointList& list )
{
   PMControlPoint* p;
   p = new PM3DControlPoint( m_location, PMLocationID, ( "Location" ) );
   list.append( p );
   list.append( new PM3DControlPoint( m_lookAt, PMLookAtID, ( "Look at" ) ) );
}

void PMCamera::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it;
   for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMLocationID:
               setLocation( ( ( PM3DControlPoint* ) *it )->point() );
               break;
            case PMLookAtID:
               setLookAt( ( ( PM3DControlPoint* ) *it )->point() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMCamera::controlPointsChanged\n";
               break;
         }
      }
   }
}

void PMCamera::calculateLookAtAngle( PMVector& right, PMVector& up, PMVector& direction )
{
   PMVector tmpVector;
   double directionLen, upLen, rightLen, handedness, angle;

   angle = m_angle;
   if( m_cameraType == Perspective )
   {
      if( angle < 0.0 )
         angle = angleDefault;
      if( angle >= 180.0 )
         angle = angleDefault;
   }
   else
      angle = 90.0;

   directionLen = m_direction.abs();
   upLen = m_up.abs();
   rightLen = m_right.abs();

   // for safety: if vector is null vector, use the default one
   if( approxZero( directionLen ) )
   {
      direction = directionDefault;
      directionLen = direction.abs();
   }
   else
      direction = m_direction;

   if( approxZero( upLen ) )
   {
      up = upDefault;
      upLen = up.abs();
   }
   else
      up = m_up;

   if( approxZero( rightLen ) )
   {
      right = rightDefault;
      rightLen = right.abs();
   }
   else
      right = m_right;

   if( m_angleEnabled )
   {
      // calculate angle
      direction /= directionLen;
      directionLen = 0.5 * rightLen / tan( angle * M_PI / 360.0 );
      direction *= directionLen;
   }

   // calculate handedness
   tmpVector = PMVector::cross( up, direction );
   handedness = PMVector::dot( tmpVector, right );

   direction = m_lookAt - m_location;
   if( approxZero( direction.abs() ) )
   {
      // Camera location and look_at point are the same
      direction = directionDefault;
   }

   // normalize new direction
   direction /= direction.abs();

   tmpVector = right;
   right = PMVector::cross( m_sky, direction );

   if( approxZero( right.abs() ) )
      right = tmpVector;

   right /= right.abs();
   up = PMVector::cross( direction, right );
   direction *= directionLen;

   if( handedness > 0.0 )
      right *= rightLen;
   else
      right *= -rightLen;
   up *= upLen;
}

QString PMCamera::cameraTypeToString( PMCamera::CameraType t )
{
   QString str( "perspective" );
   switch( t )
   {
      case Perspective:
         break;
      case Orthographic:
         str = "orthographic";
         break;
      case FishEye:
         str = "fisheye";
         break;
      case UltraWideAngle:
         str = "ultra_wide_angle";
         break;
      case Omnimax:
         str = "omnimax";
         break;
      case Panoramic:
         str = "panoramic";
         break;
      case Cylinder:
         str = "cylinder";
         break;
   }
   return str;
}

PMCamera::CameraType PMCamera::stringToCameraType( const QString& str )
{
   CameraType t = Perspective;

   if( str == "perspective" )
      t = Perspective;
   else if( str == "orthographic" )
      t = Orthographic;
   else if( str == "fisheye" )
      t = FishEye;
   else if( str == "ultra_wide_angle" )
      t = UltraWideAngle;
   else if( str == "omnimax" )
      t = Omnimax;
   else if( str == "panoramic" )
      t = Panoramic;
   else if( str == "cylinder" )
      t = Cylinder;
   else
      qDebug() << "Unknown camera type\n";

   return t;
}

void PMCamera::cleanUp() const
{
   if( s_pDefaultViewStructure )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = 0;
   }
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }

   Base::cleanUp();
}
