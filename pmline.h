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


#ifndef PMLINES_H
#define PMLINES_H

#include <QList>
#include <QVector>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "pmdebug.h"

/**
 * Line to display with index of start and end point.
 *
 * Line of a @ref PMViewStructure. Only the indices in a @ref PMPointArray
 * are stored.
 *
 * Optimized for OpenGL
 */
class PMLine
{
public:
   /**
    * Default constructor
    */
   PMLine()
   {
      m_start = 0;
      m_end = 0;
   }
   /**
    * Creates a line with start point si and end point ei. If si is greater
    * than ei, si and ei are swapped.
    */
   PMLine( const GLuint si, const GLuint ei )
   { m_start = si; m_end = ei; checkPoints(); }

   /**
    * Sets the start point.
    */
   void setStartPoint( GLuint si ) { m_start = si; checkPoints(); }
   /**
    * Sets the end point.
    */
   void setEndPoint( GLuint ei ) { m_end = ei; checkPoints(); }
   /**
    * Returns the start point.
    */
   GLuint startPoint() const { return m_start; }
   /**
    * Returns the end point.
    */
   GLuint endPoint() const { return m_end; }

private:
   /**
    * Swaps the two points.
    */
   void swapPoints() { GLuint help = m_start; m_start = m_end; m_end = help; }
   /**
    * Checks, if si < ei and swaps the two points if necessary
    */
   void checkPoints()
   {
      if( m_start == m_end ) qCritical() << "Start index = end index in PMLine" << "\n";
      if( m_start > m_end ) swapPoints();
   }
   /**
    * The start and end points (indices!)
    *
    * THESE MEMBERS HAVE TO BE THE ONLY ONE (for rendering with OpenGl)
    */
   GLuint m_start, m_end;
};

typedef QList< PMLine* >::Iterator PMLineListIterator;

/**
 * A list of @ref PMLine objects.
 *
 * This class stores all lines of a @ref PMViewStructure. A line is
 * described by a start and end point. Only the indices in a @ref PMPointArray
 * are stored.
 */
typedef QVector<PMLine> PMLineArray;

#endif
