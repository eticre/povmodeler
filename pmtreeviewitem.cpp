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


#include "pmtreeviewitem.h"
#include "pmobject.h"
#include "pmtreeview.h"
//#include "pmfactory.h"
#include "pmtexturemap.h"
#include <QPixmap>
#include <QStandardPaths>

//called from slotRefresh make the whole treeView, pmscene is topLevelItem
PMTreeViewItem::PMTreeViewItem( PMObject* object, QTreeWidget *parent )
      : QTreeWidgetItem( parent )
{
   m_pObject = object;// connect pov-obj and treeItem
   setDescriptions();
   initSelection();
   setExpanded( true );
   object->setTreeItem( this );// connect treeItem and pov-obj
}

PMTreeViewItem::PMTreeViewItem( PMObject* object )
      : QTreeWidgetItem()
{
   if( object->parent() )
   {
      QTreeWidgetItem* item = object->parent()->getTreeItem();
      int i = 0;
      if( object->prevSibling() )
         i = item->indexOfChild( object->prevSibling()->getTreeItem() ) + 1;

      item->insertChild( i, this );
      m_pObject = object;

      setDescriptions();
      initSelection();

      object->setTreeItem( this );

      if( this->treeWidget()->property( "showList" ) == true )
         this->setHidden( false );
      else
         if( this->object()->isVisibleInTree() )
            this->setHidden( true );
   }
   if( object->firstChild() )//iterate all
   {
       PMObject* obj = nullptr;
       for( obj = object->firstChild(); obj; obj = obj->nextSibling() )
          new PMTreeViewItem( obj );
   }
}

PMTreeViewItem::PMTreeViewItem( PMObject* object, QTreeWidget *parent,
                                QTreeWidgetItem *after )
      : QTreeWidgetItem( parent, after )
{
   m_pObject = object;
   setDescriptions();
   initSelection();
   object->setTreeItem( this );
}

PMTreeViewItem::PMTreeViewItem( PMObject* object, QTreeWidgetItem *parent,
                   QTreeWidgetItem *after )
      : QTreeWidgetItem( parent, after )
{
   m_pObject = object;
   setDescriptions();
   initSelection();
   object->setTreeItem( this );
}

void PMTreeViewItem::setDescriptions()
{
   QString text;

   setIcon( 0, QIcon( ":/smallicon/icons/povicons/small/" + m_pObject->pixmap() ) );
   
   if( m_pObject->canHaveName() )
   {
      text = m_pObject->name();
      if( text.isEmpty() )
         text = m_pObject->description();
   }
   else
      text = m_pObject->description();
   
   if( m_pObject->parent() )
   {
      if( m_pObject->parent()->isA( "TextureMapBase" ) )
      {
         PMTextureMapBase* tm = ( PMTextureMapBase* ) m_pObject->parent();
         if( m_pObject->type() == tm->mapType() )
            text = QString( "[%1] " ).arg( tm->mapValue( m_pObject ), 4, 'f', 4 ) + text;
      }
   }
   if( m_pObject->not_visible_in_tree )
       text = text + "  <hidden>";
   setText( 0, text );
}

QString PMTreeViewItem::key( int, bool ) const
{
   QString result;
   if( m_pObject->parent() )
      result.sprintf( "%06i", m_pObject->parent()->findChild( m_pObject ) );
   else
      result = "000000";
   return result;
}

void PMTreeViewItem::setSelected( bool select )
{
   bool ws = isSelected();
   PMTreeView* treeview = ( PMTreeView* ) treeWidget();

   // ignore selections during a move event
   if( treeview->acceptSelect() )
   {
      QTreeWidgetItem::setSelected( select );

      if( ws != isSelected() )
         treeview->itemSelected( this, isSelected() );
   }
}

void PMTreeViewItem::initSelection()
{
   QTreeWidgetItem::setSelected( m_pObject->isSelected() );
//   if( m_pObject->isSelected() )
//      repaint();
}
