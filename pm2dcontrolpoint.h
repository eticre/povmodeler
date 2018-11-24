//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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

#ifndef PM2DCONTROLPOINT_H
#define PM2DCONTROLPOINT_H




#include "pmcontrolpoint.h"
#include <QList>

/**
 * Class for free moveable control points
 */
class PM2DControlPoint : public PMControlPoint
{
public:
   /**
    * Type enum
    */
   enum CPType { PM2DXY, PM2DYX, PM2DXZ, PM2DZX, PM2DYZ, PM2DZY };
   /**
    * Creates a PM2DControlPoint with id. Point has to be a 2D vector.
    */
   PM2DControlPoint( const PMVector& point, CPType type,
                     int id, const QString& description );
   /**
    * Deletes the PM2DControlPoint
    */
   virtual ~PM2DControlPoint() { }
   
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
   /** */
   virtual void snapToGrid();
   /**
    * Returns the third coordinate
    */
   double thirdCoordinate() const { return m_thirdCoordinate; }
   /**
    * Sets the third coordinate
    */
   void setThirdCoordinate( double d ) { m_thirdCoordinate = d; }
   /**
    * Returns the 2d scale
    */
   double scale() const { return m_scale; }
   /**
    * Sets the scale
    */
   void setScale( double s ) { m_scale = s; }
   
   /**
    * Sets the base point.
    *
    * If a base point is set, an extra line is shown between
    * the base point and this control point.
    */
   void setBasePoint( PM2DControlPoint* p );
   /**
    * Returns the base point
    */
   PM2DControlPoint* basePoint() const { return m_pBasePoint; }
   /**
    * This method is used by the lathe object to link
    * the control points in the xy and xz plane. These points are
    * synchronized if both are selected.
    */
   void setLatheLink( PM2DControlPoint* p ) { m_pLatheLink = p; }
   /**
    * Returns the linked control point for lathe points
    */
   PM2DControlPoint* latheLink() const { return m_pLatheLink; }

   /** */
   virtual bool hasExtraLine() const { return m_pBasePoint; }
   /** */
   virtual PMVector extraLineStart() const { return position(); }
   /** */
   virtual PMVector extraLineEnd() const
   {
      if( m_pBasePoint )
         return m_pBasePoint->position();
      return PMVector( 0, 0, 0 );
   }

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
   void addLinkedPoint( PM2DControlPoint* p );
   void removeLinkedPoint( PM2DControlPoint* p );
   
   PMVector m_point, m_originalPoint, m_original2DPoint;
   CPType m_type;
   double m_thirdCoordinate;
   double m_scale;
   PM2DControlPoint* m_pBasePoint;
   QList<PM2DControlPoint*> m_linkedPoints;
   PM2DControlPoint* m_pLatheLink;
};

#endif
