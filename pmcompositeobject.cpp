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


#include "pmcompositeobject.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmviewstructure.h"

#include <QtXml/qdom.h>

PMMetaObject* PMCompositeObject::s_pMetaObject = nullptr;

PMCompositeObject::PMCompositeObject( PMPart* part )
      : Base( part )
{
   m_pFirstChild = nullptr;
   m_pLastChild = nullptr;
   m_selectedChildren = 0;
   m_bViewStructureChanged = true;
   m_pViewStructure = nullptr;
}

PMCompositeObject::PMCompositeObject( const PMCompositeObject& c )
      : Base( c )
{
   m_pFirstChild = nullptr;
   m_pLastChild = nullptr;
   m_selectedChildren = 0;
   m_bViewStructureChanged = true;
   m_pViewStructure = nullptr;

   PMObject* o = c.m_pFirstChild;
   for( ; o; o = o->nextSibling() )
      appendChild( o->copy() );
}

PMCompositeObject::~PMCompositeObject()
{
   PMObject* tmp;
   PMObject* next;

   tmp = m_pFirstChild;

   while( tmp )
   {
      next = tmp->m_pNextSibling;
      delete tmp;
      tmp = next;
   }

   if( m_pViewStructure )
      delete m_pViewStructure;
}

PMMetaObject* PMCompositeObject::metaObject() const
{
   if( !s_pMetaObject )
      s_pMetaObject = new PMMetaObject( "CompositeObject", Base::metaObject() );
   return s_pMetaObject;
}

void PMCompositeObject::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

PMObject* PMCompositeObject::childAt( uint index ) const
{
   PMObject* tmp;
   uint i = 0;

   for( tmp = m_pFirstChild; tmp && ( i < index ); tmp = tmp->nextSibling() )
      i++;
   return tmp;
}

int PMCompositeObject::findChild( PMObject* o )
{
   if( o->parent() != this )
      return -1;

   PMObject* tmp;
   int index = 0;

   for( tmp = m_pFirstChild; tmp; tmp = tmp->nextSibling() )
   {
      if( tmp == o )
         return index;
      else
         index++;
   }
   return -1;
}

bool PMCompositeObject::insertChild( PMObject* o, int i )
{
   if( i < 0 )
      return appendChild( o );
   else
   {
      if( i == 0 )
      {
         if( canInsert( o, nullptr ) )
         {
            o->m_pNextSibling = m_pFirstChild;
            o->m_pPrevSibling = 0;
            if( m_pFirstChild )
               m_pFirstChild->m_pPrevSibling = o;
            m_pFirstChild = o;
            if( !m_pLastChild )
               m_pLastChild = o;
            o->m_pParent = this;
         }
         else
            return false;
      }
      else
      {
         PMObject* tmp = childAt( ( uint ) ( i - 1 ) );
         if( !tmp )
         {
            qCritical() << "Index too big" << "\n";
            return false;
         }

         if( canInsert( o, tmp ) )
         {
            o->m_pPrevSibling = tmp;
            o->m_pNextSibling = tmp->m_pNextSibling;
            if( tmp->m_pNextSibling )
               tmp->m_pNextSibling->m_pPrevSibling = o;
            else
               m_pLastChild = o;
            tmp->m_pNextSibling = o;
            o->m_pParent = this;
         }
         else
            return false;
      }
      childAdded( o );
      return true;
   }
   return false;
}

bool PMCompositeObject::appendChild( PMObject* o )
{
   if( canInsert( o, m_pLastChild ) )
   {
      o->m_pParent = this;
      o->m_pPrevSibling = m_pLastChild;
      o->m_pNextSibling = nullptr;
      if( m_pLastChild )
         m_pLastChild->m_pNextSibling = o;
      else
         m_pFirstChild = o;
      m_pLastChild = o;

      childAdded( o );
      return true;
   }
   return false;
}

bool PMCompositeObject::insertChildAfter( PMObject* obj, PMObject* after )
{
   if( canInsert( obj, after ) )
   {
      if( after->m_pParent == this )
      {
         obj->m_pParent = this;
         obj->m_pPrevSibling = after;
         obj->m_pNextSibling = after->m_pNextSibling;
         if( after->m_pNextSibling )
            after->m_pNextSibling->m_pPrevSibling = obj;
         else
            m_pLastChild = obj;
         after->m_pNextSibling = obj;

         childAdded( obj );
         return true;
      }
      else
      {
         qCritical() << "Object after is no child" << "\n";
         return false;
      }
   }
   return false;
}

bool PMCompositeObject::insertChildBefore( PMObject* obj, PMObject* before )
{
   if( before )
   {
      if( canInsert( obj, before->m_pPrevSibling ) )
      {
         if( before->m_pParent == this )
         {
            obj->m_pParent = this;
            obj->m_pPrevSibling = before->m_pPrevSibling;
            obj->m_pNextSibling = before;
            if( before->m_pPrevSibling )
               before->m_pPrevSibling->m_pNextSibling = obj;
            else
               m_pFirstChild = obj;
            before->m_pPrevSibling = obj;

            childAdded( obj );
            return true;
         }
         else
         {
            qCritical() << "Object before is no child" << "\n";
            return false;
         }
      }
   }
   return false;
}

bool PMCompositeObject::takeChild( PMObject* o )
{
   if( ( PMObject* ) this == o->m_pParent )
   {
      // deselect the object and all child objects of o
      if( o->isSelected() )
         o->setSelected( false );
      else if( o->selectedChildren() > 0 )
         o->deselectChildren();

      // remove it, but do NOT delete it.
      if( o->m_pPrevSibling )
         o->m_pPrevSibling->m_pNextSibling = o->m_pNextSibling;
      else
         m_pFirstChild = o->m_pNextSibling;
      if( o->m_pNextSibling )
         o->m_pNextSibling->m_pPrevSibling = o->m_pPrevSibling;
      else
         m_pLastChild = o->m_pPrevSibling;

      o->m_pParent = nullptr;
      o->m_pPrevSibling = nullptr;
      o->m_pNextSibling = nullptr;

      childRemoved( o );
      return true;
   }
   qCritical() << "o is no child" << "\n";
   return false;
}

bool PMCompositeObject::takeChild( uint i )
{
   PMObject* tmp = childAt( i );
   if( tmp )
      return takeChild( tmp );
   qCritical() << "Index too big";
   return false;
}

void PMCompositeObject::serialize( QDomElement& e, QDomDocument& doc ) const
{
   PMObject* tmp;

   for( tmp = m_pFirstChild; tmp; tmp = tmp->m_pNextSibling )
      e.appendChild( tmp->serialize( doc ) );
}

int PMCompositeObject::countChildren() const
{
   int num = 0;
   PMObject* tmp;

   for( tmp = m_pFirstChild; tmp; tmp = tmp->m_pNextSibling )
      num++;
   return num;
}

void PMCompositeObject::adjustSelectedChildren( int num )
{
   m_selectedChildren += num;
   if( m_selectedChildren < 0 )
   {
      qCritical() << "num too big in PMCompositeObject::adjustSelectedChildren()\n";
      m_selectedChildren = 0;
   }
   if( m_pParent )
      m_pParent->adjustSelectedChildren( num );
}

void PMCompositeObject::deselectChildren()
{
   PMObject* tmp;
   if( m_selectedChildren > 0 )
   {
      tmp = m_pFirstChild;
      while( tmp && ( m_selectedChildren > 0 ) )
      {
         if( tmp->isSelected() )
            tmp->setSelected( false );
         else if( tmp->selectedChildren() > 0 )
            tmp->deselectChildren();

         tmp = tmp->m_pNextSibling;
      }
   }
}


PMViewStructure* PMCompositeObject::viewStructure()
{
   if( m_pViewStructure )
   {
      if( m_pViewStructure->parameterKey() != viewStructureParameterKey() )
      {
         // the default view structure or the parameters (detail level)
         // have changed
         m_bViewStructureChanged = true;
         delete m_pViewStructure;
         m_pViewStructure = nullptr;
      }
   }

   if( m_bViewStructureChanged )
   {
      PMViewStructure* dvs = defaultViewStructure();
      if( dvs )
         if( dvs->parameterKey() == -1 ) // newly created view structure
            dvs->setParameterKey( viewStructureParameterKey() );

      if( isDefault() )
      {
         if( dvs )
         {
            if( m_pViewStructure )
            {
               if( *m_pViewStructure != *dvs )
               {
                  delete m_pViewStructure;
                  m_pViewStructure = new PMViewStructure( dvs );
               }
            }
            else
               m_pViewStructure = new PMViewStructure( dvs );
         }

         if( !m_pViewStructure )
            qCritical() << "isDefault() returned true, but no default view structure is provided\n";
      }
      else
      {
         if( dvs )
         {
            if( m_pViewStructure && ( *m_pViewStructure == *dvs ) )
            {
               delete m_pViewStructure;
               m_pViewStructure = nullptr;
            }
         }
         createViewStructure();
         if( m_pViewStructure )
            m_pViewStructure->setParameterKey( viewStructureParameterKey() );
      }
      m_bViewStructureChanged = false;
   }
   return m_pViewStructure;
}

void PMCompositeObject::setViewStructureChanged()
{
   m_bViewStructureChanged = true;
   if( m_pMemento )
      m_pMemento->setViewStructureChanged();
}

