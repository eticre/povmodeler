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


#ifndef PMCONTROLPOINT_H
#define PMCONTROLPOINT_H



#include <QList>
#include "pmvector.h"

/**
 * Interface between the graphical views and the PMObject.
 *
 * PMControlPoint is the interface between graphical view and PMObjects that
 * can be changed graphically with the mouse. A @ref PMObject has a
 * PMControlPoint for each changeable attribute.
 *
 * There is one subclass for each change behavior like 3dpoint, normal vector,
 * radius, rotation ...
 *
 * Each control point has an id to be indentified by the PMObject. The id has
 * to be unique within a PMObject.
 */

class PMControlPoint
{
public:
   /**
    * Creates a PMControlPoint with an id and a description.
    */
   PMControlPoint( int id, const QString& description );
   /**
    * Deletes the PMControlPoint
    */
   virtual ~PMControlPoint();
   
   /**
    * Starts a graphical change at 3d cursor position startPoint. viewNormal
    * is the normal vector of the view
    */
   void startChange( const PMVector& startPoint, const PMVector& viewNormal );
   /**
    * Graphical change with 3d end cursor position endPoint
    */
   void change( const PMVector& endPoint );
   /**
    * Snaps the control point to the grid
    */
   virtual void snapToGrid() = 0;
   
   /**
    * 3d coordinates of the control point for rendering*/
   virtual PMVector position() const { return PMVector(); }
   /**
    * Returns true if the point should be displayed (rendered)
    */
   virtual bool display() const { return true; }
   /**
    * Returns the id of the control point
    */
   int id() const { return m_id; }
   /**
    * Type of the control point
    *
    * CPPoint: The control point is displayed as point, the mouse cursor
    * has to be over the control point to be active (e.g. 3DControlPoint)
    *
    * CPCross: The control point is displayed as cross, the control point
    * can be changed with the mouse in the whole view (e.g. Translation)
    */
   enum PMCPDisplayType { CPPoint = 0, CPCross = 1 };
   /**
    * Returns the type of the control point (see @ref PMCPDisplayType)
    */
   virtual PMCPDisplayType displayType() const { return CPPoint; }
   /**
    * Returns the description
    */
   QString description() const { return m_description; }
   /**
    * Selects/deselects the control point
    */
   void setSelected( bool yes ) { m_bSelected = yes; }
   /**
    * Returns true if the control point is selected
    */
   bool selected() const { return m_bSelected; }

   /**
    * Returns true, if the control point was changed and sets the
    * changed flag to false.
    */
   bool changed();

   /**
    * Returns true if an extra line should be displayed in addition to
    * the view structure
    */
   virtual bool hasExtraLine() const { return false; }
   /**
    * Returns the start point of the extra line
    */
   virtual PMVector extraLineStart() const { return PMVector( 0, 0, 0 ); }
   /**
    * Returns the end point of the extra line
    */
   virtual PMVector extraLineEnd() const { return PMVector( 0, 0, 0 ); }
   
   /**
    * Returns the grid distance for 3d points, vectors and movements
    */
   static double moveGrid() { return s_moveGrid; }
   /**
    * Sets the grid distance
    */
   static void setMoveGrid( double d );
   /**
    * Returns the grid distance rotations
    */
   static double rotateGrid() { return s_rotateGrid; }
   /**
    * Sets the grid distance
    */
   static void setRotateGrid( double d );
   /**
    * Returns the grid distance for scales
    */
   static double scaleGrid() { return s_scaleGrid; }
   /**
    * Sets the grid distance
    */
   static void setScaleGrid( double d );
   
   static void saveConfig();
   static void restoreConfig();
   
protected:
   /**
    * Called when a graphical change was started
    */
   virtual void graphicalChangeStarted() = 0;
   /**
    * Called when the control point was changed
    */
   virtual void graphicalChange( const PMVector& startPoint,
                                 const PMVector& viewNormal,
                                 const PMVector& endPoint ) = 0;
   /**
    * Sets the changed flag
    */
   void setChanged() { m_bChanged = true; }
private:
   int m_id;
   PMVector m_startPoint;
   PMVector m_normalVector;
   bool m_bChanged;
   bool m_bSelected;
   QString m_description;

   static double s_moveGrid;
   static double s_rotateGrid;
   static double s_scaleGrid;
};

typedef QList<PMControlPoint*> PMControlPointList;

#endif
