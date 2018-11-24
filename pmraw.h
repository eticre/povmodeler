//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
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


#ifndef PMRAW_H
#define PMRAW_H

#include "pmnamedobject.h"
#include <qstring.h>


/**
 * Class for raw povray code
 */
class PMRaw : public PMNamedObject
{
   typedef PMNamedObject Base;
public:
   /**
    * Creates an empty raw povray object
    */
   PMRaw( PMPart* part );
   /**
    * Copy constructor
    */
   PMRaw( const PMRaw& r );
   /**
    * Creates a raw povray object with text t
    */
   PMRaw( PMPart* part, const QString& t );
   /**
    * Deletes the raw povray object
    */
   ~PMRaw();

   /**
    * Sets the povray code
    */
   void setCode( const QString& text );
   /**
    * Returns the raw povray code
    */
   QString code() const { return m_code; }

   /** */
   virtual PMObject* copy() const { return new PMRaw( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmraw" ); }
   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMRawMementoID { PMCodeID };
   QString m_code;

   static PMMetaObject* s_pMetaObject;
};

#endif
