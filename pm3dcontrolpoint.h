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

#ifndef PM3DCONTROLPOINT_H
#define PM3DCONTROLPOINT_H




#include "pmcontrolpoint.h"

/**
 * Class for free moveable control points
 */
class PM3DControlPoint : public PMControlPoint
{
public:
   /**
    * Creates a PM3DControlPoint with id.
    */
   PM3DControlPoint( const PMVector& point, int id, const QString& description );
   /**
    * Deletes the PM3DControlPoint
    */
   virtual ~PM3DControlPoint() { }
   
   /** */
   virtual PMVector position() const { return m_point; }
   /**
    * Sets the 3d coordinates of the control point
    */
   void setPoint( const PMVector& newPoint ) { m_point = newPoint; }
   /**
    * 3d coordinates of the control point
    */
   PMVector point() const { return m_point; }
   /** */
   virtual void snapToGrid();

protected:
   /** */
   virtual void graphicalChangeStarted();
   /** */
   virtual void graphicalChange( const PMVector& startPoint,
                                 const PMVector& viewNormal,
                                 const PMVector& endPoint );
private:
   PMVector m_point, m_originalPoint;
};

#endif
