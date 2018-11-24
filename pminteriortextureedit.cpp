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


#include "pminteriortextureedit.h"
#include "pminteriortexture.h"
#include "pmlinkedit.h"

#include <qlayout.h>
#include <qlabel.h>



PMInteriorTextureEdit::PMInteriorTextureEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMInteriorTextureEdit::displayObject( PMObject* o )
{
   if( o->isA( "InteriorTexture" ) )
   {
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMInteriorTextureEdit: Can't display object\n";
}

#include "pminteriortextureedit.moc"
