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


#ifndef PMCOMMANDMANAGER_H
#define PMCOMMANDMANAGER_H



#include "pmcommand.h"
#include <QObject>

class PMPart;

/**
 * Manager for PMCommand objects.
 *
 * The class PMCommandManager stores stacks of commands for undo/redo
 * operations.
 */
class PMCommandManager : public QObject
{
   Q_OBJECT
public:
   /**
    * Creates a new PMCommandManager
    */
   PMCommandManager( PMPart* thepart );
   /**
    * Deletes the command manager. All commands are deleted as well.
    */
   ~PMCommandManager();
   /**
    * Adds the @ref PMCommand to the command stack.
    * All commands in the m_redoCommands stack are deleted.
    */
   void execute( PMCommand* cmd );
   /**
    * Moves the last command to the m_redoCommands stack.
    */
   void undo();
   /**
    * Moves the last redo command to the command stack.
    */
   void redo();
   /**
    * Deletes the commands
    */
   void clear();
   /**
    * Returns the maximal number of items that can be undone/redone.
    */
   unsigned int maxUndoRedo() const { return m_maxUndoRedo; }
   /**
    * Sets the maximal number of items that can be undone/redone.
    */
   void setMaxUndoRedo( unsigned int n ) { m_maxUndoRedo = n; }
   /**
    * Called by an executed command. Will emit objectChanged()
    */
   void cmdObjectChanged( PMObject* obj, const int mode );
   /**
    * Called by an executed command. Will emit idChanged()
    */
   void cmdIDChanged( PMObject* obj, const QString& oldID );
   /**
    * Returns a pointer to the part. For commands that need to access the
    * part directly.
    */
   PMPart* part() const { return m_pPart; }
//signals:
Q_SIGNALS:
   /**
    * emmited, when the undo and redo command texts change
    */
   void updateUndoRedo( const QString& undo, const QString& redo );
   /**
    * Signal that is emitted when an object is changed.
    * Mode is a bit combination of @ref PMChange constants.
    */
   void objectChanged( PMObject* obj, const int mode, QObject* sender );
   /**
    * Signal that is emitted when the id of the object is changed
    */
   void idChanged( PMObject* obj, const QString& oldID );
private:
   /**
    * The executed commands.
    */
   QList<PMCommand*> m_commands;
   /**
    * The undone commands.
    */
   QList<PMCommand*> m_redoCommands;
   // the maximal number of items that can be undone/redone.
   int m_maxUndoRedo;
   PMObject* m_pLastChangedObject;
   PMPart* m_pPart;
};

#endif
