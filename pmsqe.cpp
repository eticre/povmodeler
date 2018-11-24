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


#include "pmsqe.h"

#include "pmxmlhelper.h"
#include "pmsqeedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pmdefaults.h"
#include "pmmath.h"



const double c_defaultEastWestExponent = 1.0;
const double c_defaultNorthSouthExponent = 1.0;

PMViewStructure* PMSuperquadricEllipsoid::s_pDefaultViewStructure = 0;
int PMSuperquadricEllipsoid::s_vStep = c_defaultSuperquadricEllipsoidVSteps;
int PMSuperquadricEllipsoid::s_uStep = c_defaultSuperquadricEllipsoidUSteps;
int PMSuperquadricEllipsoid::s_parameterKey = 0;

PMDefinePropertyClass( PMSuperquadricEllipsoid, PMSuperquadricEllipsoidProperty );

PMMetaObject* PMSuperquadricEllipsoid::s_pMetaObject = 0;
PMObject* createNewSuperquadricEllipsoid( PMPart* part )
{
   return new PMSuperquadricEllipsoid( part );
}

PMSuperquadricEllipsoid::PMSuperquadricEllipsoid( PMPart* part )
      : Base( part )
{
   m_eastWestExponent = c_defaultEastWestExponent;
   m_northSouthExponent = c_defaultNorthSouthExponent;
}

PMSuperquadricEllipsoid::PMSuperquadricEllipsoid( const PMSuperquadricEllipsoid& s )
      : Base( s )
{
   m_eastWestExponent = s.m_eastWestExponent;
   m_northSouthExponent = s.m_northSouthExponent;
}

PMSuperquadricEllipsoid::~PMSuperquadricEllipsoid()
{
}

QString PMSuperquadricEllipsoid::description() const
{
   return ( "superquadric ellipsoid" );
}

void PMSuperquadricEllipsoid::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "value_e", m_eastWestExponent );
   e.setAttribute( "value_n", m_northSouthExponent );
   Base::serialize( e, doc );
}

void PMSuperquadricEllipsoid::readAttributes( const PMXMLHelper& h )
{
   m_eastWestExponent = h.doubleAttribute( "value_e", c_defaultEastWestExponent );
   m_northSouthExponent = h.doubleAttribute( "value_n", c_defaultNorthSouthExponent );
   Base::readAttributes( h );
}

PMMetaObject* PMSuperquadricEllipsoid::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "SuperquadricEllipsoid", Base::metaObject(),
                                        createNewSuperquadricEllipsoid );
      s_pMetaObject->addProperty(
         new PMSuperquadricEllipsoidProperty( "eastWestExponent",
                         &PMSuperquadricEllipsoid::setEastWestExponent,
                         &PMSuperquadricEllipsoid::eastWestExponent ) );
      s_pMetaObject->addProperty(
         new PMSuperquadricEllipsoidProperty( "northSouthExponent",
                         &PMSuperquadricEllipsoid::setNorthSouthExponent,
                         &PMSuperquadricEllipsoid::northSouthExponent ) );
   }
   return s_pMetaObject;
}

void PMSuperquadricEllipsoid::setEastWestExponent( double e )
{
   if( e != m_eastWestExponent )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEastWestExponentID,
                              m_eastWestExponent );
      if( e < 0.001 )
      {
         qCritical(  ) << "EastWestExponent < 0.001 in PMSuperquadricEllipsoid::setEastWestExponent\n";
         e = 0.001;
      }
      m_eastWestExponent = e;
      setViewStructureChanged();
   }
}

void PMSuperquadricEllipsoid::setNorthSouthExponent( double n )
{
   if( n != m_northSouthExponent )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNorthSouthExponentID,
                              m_northSouthExponent );
      if( n < 0.001 )
      {
         qCritical(  ) << "NorthSouthExponent < 0.001 in PMSuperquadricEllipsoid::setNorthSouthExponent\n";
         n = 0.001;
      }

      m_northSouthExponent = n;
      setViewStructureChanged();
   }
}

PMDialogEditBase* PMSuperquadricEllipsoid::editWidget( QWidget* parent ) const
{
   return new PMSuperquadricEllipsoidEdit( parent );
}

void PMSuperquadricEllipsoid::restoreMemento( PMMemento* s )
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
            case PMEastWestExponentID:
               setEastWestExponent( data->doubleData() );
               break;
            case PMNorthSouthExponentID:
               setNorthSouthExponent( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMSuperquadricEllipsoid::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}


bool PMSuperquadricEllipsoid::isDefault()
{
   if( ( m_eastWestExponent == c_defaultEastWestExponent ) &&
       ( m_northSouthExponent == c_defaultNorthSouthExponent )
         && globalDetail() )
      return true;
   return false;
}

void PMSuperquadricEllipsoid::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
   }

   int uStep = (int)( ( (float)s_uStep / 2 ) * ( displayDetail() + 1 ) );
   int vStep = (int)( ( (float)s_vStep / 2 ) * ( displayDetail() + 1 ) );
   int uStep2 = uStep * 4;
   int vStep2 = vStep * 8;
   int ptsSize = vStep2 * ( uStep2 - 1 ) + 2;
   int lineSize = vStep2 * ( uStep2 - 1 ) * 2 + vStep2;

   if( ptsSize != m_pViewStructure->points().size() )
      m_pViewStructure->points().resize( ptsSize );

   createPoints( m_pViewStructure->points(), m_eastWestExponent,
                 m_northSouthExponent, uStep, vStep );

   if( lineSize != m_pViewStructure->lines().size() )
   {
      m_pViewStructure->lines().detach();
      m_pViewStructure->lines().resize( lineSize );
      createLines( m_pViewStructure->lines(), uStep2, vStep2 );
   }
}

PMViewStructure* PMSuperquadricEllipsoid::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure || s_pDefaultViewStructure->parameterKey() != viewStructureParameterKey() )
   {
      delete s_pDefaultViewStructure;
      s_pDefaultViewStructure = 0;
      int uStep = (int)( ( (float)s_uStep / 2 ) * ( globalDetailLevel() + 1 ) );
      int vStep = (int)( ( (float)s_vStep / 2 ) * ( globalDetailLevel() + 1 ) );

      // transform u and v steps to sphere u/v steps
      int uStep2 = uStep * 4;
      int vStep2 = vStep * 8;

      s_pDefaultViewStructure =
         new PMViewStructure( vStep2 * ( uStep2 - 1 ) + 2,
                              vStep2 * ( uStep2 - 1 ) * 2 + vStep2 );

      // points
      createPoints( s_pDefaultViewStructure->points(),
                    c_defaultEastWestExponent, c_defaultNorthSouthExponent, uStep, vStep );

      createLines( s_pDefaultViewStructure->lines(), uStep2, vStep2 );
   }
   return s_pDefaultViewStructure;
}

void PMSuperquadricEllipsoid::createLines( PMLineArray& lines, int uStep, int vStep )
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
   // offset += vStep;
}

void PMSuperquadricEllipsoid::createPoints( PMPointArray& points,
                                            double e, double n, int uStep, int vStep )
{
   int u, v;
   int zi;
   int pbase = 0, pref = 0;

   if( e <= 0.001 )
      e = 0.001;
   if( n <= 0.001 )
      n = 0.001;

   double c2_e = 2.0 / e;
   double c2_n = 2.0 / n;
   double cn_2 = n / 2.0;
   double ce_2 = e / 2.0;
   double cn_e = n / e;
//   double ce_n = e / n;
   double z = 0.0, c = 0.0, a = 0.0, a2 = 0.0, x = 0.0, y = 0.0;
   double k = 0.0, k2 = 0.0, du = 0.0, dv = 0.0;
   PMPoint p;

   points[0] = PMPoint( 0, 0, 1 );
   pbase++;

   for( zi = 0; zi < 2; zi++ )
   {
      for( u = 0; u < uStep; u++ )
      {
         du = ( double ) ( u + 1 ) / ( double ) uStep;
         if( zi == 1 )
            du = 1.0 - du;
         k = tan( M_PI / 4.0 * pow( du, n < 1.0 ? n : sqrt( n ) ) );
         k2 = 1 / ( pow( k, c2_n ) + 1 );
         z = pow( k2, cn_2 );
         if( zi == 1 )
            z *= k;
         c = pow( 1 - pow( z, c2_n ), cn_e );

         for( v = 0; v < ( vStep + 1 ); v++ )
         {
            dv = ( double ) v / ( double ) vStep;
            a = tan( M_PI / 4.0 * pow( dv, e < 1.0 ? e : sqrt( e ) ) );
            a2 = 1 + pow( a, c2_e );
            x = pow( c / a2, ce_2 );
            y = x * a;

            points[pbase+v] = PMPoint( x, y, z );
         }
         // 1/8

         pref = pbase + 2 * vStep;
         for( v = 0; v < vStep; v++, pref-- )
         {
            p = points[pbase+v];
            x = p[0];
            p[0] = p[1];
            p[1] = x;
            points[pref] = p;
         }
         // 1/4

         pref = pbase + 4 * vStep;
         for( v = 0; v < ( 2 * vStep ); v++, pref-- )
         {
            p = points[pbase+v];
            p[0] = -p[0];
            points[pref] = p;
         }
         // 1/2

         pref = pbase + 8 * vStep - 1;
         for( v = 1; v < ( 4 * vStep ); v++, pref-- )
         {
            p = points[pbase+v];
            p[1] = -p[1];
            points[pref] = p;
         }

         pbase += 8 * vStep;
      }
   }

   for( u = 0; u < ( uStep * 2 - 1 ); u++ )
   {
      pbase = 1 + u * vStep * 8;
      pref = 1 + ( uStep * 4 - 2 - u ) * vStep * 8;

      for( v = 0; v < ( vStep * 8 ); v++, pref++ )
      {
         p = points[pbase + v];
         p[2] = -p[2];
         points[pref] = p;
      }
   }
   points[ vStep * 8 * ( uStep * 4 - 1 ) + 1 ] = PMPoint( 0, 0, -1 );
}

void PMSuperquadricEllipsoid::setUSteps( int u )
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
      qDebug() << "PMSuperquadricEllipsoid::setUSteps: U must be greater than 1\n";
   s_parameterKey++;
}

void PMSuperquadricEllipsoid::setVSteps( int v )
{
   if( v >= 2 )
   {
      s_vStep = v;
      if( s_pDefaultViewStructure )
      {
         delete s_pDefaultViewStructure;
         s_pDefaultViewStructure = 0;
      }
   }
   else
      qDebug() << "PMSuperquadricEllipsoid::setVSteps: V must be greater than 1\n";
   s_parameterKey++;
}

void PMSuperquadricEllipsoid::cleanUp() const
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
