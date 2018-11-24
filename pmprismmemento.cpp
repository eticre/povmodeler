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

#include "pmprismmemento.h"
#include <QList>

PMPrismMemento::PMPrismMemento( PMObject* originator )
      : PMMemento( originator )
{
   m_bPrismPointsSaved = false;
}

PMPrismMemento::~PMPrismMemento()
{
}

void PMPrismMemento::setPrismPoints( const QList< QList<PMVector> >& v )
{
   if( !m_bPrismPointsSaved )
   {
      // Direct assignment does not work with Qt 2.3.x
      // The list will be changed later in a graphical
      // change because QValueList::detach() is called
      // too late!
      // Copy the list by hand.

      QList< QList< PMVector > >::ConstIterator sit = v.begin();
      for( ; sit != v.end(); ++sit )
      {
         QList<PMVector> list;
         QList<PMVector>::ConstIterator it = ( *sit ).begin();
         for( ; it != ( *sit ).end(); ++it )
            list.append( *it );
         m_prismPoints.append( list );
      }
      
      m_bPrismPointsSaved = true;
      addChange( PMCData );
   }
}

   
