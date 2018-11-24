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


#ifndef PMTREEVIEWITEM_H
#define PMTREEVIEWITEM_H



#include <QTreeWidget>

class PMObject;

/**
 * QListViewItem for a @ref PMObject
 *
 * Each PMListViewItem is connected to a PMObject.
 */
class PMTreeViewItem : public QTreeWidgetItem
{
public:

   /**
    * Constructs a new top-level list view item in the QListView parent.
    */
   PMTreeViewItem( PMObject* object, QTreeWidget* parent );
   /**
    * Constructs a new list view item which is a child of parent and
    * first in the parent's list of children.
    */
   PMTreeViewItem( PMObject* object );
   /**
    * Constructs a list view item which is a child of parent
    * and is after after in the parent's list of children.
    */
   PMTreeViewItem( PMObject* object, QTreeWidget* parent,
                   QTreeWidgetItem* after );
   /**
    * Constructs a list view item which is a child of parent
    * and is after after in the parent's list of children.
    */
   PMTreeViewItem( PMObject* object, QTreeWidgetItem* parent,
                   QTreeWidgetItem* after );
   /**
    * Returns the connected @ref PMObject
    */
   PMObject* object() const { return m_pObject; }
   /**
    * Returns a key that can be used for sorting, here the index in the
    * parents list of children
    */
   virtual QString key( int column, bool ascending ) const;
   /**
    * Returns a pointer to the parent item
    */
   PMTreeViewItem* parent()
   {
      return ( PMTreeViewItem* ) QTreeWidgetItem::parent();
   }
   void setSelected( bool select )__attribute__( ( optimize( 0 ) ) );
   /**
    * Sets the text and pixmap
    */
   void setDescriptions();
private:
   /**
    * Initializes the selection at creation
    */
   void initSelection();
   PMObject* m_pObject;
};

#endif
