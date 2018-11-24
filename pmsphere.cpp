/*
**************************************************************************
                          pmsphere.cpp  -  description
                             -------------------
    copyright            : (C) 2001 by Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
    copyright            : (C) 2001-2002 by Andreas Zehender
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

#include "pmsphere.h"

#include "pmxmlhelper.h"
#include "pmsphereedit.h"
#include "pmmemento.h"
#include "pm3dcontrolpoint.h"
#include "pmdistancecontrolpoint.h"
#include "pmdefaults.h"



/** default param for the sphere */
const double c_defaultRadius = 0.5;
const PMVector c_defaultCentre = PMVector( 0, 0, 0 );

/** default sphere structure */
PMViewStructure* PMSphere::s_pDefaultViewStructure = 0;

int PMSphere::s_vStep = c_defaultSphereVSteps;
int PMSphere::s_uStep = c_defaultSphereUSteps;
int PMSphere::s_parameterKey = 0;

PMDefinePropertyClass( PMSphere, PMSphereProperty );

PMMetaObject* PMSphere::s_pMetaObject = 0;
PMObject* createNewSphere( PMPart* part )
{
   return new PMSphere( part );
}

PMSphere::PMSphere( PMPart* part )
      : Base( part )
{
   m_radius = c_defaultRadius;
   m_centre = c_defaultCentre;
}

PMSphere::PMSphere( const PMSphere& s )
      : Base( s )
{
   m_radius = s.m_radius;
   m_centre = s.m_centre;
}

PMSphere::~PMSphere()
{
}


QString PMSphere::description() const
{
   return ( "sphere" );
}

PMMetaObject* PMSphere::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Sphere", Base::metaObject(),
                                        createNewSphere );
      s_pMetaObject->addProperty(
         new PMSphereProperty( "radius", &PMSphere::setRadius, &PMSphere::radius ) );
      s_pMetaObject->addProperty(
         new PMSphereProperty( "center", &PMSphere::setCentre, &PMSphere::centre ) );
   }
   return s_pMetaObject;
}

void PMSphere::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "centre", m_centre.serializeXML() );
   e.setAttribute( "radius", m_radius );
   Base::serialize( e, doc );
}

void PMSphere::readAttributes( const PMXMLHelper& h )
{
   m_centre = h.vectorAttribute( "centre", c_defaultCentre );
   m_radius = h.doubleAttribute( "radius", c_defaultRadius );
   Base::readAttributes( h );
}

PMDialogEditBase* PMSphere::editWidget( QWidget* parent ) const
{
	return new PMSphereEdit( parent );
}

void PMSphere::restoreMemento( PMMemento* s )
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
            case PMRadiusID:
               setRadius( data->doubleData() );
               break;
            case PMCentreID:
               setCentre( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PSphere::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );

}

void PMSphere::controlPoints( PMControlPointList& list )
{
   PM3DControlPoint* p = new PM3DControlPoint( m_centre, PMCentreID,
                                               ( "Center" ) );
   list.append( p );
   list.append( new PMDistanceControlPoint( p, PMVector( 1.0, 0.0, 0.0 ),
                                            m_radius, PMRadiusID,
                                            ( "Radius (x)" ) ) );
   list.append( new PMDistanceControlPoint( p, PMVector( 0.0, 1.0, 0.0 ),
                                            m_radius, PMRadiusID,
                                            ( "Radius (y)" ) ) );
   list.append( new PMDistanceControlPoint( p, PMVector( 0.0, 0.0, 1.0 ),
                                            m_radius, PMRadiusID,
                                            ( "Radius (z)" ) ) );
}

void PMSphere::controlPointsChanged( PMControlPointList& list )
{
   bool radiusChanged = false;
	PMControlPointList::iterator it;

	for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMCentreID:
               setCentre( ( ( PM3DControlPoint* ) *it )->point() );
               break;
            case PMRadiusID:
               setRadius( ( ( PMDistanceControlPoint* ) *it )->distance() );
               radiusChanged = true;
               break;
            default:
               qCritical(  ) << "Wrong ID in PMSphere::controlPointsChanged\n";
               break;
         }
      }
   }

   if( radiusChanged )
		for( it = list.begin(); it != list.end(); ++it )
         if( ( *it )->id() == PMRadiusID )
            ( ( PMDistanceControlPoint* ) *it )->setDistance( m_radius );
}

bool PMSphere::isDefault()
{
   if( ( m_radius == c_defaultRadius ) && ( m_centre == c_defaultCentre ) && globalDetail() )
      return true;
   return false;
}

void PMSphere::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
   }

   int uStep = (int)( ( (float)s_uStep / 2 ) * ( displayDetail() + 1 ) );
   int vStep = (int)( ( (float)s_vStep / 2 ) * ( displayDetail() + 1 ) );
   int ptsSize = vStep * ( uStep - 1 ) + 2;
   int lineSize = vStep * ( uStep - 1 ) * 2 + vStep;
   int faceSize = vStep * uStep;

   if( ptsSize != m_pViewStructure->points().size() )
      m_pViewStructure->points().resize( ptsSize );

   createPoints( m_pViewStructure->points(), m_centre, m_radius, uStep, vStep );

   if( lineSize != m_pViewStructure->lines().size() )
   {
      m_pViewStructure->lines().detach();
      m_pViewStructure->lines().resize( lineSize );
      createLines( m_pViewStructure->lines(), uStep, vStep );
   }

   if( faceSize != m_pViewStructure->faces().size() )
   {
      m_pViewStructure->faces().resize( faceSize );
      createFaces( m_pViewStructure->faces(), uStep, vStep );
   }
}

PMViewStructure* PMSphere::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure || s_pDefaultViewStructure->parameterKey() != viewStructureParameterKey() )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = 0;
      int uStep = (int)( ( (float)s_uStep / 2 ) * ( globalDetailLevel() + 1 ) );
      int vStep = (int)( ( (float)s_vStep / 2 ) * ( globalDetailLevel() + 1 ) );

      s_pDefaultViewStructure =
         new PMViewStructure( vStep * ( uStep - 1 ) + 2,
                              vStep * ( uStep - 1 ) * 2 + vStep,
                              vStep * uStep );

      // points
      createPoints( s_pDefaultViewStructure->points(), c_defaultCentre,
                    c_defaultRadius, uStep, vStep );
      //lines
      createLines( s_pDefaultViewStructure->lines(), uStep, vStep );

      //faces
      createFaces( s_pDefaultViewStructure->faces(), uStep, vStep );
   }

   return s_pDefaultViewStructure;
}

void PMSphere::createFaces( PMFaceArray& faces, int uStep, int vStep )
{
   int u, v, offsetPt1, offsetPt2, offsetFace;

   offsetPt1 = vStep * ( uStep - 1 ) + 1;
   offsetPt2 = vStep * ( uStep - 2 ) + 1;
   offsetFace = vStep * ( uStep - 1 );
   for( v = 0; v < vStep - 1; ++v )
   {
      faces[v] = PMFace( 0, v + 2, v + 1 );
      faces[ offsetFace + v ] = PMFace( offsetPt1, v + offsetPt2, v + offsetPt2 + 1 );
   }

   faces[ vStep - 1 ] = PMFace( 0, 1, vStep );
   faces[ offsetFace + vStep - 1 ] = PMFace( offsetPt1, vStep + offsetPt2 - 1, offsetPt2 );

   offsetFace = vStep;
   for( u = 0; u < ( uStep - 2 ); ++u )
   {
      offsetPt1 = ( u * vStep ) + 1;
      offsetPt2 = ( ( u + 1 ) * vStep ) + 1;
      for( v = 0; v < ( vStep - 1 ); ++v )
         faces[ offsetFace + v ] = PMFace( v + offsetPt1, v + offsetPt1 + 1, v + offsetPt2 + 1, v + offsetPt2 );
      faces[ offsetFace + vStep - 1 ] = PMFace( offsetPt1 + vStep - 1, offsetPt1, offsetPt2, offsetPt2 + vStep - 1 );
      offsetFace += vStep;
   }
}

void PMSphere::createLines( PMLineArray& lines, int uStep, int vStep )
{
   int u, v;
   int offset = 0;

      // horizontal lines
   for( u = 0; u < ( uStep - 1 ); u++ )
   {
      for( v = 0; v < ( vStep - 1 ); v++ )
         lines[offset + v] =
               PMLine( u * vStep + v + 1, u * vStep + v + 2 );
      lines[offset + vStep - 1] =
            PMLine( u * vStep + 1, u * vStep + vStep );

      offset += vStep;
   }

   // vertical lines
   // lines at the "north pole"
   for( v = 0; v < vStep; v++ )
      lines[offset + v] = PMLine( 0, v + 1 );
   offset += vStep;

   for( v = 0; v < vStep; v++ )
   {
      for( u = 0; u < ( uStep - 2 ); u++ )
      {
         lines[offset + u] =
               PMLine( u * vStep + v + 1, ( u + 1 ) * vStep + v + 1 );
      }
      offset += ( uStep - 2 );
   }
   // lines at the "south pole"
   for( v = 0; v < vStep; v++ )
      lines[offset + v] = PMLine( ( uStep - 2 ) * vStep + v + 1,
                                    ( uStep - 1 ) * vStep + 1 );
   // offset += s_vStep;
}

void PMSphere::createPoints( PMPointArray& points, const PMVector& centre, double radius,
                             int uStep, int vStep )
{
   double l_UradStep = M_PI / uStep;
   double l_VradStep = ( 2.0 * M_PI ) / vStep;
   double l_u = l_UradStep;
   int u, v;

   points[0] = PMPoint( centre + PMVector( 0, radius, 0 ) );
   points[ vStep * ( uStep - 1 ) + 1] = PMPoint( centre - PMVector( 0, radius, 0 ) );

   for( u = 0; u < ( uStep - 1 ); u++ )
   {
      double l_v = 0.0;
      double l_rcosu = radius * sin( l_u );
      double y = ( radius * cos( l_u ) ) + centre[1];
      for( v = 0; v < vStep ; v++ )
      {

         double x = ( l_rcosu * cos( l_v ) ) + centre[0];
         double z = ( l_rcosu * sin( l_v ) ) + centre[2];

         points[u * vStep + v + 1] = PMPoint( x, y, z );
         l_v = l_v + l_VradStep;
      }
      l_u = l_u + l_UradStep;
   }
}

void PMSphere::setRadius( double radius )
{
   if( m_radius != radius )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusID, m_radius );
      m_radius = radius;
      setViewStructureChanged();
   }
}

void PMSphere::setCentre( const PMVector& centre )
{
   if( m_centre != centre )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCentreID, m_centre );
      m_centre = centre;
      setViewStructureChanged();
   }
}

void PMSphere::setUSteps( int u )
{
   if( u >= 2 )
   {
      s_uStep = u;
      if( s_pDefaultViewStructure )
      {
         delete s_pDefaultViewStructure;
         s_pDefaultViewStructure = 0;
      }
   }
   else
      qDebug() << "PMSPhere::setUSteps: U must be greater than 1\n";
   s_parameterKey++;
}

void PMSphere::setVSteps( int v )
{
   if( v >= 4 )
   {
      s_vStep = v;
      if( s_pDefaultViewStructure )
      {
         delete s_pDefaultViewStructure;
         s_pDefaultViewStructure = 0;
      }
   }
   else
      qDebug() << "PMSphere::setVSteps: V must be greater than 3\n";
   s_parameterKey++;
}

void PMSphere::cleanUp() const
{
   if( s_pDefaultViewStructure )
      delete s_pDefaultViewStructure;
   s_pDefaultViewStructure = 0;
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}
