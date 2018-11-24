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


#ifndef PMADDCOMMAND_H
#define PMADDCOMMAND_H



#include "pmcommand.h"
#include "pmobject.h"

class PMMemento;

/**
 * Command class for adding a new PMObject
 */
class PMAddCommand : public PMCommand
{
public:
   /**
    * Command that adds a new PMObject.
    *
    * The object obj will be inserted as child of parent after
    * the object after.
    *
    * If after is 0, the object becomes the first child.
    */
   PMAddCommand( PMObject* obj, PMObject* parent, PMObject* after );
   
   /**
    * Command that adds a list of new PMObjects.
    *
    * The object in the list will be inserted as children of parent after
    * the object after.
    *
    * If after is 0, the objects will be inserted as first children.
    */
   PMAddCommand( const PMObjectList& list, PMObject* parent, PMObject* after );
   /**
    * Deletes the command. The inserted object will be deleted, if
    the command was not executed (or undo-ed) */
   virtual ~PMAddCommand();
   
   /** */
   virtual int errorFlags( PMPart* );
   
protected:
   /**
    * Executes the command and stores undo information
    */
   virtual void execute( PMCommandManager* theManager );
   /**
    * Undo the command
    */
   virtual void undo( PMCommandManager* theManager );
   
private:
   PMObject* m_pParent;
   PMObjectList m_objects;
   PMObject* m_pAfter;
   bool m_executed, m_firstExecution;
   PMObjectList m_insertErrors;
   PMObjectList m_links;
   PMObjectList m_linkedDeclares;
   bool m_linksCreated;
   PMMemento* m_pParentChangeMemento;
};

#endif
