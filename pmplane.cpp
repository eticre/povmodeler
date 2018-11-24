/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmplane.h"

#include "pmxmlhelper.h"
#include "pmboxedit.h"
#include "pmmemento.h"
#include "pmdistancecontrolpoint.h"
#include "pmplanenormalcontrolpoint.h"
#include "pmdefaults.h"



#include "pmplaneedit.h"

const double defaultPlaneDistance = 0;
const PMVector defaultPlaneNormal = PMVector ( 0.0, 1.0, 0.0 );

/** default plane structure */
PMViewStructure* PMPlane::s_pDefaultViewStructure = 0;
double PMPlane::s_planeSize = c_defaultPlaneSize;
int PMPlane::s_parameterKey = 0;

PMDefinePropertyClass( PMPlane, PMPlaneProperty );

PMMetaObject* PMPlane::s_pMetaObject = 0;
PMObject* createNewPlane( PMPart* part )
{
   return new PMPlane( part );
}

PMPlane::PMPlane( PMPart* part )
      : Base( part )
{
   m_normal = defaultPlaneNormal;
   m_distance = defaultPlaneDistance;
}

PMPlane::PMPlane( const PMPlane& p )
      : Base( p )
{
   m_normal = p.m_normal;
   m_distance = p.m_distance;
}

PMPlane::~PMPlane()
{
}

QString PMPlane::description() const
{
   return ( "plane" );
}

void PMPlane::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "normal", m_normal.serializeXML() );
   e.setAttribute( "distance", m_distance );
   Base::serialize( e, doc );
}

void PMPlane::readAttributes( const PMXMLHelper& h )
{
   m_normal = h.vectorAttribute( "normal", defaultPlaneNormal );
   m_distance = h.doubleAttribute( "distance", defaultPlaneDistance );
   Base::readAttributes( h );
}

PMMetaObject* PMPlane::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Plane", Base::metaObject(),
                                        createNewPlane );
      s_pMetaObject->addProperty(
         new PMPlaneProperty( "normal", &PMPlane::setNormal, &PMPlane::normal ) );
      s_pMetaObject->addProperty(
         new PMPlaneProperty( "distance", &PMPlane::setDistance, &PMPlane::distance ) );
   }
   return s_pMetaObject;
}

void PMPlane::setNormal( const PMVector& p )
{

   if( p != m_normal )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMNormalID, m_normal );
      }
      m_normal = p;
      m_normal.resize( 3 );

      setViewStructureChanged();
   }
}

void PMPlane::setDistance( double distance )
{
   if( m_distance != distance )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMDistanceID, m_distance );
      }
      m_distance = distance;

      setViewStructureChanged();
   }
}

PMDialogEditBase* PMPlane::editWidget( QWidget* parent ) const
{
   return new PMPlaneEdit( parent );
}

void PMPlane::restoreMemento( PMMemento* s )
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
            case PMNormalID:
               setNormal( data->vectorData() );
               break;
            case PMDistanceID:
               setDistance( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMPlane::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

bool PMPlane::isDefault()
{
   if( ( m_normal == defaultPlaneNormal ) && ( m_distance == defaultPlaneDistance ) )
      return true;
   return false;
}

void PMPlane::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure () );
      m_pViewStructure->points().detach();
   }
   createPoints( m_pViewStructure->points(), m_normal, m_distance );
}

PMViewStructure* PMPlane::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure )
   {
      s_pDefaultViewStructure = new PMViewStructure( 4, 4 );
      PMLineArray& lines = s_pDefaultViewStructure->lines();

      createPoints( s_pDefaultViewStructure->points(), defaultPlaneNormal,defaultPlaneDistance );

      lines[0] = PMLine( 0 , 1 );
      lines[1] = PMLine( 1 , 2 );
      lines[2] = PMLine( 2 , 3 );
      lines[3] = PMLine( 3 , 0 );
   }
   return s_pDefaultViewStructure;
}

void PMPlane::createPoints( PMPointArray& points, const PMVector& normal, double distance )
{
   PMVector dir = normal;
   if( approxZero( dir.abs() ) )
      dir = PMVector( 0.0, 1.0, 0.0 );

   PMVector base = dir * distance;
   PMMatrix rotation = PMMatrix::rotation( dir, M_PI / 4.0 );

   PMVector endPoint1 = rotation * dir.orthogonal() * s_planeSize * sqrt( 2.0 ) * 0.5;
   PMVector endPoint2 = rotation * ( rotation * endPoint1 );

   points[0] = base + endPoint1;
   points[1] = base + endPoint2;
   points[2] = base - endPoint1;
   points[3] = base - endPoint2;
}

void PMPlane::controlPoints( PMControlPointList & list )
{
   PMDistanceControlPoint* d;
   d = new PMDistanceControlPoint( PMVector( 0, 0, 0 ), m_normal, m_distance,
                                   PMDistanceID, ( "Distance" ) );
   list.append( new PMPlaneNormalControlPoint( d, m_normal, PMNormalID,
                                               ( "Normal" ) ) );
   list.append( d );
}


void PMPlane::controlPointsChanged( PMControlPointList & list )
{
	PMControlPointList::iterator it;

	for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMNormalID:
               setNormal( ( ( PMPlaneNormalControlPoint *) *it )->normal() );
               setDistance( ( ( PMPlaneNormalControlPoint *) *it )->distance() );
               break;
            case PMDistanceID:
               setDistance( ( ( PMDistanceControlPoint *) *it )->distance() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMPlane::controlPointsChanged\n";
               break;
         }
      }
   }
}

void PMPlane::setPlaneSize( double size )
{
   if( size >= 0.1 )
   {
      s_planeSize = size;
      if( s_pDefaultViewStructure )
      {
         delete s_pDefaultViewStructure;
         s_pDefaultViewStructure = 0;
      }
   }
   else
      qDebug() << "PMPlane::setPlaneSize: Size must be greater than 0.1\n";
   s_parameterKey++;
}

void PMPlane::cleanUp() const
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
