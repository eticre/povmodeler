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


#include "pmdeletecommand.h"
#include "pmcommandmanager.h"
#include "pmdeclare.h"
#include "pmrecursiveobjectiterator.h"
#include "pmmemento.h"

#include <QHash>

PMDeleteCommand::PMDeleteCommand( PMObject* obj )
      : PMCommand( ( "Delete " + obj->name() ) )
{
   // the scene can not be deleted!
   if( obj->parent() )
      m_infoList.append( new PMDeleteInfo( obj ) );
   else
   {
      // object has no parent!
      // top level objects can't be moved, move all child items
      PMObject* tmp;
      for( tmp = obj->firstChild(); tmp; tmp = tmp->nextSibling() )
         m_infoList.append( new PMDeleteInfo( tmp ) );
   }
   m_executed = false;
   m_firstExecution = true;
   m_linksCreated = false;
}

PMDeleteCommand::PMDeleteCommand( const PMObjectList& list )
      : PMCommand( ( "Delete Objects" ) )
{
	PMObjectList::const_iterator it;

   for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->parent() )  
         m_infoList.append( new PMDeleteInfo( *it ) );
      else
      {
         // object has no parent!
         // top level objects can't be moved, move all child items
         PMObject* tmp;
         for( tmp = ( *it )->firstChild(); tmp; tmp = tmp->nextSibling() )
            m_infoList.append( new PMDeleteInfo( tmp ) );
      }
   }
         
   m_executed = false;
   m_firstExecution = true;
   m_linksCreated = false;
}

PMDeleteCommand::~PMDeleteCommand()
{
	QList<PMDeleteInfo*>::iterator it;
   if( m_executed )
		for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
         delete ( ( *it )->deletedObject() );

	for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
		delete( *it );
   m_infoList.clear();
}

void PMDeleteCommand::execute( PMCommandManager* theManager )
{
   if( !m_executed )
   {
      QList<PMDeleteInfo*>::iterator it;
      PMDeleteInfo* info = 0;
      PMObject* parent;
      
      if( !m_linksCreated )
      {
         PMDeclare* decl;
			for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
         {
            PMRecursiveObjectIterator oit( ( *it )->deletedObject() );
            for( ; oit.current(); ++oit )
            {
               decl = oit.current()->linkedObject();
               if( decl )
               {
                  m_links.append( oit.current() );
                  if( !m_linkedDeclares.contains( decl ) )
                     m_linkedDeclares.append( decl );                  
               }
            }
         }
         m_linksCreated = true;
      }

		foreach( PMObject* l, m_links )
         l->linkedObject()->removeLinkedObject( l );

		it = m_infoList.end();
		while( it != m_infoList.begin() ) {
			--it;
         info = *it;
         parent = info->parent();
         // signal has to be emitted before the item is removed
         theManager->cmdObjectChanged( info->deletedObject(), PMCRemove );
         if( m_firstExecution )
            if( parent->dataChangeOnInsertRemove()
                && !parent->mementoCreated() )
               parent->createMemento();
         parent->takeChild( info->deletedObject() );
      }

      if( m_firstExecution )
      {
			it = m_infoList.end();
			while( it != m_infoList.begin() ) {
				--it;
            parent = ( *it )->parent();
            if( parent->mementoCreated() )
               m_dataChanges.append( parent->takeMemento() );
         }
      }

      QList<PMMemento*>::iterator mit;
		for( mit = m_dataChanges.begin(); mit != m_dataChanges.end(); ++mit )
      {
			const QList<PMObjectChange*>& changed = ( *mit )->changedObjects();
			QList<PMObjectChange*>::const_iterator c;
         for( c = changed.begin(); c != changed.end(); ++c )
            theManager->cmdObjectChanged( ( *c )->object(),
                                          ( *c )->mode() );
      }

		foreach( PMObject* l, m_linkedDeclares )
         theManager->cmdObjectChanged( l, PMCData );
      
      m_executed = true;
      m_firstExecution = false;
   }
}

void PMDeleteCommand::undo( PMCommandManager* theManager )
{
   if( m_executed )
   {
      QList<PMDeleteInfo*>::iterator it;
		for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
      {
         if( ( *it )->prevSibling() )
            ( *it )->parent()
               ->insertChildAfter( ( *it )->deletedObject(),
                                   ( *it )->prevSibling() );
         else
            ( *it )->parent()
               ->insertChild( ( *it )->deletedObject(), 0 );
         theManager->cmdObjectChanged( ( *it )->deletedObject(), PMCAdd );
      }

		foreach( PMObject* l, m_links )
         l->linkedObject()->addLinkedObject( l );
		foreach( PMObject* l, m_linkedDeclares )
         theManager->cmdObjectChanged( l, PMCData );
      
      QList<PMMemento*>::iterator mit;
		for( mit = m_dataChanges.begin(); mit != m_dataChanges.end(); ++mit )
      {
         ( *mit )->originator()->restoreMemento( *mit );
			const QList<PMObjectChange*>& changed = ( *mit )->changedObjects();
			QList<PMObjectChange*>::const_iterator c;
         for( c = changed.begin(); c != changed.end(); ++c )
            theManager->cmdObjectChanged( ( *c )->object(),
                                          ( *c )->mode() );
      }
      
      m_executed = false;
   }
}

int PMDeleteCommand::errorFlags( PMPart* )
{
   PMDeleteInfo* info;
   PMDeclare* decl = 0;
   PMObject* obj;
   bool insideSelection;
   bool ok = true;
   bool error = false;
   
   // dictionary of deleted objects
   QHash<PMObject*, bool> deletedObjects;
   QList<PMDeleteInfo*>::iterator it;
	for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
      deletedObjects.insert( ( *it )->deletedObject(), true );
   
   // declares can only be deleted, if all linked
   // objects are deleted as well

	it = m_infoList.end();

	while( it != m_infoList.begin() )
   {
		--it;
		info = *it;
      ok = true;
      if( info->deletedObject()->isA( "Declare" ) )
      {
         decl = ( PMDeclare* ) ( info->deletedObject() );
			const PMObjectList& links = decl->linkedObjects();
			PMObjectList::const_iterator lit;
         
         for( lit = links.begin(); lit != links.end() && ok; ++lit )
         {
            insideSelection = false;
            for( obj = *lit; obj && !insideSelection;
                 obj = obj->parent() )
            {
               if( deletedObjects.contains( obj ) )
                  insideSelection = true;
            }

            if( insideSelection )
            {
               bool stop = false;
               for( obj = *lit; obj && !stop; obj = obj->parent() )
               {
                  if( deletedObjects.contains( obj ) )
                     stop = true;
                  else
                     deletedObjects.insert( obj, true );
               }
            }
            else
               ok = false;
         }
      }
      
      if( !ok )
      {
         m_errors.prepend( ( "The declare " + decl->id() + " can not be removed "
                                 "because of some remaining links." ) );
         
         deletedObjects.remove( decl );
         it = m_infoList.erase( it );         
         error = true;
      }
   }

   if( error )
   {
      if( m_infoList.count() == 0 )
         return PMEError | PMEFatal;
      else
         return PMEError;
   }
   
   return PMENone;
}
