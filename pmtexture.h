//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Luis Carvalho
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


#ifndef PMTEXTURE_H
#define PMTEXTURE_H



#include "pmtexturebase.h"

/**
 * Class for povray textures
 */
class PMTexture : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMTexture
    */
   PMTexture( PMPart* part );
   /**
    * Copy constructor
    */
   PMTexture( const PMTexture& t );
   /**
    * Deletes the object
    */
   virtual ~PMTexture();

   /** */
   virtual PMObject* copy() const { return new PMTexture( *this ); }
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
    * Returns a new @ref PMTextureEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmtexture" ); }

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
