//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMSQE_H
#define PMSQE_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include "pmpoint.h"
#include "pmline.h"

class PMViewStructure;

/**
 * Class for povray superquadric ellipsoids.
 */

class PMSuperquadricEllipsoid : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Creates an empty PMSuperquadricEllipsoid
    */
   PMSuperquadricEllipsoid( PMPart* part );
   /**
    * Copy constructor
    */
   PMSuperquadricEllipsoid( const PMSuperquadricEllipsoid& s );
   /**
    * deletes the PMSuperquadricEllipsoid
    */
   virtual ~PMSuperquadricEllipsoid();

   /** */
   virtual PMObject* copy() const { return new PMSuperquadricEllipsoid( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMSuperquadricEllipsoidEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmsqe" ); }

   /**
    * Returns the east west exponent
    */
   double eastWestExponent() const { return m_eastWestExponent; }
   /**
    * Sets the east west exponent
    */
   void setEastWestExponent( double e );
   /**
    * Returns the north south exponent
    */
   double northSouthExponent() const { return m_northSouthExponent; }
   /**
    * Sets the north south exponent
    */
   void setNorthSouthExponent( double n );

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual bool hasDisplayDetail() const { return true; }

   /**
    * Sets the number of latitutes
    */
   static void setUSteps( int u );
   /**
    * Sets the number of longitudes
    */
   static void setVSteps( int v );
   /**
    * Returns the number or latitutes
    */
   static int uSteps() { return s_uStep; }
   /**
    * Returns the number or longitudes
    */
   static int vSteps() { return s_vStep; }
   /** */
   virtual void cleanUp() const;

protected:
   /** */
   virtual bool isDefault();
   /** */
   virtual void createViewStructure();
   /** */
   virtual PMViewStructure* defaultViewStructure() const;
   /** */
   virtual int viewStructureParameterKey() const { return s_parameterKey + globalDetailKey(); }

private:
   /**
    * Creates the lines for the view structure
    */
   static void createLines( PMLineArray& lines, int uStep, int vStep );
   /**
    * Creates the points for the view structure
    */
   static void createPoints( PMPointArray& points, double e, double n, int uStep, int vStep );

   /**
    * IDs for @ref PMMementoData
    */
   enum PMSQEMementoID { PMEastWestExponentID, PMNorthSouthExponentID };
   double m_eastWestExponent, m_northSouthExponent;

   /**
    * The default view structure. It can be shared between sqe's
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static int s_vStep;
   static int s_uStep;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
