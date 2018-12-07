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


#include "pminsertpopup.h"
#include <QCursor>

PMInsertPopup::PMInsertPopup( QWidget* parent, bool multipleObjects,
                              int items, bool canInsertAllAsFirstChildren,
                              bool canInsertAllAsLastChildren,
                              bool canInsertAllAsSiblings, const char* )
      : QMenu( parent )
{
   QString itemText;
   if( multipleObjects )
   {
      setTitle( "Insert Objects As" );
      if( ( items & PMIFirstChild ) == PMIFirstChild )
      {
         itemText = "First Children";

         if( !canInsertAllAsFirstChildren )
             itemText += " (some)";

         addAction( QIcon( ":/smallicon/icons/povicons/small/pminsertfirstchild" ), itemText);
      }
      if( ( items & PMILastChild ) == PMILastChild )
      {
         itemText = "Last Children";

         if( !canInsertAllAsLastChildren )
            itemText.append( " (some)" );

         addAction( QIcon( ":/smallicon/icons/povicons/small/pminsertlastchild" ), itemText );
      }
      if( ( items & PMISibling ) == PMISibling )
      {
         itemText = tr( "Siblings" );

         if( !canInsertAllAsSiblings )
            itemText.append( " ( some ) " );

         addAction( QIcon( ":/smallicon/icons/povicons/small/pminsertsibling" ), itemText );
      }
   }
   else
   {
      setTitle( "Insert Object As" );

      if( ( items & PMIFirstChild ) == PMIFirstChild )
         addAction( QIcon( ":/smallicon/icons/povicons/small/pminsertfirstchild" ), "First Child" );

      if( ( items & PMILastChild ) == PMILastChild )
         addAction( QIcon( ":/smallicon/icons/povicons/small/pminsertlastchild" ), "Last Child" );

      if( ( items & PMISibling ) == PMISibling )
         addAction( QIcon( ":/smallicon/icons/povicons/small/pminsertsibling" ), "Sibling" );
   }
}

int PMInsertPopup::choosePlace( QWidget* parent, bool multipleObjects,
                                int items, bool canInsertAllAsFirstChildren,
                                bool canInsertAllAsLastChildren,
                                bool canInsertAllAsSiblings )
{
   int result;
   PMInsertPopup* popup = new PMInsertPopup( parent, multipleObjects, items,
                                             canInsertAllAsFirstChildren,
                                             canInsertAllAsLastChildren,
                                             canInsertAllAsSiblings );

   result = 1 + popup->actions().indexOf( popup->exec( QCursor::pos() ) );

   if( result < 0 )
      result = 0;

   delete popup;

   return result;
}
