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


#ifndef PMTRANSLATE_H
#define PMTRANSLATE_H



#include "pmobject.h"
#include "pmvector.h"

/**
 * Class for povray move commands.
 */

class PMTranslate : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates a move < 0, 0, 0 >
    */
   PMTranslate( PMPart* part );
   /**
    * Copy constructor
    */
   PMTranslate( const PMTranslate& t );
   /**
    * deletes the object
    */
   virtual ~PMTranslate();

   /** */
   virtual PMObject* copy() const { return new PMTranslate( *this ); }
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
    * Returns a new @ref PMTranslateEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmtranslate" ); }

   /**
    * Returns the movement
    */
   PMVector translation() const { return m_move; }
   /**
    * Sets the movement
    */
   void setTranslation( const PMVector& p );

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
   enum PMTranslateMementoID { PMTranslationID };
   PMVector m_move;

   static PMMetaObject* s_pMetaObject;
};

#endif
