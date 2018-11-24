/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmskysphere.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmskysphereedit.h"
#include "pmdebug.h"

PMMetaObject* PMSkySphere::s_pMetaObject = 0;
PMObject* createNewSkySphere( PMPart* part )
{
   return new PMSkySphere( part );
}

PMSkySphere::PMSkySphere( PMPart* part )
      : Base( part )
{
}

PMSkySphere::PMSkySphere( const PMSkySphere& s )
      : Base( s )
{
}

PMSkySphere::~PMSkySphere()
{
}

PMMetaObject* PMSkySphere::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "SkySphere", Base::metaObject(),
                                        createNewSkySphere );
   }
   return s_pMetaObject;
}

void PMSkySphere::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMSkySphere::description() const
{
   return ( "skysphere" );
}

PMDialogEditBase* PMSkySphere::editWidget( QWidget* parent ) const
{
   return new PMSkySphereEdit( parent );
}

