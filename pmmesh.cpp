/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003-2005 by Leon Pennington
    email                : leon@leonscape.co.uk
    copyright            : (C) 2006 by Andreas Zehender
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


#include "pmmesh.h"


#include "pmxmlhelper.h"
#include "pmmeshedit.h"
#include "pmmemento.h"
#include "pmtriangle.h"
#include "pm3dcontrolpoint.h"
#include "pmvectorcontrolpoint.h"
#include <QList>
#include <QVector>

const PMVector insideVectorDefault = PMVector( 0.0, 0.0, 0.0 );

PMDefinePropertyClass( PMMesh, PMMeshProperty );

class PMMeshMemento : public PMMemento
{
public:
   /**
    * Creates a memento for the object originator
    */
   PMMeshMemento( PMObject* originator ) : PMMemento( originator )
   {
      m_bTriangleMementosSaved = false;
   }
   /**
    * Deletes the memento
    */
   virtual ~PMMeshMemento()
   {
      while( !m_triangleMementos.isEmpty() )
         delete m_triangleMementos.takeFirst();
   }

   /**
    * Saves the triangles memento data
    */
   void setTriangleMementos( const QList< PMMemento* >& list )
   {
      if ( !m_bTriangleMementosSaved )
      {
	 foreach( PMMemento* m, list )
            m_triangleMementos.append( m );
         m_bTriangleMementosSaved = true;
         addChange( PMCData );
      }
   }
   /**
    * Returns the triangles memento data
    */
   QList< PMMemento* > triangleMementos() const
   {
      if ( !m_bTriangleMementosSaved )
         qCritical(  ) << "Triangles mementos not saved in PMMeshMemento::triangleMementos\n";
      return m_triangleMementos;
   }
   /**
    * Returns true if the triangle mementos have been saved
    */
   bool triangleMementosSaved() const { return m_bTriangleMementosSaved; }
private:
   QList< PMMemento* > m_triangleMementos;
   bool m_bTriangleMementosSaved;
};

PMMetaObject* PMMesh::s_pMetaObject = 0;
PMObject* createNewMesh( PMPart* part )
{
   return new PMMesh( part );
}

PMMesh::PMMesh( PMPart* part )
      : Base( part )
{
   m_hierarchy = true;
   m_enableInsideVector = false;
   m_insideVector = insideVectorDefault;
}

PMMesh::PMMesh( const PMMesh& m )
      : Base( m )
{
   m_hierarchy = m.m_hierarchy;
   m_enableInsideVector = m.m_enableInsideVector;
   m_insideVector = m.m_insideVector;
}

PMMesh::~PMMesh()
{
}

QString PMMesh::description() const
{
   return ( "mesh" );
}

void PMMesh::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "hierarchy", m_hierarchy );
   e.setAttribute( "enable_inside_vector", m_enableInsideVector );
   e.setAttribute( "inside_vector", m_insideVector.serializeXML() );
   Base::serialize( e, doc );
}

void PMMesh::readAttributes( const PMXMLHelper& h )
{
   m_hierarchy = h.boolAttribute( "hierarchy", true );
   m_enableInsideVector = h.boolAttribute( "enable_inside_vector", false );
   m_insideVector = h.vectorAttribute( "inside_vector", insideVectorDefault );
   Base::readAttributes( h );
}

PMMetaObject* PMMesh::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Mesh", Base::metaObject(), createNewMesh );

      s_pMetaObject->addProperty(
         new PMMeshProperty( "hierarchy", &PMMesh::setHierarchy, &PMMesh::hierarchy ) );
      s_pMetaObject->addProperty(
         new PMMeshProperty( "insideVectorEnabled", &PMMesh::enableInsideVector, &PMMesh::isInsideVectorEnabled ) );
      s_pMetaObject->addProperty(
         new PMMeshProperty( "insideVector", &PMMesh::setInsideVector, &PMMesh::insideVector ) );
   }
   return s_pMetaObject;
}

void PMMesh::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMMesh::setHierarchy( bool h )
{
   if( h != m_hierarchy )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHierarchyID, m_hierarchy );
      m_hierarchy = h;
   }
}

void PMMesh::enableInsideVector( bool eiv )
{
   if( eiv != m_enableInsideVector )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableInsideVectorID, m_enableInsideVector );
      m_enableInsideVector = eiv;
   }
}

void PMMesh::setInsideVector( const PMVector& iv )
{
   if( iv != m_insideVector )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMInsideVectorID, m_insideVector );
      m_insideVector = iv;
   }
}

PMDialogEditBase* PMMesh::editWidget( QWidget* parent ) const
{
   return new PMMeshEdit( parent );
}

void PMMesh::createMemento()
{
   delete m_pMemento;
   m_pMemento = new PMMeshMemento( this );
}

void PMMesh::restoreMemento( PMMemento* s )
{
   PMMeshMemento* m = ( PMMeshMemento * ) s;
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
            case PMHierarchyID:
               setHierarchy( data->boolData() );
               break;
            case PMEnableInsideVectorID:
               enableInsideVector( data->boolData() );
               break;
            case PMInsideVectorID:
               setInsideVector( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMMesh::restoreMemento\n";
               break;
         }
      }
   }

   if ( m->triangleMementosSaved() )
   {
      int i = 0;
      foreach( PMMemento* tm, m->triangleMementos() )
      {
         childAt( i )->restoreMemento( tm );
	 i++;
      }
   }

   Base::restoreMemento( s );
}

void PMMesh::controlPoints( PMControlPointList& list )
{
   unsigned numChildren = countChildren();
   PMTriangle *obj;
   pointToPoint ptp;
   bool found;
   PMVector point, normal;
   PM3DControlPoint* cp;
   PMVectorControlPoint* vp;
   int currentPoint = 0;
   int firstNormal = numChildren * 3;
   int currentNormal = firstNormal;
	PMControlPointList::iterator it;

   m_pointToPointList.clear();
   for ( unsigned i = 0; i < numChildren; ++i )
   {
      if ( childAt( i )->isA( "Triangle" ) )
      {

         obj = ( PMTriangle * ) childAt( i );
         ptp.object = obj;
         for ( unsigned j = 0; j < 3; ++j )
         {
            found = false;
            ptp.pointID = j;
            point = obj->point( j );

				for( it = list.begin(); it != list.end(); ++it )
            {
               if ( ( *it )->id() < firstNormal && point == ( *it )->position() )
               {
                  found = true;
                  ptp.listID = ( *it )->id();
                  break;
               }
            }

            if ( !found )
            {
               cp = new PM3DControlPoint( point, currentPoint,
                     ( "Mesh Point " + currentPoint ) );
               list.append( cp );
               ptp.listID = currentPoint++;
            }

            m_pointToPointList.append( ptp );

            if ( obj->isSmoothTriangle() )
            {
               found = false;
               ptp.pointID = j + 3;
               normal = obj->normal( j );

					for( it = list.begin(); it != list.end(); ++it )
               {
                  if ( ( *it )->id() >= firstNormal )
                  {
                     vp = ( PMVectorControlPoint* ) ( *it );
                     if ( vp->basePoint() == point &&
                          vp->vector() == normal )
                     {
                        found = true;
                        ptp.listID = ( *it )->id();
                        break;
                     }
                  }
               }

               if ( !found )
               {
                  vp = new PMVectorControlPoint( point, normal, currentNormal,
                        ( "Mesh Normal " + currentNormal ) );
                  list.append( vp );
                  ptp.listID = currentNormal++;
               }

               m_pointToPointList.append( ptp );
            }

         }
      }
   }
}

void PMMesh::controlPointsChangedList( PMControlPointList& list, PMObjectList& objList )
{
   int numChildren = countChildren();
   PMControlPointList::iterator it = list.begin();
   QList<pointToPoint>::ConstIterator ptpItr = m_pointToPointList.constBegin();
   QList< PMMemento* > mementoList;
   PMTriangle *obj;
   PMVector p0, p1, p2;
   PM3DControlPoint* cp0, * cp1, * cp2;
   bool bp0, bp1, bp2;
   PMVector n0, n1, n2;
   PMVectorControlPoint* cn0, * cn1, * cn2;
   bool bn0, bn1, bn2;
   PMVector triangleNormal;
   double d, normalDirection = 1.0;
   bool found, validNormal, validTriangles = true;
   int listID, pointID, numCP;

   // have to cache changed values because checking once changes them to false
   QVector<bool> changed( list.count() );
   for ( int i = 0; it != list.end(); ++i, ++it )
      changed[i] = ( *it )->changed();

   for ( int i = 0; i < numChildren && validTriangles; ++i )
   {
      if ( childAt( i )->isA( "Triangle" ) )
      {
         obj = ( PMTriangle* )childAt( i );
         obj->createMemento();
         objList.append( obj );
         validNormal = false;

         if ( obj->isSmoothTriangle() )
            numCP = 6;
         else
            numCP = 3;

         cp0 = cp1 = cp2 = 0;
         cn0 = cn1 = cn2 = 0;

         bp0 = bp1 = bp2 = bn0 = bn1 = bn2 = false;

         for ( int j = 0; j < numCP; ++j, ++ptpItr )
         {

            listID = (*ptpItr).listID;
            pointID = (*ptpItr).pointID;
            found = false;
				int k = 0;
            for ( it = list.begin(); it != list.end() && !found; ++it, ++k )
            {
               if( listID == ( *it )->id() )
               {
                  switch( pointID )
                  {
                     case 0:
                        cp0 = ( PM3DControlPoint* ) *it;
                        p0 = cp0->point();
                        bp0 = changed[ k ];
                        break;
                     case 1:
                        cp1 = ( PM3DControlPoint* ) *it;
                        p1 = cp1->point();
                        bp1 = changed[ k ];
                        break;
                     case 2:
                        cp2 = ( PM3DControlPoint* ) *it;
                        p2 = cp2->point();
                        bp2 = changed[ k ];
                        break;
                     case 3:
                        cn0 = ( PMVectorControlPoint* ) *it;
                        n0 = cn0->vector();
                        bn0 = changed[ k ];
                        break;
                     case 4:
                        cn1 = ( PMVectorControlPoint* ) *it;
                        n1 = cn1->vector();
                        bn1 = changed[ k ];
                        break;
                     case 5:
                        cn2 = ( PMVectorControlPoint* ) *it;
                        n2 = cn2->vector();
                        bn2 = changed[ k ];
                        break;
                     default:
                        break;
                  }
                  found = true;
               }
            }
         }

         if ( obj->isSmoothTriangle() )
         {
            triangleNormal = PMVector::cross( obj->point( 1 ) - obj->point( 0 ),
                                                obj->point( 2 ) - obj->point( 0 ) );
            normalDirection = PMVector::dot( triangleNormal, obj->normal( 0 ) );
            if( approxZero( normalDirection  ) )
               normalDirection = PMVector::dot( triangleNormal, obj->normal( 1 ) );
            if( approxZero( normalDirection  ) )
               normalDirection = PMVector::dot( triangleNormal, obj->normal( 2 ) );
            if( normalDirection < 0 )
               triangleNormal = -triangleNormal;
            if( !approxZero( triangleNormal.abs() ) )
            {
               validNormal = true;
               triangleNormal /= triangleNormal.abs();
            }
         }

         if ( bp0 )
         {
            if ( !( p0.approxEqual( p1 ) || p0.approxEqual( p2 ) ) )
               obj->setPoint( 0, p0 );
            else
            {
               validTriangles = false;
               cp0->setPoint( obj->point( 0 ) );
               break;
            }
         }

         if ( bp1 )
         {
            if ( !( p1.approxEqual( p0 ) || p1.approxEqual( p2 ) ) )
               obj->setPoint( 1, p1 );
            else
            {
               validTriangles = false;
               cp1->setPoint( obj->point( 1 ) );
               break;
            }
         }

         if ( bp2 )
         {
            if ( !( p2.approxEqual( p0 ) || p2.approxEqual( p1 ) ) )
               obj->setPoint( 2, p2 );
            else
            {
               validTriangles = false;
               cp2->setPoint( obj->point( 2 ) );
               break;
            }
         }

         if ( obj->isSmoothTriangle() )
         {
            if ( bn0 )
            {
               if( validNormal )
               {
                  d = PMVector::dot( triangleNormal, n0 );
                  if( d > 0 )
                     obj->setNormal( 0, n0 );
                  else
                  {
                     obj->setNormal( 0, n0 - ( d - 1e-5 ) * triangleNormal );
                     cn0->setVector( obj->normal( 0 ) );
                  }
               }
               else
                  cn0->setVector( obj->normal( 0 ) );
            }

            if ( bn1 )
            {
               if( validNormal )
               {
                  d = PMVector::dot( triangleNormal, n1 );
                  if( d > 0 )
                     obj->setNormal( 1, n1 );
                  else
                  {
                     obj->setNormal( 1, n1 - ( d - 1e-5 ) * triangleNormal );
                     cn1->setVector( obj->normal( 1 ) );
                  }
               }
               else
                  cn1->setVector( obj->normal( 1 ) );
            }

            if ( bn2 )
            {
               if( validNormal )
               {
                  d = PMVector::dot( triangleNormal, n2 );
                  if( d > 0 )
                     obj->setNormal( 2, n2 );
                  else
                  {
                     obj->setNormal( 2, n2 - ( d - 1e-5 ) * triangleNormal );
                     cn2->setVector( obj->normal( 2 ) );
                  }
               }
               else
                  cn2->setVector( obj->normal( 2 ) );
            }
         }

         mementoList.append( obj->takeMemento() );

         // Since validTriangles is set to true before the loop
         // and each of the places in the loop that sets validTriangles
         // to false (around line 450, if (bp0) and similar) already
         // breaks out of the loop, this code is never executed.
         // Perhaps it has accidentally snuck into the loop body
         // and needs to be executed after the loop?
         // 
         if ( !validTriangles )
         {
            PMMemento *tm;
            for ( int j = i; j >= 0; --j )
            {
               if ( ( tm = mementoList.takeLast() ) )
               {
                  childAt( j )->restoreMemento( tm );
                  delete tm;
               }
            }
         }
      }
   }

   if ( validTriangles )
   {
      if ( m_pMemento )
         ( ( PMMeshMemento * ) m_pMemento )->setTriangleMementos( mementoList );
      objList.append( this );
      setViewStructureChanged();
   }
}
