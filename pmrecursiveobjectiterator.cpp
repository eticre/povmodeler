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

#include "pmobject.h"
#include "pmrecursiveobjectiterator.h"

PMRecursiveObjectIterator::PMRecursiveObjectIterator( PMObject* obj )
{
   m_pObject = obj;
   m_pCurrent = obj;
}

PMObject* PMRecursiveObjectIterator::operator++()
{
   if( m_pCurrent )
   {
      if( m_pCurrent->firstChild() )
         m_pCurrent = m_pCurrent->firstChild();
      else if( m_pCurrent == m_pObject )
         m_pCurrent = 0;
      else if( m_pCurrent->nextSibling() )
         m_pCurrent = m_pCurrent->nextSibling();
      else
      {
         bool stop = false;
         do
         {
            m_pCurrent = m_pCurrent->parent();
            if( !m_pCurrent )
               stop = true;
            else if( m_pCurrent == m_pObject )
            {
               // finished
               stop = true;
               m_pCurrent = 0;
            }
            else if( m_pCurrent->nextSibling() )
            {
               m_pCurrent = m_pCurrent->nextSibling();
               stop = true;
            }
         }
         while( !stop );
      }
   }
   
   return m_pCurrent;
}
