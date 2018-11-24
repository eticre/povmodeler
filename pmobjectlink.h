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


#ifndef PMOBJECTLINK_H
#define PMOBJECTLINK_H



#include "pmsolidobject.h"
#include "pmdeclare.h"

/**
 * Class for links of graphical objects.
 */
class PMObjectLink : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Creates an empty PMObjectLink
    */
   PMObjectLink( PMPart* part );
   /**
    * Copy constructor
    */
   PMObjectLink( const PMObjectLink& ol );
   /**
    * deletes the PMObjectLink
    */
   virtual ~PMObjectLink();

   /** */
   virtual PMObject* copy() const { return new PMObjectLink( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual PMDeclare* linkedObject() const { return m_pLinkedObject; }
   /**
    * Sets the linked object. Returns true if successful
    */
   bool setLinkedObject( PMDeclare* o );

   /**
    * Method used by the properties framework
    */
   PMObject* linkedObjectProperty() const { return m_pLinkedObject; }
   /**
    * Method used by the properties framework
    */
   void setLinkedObjectProperty( PMObject* o );

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMObjectLinkEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmobjectlink" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMObjectLinkMementoID { PMLinkedObjectID };
   PMDeclare* m_pLinkedObject;

   static PMMetaObject* s_pMetaObject;
};

#endif
