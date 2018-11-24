//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMPRISMMEMENTO_H
#define PMPRISMMEMENTO_H



#include "pmmemento.h"
#include "pmvector.h"
#include <QList>


/**
 * Memento for @ref PMPrism
 */
class PMPrismMemento : public PMMemento
{
public:
   /**
    * Creates a memento for the object originator
    */
   PMPrismMemento( PMObject* originator );
   /**
    * Deletes the memento
    */
   virtual ~PMPrismMemento();

   /**
    * Saves the prism points
    */
   void setPrismPoints( const QList< QList<PMVector> >& v );
   /**
    * Returns the prism points
    */
   QList< QList<PMVector> > prismPoints() const
   {
      return m_prismPoints;
   }
   /**
    * Returns true if the prism points were saved
    */
   bool prismPointsSaved() const { return m_bPrismPointsSaved; }
   
private:   
   /**
    * The stored points
    */
   QList< QList<PMVector> > m_prismPoints;
   bool m_bPrismPointsSaved;
};

#endif
