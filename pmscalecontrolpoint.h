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

#ifndef PMSCALECONTROLPOINT_H
#define PMSCALECONTROLPOINT_H




#include "pmcontrolpoint.h"

/**
 * Control points for scale
 */
class PMScaleControlPoint : public PMControlPoint
{
public:
   /**
    * Creates a PMScaleControlPoint with id.
    */
   PMScaleControlPoint( const PMVector& scale, int id );
   /**
    * Deletes the PMScaleControlPoint
    */
   virtual ~PMScaleControlPoint() { }
   
   /** */
   virtual PMVector position() const { return PMVector( 0.0, 0.0, 0.0 ); }
   
   /**
    * Sets the scale
    */
   void setScale( const PMVector& sc ) { m_scale = sc; }
   /**
    * Returns the scale
    */
   PMVector scale() const { return m_scale; }

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
   PMVector m_scale, m_originalScale;
};

#endif
