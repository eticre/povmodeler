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

#include "pmtranslatecontrolpoint.h"
#include "pmmath.h"


#include <math.h>

PMTranslateControlPoint::PMTranslateControlPoint( const PMVector& trans, int id )
      : PMControlPoint( id, ( "Translation" ) )
{
   m_translation = trans;
}

void PMTranslateControlPoint::graphicalChangeStarted()
{
   m_originalTranslation = m_translation;
}

void PMTranslateControlPoint::graphicalChange( const PMVector& startPoint,
                                               const PMVector& /*viewNormal*/,
                                               const PMVector& endPoint )
{
   m_translation = m_originalTranslation + endPoint - startPoint;
}

void PMTranslateControlPoint::snapToGrid()
{
   int i;
   double d = moveGrid();
   if( !approxZero( d ) )
      for( i = 0; i < 3; i++ )
         m_translation[i] = rint( m_translation[i] / d ) * d;
   setChanged();
}
