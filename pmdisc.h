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


#ifndef PMDISC_H
#define PMDISC_H



#include "pmgraphicalobject.h"
#include "pmvector.h"
#include "pmviewstructure.h"

/**
 * Class for povray disc
 */

class PMDisc : public PMGraphicalObject
{
   typedef PMGraphicalObject Base;
public:
   /**
    * Creates a disc
    */
   PMDisc( PMPart* part );
   /**
    * Copy constructor
    */
   PMDisc( const PMDisc& d );

   /**
    * Deletes the disc
    */
    virtual ~PMDisc();

   /** */
   virtual PMObject* copy() const { return new PMDisc( *this ); }

   /** */
   virtual QString description() const;
   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMDiscEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view and dialog view
    */
   virtual QString pixmap() const { return QString( "pmdisc" ); }

   /**
    * Return the center
    */
   PMVector center() const { return m_center; }
   /**
    * Sets center
    */
   void setCenter( const PMVector& c );
   /**
    * Return the normal
    */
   PMVector normal() const { return m_normal; }
   /**
    * Sets normal
    */
   void setNormal( const PMVector& p );
   /**
    * Return the radius
    */
   double radius() const { return m_radius; }
   /**
    * Sets the radius
    */
   void setRadius( double radius );
   /**
    * Return the hole radius
    */
   double holeRadius() const { return m_hradius; }
   /**
    * Sets the hole radius
    */
   void setHoleRadius( double hradius );

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual bool hasDisplayDetail() const { return true; }

   /**
    * Returns the number of lines for rendering
    */
   static int steps() { return s_numSteps; }
   /**
    * Sets the number of lines for rendering
    */
   static void setSteps( int s );
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
   virtual int viewStructureParameterKey() const { return s_parameterKey+ globalDetailKey(); }

private:
   /**
    * Creates the lines for the view structure
    */
   static void createLines( PMLineArray& lines, int steps );
   /**
    * Creates the points for the view structure
    */
   static void createPoints( PMPointArray& points, const PMVector& center,
      const PMVector& normal, double radius , double hradius, int steps );

   /**
    * IDs for @ref PMMementoData
    */
   enum PMPlaneMementoID { PMCenterID, PMNormalID, PMRadiusID, PMHRadiusID };
   /**
    * center of disc
    */
   PMVector m_center;
   /**
    * normal of disc
    */
   PMVector m_normal;
   /**
    * radius of disc
    */
   double m_radius;
   /**
    * hole radius of disc
    */
   double m_hradius;
   /**
    * The default view structure. It can be shared between planes
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static int s_numSteps;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
