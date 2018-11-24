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


#include "pmsolidobject.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"


PMDefinePropertyClass( PMSolidObject, PMSolidObjectProperty );

PMMetaObject* PMSolidObject::s_pMetaObject = 0;

PMSolidObject::PMSolidObject( PMPart* part )
      : Base( part )
{
   m_inverse = false;
   m_hollow = PMUnspecified;
}

PMSolidObject::PMSolidObject( const PMSolidObject& s )
      : Base( s )
{
   m_inverse = s.m_inverse;
   m_hollow = s.m_hollow;
}

PMSolidObject::~PMSolidObject()
{
}

PMMetaObject* PMSolidObject::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "SolidObject", Base::metaObject() );
      s_pMetaObject->addProperty(
         new PMSolidObjectProperty( "inverse", &PMSolidObject::setInverse, &PMSolidObject::inverse ) );
      s_pMetaObject->addProperty(
         new PMSolidObjectProperty( "hollow", &PMSolidObject::setHollow, &PMSolidObject::hollow ) );
   }
   return s_pMetaObject;
}

void PMSolidObject::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMSolidObject::serialize( QDomElement& e, QDomDocument& doc ) const
{
   switch( m_hollow )
   {
      case PMTrue:
         e.setAttribute( "hollow", "1" );
         break;
      case PMFalse:
         e.setAttribute( "hollow", "0" );
         break;
      case PMUnspecified:
         break;
   }
   e.setAttribute( "inverse", m_inverse );

   Base::serialize( e, doc );
}

void PMSolidObject::readAttributes( const PMXMLHelper& h )
{
   m_hollow = h.threeStateAttribute( "hollow" );
   m_inverse = h.boolAttribute( "inverse", false );

   Base::readAttributes( h );
}

void PMSolidObject::setHollow( PMThreeState h )
{
   if( m_hollow != h )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHollowID, m_hollow );
      m_hollow = h;
   }
}

void PMSolidObject::setInverse( bool yes )
{
   if( m_inverse != yes )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMInverseID, m_inverse );
      m_inverse = yes;
   }
}

void PMSolidObject::restoreMemento( PMMemento* s )
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
            case PMInverseID:
               setInverse( data->boolData() );
               break;
            case PMHollowID:
               setHollow( data->threeStateData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMSolidObject::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
