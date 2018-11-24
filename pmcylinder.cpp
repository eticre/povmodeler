/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
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


#include "pmcylinder.h"

#include "pmxmlhelper.h"
#include "pmboxedit.h"
#include "pmmemento.h"
#include "pm3dcontrolpoint.h"
#include "pmdefaults.h"



#include "pmdistancecontrolpoint.h"

#include "pmcylinderedit.h"

const double defaultCylRadius = 0.5;
const double defaultHalfCylSize = 0.5;
const PMVector defaultEnd1 = PMVector ( 0, defaultHalfCylSize, 0 );
const PMVector defaultEnd2 = PMVector ( 0, -defaultHalfCylSize, 0 );
const bool defaultOpen = false;

/** default cylinder structure */
PMViewStructure* PMCylinder::s_pDefaultViewStructure = nullptr;
int PMCylinder::s_numSteps = c_defaultCylinderSteps;
int PMCylinder::s_parameterKey = 0;

PMDefinePropertyClass( PMCylinder, PMCylinderProperty );

PMMetaObject* PMCylinder::s_pMetaObject = nullptr;
PMObject* createNewCylinder( PMPart* part )
{
   return new PMCylinder( part );
}


PMCylinder::PMCylinder( PMPart* part )
      : Base( part )
{
   m_end1 = defaultEnd1;
   m_end2 = defaultEnd2;
   m_radius = defaultCylRadius;
   m_open = defaultOpen;
}

PMCylinder::PMCylinder( const PMCylinder& c )
      : Base( c )
{
   m_end1 = c.m_end1;
   m_end2 = c.m_end2;
   m_radius = c.m_radius;
   m_open = c.m_open;
}

PMCylinder::~PMCylinder()
{
}

QString PMCylinder::description() const
{
   return ( "cylinder" );
}

void PMCylinder::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "end_a", m_end1.serializeXML() );
   e.setAttribute( "end_b", m_end2.serializeXML() );
   e.setAttribute( "radius", m_radius );
   e.setAttribute( "open", m_open );
   Base::serialize( e, doc );
}

void PMCylinder::readAttributes( const PMXMLHelper& h )
{
   m_end1 = h.vectorAttribute( "end_a", defaultEnd1 );
   m_end2 = h.vectorAttribute( "end_b", defaultEnd2 );
   m_radius = h.doubleAttribute( "radius", defaultCylRadius );
   m_open = h.boolAttribute( "open", defaultOpen );
   Base::readAttributes( h );
}

PMMetaObject* PMCylinder::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Cylinder", Base::metaObject(),
                                        createNewCylinder );
      s_pMetaObject->addProperty(
         new PMCylinderProperty( "end1", &PMCylinder::setEnd1, &PMCylinder::end1 ) );
      s_pMetaObject->addProperty(
         new PMCylinderProperty( "end2", &PMCylinder::setEnd2, &PMCylinder::end2 ) );
      s_pMetaObject->addProperty(
         new PMCylinderProperty( "radius", &PMCylinder::setRadius, &PMCylinder::radius ) );
      s_pMetaObject->addProperty(
         new PMCylinderProperty( "open", &PMCylinder::setOpen, &PMCylinder::open ) );
   }
   return s_pMetaObject;
}

void PMCylinder::setEnd1( const PMVector& p )
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

void PMCylinder::setEnd2( const PMVector& p )
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

void PMCylinder::setRadius( double radius )
{
   if( m_radius != radius )
   {
      if( m_pMemento )
        m_pMemento->addData( s_pMetaObject, PMRadiusID, m_radius );
      m_radius = radius;
      setViewStructureChanged();
   }
}

void PMCylinder::setOpen( bool op )
{
   if( op != m_open )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOpenID, m_open );
      m_open = op;
   }
}

PMDialogEditBase* PMCylinder::editWidget( QWidget* parent ) const
{
   return new PMCylinderEdit( parent );
}

void PMCylinder::restoreMemento( PMMemento* s )
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
            case PMOpenID:
               setOpen( data->boolData() );
            break;
            default:
               qCritical(  ) << "Wrong ID in PMCylinder::restoreMemento\n";
            break;
         }
      }
   }
   Base::restoreMemento( s );
}

bool PMCylinder::isDefault()
{
   if( ( m_end1 == defaultEnd1 ) && ( m_end2 == defaultEnd2 )
       && ( m_radius == defaultCylRadius ) && ( m_open == defaultOpen )
         && globalDetail() )
      return true;
   return false;
}

void PMCylinder::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure () );
      m_pViewStructure->points().detach();
   }

   int steps = (int)( ( (float)s_numSteps / 2 ) * ( displayDetail() + 1 ) );
   int ptsSize = steps * 2;
   int lineSize = steps * 3;

   if( ptsSize != m_pViewStructure->points().size() )
      m_pViewStructure->points().resize( ptsSize );

   createPoints( m_pViewStructure->points(), m_end1, m_end2, m_radius, steps );

   if( ( lineSize ) != m_pViewStructure->lines().size() )
   {
      m_pViewStructure->lines().detach();
      m_pViewStructure->lines().resize( lineSize );
      createLines( m_pViewStructure->lines(), steps );
   }
}

PMViewStructure* PMCylinder::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure || s_pDefaultViewStructure->parameterKey() != viewStructureParameterKey() )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = 0;
      int steps = (int)( ( (float)s_numSteps / 2 ) * ( globalDetailLevel() + 1 ) );
      s_pDefaultViewStructure = new PMViewStructure( steps * 2, steps * 3 );

      createPoints( s_pDefaultViewStructure->points(), defaultEnd1,
                    defaultEnd2, defaultCylRadius, steps );

      createLines( s_pDefaultViewStructure->lines(), steps );
   }
   return s_pDefaultViewStructure;
}

void PMCylinder::createLines( PMLineArray& lines, int steps )
{
   int i;
   for( i = 0; i < ( steps - 1 ); i++ )
   {
         lines[i] = PMLine( i, i + 1 );
         lines[i + steps] = PMLine( i + steps, i + steps + 1 );
   }
   lines[steps - 1] = PMLine( steps - 1, 0 );
   lines[steps * 2 - 1] = PMLine( steps * 2 - 1, steps );

   for( i = 0; i < steps; i++ )
   {
         lines[i + 2 * steps] = PMLine( i, i + steps );
   }
}

void PMCylinder::createPoints( PMPointArray& points, const PMVector& end1,
                               const PMVector& end2, double radius, int steps )
{
   double angle = ( 2.0 * M_PI ) / ( double ) steps;

   PMVector pointAt = end2 - end1;
   double pl = pointAt.abs();
   if( approxZero( pl ) )
      pointAt = PMVector( 0.0, 0.0, 1.0 );
   else
      pointAt /= pl;

   PMMatrix rotation = PMMatrix::rotation( pointAt, angle );
   PMVector endPoint = pointAt.orthogonal();
   endPoint *= radius;

   int i;
   for( i = 0; i < steps; i++ )
   {
      points[i] = PMPoint( endPoint + end1 );
      points[i + steps] = PMPoint( endPoint + end2 );
      endPoint = rotation * endPoint;
   }
}

void PMCylinder::controlPoints( PMControlPointList & list )
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


void PMCylinder::controlPointsChanged( PMControlPointList & list )
{
   bool pointChanged = false;
   bool radiusChanged = false;
	PMControlPointList::iterator it;
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
               qCritical(  ) << "Wrong ID in PMCylinder::controlPointsChanged\n";
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

void PMCylinder::setSteps( int s )
{
   if( s >= 4 )
   {
      s_numSteps = s;
      if( s_pDefaultViewStructure )
      {
         delete s_pDefaultViewStructure;
         s_pDefaultViewStructure = 0;
      }
   }
   else
      qDebug() << "PMCylinder::setSteps: S must be greater than 3\n";
   s_parameterKey++;
}

void PMCylinder::cleanUp() const
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
