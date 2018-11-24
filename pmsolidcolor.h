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


#ifndef PMSOLIDCOLOR_H
#define PMSOLIDCOLOR_H



#include "pmobject.h"
#include "pmcolor.h"

/**
 * Class for solid colors.
 */

class PMSolidColor : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates a PMSolidColor
    */
   PMSolidColor( PMPart* part );
   /**
    * Copy constructor
    */
   PMSolidColor( const PMSolidColor& s );
   /**
    * deletes the PMSolidColor
    */
   virtual ~PMSolidColor();

   /** */
   virtual PMObject* copy() const { return new PMSolidColor( *this ); }
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
    * Returns a new @ref PMSolidColorEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the color
    */
   PMColor color() const { return m_color; }
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmsolidcolor" ); }
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
   enum PMSolidColorMementoID { PMColorID };
   PMColor m_color;

   static PMMetaObject* s_pMetaObject;
};

#endif
