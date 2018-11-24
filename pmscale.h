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


#ifndef PMSCALE_H
#define PMSCALE_H



#include "pmobject.h"
#include "pmvector.h"


/**
 * Class for povray scale commands.
 */

class PMScale : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates a scale < 0, 0, 0 >
    */
   PMScale( PMPart* part );
   /**
    * Copy constructor
    */
   PMScale( const PMScale& s );
   /**
    * deletes the object
    */
   virtual ~PMScale();

   /** */
   virtual PMObject* copy() const { return new PMScale( *this ); }
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
    * Returns a new @ref PMScaleEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmscale" ); }

   /**
    * Returns the scale
    */
   PMVector scale() const { return m_scale; }
   /**
    * Sets the scale
    */
   void setScale( const PMVector& p );

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
   enum PMScaleMementoID { PMScaleID };
   PMVector m_scale;

   static PMMetaObject* s_pMetaObject;
};

#endif
