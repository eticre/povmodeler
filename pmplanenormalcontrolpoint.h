//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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

#ifndef PMPLANENORMALCONTROLPOINT_H
#define PMPLANENORMALCONTROLPOINT_H



#include "pmcontrolpoint.h"

class PMDistanceControlPoint;

/**
 * Control points for the plane normal
 */
class PMPlaneNormalControlPoint : public PMControlPoint
{
public:
   /**
    * Creates a PMPlaneNormalControlPoint with id.
    *
    * If the control point is change, the distance control point
    * is changed, too!
    */
   PMPlaneNormalControlPoint( PMDistanceControlPoint* d, const PMVector& normal,
                              int id, const QString& description );
   /**
    * Deletes the PMPlaneNormalControlPoint
    */
   virtual ~PMPlaneNormalControlPoint() { }
   
   /** */
   virtual PMVector position() const;
   
   /**
    * Sets the normal vector
    */
   void setNormal( PMVector newNormal ) { m_normal = newNormal; }
   /**
    * Returns the normal vector
    */
   PMVector normal() const { return m_normal; }
   /**
    * Returns the distance
    */
   double distance() const;

   /** */
   virtual bool hasExtraLine() const { return true; }
   /**
    * Returns the start point of the extra line
    */
   virtual PMVector extraLineStart() const;
   /**
    * Returns the end point of the extra line
    */
   virtual PMVector extraLineEnd() const;

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
   PMVector m_normal, m_originalNormal;
   double m_originalDistance;
   PMDistanceControlPoint* m_pDistancePoint;
};

#endif
