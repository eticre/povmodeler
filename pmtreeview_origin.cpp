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

#include <q3listview.h>
#include <q3header.h>
#include <qlayout.h>
#include <QMenu>
#include <qcursor.h>
//Added by qt3to4:
#include <QDragLeaveEvent>
#include <QFocusEvent>
#include <QDragMoveEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMouseEvent>

#include <klocale.h>
#include <kmessagebox.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <kxmlguifactory.h>

#include "pmtreeview.h"
#include "pmtreeviewitem.h"
#include "pmcommand.h"
#include "pmpart.h"
#include "pmscene.h"
#include "pmobjectdrag.h"


PMTreeViewWidget::PMTreeViewWidget( PMPart* part, QWidget* parent /*= 0*/)
      : PMViewBase( parent )
{
   QHBoxLayout* hl = new QHBoxLayout( this );
   PMTreeView* tv = new PMTreeView( part, this );
   hl->addWidget( tv );
}

QString PMTreeViewWidget::description( ) const
{
   return i18n( "Object Tree" );
}

PMTreeView::PMTreeView( PMPart* part, QWidget* parent /*= 0*/ )
      : Q3ListView( parent )
{
   addColumn( i18n( "Objects" ) );
   header( )->hide( );
   setRootIsDecorated( true );
   setSorting( -1 );
   setSelectionMode( Multi );
   m_pPart = part;

   m_itemSelected = false;
   m_itemDeselected = false;
   m_selectionCleared = false;
   m_pLastSelected = 0;
   m_event = false;
   m_pressed = false;
   m_pDragOverItem = 0;
   m_acceptSelect = false;
   m_pressedItem = 0;

   viewport( )->setAcceptDrops( true );
   viewport( )->setMouseTracking( true );
   viewport( )->setFocusPolicy( Qt::WheelFocus );
   setFocusPolicy( Qt::WheelFocus );
   setAcceptDrops( true );

   connect( part, SIGNAL( refresh( ) ), SLOT( slotRefresh( ) ) );
   connect( part, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
                  SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );
   connect( part, SIGNAL( clear( ) ), SLOT( slotClear( ) ) );
   connect( this, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
            part, SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   slotRefresh( );
}

PMTreeView::~PMTreeView( )
{
   emit destroyed( this );
}

void PMTreeView::slotObjectChanged( PMObject* obj, const int mode,
                                    QObject* sender )
{
   PMTreeViewItem* pTreeItem = 0;
   bool as = m_acceptSelect;
   m_acceptSelect = true;

   if( sender != this )
   {
      if( ( mode & PMCAdd ) && !( mode & PMCInsertError ) )
      {
         // object was added
         if( !obj->parent( ) )
         {
            // object has no parent, append it as top level item
            pTreeItem = new PMTreeViewItem( obj, this );
         }
         else
         {
            // find the parent in the listview
            Q3ListViewItem* pParentTreeItem = findObject( obj->parent( ) );
            if( pParentTreeItem )
            {
               PMObject* hObj = obj->prevSibling( );
               Q3ListViewItem* pSibling = 0;
               bool found = false;

               if( hObj )
               {
                  // find the previous sibling
                  pSibling = pParentTreeItem->firstChild( );
                  while( pSibling && !found )
                  {
                     if( ( ( PMTreeViewItem* ) pSibling )->object( ) == hObj )
                        found = true;
                     else
                        pSibling = pSibling->nextSibling( );
                  }
               }
               if( found )
               {
                  // object has sibling
                  pTreeItem = new PMTreeViewItem( obj, pParentTreeItem, pSibling );
               }
               else
               {
                  // object has no sibling
                  pTreeItem = new PMTreeViewItem( obj, pParentTreeItem );
               }
            }
         }

         if( pTreeItem )
         {
            // add child items if necessary
            if( obj->countChildren( ) > 0 )
               addChildItems( pTreeItem );
         }
      }
      if( mode & PMCDescription )
      {
         if( !pTreeItem )
            pTreeItem = findObject( obj );

         if( pTreeItem )
            pTreeItem->setDescriptions( );
      }
      if( mode & PMCChildren )
      {
         if( !pTreeItem )
            pTreeItem = findObject( obj );

         if( pTreeItem )
         {
            // delete old items
            while( pTreeItem->firstChild( ) )
               delete pTreeItem->firstChild( );
            // create new
            addChildItems( pTreeItem );
            pTreeItem->setOpen( true );
         }
      }
      if( mode & PMCNewSelection )
      {
         clearSelection( );

         if( !pTreeItem )
             pTreeItem = findObject( obj );

         if( pTreeItem )
         {
             PMTreeViewItem* p;
             for( p = pTreeItem->parent( ); p; p = p->parent( ) )
                 p->setOpen( true );
             pTreeItem->setSelected( true );
             setCurrentItem( pTreeItem );
         }
      }
      if( mode & PMCDeselected )
      {
         if( !pTreeItem )
            pTreeItem = findObject( obj );
         pTreeItem->setSelected( false );
      }
      if( mode & PMCSelected )
      {
         if( !pTreeItem )
            pTreeItem = findObject( obj );
         pTreeItem->setSelected( true );
      }
      if( mode & PMCRemove )
      {
         // object was removed, remove the listview item
         if( !pTreeItem )
            pTreeItem = findObject( obj );
         delete pTreeItem;
	 pTreeItem = 0;
      }
      if( mode & PMCData )
      {
         // special case for texture maps
         if( obj )
         {
            if( obj->isA( "TextureMapBase" ) )
            {
               if( !pTreeItem )
                  pTreeItem = findObject( obj );
               if( pTreeItem )
               {
                  PMTreeViewItem* it = ( PMTreeViewItem* ) pTreeItem->firstChild( );
                  for( ; it; it = ( PMTreeViewItem* ) it->nextSibling( ) )
                     it->setDescriptions( );
               }
            }
         }
      }
   }
   m_acceptSelect = as;
}


PMTreeViewItem* PMTreeView::findObject( const PMObject* obj )
{
   PMTreeViewItem* pTreeItem = 0;

   if( !obj->parent( ) )
   {
      // top level object
      pTreeItem = ( PMTreeViewItem* ) firstChild( );
      for( ; pTreeItem; pTreeItem = ( PMTreeViewItem* ) pTreeItem->nextSibling( ) )
         if( pTreeItem->object( ) == obj )
            return pTreeItem;
   }
   else
   {
      pTreeItem = findObject( obj->parent( ) );
      if( pTreeItem )
      {
         pTreeItem = ( PMTreeViewItem* ) pTreeItem->firstChild( );
         for( ; pTreeItem; pTreeItem = ( PMTreeViewItem* ) pTreeItem->nextSibling( ) )
            if( pTreeItem->object( ) == obj )
               return pTreeItem;
      }
   }
   return 0;
}


void PMTreeView::selectItem( Q3ListViewItem* /*sitem*/ )
{
/*   QListViewItem* pItem = 0;
   bool emitSig;
   emitSig = ( m_pSelectedObject != ( ( PMTreeViewItem* ) sitem )->object( ) );

   m_pSelectedObject = ( ( PMTreeViewItem* ) sitem )->object( );

   for( pItem = sitem->parent( ); pItem; pItem = pItem->parent( ) )
      pItem->setOpen( true );
   ensureItemVisible( sitem );
   setCurrentItem( sitem );
   setSelected( sitem, true );
   if( emitSig )
      emit objectSelected( m_pSelectedObject );
*/
}

void PMTreeView::addChildItems( PMTreeViewItem* item )
{
   PMObject* obj = 0;
   PMTreeViewItem* listItem = 0;

   for( obj = item->object( )->firstChild( ); obj; obj = obj->nextSibling( ) )
   {
      // insert all child objects
      if( listItem )
         listItem = new PMTreeViewItem( obj, item, listItem );
      else
         // first child
         listItem = new PMTreeViewItem( obj, item );
      // recursive call, if child has children
      if( obj->countChildren( ) > 0 )
         addChildItems( listItem );
   }
}

void PMTreeView::slotRefresh( )
{
   PMTreeViewItem* item;
   slotClear( );
   // insert the top level items
   if( m_pPart->scene( ) )
   {
      item = new PMTreeViewItem( m_pPart->scene( ), this );
      addChildItems( item );
      item->setOpen( true );
//   item = new PMTreeViewItem( m_pPart->insertErrors( ), this );
//   addChildItems( item );
//   item->setOpen( true );
   }
}

void PMTreeView::slotClear( )
{
   clear( );
   m_pLastSelected = 0;
   m_pDragOverItem = 0;
   m_pressedItem = 0;
}

void PMTreeView::itemSelected( PMTreeViewItem* item, bool selected )
{
   repaintItem( item );

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

void PMTreeView::contentsMousePressEvent( QMouseEvent * e )
{
   m_itemSelected = false;
   m_itemDeselected = false;
   m_pLastSelected = 0;
   m_selectionCleared = false;
   m_selectOnReleaseEvent = false;
   bool specialAction = false;

   Q3ListViewItem* oldCurrent = currentItem( );

   m_event = true;
   m_acceptSelect = true;
   Q3ListView::contentsMousePressEvent( e );
   m_event = false;
   m_acceptSelect = true;

   if( m_selectionCleared )
   {
      emit objectChanged( 0, PMCNewSelection, this );
      specialAction = true;
   }
   else if( m_itemSelected || m_itemDeselected )
   {
      if( !( e->modifiers( ) & ( Qt::ShiftModifier | Qt::ControlModifier ) ) )
      {
         specialAction = true;
         // simple click, deselect all selected item
         // m_pLastSelected is the new selection

         if( m_itemSelected )
         {
            clearSelection( );
            m_pLastSelected->setSelected( true );

            emit objectChanged( m_pLastSelected->object( ), PMCNewSelection,
                                this );
         }
         else
         {
            m_selectOnReleaseEvent = true;
            m_pLastSelected->setSelected( true );
         }
      }
      else if( ( e->modifiers( ) & Qt::ShiftModifier ) && oldCurrent && m_pLastSelected )
      {
         if( ( oldCurrent != m_pLastSelected ) &&
             ( oldCurrent->parent( ) == m_pLastSelected->parent( ) ) )
         {
            specialAction = true;

            // shift click, old current item has the same parent
            // as the new selection. Select all items between the two
            // items
            if( m_pLastSelected->object( )->isSelectable( ) )
            {
               bool down = oldCurrent->itemPos( ) < m_pLastSelected->itemPos( );
               Q3ListViewItem* tmp;

               if( down )
               {
                  for( tmp = oldCurrent; tmp; tmp = tmp->nextSibling( ) )
                  {
                     tmp->setSelected( true );
                     emit objectChanged( (( PMTreeViewItem* ) tmp)->object( ),
                                         PMCSelected, this );
                     if( tmp == m_pLastSelected )
                        break;
                  }
               }
               else
               {
                  for( tmp = m_pLastSelected; tmp; tmp = tmp->nextSibling( ) )
                  {
                     tmp->setSelected( true );
                     emit objectChanged( (( PMTreeViewItem* ) tmp)->object( ),
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
         if( m_pLastSelected->object( )->isSelectable( ) )
            emit objectChanged( m_pLastSelected->object( ), PMCSelected, this );
         else
            m_pLastSelected->setSelected( false );
      }
      else if( m_itemDeselected )
         emit objectChanged( m_pLastSelected->object( ), PMCDeselected, this );
   }
   m_acceptSelect = false;
}

void PMTreeView::contentsMouseMoveEvent( QMouseEvent * e )
{
   m_itemSelected = false;
   m_itemDeselected = false;
   m_pLastSelected = 0;
   m_selectionCleared = false;

   m_event = true;
   Q3ListView::contentsMouseMoveEvent( e );
   m_event = false;

   // ignore all selections/deselections
   if( m_itemSelected || m_itemDeselected )
      m_pLastSelected->setSelected( m_pLastSelected->object( )->isSelected( ) );
}

void PMTreeView::viewportMousePressEvent( QMouseEvent* e )
{
   m_acceptSelect = true;
   Q3ListView::viewportMousePressEvent( e );
   m_acceptSelect = false;

   m_pressed = false;

   QPoint p = e->pos( );

   if( e->button( ) & Qt::RightButton )
   {
      if( m_pPart->factory( ) ) 
      {
         QMenu* m =
            ( QMenu* ) m_pPart->factory( )->container( "treeViewPopup", m_pPart );
         if( m )
            m->exec( QCursor::pos( ) );
      }
      return;
   }

   PMTreeViewItem *item = ( PMTreeViewItem* )itemAt( p );
   if( item )
   {
      // check if the root decoration was clicked
      if( !( p.x( ) > header( )->cellPos( header( )->mapToActual( 0 ) ) +
             treeStepSize( ) * ( item->depth( ) + ( rootIsDecorated( ) ? 1 : 0 ) )
             + itemMargin( ) ||
             p.x( ) < header( )->cellPos( header( )->mapToActual( 0 ) ) ) )
         item = 0; // p is on the root decoration
   }

   if( item )
   {
      if( e->button( ) == Qt::LeftButton || e->button( ) == Qt::MidButton )
      {
         m_pressed = true;
         m_pressedPos = e->pos( );
         m_pressedItem = item;
         return;
      }
   }
}

void PMTreeView::viewportMouseReleaseEvent( QMouseEvent* e )
{
   Q3ListView::viewportMouseReleaseEvent( e );

   if( !m_pressed )
      return;

   m_pressed = false;
   m_pressedItem = 0L;

   if( m_selectOnReleaseEvent )
   {
      if( m_pLastSelected )
      {
         m_acceptSelect = true;
         clearSelection( );
         m_pLastSelected->setSelected( true );
         m_acceptSelect = false;

         emit objectChanged( m_pLastSelected->object( ), PMCNewSelection, this );
      }
   }
}

void PMTreeView::viewportMouseMoveEvent( QMouseEvent *e )
{
   Q3ListView::viewportMouseMoveEvent( e );

   if( m_pressed && m_pressedItem )
   {
      int x = e->pos( ).x( );
      int y = e->pos( ).y( );

      //Is it time to start a drag?
      if( abs( x - m_pressedPos.x( ) ) > KGlobalSettings::dndEventDelay( ) ||
           abs( y - m_pressedPos.y( ) ) > KGlobalSettings::dndEventDelay( ) )
      {
         m_selectOnReleaseEvent = false;

         // Calculate hotspot
         QPoint hotspot;
         PMObjectList sortedList = m_pPart->selectedObjects( );

         // Do not handle more mouse move or mouse release events
         m_pressed = false;

         if( sortedList.count( ) > 0 )
         {
				QDrag* d = new QDrag( viewport( ) );
            PMObjectDrag* m = new PMObjectDrag( m_pPart, sortedList );
				d->setMimeData( m );

            hotspot.setX( m_pressedItem->pixmap( 0 )->width( ) / 2 );
            hotspot.setY( m_pressedItem->pixmap( 0 )->height( ) / 2 );
				d->setHotSpot( hotspot );
            if( sortedList.count( ) == 1 )
               d->setPixmap( SmallIcon( sortedList.first( )->pixmap( ) ) );
            else
               d->setPixmap( SmallIcon( "pmdrag" ) );

				Qt::DropAction result = d->start( Qt::CopyAction | Qt::MoveAction );
            if( result == Qt::MoveAction )
            {
               if( !targetDisplaysPart( d->target( ) ) )
                  m_pPart->dragMoveSelectionTo( 0 );
            }
         }
      }
   }
}

void PMTreeView::viewportDragMoveEvent( QDragMoveEvent *e )
{
   bool accept = false;

   if( m_pPart->isReadWrite( ) )
   {
      if( PMObjectDrag::canDecode( e->mimeData( ), m_pPart ) )
      {
         PMTreeViewItem *item = ( PMTreeViewItem* ) itemAt( e->pos( ) );
         PMObject* obj = 0;

         if( !item )
         {
            m_pDragOverItem = 0L;
            obj = 0;
         }
         else
         {
            obj = item->object( );
            if( ( obj->isSelectable( ) &&
                   !obj->isSelected( ) ) || ( e->source( ) != viewport( ) ) )
            {
               accept = true;
               setCurrentItem( item );
               m_pDragOverItem = item;
            }
            else
               m_pDragOverItem = 0L;
         }

         if( accept )
         {
            accept = false;
            if( !obj->isReadOnly( ) )
               accept = true;
            if( obj->parent( ) )
               if( !obj->parent( )->isReadOnly( ) )
                  accept = true;
         }
      }
   }

	if(accept)
	{
		if( targetDisplaysPart( e->source( ) ) )
		{
			e->setDropAction( Qt::MoveAction );
			e->accept( );
		}
		else
		{
			e->setDropAction( Qt::CopyAction );
			e->accept( );
		}
	}
   else
		e->ignore( );
}

void PMTreeView::viewportDragEnterEvent( QDragEnterEvent *e )
{
   m_pDragOverItem = 0L;

   if( m_pPart->isReadWrite( ) )
	{
		if( PMObjectDrag::canDecode( e->mimeData( ), m_pPart ) )
		{
			if( targetDisplaysPart( e->source( ) ) )
			{
				e->setDropAction( Qt::MoveAction );
				e->accept( );
			}
			else
			{
				e->setDropAction( Qt::CopyAction );
				e->accept( );
			}
		}
		else
			e->ignore( );
	}
   else
      e->ignore( );
}

void PMTreeView::viewportDragLeaveEvent( QDragLeaveEvent* )
{
   m_pDragOverItem = 0L;
}

void PMTreeView::viewportDropEvent( QDropEvent* e )
{
   PMObject* obj;

   if( m_pPart->isReadWrite( ) )
   {
      if( m_pDragOverItem )
         obj = m_pDragOverItem->object( );
      else
         obj = m_pPart->scene( );

      if( PMObjectDrag::canDecode( e->mimeData( ), m_pPart ) )
      {
			if( targetDisplaysPart( e->source( ) ) )
         {
            if( m_pPart->dragMoveSelectionTo( obj ) )
				{
					e->setDropAction( Qt::MoveAction );
					e->accept( );
				}
            else
               e->ignore( );
         }
         else
         {
            if( m_pPart->drop( obj, e->mimeData( ) ) )
				{
					e->setDropAction( Qt::CopyAction );
					e->accept( );
				}
            else
               e->ignore( );
         }
      }
      else
         e->ignore( );
   }
   else
      e->ignore( );

   m_pDragOverItem = 0L;
}

void PMTreeView::focusOutEvent( QFocusEvent* e )
{
   QWidget::focusOutEvent( e );
   m_pressed = false;
   m_pressedItem = 0;
}

void PMTreeView::focusInEvent( QFocusEvent* e )
{
   QWidget::focusInEvent( e );
   m_pressed = false;
   m_pressedItem = 0;
}

void PMTreeView::keyPressEvent( QKeyEvent* e )
{
   Q3ListViewItem* current = currentItem( );
   Q3ListViewItem* newSelection = 0;
   bool accept = false;
   bool deleteItem = false;
   bool pasteItem = false;

   if( current )
   {
      switch( e->key( ) )
      {
         case Qt::Key_Up:
            newSelection = current->itemAbove( );
            accept = true;
            break;
         case Qt::Key_Down:
            newSelection = current->itemBelow( );
            accept = true;
            break;
         case Qt::Key_Left:
            newSelection = current->parent( );
            accept = true;
            break;
         case Qt::Key_Right:
            newSelection = current->firstChild( );
            accept = true;
            break;
         case Qt::Key_Plus:
            current->setOpen( true );
            accept = true;
            break;
         case Qt::Key_Minus:
            current->setOpen( false );
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
      clearSelection( );
      newSelection->setSelected( true );
      setCurrentItem( newSelection );
      ensureItemVisible( newSelection );
      m_acceptSelect = false;

      emit objectChanged( ( ( PMTreeViewItem* ) newSelection )->object( ),
                          PMCNewSelection, this );
   }

   if( deleteItem && m_pPart->isReadWrite( ) )
   {
      m_pPart->slotEditDelete( );
      m_pPart->setModified( true );
   }
   
   if( pasteItem && m_pPart->isReadWrite( ) )
   {
      m_pPart->slotEditPaste( );
      m_pPart->setModified( true );
   }

   if( accept )
      e->accept( );
   else
      e->ignore( );
   QWidget::keyPressEvent( e );
}

bool PMTreeView::targetDisplaysPart( QWidget* target )
{
   bool result = false;
   if( !target ) // another application
      result = false;
   else if( target == viewport( ) ) // self
      result = true;
   else
   {
      // Widget may be a view port
      // find the tree view
      QWidget* t = target;
      while( t && QString( t->metaObject( )->className( ) ) != QString( "PMTreeView" ) )
         t = t->parentWidget( );
      if( t )
         if( ( ( PMTreeView* ) t )->part( ) == m_pPart )
            result = true;
   }
   return result;
}

QString PMTreeViewFactory::description( ) const
{
   return i18n( "Object Tree" );
}

#include "pmtreeview.moc"
