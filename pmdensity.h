//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMDENSITY_H
#define PMDENSITY_H



#include "pmtexturebase.h"

/**
 * Class for povray densities
 */
class PMDensity : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMDensity
    */
   PMDensity( PMPart* part );
   /**
    * Copy constructor
    */
   PMDensity( const PMDensity& d );
   /**
    * Deletes the object
    */
   virtual ~PMDensity();

   /** */
   virtual PMObject* copy() const { return new PMDensity( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /**
    * Returns a new @ref PMDensityEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmdensity" ); }

private:
   /**
    * IDs for @ref PMMementoData
    */
//   enum PMDensityMementoID { };

   static PMMetaObject* s_pMetaObject;
};


#endif
