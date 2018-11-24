/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003-2006 by Andreas Zehender
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

#include "pmmessage.h"

PMMessage::PMMessage()
{
   m_pObject = 0;
}

PMMessage::PMMessage( const QString& text, PMObject* object )
{
   m_sText = text;
   m_pObject = object;
}

PMMessage::PMMessage( const PMMessage& m )
{
   m_sText = m.m_sText;
   m_pObject = m.m_pObject;
}

PMMessage& PMMessage::operator=( const PMMessage& m )
{
   m_sText = m.m_sText;
   m_pObject = m.m_pObject;
   return *this;
}

