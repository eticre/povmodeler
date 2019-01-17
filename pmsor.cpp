/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002-2006 by Andreas Zehender
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


#include "pmsor.h"

#include "pmxmlhelper.h"
#include "pmsoredit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pmsorcontrolpoint.h"
#include "pmsplinememento.h"
#include "pmsorsegment.h"
#include "pmdefaults.h"
#include "pmobjectaction.h"


#include <QList>

const int defaultNumberOfPoints = 4;
const PMVector defaultPoint[defaultNumberOfPoints] =
{
   PMVector( 0.0, 0.0 ),
   PMVector( 0.5, 0.3 ),
   PMVector( 0.5, 0.7 ),
   PMVector( 0.0, 1.0 )
};

const bool defaultSturm = false;
const bool defaultOpen = false;

int PMSurfaceOfRevolution::s_rSteps = c_defaultSurfaceOfRevolutionRSteps;
int PMSurfaceOfRevolution::s_sSteps = c_defaultSurfaceOfRevolutionSSteps;
int PMSurfaceOfRevolution::s_parameterKey = 0;
PMMetaObject* PMSurfaceOfRevolution::s_pMetaObject = nullptr;
PMObject* createNewSurfaceOfRevolution( PMPart* part )
{
   return new PMSurfaceOfRevolution( part );
}

PMDefinePropertyClass( PMSurfaceOfRevolution, PMSurfaceOfRevolutionProperty );

class PMPointProperty : public PMPropertyBase
{
public:
   PMPointProperty()
         : PMPropertyBase( "controlPoints", PMVariant::Vector )
   {
      m_index = 0;
   }
   virtual int dimensions() const { return 1; }
   virtual void setIndex( int /*dimension*/, int index )
   {
      m_index = index;
   }
   virtual int size( PMObject* object, int /*dimension*/ ) const
   {
      return ( dynamic_cast<PMSurfaceOfRevolution*>(object) )->numberOfPoints();
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& var )
   {
      PMSurfaceOfRevolution* p = dynamic_cast<PMSurfaceOfRevolution*>(obj);
      QList<PMVector> list = p->points();
      QList<PMVector>::Iterator it = list.begin();
      int i;
      PMVector v = var.vectorData();
      v.resize( 2 );

      for( i = 0; i < m_index && it != list.end(); ++i )
         ++it;
      // expand the list if necessary
      for( ; i < m_index; ++i )
         list.insert( it, v );
      if( it == list.end() )
         it = list.insert( it, v );
      else
         *it = v;

      p->setPoints( list );
      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      PMSurfaceOfRevolution* p = ( PMSurfaceOfRevolution* ) obj;
      QList<PMVector> list = p->points();

      if( m_index >= list.count() )
      {
         qCritical(  ) << "Range error in PMSurfaceOfRevolution::PointProperty::get" << endl;
         return PMVariant();
      }

      return PMVariant( list.at( m_index ) );
   }

private:
   int m_index;
};

PMSurfaceOfRevolution::PMSurfaceOfRevolution( PMPart* part )
      : Base( part )
{
   int i;

   for( i = 0; i < defaultNumberOfPoints; ++i )
      m_points.append( defaultPoint[i] );
   m_sturm = defaultSturm;
   m_open = defaultOpen;
}

PMSurfaceOfRevolution::PMSurfaceOfRevolution( const PMSurfaceOfRevolution& s )
      : Base( s )
{
   m_points = s.m_points;
   m_sturm = s.m_sturm;
   m_open = s.m_open;
}

PMSurfaceOfRevolution::~PMSurfaceOfRevolution()
{
}

QString PMSurfaceOfRevolution::description() const
{
   return ( "surface of revolution" );
}

void PMSurfaceOfRevolution::serialize( QDomElement& e, QDomDocument& doc ) const
{
   QDomElement data = doc.createElement( "extra_data" );
   QDomElement p;

   e.setAttribute( "sturm", m_sturm );
   e.setAttribute( "open", m_open );

   QList<PMVector>::ConstIterator it;
   for( it = m_points.begin(); it != m_points.end(); ++it )
   {
      p = doc.createElement( "point" );
      p.setAttribute( "vector", ( *it ).serializeXML() );
      data.appendChild( p );
   }

   e.appendChild( data );
   Base::serialize( e, doc );
}

void PMSurfaceOfRevolution::readAttributes( const PMXMLHelper& h )
{
   m_sturm = h.boolAttribute( "sturm", defaultSturm );
   m_open = h.boolAttribute( "open", defaultOpen );

   m_points.clear();
   PMVector v( 2 );

   QDomElement e = h.extraData();
   if( !e.isNull() )
   {
      QDomNode c = e.firstChild();
      while( !c.isNull() )
      {
         if( c.isElement() )
         {
            QDomElement ce = c.toElement();
            if( ce.tagName() == "point" )
            {
               QString str = ce.attribute( "vector" );
               if( !str.isNull() )
               {
                  v.loadXML( str );
                  m_points.append( v );
               }
            }
         }
         c = c.nextSibling();
      }
   }

   Base::readAttributes( h );
}

PMMetaObject* PMSurfaceOfRevolution::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "SurfaceOfRevolution", Base::metaObject(),
                                        createNewSurfaceOfRevolution );
      s_pMetaObject->addProperty(
         new PMSurfaceOfRevolutionProperty( "sturm", &PMSurfaceOfRevolution::setSturm,
                         &PMSurfaceOfRevolution::sturm ) );
      s_pMetaObject->addProperty(
         new PMSurfaceOfRevolutionProperty( "open", &PMSurfaceOfRevolution::setOpen,
                         &PMSurfaceOfRevolution::open ) );
      s_pMetaObject->addProperty( new PMPointProperty() );
   }
   return s_pMetaObject;
}

void PMSurfaceOfRevolution::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMSurfaceOfRevolution::setSturm( bool s )
{
   if( m_sturm != s )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSturmID, m_sturm );
      m_sturm = s;
   }
}

void PMSurfaceOfRevolution::setOpen( bool o )
{
   if( m_open != o )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOpenID, m_open );
      m_open = o;
   }
}

void PMSurfaceOfRevolution::setPoints( const QList<PMVector>& points )
{
   if( m_points != points )
   {
      if( m_pMemento )
         ( ( PMSplineMemento* ) m_pMemento )->setSplinePoints( m_points );

      setViewStructureChanged();
      m_points = points;
   }
}

PMDialogEditBase* PMSurfaceOfRevolution::editWidget( QWidget* parent ) const
{
   return new PMSurfaceOfRevolutionEdit( parent );
}

void PMSurfaceOfRevolution::createMemento()
{
   if( m_pMemento )
      delete m_pMemento;
   m_pMemento = new PMSplineMemento( this );
}

void PMSurfaceOfRevolution::restoreMemento( PMMemento* s )
{
   PMSplineMemento* m = ( PMSplineMemento* ) s;
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
            case PMSturmID:
               setSturm( data->boolData() );
               break;
            case PMOpenID:
               setOpen( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMSurfaceOfRevolution::restoreMemento\n";
               break;
         }
      }
   }
   if( m->splinePointsSaved() )
      setPoints( m->splinePoints() );

   Base::restoreMemento( s );
}


void PMSurfaceOfRevolution::createViewStructure()
{
   if( s_sSteps == 0 )
      s_sSteps = c_defaultSurfaceOfRevolutionSSteps;
   if( s_rSteps == 0 )
      s_rSteps = c_defaultSurfaceOfRevolutionRSteps;

   int rSteps = (int)( ( (float)s_rSteps / 2 ) * ( displayDetail() + 1 ) );
   int sSteps = (int)( ( (float)s_sSteps / 2 ) * ( displayDetail() + 1 ) );

   int np = m_points.count();
   int i, j, r;

   // calculate number of segments
   int ns = np - 3;

   // calculate number of points and lines of the view structure
   int vsp = ns * sSteps + 1;
   int vsl = ( 2 * vsp - 1 ) * rSteps;
   vsp *= rSteps;

   if( m_pViewStructure )
   {
      if( m_pViewStructure->points().size() != vsp )
         m_pViewStructure->points().resize( vsp );
      if( m_pViewStructure->lines().size() != vsl )
         m_pViewStructure->lines().resize( vsl );
   }
   else
      m_pViewStructure = new PMViewStructure( vsp, vsl );


   // calculate the spline segments
   QList<PMSorSegment> segments;
   QList<PMVector>::Iterator it1, it2, it3, it4;
   it1 = m_points.begin();
   it2 = it1; ++it2;
   it3 = it2; ++it3;
   it4 = it3; ++it4;

   for( i = 0; i < ns; ++i, ++it1, ++it2, ++it3, ++it4 )
      segments.append( PMSorSegment( *it1, *it2, *it3, *it4 ) );

   // create the line array
   PMLineArray& lines = m_pViewStructure->lines();
   int vl = ns * sSteps;
   int lb = 0;
   for( i = 0; i < vl + 1; ++i )
   {
      for( j = 0; j < rSteps - 1; ++j )
         lines[lb+j] = PMLine( lb + j, lb + j + 1 );
      lines[lb+rSteps-1] = PMLine( lb, lb + rSteps - 1 );
      lb += rSteps;
   }
   int pi = 0;
   for( i = 0; i < vl; ++i )
   {
      for( j = 0; j < rSteps; ++j )
      {
         lines[lb] = PMLine( pi, pi + rSteps );
         ++pi;
         ++lb;
      }
   }

   // calculate the points
   PMVector point2, point3;
   QList<PMSorSegment>::Iterator sit = segments.begin();

   double poffset = 1.0 / sSteps;
   PMMatrix rot = PMMatrix::rotation( 0.0, M_PI * 2.0 / rSteps, 0.0 );
   PMPointArray& points = m_pViewStructure->points();
   pi = 0;

   for( i = 0; i < ns; ++i, ++sit )
   {
      for( j = 0; j < sSteps; ++j )
      {
         point2 = ( *sit ).point( poffset * j );
         point3[0] = point2[0];
         point3[1] = point2[1];
         point3[2] = 0.0;

         for( r = 0; r < rSteps; ++r )
         {
            points[pi] = PMPoint( point3 );
            if( r != rSteps - 1 )
               point3.transform( rot );
            ++pi;
         }
      }
      if( i == ns - 1 )
      {
         point2 = ( *sit ).point( 1.0 );
         point3[0] = point2[0];
         point3[1] = point2[1];
         point3[2] = 0.0;

         for( r = 0; r < rSteps; ++r )
         {
            points[pi] = PMPoint( point3 );
            if( r != rSteps - 1 )
               point3.transform( rot );
            ++pi;
         }
      }
   }
}

void PMSurfaceOfRevolution::controlPoints( PMControlPointList& list )
{
   QList<PMVector>::Iterator it;
   QList<PMSorControlPoint*> tmp1, tmp2;
   int i;
   QString str_point;
   PMSorControlPoint* cp = 0;

   PMSorControlPoint* lastPoint = 0;
   cp = 0;

   for( it = m_points.begin(), i = 0; it != m_points.end(); ++it, ++i )
   {
      str_point = "Point " + QString::number( i+1 )  + "(xy)";
      lastPoint = cp;
      cp = new PMSorControlPoint( lastPoint, *it, PMSorControlPoint::PM2DXY, i,
                                  ( str_point ) );
      tmp1.append( cp );
   }

   lastPoint = 0;
   cp = 0;

   for( it = m_points.begin(), i = 0; it != m_points.end(); ++it, ++i )
   {
      str_point = "Point " + QString::number( i+1 )  + "(xy)";
      lastPoint = cp;
      cp = new PMSorControlPoint( lastPoint, *it, PMSorControlPoint::PM2DZY, i,
                                  ( str_point ) );
      tmp2.append( cp );
   }

   QList<PMSorControlPoint*>::iterator cit1, cit2;
   for( cit1 = tmp1.begin(), cit2 = tmp2.begin(); cit1 != tmp1.end() && cit2 != tmp2.end(); ++cit1, ++cit2 )
   {
      ( *cit1 )->setSorLink( *cit2 );
      ( *cit2 )->setSorLink( *cit1 );
   }
   for( cit1 = tmp1.begin(); cit1 != tmp1.end(); ++cit1 )
      list.append( *cit1 );
   for( cit2 = tmp2.begin(); cit2 != tmp2.end(); ++cit2 )
      list.append( *cit2 );
}

void PMSurfaceOfRevolution::controlPointsChanged( PMControlPointList& list )
{
   PMControlPointList::iterator it1 = list.begin(), it2 = list.begin();
   QList<PMVector>::Iterator pit = m_points.begin();
   PMSorControlPoint* p1;
   PMSorControlPoint* p2;
   bool firstChange = true;
   PMVector lastPoint( 2 );
   int num = list.count() / 2;
   int pnr = 0;

   for( it2 += num; it2 != list.end(); ++it1, ++it2, ++pit, ++pnr )
   {
      p1 = ( PMSorControlPoint* ) *it1;
      p2 = ( PMSorControlPoint* ) *it2;

      if( p1->changed() )
      {
         if( firstChange )
         {
            if( m_pMemento )
            {
               PMSplineMemento* m = ( PMSplineMemento* ) m_pMemento;
               if( !m->splinePointsSaved() )
                  m->setSplinePoints( m_points );
            }
            firstChange = false;
            setViewStructureChanged();
         }
         p2->setPoint( p1->point() );
         ( *pit ) = p1->point();
      }
      else if( p2->changed() )
      {
         if( firstChange )
         {
            if( m_pMemento )
            {
               PMSplineMemento* m = ( PMSplineMemento* ) m_pMemento;
               if( !m->splinePointsSaved() )
                  m->setSplinePoints( m_points );
            }
            firstChange = false;
            setViewStructureChanged();
         }
         p1->setPoint( p2->point() );
         ( *pit ) = p2->point();
      }

      if( ( pnr > 1 ) && ( pnr < ( num - 1 ) ) )
      {
         if( ( ( *pit )[1] - lastPoint[1] ) < c_sorTolerance )
         {
            ( *pit )[1] = lastPoint[1] + c_sorTolerance;
            p1->setPoint( *pit );
            p2->setPoint( *pit );
         }
      }
      if( ( pnr == ( num - 1 ) ) || ( pnr == 2 ) )
      {
         QList<PMVector>::Iterator hit = pit;
         --hit; --hit;

         if( approxZero( ( *hit )[1] - ( *pit )[1], c_sorTolerance ) )
         {
            ( *pit )[1] = ( *hit )[1] + c_sorTolerance;
            p1->setPoint( *pit );
            p2->setPoint( *pit );
         }
      }

      lastPoint = *pit;
   }
}

void PMSurfaceOfRevolution::addObjectActions( const PMControlPointList& /*cp*/,
                                QList<PMObjectAction*>& actions )
{
   PMObjectAction* a;

   a = new PMObjectAction( s_pMetaObject, PMSplitSegmentID,
                           ( "Add Point" ) );
   actions.append( a );

   a = new PMObjectAction( s_pMetaObject, PMJoinSegmentsID,
                           ( "Remove Point" ) );
   int np = m_points.count();

   if( np < 5 )
      a->setEnabled( false );
   actions.append( a );
}

void PMSurfaceOfRevolution::objectActionCalled( const PMObjectAction* action,
                                  const PMControlPointList& cp,
                                  const QList<PMVector*>& cpViewPosition,
                                  const PMVector& clickPosition )
{
   if( action->objectType() == s_pMetaObject )
   {
      switch( action->actionID() )
      {
         case PMSplitSegmentID:
            splitSegment( cp, cpViewPosition, clickPosition );
            break;
         case PMJoinSegmentsID:
            joinSegments( cp, cpViewPosition, clickPosition );
            break;
         default:
            qCritical(  ) << "Wrong ID in PMSurfaceOfRevolution::objectActionCalled\n";
            break;
      }
   }
   else
      Base::objectActionCalled( action, cp, cpViewPosition, clickPosition );
}

void PMSurfaceOfRevolution::splitSegment( const PMControlPointList& /*cp*/,
                            const QList<PMVector*>& cpViewPosition,
                            const PMVector& clickPosition )
{
   // find nearest segment
   int nump = cpViewPosition.count() / 2 - 1;
   double abs = 0.0, minabs = 1e10;
   int ns = -1;
   int i, j;
   PMVector mid( 3 ), dist( 2 );

   QList<PMVector*>::const_iterator it1 = cpViewPosition.begin();
   QList<PMVector*>::const_iterator it2 = cpViewPosition.begin();
   ++it2;

   for( j = 0; j < 2; ++j )
   {
      ++it1;
      ++it2;
      for( i = 1; i < ( nump - 1 ); ++i )
      {
         mid = ( **it1 + **it2 ) / 2.0;
         dist[0] = mid[0];
         dist[1] = mid[1];
         dist -= clickPosition;
         abs = dist.abs();

         if( ( minabs > abs ) || ( ns < 0 ) )
         {
            minabs = abs;
            ns = i;
         }
         ++it1;
         ++it2;
      }
      ++it1;
      ++it2;
      ++it1;
      ++it2;
   }

   // add a new segment
   QList<PMVector> newPoints = m_points;
   QList<PMVector>::Iterator it = newPoints.begin() + ns;
   PMVector p[4];
   QList<PMVector>::Iterator hit = it;

   // calculate the spline segment
   --hit;
   for( i = 0; i < 4; ++i, ++hit )
      p[i] = *hit;
   PMSorSegment segment( p[0], p[1], p[2], p[3] );

   mid = segment.point( 0.5 );
   if( mid[0] < 0 )
      mid[0] = 0;
   ++it;
   it = newPoints.insert( it, mid );
   hit = it;
   --it;

   for( ; hit != newPoints.end(); ++it, ++hit )
      if( ( ( *hit )[1] - ( *it )[1] ) < c_sorTolerance )
         ( *hit )[1] = ( *it )[1] + c_sorTolerance;

   setPoints( newPoints );
}

void PMSurfaceOfRevolution::joinSegments( const PMControlPointList& /*cp*/,
                            const QList<PMVector*>& cpViewPosition,
                            const PMVector& clickPosition )
{
   // find nearest point
   int nump = cpViewPosition.count() / 2;

   if( nump < 5 )
   {
      qCritical(  ) << "Not enough points in PMSurfaceOfRevolution::joinSegments\n";
      return;
   }

   double abs = 0.0, minabs = 1e10;
   int ns = -1;
   int i, j;
   PMVector* p;
   PMVector dist( 2 );

   QList<PMVector*>::const_iterator it1 = cpViewPosition.begin();

   for( j = 0; j < 2; ++j )
   {
      for( i = 0; i < nump; ++i )
      {
         p = *it1;
         dist[0] = (*p)[0];
         dist[1] = (*p)[1];
         dist -= clickPosition;
         abs = dist.abs();

         if( ( minabs > abs ) || ( ns < 0 ) )
         {
            minabs = abs;
            ns = i;
         }
         ++it1;
      }
   }

   // join two segments
   QList<PMVector> newPoints = m_points;
   QList<PMVector>::Iterator it;

   // never remove the first or last point
   if( ns == 0 )
      ++ns;
   if( ns == ( nump - 1 ) )
      --ns;
   newPoints.removeAt( ns );

   setPoints( newPoints );
}

void PMSurfaceOfRevolution::setRSteps( int r )
{
   if( r >= 4 )
      s_rSteps = r;
   else
      qDebug() << "PMSurfaceOfRevolution::setRSteps: R must be greater than 3\n";
   ++s_parameterKey;
}

void PMSurfaceOfRevolution::setSSteps( int s )
{
   if( s >= 1 )
      s_sSteps = s;
   else
      qDebug() << "PMSurfaceOfRevolution::setSSteps: S must be greater than 0\n";
   ++s_parameterKey;
}
