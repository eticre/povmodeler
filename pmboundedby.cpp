/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#include "pmboundedby.h"
#include "pmboundedbyedit.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"



PMDefinePropertyClass( PMBoundedBy, PMBoundedByProperty );

PMMetaObject* PMBoundedBy::s_pMetaObject = 0;
PMObject* createNewBoundedBy( PMPart* part )
{
   return new PMBoundedBy( part );
}

PMBoundedBy::PMBoundedBy( PMPart* part )
      : Base( part )
{
}

PMBoundedBy::~PMBoundedBy()
{
}

PMMetaObject* PMBoundedBy::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "BoundedBy", Base::metaObject(),
                                        createNewBoundedBy );
      s_pMetaObject->addProperty(
         new PMBoundedByProperty( "clippedBy", 0, &PMBoundedBy::clippedBy ) );
   }
   return s_pMetaObject;
}

void PMBoundedBy::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMBoundedBy::description() const
{
   return ( "bounded by" );
}

bool PMBoundedBy::clippedBy() const
{
   bool cb = true;
   PMObject* o = firstChild();

   for( ; o && cb; o = o->nextSibling() )
      if( o->type() != "Comment" )
         cb = false;

   return cb;
}

void PMBoundedBy::serialize( QDomElement& e, QDomDocument& doc ) const
{
   // no extra data at the moment
   Base::serialize( e, doc );
}

void PMBoundedBy::readAttributes( const PMXMLHelper& h )
{
   // no extra data at the moment
   Base::readAttributes( h );
}

PMDialogEditBase* PMBoundedBy::editWidget( QWidget* parent ) const
{
   return new PMBoundedByEdit( parent );
}

void PMBoundedBy::childRemoved( PMObject* o )
{
   Base::childRemoved( o );

   // add a dummy change
   if( m_pMemento )
      m_pMemento->addData( s_pMetaObject, PMClippedByID, true );
}

void PMBoundedBy::childAdded( PMObject* o )
{
   Base::childAdded( o );

   if( m_pMemento )
      m_pMemento->addData( s_pMetaObject, PMClippedByID, true );
}
