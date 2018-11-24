/*
**************************************************************************
                                 description
                             -------------------
    and                  : (C) 2002 by Andreas Zehender
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

#include "pmlookslike.h"

#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmnamedobjectedit.h"



PMMetaObject* PMLooksLike::s_pMetaObject = 0;
PMObject* createNewLooksLike( PMPart* part )
{
   return new PMLooksLike( part );
}

PMLooksLike::PMLooksLike( PMPart* part )
      : Base( part )
{
}

PMLooksLike::PMLooksLike( const PMLooksLike& l )
      : Base( l )
{
}

PMLooksLike::~PMLooksLike()
{
}


QString PMLooksLike::description() const
{
   return ( "looks like" );
}

PMMetaObject* PMLooksLike::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "LooksLike", Base::metaObject(),
                                        createNewLooksLike );
      // no properties
   }
   return s_pMetaObject;
}

void PMLooksLike::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMLooksLike::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
}

void PMLooksLike::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
}

PMDialogEditBase* PMLooksLike::editWidget( QWidget* parent ) const
{
   return new PMNamedObjectEdit( parent );
}

void PMLooksLike::restoreMemento( PMMemento* s )
{
   Base::restoreMemento( s );
}

