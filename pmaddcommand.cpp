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


#include "pmaddcommand.h"
#include "pmcommandmanager.h"
#include "pmpart.h"
#include "pmdeclare.h"
#include "pmerrorflags.h"
#include "pmrecursiveobjectiterator.h"
#include "pmmemento.h"



PMAddCommand::PMAddCommand( PMObject* obj, PMObject* parent, PMObject* after )
      : PMCommand( ( "Add New " + obj->description() ) )
{
   m_objects.append( obj );
   m_pParent = parent;
   m_pAfter = after;
   m_executed = false;
   m_firstExecution = true;
   m_linksCreated = false;
   m_pParentChangeMemento = nullptr;
}

PMAddCommand::PMAddCommand( const PMObjectList& list, PMObject* parent,
                            PMObject* after )
      : PMCommand( ( "Add Objects" ) )
{
   m_objects = list;
   m_pParent = parent;
   m_pAfter = after;
   m_executed = false;
   m_firstExecution = true;
   m_linksCreated = false;
   m_pParentChangeMemento = nullptr;
}

PMAddCommand::~PMAddCommand()
{
   if( !m_executed )
   {
		foreach(PMObject* obj, m_objects) delete obj;
      m_objects.clear();
   }
	foreach(PMObject* obj, m_insertErrors) delete obj;
}

void PMAddCommand::execute( PMCommandManager* theManager )
{
   if( !m_executed )
   {
      PMObjectList::iterator it;
      PMObject* prev = m_pAfter;
      PMObjectList errors;
      PMObject* current;
      bool error = false;

      if( m_firstExecution )
         if( m_pParent->dataChangeOnInsertRemove() )
            m_pParent->createMemento();
      
      for( it = m_objects.begin(); it != m_objects.end(); ++it )
      {
         current = *it;
         if( !prev )
         {
            if( m_pParent->canInsert( current, nullptr ) )
            {
               m_pParent->insertChild( current, 0 );
               prev = current;
               theManager->cmdObjectChanged( current, PMCAdd );
            }
            else
               error = true;
         }
         else
         {
            if( m_pParent->canInsert( current, prev ) )
            {
               m_pParent->insertChildAfter( current, prev );
               prev = current;
               theManager->cmdObjectChanged( current, PMCAdd );
            }
            else
               error = true;
         }
         
         if( error )
         {
            errors.append( current );
            theManager->cmdObjectChanged( current, PMCAdd | PMCInsertError );
            if( current->isA( "Declare" ) )
            {
               // the object, that couldn't be inserted was a declare,
               // remove all links
					const PMObjectList& links =
                        ( static_cast<PMDeclare*>(current) )->linkedObjects();
               foreach( PMObject* l, links )
               {
                  if( l->parent() )
                     l->parent()->takeChild( l );
                  else
                     m_objects.removeAll( l );
                  m_insertErrors.append( l );
               }
            }
            if( current->linkedObject() )
               current->linkedObject()->removeLinkedObject( current );
            error = false;
         }
      }

      if( m_pParent->mementoCreated() )
         m_pParentChangeMemento = m_pParent->takeMemento();

      if( m_pParentChangeMemento )
      {
			const QList<PMObjectChange*>& changed = m_pParentChangeMemento->changedObjects();
			QList<PMObjectChange*>::const_iterator c;
            for( c = changed.begin(); c != changed.end(); ++c )
            theManager->cmdObjectChanged( ( *c )->object(),
                                          ( *c )->mode() );
      }

      if( m_linksCreated )
      {
			foreach( PMObject* l, m_links )
            l->linkedObject()->addLinkedObject( l );
			foreach( PMObject* obj, m_linkedDeclares )
            theManager->cmdObjectChanged( obj, PMCData );
      }
      
      PMObjectList::iterator errorit;
      for( errorit = errors.begin(); errorit != errors.end(); ++errorit )
      {
         m_objects.removeAll( *errorit );
         m_insertErrors.append( *errorit );

         PMRecursiveObjectIterator lit( *errorit );
         for( ; lit.current(); ++lit )
            if( lit.current()->linkedObject() )
               lit.current()->linkedObject()->removeLinkedObject( lit.current() );
      }
      
      m_executed = true;
      m_firstExecution = false;
   }
}

void PMAddCommand::undo( PMCommandManager* theManager )
{
   if( m_executed )
   {
		PMObjectList::iterator it;
      PMDeclare* decl;

      if( !m_linksCreated )
      {
         for(it = m_objects.begin(); it != m_objects.end(); ++it )
         {
            PMRecursiveObjectIterator lit( *it );
            for( ; lit.current(); ++lit )
            {
               decl = lit.current()->linkedObject();
               if( decl )
               {
                  m_links.append( lit.current() );
                  if( !m_linkedDeclares.contains( decl ) )
                     m_linkedDeclares.append( decl );
               }
            }
         }
         m_linksCreated = true;
      }

		foreach( PMObject* l, m_links )
            l->linkedObject()->removeLinkedObject( l );

        it = m_objects.end();
        while( it != m_objects.begin() )
		{
			--it;
         // signal has to be emitted before the item is removed
         theManager->cmdObjectChanged( *it, PMCRemove );

         if( ( *it )->parent() )
            ( *it )->parent()->takeChild( ( * it) );
      }

      if( m_pParentChangeMemento )
      {
         m_pParent->restoreMemento( m_pParentChangeMemento );
            const QList<PMObjectChange*>& changed = m_pParentChangeMemento->changedObjects();
			QList<PMObjectChange*>::const_iterator c;
         for( c = changed.begin(); c != changed.end(); ++c )
            theManager->cmdObjectChanged( ( *c )->object(),
                                          ( *c )->mode() );
      }

		foreach(PMObject* l, m_linkedDeclares )
         theManager->cmdObjectChanged( l, PMCData );
      
      m_executed = false;
   }
}

int PMAddCommand::errorFlags( PMPart* )
{
   return PMENone;
}
