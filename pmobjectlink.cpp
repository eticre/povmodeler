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


#include "pmobjectlink.h"
#include "pmdeclare.h"
#include "pmpart.h"
#include "pmsymboltable.h"
#include "pmobjectlinkedit.h"
#include "pmparser.h"

#include "pmmemento.h"
#include "pmxmlhelper.h"



PMDefinePropertyClass( PMObjectLink, PMObjectLinkProperty );

PMMetaObject* PMObjectLink::s_pMetaObject = 0;
PMObject* createNewObjectLink( PMPart* part )
{
   return new PMObjectLink( part );
}

PMObjectLink::PMObjectLink( PMPart* part )
      : Base( part )
{
   m_pLinkedObject = 0;
}

PMObjectLink::PMObjectLink( const PMObjectLink& o )
      : Base( o )
{
   m_pLinkedObject = 0;
   setLinkedObject( o.m_pLinkedObject );
}

PMObjectLink::~PMObjectLink()
{
}

QString PMObjectLink::description() const
{
   return ( "object link" );
}

PMMetaObject* PMObjectLink::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "ObjectLink", Base::metaObject(),
                                        createNewObjectLink );
      s_pMetaObject->addProperty(
         new PMObjectLinkProperty( "linkedObject", &PMObjectLink::setLinkedObjectProperty,
                         &PMObjectLink::linkedObjectProperty ) );
   }
   return s_pMetaObject;
}

void PMObjectLink::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

bool PMObjectLink::setLinkedObject( PMDeclare* obj )
{
   if( obj )
   {
      if( obj->declareType() == "GraphicalObject" ||
          obj->declareType() == "Light" )
      {
         if( m_pLinkedObject != obj )
         {
            if( m_pMemento )
            {
               m_pMemento->addData( s_pMetaObject, PMLinkedObjectID,
                                    m_pLinkedObject );
               m_pMemento->setViewStructureChanged();
            }

            if( m_pLinkedObject )
            {
               m_pLinkedObject->removeLinkedObject( this );
               if( m_pMemento )
                  m_pMemento->addChangedObject( m_pLinkedObject, PMCData );
            }
            m_pLinkedObject = obj;
            m_pLinkedObject->addLinkedObject( this );
            if( m_pMemento )
               m_pMemento->addChangedObject( m_pLinkedObject, PMCData );
         }
         return true;
      }
      else
         return false;
   }
   else
   {
      if( m_pLinkedObject != 0 )
      {
         if( m_pMemento )
         {
            m_pMemento->addData( s_pMetaObject, PMLinkedObjectID,
                                 m_pLinkedObject );
            m_pMemento->addChangedObject( m_pLinkedObject, PMCData );
         }
         m_pLinkedObject->removeLinkedObject( this );
         m_pLinkedObject = 0;
      }
      return true;
   }
   return false;
}

void PMObjectLink::setLinkedObjectProperty( PMObject* o )
{
   if( o == 0 )
      setLinkedObject( 0 );
   else if( o->isA( "Declare" ) )
      setLinkedObject( ( PMDeclare* ) o );
}

void PMObjectLink::serialize( QDomElement& e, QDomDocument& doc ) const
{
   if( m_pLinkedObject )
      e.setAttribute( "prototype", m_pLinkedObject->id() );
   Base::serialize( e, doc );
}

void PMObjectLink::readAttributes( const PMXMLHelper& h )
{
   QString id = h.stringAttribute( "prototype", "" );
   if( !id.isEmpty() )
   {
      PMDeclare* link = h.parser()->checkLink( id );
      if( link )
      {
         if( link->type() == "Declare" )
         {
            m_pLinkedObject = ( PMDeclare* ) link;
            m_pLinkedObject->addLinkedObject( this );
         }
         else
            h.parser()->printError( ( "Declare " + id + " has wrong type." ) );
      }
   }
   Base::readAttributes( h );
}

PMDialogEditBase* PMObjectLink::editWidget( QWidget* parent ) const
{
   return new PMObjectLinkEdit( parent );
}

void PMObjectLink::restoreMemento( PMMemento* s )
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
            case PMLinkedObjectID:
               setLinkedObject( ( PMDeclare* ) data->objectData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMObjectLink::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
