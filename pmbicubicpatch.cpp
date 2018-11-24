/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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


#include "pmbicubicpatch.h"

#include "pmxmlhelper.h"
#include "pmbicubicpatchedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"
#include "pmmath.h"



const double c_defaultPatchSize = 6.0;
const int c_defaultPatchType = 0;
const int c_defaultPatchUSteps = 3;
const int c_defaultPatchVSteps = 3;
const double c_defaultPatchFlatness = 0;
const PMVector c_defaultUVVector0 = PMVector( 0.0, 0.0 );
const PMVector c_defaultUVVector1 = PMVector( 1.0, 0.0 );
const PMVector c_defaultUVVector2 = PMVector( 1.0, 1.0 );
const PMVector c_defaultUVVector3 = PMVector( 0.0, 1.0 );

PMDefinePropertyClass( PMBicubicPatch, PMBicubicPatchProperty );

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
      if( index < 0 || index > 15 )
         qCritical(  ) << "Illegal index in PMBicubicPatch::PointProperty::setIndex" << endl;
      else
         m_index = index;
   }
   virtual int size( PMObject* /*object*/, int /*dimension*/ ) const
   {
      return 16;
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& v )
   {
      PMBicubicPatch* p = ( PMBicubicPatch* ) obj;
      p->setControlPoint( m_index, v.vectorData() );
      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      const PMBicubicPatch* p = ( const PMBicubicPatch* ) obj;
      return PMVariant( p->controlPoint( m_index ) );
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
      if( index < 0 || index > 3 )
         qCritical() << "Illegal index in PMBicubicPatch::UVVectorProperty::setIndex" << endl;
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
      PMBicubicPatch* p = static_cast<PMBicubicPatch*>(obj);
      p->setUVVector( m_index, v.vectorData() );
      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      const PMBicubicPatch* p = ( const PMBicubicPatch* ) obj;
      return PMVariant( p->uvVector( m_index ) );
   }

private:
   int m_index;
};

PMMetaObject* PMBicubicPatch::s_pMetaObject = nullptr;
PMObject* createNewBicubicPatch( PMPart* part )
{
   return new PMBicubicPatch( part );
}

PMBicubicPatch::PMBicubicPatch( PMPart* part )
      : Base( part )
{
   int x, z;
   double o = -c_defaultPatchSize / 2.0, s = c_defaultPatchSize / 3.0;
   m_patchType = c_defaultPatchType;
   m_numUSteps = c_defaultPatchUSteps;
   m_numVSteps = c_defaultPatchVSteps;
   m_flatness = c_defaultPatchFlatness;
   for( x = 0; x < 4; x++ )
      for( z = 0; z < 4; z++ )
         m_point[x+z*4] = PMVector( o + s * x, 0, o + s * z );
   m_vsUSteps = 0;
   m_vsVSteps = 0;
   m_uvEnabled = false;
   m_uvVectors[0] = c_defaultUVVector0;
   m_uvVectors[1] = c_defaultUVVector1;
   m_uvVectors[2] = c_defaultUVVector2;
   m_uvVectors[3] = c_defaultUVVector3;
}

PMBicubicPatch::PMBicubicPatch( const PMBicubicPatch& p )
      : Base( p )
{
   int i;
   m_patchType = p.m_patchType;
   m_numUSteps = p.m_numUSteps;
   m_numVSteps = p.m_numVSteps;
   m_flatness = p.m_flatness;
   for( i = 0; i < 16; i++ )
      m_point[i] = p.m_point[i];
   m_vsUSteps = 0;
   m_vsVSteps = 0;
   m_uvEnabled = p.m_uvEnabled;
   for( i = 0; i < 4; ++i )
      m_uvVectors[i] = p.m_uvVectors[i];
}

PMBicubicPatch::~PMBicubicPatch()
{
}

QString PMBicubicPatch::description() const
{
   return ( "bicubic patch" );
}

void PMBicubicPatch::serialize( QDomElement& e, QDomDocument& doc ) const
{
   int i;

   e.setAttribute( "type", m_patchType );
   e.setAttribute( "flatness", m_flatness );
   e.setAttribute( "uSteps", m_numUSteps );
   e.setAttribute( "vSteps", m_numVSteps );
   e.setAttribute( "uvEnabled", m_uvEnabled );

   for( i = 0; i < 16; i++ )
      e.setAttribute( QString( "cp%1" ).arg( i ), m_point[i].serializeXML() );

   for( i = 0; i < 4; ++i )
      e.setAttribute( QString( "uv%1" ).arg( i ), m_uvVectors[i].serializeXML() );

   Base::serialize( e, doc );
}

void PMBicubicPatch::readAttributes( const PMXMLHelper& h )
{
   int u, v;
   double o = -c_defaultPatchSize / 2.0, s = c_defaultPatchSize / 3.0;

   m_patchType = h.intAttribute( "type", c_defaultPatchType );
   m_flatness = h.doubleAttribute( "flatness", c_defaultPatchFlatness );
   m_numUSteps = h.intAttribute( "uSteps", c_defaultPatchUSteps );
   m_numVSteps = h.intAttribute( "vSteps", c_defaultPatchVSteps );
   m_uvEnabled = h.boolAttribute( "uvEnabled", m_uvEnabled );

   for( v = 0; v < 4; v++ )
      for( u = 0; u < 4; u++ )
         m_point[u+v*4] = h.vectorAttribute( QString( "cp%1" ).arg( u+v*4 ),
                                      PMVector( o + s * u, 0, o + s * v ) );

   m_uvVectors[0] = h.vectorAttribute( "uv0", c_defaultUVVector0 );
   m_uvVectors[1] = h.vectorAttribute( "uv1", c_defaultUVVector1 );
   m_uvVectors[2] = h.vectorAttribute( "uv2", c_defaultUVVector2 );
   m_uvVectors[3] = h.vectorAttribute( "uv3", c_defaultUVVector3 );

   Base::readAttributes( h );
}

PMMetaObject* PMBicubicPatch::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "BicubicPatch", Base::metaObject(),
                                        createNewBicubicPatch );
      s_pMetaObject->addProperty(
         new PMBicubicPatchProperty( "patchType", &PMBicubicPatch::setPatchType,
                         &PMBicubicPatch::patchType ) );
      s_pMetaObject->addProperty(
         new PMBicubicPatchProperty( "uSteps", &PMBicubicPatch::setUSteps,
                         &PMBicubicPatch::uSteps ) );
      s_pMetaObject->addProperty(
         new PMBicubicPatchProperty( "vSteps", &PMBicubicPatch::setVSteps,
                         &PMBicubicPatch::vSteps ) );
      s_pMetaObject->addProperty(
         new PMBicubicPatchProperty( "flatness", &PMBicubicPatch::setFlatness,
                         &PMBicubicPatch::flatness ) );
      s_pMetaObject->addProperty(
         new PMBicubicPatchProperty( "uvEnabled", &PMBicubicPatch::enableUV,
                         &PMBicubicPatch::isUVEnabled ) );
      s_pMetaObject->addProperty( new PMPointProperty() );
      s_pMetaObject->addProperty( new PMUVVectorProperty() );
   }
   return s_pMetaObject;
}

void PMBicubicPatch::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMBicubicPatch::setPatchType( int patchType )
{
   if( ( patchType == 0 ) || ( patchType == 1 ) )
   {
      if( patchType != m_patchType )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMTypeID, m_patchType );
         m_patchType = patchType;
      }
   }
   else
      qCritical(  ) << "Wrong type in PMBicubicPatch::setPatchType()\n";
}

void PMBicubicPatch::setFlatness( double flatness )
{
   if( flatness >= 0.0 )
   {
      if( flatness != m_flatness )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMFlatnessID, m_flatness );
         m_flatness = flatness;
      }
   }
   else
      qCritical(  ) << "Flatness has to be >= 0 in PMBicubicPatch::setFlatness()\n";
}

void PMBicubicPatch::setUSteps( int steps )
{
   if( steps >= 0 )
   {
      if( steps != m_numUSteps )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMUStepsID, m_numUSteps );
         m_numUSteps = steps;
         setViewStructureChanged();
      }
   }
   else
      qCritical(  ) << "uSteps has to be >= 0 in PMBicubicPatch::setUSteps()\n";
}

void PMBicubicPatch::setVSteps( int steps )
{
   if( steps >= 0 )
   {
      if( steps != m_numVSteps )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMVStepsID, m_numVSteps );
         m_numVSteps = steps;
         setViewStructureChanged();
      }
   }
   else
      qCritical(  ) << "vSteps has to be >= 0 in PMBicubicPatch::setVSteps()\n";
}

void PMBicubicPatch::setControlPoint( int i, const PMVector& p )
{
   if( ( i >= 0 ) && ( i <= 15 ) )
   {
      if( p != m_point[i] )
      {
         if( m_pMemento )
            m_pMemento->addData( s_pMetaObject, PMCP0ID + i, m_point[i] );
         m_point[i] = p;
         setViewStructureChanged();
      }
   }
   else
      qCritical(  ) << "Wrong index in PMBicubicPatch::setControlPoint()\n";
}

PMVector PMBicubicPatch::controlPoint( int i ) const
{
   if( ( i >= 0 ) && ( i <= 15 ) )
      return m_point[i];
   else
      qCritical(  ) << "Wrong index in PMBicubicPatch::controlPoint()\n";
   return PMVector( 0, 0, 0 );
}

void PMBicubicPatch::enableUV( bool yes )
{
   if( yes != m_uvEnabled )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUVEnabledID, m_uvEnabled );
      m_uvEnabled = yes;
   }
}

void PMBicubicPatch::setUVVector( int i, const PMVector& v )
{
   if ( i >= 0 && i < 4 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUV0ID + i, m_uvVectors[i] );
      m_uvVectors[i] = v;
      m_uvVectors[i].resize( 2 );
   }
   else
      qCritical(  ) << "Wrong index in PMBicubicPatch::setUVVector\n";
}

PMVector PMBicubicPatch::uvVector( int i ) const
{
   if( i >= 0 && i < 4 )
      return m_uvVectors[i];
   else
      qCritical(  ) << "Wrong index in PMBicubicPatch::uvVector\n";
   return PMVector( 0.0, 0.0 );
}

PMDialogEditBase* PMBicubicPatch::editWidget( QWidget* parent ) const
{
   return new PMBicubicPatchEdit( parent );
}

void PMBicubicPatch::restoreMemento( PMMemento* s )
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
            case PMTypeID:
               setPatchType( data->intData() );
               break;
            case PMFlatnessID:
               setFlatness( data->doubleData() );
               break;
            case PMUStepsID:
               setUSteps( data->intData() );
               break;
            case PMVStepsID:
               setVSteps( data->intData() );
               break;
            case PMUVEnabledID:
               enableUV( data->boolData() );
               break;
            default:
               if( ( data->valueID() >= PMCP0ID ) && ( data->valueID() <= PMCP15ID ) )
                  setControlPoint( data->valueID() - PMCP0ID, data->vectorData() );
               else if ( data->valueID() >= PMUV0ID && data->valueID() <= PMUV3ID )
                  setUVVector( data->valueID() - PMUV0ID, data->vectorData() );
               else
                  qCritical(  ) << "Wrong ID in PMBicubicPatch::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMBicubicPatch::createViewStructure()
{
   int u, v, i, j;
   int uSteps = m_numUSteps, vSteps = m_numVSteps;
   if( uSteps > 5 ) uSteps = 5;
   if( vSteps > 5 ) vSteps = 5;
   if( uSteps < 0 ) uSteps = 0;
   if( vSteps < 0 ) vSteps = 0;

   // bugfix: Swap u and v
   int segmentsU = pmpot( 2, vSteps );
   int segmentsV = pmpot( 2, uSteps );

   int np = ( segmentsU + 1 ) * ( segmentsV + 1 );
   int nl = segmentsU * ( segmentsV + 1 ) + ( segmentsU + 1 ) * segmentsV;

   int offset = 0;

   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( np, nl );
      m_vsUSteps = uSteps + 1;
   }
   else
   {
      if( m_pViewStructure->points().size() != np )
         m_pViewStructure->points().resize( np );
      if( m_pViewStructure->lines().size() != nl )
         m_pViewStructure->lines().resize( nl );
   }

   if( ( m_vsUSteps != uSteps ) || ( m_vsVSteps != vSteps ) )
   {
      PMLineArray& lines = m_pViewStructure->lines();
      int poffset = 0;
      for( v = 0; v < ( segmentsV + 1 ); v++ )
      {
         for( u = 0; u < segmentsU; u++ )
         {
            lines[offset + u] = PMLine( poffset, poffset + 1 );
            poffset++;
         }
         poffset++;
         offset += segmentsU;
      }
      poffset = 0;
      for( v = 0; v < segmentsV; v++ )
      {
         for( u = 0; u < ( segmentsU + 1 ); u++ )
         {
            lines[offset + u] = PMLine( poffset, poffset + segmentsU + 1 );
            poffset++;
         }
         offset += segmentsU + 1;
      }
      m_vsUSteps = uSteps;
      m_vsVSteps = vSteps;
   }

   PMPointArray& points = m_pViewStructure->points();

   offset = 0;
   double incU = 1.0 / segmentsU;
   double incV = 1.0 / segmentsV;

   PMVector* hp[4];
   for( v = 0; v < 4; v++ )
      hp[v] = new PMVector[segmentsU+1];

   PMVector tp[4];

   double cu, cv;

   // points in u direction
   for( v = 0; v < 4; v++ )
   {
      for( u = 1; u < segmentsU; u++ )
      {
         cu = u * incU;

         for( i = 0; i < 4; i++ )
            tp[i] = m_point[v*4+i];
         for( i = 3; i > 0; i-- )
            for( j = 0; j < i; j++ )
               tp[j] = tp[j] * ( 1 - cu ) + tp[j+1] * cu;
         hp[v][u] = tp[0];
      }
      hp[v][0] = m_point[v*4];
      hp[v][segmentsU] = m_point[v*4+3];
   }

   for( v = 0; v <= segmentsV; v++ )
   {
      cv = v * incV;
      for( u = 0; u <= segmentsU; u++ )
      {
         for( i = 0; i < 4; i++ )
            tp[i] = hp[i][u];
         for( i = 3; i > 0; i-- )
            for( j = 0; j < i; j++ )
               tp[j] = tp[j] * ( 1 - cv ) + tp[j+1] * cv;
         points[offset] = tp[0];
         offset++;
      }
   }

   for( v = 0; v < 4; v++ )
      delete[] hp[v];
}

void PMBicubicPatch::controlPoints( PMControlPointList& list )
{
   int u, v;
   for( v = 0; v < 4; v++ )
      for( u = 0; u < 4; u++ )
         list.append( new PM3DControlPoint( m_point[u+v*4], u+v*4,
                                            ( "Point " + QString::number( u ) + "," + QString::number( v ) ) ) );
}

void PMBicubicPatch::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it;
	for( it = list.begin(); it != list.end(); ++it )
	{
      if( ( *it )->changed() )
      {
         setControlPoint( ( *it )->id(), ( *it )->position() );
      }
   }
}
