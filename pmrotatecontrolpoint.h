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

#ifndef PMROTATECONTROLPOINT_H
#define PMROTATECONTROLPOINT_H




#include "pmcontrolpoint.h"
#include "pmmatrix.h"

/**
 * Control points for rotation
 */
class PMRotateControlPoint : public PMControlPoint
{
public:
   /**
    * Creates a PMRotateControlPoint with id.
    */
   PMRotateControlPoint( const PMVector& rotation, int id );
   /**
    * Deletes the PMRotateControlPoint
    */
   virtual ~PMRotateControlPoint() { }
   
   /** */
   virtual PMVector position() const { return PMVector( 0.0, 0.0, 0.0 ); }
   
   /**
    * Sets the rotation
    */
   void setRotation( const PMVector& rot ) { m_rotation = rot; }
   /**
    * Returns the rotation
    */
   PMVector rotation() const { return m_rotation; }

   /** */
   virtual PMCPDisplayType displayType() const { return CPCross; }
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
   PMVector m_rotation, m_originalRotation;
   PMMatrix m_originalTransformation;
};

#endif
