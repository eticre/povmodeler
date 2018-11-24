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


#include "pmrotate.h"
#include "pmrotateedit.h"

#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmrotatecontrolpoint.h"


#include <QtXml/qdom.h>

const PMVector rotateDefault = PMVector( 0, 0, 0 );

PMDefinePropertyClass( PMRotate, PMRotateProperty );

PMMetaObject* PMRotate::s_pMetaObject = nullptr;
PMObject* createNewRotate( PMPart* part )
{
   return new PMRotate( part );
}

PMRotate::PMRotate( PMPart* part )
      : Base( part )
{
}

PMRotate::PMRotate( const PMRotate& r )
      : Base( r )
{
   m_rotate = r.m_rotate;
}

PMRotate::~PMRotate()
{
}

QString PMRotate::description() const
{
   return ( "rotate" );
}

void PMRotate::serialize( QDomElement& e, QDomDocument& /*doc*/ ) const
{
   e.setAttribute( "value", m_rotate.serializeXML() );
}

void PMRotate::readAttributes( const PMXMLHelper& h )
{
   m_rotate = h.vectorAttribute( "value", rotateDefault );
}

PMMetaObject* PMRotate::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Rotate", Base::metaObject(),
                                        createNewRotate );
      s_pMetaObject->addProperty(
         new PMRotateProperty( "rotation", &PMRotate::setRotation, &PMRotate::rotation ) );
   }
   return s_pMetaObject;
}

void PMRotate::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMRotate::setRotation( const PMVector& p )
{
   if( p != m_rotate )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMRotationID, m_rotate );
         m_pMemento->setViewStructureChanged();
      }
      m_rotate = p;
      m_rotate.resize( 3 );
   }
}

PMDialogEditBase* PMRotate::editWidget( QWidget* parent ) const
{
   return new PMRotateEdit( parent );
}

void PMRotate::restoreMemento( PMMemento* s )
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
            case PMRotationID:
               setRotation( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMRotate::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

PMMatrix PMRotate::transformationMatrix() const
{
   return PMMatrix::rotation( deg2Rad( m_rotate[0] ), deg2Rad( m_rotate[1] ),
                              deg2Rad( m_rotate[2] ) );
}

void PMRotate::controlPoints( PMControlPointList& list )
{
   list.append( new PMRotateControlPoint( m_rotate, PMRotationID ) );
}

void PMRotate::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it;

	for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMRotationID:
               setRotation( ( ( PMRotateControlPoint* ) *it )->rotation() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMRotate::controlPointsChanged\n";
               break;
         }
      }
   }
}
