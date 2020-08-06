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


#include "pmisosurface.h"

#include "pmxmlhelper.h"
#include "pmisosurfaceedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"



const PMIsoSurface::ContainedByType c_defaultContainedBy = PMIsoSurface::Box;
const PMVector c_defaultCorner1 = PMVector( -1, -1, -1 );
const PMVector c_defaultCorner2 = PMVector( 1, 1, 1 );
const PMVector c_defaultCenter = PMVector( 0, 0, 0 );
const double c_defaultRadius = 1;

const double c_defaultThreshold = 0.0;
const double c_defaultAccuracy = 0.001;
const double c_defaultMaxGradient = 1.1;
const bool c_defaultEvaluate = false;
const double c_defaultEvaluate0 = 5;
const double c_defaultEvaluate1 = 1.2;
const double c_defaultEvaluate2 = 0.95;
const double c_defaultOpen = false;
const int c_defaultMaxTrace = 1;
const bool c_defaultAllIntersections = false;

PMDefinePropertyClass( PMIsoSurface, PMIsoSurfaceProperty );

PMViewStructure* PMIsoSurface::s_pDefaultViewStructure = 0;
PMMetaObject* PMIsoSurface::s_pMetaObject = 0;
PMObject* createNewIsoSurface( PMPart* part )
{
   return new PMIsoSurface( part );
}

PMIsoSurface::PMIsoSurface( PMPart* part )
      : Base( part )
{
   m_containedBy = c_defaultContainedBy;
   m_corner1 = c_defaultCorner1;
   m_corner2 = c_defaultCorner2;
   m_center = c_defaultCenter;
   m_radius = c_defaultRadius;
   m_threshold = c_defaultThreshold;
   m_accuracy = c_defaultAccuracy;
   m_maxGradient = c_defaultMaxGradient;
   m_bEvaluate = c_defaultEvaluate;
   m_evaluate[0] = c_defaultEvaluate0;
   m_evaluate[1] = c_defaultEvaluate1;
   m_evaluate[2] = c_defaultEvaluate2;
   m_bOpen = c_defaultOpen;
   m_maxTrace = c_defaultMaxTrace;
   m_bAllIntersections = c_defaultAllIntersections;
}

PMIsoSurface::PMIsoSurface( const PMIsoSurface& b )
      : Base( b )
{
   m_function = b.m_function;
   m_containedBy = b.m_containedBy;
   m_corner1 = b.m_corner1;
   m_corner2 = b.m_corner2;
   m_center = b.m_center;
   m_radius = b.m_radius;
   m_threshold = b.m_threshold;
   m_accuracy = b.m_accuracy;
   m_maxGradient = m_maxGradient;
   m_bEvaluate = b.m_bEvaluate;
   m_evaluate[0] = b.m_evaluate[0];
   m_evaluate[1] = b.m_evaluate[1];
   m_evaluate[2] = b.m_evaluate[2];
   m_bOpen = b.m_bOpen;
   m_maxTrace = b.m_maxTrace;
   m_bAllIntersections = b.m_bAllIntersections;
}

PMIsoSurface::~PMIsoSurface()
{
}

QString PMIsoSurface::description() const
{
   return ( "isosurface" );
}

void PMIsoSurface::serialize( QDomElement& e, QDomDocument& doc ) const
{
   QDomText t = doc.createTextNode( m_function );
   e.appendChild( t );

   if( m_containedBy == Box )
      e.setAttribute( "contained_by", "box" );
   else
      e.setAttribute( "contained_by", "sphere" );
   e.setAttribute( "corner_a", m_corner1.serializeXML() );
   e.setAttribute( "corner_b", m_corner2.serializeXML() );
   e.setAttribute( "center", m_center.serializeXML() );
   e.setAttribute( "radius", m_radius );
   e.setAttribute( "threshold", m_threshold );
   e.setAttribute( "accuracy", m_accuracy );
   e.setAttribute( "max_gradient", m_maxGradient );
   e.setAttribute( "evaluate", m_bEvaluate );
   e.setAttribute( "e0", m_evaluate[0] );
   e.setAttribute( "e1", m_evaluate[1] );
   e.setAttribute( "e2", m_evaluate[2] );
   e.setAttribute( "open", m_bOpen );
   e.setAttribute( "max_trace", m_maxTrace );
   e.setAttribute( "all_intersections", m_bAllIntersections );
   Base::serialize( e, doc );
}

void PMIsoSurface::readAttributes( const PMXMLHelper& h )
{
   QDomNode e = h.element().firstChild();
   if( e.isText() )
      m_function = e.toText().data();

   QString str = h.stringAttribute( "contained_by", "" );
   if( str == "sphere" )
      m_containedBy = Sphere;
   else
      m_containedBy = Box;

   m_corner1 = h.vectorAttribute( "corner_a", c_defaultCorner1 );
   m_corner2 = h.vectorAttribute( "corner_b", c_defaultCorner1 );
   m_center = h.vectorAttribute( "center", c_defaultCenter );
   m_radius = h.doubleAttribute( "radius", c_defaultRadius );
   m_threshold = h.doubleAttribute( "threshold", c_defaultThreshold );
   m_accuracy = h.doubleAttribute( "accuracy", c_defaultAccuracy );
   m_maxGradient = h.doubleAttribute( "max_gradient", c_defaultMaxGradient );
   m_bEvaluate = h.boolAttribute( "evaluate", c_defaultEvaluate );
   m_evaluate[0] = h.doubleAttribute( "e0", c_defaultEvaluate0 );
   m_evaluate[1] = h.doubleAttribute( "e1", c_defaultEvaluate1 );
   m_evaluate[2] = h.doubleAttribute( "e2", c_defaultEvaluate2 );
   m_bOpen = h.boolAttribute( "open", c_defaultOpen );
   m_maxTrace = h.intAttribute( "max_trace", c_defaultMaxTrace );
   m_bAllIntersections = h.boolAttribute( "all_intersections", c_defaultAllIntersections );

   Base::readAttributes( h );
}

PMMetaObject* PMIsoSurface::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "IsoSurface", Base::metaObject(),
                                        createNewIsoSurface );
      // TODO
      /*
      s_pMetaObject->addProperty(
         new PMIsoSurfaceProperty( "corner1", &PMIsoSurface::setCorner1, &PMIsoSurface::corner1 ) );
      s_pMetaObject->addProperty(
         new PMIsoSurfaceProperty( "corner2", &PMIsoSurface::setCorner2, &PMIsoSurface::corner2 ) );
      */
   }
   return s_pMetaObject;
}

void PMIsoSurface::setFunction( const QString& f )
{
   if( f != m_function )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, FunctionID, m_function );
      m_function = f;
   }
}

void PMIsoSurface::setContainedBy( ContainedByType type )
{
   if( type != m_containedBy )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, ContainedByID, m_containedBy );
      m_containedBy = type;
   }
}

void PMIsoSurface::setCorner1( const PMVector& p )
{
   if( p != m_corner1 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, Corner1ID, m_corner1 );
      m_corner1 = p;
      m_corner1.resize( 3 );
      //setViewStructureChanged();
   }
}

void PMIsoSurface::setCorner2( const PMVector& p )
{
   if( p != m_corner2 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, Corner2ID, m_corner2 );
      m_corner2 = p;
      m_corner2.resize( 3 );
      //setViewStructureChanged();
   }
}

void PMIsoSurface::setCenter( const PMVector& v )
{
   if( v != m_center )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, CenterID, m_center );
      m_center = v;
   }
}

void PMIsoSurface::setRadius( double r )
{
   if( r != m_radius )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, RadiusID, m_radius );
      m_radius = r;
   }
}
void PMIsoSurface::setThreshold( double d )
{
   if( d != m_threshold )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, ThresholdID, m_threshold );
      m_threshold = d;
   }
}

void PMIsoSurface::setAccuracy( double d )
{
   if( d != m_accuracy )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, AccuracyID, m_accuracy );
      m_accuracy = d;
   }
}

void PMIsoSurface::setMaxGradient( double d )
{
   if( d != m_maxGradient )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, MaxGradientID, m_maxGradient );
      m_maxGradient = d;
   }
}
void PMIsoSurface::setEvaluate( bool yes )
{
   if( yes != m_bEvaluate )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, EvaluateID, m_evaluate );
      m_bEvaluate = yes;
   }
}

void PMIsoSurface::setEvaluateValue( int index, double v )
{
   if( index < 0 || index > 2 )
   {
      qCritical(  ) << "Illegal index in PMIsoSurface::setEvaluateValue" << Qt::endl;
      return;
   }

   if( v != m_evaluate[index] )
   {
      if( m_pMemento )
      {
         int id = Evaluate0ID;
         switch( index )
         {
            case 0: id = Evaluate0ID; break;
            case 1: id = Evaluate1ID; break;
            case 2: id = Evaluate2ID; break;
            default: break;
         }

         m_pMemento->addData( s_pMetaObject, id, m_evaluate[index] );
      }
      m_evaluate[index] = v;
   }
}

void PMIsoSurface::setOpen( bool yes )
{
   if( yes != m_bOpen )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, OpenID, m_bOpen );
      m_bOpen = yes;
   }
}

void PMIsoSurface::setMaxTrace( int i )
{
   if( i != m_maxTrace )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, MaxTraceID, m_maxTrace );
      m_maxTrace = i;
   }
}

void PMIsoSurface::setAllIntersections( bool yes )
{
   if( yes != m_bAllIntersections )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, AllIntersectionsID, m_bAllIntersections );
      m_bAllIntersections = yes;
   }
}

PMDialogEditBase* PMIsoSurface::editWidget( QWidget* parent ) const
{
   return new PMIsoSurfaceEdit( parent );
}

void PMIsoSurface::restoreMemento( PMMemento* s )
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
            case FunctionID:
               setFunction( data->stringData() );
               break;
            case ContainedByID:
               setContainedBy( ( ContainedByType ) data->intData() );
               break;
            case Corner1ID:
               setCorner1( data->vectorData() );
               break;
            case Corner2ID:
               setCorner2( data->vectorData() );
               break;
            case CenterID:
               setCenter( data->vectorData() );
               break;
            case RadiusID:
               setRadius( data->doubleData() );
               break;
            case ThresholdID:
               setThreshold( data->doubleData() );
               break;
            case AccuracyID:
               setAccuracy( data->doubleData() );
               break;
            case MaxGradientID:
               setMaxGradient( data->doubleData() );
               break;
            case EvaluateID:
               setEvaluate( data->boolData() );
               break;
            case Evaluate0ID:
               setEvaluateValue( 0, data->doubleData() );
               break;
            case Evaluate1ID:
               setEvaluateValue( 1, data->doubleData() );
               break;
            case Evaluate2ID:
               setEvaluateValue( 2, data->doubleData() );
               break;
            case OpenID:
               setOpen( data->boolData() );
               break;
            case MaxTraceID:
               setMaxTrace( data->intData() );
               break;
            case AllIntersectionsID:
               setAllIntersections( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMIsoSurface::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}


void PMIsoSurface::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}
