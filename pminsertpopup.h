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


#ifndef PMINSERTPOPUP_H
#define PMINSERTPOPUP_H



#include <QMenu>

/**
 * Popup menu. Ask the user where to insert new objects
 */
class PMInsertPopup : public QMenu
{
public:
   /**
    * Places where a new object can be inserted
    */
   enum { PMIFirstChild = 1, PMILastChild = 2, PMISibling = 4 };
   /**
    * Creates a popup menu
    *
    * @param parent The parent widget
    * @param multipleObjects True if more than one object will be inserted
    * @param items Which items to display. Can be a bitwise combination
    *              of PMIFirstChild, PMILastChild, PMISibling.
    * @param name Internal name of the popup menu
    */
   PMInsertPopup( QWidget* parent, bool multipleObjects,
                  int items = PMIFirstChild | PMILastChild | PMISibling,
                  bool canInsertAllAsFirstChildren = true,
                  bool canInsertAllAsLastChildren = true,
                  bool canInsertAllAsSiblings = true,
                  const char* name = 0 );
   /**
    * Deletes the popup menu
    */
   ~PMInsertPopup() { }

   /**
    * Popups a PMInsertPopup menu
    *
    * @param parent The parent widget
    * @param multipleObjects True if more than one object will be inserted
    * @param items Which items to display. Can be a bitwise combination
    *              of PMIFirstChild, PMILastChild, PMISibling.
    * @param canInsertAllAsChildren If false the text "(some)" will appear
    *              behind the "first Children" and "last Children" items
    * @param canInsertAllAsSiblings If false the text "(some)" will appear
    *              behind the "Siblings" items
    *
    * Returns the selected item or 0 if no item was selected.
    */
   static int choosePlace( QWidget* parent, bool multipleObjects,
                           int items = PMIFirstChild | PMILastChild
                           | PMISibling, 
                           bool canInsertAllAsFirstChildren = true,
                           bool canInsertAllAsLastChildren = true,
                           bool canInsertAllAsSiblings = true );
};

#endif
