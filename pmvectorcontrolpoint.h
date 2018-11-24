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

#ifndef PMVECTORCONTROLPOINT_H
#define PMVECTORCONTROLPOINT_H




#include "pmcontrolpoint.h"

/**
 * Class for vector like control points
 */
class PMVectorControlPoint : public PMControlPoint
{
public:
   /**
    * Creates a PMVectorControlPoint with id.
    *
    * The base point of the vector is given by the control point location.
    */
   PMVectorControlPoint( PMControlPoint* location, const PMVector& v,
                         int id, const QString& description,
                         bool extraLine = true, bool normalize = false );
   /**
    * Creates a PMVectorControlPoint with id.
    *
    * The base point of the vector is given by the vector p.
    */
   PMVectorControlPoint( const PMVector& location, const PMVector& v,
                         int id, const QString& description,
                         bool extraLine = true, bool normalize = false );
   /**
    * Deletes the PMVectorControlPoint
    */
   virtual ~PMVectorControlPoint() { }
   
   /** */
   virtual PMVector position() const;
   /**
    * Returns the base point
    */
   PMVector basePoint() const;
   
   /**
    * Sets the vector
    */
   void setVector( PMVector newVector ) { m_vector = newVector; }
   /**
    * returns the vector
    */
   PMVector vector() const { return m_vector; }

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
   PMVector m_vector, m_originalVector;
   PMControlPoint* m_pBasePoint;
   PMVector m_constBasePoint;
   bool m_extraLine;
   bool m_normalize;
};

#endif
