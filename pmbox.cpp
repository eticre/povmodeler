/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#include "pmbox.h"

#include "pmxmlhelper.h"
#include "pmboxedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"



const double defaultBoxSize = 0.5;
const PMVector corner1Default = PMVector( -defaultBoxSize, -defaultBoxSize, -defaultBoxSize );
const PMVector corner2Default = PMVector( defaultBoxSize, defaultBoxSize, defaultBoxSize );

PMDefinePropertyClass( PMBox, PMBoxProperty );

PMViewStructure* PMBox::s_pDefaultViewStructure = nullptr;
PMMetaObject* PMBox::s_pMetaObject = nullptr;
PMObject* createNewBox( PMPart* part )
{
   return new PMBox( part );
}

PMBox::PMBox( PMPart* part )
      : Base( part )
{
   m_corner1 = corner1Default;
   m_corner2 = corner2Default;
}

PMBox::PMBox( const PMBox& b )
      : Base( b )
{
   m_corner1 = b.m_corner1;
   m_corner2 = b.m_corner2;
}

PMBox::~PMBox()
{
}

QString PMBox::description() const
{
   return ( "box" );
}

void PMBox::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "corner_a", m_corner1.serializeXML() );
   e.setAttribute( "corner_b", m_corner2.serializeXML() );
   Base::serialize( e, doc );
}

void PMBox::readAttributes( const PMXMLHelper& h )
{
   m_corner1 = h.vectorAttribute( "corner_a", corner1Default );
   m_corner2 = h.vectorAttribute( "corner_b", corner2Default );
   Base::readAttributes( h );
}

PMMetaObject* PMBox::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Box", Base::metaObject(),
                                        createNewBox );
      s_pMetaObject->addProperty(
         new PMBoxProperty( "corner1", &PMBox::setCorner1, &PMBox::corner1 ) );
      s_pMetaObject->addProperty(
         new PMBoxProperty( "corner2", &PMBox::setCorner2, &PMBox::corner2 ) );
   }
   return s_pMetaObject;
}

void PMBox::setCorner1( const PMVector& p )
{
   if( p != m_corner1 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCorner1ID, m_corner1 );
      m_corner1 = p;
      m_corner1.resize( 3 );
      setViewStructureChanged();
   }
}

void PMBox::setCorner2( const PMVector& p )
{
   if( p != m_corner2 )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCorner2ID, m_corner2 );
      m_corner2 = p;
      m_corner2.resize( 3 );
      setViewStructureChanged();
   }
}

PMDialogEditBase* PMBox::editWidget( QWidget* parent ) const
{
   return new PMBoxEdit( parent );
}

void PMBox::restoreMemento( PMMemento* s )
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
            case PMCorner1ID:
               setCorner1( data->vectorData() );
               break;
            case PMCorner2ID:
               setCorner2( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMBox::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}


bool PMBox::isDefault()
{
   if( ( m_corner1 == corner1Default ) && ( m_corner2 == corner2Default ) )
      return true;
   return false;
}

void PMBox::createViewStructure()
{
   if( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
   }

   PMPointArray& points = m_pViewStructure->points();

   points[0][0] = m_corner1[0];
   points[0][1] = m_corner1[1];
   points[0][2] = m_corner1[2];

   points[1][0] = m_corner2[0];
   points[1][1] = m_corner1[1];
   points[1][2] = m_corner1[2];

   points[2][0] = m_corner2[0];
   points[2][1] = m_corner1[1];
   points[2][2] = m_corner2[2];

   points[3][0] = m_corner1[0];
   points[3][1] = m_corner1[1];
   points[3][2] = m_corner2[2];

   points[4][0] = m_corner1[0];
   points[4][1] = m_corner2[1];
   points[4][2] = m_corner1[2];

   points[5][0] = m_corner2[0];
   points[5][1] = m_corner2[1];
   points[5][2] = m_corner1[2];

   points[6][0] = m_corner2[0];
   points[6][1] = m_corner2[1];
   points[6][2] = m_corner2[2];

   points[7][0] = m_corner1[0];
   points[7][1] = m_corner2[1];
   points[7][2] = m_corner2[2];
}

PMViewStructure* PMBox::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure )
   {
      s_pDefaultViewStructure = new PMViewStructure( 8, 12 );
      PMPointArray& points = s_pDefaultViewStructure->points();
      PMLineArray& lines = s_pDefaultViewStructure->lines();

      points[0] = PMPoint( -defaultBoxSize, -defaultBoxSize, -defaultBoxSize );
      points[1] = PMPoint(  defaultBoxSize, -defaultBoxSize, -defaultBoxSize );
      points[2] = PMPoint(  defaultBoxSize, -defaultBoxSize,  defaultBoxSize );
      points[3] = PMPoint( -defaultBoxSize, -defaultBoxSize,  defaultBoxSize );
      points[4] = PMPoint( -defaultBoxSize,  defaultBoxSize, -defaultBoxSize );
      points[5] = PMPoint(  defaultBoxSize,  defaultBoxSize, -defaultBoxSize );
      points[6] = PMPoint(  defaultBoxSize,  defaultBoxSize,  defaultBoxSize );
      points[7] = PMPoint( -defaultBoxSize,  defaultBoxSize,  defaultBoxSize );

      lines[ 0] = PMLine( 0, 1 );
      lines[ 1] = PMLine( 0, 3 );
      lines[ 2] = PMLine( 0, 4 );
      lines[ 3] = PMLine( 1, 2 );
      lines[ 4] = PMLine( 1, 5 );
      lines[ 5] = PMLine( 2, 3 );
      lines[ 6] = PMLine( 2, 6 );
      lines[ 7] = PMLine( 3, 7 );
      lines[ 8] = PMLine( 4, 5 );
      lines[ 9] = PMLine( 4, 7 );
      lines[10] = PMLine( 5, 6 );
      lines[11] = PMLine( 6, 7 );
   }
   return s_pDefaultViewStructure;
}

void PMBox::controlPoints( PMControlPointList& list )
{
   list.append( new PM3DControlPoint( m_corner1, PMCorner1ID,
                                      ( "Corner 1" ) ) );
   list.append( new PM3DControlPoint( m_corner2, PMCorner2ID,
                                      ( "Corner 2" ) ) );
}

void PMBox::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it;
   for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMCorner1ID:
               setCorner1( ( static_cast<PM3DControlPoint*>(*it) )->point() );
               break;
            case PMCorner2ID:
               setCorner2( ( static_cast<PM3DControlPoint*>(*it) )->point() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMBox::controlPointsChanged\n";
               break;
         }
      }
   }
}

void PMBox::cleanUp() const
{
   if( s_pDefaultViewStructure )
      delete s_pDefaultViewStructure;
   s_pDefaultViewStructure = nullptr;
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}
