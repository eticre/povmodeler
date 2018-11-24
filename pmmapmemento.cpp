/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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

#include "pmmapmemento.h"
#include <QList>

PMMapMemento::PMMapMemento( PMObject* originator )
      : PMMemento( originator )
{
   m_bMapValuesSaved = false;
   m_bRemovedValuesSaved = false;
}

PMMapMemento::~PMMapMemento()
{
}

void PMMapMemento::setMapValues( const QList<double>& v )
{
   if( !m_bMapValuesSaved )
   {
      m_mapValues = v;
      m_bMapValuesSaved = true;
      addChange( PMCData );
   }
}

void PMMapMemento::setRemovedValues( const QList<double>& v )
{
   if( !m_bRemovedValuesSaved )
   {
      m_removedValues = v;
      m_bRemovedValuesSaved = true;
      addChange( PMCData );
   }
}

   
