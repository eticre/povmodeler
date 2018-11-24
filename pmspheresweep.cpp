/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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


#include "pmspheresweep.h"

#include "pmxmlhelper.h"
#include "pmspheresweepedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"
#include "pmdistancecontrolpoint.h"
#include "pmsplinememento.h"
#include "pmdefaults.h"
#include "pmenumproperty.h"
#include "pmobjectaction.h"
#include "pmpoint.h"
#include "pmmatrix.h"


#include <QList>

const int defaultNumberOfPoints = 2;
const PMVector defaultPoint[defaultNumberOfPoints] =
{
   PMVector( 0.0, 1.0, 0.0 ),
   PMVector( 0.0, 0.0, 0.0 )
};
const double defaultRadii[defaultNumberOfPoints] =
{
   0.3, 0.5
};

const double defaultTolerance = 1e-6;
const PMSphereSweep::SplineType defaultSplineType = PMSphereSweep::LinearSpline;

PMDefinePropertyClass( PMSphereSweep, PMSphereSweepProperty );
PMDefineEnumPropertyClass( PMSphereSweep, PMSphereSweep::SplineType, PMSplineTypeProperty );

PMMetaObject* PMSphereSweep::s_pMetaObject = nullptr;
PMObject* createNewSphereSweep( PMPart* part )
{
   return new PMSphereSweep( part );
}

int PMSphereSweep::s_rSteps = c_defaultSphereSweepRSteps;
int PMSphereSweep::s_sSteps = c_defaultSphereSweepSSteps;
int PMSphereSweep::s_parameterKey = 0;


/**
 * Memento for @ref PMLathe
 */
class PMSphereSweepMemento : public PMSplineMemento
{
public:

    /**
     * Returns true if the spline points were saved
     */
    bool radiiSaved() const { return m_bRadiiSaved; }
   /**
    * Creates a memento for the object originator
    */
   PMSphereSweepMemento( PMObject* originator )
      : PMSplineMemento( originator )
   {
      m_bRadiiSaved = false;
   }
   /**
    * Deletes the memento
    */
   virtual ~PMSphereSweepMemento() { }

   /**
    * Saves the radii
    */
   void setRadii( const QList<double>& r )
   {
      if( !m_bRadiiSaved )
      {
         // Direct assignment does not work with Qt 2.3.x
         // The list will be changed later in a graphical
         // change because QValueList::detach() is called
         // too late!
         // Copy the list by hand.

         QList<double>::ConstIterator it = r.begin();
         for( ; it != r.end(); ++it )
            m_radii.append( *it );

         m_bRadiiSaved = true;
         addChange( PMCData );
      }
   }
   /**
    * Returns the radii
    */
   QList<double> radii() const
   {
      if( !m_bRadiiSaved )
         qCritical(  ) << "Radii points not saved in PMSphereSweepMemento::radii\n";
      return m_radii;
   }


private:
   /**
    * The stored radii
    */
   QList<double> m_radii;
   bool m_bRadiiSaved;
};


PMSphereSweep::PMSphereSweep( PMPart* part )
      : Base( part )
{
   int i;

   for( i = 0; i < defaultNumberOfPoints; i++ )
   {
      m_points.append( defaultPoint[i] );
      m_radii.append( defaultRadii[i] );
   }
   m_splineType = defaultSplineType;
   m_tolerance = defaultTolerance;
}

PMSphereSweep::PMSphereSweep( const PMSphereSweep& l )
      : Base( l )
{
   m_points = l.m_points;
   m_radii = l.m_radii;
   m_splineType = l.m_splineType;
   m_tolerance = l.m_tolerance;
}

PMSphereSweep::~PMSphereSweep()
{
}

QString PMSphereSweep::description() const
{
   return ( "sphere sweep" );
}

void PMSphereSweep::serialize( QDomElement& e, QDomDocument& doc ) const
{
   QDomElement data = doc.createElement( "extra_data" );
   QDomElement p;

   e.setAttribute( "spline_type", m_splineType );
   e.setAttribute( "tolerance", m_tolerance );

   QList<PMVector>::ConstIterator it;
   QList<double>::ConstIterator it2;
   for( it = m_points.begin(), it2 = m_radii.begin();
        it != m_points.end() && it2 != m_radii.end(); ++it, ++it2 )
   {
      p = doc.createElement( "point" );
      p.setAttribute( "vector", ( *it ).serializeXML() );
      p.setAttribute( "radius", *it2 );
      data.appendChild( p );
   }

   e.appendChild( data );
   Base::serialize( e, doc );
}

void PMSphereSweep::readAttributes( const PMXMLHelper& h )
{
   m_splineType = ( SplineType ) h.intAttribute( "spline_type", defaultSplineType );
   m_tolerance = h.doubleAttribute( "tolerance", defaultTolerance );

   m_points.clear();
   m_radii.clear();
   PMVector v( 3 );

   bool ok;
   double dbl;

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
                  dbl = 0;
                  v.loadXML( str );
                  m_points.append( v );
                  QString str = ce.attribute( "radius" );
                  dbl = str.toDouble( &ok );
                  if( !ok )
                  {
                      str.replace(",", ".");
                      dbl = str.toDouble( &ok );
                   }

                  m_radii.append( dbl );
               }
            }
         }
         c = c.nextSibling();
      }
   }

   Base::readAttributes( h );
}

PMMetaObject* PMSphereSweep::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "SphereSweep", Base::metaObject(),
                                        createNewSphereSweep );
      s_pMetaObject->addProperty(
         new PMSphereSweepProperty( "tolerance", &PMSphereSweep::setTolerance, &PMSphereSweep::tolerance ) );
      PMSplineTypeProperty* p = new PMSplineTypeProperty(
         "splineType", &PMSphereSweep::setSplineType, &PMSphereSweep::splineType );
      p->addEnumValue( "LinearSpline", LinearSpline );
      p->addEnumValue( "BSpline", BSpline );
      p->addEnumValue( "CubicSpline", CubicSpline );
      s_pMetaObject->addProperty( p );
      //s_pMetaObject->addProperty( new PMPointProperty() );
   }
   return s_pMetaObject;
}

void PMSphereSweep::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMSphereSweep::setSplineType( PMSphereSweep::SplineType t )
{
   if( m_splineType != t )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSplineTypeID, ( int ) m_splineType );
      setViewStructureChanged();
      m_splineType = t;
   }
}

void PMSphereSweep::setTolerance( double t )
{
   if( m_tolerance != t )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMToleranceID, m_tolerance );
      m_tolerance = t;
   }
}

void PMSphereSweep::setPoints( const QList<PMVector>& points )
{
   if( m_points != points )
   {
      if( m_pMemento )
         ( ( PMSplineMemento* ) m_pMemento )->setSplinePoints( m_points );

      setViewStructureChanged();
      m_points = points;
   }
}

void PMSphereSweep::setRadii( const QList<double>& radii )
{
   if( m_radii != radii )
   {
      if( m_pMemento )
         ( ( PMSphereSweepMemento* ) m_pMemento )->setRadii( m_radii );

      setViewStructureChanged();
      m_radii = radii;
   }
}

PMDialogEditBase* PMSphereSweep::editWidget( QWidget* parent ) const
{
   return new PMSphereSweepEdit( parent );
}

void PMSphereSweep::createMemento()
{
   if( m_pMemento )
      delete m_pMemento;
   m_pMemento = new PMSphereSweepMemento( this );
}

void PMSphereSweep::restoreMemento( PMMemento* s )
{
   PMSphereSweepMemento* m = ( PMSphereSweepMemento* ) s;
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
            case PMToleranceID:
               setTolerance( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMSphereSweep::restoreMemento\n";
               break;
         }
      }
   }
   if( m->splinePointsSaved() )
      setPoints( m->splinePoints() );
   if( m->radiiSaved() )
      setRadii( m->radii() );

   Base::restoreMemento( s );
}


void PMSphereSweep::createViewStructure()
{
   int numSegments = 0;
   int numSpheres = m_points.size();
   m_segments.clear();

   int rSteps = (int)( ( (float)s_rSteps / 2 ) * ( displayDetail() + 1 ) );
   int sSteps = (int)( ( (float)s_sSteps / 2 ) * ( displayDetail() + 1 ) );

   switch ( m_splineType )
   {
      case LinearSpline:
         numSegments = numSpheres - 1;
         setLinear( sSteps );
         break;
      case BSpline:
         numSegments = numSpheres - 3;
         setCurved( false, sSteps );
         break;
      case CubicSpline:
         numSegments = numSpheres - 3;
         setCurved( true, sSteps );
         break;
   }

   //Calculates sphere points
   int numPoints = ( ( rSteps * ( rSteps - 2 ) ) + 2 ) *
                   ( numSegments + 1 );
   //Calculates segments points
   numPoints += ( rSteps * sSteps ) * numSegments;

   //Calculates sphere lines
   int numLines = ( rSteps * ( rSteps + rSteps - 3 ) ) *
                  ( numSegments + 1 );
   //Calculates segments lines
   numLines += ( ( sSteps * rSteps ) +
                 ( ( sSteps - 1 ) * rSteps ) ) * numSegments;

   if ( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( numPoints, numLines );
   }
   else
   {
      m_pViewStructure->points().resize( numPoints );
      m_pViewStructure->lines().resize( numLines );
   }

   PMPointArray& points = m_pViewStructure->points();
   PMLineArray& lines = m_pViewStructure->lines();
   m_nextPoint = m_nextLine = 0;

   PMVector v1, v2;
   double rotval = M_PI / ( rSteps / 2 );

   createSphere( m_segments[0].points[0], m_segments[0].radii[0], rSteps );
   for ( int i = 0; i < numSegments; ++i )
   {
      //int removeme = m_segments[i].points.count();
      for ( int j = 0; j < sSteps; ++ j )
      {
         v1 = m_segments[i].points[sSteps-1] - m_segments[i].points[0];
         v1 = PMVector::cross( m_segments[i].direction[j], v1.orthogonal() );
         v1 = ( v1 * ( 1 / v1.abs() ) ) * m_segments[i].radii[j];

         for ( int k = 0; k < rSteps; ++k )
         {
            v2 = PMMatrix::rotation( m_segments[i].direction[j],
                                     ( rotval * k ) ) * v1;
            points[m_nextPoint++] = PMPoint( v2 + m_segments[i].points[j] );
            if ( k < ( rSteps - 1 ) )
               lines[m_nextLine++] = PMLine(
                  m_nextPoint - 1, m_nextPoint );
            else
               lines[m_nextLine++] = PMLine(
                  m_nextPoint - 1, m_nextPoint - rSteps );

            if ( j < ( sSteps - 1 ) )
            {
               lines[m_nextLine++] = PMLine(
                  m_nextPoint - 1, m_nextPoint + ( rSteps - 1 ) );
            }
         }
      }
      createSphere( m_segments[i].points[sSteps - 1],
                    m_segments[i].radii[sSteps - 1], rSteps );
   }
}

void PMSphereSweep::controlPoints( PMControlPointList& list )
{
   QList<PMVector>::Iterator it;
   QList<double>::Iterator it2;
   int i, nr;

   for( it = m_points.begin(), it2 = m_radii.begin(), nr = 1, i = 0;
        it != m_points.end() && it2 != m_radii.end(); ++it, ++it2, ++nr )
   {
      PM3DControlPoint* p = new PM3DControlPoint( *it, i++,
                                                  ( "Center " + QString::number( nr ) ) );
      list.append( p );
      list.append( new PMDistanceControlPoint( p, PMVector( 1.0, 0.0, 0.0 ),
                                               *it2, i++,
                                               ( "Radius " + QString::number( nr ) + "(x)" ),
                                               true ) );
      list.append( new PMDistanceControlPoint( p, PMVector( 0.0, 1.0, 0.0 ),
                                               *it2, i++,
                                               ( "Radius " + QString::number( nr ) + "(y)" ),
                                               true ) );
      list.append( new PMDistanceControlPoint( p, PMVector( 0.0, 0.0, 1.0 ),
                                               *it2, i++,
                                               ( "Radius " + QString::number( nr ) + "(z)" ),
                                               true ) );
   }
}

void PMSphereSweep::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it1;
   QList<PMVector>::Iterator pit = m_points.begin();
   QList<double>::Iterator rit = m_radii.begin();
   int i;
   PM3DControlPoint* p;
   PMDistanceControlPoint* r;
   bool firstChange = true;

	for( it1 = list.begin(); it1 != list.end() && pit != m_points.end() && rit != m_radii.end();
        ++pit, ++rit )
   {
      p = ( PM3DControlPoint* ) *it1;
      if( p->changed() )
      {
         if( firstChange )
         {
            firstChange = false;
            setViewStructureChanged();
         }
         if( m_pMemento )
         {
            PMSphereSweepMemento* m = ( PMSphereSweepMemento* ) m_pMemento;
            if( !m->splinePointsSaved() )
               m->setSplinePoints( m_points );
         }
         ( *pit ) = p->point();
      }
      ++it1;

      for( i = 0; i < 3 && it1 != list.end(); i++ )
      {
         r = ( PMDistanceControlPoint* ) *it1;
         if( r->changed() )
         {
            if( firstChange )
            {
               firstChange = false;
               setViewStructureChanged();
            }
            if( m_pMemento )
            {
               PMSphereSweepMemento* m = ( PMSphereSweepMemento* ) m_pMemento;
               if( !m->radiiSaved() )
                  m->setRadii( m_radii );
            }
            ( *rit ) = r->distance();
         }
         ++it1;
      }
   }

   for( it1 = list.begin(), rit = m_radii.begin(); rit != m_radii.end() && it1 != list.end(); ++rit )
   {
      ++it1;
      for( i = 0; i < 3 && it1 != list.end(); ++i, ++it1 )
         ( ( PMDistanceControlPoint* ) *it1 )->setDistance( *rit );
   }
}

void PMSphereSweep::addObjectActions( const PMControlPointList& /*cp*/,
                                QList<PMObjectAction*>& actions )
{
   PMObjectAction* a;

   a = new PMObjectAction( s_pMetaObject, PMSplitSegmentID,
                           ( "Add Sphere" ) );
   actions.append( a );

   a = new PMObjectAction( s_pMetaObject, PMJoinSegmentsID,
                           ( "Remove Sphere" ) );
   int np = m_points.count();
   int minp = 2;
   switch( m_splineType )
   {
      case LinearSpline:
         minp = 2;
         break;
      case BSpline:
         minp = 4;
         break;
      case CubicSpline:
         minp = 4;
         break;
   }

   if( np < minp )
      a->setEnabled( false );
   actions.append( a );
}

void PMSphereSweep::objectActionCalled( const PMObjectAction* action,
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
            qCritical(  ) << "Wrong ID in PMSphereSweep::objectActionCalled\n";
            break;
      }
   }
   else
      Base::objectActionCalled( action, cp, cpViewPosition, clickPosition );
}

void PMSphereSweep::splitSegment( const PMControlPointList& /*cp*/,
                            const QList<PMVector*>& cpViewPosition,
                            const PMVector& clickPosition )
{
   // find nearest segment
   int nump = cpViewPosition.count() / 4 - 1;
   double abs = 0.0, minabs = 1e10;
   int ns = -1;
   int i, j;
   PMVector mid( 3 ), dist( 2 );

   QList<PMVector*>::const_iterator it1 = cpViewPosition.begin();
   QList<PMVector*>::const_iterator it2 = cpViewPosition.begin();
   ++it2;

   for( i = 0; i < nump; i++ )
   {
      bool skip = false;
      switch( m_splineType )
      {
         case LinearSpline:
            break;
         case BSpline:
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
      for( j = 0; j < 4; j++ )
      {
         ++it1;
         ++it2;
      }
   }

   // add a new segment
   QList<PMVector> newPoints = m_points;
   QList<double> newRadii = m_radii;

   QList<PMVector>::Iterator it = newPoints.begin() + ns;
   QList<PMVector>::Iterator hit = it;
   ++it;
   mid = ( *it + *hit ) / 2;
   newPoints.insert( it, mid );

   QList<double>::Iterator rit = newRadii.begin() + ns;
   QList<double>::Iterator rhit = rit;
   ++rit;
   newRadii.insert( rit, ( *rit + *rhit ) / 2 );

   setPoints( newPoints );
   setRadii( newRadii );
}

void PMSphereSweep::joinSegments( const PMControlPointList& /*cp*/,
                            const QList<PMVector*>& cpViewPosition,
                            const PMVector& clickPosition )
{
   // find nearest point
   int nump = cpViewPosition.count() / 4;
   int minp = 0;

   switch( m_splineType )
   {
      case LinearSpline:
         minp = 3;
         break;
      case BSpline:
      case CubicSpline:
         minp = 5;
         break;
   }

   if( nump < minp )
   {
      qCritical(  ) << "Not enough points in PMSphereSweep::joinSegments\n";
      return;
   }

   double abs = 0.0, minabs = 1e10;
   int ns = -1;
   int i, j;
   PMVector* p;
   PMVector dist( 2 );

   QList<PMVector*>::const_iterator it1 = cpViewPosition.begin();

   for( i = 0; i < nump; i++ )
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
      for( j = 0; j < 4; j++ )
         ++it1;
   }

   // join two segments
   QList<PMVector> newPoints = m_points;
   QList<double> newRadii = m_radii;

   // never remove the first or last point
   if( ns == 0 )
      ns++;
   if( ns == ( nump - 1 ) )
      ns--;
   newPoints.removeAt( ns );
   newRadii.removeAt( ns );

   setPoints( newPoints );
   setRadii( newRadii );
}

void PMSphereSweep::setRSteps( int r )
{
   if( r >= 4 )
      s_rSteps = r;
   else
      qDebug() << "PMSphereSweep::setRSteps: R must be greater than 3\n";
   s_parameterKey++;
}

void PMSphereSweep::setSSteps( int s )
{
   if( s >= 1 )
      s_sSteps = s;
   else
      qDebug() << "PMSphereSweep::setSSteps: S must be greater than 0\n";
   s_parameterKey++;
}

void PMSphereSweep::setLinear( int sSteps )
{
   int numsegments = ( m_points.size() - 1 );

   double raddif;
   PMVector diff, angle;
   Segment seg;

   for ( int i = 0; i < numsegments; ++i )
   {
      seg.points.clear();
      seg.radii.clear();
      seg.direction.clear();
      diff = ( m_points[i + 1] - m_points[i] ) / ( sSteps - 1.0 );
      raddif = ( m_radii[i + 1] - m_radii[i] ) / ( sSteps - 1.0 );
      angle = diff * ( 1 / diff.abs() );

      for ( int j = 0; j < sSteps; ++j )
      {
         seg.points.append( m_points[i] + ( diff * j ) );
         seg.radii.append( m_radii[i] + ( raddif * j ) );
         seg.direction.append( angle );
      }
      m_segments.append( seg );
   }
}

void PMSphereSweep::setCurved( bool cubic, int sSteps )
{
   int numsegments = ( m_points.size() - 3 );
   PMVector centres[4];
   PMVector vtr;
   double divs = 1.0 / ( sSteps - 1.0 );
   double raddif;
   Segment seg;

   for ( int i = 0; i < numsegments; ++i )
   {
      seg.points.clear();
      seg.radii.clear();
      seg.direction.clear();
      raddif = ( m_radii[i + 2] - m_radii[i + 1] ) / ( sSteps - 1.0 );
      for ( int j = 0; j < 4; ++j )
         centres[j] = m_points[ i + j ];

      for ( int j = 0; j < sSteps; ++j )
      {
         if ( cubic )
            seg.points.append( catmullRom( centres, ( divs * j ) ) );
         else
            seg.points.append( bSpline( centres, ( divs * j ) ) );

         seg.radii.append( m_radii[i + 1] + ( raddif * j ) );
      }

      seg.direction.append( seg.points[0] - seg.points[1] );
      for ( int j = 1; j < ( sSteps - 1 ) ; ++ j )
      {
         vtr = seg.points[ j - 1 ] - seg.points[j];
         vtr += seg.points[j] - seg.points[ j + 1 ];
         seg.direction.append( vtr );
      }
      seg.direction.append( seg.points[ sSteps - 2 ] -
                            seg.points[ sSteps - 1 ] );

      m_segments.append( seg );
   }
}

PMVector PMSphereSweep::catmullRom( PMVector *v, double t )
{
   PMVector rst;
   double t2 = t * t;
   double t3 = t * t * t;

   rst.setX( (
               ( -t3 + 2 * t2 - t )     * v[0].x() +
               ( 3 * t3 -5 * t2 + 2 )   * v[1].x() +
               ( -3 * t3 + 4 * t2 + t ) * v[2].x() +
               ( t3 - t2 )              * v[3].x()
             ) / 2
           );
   rst.setY( (
               ( -t3 + 2 * t2 -t )      * v[0].y() +
               ( 3 * t3 -5 * t2 + 2 )   * v[1].y() +
               ( -3 * t3 + 4 * t2 + t ) * v[2].y() +
               ( t3 - t2)               * v[3].y()
             ) / 2
           );
   rst.setZ( (
               ( -t3 + 2 * t2 - t )     * v[0].z() +
               ( 3 * t3 -5 * t2 + 2 )   * v[1].z() +
               ( -3 * t3 + 4 * t2 + t ) * v[2].z() +
               ( t3 - t2 )              * v[3].z()
             ) / 2
           );

   return rst;
}

PMVector PMSphereSweep::bSpline( PMVector *v, double t )
{
   PMVector rst;
   double t2 = t * t;
   double t3 = t * t * t;

   rst.setX( (
               ( -t3 + 3 * t2 -3 * t + 1 )      * v[0].x() +
               ( 3 * t3 -6 * t2 + 4 )           * v[1].x() +
               ( -3 * t3 + 3 * t2 + 3 * t + 1 ) * v[2].x() +
               ( t3 )                           * v[3].x()
             ) / 6
           );
   rst.setY( (
               ( -t3 + 3 * t2 -3 * t + 1 )      * v[0].y() +
               ( 3 * t3 -6 * t2 + 4 )           * v[1].y() +
               ( -3 * t3 + 3 * t2 + 3 * t + 1 ) * v[2].y() +
               ( t3 )                           * v[3].y()
             ) / 6
           );
   rst.setZ( (
               ( -t3 + 3 * t2 -3 * t + 1 )      * v[0].z() +
               ( 3 * t3 -6 * t2 + 4 )           * v[1].z() +
               ( -3 * t3 + 3 * t2 + 3 * t + 1 ) * v[2].z() +
               ( t3 )                           * v[3].z()
             ) / 6
           );

   return rst;
}

void PMSphereSweep::createSphere( PMVector v, double r, int rSteps )
{
   PMPointArray& points = m_pViewStructure->points();
   PMLineArray& lines = m_pViewStructure->lines();

   PMVector point = PMVector( 0, 1, 0 ) * r;
   int pointUp = m_nextPoint++;
   int pointDown = m_nextPoint++;
   double rotVal1 = M_PI / ( rSteps - 1 );
   double rotVal2 = ( M_PI / rSteps ) * 2;

   points[pointUp] = PMPoint ( point + v );
   points[pointDown] = PMPoint ( ( PMMatrix::rotation( 0, 0, M_PI ) * point ) + v );


   for ( int i = 0; i < rSteps; ++i )
   {
      lines[ m_nextLine++] = PMLine( pointUp, m_nextPoint );
      for ( int j = 1; j < ( rSteps - 1 ); ++j )
      {
         points[m_nextPoint++] = PMPoint(
            ( PMMatrix::rotation( ( rotVal1 * j ), ( rotVal2 * i ), 0 ) * point ) + v );

         if ( i < ( rSteps - 1 ) )
            lines[m_nextLine++] = PMLine( m_nextPoint - 1, m_nextPoint +
               rSteps - 3 );
         else
            lines[m_nextLine++] = PMLine( m_nextPoint - 1, m_nextPoint -
               ( ( rSteps - 1 ) * ( rSteps - 2 ) ) - 1 );

         if ( j < ( rSteps - 2 ) )
            lines[m_nextLine++] = PMLine( m_nextPoint - 1, m_nextPoint );
         else
            lines[m_nextLine++] = PMLine( m_nextPoint - 1, pointDown );
      }
   }
}
