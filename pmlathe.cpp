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


#include "pmlathe.h"

#include "pmxmlhelper.h"
#include "pmlatheedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm2dcontrolpoint.h"
#include "pmsplinememento.h"
#include "pmsplinesegment.h"
#include "pmdefaults.h"
#include "pmenumproperty.h"
#include "pmobjectaction.h"


#include <QList>

const int defaultNumberOfPoints = 4;
const PMVector defaultPoint[defaultNumberOfPoints] =
{
   PMVector( 0.0, 1.0 ),
   PMVector( 0.5, 0.7 ),
   PMVector( 0.5, 0.3 ),
   PMVector( 0.0, 0.0 )
};

const bool defaultSturm = false;
const PMLathe::SplineType defaultSplineType = PMLathe::LinearSpline;

PMDefinePropertyClass( PMLathe, PMLatheProperty );
PMDefineEnumPropertyClass( PMLathe, PMLathe::SplineType, PMSplineTypeProperty );

PMMetaObject* PMLathe::s_pMetaObject = 0;
PMObject* createNewLathe( PMPart* part )
{
   return new PMLathe( part );
}

class PMPointProperty : public PMPropertyBase
{
public:
   PMPointProperty()
         : PMPropertyBase( "splinePoints", PMVariant::Vector )
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
      return ( ( PMLathe* ) object )->numberOfPoints();
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& var )
   {
      PMLathe* p = ( PMLathe* ) obj;
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
      PMLathe* p = ( PMLathe* ) obj;
      QList<PMVector> list = p->points();

      if( m_index >= list.count() )
      {
         qCritical(  ) << "Range error in PMLathe::PointProperty::get" << Qt::endl;
         return PMVariant();
      }

      return PMVariant( list.at( m_index ) );
   }

private:
   int m_index;
};


int PMLathe::s_rSteps = c_defaultLatheRSteps;
int PMLathe::s_sSteps = c_defaultLatheSSteps;
int PMLathe::s_parameterKey = 0;

PMLathe::PMLathe( PMPart* part )
      : Base( part )
{
   int i;

   for( i = 0; i < defaultNumberOfPoints; ++i )
      m_points.append( defaultPoint[i] );
   m_splineType = defaultSplineType;
   m_sturm = defaultSturm;
}

PMLathe::PMLathe( const PMLathe& l )
      : Base( l )
{
   m_points = l.m_points;
   m_splineType = l.m_splineType;
   m_sturm = l.m_sturm;
}

PMLathe::~PMLathe()
{
}

QString PMLathe::description() const
{
   return ( "lathe" );
}

void PMLathe::serialize( QDomElement& e, QDomDocument& doc ) const
{
   QDomElement data = doc.createElement( "extra_data" );
   QDomElement p;

   e.setAttribute( "spline_type", m_splineType );
   e.setAttribute( "sturm", m_sturm );

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

void PMLathe::readAttributes( const PMXMLHelper& h )
{
   m_splineType = ( SplineType ) h.intAttribute( "spline_type", defaultSplineType );
   m_sturm = h.boolAttribute( "sturm", defaultSturm );

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

PMMetaObject* PMLathe::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Lathe", Base::metaObject(),
                                        createNewLathe );
      s_pMetaObject->addProperty(
         new PMLatheProperty( "sturm", &PMLathe::setSturm, &PMLathe::sturm ) );
      PMSplineTypeProperty* p = new PMSplineTypeProperty(
         "splineType", &PMLathe::setSplineType, &PMLathe::splineType );
      p->addEnumValue( "LinearSpline", LinearSpline );
      p->addEnumValue( "QuadraticSpline", QuadraticSpline );
      p->addEnumValue( "CubicSpline", CubicSpline );
      p->addEnumValue( "BezierSpline", BezierSpline );
      s_pMetaObject->addProperty( p );
      s_pMetaObject->addProperty( new PMPointProperty() );
   }
   return s_pMetaObject;
}

void PMLathe::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMLathe::setSplineType( PMLathe::SplineType t )
{
   if( m_splineType != t )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSplineTypeID, ( int ) m_splineType );
      setViewStructureChanged();
      m_splineType = t;
   }
}

void PMLathe::setSturm( bool s )
{
   if( m_sturm != s )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSturmID, m_sturm );
      m_sturm = s;
   }
}

void PMLathe::setPoints( const QList<PMVector>& points )
{
   if( m_points != points )
   {
      if( m_pMemento )
         ( ( PMSplineMemento* ) m_pMemento )->setSplinePoints( m_points );

      setViewStructureChanged();
      m_points = points;
   }
}

PMDialogEditBase* PMLathe::editWidget( QWidget* parent ) const
{
   return new PMLatheEdit( parent );
}

void PMLathe::createMemento()
{
   if( m_pMemento )
      delete m_pMemento;
   m_pMemento = new PMSplineMemento( this );
}

void PMLathe::restoreMemento( PMMemento* s )
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
            case PMSplineTypeID:
               setSplineType( ( SplineType ) data->intData() );
               break;
            case PMSturmID:
               setSturm( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMLathe::restoreMemento\n";
               break;
         }
      }
   }
   if( m->splinePointsSaved() )
      setPoints( m->splinePoints() );

   Base::restoreMemento( s );
}


void PMLathe::createViewStructure()
{
   if( s_sSteps == 0 )
      s_sSteps = c_defaultLatheSSteps;
   if( s_rSteps == 0 )
      s_rSteps = c_defaultLatheRSteps;

   int rSteps = (int)( ( (float)s_rSteps / 2 ) * ( displayDetail() + 1 ) );
   int sSteps = (int)( ( (float)s_sSteps / 2 ) * ( displayDetail() + 1 ) );

   int np = m_points.count();
   int ns = 0;
   int i, j, r, si;

   // calculate number of segments
   switch( m_splineType )
   {
      case LinearSpline:
         ns = np - 1;
         break;
      case QuadraticSpline:
         ns = np - 2;
         break;
      case CubicSpline:
         ns = np - 3;
         break;
      case BezierSpline:
         ns = np / 4;
         break;
   }

   // calculate number of points and lines of the view structure
   int vsp = 0;
   if( m_splineType != BezierSpline )
      vsp = ns * sSteps + 1;
   else
      vsp = ns * ( sSteps + 1 );

   int vsl = 0;
   if( m_splineType != BezierSpline )
      vsl = ( 2 * vsp - 1 ) * rSteps;
   else
      vsl = ns * ( ( 2 * sSteps + 1 ) * rSteps );

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
   QList<PMSplineSegment> segments;
   QList<PMVector>::Iterator it1, it2, it3, it4;
   it1 = m_points.begin();
   it2 = it1; ++it2;
   it3 = it2; ++it3;
   it4 = it3; ++it4;
   PMSplineSegment s;

   for( i = 0; i < ns; ++i )
   {
      switch( m_splineType )
      {
         case LinearSpline:
            s.calculateLinear( *it1, *it2 );
            ++it1;
            ++it2;
            break;
         case QuadraticSpline:
            s.calculateQuadratic( *it1, *it2, *it3 );
            ++it1;
            ++it2;
            ++it3;
            break;
         case CubicSpline:
            s.calculateCubic( *it1, *it2, *it3, *it4 );
            ++it1;
            ++it2;
            ++it3;
            ++it4;
            break;
         case BezierSpline:
            s.calculateBezier( *it1, *it2, *it3, *it4 );
            for( j = 0; j < 4; ++j )
            {
               ++it1;
               ++it2;
               ++it3;
               ++it4;
            }
            break;
      }
      segments.append( s );
   }

   // create the line array
   if( m_splineType != BezierSpline )
   {
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
   }
   else
   {
      PMLineArray& lines = m_pViewStructure->lines();
      int lb = 0;
      int pi = 0;

      for( si = 0; si < ns; ++si )
      {
         for( i = 0; i < sSteps + 1; ++i )
         {
            for( j = 0; j < rSteps - 1; ++j )
               lines[lb+j] = PMLine( lb + j, lb + j + 1 );
            lines[lb+rSteps-1] = PMLine( lb, lb + rSteps - 1 );
            lb += rSteps;
         }
      }
      for( si = 0; si < ns; ++si )
      {
         for( i = 0; i < sSteps; ++i )
         {
            for( j = 0; j < rSteps; ++j )
            {
               lines[lb] = PMLine( pi, pi + rSteps );
               ++pi;
               ++lb;
            }
         }
         pi += rSteps;
      }
   }
   // calculate the points
   PMVector point2, point3;
   QList<PMSplineSegment>::Iterator sit = segments.begin();
   int pi = 0;

   double poffset = 1.0 / sSteps;
   PMMatrix rot = PMMatrix::rotation( 0.0, M_PI * 2.0 / rSteps, 0.0 );
   PMPointArray& points = m_pViewStructure->points();

   if( m_splineType != BezierSpline )
   {
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
   else
   {
      for( i = 0; i < ns; ++i, ++sit )
      {
         for( j = 0; j < sSteps + 1; ++j )
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
      }
   }
}

void PMLathe::controlPoints( PMControlPointList& list )
{
   QList<PMVector>::Iterator it;
   int i, d;

   PM2DControlPoint* cp = 0;
   QList<PM2DControlPoint*> tmp[2];
   QString str_point;

   for( d = 0; d < 2; ++d )
   {
      if( m_splineType != BezierSpline )
      {
         PM2DControlPoint* firstPoint = 0;
         PM2DControlPoint* lastPoint = 0;

         for( it = m_points.begin(), i = 0; it != m_points.end(); ++it, ++i )
         {
            lastPoint = cp;
            str_point = "Point " + QString::number( i+1 )  + "(xy)";
            if( d == 0 )
               cp = new PM2DControlPoint( *it, PM2DControlPoint::PM2DXY, i,
                                          ( str_point ) );
            else
               cp = new PM2DControlPoint( *it, PM2DControlPoint::PM2DZY, i,
                                          ( str_point ) );

            if( i == 0 )
               firstPoint = cp;
            if( ( i == 1 ) && ( m_splineType != LinearSpline ) )
               firstPoint->setBasePoint( cp );

            tmp[d].append( cp );
         }
         if( m_splineType == CubicSpline )
            cp->setBasePoint( lastPoint );
      }
      else
      {
         PM2DControlPoint* helpPoint = 0;

         for( it = m_points.begin(), i = 0; it != m_points.end(); ++it, ++i )
         {
            str_point = "Point " + QString::number( i+1 )  + "(xy)";
            int imod4 = i % 4;
            if( d == 0 )
               cp = new PM2DControlPoint( *it, PM2DControlPoint::PM2DXY, i,
                                          ( str_point ) );
            else
               cp = new PM2DControlPoint( *it, PM2DControlPoint::PM2DZY, i,
                                          ( str_point ) );
            switch( imod4 )
            {
               case 0:
                  helpPoint = cp;
                  break;
               case 1:
                  cp->setBasePoint( helpPoint );
                  break;
               case 2:
                  helpPoint = cp;
                  break;
               case 3:
                  helpPoint->setBasePoint( cp );
                  break;
               default:
                  break;
            }

            tmp[d].append( cp );
         }
      }
   }

	QList<PM2DControlPoint*>::iterator cit1, cit2;

   for( cit1 = tmp[0].begin(), cit2 = tmp[1].begin(); cit1 != tmp[0].end() && cit2 != tmp[1].end(); ++cit1, ++cit2 )
   {
      ( *cit1 )->setLatheLink( *cit2 );
      ( *cit2 )->setLatheLink( *cit1 );
   }
   for( cit1 = tmp[0].begin(); cit1 != tmp[0].end(); ++cit1 )
      list.append( *cit1 );
   for( cit2 = tmp[1].begin(); cit2 != tmp[1].end(); ++cit2 )
      list.append( *cit2 );
}

void PMLathe::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it1 = list.begin(), it2 = list.begin();
   QList<PMVector>::Iterator pit = m_points.begin();
   PM2DControlPoint* p1;
   PM2DControlPoint* p2;
   bool firstChange = true;

   for( it2 += list.count() / 2; it2 != list.end(); ++it1, ++it2, ++pit )
   {
      p1 = ( PM2DControlPoint* ) *it1;
      p2 = ( PM2DControlPoint* ) *it2;

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
   }
}

void PMLathe::addObjectActions( const PMControlPointList& /*cp*/,
                                QList<PMObjectAction*>& actions )
{
   PMObjectAction* a;

   a = new PMObjectAction( s_pMetaObject, PMSplitSegmentID,
                           ( "Add Point" ) );
   actions.append( a );

   a = new PMObjectAction( s_pMetaObject, PMJoinSegmentsID,
                           ( "Remove Point" ) );
   int np = m_points.count();
   int minp =  3;
   switch( m_splineType )
   {
      case LinearSpline:
         minp = 3;
         break;
      case QuadraticSpline:
         minp = 4;
         break;
      case CubicSpline:
         minp = 5;
         break;
      case BezierSpline:
         minp = 8;
         break;
   }

   if( np < minp )
      a->setEnabled( false );
   actions.append( a );
}

void PMLathe::objectActionCalled( const PMObjectAction* action,
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
            qCritical(  ) << "Wrong ID in PMLathe::objectActionCalled\n";
            break;
      }
   }
   else
      Base::objectActionCalled( action, cp, cpViewPosition, clickPosition );
}

void PMLathe::splitSegment( const PMControlPointList& /*cp*/,
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
      for( i = 0; i < nump; ++i )
      {
         bool skip = false;
         switch( m_splineType )
         {
            case LinearSpline:
            case BezierSpline:
               break;
            case QuadraticSpline:
               if( i == 0 )
                  skip = true;
               break;
            case CubicSpline:
               if( ( i == 0 ) || ( i == ( nump - 1 ) ) )
                  skip = true;
               break;
         }

         if( !skip )
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
         }
         ++it1;
         ++it2;
      }
      ++it1;
      ++it2;
   }

   // add a new segment
   QList<PMVector> newPoints = m_points;

   if( m_splineType == BezierSpline )
   {
      ns /= 4;
      ns *= 4;
   }
   QList<PMVector>::Iterator it = newPoints.begin() + ns;
   PMVector p[4];
   QList<PMVector>::Iterator hit = it;

   // calculate the spline segment
   PMSplineSegment segment;
   switch( m_splineType )
   {
      case LinearSpline:
         for( i = 0; i < 2; ++i, ++hit )
            p[i] = *hit;
         segment.calculateLinear( p[0], p[1] );
         break;
      case QuadraticSpline:
         --hit;
         for( i = 0; i < 3; ++i, ++hit )
            p[i] = *hit;
         segment.calculateQuadratic( p[0], p[1], p[2] );
         break;
      case CubicSpline:
         --hit;
         for( i = 0; i < 4; ++i, ++hit )
            p[i] = *hit;
         segment.calculateCubic( p[0], p[1], p[2], p[3] );
         break;
      case BezierSpline:
         for( i = 0; i < 4; ++i, ++hit )
            p[i] = *hit;
         segment.calculateBezier( p[0], p[1], p[2], p[3] );
         break;
   }

   mid = segment.point( 0.5 );
   if( m_splineType != BezierSpline )
   {
      ++it;
      newPoints.insert( it, mid );
   }
   else
   {
      PMVector end = *it;
      ++it;
      *it = end + ( *it - end ) / 2.0;
      ++it;

      PMVector grad = segment.gradient( 0.5 ) / 4.0;

      newPoints.insert( it, mid - grad );
      newPoints.insert( it, mid );
      newPoints.insert( it, mid );
      newPoints.insert( it, mid + grad );

      ++it;
      end = *it;
      --it;
      *it = end + ( *it - end ) / 2.0;
   }
   setPoints( newPoints );
}

void PMLathe::joinSegments( const PMControlPointList& /*cp*/,
                            const QList<PMVector*>& cpViewPosition,
                            const PMVector& clickPosition )
{
   // find nearest point
   int nump = cpViewPosition.count() / 2;
   int minp = 0;

   switch( m_splineType )
   {
      case LinearSpline:
         minp = 3;
         break;
      case QuadraticSpline:
         minp = 4;
         break;
      case CubicSpline:
         minp = 5;
         break;
      case BezierSpline:
         minp = 8;
         break;
   }

   if( nump < minp )
   {
      qCritical(  ) << "Not enough points in PMLathe::joinSegments\n";
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

   if( m_splineType != BezierSpline )
   {
      // never remove the first or last point
      if( ns == 0 )
         ++ns;
      if( ns == ( nump - 1 ) )
         --ns;
      newPoints.removeAt( ns );
   }
   else
   {
      ns = ( ns - 2 ) / 4;
      if( ns < 0 )
         ns = 0;
      if( ns >= ( nump / 4 - 1 ) )
         ns = nump / 4 - 2;

      it = newPoints.begin() + ( ns * 4 + 2 );
      for( i = 0; i < 4; ++i )
         it = newPoints.erase( it );
   }
   setPoints( newPoints );
}

void PMLathe::setRSteps( int r )
{
   if( r >= 4 )
      s_rSteps = r;
   else
      qDebug() << "PMLathe::setRSteps: R must be greater than 3\n";
   ++s_parameterKey;
}

void PMLathe::setSSteps( int s )
{
   if( s >= 1 )
      s_sSteps = s;
   else
      qDebug() << "PMLathe::setSSteps: S must be greater than 0\n";
   ++s_parameterKey;
}
