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

#ifndef PMRECURSIVEOBJECTITERATOR_H
#define PMRECURSIVEOBJECTITERATOR_H

class PMObject;

/**
 * Iterator that
 */
class PMRecursiveObjectIterator
{
public:
   /**
    * Creates an iterator that iterates recursively over the childs
    * of obj.
    */
   PMRecursiveObjectIterator( PMObject* obj );
   /**
    * Makes the next object the current one and returns it.
    */
   PMObject* operator++();
   /**
    * Returns the current object.
    */
   PMObject* current() const { return m_pCurrent; }
   
private:
   PMObject* m_pObject;
   PMObject* m_pCurrent;
};

#endif
