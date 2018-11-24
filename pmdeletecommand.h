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


#ifndef PMDELETECOMMAND_H
#define PMDELETECOMMAND_H



#include "pmcommand.h"
#include <qstring.h>
#include <QList>

#include "pmobject.h"

class PMMemento;

/**
 * Class that stores undo information for the @ref PMDeleteCommand
 */
class PMDeleteInfo
{
public:
   /**
    * Creates undo information for the object deletedObject.
    * The object has to have a parent!
    */
   PMDeleteInfo( PMObject* deletedObject )
   {
      m_pDeletedObject = deletedObject;
      m_pParent = deletedObject->parent();
      m_pPrevSibling = deletedObject->prevSibling();
      m_insertError = false;
   }
   /**
    * Deletes the object. The object deletedObject will not be deleted!
    */
   ~PMDeleteInfo() { }
   /**
    * Returns a pointer to the deleted object
    */
   PMObject* deletedObject() const { return m_pDeletedObject; }
   /**
    * Returns a pointer to the parent of the deleted object
    */
   PMObject* parent() const { return m_pParent; }
   /**
    * Returns the previous sibling of the deleted object
    */
   PMObject* prevSibling() const { return m_pPrevSibling; }
   /**
    * Returns true if this object could not be inserted in a move command
    */
   bool insertError() const { return m_insertError; }
   /**
    * Sets the insert error flag
    */
   void setInsertError() { m_insertError = true; }
private:
   PMObject* m_pDeletedObject;
   PMObject* m_pParent;
   PMObject* m_pPrevSibling;
   bool m_insertError;
};

/**
 * Command class for removing PMObjects
 */
class PMDeleteCommand : public PMCommand
{
public:
   /**
    * Delete Command that removes the object obj.
    */
   PMDeleteCommand( PMObject* obj );
   /**
    * Command that deletes a list of PMObjects. The list has to be sorted!
    */
   PMDeleteCommand( const PMObjectList& list );
   /**
    * Deletes the command. The deleted object will be deleted, if removed
    */
   virtual ~PMDeleteCommand();

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
   QList<PMDeleteInfo*> m_infoList;
   bool m_executed, m_firstExecution;
   PMObjectList m_links;
   PMObjectList m_linkedDeclares;
   bool m_linksCreated;
   QList<PMMemento*> m_dataChanges;
};

#endif
