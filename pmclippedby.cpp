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


#include "pmclippedby.h"
#include "pmclippedbyedit.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"



PMDefinePropertyClass( PMClippedBy, PMClippedByProperty );

PMMetaObject* PMClippedBy::s_pMetaObject = 0;
PMObject* createNewClippedBy( PMPart* part )
{
   return new PMClippedBy( part );
}

PMClippedBy::PMClippedBy( PMPart* part )
      : Base( part )
{
}

PMClippedBy::~PMClippedBy()
{
}

PMMetaObject* PMClippedBy::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "ClippedBy", Base::metaObject(),
                                        createNewClippedBy );
      s_pMetaObject->addProperty(
         new PMClippedByProperty( "boundedBy", 0, &PMClippedBy::boundedBy ) );
   }
   return s_pMetaObject;
}

void PMClippedBy::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMClippedBy::description() const
{
   return ( "clipped by" );
}

bool PMClippedBy::boundedBy() const
{
   bool cb = true;
   PMObject* o = firstChild();

   for( ; o && cb; o = o->nextSibling() )
      if( o->type() != "Comment" )
         cb = false;

   return cb;
}

void PMClippedBy::serialize( QDomElement& e, QDomDocument& doc ) const
{
   // no extra data at the moment
   Base::serialize( e, doc );
}

void PMClippedBy::readAttributes( const PMXMLHelper& h )
{
   // no extra data at the moment
   Base::readAttributes( h );
}

PMDialogEditBase* PMClippedBy::editWidget( QWidget* parent ) const
{
   return new PMClippedByEdit( parent );
}

void PMClippedBy::childRemoved( PMObject* o )
{
   Base::childRemoved( o );

   // add a dummy change
   if( m_pMemento )
      m_pMemento->addData( s_pMetaObject, PMBoundedByID, true );
}

void PMClippedBy::childAdded( PMObject* o )
{
   Base::childAdded( o );

   if( m_pMemento )
      m_pMemento->addData( s_pMetaObject, PMBoundedByID, true );
}
