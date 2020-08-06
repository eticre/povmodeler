/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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


#include "pmtriangle.h"
#include "pmtriangleedit.h"

#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"
#include "pmvectorcontrolpoint.h"



const PMVector point0Default = PMVector( -1.0, 0.0, 0.0 );
const PMVector point1Default = PMVector( 1.0, 0.0, 0.0 );
const PMVector point2Default = PMVector( 0.0, 0.0, 1.0 );
const PMVector normal0Default = PMVector( 0.0, 1.0, 0.0 );
const PMVector normal1Default = PMVector( 0.0, 1.0, 0.0 );
const PMVector normal2Default = PMVector( 0.0, 1.0, 0.0 );
const PMVector uvVector0Default = PMVector( 0.0, 0.0 );
const PMVector uvVector1Default = PMVector( 1.0, 0.0 );
const PMVector uvVector2Default = PMVector( 0.5, 1.0 );

PMDefinePropertyClass( PMTriangle, PMTriangleProperty );

class PMPointProperty : public PMPropertyBase
{
public:
   PMPointProperty() : PMPropertyBase( "points", PMVariant::Vector )
   {
      m_index = 0;
   }
   virtual int dimensions() const { return 1; }
   virtual void setIndex( int /*dimension*/, int index )
   {
      if( index < 0 || index > 2 )
         qCritical(  ) << "Illegal index in PMTriangle::PointProperty::setIndex" << Qt::endl;
      else
         m_index = index;
   }
   virtual int size( PMObject* /*object*/, int /*dimension*/ ) const
   {
      return 3;
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& v )
   {
      PMTriangle* p = ( PMTriangle* ) obj;
      p->setPoint( m_index, v.vectorData() );
      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      const PMTriangle* p = ( const PMTriangle* ) obj;
      return PMVariant( p->point( m_index ) );
   }

private:
   int m_index;
};

class PMNormalProperty : public PMPropertyBase
{
public:
   PMNormalProperty() : PMPropertyBase( "normals", PMVariant::Vector )
   {
      m_index = 0;
   }
   virtual int dimensions() const { return 1; }
   virtual void setIndex( int /*dimension*/, int index )
   {
      if( index < 0 || index > 2 )
         qCritical(  ) << "Illegal index in PMTriangle::NormalProperty::setIndex" << Qt::endl;
      else
         m_index = index;
   }
   virtual int size( PMObject* /*object*/, int /*dimension*/ ) const
   {
      return 3;
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& v )
   {
      PMTriangle* p = ( PMTriangle* ) obj;
      p->setNormal( m_index, v.vectorData() );
      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      const PMTriangle* p = ( const PMTriangle* ) obj;
      return PMVariant( p->normal( m_index ) );
   }

private:
   int m_index;
};

class PMUVVectorProperty : public PMPropertyBase
{
public:
   PMUVVectorProperty()
         : PMPropertyBase( "uvVectors", PMVariant::Vector )
   {
      m_index = 0;
   }
   virtual int dimensions() const { return 1; }
   virtual void setIndex( int /*dimension*/, int index )
   {
      if( index < 0 || index > 2 )
         qCritical(  ) << "Illegal index in PMTriangle::UVVectorProperty::setIndex" << Qt::endl;
      else
         m_index = index;
   }
   virtual int size( PMObject* /*object*/, int /*dimension*/ ) const
   {
      return 2;
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& v )
   {
      PMTriangle* p = ( PMTriangle* ) obj;
      p->setUVVector( m_index, v.vectorData() );
      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      const PMTriangle* p = ( const PMTriangle* ) obj;
      return PMVariant( p->uvVector( m_index ) );
   }

private:
   int m_index;
};

PMMetaObject* PMTriangle::s_pMetaObject = 0;
PMObject* createNewTriangle( PMPart* part )
{
   return new PMTriangle( part );
}
PMViewStructure* PMTriangle::s_pDefaultViewStructure = 0;

PMTriangle::PMTriangle( PMPart* part )
      : Base( part )
{
   m_point[0] = point0Default;
   m_point[1] = point1Default;
   m_point[2] = point2Default;
   m_normal[0] = normal0Default;
   m_normal[1] = normal1Default;
   m_normal[2] = normal2Default;
   m_smooth = false;
   m_uvVector[0] = uvVector0Default;
   m_uvVector[1] = uvVector1Default;
   m_uvVector[2] = uvVector2Default;
   m_uvEnabled = false;
}

PMTriangle::PMTriangle( const PMTriangle& t )
      : Base( t )
{
   int i;
   for( i = 0; i < 3; i++ )
   {
      m_point[i] = t.m_point[i];
      m_normal[i] = t.m_normal[i];
      m_uvVector[i] = t.m_uvVector[i];
   }
   m_smooth = t.m_smooth;
   m_uvEnabled = t.m_uvEnabled;
}

PMTriangle::~PMTriangle()
{
}

QString PMTriangle::description() const
{
   if( m_smooth )
      return ( "smooth triangle" );
   return ( "triangle" );
}

void PMTriangle::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "point0", m_point[0].serializeXML() );
   e.setAttribute( "point1", m_point[1].serializeXML() );
   e.setAttribute( "point2", m_point[2].serializeXML() );
   e.setAttribute( "normal0", m_normal[0].serializeXML() );
   e.setAttribute( "normal1", m_normal[1].serializeXML() );
   e.setAttribute( "normal2", m_normal[2].serializeXML() );
   e.setAttribute( "smooth", m_smooth );
   e.setAttribute( "uvVector0", m_uvVector[0].serializeXML() );
   e.setAttribute( "uvVector1", m_uvVector[1].serializeXML() );
   e.setAttribute( "uvVector2", m_uvVector[2].serializeXML() );
   e.setAttribute( "uvEnabled", m_uvEnabled );
   Base::serialize( e, doc );
}

void PMTriangle::readAttributes( const PMXMLHelper& h )
{
   m_point[0] = h.vectorAttribute( "point0", point0Default );
   m_point[1] = h.vectorAttribute( "point1", point1Default );
   m_point[2] = h.vectorAttribute( "point2", point2Default );
   m_normal[0] = h.vectorAttribute( "normal0", normal0Default );
   m_normal[1] = h.vectorAttribute( "normal1", normal1Default );
   m_normal[2] = h.vectorAttribute( "normal2", normal2Default );
   m_smooth = h.boolAttribute( "smooth", false );
   m_uvVector[0] = h.vectorAttribute( "uvVector0", uvVector0Default );
   m_uvVector[1] = h.vectorAttribute( "uvVector1", uvVector1Default );
   m_uvVector[2] = h.vectorAttribute( "uvVector2", uvVector2Default );
   m_uvEnabled = h.boolAttribute( "uvEnabled", m_uvEnabled );
   Base::readAttributes( h );
}

PMMetaObject* PMTriangle::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Triangle", Base::metaObject(),
                                        createNewTriangle );
      s_pMetaObject->addProperty(
         new PMTriangleProperty( "smooth", &PMTriangle::setSmoothTriangle,
                         &PMTriangle::isSmoothTriangle ) );
      s_pMetaObject->addProperty( new PMPointProperty() );
      s_pMetaObject->addProperty( new PMNormalProperty() );
      s_pMetaObject->addProperty( new PMUVVectorProperty() );
   }
   return s_pMetaObject;
}

void PMTriangle::setPoint( int i, const PMVector& p )
{
   if( ( i >= 0 ) && ( i <= 2 ) )
   {
      if( p != m_point[i] )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMPoint0ID + i, m_point[i] );
         m_point[i] = p;
         m_point[i].resize( 3 );
         setViewStructureChanged();
      }
   }
   else
      qCritical(  ) << "Wrong index in PMTriangle::setPoint\n";
}

PMVector PMTriangle::point( int i ) const
{
   if( ( i >= 0 ) && ( i <= 2 ) )
      return m_point[i];
   else
      qCritical(  ) << "Wrong index in PMTriangle::point\n";
   return PMVector( 0.0, 0.0, 0.0 );
}

void PMTriangle::setNormal( int i, const PMVector& p )
{
   if( ( i >= 0 ) && ( i <= 2 ) )
   {
      if( p != m_normal[i] )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMNormal0ID + i, m_normal[i] );
         m_normal[i] = p;
         m_normal[i].resize( 3 );
         setViewStructureChanged();
      }
   }
   else
      qCritical(  ) << "Wrong index in PMTriangle::setNormal\n";
}

PMVector PMTriangle::normal( int i ) const
{
   if( ( i >= 0 ) && ( i <= 2 ) )
      return m_normal[i];
   else
      qCritical(  ) << "Wrong index in PMTriangle::normal\n";
   return PMVector( 0.0, 0.0, 0.0 );
}

void PMTriangle::setSmoothTriangle( bool on )
{
   if( on != m_smooth )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMSmoothID, m_smooth );
         m_pMemento->setDescriptionChanged();
      }
      m_smooth = on;
      setViewStructureChanged();
   }
}

PMVector PMTriangle::uvVector( int i ) const
{
   if( i >= 0 && i < 3 )
      return m_uvVector[i];
   else
      qCritical(  ) << "Wrong index in PMTriangle::uvVector\n";
   return PMVector( 0.0, 0.0 );
}

void PMTriangle::setUVVector( int i, const PMVector& v )
{
   if( i >= 0 && i < 3 )
   {
      if( v != m_uvVector[i] )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMUVVector0ID + i, m_uvVector[i] );
         m_uvVector[i] = v;
         m_uvVector[i].resize( 2 );
      }
   }
   else
      qCritical(  ) << "Wrong index in PMTriangle::setNormal\n";
}

void PMTriangle::enableUV( bool yes )
{
   if( yes != m_uvEnabled )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUVEnabledID, m_uvEnabled );
      m_uvEnabled = yes;
   }
}

PMDialogEditBase* PMTriangle::editWidget( QWidget* parent ) const
{
   return new PMTriangleEdit( parent );
}

void PMTriangle::restoreMemento( PMMemento* s )
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
            case PMPoint0ID:
               setPoint( 0, data->vectorData() );
               break;
            case PMPoint1ID:
               setPoint( 1, data->vectorData() );
               break;
            case PMPoint2ID:
               setPoint( 2, data->vectorData() );
               break;
            case PMNormal0ID:
               setNormal( 0, data->vectorData() );
               break;
            case PMNormal1ID:
               setNormal( 1, data->vectorData() );
               break;
            case PMNormal2ID:
               setNormal( 2, data->vectorData() );
               break;
            case PMSmoothID:
               setSmoothTriangle( data->boolData() );
               break;
            case PMUVVector0ID:
               setUVVector( 0, data->vectorData() );
               break;
            case PMUVVector1ID:
               setUVVector( 1, data->vectorData() );
               break;
            case PMUVVector2ID:
               setUVVector( 2, data->vectorData() );
               break;
            case PMUVEnabledID:
               enableUV( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMTriangle::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}


bool PMTriangle::isDefault()
{
   if( ( m_point[0] == point0Default )
       && ( m_point[1] == point1Default )
       && ( m_point[2] == point2Default ) )
      return true;
   return false;
}

void PMTriangle::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
   }

   PMPointArray& points = m_pViewStructure->points();

   points[0] = m_point[0];
   points[1] = m_point[1];
   points[2] = m_point[2];
}

PMViewStructure* PMTriangle::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure )
   {
      s_pDefaultViewStructure = new PMViewStructure( 3, 3 );
      PMPointArray& points = s_pDefaultViewStructure->points();
      PMLineArray& lines = s_pDefaultViewStructure->lines();

      points[0] = point0Default;
      points[1] = point1Default;
      points[2] = point2Default;

      lines[0] = PMLine( 0, 1 );
      lines[1] = PMLine( 1, 2 );
      lines[2] = PMLine( 0, 2 );
   }
   return s_pDefaultViewStructure;
}

void PMTriangle::controlPoints( PMControlPointList& list )
{
   PM3DControlPoint* cp;

   cp = new PM3DControlPoint( m_point[0], PMPoint0ID,
                              ( "Point 1" ) );
   list.append( cp );
   if( m_smooth )
      list.append( new PMVectorControlPoint( cp, m_normal[0], PMNormal0ID,
                                             ( "Normal 1" ) ) );

   cp = new PM3DControlPoint( m_point[1], PMPoint1ID,
                              ( "Point 2" ) );
   list.append( cp );
   if( m_smooth )
      list.append( new PMVectorControlPoint( cp, m_normal[1], PMNormal1ID,
                                             ( "Normal 2" ) ) );

   cp = new PM3DControlPoint( m_point[2], PMPoint2ID,
                              ( "Point 3" ) );
   list.append( cp );
   if( m_smooth )
      list.append( new PMVectorControlPoint( cp, m_normal[2], PMNormal2ID,
                                             ( "Normal 3" ) ) );
}

void PMTriangle::controlPointsChanged( PMControlPointList& list )
{
   PMVector p0, p1, p2;
   PMVector n0, n1, n2;
   double normalDirection = 1.0;
   PMVector triangleNormal;
   bool validNormal = false;
   double d;
	PMControlPointList::iterator it;

	for( it = list.begin(); it != list.end(); ++it )
   {
      switch( ( *it )->id() )
      {
         case PMPoint0ID:
            p0 = ( ( PM3DControlPoint* ) *it )->point();
            break;
         case PMPoint1ID:
            p1 = ( ( PM3DControlPoint* ) *it )->point();
            break;
         case PMPoint2ID:
            p2 = ( ( PM3DControlPoint* ) *it )->point();
            break;
         case PMNormal0ID:
            n0 = ( ( PMVectorControlPoint* ) *it )->vector();
            break;
         case PMNormal1ID:
            n1 = ( ( PMVectorControlPoint* ) *it )->vector();
            break;
         case PMNormal2ID:
            n2 = ( ( PMVectorControlPoint* ) *it )->vector();
            break;
         default:
            break;
      }
   }

   if( m_smooth )
   {
      triangleNormal = PMVector::cross( m_point[1] - m_point[0],
                                        m_point[2] - m_point[0] );
      normalDirection = PMVector::dot( triangleNormal, m_normal[0] );
      if( approxZero( normalDirection  ) )
         normalDirection = PMVector::dot( triangleNormal, m_normal[1] );
      if( approxZero( normalDirection  ) )
         normalDirection = PMVector::dot( triangleNormal, m_normal[2] );
      if( normalDirection < 0 )
         triangleNormal = -triangleNormal;
      if( !approxZero( triangleNormal.abs() ) )
      {
         validNormal = true;
         triangleNormal /= triangleNormal.abs();
      }
   }

	for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMPoint0ID:
               if( !( p0.approxEqual( p1 ) || p0.approxEqual( p2 ) ) )
                  setPoint( 0, p0 );
               else
                  ( ( PM3DControlPoint* ) *it )->setPoint( m_point[0] );
               break;
            case PMPoint1ID:
               if( !( p1.approxEqual( p0 ) || p1.approxEqual( p2 ) ) )
                  setPoint( 1, p1 );
               else
                  ( ( PM3DControlPoint* ) *it )->setPoint( m_point[1] );
               break;
            case PMPoint2ID:
               if( !( p2.approxEqual( p0 ) || p2.approxEqual( p1 ) ) )
                  setPoint( 2, p2 );
               else
                  ( ( PM3DControlPoint* ) *it )->setPoint( m_point[2] );
               break;

            case PMNormal0ID:
               if( validNormal )
               {
                  d = PMVector::dot( triangleNormal, n0 );
                  if( d > 0 )
                     setNormal( 0, n0 );
                  else
                  {
                     setNormal( 0, n0 - ( d - 1e-5 ) * triangleNormal );
                     ( ( PMVectorControlPoint* ) *it )->setVector( m_normal[0] );
                  }
               }
               else
                  ( ( PMVectorControlPoint* ) *it )->setVector( m_normal[0] );
               break;
            case PMNormal1ID:
               if( validNormal )
               {
                  d = PMVector::dot( triangleNormal, n1 );
                  if( d > 0 )
                     setNormal( 1, n1 );
                  else
                  {
                     setNormal( 1, n1 - ( d - 1e-5 ) * triangleNormal );
                     ( ( PMVectorControlPoint* ) *it )->setVector( m_normal[1] );
                  }
               }
               else
                  ( ( PMVectorControlPoint* ) *it )->setVector( m_normal[1] );
               break;
            case PMNormal2ID:
               if( validNormal )
               {
                  d = PMVector::dot( triangleNormal, n2 );
                  if( d > 0 )
                     setNormal( 2, n2 );
                  else
                  {
                     setNormal( 2, n2 - ( d - 1e-5 ) * triangleNormal );
                     ( ( PMVectorControlPoint* ) *it )->setVector( m_normal[2] );
                  }
               }
               else
                  ( ( PMVectorControlPoint* ) *it )->setVector( m_normal[2] );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMTriangle::controlPointsChanged\n";
               break;
         }
      }
   }
}

void PMTriangle::cleanUp() const
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
