//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMPOLYNOMEXPONENTS_H
#define PMPOLYNOMEXPONENTS_H



#include <QList>

/**
 * Helper class for the @ref PMPolynomBaseEdit widget
 */
class PMPolynomExponents
{
public:
   /**
    * All exponents are initialized with 0
    */
   PMPolynomExponents()
   {
      m_exponents[0] = m_exponents[1] = m_exponents[2] = 0;
   }
   /**
    * The exponents are initialized with x, y, z
    */
   PMPolynomExponents( int x, int y, int z )
   {
      m_exponents[0] = x;
      m_exponents[1] = y;
      m_exponents[2] = z;
   }

   /**
    * Returns the ith exponent
    */
   int exponent( int i ) const { return m_exponents[i]; }
   /**
    * Sets the ith exponent
    */
   void setExponent( int i, int exp ) { m_exponents[i] = exp; }

   /**
    * Returns the x exponent
    */
   int xExponent() const { return m_exponents[0]; }
   /**
    * Sets the x exponent
    */
   void setXExponent( int exp ) { m_exponents[0] = exp; }
   /**
    * Returns the y exponent
    */
   int yExponent() const { return m_exponents[1]; }
   /**
    * Sets the y exponent
    */
   void setYExponent( int exp ) { m_exponents[1] = exp; }
   /**
    * Returns the z exponent
    */
   int zExponent() const { return m_exponents[2]; }
   /**
    * Sets the z exponent
    */
   void setZExponent( int exp ) { m_exponents[2] = exp; }

   bool operator== ( const PMPolynomExponents& e ) const
   {
      return ( m_exponents[0] == e.m_exponents[0] ) &&
             ( m_exponents[1] == e.m_exponents[1] ) &&
             ( m_exponents[2] == e.m_exponents[2] );
   }
   bool operator!= ( const PMPolynomExponents& e ) const
   {
      return !( *this == e );
   }
   PMPolynomExponents& operator= ( const PMPolynomExponents& p )
   {
      m_exponents[0] = p.m_exponents[0];
      m_exponents[1] = p.m_exponents[1];
      m_exponents[2] = p.m_exponents[2];
      return *this;
   }
   friend PMPolynomExponents operator+ ( const PMPolynomExponents& p1,
                                         const PMPolynomExponents& p2 );

   /**
    * Returns the sum of the exponents
    */
   int sum() const { return m_exponents[0] + m_exponents[1] + m_exponents[2]; }

private:
   int m_exponents[3];

   // static stuff
public:
   /**
    * Returns the exponents for a polynom with order n ( 2 <= n <= 7 )
    */
   static QList<PMPolynomExponents>& polynom( int n );
   
private:
   static QList<PMPolynomExponents> //...
   recPolynom( const PMPolynomExponents& base, int xyz, int n, int rem );
   static bool m_created[6];
   static QList<PMPolynomExponents> m_lists[6];
};

#endif
