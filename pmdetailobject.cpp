/*
**************************************************************************
                               description
                           --------------------
  copyright            : (C) 2004 by Leon Pennington
  email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/
#include "pmdetailobject.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmdefaults.h"

/** Default local detail value*/
const bool c_defaulGlobalDetail = true;
const int c_defaultLocalDetailLevel = 1;

/** Static global detail */
int PMDetailObject::s_globalDetailLevel = c_defaultDetailObjectGlobalDetailLevel;
int PMDetailObject::s_globalDetailKey = 0;

PMMetaObject* PMDetailObject::s_pMetaObject = nullptr;

PMDefinePropertyClass( PMDetailObject, PMDetailObjectProperty );

PMDetailObject::PMDetailObject( PMPart *part ) : Base( part )
{
   m_globalDetail = c_defaulGlobalDetail;
   m_localDetailLevel = c_defaultLocalDetailLevel;
}

PMDetailObject::PMDetailObject( const PMDetailObject& o ) : Base( o )
{
   m_globalDetail = o.m_globalDetail;
   m_localDetailLevel = o.m_localDetailLevel;
}

PMDetailObject::~PMDetailObject()
{
}

PMMetaObject* PMDetailObject::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "DetailObject", Base::metaObject() );
      s_pMetaObject->addProperty(
            new PMDetailObjectProperty( "globalDetail", &PMDetailObject::setGlobalDetail, &PMDetailObject::globalDetail ) );
      s_pMetaObject->addProperty(
            new PMDetailObjectProperty( "localDetailLevel", &PMDetailObject::setLocalDetailLevel, &PMDetailObject::localDetailLevel ) );
   }
   return s_pMetaObject;
}

void PMDetailObject::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMDetailObject::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "global_detail", m_globalDetail );
   e.setAttribute( "local_detail_level", m_localDetailLevel );
   Base::serialize( e, doc );
}

void PMDetailObject::readAttributes( const PMXMLHelper& h )
{
   m_globalDetail = h.boolAttribute( "global_detail", c_defaulGlobalDetail );
   m_localDetailLevel = h.intAttribute( "local_detail_level", c_defaultLocalDetailLevel );
   Base::readAttributes( h );
}

int PMDetailObject::displayDetail() const
{
   if( m_globalDetail )
      return s_globalDetailLevel;
   else
      return m_localDetailLevel;
}

void PMDetailObject::setGlobalDetail( bool yes )
{
   if( m_globalDetail != yes )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMGlobalDetailID, m_globalDetail );
      m_globalDetail = yes;
      setViewStructureChanged();
   }
}

void PMDetailObject::setLocalDetailLevel( int level )
{
   if( m_localDetailLevel != level )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMLocalDetailLevelID, m_localDetailLevel );
      m_localDetailLevel = level;
      setViewStructureChanged();
   }
}

void PMDetailObject::setGlobalDetailLevel( int level )
{
   if( s_globalDetailLevel != level )
   {
      ++s_globalDetailKey;
      s_globalDetailLevel = level;
   }
}

void PMDetailObject::restoreMemento( PMMemento* s )
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
            case PMGlobalDetailID:
               setGlobalDetail( data->boolData() );
               break;
            case PMLocalDetailLevelID:
               setLocalDetailLevel( data->intData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMDetailObject::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
