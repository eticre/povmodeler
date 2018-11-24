/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#include "pmblobcylinder.h"

#include "pmxmlhelper.h"
#include "pmboxedit.h"
#include "pmmemento.h"
#include "pm3dcontrolpoint.h"
#include "pmdefaults.h"
#include "pmdistancecontrolpoint.h"
#include "pmblobcylinderedit.h"



PMDefinePropertyClass( PMBlobCylinder, PMBlobCylinderProperty );

PMMetaObject* PMBlobCylinder::s_pMetaObject = nullptr;
PMObject* createNewBlobCylinder( PMPart* part )
{
   return new PMBlobCylinder( part );
}

const double c_defaultRadius = 0.5;
const PMVector c_defaultEnd1 = PMVector ( 0, 0.5, 0 );
const PMVector c_defaultEnd2 = PMVector ( 0, -0.5, 0 );
const double c_defaultStrength = 1.0;

/** default cylinder structure */
PMViewStructure* PMBlobCylinder::s_pDefaultViewStructure = nullptr;
int PMBlobCylinder::s_vStep = c_defaultBlobCylinderVSteps;
int PMBlobCylinder::s_uStep = c_defaultBlobCylinderUSteps;
int PMBlobCylinder::s_parameterKey = 0;


PMBlobCylinder::PMBlobCylinder( PMPart* part )
      : Base( part )
{
   m_end1 = c_defaultEnd1;
   m_end2 = c_defaultEnd2;
   m_radius = c_defaultRadius;
   m_strength = c_defaultStrength;
}

PMBlobCylinder::PMBlobCylinder( const PMBlobCylinder& c )
      : Base( c )
{
   m_end1 = c.m_end1;
   m_end2 = c.m_end2;
   m_radius = c.m_radius;
   m_strength = c.m_strength;
}

PMBlobCylinder::~PMBlobCylinder()
{
}

QString PMBlobCylinder::description() const
{
   return ( "blob cylinder" );
}

void PMBlobCylinder::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "end_a", m_end1.serializeXML() );
   e.setAttribute( "end_b", m_end2.serializeXML() );
   e.setAttribute( "radius", m_radius );
   e.setAttribute( "strength", m_strength );
   Base::serialize( e, doc );
}

void PMBlobCylinder::readAttributes( const PMXMLHelper& h )
{
   m_end1 = h.vectorAttribute( "end_a", c_defaultEnd1 );
   m_end2 = h.vectorAttribute( "end_b", c_defaultEnd2 );
   m_radius = h.doubleAttribute( "radius", c_defaultRadius );
   m_strength = h.doubleAttribute( "strength", c_defaultStrength );
   Base::readAttributes( h );
}

PMMetaObject* PMBlobCylinder::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "BlobCylinder", Base::metaObject(),
                                        createNewBlobCylinder );
      s_pMetaObject->addProperty(
         new PMBlobCylinderProperty( "end1", &PMBlobCylinder::setEnd1,
                         &PMBlobCylinder::end1 ) );
      s_pMetaObject->addProperty(
         new PMBlobCylinderProperty( "end2", &PMBlobCylinder::setEnd2,
                         &PMBlobCylinder::end2 ) );
      s_pMetaObject->addProperty(
         new PMBlobCylinderProperty( "radius", &PMBlobCylinder::setRadius,
                         &PMBlobCylinder::radius ) );
      s_pMetaObject->addProperty(
         new PMBlobCylinderProperty( "strength", &PMBlobCylinder::setStrength,
                         &PMBlobCylinder::strength ) );
   }
   return s_pMetaObject;
}

void PMBlobCylinder::setEnd1( const PMVector& p )
{
   if( p != m_end1 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnd1ID, m_end1 );
      m_end1 = p;
      m_end1.resize( 3 );
      setViewStructureChanged();
   }
}

void PMBlobCylinder::setEnd2( const PMVector& p )
{
   if( p != m_end2 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnd2ID, m_end2 );
      m_end2 = p;
      m_end2.resize( 3 );
      setViewStructureChanged();
   }
}

void PMBlobCylinder::setRadius( double radius )
{
   if( m_radius != radius )
   {
      if( m_pMemento )
        m_pMemento->addData( s_pMetaObject, PMRadiusID, m_radius );
      m_radius = radius;
      setViewStructureChanged();
   }
}

void PMBlobCylinder::setStrength( double s )
{
   if( s != m_strength )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMStrengthID, m_strength );
      m_strength = s;
   }
}

PMDialogEditBase* PMBlobCylinder::editWidget( QWidget* parent ) const
{
   return new PMBlobCylinderEdit( parent );
}

void PMBlobCylinder::restoreMemento( PMMemento* s )
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
            case PMEnd1ID:
               setEnd1( data->vectorData() );
               break;
            case PMEnd2ID:
               setEnd2( data->vectorData() );
               break;
            case PMRadiusID:
               setRadius( data->doubleData() );
               break;
            case PMStrengthID:
               setStrength( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMBlobCylinder::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

bool PMBlobCylinder::isDefault()
{
   if( ( m_end1 == c_defaultEnd1 ) && ( m_end2 == c_defaultEnd2 )
       && ( m_radius == c_defaultRadius )
       && globalDetail() )
      return true;
   return false;
}

void PMBlobCylinder::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure () );
      m_pViewStructure->points().detach();
   }

   int uStep = (int)( ( (float)s_uStep / 2 ) * ( displayDetail() + 1 ) );
   int vStep = (int)( ( (float)s_vStep / 2 ) * ( displayDetail() + 1 ) );
   int ptsSize = vStep * uStep * 2 + 2;
   int lineSize = vStep * uStep * 4 + vStep;

   if( ptsSize != m_pViewStructure->points().size() )
      m_pViewStructure->points().resize( ptsSize );

   createPoints( m_pViewStructure->points(), m_end1, m_end2, m_radius, uStep, vStep );

   if( lineSize != m_pViewStructure->lines().size() )
   {
      m_pViewStructure->lines().detach();
      m_pViewStructure->lines().resize( lineSize );
      createLines( m_pViewStructure->lines(), uStep, vStep );
   }
}

PMViewStructure* PMBlobCylinder::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure || s_pDefaultViewStructure->parameterKey() != viewStructureParameterKey() )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = 0;
      int uStep = (int)( ( (float)s_uStep / 2 ) * ( displayDetail() + 1 ) );
      int vStep = (int)( ( (float)s_vStep / 2 ) * ( displayDetail() + 1 ) );

      s_pDefaultViewStructure =
         new PMViewStructure( vStep * uStep * 2 + 2,
                              vStep * uStep * 4 + vStep );

      createPoints( s_pDefaultViewStructure->points(), c_defaultEnd1,
                    c_defaultEnd2, c_defaultRadius, uStep, vStep );

      createLines( s_pDefaultViewStructure->lines(), uStep, vStep );
   }
   return s_pDefaultViewStructure;
}

void PMBlobCylinder::createLines( PMLineArray& lines, int uStep, int vStep )
{
   int u, v;
   int offset = 0;

   // horizontal lines
   for( u = 0; u < ( uStep * 2 ); u++ )
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
      for( u = 0; u < ( 2 * uStep - 1 ); u++ )
      {
         lines[offset + u] =
            PMLine( u * vStep + v + 1, ( u + 1 ) * vStep + v + 1 );
      }
      offset += ( 2 * uStep - 1 );
   }
   // lines at the "south pole"
   for( v = 0; v < vStep; v++ )
      lines[offset + v] = PMLine( ( 2 * uStep - 1 ) * vStep + v + 1,
                                    2 * uStep * vStep + 1 );
   // offset += vStep;
}

void PMBlobCylinder::createPoints( PMPointArray& points, const PMVector& end1,
                                   const PMVector& end2, double radius, int uStep, int vStep )
{
   double uRadStep = M_PI / uStep / 2.0;
   double vRadStep = 2 * M_PI / vStep;
   double du = uRadStep;

   if( radius < 0 )
      radius = -radius;

   PMVector pointAt = end2 - end1;
   double pl = pointAt.abs();
   if( approxZero( pl ) )
      pointAt = PMVector( 0.0, 0.0, 1.0 );
   else
      pointAt /= pl;

   PMMatrix rotation = PMMatrix::rotation( pointAt, vRadStep );
   PMVector ortho = pointAt.orthogonal();
   ortho /= ortho.abs();

   points[0] = PMPoint( end1 - pointAt * radius );
   points[vStep * uStep * 2 + 1] = PMPoint( end2 + pointAt * radius );

   int u, v;
   for( u = 0; u < uStep; u++ )
   {
      PMVector end = ortho * radius * sin( du );
      PMVector pv = pointAt * radius * cos( du );
      PMVector e1 = end1 - pv;
      PMVector e2 = end2 + pv;

      for( v = 0; v < vStep; v++ )
      {
         points[u * vStep + v + 1] = PMPoint( e1 + end );
         points[vStep * uStep * 2 - ( u + 1 ) * vStep + v + 1]
            = PMPoint( e2 + end );
         end = rotation * end;
      }
      du += uRadStep;
   }
}

void PMBlobCylinder::controlPoints( PMControlPointList & list )
{
   PMVector center, angle1, angle2;
   center = m_end1 - m_end2;
   double pl = center.abs();
   if( approxZero( pl ) )
      center = PMVector( 0.0, 1.0, 0.0 );
   else
      center /= pl;

   angle1 = center.orthogonal();
   angle2 = PMVector::cross( center, angle1 );

   PM3DControlPoint* pb = new PM3DControlPoint( m_end1, PMEnd1ID, ( "End 1" ) );
   list.append( pb );
   list.append( new PM3DControlPoint( m_end2, PMEnd2ID, ( "End 2" ) ) );
   list.append( new PMDistanceControlPoint( pb, angle1, m_radius, PMRadiusID, ( "Radius (1)" ) ) );
   list.append( new PMDistanceControlPoint( pb, angle2, m_radius, PMRadiusID, ( "Radius (2)" ) ) );
}


void PMBlobCylinder::controlPointsChanged( PMControlPointList & list )
{
	PMControlPointList::iterator it;
   bool pointChanged = false;
   bool radiusChanged = false;

   for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMEnd1ID:
               setEnd1( ( ( PM3DControlPoint *) *it )->point() );
               pointChanged = true;
               break;
            case PMEnd2ID:
               setEnd2( ( ( PM3DControlPoint *) *it )->point() );
               pointChanged = true;
               break;
            case PMRadiusID:
               setRadius( ( ( PMDistanceControlPoint *) *it )->distance() );
               radiusChanged = true;
               break;
            default:
               qCritical(  ) << "Wrong ID in PMBlobCylinder::controlPointsChanged\n";
               break;
         }
      }
   }

   if( pointChanged )
   {
      PMVector center, angle1, angle2;
      bool firstPoint = true;

      center = m_end1 - m_end2;
      double pl = center.abs();
      if( approxZero( pl ) )
         center = PMVector( 0.0, 1.0, 0.0 );
      else
        center /= pl;

      angle1 = center.orthogonal();
      angle2 = PMVector::cross( center, angle1 );

		for( it = list.begin(); it != list.end(); ++it )
		{
         if( ( *it )->id() == PMRadiusID )
         {
            if( firstPoint )
            {
               ( ( PMDistanceControlPoint *) *it )->setDirection( angle1 );
               firstPoint = false;
            }
            else
               ( ( PMDistanceControlPoint *) *it )->setDirection( angle2 );
         }
		}
	}
	
   if( radiusChanged )
		for( it = list.begin(); it != list.end(); ++it )
			if( ( *it )->id() == PMRadiusID )
				( ( PMDistanceControlPoint *) *it )->setDistance( m_radius );
}

void PMBlobCylinder::setUSteps( int u )
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
      qDebug() << "PMBlobCylinder::setUSteps: U must be greater than 1\n";
   s_parameterKey++;
}

void PMBlobCylinder::setVSteps( int v )
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
      qDebug() << "PMBlobCylinder::setVSteps: V must be greater than 3\n";
   s_parameterKey++;
}

void PMBlobCylinder::cleanUp() const
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
