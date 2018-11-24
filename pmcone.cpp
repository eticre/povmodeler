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


#include "pmcone.h"

#include "pmxmlhelper.h"
#include "pmboxedit.h"
#include "pmmemento.h"
#include "pm3dcontrolpoint.h"
#include "pmdefaults.h"
#include "pmdistancecontrolpoint.h"
#include "pmconeedit.h"




const double defaultConeRadius1 = 0.0;
const double defaultConeRadius2 = 0.5;
const double defaultHalfConeSize = 0.5;
const PMVector defaultEnd1 = PMVector ( 0, defaultHalfConeSize, 0 );
const PMVector defaultEnd2 = PMVector ( 0, -defaultHalfConeSize, 0 );
const bool defaultOpen = false;

/** default cone structure */
PMViewStructure* PMCone::s_pDefaultViewStructure = 0;
int PMCone::s_numSteps = c_defaultConeSteps;
int PMCone::s_parameterKey = 0;

PMDefinePropertyClass( PMCone, PMConeProperty );

PMMetaObject* PMCone::s_pMetaObject = 0;
PMObject* createNewCone( PMPart* part )
{
   return new PMCone( part );
}

PMCone::PMCone( PMPart* part )
      :Base( part )
{
   m_end1 = defaultEnd1;
   m_end2 = defaultEnd2;
   m_radius1 = defaultConeRadius1;
   m_radius2 = defaultConeRadius2;
   m_open = defaultOpen;
}

PMCone::PMCone( const PMCone& c )
      : Base( c )
{
   m_end1 = c.m_end1;
   m_end2 = c.m_end2;
   m_radius1 = c.m_radius1;
   m_radius2 = c.m_radius2;
   m_open = c.m_open;
}

PMCone::~PMCone()
{
}

QString PMCone::description() const
{
   return ( "cone" );
}

void PMCone::serialize( QDomElement & e, QDomDocument & doc ) const
{
   e.setAttribute( "end_a", m_end1.serializeXML() );
   e.setAttribute( "end_b", m_end2.serializeXML() );
   e.setAttribute( "radius_a", m_radius1 );
   e.setAttribute( "radius_b", m_radius2 );
   e.setAttribute( "open", m_open );
   Base::serialize( e, doc );
}

void PMCone::readAttributes( const PMXMLHelper & h )
{
   m_end1 = h.vectorAttribute( "end_a", defaultEnd1 );
   m_end2 = h.vectorAttribute( "end_b", defaultEnd2 );
   m_radius1 = h.doubleAttribute( "radius_a", defaultConeRadius1 );
   m_radius2 = h.doubleAttribute( "radius_b", defaultConeRadius2 );
   m_open = h.boolAttribute( "open", defaultOpen );
   Base::readAttributes( h );
}

PMMetaObject* PMCone::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Cone", Base::metaObject(),
                                        createNewCone );
      s_pMetaObject->addProperty(
         new PMConeProperty( "end1", &PMCone::setEnd1, &PMCone::end1 ) );
      s_pMetaObject->addProperty(
         new PMConeProperty( "end2", &PMCone::setEnd2, &PMCone::end2 ) );
      s_pMetaObject->addProperty(
         new PMConeProperty( "radius1", &PMCone::setRadius1, &PMCone::radius1 ) );
      s_pMetaObject->addProperty(
         new PMConeProperty( "radius2", &PMCone::setRadius2, &PMCone::radius2 ) );
      s_pMetaObject->addProperty(
         new PMConeProperty( "open", &PMCone::setOpen, &PMCone::open ) );
   }
   return s_pMetaObject;
}

void PMCone::setEnd1( const PMVector & p )
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

void PMCone::setEnd2( const PMVector & p )
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

void PMCone::setRadius1( double radius )
{
   if( m_radius1 != radius )
   {
      if( m_pMemento )
        m_pMemento->addData( s_pMetaObject, PMRadius1ID, m_radius1 );
      m_radius1 = radius;
      setViewStructureChanged();
   }
}

void PMCone::setRadius2( double radius )
{
   if( m_radius2 != radius )
   {
      if( m_pMemento )
        m_pMemento->addData( s_pMetaObject, PMRadius2ID, m_radius2 );
      m_radius2 = radius;
      setViewStructureChanged();
   }
}

void PMCone::setOpen( bool op )
{
   if( op != m_open )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOpenID, m_open );
      m_open = op;
   }
}

PMDialogEditBase* PMCone::editWidget( QWidget * parent ) const
{
    return new PMConeEdit( parent );
}

void PMCone::restoreMemento( PMMemento * s )
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
            case PMRadius1ID:
               setRadius1( data->doubleData() );
            break;
            case PMRadius2ID:
               setRadius2( data->doubleData() );
            break;
            case PMOpenID:
               setOpen( data->boolData() );
            break;
            default:
               qCritical(  ) << "Wrong ID in PMCone::restoreMemento\n";
            break;
         }
      }
   }
   Base::restoreMemento( s );
}

bool PMCone::isDefault()
{
   if( ( m_end1 == defaultEnd1 ) && ( m_radius1 == defaultConeRadius1 )
         && ( m_end2 == defaultEnd2 ) && ( m_radius2 == defaultConeRadius2 ) && ( m_open == defaultOpen )
         && globalDetail() )
      return true;
   return false;
}

void PMCone::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure(defaultViewStructure () );
      m_pViewStructure->points().detach();
   }

   int steps = (int)( ( (float)s_numSteps / 2 ) * ( displayDetail() + 1 ) );
   int ptsSize = steps * 2;
   int lineSize = steps * 3;

   if( ptsSize != m_pViewStructure->points().size() )
      m_pViewStructure->points().resize( ptsSize );

   createPoints( m_pViewStructure->points(), m_end1, m_end2, m_radius1, m_radius2, steps );

   if( lineSize != m_pViewStructure->lines().size() )
   {
      m_pViewStructure->lines().detach();
      m_pViewStructure->lines().resize( lineSize );
      createLines( m_pViewStructure->lines(), steps );
   }
}

PMViewStructure* PMCone::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure || s_pDefaultViewStructure->parameterKey() != viewStructureParameterKey() )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = 0;
      int steps = (int)( ( (float)s_numSteps / 2 ) * ( globalDetailLevel() + 1 ) );
      s_pDefaultViewStructure = new PMViewStructure( steps  * 2,  steps * 3 );

      createPoints( s_pDefaultViewStructure->points(), defaultEnd1,
            defaultEnd2,defaultConeRadius1,defaultConeRadius2, steps );

      createLines( s_pDefaultViewStructure->lines(), steps );
   }
   return s_pDefaultViewStructure;
}

void PMCone::createLines( PMLineArray& lines, int steps )
{
   int i;
   for( i = 0; i < ( steps - 1 ); i++ )
   {
         lines[i] = PMLine( i, i + 1 );
         lines[i + steps] = PMLine( i + steps, i +  steps+ 1 );
   }
   lines[steps - 1] = PMLine( steps - 1, 0 );
   lines[steps * 2 - 1] = PMLine( steps * 2 - 1,  steps );

   for( i = 0; i < steps; i++ )
   {
         lines[i + 2 * steps] = PMLine( i, i + steps );
   }
}

void PMCone::createPoints( PMPointArray& points, const PMVector& end1,
                               const PMVector& end2, double radius1, double radius2, int steps )
{

   double angle = ( 2.0 * M_PI ) / (double) steps;

   PMVector pointAt = end2 - end1;
   double pl = pointAt.abs();
   if( approxZero( pl ) )
      pointAt = PMVector( 0.0, 1.0, 0.0 );
   else
      pointAt /= pl;

   PMMatrix rotation = PMMatrix::rotation( pointAt, angle );
   PMVector endPoint1 = pointAt.orthogonal();
   endPoint1 *= radius1;
   PMVector endPoint2 = pointAt.orthogonal();
   endPoint2 *= radius2;

   int i;
   for( i = 0; i < steps; i++ )
   {
      points[i] = PMPoint( endPoint1 + end1 );
      points[i + steps] = PMPoint( endPoint2 + end2 );
      endPoint1 = rotation * endPoint1;
      endPoint2 = rotation * endPoint2;
   }
}

void PMCone::controlPoints( PMControlPointList & list )
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

   PM3DControlPoint* pb1 = new PM3DControlPoint( m_end1, PMEnd1ID, ( "End 1" ) );
   list.append( pb1 );
   PM3DControlPoint* pb2 = new PM3DControlPoint( m_end2, PMEnd2ID, ( "End 2" ) );
   list.append( pb2 );

   list.append( new PMDistanceControlPoint( pb1, angle1, m_radius1, PMRadius1ID, ( "Radius 1 (1)" ) ) );
   list.append( new PMDistanceControlPoint( pb1, angle2, m_radius1, PMRadius1ID, ( "Radius 1 (2)" ) ) );
   list.append( new PMDistanceControlPoint( pb2, angle1, m_radius2, PMRadius2ID, ( "Radius 2 (1)" ) ) );
   list.append( new PMDistanceControlPoint( pb2, angle2, m_radius2, PMRadius2ID, ( "Radius 2 (2)" ) ) );
}

void PMCone::controlPointsChanged( PMControlPointList & list )
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
            case PMRadius1ID:
               setRadius1( ( ( PMDistanceControlPoint *) *it )->distance() );
               radiusChanged = true;
            break;
            case PMRadius2ID:
               setRadius2( ( ( PMDistanceControlPoint *) *it )->distance() );
               radiusChanged = true;
            break;
            default:
               qCritical () << "Wrong ID in PMCone::controlPointsChanged\n";
            break;
         }
      }
   }

   if( pointChanged )
   {
      PMVector center, angle1, angle2;
      bool firstPoint1 = true;
      bool firstPoint2 = true;

      center = m_end1 - m_end2;
      double pl = center.abs();
      if( approxZero( pl )  )
         center = PMVector( 0.0, 1.0, 0.0 );
      else
        center /= pl;

      angle1 = center.orthogonal();
      angle2 = PMVector::cross( center, angle1 );

		for( it = list.begin(); it != list.end(); ++it )
      {
         if( ( *it )->id() == PMRadius1ID )
         {
            if( firstPoint1 )
            {
               ( ( PMDistanceControlPoint *) *it )->setDirection( angle1 );
               firstPoint1 = false;
            }
            else
               ( ( PMDistanceControlPoint *) *it )->setDirection( angle2 );
         }
			else if( ( *it )->id() == PMRadius2ID )
         {
            if( firstPoint2 )
            {
               ( ( PMDistanceControlPoint *) *it )->setDirection( angle1 );
               firstPoint2 = false;
            }
            else
               ( ( PMDistanceControlPoint *) *it )->setDirection( angle2 );
         }
      }
   }

   if( radiusChanged )
		for( it = list.begin(); it != list.end(); ++it )
			if( ( *it )->id() == PMRadius1ID )
				( ( PMDistanceControlPoint *) *it )->setDistance( m_radius1 );
			else if( ( *it )->id() == PMRadius2ID )
				( ( PMDistanceControlPoint *) *it )->setDistance( m_radius2 );
}

void PMCone::setSteps( int s )
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
      qDebug() << "PMCone::setSteps: S must be greater than 4\n";
   s_parameterKey++;
}

void PMCone::cleanUp() const
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
