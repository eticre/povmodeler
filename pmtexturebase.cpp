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


#include "pmtexturebase.h"
#include "pmdeclare.h"
#include "pmpart.h"
#include "pmsymboltable.h"
#include "pmtexturebaseedit.h"
#include "pmparser.h"

#include "pmmemento.h"
#include "pmxmlhelper.h"



PMDefinePropertyClass( PMTextureBase, PMTextureBaseProperty );

PMMetaObject* PMTextureBase::s_pMetaObject = 0;

PMTextureBase::PMTextureBase( PMPart* part )
      : Base( part )
{
   m_pLinkedObject = 0;
}

PMTextureBase::PMTextureBase( const PMTextureBase& b )
      : Base( b )
{
   m_pLinkedObject = 0;
   setLinkedObject( b.m_pLinkedObject );
}

PMTextureBase::~PMTextureBase()
{
}

PMMetaObject* PMTextureBase::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "TextureBase", Base::metaObject() );
      s_pMetaObject->addProperty(
         new PMTextureBaseProperty( "linkedObject", &PMTextureBase::setLinkedObjectProperty,
                         &PMTextureBase::linkedObjectProperty ) );
      s_pMetaObject->addProperty(
         new PMTextureBaseProperty( "hasLinkedObject", 0, &PMTextureBase::hasLinkedObject ) );
   }
   return s_pMetaObject;
}

void PMTextureBase::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

bool PMTextureBase::setLinkedObject( PMDeclare* obj )
{
   if( obj )
   {
      if( obj->declareType() == type() )
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

void PMTextureBase::setLinkedObjectProperty( PMObject* o )
{
   if( o == 0 )
      setLinkedObject( 0 );
   else if( o->isA( "Declare" ) )
      setLinkedObject( ( PMDeclare* ) o );
}

void PMTextureBase::serialize( QDomElement& e, QDomDocument& doc ) const
{
   if( m_pLinkedObject )
      e.setAttribute( "prototype", m_pLinkedObject->id() );
   Base::serialize( e, doc );
}

void PMTextureBase::readAttributes( const PMXMLHelper& h )
{
   QString id = h.stringAttribute( "prototype", "" );
   if( !id.isEmpty() )
   {
      PMDeclare* link = h.parser()->checkLink( id );
      if( link )
      {
         if( link->declareType() == type() )
         {
            m_pLinkedObject = link;
            m_pLinkedObject->addLinkedObject( this );
         }
         else
            h.parser()->printError( "Declare " + id + " has wrong type."  );
      }
   }
   Base::readAttributes( h );
}

void PMTextureBase::restoreMemento( PMMemento* s )
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
               qCritical(  ) << "Wrong ID in PMTextureBase::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
