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


#include <stdlib.h>
#include <QLayout>
#include <QMenu>
#include <QCursor>
//Added by qt3to4:
#include <QDragLeaveEvent>
#include <QFocusEvent>
#include <QDragMoveEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QApplication>

#include "pmtreeview.h"
#include "pmtreeviewitem.h"
#include "pmcommand.h"
#include "pmpart.h"
#include "pmscene.h"
#include "pmobjectdrag.h"
#include "pmdeclare.h"
#include <QTreeWidgetItem>
#include <QtDebug>

PMTreeViewWidget::PMTreeViewWidget( PMPart* part, QWidget* parent /*= 0*/)
      : PMViewBase( parent )
{
   QHBoxLayout* hl = new QHBoxLayout( this );
   PMTreeView* tv = new PMTreeView( part, this );
   hl->addWidget( tv );
}

QString PMTreeViewWidget::description() const
{
   return "Object Tree";
}

PMTreeView::PMTreeView( PMPart* part, QWidget* parent/*= 0*/ )
    : QTreeWidget( parent )
{
   //addColumn( tr( "Objects" ) );
   setColumnCount( 1 );
   setHeaderHidden( true );
   setRootIsDecorated( true );
   setSortingEnabled( false );
   setDragEnabled( true );
   viewport()->setAcceptDrops( true );
   setDropIndicatorShown( true );
   setDragDropMode( QAbstractItemView::InternalMove );
   //setSorting( -1 );
   //setSelectionMode( QAbstractItemView::MultiSelection );
   m_pPart = part;

   m_itemSelected = false;
   m_itemDeselected = false;
   m_selectionCleared = false;
   m_pLastSelected = nullptr;
   m_event = false;
   m_pressed = false;
   m_pDragOverItem = nullptr;
   m_acceptSelect = false;
   m_pressedItem = nullptr;
   show_list = false;//eticre view<-->hide not_in_tree
   m_pCurrent = nullptr;
   m_pOldCurrent = nullptr;

   viewport()->setAcceptDrops( true );
   viewport()->setMouseTracking( true );
   viewport()->setFocusPolicy( Qt::WheelFocus );
   setFocusPolicy( Qt::WheelFocus );
   setAcceptDrops( true );

   connect( part, SIGNAL( refresh() ), SLOT( slotRefresh() ) );

   connect( part, SIGNAL( refreshList() ), SLOT( slotRefreshList() ) );

   connect( part, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
                  SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   connect( part, SIGNAL( clear() ), SLOT( slotClear() ) );

   connect( this, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
            part, SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   slotRefresh();
}

PMTreeView::~PMTreeView()
{
   emit destroyed( this );
}

void PMTreeView::slotShowNotInTree()
{
}

void PMTreeView::slotObjectChanged( PMObject* obj, const int mode,
                                    QObject* sender )
{
   PMTreeViewItem* pTreeItem = nullptr;
   bool as = m_acceptSelect;
   m_acceptSelect = true;

   if( sender != this )
   {
      if( ( mode & PMCAdd ) && !( mode & PMCInsertError ) )
      {
         if( !obj->parent() )
            pTreeItem = new PMTreeViewItem( obj, this );//topLevelItem
         else
            pTreeItem = new PMTreeViewItem( obj );//child and children
      }
      if( mode & PMCDescription )
      {
         if( !pTreeItem )
            pTreeItem = obj->getTreeItem();

         if( pTreeItem )
            pTreeItem->setDescriptions();
      }
      if( mode & PMCChildren )
      {
         if( !pTreeItem )
             pTreeItem = obj->getTreeItem();

         if( pTreeItem )
         {
            pTreeItem->takeChildren();
            // create new
            //addChildItems( pTreeItem );
            pTreeItem = new PMTreeViewItem( pTreeItem->object() );
            pTreeItem->parent()->setExpanded( true );
         }
      }
      if( mode & PMCNewSelection )
      {
         clearSelection();
         obj->getTreeItem()->setSelected( true );
         setCurrentItem( obj->getTreeItem() );
         obj->getTreeItem()->setExpanded( true );
      }
      if( mode & PMCDeselected )
      {
         if( !pTreeItem )
            obj->getTreeItem()->setSelected( false );
      }
      if( mode & PMCSelected )
      {
         if( !pTreeItem )
            obj->getTreeItem()->setSelected( true );
      }
      if( mode & PMCRemove )
      {
         // object was removed, remove the listview item
         if( !pTreeItem && obj->getTreeItem() != nullptr )
         {
             obj->getTreeItem()->takeChildren();
             delete obj->getTreeItem();
             obj->setTreeItem( nullptr );
         }
         pTreeItem = nullptr;
      }
      if( mode & PMCData )
      {
         // special case for texture maps
         if( obj )
         {
            if( obj->isA( "TextureMapBase" ) )
            {
               if( !pTreeItem )
                  pTreeItem = obj->getTreeItem();
               if( pTreeItem )
               {
                  for( int i = 0; i < pTreeItem->childCount(); ++i )
                  {
                      PMTreeViewItem* item = ( PMTreeViewItem* ) pTreeItem->child( i );
                      item->setDescriptions();
                  }
               }
            }
         }
      }
   }
   m_acceptSelect = as;
}

void PMTreeView::addChildItems( PMTreeViewItem* item )
{
   PMObject* obj = nullptr;
   PMTreeViewItem* listItem = nullptr;

   for( obj = item->object()->firstChild(); obj; obj = obj->nextSibling() )
   {
      if( !obj->isVisibleInTree() || show_list )
      {
          listItem = new PMTreeViewItem( obj );
          if( obj->countChildren() > 0  && listItem )
              addChildItems( listItem );
      }
   }
}

//remake the treeView
void PMTreeView::slotRefresh()
{
   slotClear();
   if( m_pPart->scene() )
   {
      setProperty( "showList", show_list );
      new PMTreeViewItem( m_pPart->scene(), this );//toplevel
      //item->setExpanded( true );
      //addChildItems( item );
      new PMTreeViewItem( m_pPart->scene() );//children
   }
}

/**eticre view not_in_tree object**/
void PMTreeView::slotRefreshList()
{
   if( show_list )
       show_list = false;
   else
       show_list = true;

   slotRefresh();
}

void PMTreeView::slotClear()
{
   clear();
   m_pLastSelected = nullptr;
   m_pDragOverItem = nullptr;
   m_pressedItem = nullptr;
}

void PMTreeView::itemSelected( PMTreeViewItem* item, bool selected )
{
   //repaintItem( item );
   repaint();
   update();
   if( m_event )
   {
      m_pLastSelected = item;

      if( selected )
         m_itemSelected = true;
      else
      {
         if( m_itemDeselected )
            m_selectionCleared = true;
         else
            m_itemDeselected = true;
      }
   }
}

void PMTreeView::selectionManager( QMouseEvent* e )
{
   m_itemSelected = false;
   m_itemDeselected = false;
   m_pLastSelected = nullptr;
   m_selectionCleared = false;
   m_selectOnReleaseEvent = false;
   bool specialAction = false;
   QTreeWidgetItem* oldCurrent = currentItem();
   m_pOldCurrent = oldCurrent;
   m_event = true;
   m_acceptSelect = true;
   if( ( PMTreeViewItem* )itemAt( e->pos() ) )
   {
      m_pCurrent = ( PMTreeViewItem* )itemAt( e->pos() );
      m_pCurrent->setSelected( true );
   }
   QTreeWidget::mousePressEvent( e );
   m_event = false;
   m_acceptSelect = true;
   if( m_selectionCleared )
   {
      emit objectChanged( nullptr, PMCNewSelection, this );
      specialAction = true;
   }
   else if( m_itemSelected || m_itemDeselected )
   {
      if( !( e->modifiers() & ( Qt::ShiftModifier | Qt::ControlModifier ) ) )
      {
         specialAction = true;
         // simple click, deselect all selected item
         // m_pLastSelected is the new selection

         if( m_itemSelected )
         {
            clearSelection();
            m_pLastSelected->setSelected( true );
            emit objectChanged( m_pLastSelected->object(), PMCNewSelection, this );
         }
         else
         {
            m_selectOnReleaseEvent = true;
            m_pLastSelected->setSelected( true );
         }
      }
      else if( ( e->modifiers() & Qt::ShiftModifier ) && oldCurrent && m_pLastSelected )
      {
         if( ( oldCurrent != m_pLastSelected ) &&
             ( oldCurrent->parent() == m_pLastSelected->parent() ) )
         {
            specialAction = true;

            // shift click, old current item has the same parent
            // as the new selection. Select all items between the two
            // items
            if( m_pLastSelected->object()->isSelectable() )
            {
               int firstRow, lastRow;
               firstRow = QTreeWidget::indexFromItem( oldCurrent ).row();
               lastRow = QTreeWidget::indexFromItem( m_pLastSelected ).row();
               QTreeWidgetItem* tmp;
               if( firstRow < lastRow )
               {
                  for( tmp = oldCurrent; tmp; tmp = tmp->treeWidget()->itemBelow( tmp ) )
                  {
                     tmp->setSelected( true );
                     emit objectChanged( (( PMTreeViewItem* ) tmp)->object(),
                                         PMCSelected, this );
                     if( tmp == m_pLastSelected )
                        break;
                  }
               }
               else
               {
                  for( tmp = m_pLastSelected; tmp; tmp = tmp->treeWidget()->itemBelow( tmp ) )
                  {
                     tmp->setSelected( true );
                     emit objectChanged( (( PMTreeViewItem* ) tmp)->object(),
                                         PMCSelected, this );
                     if( tmp == oldCurrent )
                        break;
                  }
               }
            }
            else
               m_pLastSelected->setSelected( false );
         }
      }
   }
   if( !specialAction )
   {
      // no special action
      // object is selected or deselected, no other objects are changed
      if( m_itemSelected )
      {
         if( m_pLastSelected->object()->isSelectable() )
            emit objectChanged( m_pLastSelected->object(), PMCSelected, this );
         else
            m_pLastSelected->setSelected( false );
      }
      else if( m_itemDeselected )
         emit objectChanged( m_pLastSelected->object(), PMCDeselected, this );
   }
   m_acceptSelect = false;
}

void PMTreeView::mouseMoveEvent( QMouseEvent * e )
{
   m_itemSelected = false;
   m_itemDeselected = false;
   m_pLastSelected = nullptr;
   m_selectionCleared = false;
   m_event = true;
   moveMouseManager( e );
   m_event = false;
   // ignore all selections/deselections
   if( m_itemSelected || m_itemDeselected )
      m_pLastSelected->setSelected( m_pLastSelected->object()->isSelected() );
}

void PMTreeView::mousePressEvent( QMouseEvent* e )
{
   m_acceptSelect = true;
   selectionManager( e );
   m_acceptSelect = false;

   m_pressed = false;

   QPoint p = e->pos();

   if( e->button() & Qt::RightButton )
   {
      if( m_pPart ) //eticre copypaste
      {
         QMenu* m = new QMenu( this );
         m = m_pPart->getEditMenu();
         if( m )
            m->exec( QCursor::pos() );
      }
      else
      {
         QMenu* m = new QMenu( "copy", this );
         m->addAction( m_pPart->copyaction() );
         m->exec( QCursor::pos() );
      }
      return;
   }

   PMTreeViewItem *item = ( PMTreeViewItem* )itemAt( p );
   if( item )
   {
      // check if the root decoration was clicked
      /*
      if( !( p.x() > header()->cellPos( header()->mapToActual( 0 ) ) +
             treeStepSize() * ( item->depth() + ( rootIsDecorated() ? 1 : 0 ) )
             + itemMargin() ||
             p.x() < header()->cellPos( header()->mapToActual( 0 ) ) ) )
         item = 0; // p is on the root decoration*/
   }

   if( item )
   {
      if( e->button() == Qt::LeftButton || e->button() == Qt::MidButton )
      {
         m_pressed = true;
         m_pressedPos = e->pos();
         m_pressedItem = item;
         return;
      }
   }
}

void PMTreeView::mouseReleaseEvent( QMouseEvent* e )
{

   QTreeWidget::event( e );

   if( !m_pressed )
      return;

   m_pressed = false;
   m_pressedItem = nullptr;

   if( m_selectOnReleaseEvent )
   {
      if( m_pLastSelected )
      {
         m_acceptSelect = true;
         clearSelection();
         m_pLastSelected->setSelected( true );
         m_acceptSelect = false;

         emit objectChanged( m_pLastSelected->object(), PMCNewSelection, this );
      }
   }
}

void PMTreeView::moveMouseManager( QMouseEvent *e )
{
   if( m_pressed && m_pressedItem )
   {
      int x = e->pos().x();
      int y = e->pos().y();

      //Is it time to start a drag?
      if( abs( x - m_pressedPos.x() ) > QApplication::startDragDistance() ||
           abs( y - m_pressedPos.y() ) > QApplication::startDragDistance() )
      {
         m_selectOnReleaseEvent = false;

         // Calculate hotspot
         QPoint hotspot;
         PMObjectList sortedList = m_pPart->selectedObjects();

         // Do not handle more mouse move or mouse release events
         m_pressed = false;

         if( sortedList.count() > 0 )
         {
            QDrag* d = new QDrag( viewport() );
            PMObjectDrag* m = new PMObjectDrag( m_pPart, sortedList );
            d->setMimeData( m );

            hotspot.setX( m_pressedItem->sizeHint( 0 ).rwidth() / 2 );
            hotspot.setY( m_pressedItem->sizeHint( 0 ).rheight() / 2 );
            d->setHotSpot( hotspot );
            if( sortedList.count() == 1 )
            {
               d->setPixmap( sortedList.first()->pixmap() );
            }
            else
            {
              // d->setPixmap( "pmdrag" );
             }

            Qt::DropAction result = d->start( Qt::CopyAction | Qt::MoveAction );
            if( result == Qt::MoveAction )
            {
               if( !targetDisplaysPart( d->target() ) )
                  m_pPart->dragMoveSelectionTo( nullptr );
            }
         }
      }
   }
}

void PMTreeView::dragMoveEvent( QDragMoveEvent *e )
{
   bool accept = false;

   if( m_pPart->isReadWrite() )
   {
      if( PMObjectDrag::canDecode( e->mimeData(), m_pPart ) )
      {
         PMTreeViewItem *item = ( PMTreeViewItem* ) itemAt( e->pos() );
         PMObject* obj = nullptr;

         if( !item )
         {
            m_pDragOverItem = nullptr;
            obj = nullptr;
         }
         else
         {
            obj = item->object();
            if( ( obj->isSelectable() &&
                   !obj->isSelected() ) || ( e->source() != viewport() ) )
            {
               accept = true;
               setCurrentItem( item );
               m_pDragOverItem = item;
            }
            else
               m_pDragOverItem = nullptr;
         }

         if( accept )
         {
            accept = false;
            if( !obj->isReadOnly() )
               accept = true;
            if( obj->parent() )
               if( !obj->parent()->isReadOnly() )
                  accept = true;
         }
      }
   }

	if(accept)
	{
		if( targetDisplaysPart( e->source() ) )
		{
			e->setDropAction( Qt::MoveAction );
			e->accept();
		}
		else
		{
			e->setDropAction( Qt::CopyAction );
			e->accept();
		}
	}
   else
		e->ignore();
}

void PMTreeView::dragEnterEvent( QDragEnterEvent *e )
{
   m_pDragOverItem = nullptr;

   if( m_pPart->isReadWrite() )
	{
		if( PMObjectDrag::canDecode( e->mimeData(), m_pPart ) )
		{
			if( targetDisplaysPart( e->source() ) )
			{
				e->setDropAction( Qt::MoveAction );
				e->accept();
			}
			else
			{
				e->setDropAction( Qt::CopyAction );
				e->accept();
			}
		}
		else
			e->ignore();
	}
   else
      e->ignore();
}

void PMTreeView::dragLeaveEvent( QDragLeaveEvent* )
{
   m_pDragOverItem = nullptr;
}

void PMTreeView::dropEvent( QDropEvent* e )
{
   PMObject* obj;

   if( m_pPart->isReadWrite() )
   {
      if( m_pDragOverItem )
         obj = m_pDragOverItem->object();
      else
         obj = m_pPart->scene();

      if( PMObjectDrag::canDecode( e->mimeData(), m_pPart ) )
      {
			if( targetDisplaysPart( e->source() ) )
         {
            if( m_pPart->dragMoveSelectionTo( obj ) )
				{
					e->setDropAction( Qt::MoveAction );
					e->accept();
				}
            else
               e->ignore();
         }
         else
         {
            if( m_pPart->drop( obj, e->mimeData() ) )
				{
					e->setDropAction( Qt::CopyAction );
					e->accept();
				}
            else
               e->ignore();
         }
      }
      else
         e->ignore();
   }
   else
      e->ignore();

   m_pDragOverItem = nullptr;
}

void PMTreeView::focusOutEvent( QFocusEvent* e )
{
   QWidget::focusOutEvent( e );
   m_pressed = false;
   m_pressedItem = nullptr;
}

void PMTreeView::focusInEvent( QFocusEvent* e )
{
   QWidget::focusInEvent( e );
   m_pressed = false;
   m_pressedItem = nullptr;
}

void PMTreeView::keyPressEvent( QKeyEvent* e )
{
   QTreeWidgetItem* current = currentItem();
   QTreeWidgetItem* newSelection = nullptr;
   bool accept = false;
   bool deleteItem = false;
   bool pasteItem = false;

   if( current )
   {
      switch( e->key() )
      {
         case Qt::Key_Up:
            newSelection = current->treeWidget()->itemAbove( current );
            accept = true;
            break;
         case Qt::Key_Down:
            newSelection = current->treeWidget()->itemBelow( current );
            accept = true;
            break;
         case Qt::Key_Left:
            newSelection = current->parent();
            accept = true;
            break;
         case Qt::Key_Right:
            newSelection = current->child( 0 );
            accept = true;
            break;
         case Qt::Key_Plus:
            current->setExpanded( true );
            accept = true;
            break;
         case Qt::Key_Minus:
            current->setExpanded( false );
            accept = true;
         case Qt::Key_Delete:
            deleteItem = true;
            accept = true;
            break;
         case Qt::CTRL+Qt::Key_V:
         case Qt::SHIFT+Qt::Key_Insert:
            pasteItem = true;
            accept = true;
            break;
      }
   }

   if( newSelection )
   {
      m_acceptSelect = true;
      clearSelection();
      newSelection->setSelected( true );
      setCurrentItem( newSelection );
      //ensureItemVisible( newSelection );
      m_acceptSelect = false;

      emit objectChanged( ( ( PMTreeViewItem* ) newSelection )->object(),
                          PMCNewSelection, this );
   }

   if( deleteItem && m_pPart->isReadWrite() )
   {
      m_pPart->slotEditDelete();
      m_pPart->setModified( true );
   }
   
   if( pasteItem && m_pPart->isReadWrite() )
   {
      m_pPart->slotEditPaste();
      m_pPart->setModified( true );
   }

   if( accept )
      e->accept();
   else
      e->ignore();
   QWidget::keyPressEvent( e );
}

bool PMTreeView::targetDisplaysPart( QObject* target )
{
   bool result = false;
   if( !target ) // another application
      result = false;
   else if( target == viewport() ) // self
      result = true;
   else
   {
      // Widget may be a view port
      // find the tree view
      QObject* t = target;
      while( t && QString( t->metaObject()->className() ) != QString( "PMTreeView" ) )
         t = t->parent();
      if( t )
         if( ( ( PMTreeView* ) t )->part() == m_pPart )
            result = true;
   }
   return result;
}

QString PMTreeViewFactory::description() const
{
   return "Object Tree";
}

