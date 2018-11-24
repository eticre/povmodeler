//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmlibraryiconview.h"

#include <unistd.h>

#include <qstring.h>
#include <qdir.h>
#include <QList>
#include <QDropEvent>

#include <kurl.h>
#include <kio/job.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <stdlib.h>

#include "pmlibraryhandle.h"
#include "pmlibraryobject.h"
#include "pmdebug.h"

const char* PMLibraryIconDrag::format( int i ) const
{
   switch( i )
   {
      case 0:
         return "application/x-qiconlist";
         break;
      case 1:
         return "text/sublib-list";
         break;
      default:
         return 0;
         break;
   }
}

QByteArray PMLibraryIconDrag::encodedData( const char* mime ) const
{
   QByteArray a;
   if ( QString( mime ) == "application/x-qiconlist" )
      a = Q3IconDrag::encodedData( mime );
   else if ( QString( mime ) == "text/sublib-list" )
   {
      QString s , l;
      for( unsigned i = 0; i < m_paths.count( ); ++i )
      {
         if( m_subLibs[i] )
            l = "true";
         else
            l = "false";
         s += m_paths[i] + "\r" + l + "\n";
      }
      a.resize( s.length( ) );
      memcpy( a.data( ), s.toLatin1( ), s.length( ) );
   }
   return a;
}

bool PMLibraryIconDrag::canDecode( QMimeSource* e )
{
   return e->provides( "application/x-qiconlist" ) ||
          e->provides( "text/sublib-list" );
}

bool PMLibraryIconDrag::decode( QMimeSource* e, QStringList& strList, QList<bool>& subLibList )
{
   QByteArray a = e->encodedData( "text/sublib-list" );
   if( a.isEmpty( ) )
      return false;

   QStringList list = QStringList::split( "\n", QString( a ) );
   for( unsigned i = 0; i < list.count( ); ++i )
   {
      strList.append( list[i].section( "\r", 0, 0 ) );
      if( list[i].section( "\r", 1, 1 ) == "true" )
         subLibList.append( true );
      else
         subLibList.append( false );
   }
   return true;
}

void PMLibraryIconDrag::append( const Q3IconDragItem &item, const QRect &pr,
                               const QRect &tr, const QString &path, bool isSubLibrary )
{
   Q3IconDrag::append( item, pr, tr );
   m_paths << path;
   m_subLibs.append( isSubLibrary );
}

PMLibraryIconView::PMLibraryIconView( QWidget* parent )
      : K3IconView( parent, name )
{
   m_pLibrary = NULL;
   m_pCurrentLibrary = NULL;
   setSelectionMode( Single );
   setMode( Execute );
}

void PMLibraryIconView::setLibrary( PMLibraryHandle* h )
{
   m_pLibrary = h;
   refresh( );
}

void PMLibraryIconView::refresh( )
{
   // Clear all the icons
   clear( );

   PMLibraryHandle::EntryIterator* it;

   // Scan all the library objects and load them into the view
   // First let's add the libraries
   it = m_pLibrary->createSubLibraryIterator( );
   for( ; it->current( ); ++(*it) )
   {
      QString f_name = *( it->current( ) );
      PMLibraryHandle h( f_name );

      new PMLibraryIconViewItem( this, h.name( ), f_name, true );
   }
   delete it;

   // Then the objects
   it = m_pLibrary->createObjectIterator( );
   for( ; it->current( ); ++(*it) )
   {
      QString f_name = *( it->current( ) );
      PMLibraryObject obj( f_name );

      if( obj.preview( ) )
         new PMLibraryIconViewItem( this, obj.name( ), obj.preview( )->copy( ), f_name, false );
      else
         new PMLibraryIconViewItem( this, obj.name( ), f_name, false );
   }
   delete it;
}

void PMLibraryIconView::slotDropped( QDropEvent *e, const QList<Q3IconDragItem> & )
{
   e->ignore( );
}

Q3DragObject* PMLibraryIconView::dragObject( )
{
   if ( !currentItem( ) )
      return 0;

   QPoint orig = viewportToContents( viewport( )->mapFromGlobal( QCursor::pos( ) ) );
   PMLibraryIconDrag *drag = new PMLibraryIconDrag( viewport( ) );
   drag->setPixmap( *currentItem( )->pixmap( ),
                     QPoint( currentItem( )->pixmapRect( ).width( ) / 2,
                             currentItem( )->pixmapRect( ).height( ) / 2 ) );

   for ( PMLibraryIconViewItem *item = (PMLibraryIconViewItem*)firstItem( );
         item; item = ( PMLibraryIconViewItem* )item->nextItem( ) )
   {
      if ( item->isSelected( ) )
      {
         Q3IconDragItem id;
         id.setData( item->path( ).toLatin1( ) );
         drag->append( id,
                       QRect( item->pixmapRect( false ).x( ) - orig.x( ),
                              item->pixmapRect( false ).y( ) - orig.y( ),
                              item->pixmapRect( ).width( ),
                              item->pixmapRect( ).height( ) ),
                       QRect( item->textRect( false ).x( ) - orig.x( ),
                              item->textRect( false ).y( ) - orig.y( ),
                              item->textRect().width( ),
                              item->textRect( ).height( ) ),
                       item->path( ),
                       item->isSubLibrary( ) );
      }
   }
   return drag;
}

PMLibraryIconViewItem::PMLibraryIconViewItem( Q3IconView *parent, const QString &text, const QString& path, bool isSubLibrary )
   : K3IconViewItem( parent, text )
{
   m_path = path;
   m_isSubLibrary = isSubLibrary;
}

PMLibraryIconViewItem::PMLibraryIconViewItem( Q3IconView *parent, const QString &text, const QImage& image, const QString& path, bool isSubLibrary )
   : K3IconViewItem( parent, text, image )
{
   m_path = path;
   m_isSubLibrary = isSubLibrary;
}

bool PMLibraryIconViewItem::acceptDrop( const QMimeSource *e ) const
{
   if ( m_isSubLibrary && e->provides( "text/sublib-list" ) )
      return true;

   return false;
}

void PMLibraryIconViewItem::dropped( QDropEvent *e, const QList<Q3IconDragItem> & )
{
   QStringList pathList;
   QList<bool> subLibList;
   if( m_isSubLibrary && PMLibraryIconDrag::decode( e, pathList, subLibList ) )
   {
      PMLibraryIconView* source = static_cast<PMLibraryIconView*>( e->source( )->parentWidget( ) );
      PMLibraryHandle* parentLib = source->library( );
      PMLibraryHandle newParentLib = PMLibraryHandle( m_path );
      if ( parentLib->isReadOnly() || newParentLib.isReadOnly() )
      {
         e->ignore();
         return;
      }

      for( unsigned i = 0; i < pathList.count( ); ++i )
      {
         bool success = true;
         QString path = pathList[i];
         if( path != ( m_path +"/" + path.section( '/', -1 ) ) )
         {
            if( subLibList[i] )
            {
               QString newpath = newPath( path, true );
               if( parentLib->deleteSubLibrary( path ) == PMLibraryHandle::Ok )
               {
                  PMLibraryHandle lib = PMLibraryHandle( path );
                  if( newParentLib.addSubLibrary( newpath, lib.name() ) == PMLibraryHandle::Ok )
                  {
                     lib.changeParentLibrary( m_path );
                     KIO::move( path, newpath );
                  }
                  else
                  {
                     success = false;
                     parentLib->addSubLibrary( path, lib.name( ) );
                  }
               }
               else
                  success = false;
            }
            else
            {
               QString newpath = newPath( path, false );
               if( parentLib->deleteObject( path ) == PMLibraryHandle::Ok )
               {
                  PMLibraryObject obj = PMLibraryObject( path );
                  if( newParentLib.addObject( newpath, obj.name() ) == PMLibraryHandle::Ok )
                  {
                     KIO::move( path, newpath );
                  }
                  else
                  {
                     success = false;
                     parentLib->addObject( path, obj.name( ) );
                  }
               }
               else
                  success = false;
            }

            if( !success )
            {
               KMessageBox::error( 0, i18n( "Error moving \"%1\" to \"%2\"", path, m_path ) );
               e->ignore( );
               return;
            }
         }
      }
      e->acceptAction( );
      source->refresh( );
   }
   else
   {
      e->ignore( );
   }
}

QString PMLibraryIconViewItem::newPath( const QString oldPath, bool /*subLib*/ )
{
   /// @todo Need to replace mkdtemp and mkstemps before enabling libs
   return oldPath;
   /*
   QString path = m_path + "/" + oldPath.section( '/', -1 );
   if( subLib )
   {
      QString test = path + "/library_index.xml";
      if( QFile::exists( test ) )
      {
         QCString s = m_path.toLatin1();
         s+= "/libXXXXXX";
         char* dirname = mkdtemp( s.data() );
         rmdir( dirname );
         path = dirname;
      }
   }
   else if( QFile::exists( path ) )
   {
      // we need to rename it.
      QCString s = m_path.toLatin1();
      s += "/objXXXXXX.kpml";
      int fh = mkstemps( s.data( ), 5 );
      close( fh );
      unlink( s.data() );
      path = s;
   }

   return path;
   */
}

#include "pmlibraryiconview.moc"
