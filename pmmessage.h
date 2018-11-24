//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003-2006 by Andreas Zehender
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

#ifndef PMMESSAGE_H
#define PMMESSAGE_H



#include "pmobject.h"

#include <QList>


/**
 * Class for messages in the @ref PMErrorDialog
 */
class PMMessage
{
public:
   /**
    * Default constructor
    */
   PMMessage();
   /**
    * Creates a message with a text and optionally with
    * a link to an object
    */
   PMMessage( const QString& text, PMObject* object = 0 );
   /**
    * Copy constructor
    */
   PMMessage( const PMMessage& m );
   /**
    * Assignment operator
    */
   PMMessage& operator=( const PMMessage& m );

   /**
    * Returns the message text
    */
   QString text() const { return m_sText; }
   /**
    * Sets the message text
    */
   void setText( const QString& text ) { m_sText = text; }
   /**
    * Returns the linked object or 0
    */
   PMObject* linkedObject() const { return m_pObject; }
   /**
    * Sets the linked object
    */
   void setLinkedObject( PMObject* o ) { m_pObject = o; }

private:
   QString m_sText;
   PMObject* m_pObject;
};

typedef QList<PMMessage> PMMessageList;

#endif
