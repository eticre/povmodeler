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


// some helper functions

#include "pmmath.h"
#include <math.h>

bool approx( double v1, double v2, double epsilon )
{
   return fabs( v1 - v2 ) < epsilon;
}

bool approxZero( double v, double epsilon )
{
   return fabs( v ) < epsilon;
}

double rad2Deg( double rad )
{
   return rad / M_PI * 180.0;
}

double deg2Rad( double deg )
{
   return deg * M_PI / 180.0;
}

double pmatan( double s, double c )
{
   double a;
   if( approxZero( c ) )
   {
      if( s > 0 )
         a = M_PI_2;
      else
         a = -M_PI_2;
   }
   else
      a = atan2( s, c );
   
   return a;
}

int pmpot( int x, int y )
{
   int i;
   int result = 1;
   for( i = 0; i < y; i++ )
      result *= x;
   return result;
}
