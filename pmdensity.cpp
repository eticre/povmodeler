/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.tp
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmdensity.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmdensityedit.h"



PMMetaObject* PMDensity::s_pMetaObject = 0;
PMObject* createNewDensity( PMPart* part )
{
   return new PMDensity( part );
}

PMDensity::PMDensity( PMPart* part )
      : Base( part )
{
}

PMDensity::PMDensity( const PMDensity& d )
      : Base( d )
{
}

PMDensity::~PMDensity()
{
}

PMMetaObject* PMDensity::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Density", Base::metaObject(),
                                        createNewDensity );
   }
   return s_pMetaObject;
}

void PMDensity::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMDensity::description() const
{
   return ( "density" );
}


PMDialogEditBase* PMDensity::editWidget( QWidget* parent ) const
{
   return new PMDensityEdit( parent );
}

