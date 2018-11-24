/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#include "pmmaterialedit.h"
#include "pmmaterial.h"
#include "pmlinkedit.h"

#include <qlayout.h>
#include <qlabel.h>



PMMaterialEdit::PMMaterialEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMMaterialEdit::displayObject( PMObject* o )
{
   if( o->isA( "Material" ) )
   {
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMMaterialEdit: Can't display object\n";
}

#include "pmmaterialedit.moc"
