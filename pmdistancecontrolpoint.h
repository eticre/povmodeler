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

#ifndef PMDISTANCECONTROLPOINT_H
#define PMDISTANCECONTROLPOINT_H




#include "pmcontrolpoint.h"

/**
 * Class for distance control points
 */
class PMDistanceControlPoint : public PMControlPoint
{
public:
   /**
    * Creates a PMDistanceControlPoint with id.
    *
    * The base point of the vector is given by the control point location.
    */
   PMDistanceControlPoint( PMControlPoint* location, const PMVector& direction,
                           double distance, int id, const QString& description,
                           bool extraLine = false );
   /**
    * Creates a PMDistanceControlPoint with id.
    *
    * The base point of the vector is given by the vector p.
    */
   PMDistanceControlPoint( const PMVector& location, const PMVector& direction,
                           double distance, int id, const QString& description,
                           bool extraLine = false );
   
   /**
    * Deletes the PMDistanceControlPoint
    */
   virtual ~PMDistanceControlPoint() { }
   
   /** */
   virtual PMVector position() const;
   
   /**
    * Sets the distance
    */
   void setDistance( double distance ) { m_distance = distance; }
   /**
    * returns the distance
    */
   double distance() const { return m_distance; }
   /**
    * Sets the direction
    */
   void setDirection( const PMVector& d );
   /**
    * Returns the direction
    */
   PMVector direction() const { return m_direction; }
   /**
    * Returns the base point
    */
   PMVector basePoint() const;
   
   /** */
   virtual bool hasExtraLine() const { return m_extraLine; }
   /**
    * Returns the start point of the extra line
    */
   virtual PMVector extraLineStart() const { return basePoint(); }
   /**
    * Returns the end point of the extra line
    */
   virtual PMVector extraLineEnd() const { return position(); }

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
   double m_distance, m_originalDistance;
   PMControlPoint* m_pBasePoint;
   PMVector m_constBasePoint;
   PMVector m_direction;
   double m_directionLength;
   bool m_extraLine;
};

#endif
