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


#include "pmmovecommand.h"
#include "pmobject.h"
#include "pmpart.h"
#include "pmcommandmanager.h"
#include "pmdeclare.h"
#include "pmerrorflags.h"
#include "pmrecursiveobjectiterator.h"
#include "pmmemento.h"
#include "pmdebug.h"


#include <QHash>

PMMoveCommand::PMMoveCommand( PMObject* obj, PMObject* parent, PMObject* after )
      : PMCommand( ( "Move " + obj->description() ) )
{
   m_pParent = parent;
   m_pAfter = after;

   if( obj->parent() )
   {
      m_infoList.append( new PMDeleteInfo( obj ) );
   }
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
}

PMMoveCommand::PMMoveCommand( const PMObjectList& list, PMObject* parent,
                            PMObject* after )
      : PMCommand( ( "Move Objects" ) )
{
	PMObjectList::const_iterator it;

   for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->parent() )
         m_infoList.append( new PMDeleteInfo( ( *it ) ) );
      else
      {
         // object has no parent!
         // top level objects can't be moved, move all child items
         PMObject* tmp;
         for( tmp = ( *it )->firstChild(); tmp; tmp = tmp->nextSibling() )
            m_infoList.append( new PMDeleteInfo( tmp ) );
      }
   }  

   m_pParent = parent;
   m_pAfter = after;
   m_executed = false;
   m_firstExecution = true;
}

PMMoveCommand::~PMMoveCommand()
{
   if( m_executed )
   {
		foreach(PMObject* obj, m_insertErrors) delete obj;
      m_insertErrors.clear();
   }
}

void PMMoveCommand::execute( PMCommandManager* theManager )
{
   if( !m_executed )
   {
      // remove objects
      QList<PMDeleteInfo*>::iterator it;
      PMObject* prev = m_pAfter;
      PMObject* obj;
      bool error = false;
      PMDeleteInfo* info = 0;
      PMObject* parent;

		it = m_infoList.end();
      while( it != m_infoList.begin() )
      {
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

      // insert at new position
      if( m_firstExecution )
         if( m_pParent->dataChangeOnInsertRemove()
             && !m_pParent->mementoCreated() )
            m_pParent->createMemento();
               
      for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
      {
         if( !( *it )->insertError() )
         {
            obj = ( *it )->deletedObject();

            if( !prev )
            {
               if( m_pParent->canInsert( obj, 0 ) )
               {
                  m_pParent->insertChild( obj, 0 );
                  prev = obj;
                  theManager->cmdObjectChanged( obj, PMCAdd );
               }
               else
                  error = true;
            }
            else
            {
               if( m_pParent->canInsert( obj, prev ) )
               {
                  m_pParent->insertChildAfter( obj, prev );
                  prev = obj;
                  theManager->cmdObjectChanged( obj, PMCAdd );
               }
               else
                  error = true;
            }
            
            if( error )
            {
               m_insertErrors.append( ( *it )->deletedObject() );
               ( *it )->setInsertError();
               theManager->cmdObjectChanged( obj, PMCAdd | PMCInsertError );
               error = false;
            }
         }
      }

      if( m_firstExecution )
      {
         if( m_pParent->mementoCreated() )
            m_dataChanges.append( m_pParent->takeMemento() );

			it = m_infoList.end();
			while( it != m_infoList.begin() ){
				--it;
            info = ( *it );
            parent = info->parent();

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

		m_executed = true;
      m_firstExecution = true;
   }
}

void PMMoveCommand::undo( PMCommandManager* theManager )
{
   if( m_executed )
   {
      QList<PMDeleteInfo*>::iterator it = m_infoList.end();
      PMObject* obj;
		while( it != m_infoList.begin() ){
			--it;
         // signal has to be emitted before the item is removed
         obj = ( *it )->deletedObject();
         theManager->cmdObjectChanged( obj, PMCRemove );
         if( obj->parent() )
            obj->parent()->takeChild( obj );
      }
      
      for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
      {
         obj = ( *it )->deletedObject();
         if( ( *it )->prevSibling() )
            ( *it )->parent()
               ->insertChildAfter( obj, ( *it )->prevSibling() );
         else
            ( *it )->parent()->insertChild( obj, 0 );
         theManager->cmdObjectChanged( obj, PMCAdd );
      }

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

int PMMoveCommand::errorFlags( PMPart* )
{
   PMDeleteInfo* info;
   PMDeclare* decl = 0;
   PMObject* obj;
   bool insideSelection;
   bool ok = true;
   bool declareInsertError = false;
   bool error = false;
   bool stop;

   // dictionary of deleted objects
   QHash<PMObject*, bool> deletedObjects;
   QHash<PMObject*, bool> objectsAfterInsertPosition;
   QHash<PMObject*, bool> declaresBeforeInsertPosition;


   QList<PMDeleteInfo*>::iterator it;
	for( it = m_infoList.begin(); it != m_infoList.end(); ++it )
      deletedObjects.insert( ( *it )->deletedObject(), true );

   // declares can only be moved, if all linked
   // objects are moved as well or the insert position is before the first
   // linked object and all declares can be inserted

	it = m_infoList.end();

	while( it != m_infoList.begin() )
   {
		--it;
		info = *it;
      ok = true;
      declareInsertError = false;
      
      if( info->deletedObject()->isA( "Declare" ) )
      {
         decl = ( PMDeclare* ) ( info->deletedObject() );
         
         if( !m_pParent->canInsert( decl, m_pAfter ) )
         {
            declareInsertError = true;
            ok = false;
         }
         else
         {
				const PMObjectList& links = decl->linkedObjects();
				PMObjectList::const_iterator lit;
            
            for( lit = links.begin(); lit != links.end() && ok; ++lit )
            {
               insideSelection = false;
               for( obj = ( *lit ); obj && !insideSelection;
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
               {                  
                  // link will not be moved
                  // check, if after insert position
                  // declares can only be inserted as childs of the scene

                  if( m_pAfter )
                  {
                     // insert point is not the first item
                     // find the top level parent item
                     stop = false;
                     obj = *lit;
                     do
                     {
                        if( obj->parent() )
                        {
                           if( obj->parent()->type() == "Scene" )
                              stop = true;
                           else
                              obj = obj->parent();
                        }
                        else
                           stop = true; // error
                     }
                     while( !stop );

                     PMObject* topParent = obj;

                     // check if insert point is before the top level
                     // parent object
                     
                     if( !objectsAfterInsertPosition.contains( obj ) )
                     {
                        stop = false;
                        obj = obj->prevSibling();
                        while( obj && !stop )
                        {
                           if( obj == m_pAfter )
                              stop = true;
                           else if( objectsAfterInsertPosition.contains( obj ) )
                              stop = true;
                           else
                              obj = obj->prevSibling();
                        }
                        
                        if( stop )
                           objectsAfterInsertPosition.insert( topParent, true );
                        else
                           ok = false;
                     }
                  }
               }
            }
         }
      }
      
      if( !ok )
      {
         if( declareInsertError )
            m_errors.prepend( ( "Cannot insert the declare " + decl->id() + " at that point." ) );
         else
            m_errors.prepend( ( "The declare " + decl->id() + " cannot be moved behind linked objects." ) );
         
         deletedObjects.remove( info->deletedObject() );
         it = m_infoList.erase( it );         
         error = true;
      }
   }

   // check if links are moved before the declare object

   // find all declares bevore the insert position
   if( m_pParent->type() == "Scene" )
      obj = m_pAfter;
   else
   {
      stop = false;
      obj = m_pParent;
      do
      {
         if( obj->parent() )
         {
            if( obj->parent()->type() == "Scene" )
               stop = true;
            else
               obj = obj->parent();
         }
         else
            stop = true; // error
      }
      while( !stop );
      
      if( obj )
         obj = obj->prevSibling();
   }
   
   while( obj )
   {
      if( obj->isA( "Declare" ) )
         declaresBeforeInsertPosition.insert( obj, true );
      obj = obj->prevSibling();
   }
   
   it = m_infoList.begin();
   while( it != m_infoList.end() )
   {
		info = *it;
      ok = true;
      PMRecursiveObjectIterator oit( info->deletedObject() );
      PMObject* linked = 0;
      
      while( oit.current() && ok )
      {
         linked = oit.current()->linkedObject();

         if( linked )
         {
            if( !deletedObjects.contains( linked ) )
            {
               // Object is linked to a declare and the declare
               // will not be moved.
               // Check, if the insert point is after the declare

               if( !declaresBeforeInsertPosition.contains( linked ) )
                  ok = false;
            }
         }

         if( ok )
            ++oit;
      }
      
      if( ok )
         ++it;
      else
      {
         if( linked )
         {
            obj = info->deletedObject();
            QString name = obj->name();
            decl = ( PMDeclare* ) linked;
            
            if( name.isEmpty() )
               name = ( "(unnamed)" );
            m_errors.append( ( "The " +obj->description() + " " +  name + " cannot be moved because it "
                                   "contains a link to the declare " + decl->id() + " "
                                   "and the insert point is not after "
                                   "the declare." ) );
         }
         else
            qCritical(  ) << "linked is 0 in PMMoveCommand::errorFlags\n";
         
         deletedObjects.remove( info->deletedObject() );
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
