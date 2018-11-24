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


#ifndef PMMOVECOMMAND_H
#define PMMOVECOMMAND_H



#include "pmcommand.h"
#include "pmdeletecommand.h"

class PMObject;
class PMMemento;

/**
 * Command class for moving objects
 */
class PMMoveCommand : public PMCommand
{
public:
   /**
    * Command that moves one PMObject.
    *
    * The object obj will be inserted as child of parent after
    * the object after.
    *
    * If after is 0, the object becomes the first child.
    */
   PMMoveCommand( PMObject* obj, PMObject* parent, PMObject* after );
   
   /**
    * Command that moves a list of new PMObjects.
    * The objects don't have to be siblings of each other.
    *
    * The object in the list will be inserted as children of parent after
    * the object after.
    *
    * If after is 0, the objects will be inserted as first children.
    */
   PMMoveCommand( const PMObjectList& list, PMObject* parent, PMObject* after );
   /**
    * Deletes the command.
    */
   virtual ~PMMoveCommand();

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
   PMObject* m_pAfter;

   QList<PMDeleteInfo*> m_infoList;
   PMObjectList m_insertErrors;
   bool m_executed;
   bool m_firstExecution;
   QList<PMMemento*> m_dataChanges;
};

#endif
