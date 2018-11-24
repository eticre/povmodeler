//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMCOMMENT_H
#define PMCOMMENT_H

#include "pmobject.h"
#include <QString>


/**
 * Class for povray comments
 */

class PMComment : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates an enpty comment
    */
   PMComment( PMPart* part );
   /**
    * Creates a comment with text t
    */
   PMComment( PMPart* part, const QString& t );
   /**
    * Copy constructor
    */
   PMComment( const PMComment& c );
   /**
    * Deletes the comment
    */
   ~PMComment();

   /**
    * Sets the comment text
    */
   void setText( const QString& text );
   /**
    * Returns the comment text
    */
   QString text() const { return m_text; }

   /** */
   virtual PMObject* copy() const { return new PMComment( *this ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString description() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /** */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmcomment" ); }
   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMCommentMementoID { PMTextID };
   QString m_text;
   static PMMetaObject* s_pMetaObject;
};

#endif
