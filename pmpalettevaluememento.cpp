/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Passos Carvalho
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

#include "pmpalettevaluememento.h"
#include "pmdebug.h"
#include <QList>

PMPaletteValueMemento::PMPaletteValueMemento( PMObject* originator )
      : PMMemento( originator )
{
   m_bFilterPaletteValuesSaved = false;
   m_bTransmitPaletteValuesSaved = false;
}

PMPaletteValueMemento::~PMPaletteValueMemento()
{
}

void PMPaletteValueMemento::setFilterPaletteValues( const QList<PMPaletteValue>& v )
{
   if( !m_bFilterPaletteValuesSaved )
   {
      // Direct assignment does not work with Qt 2.3.x
      // The list will be changed later in a graphical
      // change because QValueList::detach() is called
      // too late!
      // Copy the list by hand.

      QList<PMPaletteValue>::ConstIterator it = v.begin();
      for( ; it != v.end(); ++it )
         m_filterPaletteValues.append( *it );

      m_bFilterPaletteValuesSaved = true;
      addChange( PMCData );
   }
}

QList<PMPaletteValue> PMPaletteValueMemento::filterPaletteValues() const
{
   if( !m_bFilterPaletteValuesSaved )
      qCritical(  ) << "Filter palette values not saved in PMPaletteValueMemento::filterPaletteValues\n";
   
   return m_filterPaletteValues;
}

void PMPaletteValueMemento::setTransmitPaletteValues( const QList<PMPaletteValue>& v )
{
   if( !m_bTransmitPaletteValuesSaved )
   {
      // Direct assignment does not work with Qt 2.3.x
      // The list will be changed later in a graphical
      // change because QValueList::detach() is called
      // too late!
      // Copy the list by hand.

      QList<PMPaletteValue>::ConstIterator it = v.begin();
      for( ; it != v.end(); ++it )
         m_transmitPaletteValues.append( *it );

      m_bTransmitPaletteValuesSaved = true;
      addChange( PMCData );
   }
}

QList<PMPaletteValue> PMPaletteValueMemento::transmitPaletteValues() const
{
   if( !m_bTransmitPaletteValuesSaved )
      qCritical(  ) << "Transmit palette values not saved in PMPaletteValueMemento::transmitPaletteValues\n";
   
   return m_transmitPaletteValues;
}
