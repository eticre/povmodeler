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

#ifndef PMSORCONTROLPOINT_H
#define PMSORCONTROLPOINT_H



#include "pmcontrolpoint.h"

const double c_sorTolerance = 0.0001;

/**
 * Class for free moveable control points
 */
class PMSorControlPoint : public PMControlPoint
{
public:
   /**
    * Type enum
    */
   enum CPType { PM2DXY, PM2DYX, PM2DXZ, PM2DZX, PM2DYZ, PM2DZY };
   /**
    * Creates a PMSorControlPoint with id. Point has to be a 2D vector.
    */
   PMSorControlPoint( PMSorControlPoint* prev,
                      const PMVector& point, CPType type,
                      int id, const QString& description );
   /**
    * Deletes the PMSorControlPoint
    */
   virtual ~PMSorControlPoint() { }
   
   /** */
   virtual PMVector position() const { return to3D( m_point ); }
   /**
    * Sets the 2d coordinates of the control point
    */
   void setPoint( const PMVector& newPoint ) { m_point = newPoint; }
   /**
    * 2d coordinates of the control point
    */
   PMVector point() const { return m_point; }
   /**
    * This method is used by the sor object to link
    * the control points in the xy and xz plane. These points are
    * synchronized if both are selected.
    */
   void setSorLink( PMSorControlPoint* p ) { m_pSorLink = p; }
   /**
    * Returns the linked control point for lathe points
    */
   PMSorControlPoint* sorLink() const { return m_pSorLink; }
   /** */
   virtual void snapToGrid();
   
   /** */
   virtual bool hasExtraLine() const;
   /** */
   virtual PMVector extraLineStart() const;
   /** */
   virtual PMVector extraLineEnd() const;

protected:
   /** */
   virtual void graphicalChangeStarted();
   /** */
   virtual void graphicalChange( const PMVector& startPoint,
                                 const PMVector& viewNormal,
                                 const PMVector& endPoint );
private:
   PMVector to2D( const PMVector& v ) const;
   PMVector to3D( const PMVector& v ) const;
   
   PMVector m_point, m_originalPoint, m_original2DPoint;
   CPType m_type;
   PMSorControlPoint* m_pPrev;
   PMSorControlPoint* m_pNext;
   PMSorControlPoint* m_pSorLink;
};

#endif
