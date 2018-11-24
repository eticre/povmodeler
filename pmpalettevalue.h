//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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


#ifndef PMPALETTEVALUE_H
#define PMPALETTEVALUE_H



#include "pmnamedobject.h"

/**
 * Class for pallete values used for filter and transmit in image maps.
 */ 
class PMPaletteValue
{
public:
   /** */
   PMPaletteValue();
   /** */
   PMPaletteValue( int idx, double value );
   /**
    * Returns the index of the palette entry.
    */
   int index() const { return m_index; } 
   /**
    * Returns the value of the palette entry.
    */
   double value() const { return m_value; }
   /**
    * Sets the index of the palette entry.
    */
   void setIndex( int idx );
   /**
    * Sets the value of the palette entry.
    */
   void setValue( double val );
   /**
    * Serializes the palette entry into a XML document.
    */
   void serialize( QDomElement& e, QDomDocument& doc ) const;
   /**
    * Reads the palette entry from a XML document.
    */
   void readAttributes( const QDomElement& h );

   /**
    * Returns true if both values have the same index and value
    */
   bool operator== ( const PMPaletteValue& p) const;
   /**
    * Returns false if both values have the same index and value
    */
   bool operator!= ( const PMPaletteValue& p) const;

private:
   int m_index;
   double m_value;
};

#endif
