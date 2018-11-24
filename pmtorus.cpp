/***************************************************************************
                          pmtorus.cpp  -  description
                             -------------------
    copyright            : (C) 2001 Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
    copyright            : (C) 2002 Andreas Zehender
    email                : zehender@kde.org
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pmtorus.h"

#include "pmxmlhelper.h"
#include "pmtorusedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"
#include "pmdistancecontrolpoint.h"
#include "pmdefaults.h"



/** default param for the Torus */
const double c_defaultminorRadius = 0.25;
const double c_defaultmajorRadius = 0.5;
const bool c_defaultsturm = false;

/** default Torus structure */
PMViewStructure* PMTorus::s_pDefaultViewStructure = 0;

int PMTorus::s_vStep = c_defaultTorusVSteps;
int PMTorus::s_uStep = c_defaultTorusUSteps;
int PMTorus::s_parameterKey = 0;

PMDefinePropertyClass( PMTorus, PMTorusProperty );

PMMetaObject* PMTorus::s_pMetaObject = 0;
PMObject* createNewTorus( PMPart* part )
{
   return new PMTorus( part );
}

PMTorus::PMTorus( PMPart* part )
      : Base( part )
{
   m_minorRadius = c_defaultminorRadius;
   m_majorRadius = c_defaultmajorRadius;
   m_sturm = c_defaultsturm ;
}

PMTorus::PMTorus( const PMTorus& t )
      : Base( t )
{
   m_minorRadius = t.m_minorRadius;
   m_majorRadius = t.m_majorRadius;
   m_sturm = t.m_sturm;
}

PMTorus::~PMTorus()
{
}


QString PMTorus::description() const
{
   return ( "torus" );
}

PMMetaObject* PMTorus::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Torus", Base::metaObject(),
                                        createNewTorus );
      s_pMetaObject->addProperty(
         new PMTorusProperty( "minorRadius", &PMTorus::setMinorRadius,
                         &PMTorus::minorRadius ) );
      s_pMetaObject->addProperty(
         new PMTorusProperty( "majorRadius", &PMTorus::setMajorRadius,
                         &PMTorus::majorRadius ) );
      s_pMetaObject->addProperty(
         new PMTorusProperty( "sturm", &PMTorus::setSturm, &PMTorus::sturm ) );
   }
   return s_pMetaObject;
}

void PMTorus::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "minor_radius", m_minorRadius );
   e.setAttribute( "major_radius", m_majorRadius );
   e.setAttribute( "sturm", m_sturm );

   Base::serialize( e, doc );
}

void PMTorus::readAttributes( const PMXMLHelper& h )
{
   m_minorRadius = h.doubleAttribute( "minor_radius", c_defaultminorRadius );
   m_majorRadius = h.doubleAttribute( "major_radius", c_defaultmajorRadius );
   m_sturm = h.boolAttribute( "sturm", c_defaultsturm );

   Base::readAttributes( h );
}

PMDialogEditBase* PMTorus::editWidget( QWidget* parent ) const
{

 return new PMTorusEdit( parent );
}

void PMTorus::restoreMemento( PMMemento* s )
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
            case PMMinorRadiusID:
               setMinorRadius( data->doubleData() );
               break;
            case PMMajorRadiusID:
               setMajorRadius( data->doubleData() );
               break;
            case PMSturmID:
               setSturm( data->boolData() );
            default:
               qCritical(  ) << "Wrong ID in PMTorus::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );

}

void PMTorus::controlPoints( PMControlPointList& list )
{
   PMVector majorCenter( 0, 0, 0 );
   /** control points of the major radius */
   list.append( new PMDistanceControlPoint( majorCenter, PMVector( 1.0, 0.0, 0.0 ),
                                            m_majorRadius, PMMajorRadiusID,
                                            ( "Major radius (x)" ) ) );
   PMDistanceControlPoint* rcp =
      new PMDistanceControlPoint( majorCenter, PMVector( 0.0, 0.0, 1.0 ),
                                  m_majorRadius, PMMajorRadiusID,
                                  ( "Major radius (z)" ) );
   list.append( rcp );

   PMVector minorCenter( 0.0, 0.0, m_majorRadius );
   list.append( new PMDistanceControlPoint( rcp, PMVector( 0.0, 1.0, 0.0 ),
                                            m_minorRadius, PMMinorRadiusID,
                                            ( "Minor radius (y)" ) ) );
   list.append( new PMDistanceControlPoint( rcp, PMVector( 0.0, 0.0, 1.0 ),
                                            m_minorRadius, PMMinorRadiusID,
                                            ( "Minor radius (z)" ) ) );
}

void PMTorus::controlPointsChanged( PMControlPointList& list )
{
   bool majorChanged = false, minorChanged = false;
	PMControlPointList::iterator it;

	for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMMinorRadiusID:
               setMinorRadius( ( ( PMDistanceControlPoint* ) *it )->distance() );
               ( ( PMDistanceControlPoint* ) *it )->setDistance( m_minorRadius );
               minorChanged = true;
               break;
            case PMMajorRadiusID:
               setMajorRadius( ( ( PMDistanceControlPoint* ) *it )->distance() );
               ( ( PMDistanceControlPoint* ) *it )->setDistance( m_majorRadius );
               majorChanged = true;
               break;
            default:
               qCritical(  ) << "Wrong ID in PMTorus::controlPointsChanged\n";
               break;
         }
      }
   }

   if( majorChanged )
		for( it = list.begin(); it != list.end(); ++it )
         if( ( *it )->id() == PMMajorRadiusID )
            ( ( PMDistanceControlPoint* ) *it )->setDistance( m_majorRadius );
   if( minorChanged )
		for( it = list.begin(); it != list.end(); ++it )
         if( ( *it )->id() == PMMinorRadiusID )
            ( ( PMDistanceControlPoint* ) *it )->setDistance( m_minorRadius );
}

bool PMTorus::isDefault()
{
  if( ( m_minorRadius == c_defaultminorRadius )
      && ( m_majorRadius == c_defaultmajorRadius )
      && globalDetail() )
      return true;
   return false;
}

void PMTorus::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
   }

   int uStep = (int)( ( (float)s_uStep / 2 ) * ( displayDetail() + 1 ) );
   int vStep = (int)( ( (float)s_vStep / 2 ) * ( displayDetail() + 1 ) );
   int ptsSize = vStep * uStep;
   int lineSize = vStep * uStep * 2;

   if( ptsSize != m_pViewStructure->points().size() )
      m_pViewStructure->points().resize( ptsSize );

   createPoints( m_pViewStructure->points(), m_minorRadius, m_majorRadius, uStep, vStep );

   if( lineSize != m_pViewStructure->lines().size() )
   {
      m_pViewStructure->lines().detach();
      m_pViewStructure->lines().resize( lineSize );
      createLines( m_pViewStructure->lines(), uStep, vStep );
   }
}

PMViewStructure* PMTorus::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure || s_pDefaultViewStructure->parameterKey() != viewStructureParameterKey() )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = 0;
      int uStep = (int)( ( (float)s_uStep / 2 ) * ( globalDetailLevel() + 1 ) );
      int vStep = (int)( ( (float)s_vStep / 2 ) * ( globalDetailLevel() + 1 ) );

      s_pDefaultViewStructure =
         new PMViewStructure(  vStep * uStep ,
                               vStep * uStep * 2 );

      createPoints( s_pDefaultViewStructure->points(), c_defaultminorRadius,
                    c_defaultmajorRadius, uStep, vStep );

      createLines( s_pDefaultViewStructure->lines(), uStep, vStep );
   }
   return s_pDefaultViewStructure;
}

void PMTorus::createLines( PMLineArray& lines, int uStep, int vStep )
{
   int u, v;
   for( u = 0; u < uStep; ++u )
   {
      for( v = 0; v < vStep; ++v )
      {
         lines[ u * vStep + v ] = PMLine( u * vStep  + v, u * vStep + ( (v+1) %  vStep ) );
         lines[ uStep * vStep + u * vStep + v ] = PMLine( u * vStep  + v, ( (u+1) % uStep ) * vStep + v );
      }
   }
}

void PMTorus::createPoints( PMPointArray& points, double minor_radius,
                            double  major_radius, int uStep, int vStep )
{
   double l_UradStep = ( 2.0 * M_PI ) / uStep;
   double l_VradStep = ( 2.0 * M_PI ) / vStep;
   double l_u = l_UradStep;
   int u, v;

   for( u = 0; u < uStep; ++u )
   {
      double l_v = 0.0;
      double y = minor_radius * sin ( l_u );
      double l_rcosu = major_radius + minor_radius  * cos( l_u );

      for( v = 0; v < vStep; ++v )
      {
         double x = l_rcosu * cos( l_v );
         double z = l_rcosu * sin( l_v );
         points[u * vStep + v ] = PMPoint( x, y, z );
         l_v = l_v + l_VradStep;
      }
      l_u = l_u + l_UradStep;
   }
}

void PMTorus::setMinorRadius( double minor_radius )
{
   if( m_minorRadius != minor_radius )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMinorRadiusID, m_minorRadius );
      m_minorRadius =  minor_radius;
      setViewStructureChanged();
   }
}

void PMTorus::setMajorRadius( double major_radius )
{
   if( m_majorRadius != major_radius )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMajorRadiusID, m_majorRadius );
      m_majorRadius =  major_radius;
      setViewStructureChanged();
   }
}

void PMTorus::setSturm( bool sturm )
{
   if( m_sturm != sturm )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSturmID, m_sturm );
      m_sturm =  sturm;
      setViewStructureChanged();
   }

}

void PMTorus::setUSteps( int u )
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
      qDebug() << "PMTorus::setUSteps: U must be greater than 1\n";
   s_parameterKey++;
}

void PMTorus::setVSteps( int v )
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
      qDebug() << "PMTorus::setVSteps: V must be greater than 3\n";
   s_parameterKey++;
}

void PMTorus::cleanUp() const
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
