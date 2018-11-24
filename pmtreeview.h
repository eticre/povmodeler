//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2003 by Andreas Zehender
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


#ifndef PMTREEVIEW_H
#define PMTREEVIEW_H


#include <QTreeWidget>
#include <QDrag>
//Added by qt3to4:
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QDropEvent>
#include "pmobject.h"
#include "pmviewbase.h"
#include "pmviewfactory.h"
#include "pmparser.h"
 
class PMTreeViewItem;
class PMPart;


/**
 * Wrapper class for the treeview/dock widget
 */
class PMTreeViewWidget : public PMViewBase
{
public:
   /**
    * Default constructor
    */
   PMTreeViewWidget( PMPart* pare, QWidget* parent );
   
   /** */
   virtual QString viewType() const { return QString( "treeview" ); }
   /** */
   virtual QString description() const;
};

/**
 * Widget that displays the scene as tree view
 */
class PMTreeView : public QTreeWidget
{
   Q_OBJECT
   friend class PMTreeViewItem;
public:
   /**
    * Creates a PMTreeView with parent and name that displays the
    * document doc
    */
   PMTreeView(PMPart* part, QWidget *parent = 0 );
   /**
    * Deletes the PMTreeView
    */
   ~PMTreeView();

   /**
    * Returns true PMTreeViewItem::setSelected should be accepted
    */
   bool acceptSelect() const { return m_acceptSelect; }
   /**
    * Returns the connected part
    */
   PMPart* part() const { return m_pPart; }
   /**
     eticre view not_in_tree
   */
   bool show_list;
   
public slots:
   /**
    * Called when an object is changed.
    * @see PMPart::objectChanged() */
   void slotObjectChanged( PMObject* obj, const int mode, QObject* sender );
   /**
    * Refreshes the whole csg tree
    */
   void slotRefresh();
   void slotRefreshList();//eticre view<-->hide not_in_tree object
   /**
    * Clears all data
    */
   void slotClear();
   /**
     eticre view not_in_tree
   */
   void slotShowNotInTree();
   
signals:
   /**
    * Emitted, when an object is selected or deselected
    */
   void objectChanged( PMObject* obj, const int mode, QObject* sender );
   /**
    * Emitted in the destructor
    */
   void destroyed( PMTreeView* v );
   
protected:
   void mousePressEvent( QMouseEvent* e )__attribute__((optimize(0)));
   void mouseMoveEvent( QMouseEvent* e );
   void itemSelected( PMTreeViewItem* item, bool selected );
   
   void selectionManager( QMouseEvent * e );
   void mouseReleaseEvent( QMouseEvent* e );
   void moveMouseManager( QMouseEvent* e );
   void dragMoveEvent( QDragMoveEvent *e );
   void dragEnterEvent( QDragEnterEvent *e );
   void dragLeaveEvent( QDragLeaveEvent* e );
   void dropEvent( QDropEvent* e );

   void focusOutEvent( QFocusEvent* e );
   void focusInEvent( QFocusEvent* e );
   
   void keyPressEvent( QKeyEvent* e );

private:
   /**
    * Adds child items of item to the tree view
    */
   void addChildItems( PMTreeViewItem* item );
   /**
    * Returns true if the drop target is a tree view for the same part
    */
   bool targetDisplaysPart( QObject* target );
   
   /**
    * the displayed document
    */
   PMPart* m_pPart;

   /**
    * the selected items
    */
//   QPtrList<PMTreeViewItem> m_selectedItems;
   PMTreeViewItem* m_pLastSelected;
   PMTreeViewItem* m_pCurrent;
   QTreeWidgetItem* m_pOldCurrent;
   bool m_itemSelected;
   bool m_itemDeselected;
   bool m_selectionCleared;
   bool m_event;
   bool m_acceptSelect;
   bool m_selectOnReleaseEvent;
   
   PMTreeViewItem* m_pDragOverItem;
//   QStringList m_lstDropFormats;

   // for drag and drop, copied from KonqBaseListViewWidget
   bool m_pressed;
   QPoint m_pressedPos;
   PMTreeViewItem* m_pressedItem;
};

/**
 * Factory class for the tree view
 */
class PMTreeViewFactory : public PMViewTypeFactory
{
public:
   PMTreeViewFactory() { }
   virtual QString viewType() const { return QString( "treeview" ); }
   virtual QString description() const;
   virtual QString iconName() const { return QString( "pmtreeview" ); }
   virtual PMViewBase* newInstance( QWidget* parent, PMPart* part ) const
   {
      return new PMTreeViewWidget( part, parent );
   }
};

#endif
