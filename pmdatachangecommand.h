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


#ifndef PMDATACHANGECOMMAND_H
#define PMDATACHANGECOMMAND_H



#include "pmcommand.h"

class PMMemento;

/**
 * Command that stores undo information for object changes
 *
 * Let an object create a @ref PMMemento, change the objects data, take the
 * memento and create a PMDataChangeCommand with this memento.
 *
 * Then execute the command. The object will not be changed again!
 */
class PMDataChangeCommand : public PMCommand
{
public:
   /**
    * Creates a PMDataChangeCommand for the memento
    */
   PMDataChangeCommand( PMMemento* memento );
   /**
    * Deletes the command. The memento will be deleted.
    */
   virtual ~PMDataChangeCommand();
protected:
   /**
    * Executes the command
    */
   virtual void execute( PMCommandManager* theManager );
   /**
    * Undo the command
    */
   virtual void undo( PMCommandManager* theManager );
   
private:
   void signalChanges( PMCommandManager* theManager, PMMemento* memento );
   PMMemento* m_pNewState;
   PMMemento* m_pOldState;
   bool m_executed;
   bool m_unexecuted;
};

#endif
