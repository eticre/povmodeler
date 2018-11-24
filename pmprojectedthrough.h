//-*-C++-*-
/*
**************************************************************************
                                 description
                             -------------------
    and                  : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMPROJECTEDTHROUGH_H
#define PMPROJECTEDTHROUGH_H



#include "pmnamedobject.h"

/**
 * Class for povray projected_through statements.
 */
class PMProjectedThrough : public PMNamedObject
{
   typedef PMNamedObject Base;

public:
   /**
    * Constructor
    */
   PMProjectedThrough( PMPart* part );
   /**
    * Copy constructor
    */
   PMProjectedThrough( const PMProjectedThrough& l );
   /**
    * Deletes the PMProjectedThrough
    */
   virtual ~PMProjectedThrough();

   /** */
   virtual PMObject* copy() const { return new PMProjectedThrough( *this ); }
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
    * Returns a new @ref PMProjectedThroughEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmprojectedthrough" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );

private:
   static PMMetaObject* s_pMetaObject;
};


#endif
