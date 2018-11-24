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


#include "pmcsg.h"


#include "pmxmlhelper.h"
#include "pmcsgedit.h"
#include "pmmemento.h"
#include "pmenumproperty.h"

PMDefineEnumPropertyClass( PMCSG, PMCSG::PMCSGType, PMCSGTypeProperty );

PMMetaObject* PMCSG::s_pMetaObject = 0;
PMObject* createNewCSG( PMPart* part )
{
   return new PMCSG( part );
}

PMCSG::PMCSG( PMPart* part )
      : Base( part )
{
   m_type = CSGUnion;
}

PMCSG::PMCSG( PMPart* part, const PMCSGType t )
      : Base( part )
{
   m_type = t;
}

PMCSG::PMCSG( const PMCSG& c )
      : Base( c )
{
   m_type = c.m_type;
}

PMCSG::~PMCSG()
{
}

QString PMCSG::description() const
{
   switch( m_type )
   {
      case CSGUnion:
         return ( "union" );
         break;
      case CSGIntersection:
         return ( "intersection" );
         break;
      case CSGDifference:
         return ( "difference" );
         break;
      case CSGMerge:
         return ( "merge" );
         break;
   }
   return QString( "" );
}

QString PMCSG::pixmap() const
{
   switch( m_type )
   {
      case CSGUnion:
         return QString( "pmunion" );
         break;
      case CSGIntersection:
         return QString( "pmintersection" );
         break;
      case CSGDifference:
         return QString( "pmdifference" );
         break;
      case CSGMerge:
         return QString( "pmmerge" );
         break;
   }
   return QString( "" );
}

void PMCSG::serialize( QDomElement& e, QDomDocument& doc ) const
{
   switch( m_type )
   {
      case CSGUnion:
         e.setAttribute( "csgtype", "union" );
         break;
      case CSGIntersection:
         e.setAttribute( "csgtype", "intersection" );
         break;
      case CSGDifference:
         e.setAttribute( "csgtype", "difference" );
         break;
      case CSGMerge:
         e.setAttribute( "csgtype", "merge" );
         break;
   }

   Base::serialize( e, doc );
}

void PMCSG::readAttributes( const PMXMLHelper& h )
{
   QString str = h.stringAttribute( "csgtype", "union" );
   if( str == "union" )
      m_type = CSGUnion;
   else if( str == "intersection" )
      m_type = CSGIntersection;
   else if( str == "difference" )
      m_type = CSGDifference;
   else
      m_type = CSGMerge;

   Base::readAttributes( h );
}

PMMetaObject* PMCSG::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "CSG", Base::metaObject(),
                                        createNewCSG );
      PMCSGTypeProperty* p = new PMCSGTypeProperty( "csgType", &PMCSG::setCSGType, &PMCSG::csgType );
      p->addEnumValue( "union", CSGUnion );
      p->addEnumValue( "intersection", CSGIntersection );
      p->addEnumValue( "difference", CSGDifference );
      p->addEnumValue( "merge", CSGMerge );
      s_pMetaObject->addProperty( p );
   }
   return s_pMetaObject;
}

void PMCSG::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMCSG::setCSGType( const PMCSGType t )
{
   if( t != m_type )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMTypeID, ( int ) m_type );
         m_pMemento->setDescriptionChanged();
      }
      m_type = t;
   }
}

PMDialogEditBase* PMCSG::editWidget( QWidget* parent ) const
{
   return new PMCSGEdit( parent );
}

void PMCSG::restoreMemento( PMMemento* s )
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
               setCSGType( ( PMCSGType ) data->intData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMCSG::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

