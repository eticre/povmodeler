//-*-C++-*-
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

#ifndef PMCOLOR_H
#define PMCOLOR_H

#include <qstring.h>
#include <qcolor.h>

class PMVector;

/**
 * Class for povray colors (red, green, blue, filter and transmit amount)
 */
class PMColor
{
public:
   /**
    * Creates a color with all values set to 0.0
    */
   PMColor();
   /**
    * Creates a color
    */
   PMColor( const double red, const double green, const double blue,
            const double filter, const double transmit );
   /**
    * Creates a color with filter and transmit set to 0
    */
   PMColor( const double red, const double green, const double blue );
   /**
    * Creates a color from a @ref PMVector. The vector has to be of size 5
    */
   PMColor( const PMVector& v );
   /**
    * Creates a color from a @ref QColor object.
    * filter and transmit are set to 0
    */
   PMColor( const QColor& c );

   /**
    * Sets the red value
    */
   void setRed( double d ) { m_colorValue[0] = d; }
   /**
    * Returns the red value
    */
   double red() const { return m_colorValue[0]; }
   /**
    * Sets the green value
    */
   void setGreen( double d ) { m_colorValue[1] = d; }
   /**
    * Returns the green value
    */
   double green() const { return m_colorValue[1]; }
   /**
    * Sets the blue value
    */
   void setBlue( double d ) { m_colorValue[2] = d; }
   /**
    * Returns the red value
    */
   double blue() const { return m_colorValue[2]; }
   /**
    * Sets the filter value
    */
   void setFilter( double d ) { m_colorValue[3] = d; }
   /**
    * Returns the filter value
    */
   double filter() const { return m_colorValue[3]; }
   /**
    * Sets the transmit value
    */
   void setTransmit( double d ) { m_colorValue[4] = d; }
   /**
    * Returns the transmit value
    */
   double transmit() const { return m_colorValue[4]; }
   /**
    * Returns the rgb value as QColor
    */
   QColor toQColor() const;
   
   /**
    * Returns a string for serialization
    */
   QString serialize( bool addColorKeyword = false ) const;
   /**
    * Returns a string for xml output
    */
   QString serializeXML() const;
   /**
    * loads the color data from the xml string
    */
   bool loadXML( const QString& str );

   /**
    * Returns true if the colors are equal*/
   bool operator== ( const PMColor& p ) const;
   /**
    * Returns false if the colors are equal
    */
   bool operator!= ( const PMColor& p ) const;
   /**
    * Set as srgb instead rgb
    */
   void setSrgb( bool bb ) { useSrgb = bb; }
   void setUseSrgb ( bool bb ) { m_colorValue[5] = bb; }
   bool getSrgb () const { return useSrgb; }

private:
   /**
    * The color values. Index 0 is red, 1 green, 2 blue, 3 filter, 4 transmit
    */
   double m_colorValue[6];
   bool useSrgb;
   
};

#endif
