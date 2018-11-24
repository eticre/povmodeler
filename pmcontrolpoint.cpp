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

#include "pmcontrolpoint.h"
#include "pmdefaults.h"
#include "pmdebug.h"

#include <QSettings>


double PMControlPoint::s_moveGrid = c_defaultMoveGrid;
double PMControlPoint::s_scaleGrid = c_defaultScaleGrid;
double PMControlPoint::s_rotateGrid = c_defaultRotateGrid;

PMControlPoint::PMControlPoint( int id, const QString& description )
{
   m_id = id;
   m_bChanged = false;
   m_description = description;
   m_bSelected = false;
}

PMControlPoint::~PMControlPoint()
{
}

void PMControlPoint::startChange( const PMVector& s, const PMVector& n )
{
   m_startPoint = s;
   m_normalVector = n;
   graphicalChangeStarted();
}

void PMControlPoint::change( const PMVector& endPoint )
{
   m_bChanged = true;
   graphicalChange( m_startPoint, m_normalVector, endPoint );
}
   
bool PMControlPoint::changed()
{
   bool h = m_bChanged;
   m_bChanged = false;

   return h;
}

void PMControlPoint::setMoveGrid( double d )
{
   if( d > 0 )
      s_moveGrid = d;
   else
      qCritical() << "Grid has to be greater than 0\n";
}

void PMControlPoint::setScaleGrid( double d )
{
   if( d > 0 )
      s_scaleGrid = d;
   else
      qCritical() << "Grid has to be greater than 0\n";
}

void PMControlPoint::setRotateGrid( double d )
{
   if( d > 0 )
      s_rotateGrid = d;
   else
      qCritical() << "Grid has to be greater than 0\n";
}

void PMControlPoint::saveConfig()
{
   QSettings qset;
   qset.setValue( "Grid/MoveGrid", s_moveGrid );
   qset.setValue( "Grid/RotateGrid", s_rotateGrid );
   qset.setValue( "Grid/ScaleGrid", s_scaleGrid );
}

void PMControlPoint::restoreConfig()
{
   QSettings qset;
   s_moveGrid = qset.value( "MoveGrid", c_defaultMoveGrid ).value<double>();
   s_rotateGrid = qset.value( "RotateGrid", c_defaultRotateGrid ).value<double>();
   s_scaleGrid = qset.value( "ScaleGrid", c_defaultScaleGrid ).value<double>();
}
