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


#ifndef PMMATERIAL_H
#define PMMATERIAL_H



#include "pmtexturebase.h"

/**
 * Class for povray materials
 */
class PMMaterial : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMMaterial
    */
   PMMaterial( PMPart* part );
   /**
    * Copy constructor
    */
   PMMaterial( const PMMaterial& m );
   /**
    * Deletes the object
    */
   virtual ~PMMaterial();

   /** */
   virtual PMObject* copy() const { return new PMMaterial( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /**
    * Returns a new @ref PMMaterialEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmmaterial" ); }

private:
   /**
    * IDs for @ref PMMementoData
    */
//   enum PMMaterialMementoID { };
   static PMMetaObject* s_pMetaObject;
};


#endif
