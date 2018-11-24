//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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


#ifndef PMOBJECTDRAG_H
#define PMOBJECTDRAG_H

#include <qstringlist.h>
#include <QList>
#include <QMimeData>

#include "pmobject.h"
class PMParser;
class PMPart;

/**
 * Supports drag/drop and copy/paste of kpovmodeler objects
 */

class PMObjectDrag : public QMimeData
{
public:
   /**
    * Creates a drag object for one object
    *
    * The object drag will contain all formats that are supported
    * by the part.
    * @see PMIOManager
    */
   PMObjectDrag( PMPart* part, PMObject* object );
   /**
    * Creates a drag object for all objects in objList
    */
   PMObjectDrag( PMPart* part, const PMObjectList& objList );
   /**
    * Deletes the drag object
    */
   ~PMObjectDrag();
   /** 
    * Returns true if the information in e can be decoded 
    */ 
   static bool canDecode( const QMimeData* e, PMPart* part ); 
   /**
    * Returns a pointer to a parser for this drag object or 0, if the data
    * can't be decoded.
    *
    * The caller is responsible to delete the parser.
    */
   static PMParser* newParser( const QMimeData* e, PMPart* part );
};

#endif
