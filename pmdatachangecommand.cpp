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


#include "pmdatachangecommand.h"
#include "pmcommandmanager.h"
#include "pmmemento.h"
#include "pmobject.h"


PMDataChangeCommand::PMDataChangeCommand( PMMemento* memento )
      : PMCommand()
{
   QString text = memento->originator()->name();
   if( text.isEmpty() )
      text = memento->originator()->description();
   setText( ( "Change " + text ) );

   // the data is already changed when the command is created
   m_executed = true;
   m_unexecuted = false;
   m_pOldState = memento;
   m_pNewState = 0;
}

PMDataChangeCommand::~PMDataChangeCommand()
{
   if( m_pOldState )
      delete m_pOldState;
   if( m_pNewState )
      delete m_pNewState;
}

void PMDataChangeCommand::execute( PMCommandManager* theManager )
{
   PMObject* obj = m_pOldState->originator();
   if( !m_executed )
   {
      // if the command is not executed
      // restore the memento
      if( m_pNewState )
      {      
         if( m_pNewState->containsChanges() )
         {
            obj->restoreMemento( m_pNewState );

            if( m_pOldState->idChanged() )
               theManager->cmdIDChanged( obj, m_pOldState->oldID() );
            signalChanges( theManager, m_pNewState );
         }
      }
      m_executed = true;
   }
   else if( !m_unexecuted )
   {
      // the data can be changed multiple times
      // if the command was never unexecuted, emit the signal
      // ( the data was already changed, so this is not necessary here )
      if( m_pOldState->idChanged() )
         theManager->cmdIDChanged( obj, m_pOldState->oldID() );
      signalChanges( theManager, m_pOldState );
   }
}

void PMDataChangeCommand::undo( PMCommandManager* theManager )
{
   if( m_executed )
   {
      if( m_pOldState->containsChanges() )
      {
         PMObject* obj = m_pOldState->originator();
         if( !m_pNewState )
            obj->createMemento();
      
         obj->restoreMemento( m_pOldState );

         if( !m_pNewState )
            m_pNewState = obj->takeMemento();
         
         if( m_pNewState->idChanged() )
            theManager->cmdIDChanged( obj, m_pNewState->oldID() );
         signalChanges( theManager, m_pOldState );
      }
      m_executed = false;
      m_unexecuted = true;
   }
}

void PMDataChangeCommand::signalChanges( PMCommandManager* theManager,
                                         PMMemento* memento )
{
	const QList<PMObjectChange*>& changed = memento->changedObjects();
	QList<PMObjectChange*>::const_iterator c;
	for( c = changed.begin(); c != changed.end(); ++c )
       theManager->cmdObjectChanged( ( *c )->object(), ( *c )->mode() );
}
