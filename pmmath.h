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


#ifndef PMMATH_H
#define PMMATH_H

/**
 * Same as ( fabs( v1 - v2 ) < epsilon )
 */
bool approx( double v1, double v2, double epsilon = 1e-6 );
/**
 * Same as ( fabs( v ) < epsilon )
 */
bool approxZero( double v, double epsilon = 1e-6 );
/**
 * Converts rad to deg
 */
double rad2Deg( double rad );
/**
 * Converts deg to rad
 */
double deg2Rad( double deg );
/**
 * Returns the atan( s / c ) with all special cases
 */
double pmatan( double s, double c );
/**
 * Returns x^y. y has to be >= 0
 */
int pmpot( int x, int y );

#endif
