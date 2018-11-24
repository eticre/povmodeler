//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk2if.ufrj.br
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


#ifndef PMQUICKCOLOR_H
#define PMQUICKCOLOR_H



#include "pmobject.h"
#include "pmcolor.h"

/**
 * Class for quick colors.
 * Tell POV-Ray what solid color to use for quick renders instead of a paterned pigment.
 */

class PMQuickColor : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates a PMQuickColor
    */
   PMQuickColor( PMPart* part );
   /**
    * Copy constructor
    */
   PMQuickColor( const PMQuickColor& c );
   /**
    * deletes the PMQuickColor
    */
   virtual ~PMQuickColor();

   /** */
   virtual PMObject* copy() const { return new PMQuickColor( *this ); }
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
    * Returns a new @ref PMQuickColorEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmquickcolor" ); }

   /**
    * Returns the color
    */
   PMColor color() const { return m_color; }
   /**
    * Sets the color
    */
   void setColor( const PMColor& c );

   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMQuickColorMementoID { PMColorID };
   PMColor m_color;

   static PMMetaObject* s_pMetaObject;
};

#endif
