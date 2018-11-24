//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#ifndef PMROTATE_H
#define PMROTATE_H



#include "pmobject.h"
#include "pmvector.h"

/**
 * Class for povray rotate commands.
 */

class PMRotate : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates a rotate < 0, 0, 0 >
    */
   PMRotate( PMPart* part );
   /**
    * Copy constructor
    */
   PMRotate( const PMRotate& r );
   /**
    * deletes the object
    */
   virtual ~PMRotate();

   /** */
   virtual PMObject* copy() const { return new PMRotate( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMRotateEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmrotate" ); }

   /**
    * Returns the rotation
    */
   PMVector rotation() const { return m_rotate; }
   /**
    * Sets the rotation
    */
   void setRotation( const PMVector& p );

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual bool hasTransformationMatrix() const { return true; }
   /** */
   virtual PMMatrix transformationMatrix() const;
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMRotateMementoID { PMRotationID };
   PMVector m_rotate;

   static PMMetaObject* s_pMetaObject;
};

#endif
