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


#ifndef PMPIGMENT_H
#define PMPIGMENT_H



#include "pmtexturebase.h"

/**
 * Class for povray pigments
 */
class PMPigment : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMPigment
    */
   PMPigment( PMPart* part );
   /**
    * Copy constructor
    */
   PMPigment( const PMPigment& );
   /**
    * Deletes the object
    */
   virtual ~PMPigment();

   /** */
   virtual PMObject* copy() const { return new PMPigment( *this ); }
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
    * Returns a new @ref PMPigmentEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmpigment" ); }

   /**
    * Returns the uv mapping flag
    */
   bool uvMapping() const { return m_uvMapping; }
   /**
    * Sets the uv maaping flag
    */
   void setUVMapping( bool m );

   /** */
   virtual void restoreMemento( PMMemento *s );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMPigmentMementoID { PMUVMappingID };

   bool m_uvMapping;

   static PMMetaObject* s_pMetaObject;
};


#endif
