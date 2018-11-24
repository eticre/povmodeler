//-*-C++-*-
/*
**************************************************************************
                                 description
                             -------------------
    and                  : (C) 2002 by Andreas Zehender
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


#ifndef PMLOOKSLIKE_H
#define PMLOOKSLIKE_H



#include "pmnamedobject.h"

/**
 * Class for povray looks_like statements.
 */
class PMLooksLike : public PMNamedObject
{
   typedef PMNamedObject Base;

public:
   /**
    * Constructor
    */
   PMLooksLike( PMPart* part );
   /**
    * Copy constructor
    */
   PMLooksLike( const PMLooksLike& l );
   /**
    * Deletes the PMLooksLike
    */
   virtual ~PMLooksLike();

   /** */
   virtual PMObject* copy() const { return new PMLooksLike( *this ); }
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
    * Returns a new @ref PMLooksLikeEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmlookslike" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );

private:
   static PMMetaObject* s_pMetaObject;
};


#endif
