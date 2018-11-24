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

#include "pmpolynomexponents.h"
#include "pmdebug.h"
#include <QList>

bool PMPolynomExponents::m_created[6] = { false, false, false,
                                          false, false, false };
QList<PMPolynomExponents> PMPolynomExponents::m_lists[6];

PMPolynomExponents operator+ ( const PMPolynomExponents& p1,
                               const PMPolynomExponents& p2 )
{
   return PMPolynomExponents( p1.m_exponents[0] + p2.m_exponents[0],
                              p1.m_exponents[1] + p2.m_exponents[1],
                              p1.m_exponents[2] + p2.m_exponents[2] );
}

QList<PMPolynomExponents>& PMPolynomExponents::polynom( int n )
{
   if( ( n < 2 ) || ( n > 7 ) )
   {
      n = 2;
      qCritical(  ) << "Wrong polynom order in PMPolynomExponents::polynom()\n";
   }

   if( !m_created[n-2] )
   {
      m_lists[n-2] = recPolynom( PMPolynomExponents( 0, 0, 0 ), 0, n, 0 );
      m_created[n-2] = true;
//      qDebug() << "Polynom n: " << n << " size: " << m_lists[n-2].count();
   }

   return m_lists[n-2];
}

QList<PMPolynomExponents>
PMPolynomExponents::recPolynom( const PMPolynomExponents& base,
                               int xyz, int n, int rem )
{
   QList<PMPolynomExponents> res;

   if( n >= 0 )
   {
      if( ( ( rem + n ) == 0 ) || ( xyz > 2 ) )
         res.append( base );
      else
      {
         PMPolynomExponents newBase = base;
         newBase.setExponent( xyz, n );
         res += recPolynom( newBase, xyz + 1, rem, 0 );
         res += recPolynom( base, xyz, n - 1, rem + 1 );
      }
   }
   
   return res;
}
