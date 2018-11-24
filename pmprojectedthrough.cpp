/*
**************************************************************************
                                 description
                             -------------------
    and                  : (C) 2003 by Leon Pennington
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

#include "pmprojectedthrough.h"

#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmnamedobjectedit.h"



PMMetaObject* PMProjectedThrough::s_pMetaObject = nullptr;
PMObject* createNewProjectedThrough( PMPart* part )
{
   return new PMProjectedThrough( part );
}

PMProjectedThrough::PMProjectedThrough( PMPart* part )
      : Base( part )
{
}

PMProjectedThrough::PMProjectedThrough( const PMProjectedThrough& l )
      : Base( l )
{
}

PMProjectedThrough::~PMProjectedThrough()
{
}


QString PMProjectedThrough::description() const
{
   return ( "projected through" );
}

PMMetaObject* PMProjectedThrough::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "ProjectedThrough", Base::metaObject(),
                                        createNewProjectedThrough );
      // no properties
   }
   return s_pMetaObject;
}

void PMProjectedThrough::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMProjectedThrough::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
}

void PMProjectedThrough::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
}

PMDialogEditBase* PMProjectedThrough::editWidget( QWidget* parent ) const
{
   return new PMNamedObjectEdit( parent );
}

void PMProjectedThrough::restoreMemento( PMMemento* s )
{
   Base::restoreMemento( s );
}

