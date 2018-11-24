//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
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


#ifndef PMCONE_H
#define PMCONE_H



#include "pmsolidobject.h"
#include "pmvector.h"
#include "pmviewstructure.h"

class PMViewStructure;

/**
 * Class for Povray Cone
 *
 */

class PMCone : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Create a Cone
    */
   PMCone( PMPart* part );
   /**
    * Copy constructor
    */
   PMCone( const PMCone& c );

   /**
    * delete the Cone
    */
   virtual ~PMCone();

   /** */
   virtual PMObject* copy() const { return new PMCone( *this ); }

   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMConeEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view and dialog view
    */
   virtual QString pixmap() const { return QString( "pmcone" ); }

   /**
    * Return the end_1
    */
   PMVector end1() const { return m_end1; }
   /**
    * Sets end_1
    */
   void setEnd1( const PMVector & p );
   /**
    * Return the end_2
    */
   PMVector end2() const { return m_end2; }
   /**
    * Sets end_2
    */
   void setEnd2( const PMVector & p );
   /**
    * return the first radius of the cone
    */
   double radius1() const { return m_radius1; }
   /**
    * Sets the first radius of the cone
    */
   void setRadius1( double radius );
   /**
    * return the second radius of the cone
    */
   double radius2() const { return m_radius2; }
   /**
    * Sets the second radius of the cone
    */
   void setRadius2( double radius );
   /**
    * Return open = 1 or close = 0
    */
   bool open() const { return m_open; }
   /**
    * Sets open cylinder
    */
   void setOpen( bool op );

   /**
    * Returns the number of lines for rendering
    */
   static int steps() { return s_numSteps; }
   /**
    * Sets the number of lines for rendering
    */
   static void setSteps( int s );

   /** */
   virtual void restoreMemento( PMMemento * s );
   /** */
   virtual void controlPoints( PMControlPointList & list );
   /** */
   virtual void controlPointsChanged( PMControlPointList & list );
   /** */
   virtual bool hasDisplayDetail() const { return true; }
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
   static void createLines( PMLineArray& lines, int steps );
   /**
    * Creates the points for the view structure
    */
   static void createPoints( PMPointArray& points, const PMVector& end1,
                             const PMVector& end2, double radius1, double radius2, int steps );

   /**
    * IDs for @ref PMMementoData
    */
   enum PMConeMementoID { PMEnd1ID, PMEnd2ID, PMRadius1ID, PMRadius2ID, PMOpenID };
   /**
    * ends of cone
    */
   PMVector m_end1, m_end2;
   /**
    * radius of cone
    */
   double m_radius1,m_radius2;
   /**
    * open
    */
   bool m_open;
   /**
    * The default view structure. It can be shared between cones
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static int s_numSteps;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
