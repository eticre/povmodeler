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


#ifndef PMNORMAL_H
#define PMNORMAL_H



#include "pmtexturebase.h"

/**
 * Class for povray normals
 */
class PMNormal : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMNormal
    */
   PMNormal( PMPart* part );
   /**
    * Copy constructor
    */
   PMNormal( const PMNormal& n );
   /**
    * Deletes the object
    */
   virtual ~PMNormal();

   /** */
   virtual PMObject* copy() const { return new PMNormal( *this ); }
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
    * Returns a new @ref PMNormalEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmnormal" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );

   /**
    * Returns true if bump_size is enabled
    */
   bool isBumpSizeEnabled() const { return m_enableBumpSize; }
   /**
    * enables/disables bump_size
    */
   void enableBumpSize( bool c );
   /**
    * Returns the normal's bump size
    */
   double bumpSize() const { return m_bumpSize; }
   /**
    * Sets the normal's bump size
    */
   void setBumpSize( double c );
   /**
    * Returns the normal's accuracy
    */
   double accuracy() const { return m_accuracy; }
   /**
    * Sets the normal's accuracy
    */
   void setAccuracy( double c );
   /**
    * Returns the uv mapping flag
    */
   bool uvMapping() const { return m_uvMapping; }
   /**
    * Sets the uv mapping flag
    */
   void setUVMapping( bool m );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMNormalMementoID { PMEnableBumpSizeID, PMBumpSizeID, PMAccuracyID, PMUVMappingID };

   bool m_enableBumpSize;
   double m_bumpSize;
   double m_accuracy;
   bool m_uvMapping;

   static PMMetaObject* s_pMetaObject;
};


#endif
