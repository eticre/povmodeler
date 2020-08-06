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


#include "pmcommandmanager.h"
#include "pmpart.h"

PMCommandManager::PMCommandManager( PMPart* thePart )
{
   m_maxUndoRedo = 50;
   m_pPart = thePart;
}

PMCommandManager::~PMCommandManager()
{
	foreach( PMCommand* cmd, m_commands ) delete cmd;
	foreach( PMCommand* cmd, m_redoCommands ) delete cmd;
   m_commands.clear();
   m_redoCommands.clear();
}

void PMCommandManager::execute( PMCommand* cmd )
{
   // some commands (like control point commands) can be executed
   // multiple times.
   cmd->execute( this );
   
   if( m_commands.isEmpty() || m_commands.last() != cmd )
      m_commands.append( cmd );
   while( m_commands.count() > m_maxUndoRedo )
	{
		delete m_commands.first();
      m_commands.removeFirst();
	}
	foreach( PMCommand* cmd, m_redoCommands ) delete cmd;
   m_redoCommands.clear();

   emit updateUndoRedo( cmd->text(), QString() );
}

void PMCommandManager::undo()
{
   if( !m_commands.isEmpty() )
   {
      PMCommand* last = m_commands.last();
      last->undo( this );
      
      m_redoCommands.append( last );
		m_commands.removeLast();

      if( m_commands.isEmpty() )
         emit updateUndoRedo( QString(), last->text() );
      else
         emit updateUndoRedo( m_commands.last()->text(), last->text() );
   }
}

void PMCommandManager::redo()
{
   if( !m_redoCommands.isEmpty() )
   {
      PMCommand* last = m_redoCommands.last();
      last->execute( this );
      
      m_commands.append( last );
		m_redoCommands.removeLast();
      
      if( m_redoCommands.isEmpty() )
         emit updateUndoRedo( last->text(), QString() );
      else
         emit updateUndoRedo( last->text(), m_redoCommands.last()->text() );
   }
}

void PMCommandManager::clear()
{
	foreach( PMCommand* cmd, m_commands ) delete cmd;
	foreach( PMCommand* cmd, m_redoCommands ) delete cmd;
   m_redoCommands.clear();
   m_commands.clear();
   emit updateUndoRedo( QString(), QString() );
}

void PMCommandManager::cmdObjectChanged( PMObject* obj, const int mode )
{
   emit objectChanged( obj, mode, this );
}

void PMCommandManager::cmdIDChanged( PMObject* obj, const QString& oldID )
{
   emit idChanged( obj, oldID );
}
#include "pmcommandmanager.moc"
