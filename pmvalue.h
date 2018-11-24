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


#ifndef PMVALUE_H
#define PMVALUE_H



#include "pmvector.h"

enum PMValueType { PMVFloat, PMVVector, PMVColor };

/**
 * Helper class for parsing numeric expressions because wo don't know
 * at begin of an expression, which type it is.
 *
 * Colors are stored as 5D vectors.
 */

class PMValue
{
public:
   /**
    * Creates a PMValue with type PMVFloat and value 0.0
    */
   PMValue() : m_v( 0 ) { m_d = 0.0; m_type = PMVFloat; }
   /**
    * Copy constructor
    */
   PMValue( const PMValue& v )
   {
      m_type = v.m_type;
      m_d = v.m_d;
      m_v = v.m_v;
   }
   /**
    * Returns the type of the value.
    * Values can be PMVFloat, PMVVector, PMVColor
    */
   PMValueType type() const { return m_type; }

   /**
    * Sets the float value and sets the type to PMVFloat
    */
   void setFloat( const double d ) { m_d = d; m_type = PMVFloat; }
   /**
    * Sets the vector value and sets the type to PMVVector
    */
   void setVector( const PMVector& v ) { m_v = v; m_type = PMVVector; }
   /**
    * Sets the color value and sets the type to PMVColor
    */
   void setColor( const PMVector& v ) { m_v = v; m_type = PMVColor; }

   /**
    * Returns the float value
    */
   double floatValue() const { return m_d; }
   /**
    * Returns the vector value
    */
   PMVector vector() const { return m_v; }
   /**
    * Returns the color value
    */
   PMVector color() const { return m_v; }
   /**
    * Assigns v to the value
    */
   PMValue& operator= ( const PMValue& v )
   {
      m_type = v.m_type;
      m_d = v.m_d;
      m_v = v.m_v;
      return *this;
   }

private:
   PMValueType m_type;
   double m_d;
   PMVector m_v;
};

#endif
