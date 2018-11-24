//-*-C++-*-
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


#ifndef PMPALETTEVALUEMEMENTO_H
#define PMPALETTEVALUEMEMENTO_H



#include "pmmemento.h"
#include "pmpalettevalue.h"
#include <QList>


/**
 * Memento for @ref PMImageMap
 */
class PMPaletteValueMemento : public PMMemento
{
public:
   /**
    * Creates a memento for the object originator
    */
   PMPaletteValueMemento( PMObject* originator );
   /**
    * Deletes the memento
    */
   virtual ~PMPaletteValueMemento();

   /**
    * Saves the filter palette values
    */
   void setFilterPaletteValues( const QList<PMPaletteValue>& v );
   /**
    * Returns the filter palette values
    */
   QList<PMPaletteValue> filterPaletteValues() const;
   /**
    * Returns true if the filter palette values were saved
    */
   bool filterPaletteValuesSaved() const { return m_bFilterPaletteValuesSaved; }
   /**
    * Saves the transmit palette values
    */
   void setTransmitPaletteValues( const QList<PMPaletteValue>& v );
   /**
    * Returns the transmit palette values
    */
   QList<PMPaletteValue> transmitPaletteValues() const;
   /**
    * Returns true if the transmit palette values were saved
    */
   bool transmitPaletteValuesSaved() const { return m_bTransmitPaletteValuesSaved; }

private:
   /**
    * The stored values for filter
    */
   QList<PMPaletteValue> m_filterPaletteValues;
   bool m_bFilterPaletteValuesSaved;
   /**
    * The stored values for transmit
    */
   QList<PMPaletteValue> m_transmitPaletteValues;
   bool m_bTransmitPaletteValuesSaved;
};

#endif
