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


#include "pmdisc.h"

#include "pmxmlhelper.h"
#include "pmboxedit.h"
#include "pmmemento.h"
#include "pmdistancecontrolpoint.h"
#include "pmvectorcontrolpoint.h"
#include "pm3dcontrolpoint.h"
#include "pmdefaults.h"



#include "pmdiscedit.h"

const double defaultDiscRadius = 1.0;
const double defaultDiscHoleRadius = 0.0;
const PMVector defaultDiscCenter = PMVector ( 0.0, 0.0, 0.0 );
const PMVector defaultDiscNormal = PMVector ( 0.0, 1.0, 0.0 );

/** default disc structure */
PMViewStructure* PMDisc::s_pDefaultViewStructure = nullptr;
int PMDisc::s_numSteps = c_defaultDiscSteps;
int PMDisc::s_parameterKey = 0;

PMDefinePropertyClass( PMDisc, PMDiscProperty );

PMMetaObject* PMDisc::s_pMetaObject = nullptr;
PMObject* createNewDisc( PMPart* part )
{
   return new PMDisc( part );
}

PMDisc::PMDisc( PMPart* part )
      : Base( part )
{
   m_center = defaultDiscCenter;
   m_normal = defaultDiscNormal;
   m_radius = defaultDiscRadius;
   m_hradius = defaultDiscHoleRadius;
}

PMDisc::PMDisc( const PMDisc& d )
      : Base( d )
{
   m_center = d.m_center;
   m_normal = d.m_normal;
   m_radius = d.m_radius;
   m_hradius = d.m_hradius;
}


PMDisc::~PMDisc()
{
}

QString PMDisc::description() const
{
   return ( "disc" );
}

void PMDisc::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "center", m_center.serializeXML() );
   e.setAttribute( "normal", m_normal.serializeXML() );
   e.setAttribute( "radius", m_radius );
   e.setAttribute( "hole_radius", m_hradius );
   Base::serialize( e, doc );
}

void PMDisc::readAttributes( const PMXMLHelper& h )
{
   m_center = h.vectorAttribute( "center", defaultDiscCenter );
   m_normal = h.vectorAttribute( "normal", defaultDiscNormal );
   m_radius = h.doubleAttribute( "radius", defaultDiscRadius );
   m_hradius = h.doubleAttribute( "hole_radius", defaultDiscHoleRadius );
   Base::readAttributes( h );
}

PMMetaObject* PMDisc::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Disc", Base::metaObject(),
                                        createNewDisc );
      s_pMetaObject->addProperty(
         new PMDiscProperty( "center", &PMDisc::setCenter, &PMDisc::center ) );
      s_pMetaObject->addProperty(
         new PMDiscProperty( "normal", &PMDisc::setNormal, &PMDisc::normal ) );
      s_pMetaObject->addProperty(
         new PMDiscProperty( "radius", &PMDisc::setRadius, &PMDisc::radius ) );
      s_pMetaObject->addProperty(
         new PMDiscProperty( "holeRadius", &PMDisc::setHoleRadius, &PMDisc::holeRadius ) );
   }
   return s_pMetaObject;
}

void PMDisc::setCenter( const PMVector& c )
{
   if( c != m_center )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCenterID, m_center );

      m_center = c;
      m_center.resize( 3 );

      setViewStructureChanged();
   }
}

void PMDisc::setNormal( const PMVector& p )
{
   if( p != m_normal )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNormalID, m_normal );

      m_normal = p;
      m_normal.resize( 3 );

      setViewStructureChanged();
   }
}

void PMDisc::setRadius( double radius )
{
   if( m_radius != radius )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRadiusID, m_radius );

      if( radius <= m_hradius )
         m_radius = m_hradius;
      else
        m_radius = radius;

      setViewStructureChanged();
   }
}

void PMDisc::setHoleRadius( double hradius )
{
   if( m_hradius != hradius )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHRadiusID, m_hradius );

      if( hradius >= m_radius )
         m_hradius = m_radius;
      else if( hradius <= 0.0 )
         m_hradius = 0.0;
      else
         m_hradius = hradius;

      setViewStructureChanged();
   }
}

PMDialogEditBase* PMDisc::editWidget( QWidget* parent ) const
{
   return new PMDiscEdit( parent );
}

void PMDisc::restoreMemento( PMMemento* s )
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
            case PMCenterID:
               setCenter( data->vectorData() );
               break;
            case PMNormalID:
               setNormal( data->vectorData() );
               break;
            case PMRadiusID:
               setRadius( data->doubleData() );
               break;
            case PMHRadiusID:
               setHoleRadius( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMDisc::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

bool PMDisc::isDefault()
{
   if( ( m_center == defaultDiscCenter ) && ( m_normal == defaultDiscNormal )
       && ( m_radius == defaultDiscRadius ) && ( m_hradius == defaultDiscHoleRadius )
       && globalDetail() )
      return true;
   return false;
}

void PMDisc::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure () );
      m_pViewStructure->points().detach();
   }

   int steps = (int)( ( (float)s_numSteps / 2 ) * ( displayDetail() + 1 ) );
   int size = steps * 2;

   if( size != m_pViewStructure->points().size() )
   {
      m_pViewStructure->points().resize( size );
      m_pViewStructure->lines().detach();
      m_pViewStructure->lines().resize( size );
      createLines( m_pViewStructure->lines(), steps );
   }
   createPoints( m_pViewStructure->points(), m_center, m_normal, m_radius, m_hradius, steps );
}

PMViewStructure* PMDisc::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure || s_pDefaultViewStructure->parameterKey() != viewStructureParameterKey() )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = nullptr;
      int steps = (int)( ( (float)s_numSteps / 2 ) * ( globalDetailLevel() + 1 ) );
      s_pDefaultViewStructure = new PMViewStructure( steps * 2 , steps * 2 );

      createPoints( s_pDefaultViewStructure->points(), defaultDiscCenter, defaultDiscNormal,
                    defaultDiscRadius, defaultDiscHoleRadius, steps );

      createLines( s_pDefaultViewStructure->lines(), steps );
   }
   return s_pDefaultViewStructure;
}

void PMDisc::createLines( PMLineArray& lines, int steps )
{
   int i;
   for( i = 0; i < ( steps - 1)  ; i++ )
   {
      lines[ i ] = PMLine( i, i + 1 );
      lines[ i + steps ] = PMLine( i + steps, i + steps + 1 );
   }
   lines[ i ] = PMLine( i , 0 );
   lines[i + steps] = PMLine( i + steps, steps );
}

void PMDisc::createPoints( PMPointArray& points, const PMVector& center,
   const PMVector& normal, double radius , double hradius, int steps )
{
   PMVector pv = normal;
   double pl = pv.abs();
   if( approxZero( pl ) )
      pv = PMVector( 0.0 , 1.0 , 0.0 );
   else
      pv /= pl;

   double angle = ( 2.0 * M_PI ) / ( double ) steps;
   PMMatrix rotation = PMMatrix::rotation( pv , angle );
   PMVector endP = pv.orthogonal();

   for( int i = 0 ; i < steps ; i++ )
   {
      points[ i ] = PMPoint( ( endP * radius ) + center );
      points[ i + steps ] = PMPoint( ( endP * hradius ) + center );
      endP = rotation * endP;
   }
}

void setVectorBase( PMVector pn , PMVector& vn , PMVector& v1, PMVector& v2 )
{
   double pl = pn.abs();
   if( approxZero( pl ) )
      vn = PMVector( 0.0 , 1.0 , 0.0 );
   else
      vn = ( pn / pl  );

   v1 = vn.orthogonal();
   v2 = PMVector::cross( vn, v1 );
}

void PMDisc::controlPoints( PMControlPointList & list )
{
   PMVector vetN , vet1 , vet2 ;
   setVectorBase( m_normal , vetN , vet1 , vet2 );

   PM3DControlPoint* pb = new PM3DControlPoint( m_center, PMCenterID, ( "Center" ) );
   list.append( pb );

   PMDistanceControlPoint* d;
   d = new PMDistanceControlPoint( pb , vet1, m_radius, PMRadiusID, ( "Radius (1)" ) );
   list.append( d );
   d = new PMDistanceControlPoint( pb , vet2, m_radius, PMRadiusID, ( "Radius (2)" ) );
   list.append( d );
   d = new PMDistanceControlPoint( pb , vet1, m_hradius, PMHRadiusID, ( "Hole Radius (1)" ) );
   list.append( d );
   d = new PMDistanceControlPoint( pb , vet2, m_hradius, PMHRadiusID, ( "Hole Radius (2)" ) );
   list.append( d );

   list.append( new PMVectorControlPoint( pb, m_normal, PMNormalID, ( "Normal" ) ) );
}

void PMDisc::controlPointsChanged( PMControlPointList & list )
{
   bool radius1Changed = false;
   bool radius2Changed = false;
   bool pointChanged = false;
   PMVector pt;
	PMControlPointList::iterator it;

   for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMCenterID:
               pt =( (PM3DControlPoint* ) *it )->point() ;
               setCenter( pt );
               pointChanged = true;
               break;
            case PMNormalID:
               setNormal( ( ( (PM3DControlPoint* ) *it )->point() )  );
               pointChanged = true;
               break;
            case PMRadiusID:
               setRadius( ( ( PMDistanceControlPoint *) *it )->distance() );
               radius1Changed = true;
               break;
            case PMHRadiusID:
               setHoleRadius( ( ( PMDistanceControlPoint *) *it )->distance() );
               radius2Changed = true;
               break;
            default:
               qCritical(  ) << "Wrong ID in PMDisc::controlPointsChanged\n";
               break;
         }
      }
   }

   if( radius1Changed )
		for( it = list.begin(); it != list.end(); ++it )
         if( ( *it )->id() == PMRadiusID )
            ( ( PMDistanceControlPoint *) *it )->setDistance( m_radius );

   if( radius2Changed )
		for( it = list.begin(); it != list.end(); ++it )
         if( ( *it )->id() == PMHRadiusID )
            ( ( PMDistanceControlPoint *) *it )->setDistance( m_hradius );

   if( pointChanged )
   {
      PMVector vetN, vet1, vet2;
      setVectorBase( m_normal , vetN, vet1, vet2 );

      bool firstPoint1 = true;
      bool firstPoint2 = true;

		for( it = list.begin(); it != list.end(); ++it )
      {
         if( ( *it )->id() == PMRadiusID )
         {
            if( firstPoint1 )
            {
               ( ( PMDistanceControlPoint *) *it )->setDirection( vet1 );
               firstPoint1 = false;
            }
            else
               ( ( PMDistanceControlPoint *) *it )->setDirection( vet2 );
         }
         if( ( *it )->id() == PMHRadiusID )
         {
            if( firstPoint2 )
            {
               ( ( PMDistanceControlPoint *) *it )->setDirection( vet1 );
               firstPoint2 = false;
            }
            else
               ( ( PMDistanceControlPoint *) *it )->setDirection( vet2 );
         }
      }
   }
}

void PMDisc::setSteps( int s )
{
   if( s >= 4 )
   {
      s_numSteps = s;
      if( s_pDefaultViewStructure )
      {
         delete s_pDefaultViewStructure;
         s_pDefaultViewStructure = nullptr;
      }
   }
   else
      qDebug() << "PMDisc::setSteps: S must be greater than 3\n";
   s_parameterKey++;
}

void PMDisc::cleanUp() const
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
