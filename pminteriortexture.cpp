/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
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


#include "pminteriortexture.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pminteriortextureedit.h"



PMMetaObject* PMInteriorTexture::s_pMetaObject = 0;
PMObject* createNewInteriorTexture( PMPart* part )
{
   return new PMInteriorTexture( part );
}

PMInteriorTexture::PMInteriorTexture( PMPart* part )
      : Base( part )
{
}

PMInteriorTexture::PMInteriorTexture( const PMInteriorTexture& t )
      : Base( t )
{
}

PMInteriorTexture::~PMInteriorTexture()
{
}

PMMetaObject* PMInteriorTexture::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "InteriorTexture", Base::metaObject(),
                                        createNewInteriorTexture );
   }
   return s_pMetaObject;
}

void PMInteriorTexture::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMInteriorTexture::description() const
{
   return ( "interior texture" );
}

PMDialogEditBase* PMInteriorTexture::editWidget( QWidget* parent ) const
{
   return new PMInteriorTextureEdit( parent );
}

