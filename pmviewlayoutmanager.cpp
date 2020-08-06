/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
    copyright            : (C) 2003 by Andreas Zehender
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
#include "pmshell.h"
#include "pmdebug.h"
#include "pmviewfactory.h"
#include "pmviewlayoutmanager.h"
#include "pmdockwidget_private.h"
#include <QLocale>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QLineEdit>
#include <QLayout>
#include <QTextStream>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStandardPaths>
#include <QObject>


PMViewLayoutManager* PMViewLayoutManager::s_pInstance = nullptr;


//================  PMViewLayoutEntry ====================

PMViewLayoutEntry::PMViewLayoutEntry()
{
   m_viewType = "treeview";
   m_dockPosition = PMDockWidget::DockRight;
   m_columnWidth = 33;
   m_height = 50;
   m_floatingWidth = 400;
   m_floatingHeight = 400;
   m_floatingPositionX = 200;
   m_floatingPositionY = 200;
   m_pCustomOptions = nullptr;
}

PMViewLayoutEntry::PMViewLayoutEntry( const PMViewLayoutEntry& e )
{
   m_viewType = e.m_viewType;
   m_dockPosition = e.m_dockPosition;
   m_columnWidth = e.m_columnWidth;
   m_height = e.m_height;
   m_floatingWidth = e.m_floatingWidth;
   m_floatingHeight = e.m_floatingHeight;
   m_floatingPositionX = e.m_floatingPositionX;
   m_floatingPositionY = e.m_floatingPositionY;
   if( e.m_pCustomOptions )
      m_pCustomOptions = e.m_pCustomOptions->copy();
   else
      m_pCustomOptions = nullptr;
}

PMViewLayoutEntry::~PMViewLayoutEntry()
{
   if( m_pCustomOptions )
      delete m_pCustomOptions;
}

void PMViewLayoutEntry::loadData( QDomElement& e )
{
   QString s;
   bool ok;

   // Read the view type
   s = e.tagName();
   m_viewType = s;

   // Read dock position
   s = e.attribute( "position", "Right" );
   if( s == "Right" ) m_dockPosition = PMDockWidget::DockRight;
   else if( s == "Bottom" ) m_dockPosition = PMDockWidget::DockBottom;
   else if( s == "Center" ) m_dockPosition = PMDockWidget::DockCenter;
   else if( s == "None" ) m_dockPosition = PMDockWidget::DockNone;

   // Read the column width
   s = e.attribute( "columnWidth", "33" );
   m_columnWidth = s.toInt( &ok );
   if( !ok ) m_columnWidth = 33;

   // and view height
   s = e.attribute( "height", "50" );
   m_height = s.toInt( &ok );
   if( !ok ) m_height = 50;

   s = e.attribute( "floatingHeight", "400" );
   m_floatingHeight = s.toInt( &ok );
   if( !ok ) m_floatingHeight = 400;
   s = e.attribute( "floatingWidth", "400" );
   m_floatingWidth = s.toInt( &ok );
   if( !ok ) m_floatingWidth = 400;

   s = e.attribute( "floatingPosX", "200" );
   m_floatingPositionX = s.toInt( &ok );
   if( !ok ) m_floatingPositionX = 200;
   s = e.attribute( "floatingPosY", "200" );
   m_floatingPositionY = s.toInt( &ok );
   if( !ok ) m_floatingPositionY = 200;

   // Load custom options
   if( m_pCustomOptions )
      delete m_pCustomOptions;

   m_pCustomOptions = PMViewFactory::theFactory()->newOptionsInstance( m_viewType );

   if( m_pCustomOptions )
      m_pCustomOptions->loadData( e );
}

void PMViewLayoutEntry::saveData( QDomElement& e ) const
{
   QString s;
   e.setTagName( m_viewType );
   switch( m_dockPosition )
   {
      case PMDockWidget::DockRight:
         e.setAttribute( "position", "Right" );
         break;
      case PMDockWidget::DockBottom:
         e.setAttribute( "position", "Bottom" );
         break;
      case PMDockWidget::DockCenter:
         e.setAttribute( "position", "Center" );
         break;
      case PMDockWidget::DockNone:
         e.setAttribute( "position", "None" );
         break;
      default:
         qCritical(  ) << ( "Unknown dock position." )
                           << Qt::endl;
         break;
   }
   s.setNum( m_columnWidth );
   e.setAttribute( "columnWidth", s );
   s.setNum( m_height );
   e.setAttribute( "height", s );
   s.setNum( m_floatingHeight );
   e.setAttribute( "floatingHeight", s );
   s.setNum( m_floatingWidth );
   e.setAttribute( "floatingWidth", s );
   s.setNum( m_floatingPositionX );
   e.setAttribute( "floatingPosX", s );
   s.setNum( m_floatingPositionY );
   e.setAttribute( "floatingPosY", s );

   // save custom options
   if( m_pCustomOptions )
      m_pCustomOptions->saveData( e );
}

void PMViewLayoutEntry::setViewType( const QString& v )
{
   m_viewType = v;
}

void PMViewLayoutEntry::setDockPosition( PMDockWidget::DockPosition i )
{
   m_dockPosition = i;
}

void PMViewLayoutEntry::setColumnWidth( int i )
{
   m_columnWidth = i;
   if( i < 1 )
      m_columnWidth = 1;
}

void PMViewLayoutEntry::setHeight( int i )
{
   m_height = i;
   if( i < 1 )
      m_height = 1;
}

const QString PMViewLayoutEntry::viewTypeAsString()
{
   PMViewTypeFactory* f = PMViewFactory::theFactory()->viewFactory( m_viewType );
   if( f )
   {
      if( m_pCustomOptions )
         return f->description( m_pCustomOptions );
      else
         return f->description();
   }

   qCritical(  ) << ( "Unknown view type." )<< Qt::endl;
   return ( "Unknown" );
}

const QString PMViewLayoutEntry::dockPositionAsString()
{
   switch( m_dockPosition )
   {
      case PMDockWidget::DockRight:
         return ( "New Column" );
      case PMDockWidget::DockBottom:
         return ( "Below" );
      case PMDockWidget::DockCenter:
         return ( "Tabbed" );
      case PMDockWidget::DockNone:
         return ( "Floating" );
      default:
         qCritical(  ) << ( "Unknown dock position." )
                           << Qt::endl;
         return ( "unknown" );
   }
}

void PMViewLayoutEntry::setCustomOptions( PMViewOptions* o )
{
   if( m_pCustomOptions && m_pCustomOptions != o )
      delete m_pCustomOptions;
   m_pCustomOptions = o;
}

//==================  PMViewLayout ======================

PMViewLayout::PMViewLayout()
{
   m_entries.clear();
}

PMViewLayout::PMViewLayout( const PMViewLayout& vl )
{
   m_entries = vl.m_entries;
   m_name = vl.m_name;
}

PMViewLayout& PMViewLayout::operator = ( const PMViewLayout& vl )
{
   m_entries = vl.m_entries;
   m_name = vl.m_name;
   return *this;
}

void PMViewLayout::loadData( QDomElement& e )
{
   m_entries.clear();
   m_name = e.attribute( "name", "unnamed" );

   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         PMViewLayoutEntry vle;

         vle.loadData( me );
         m_entries.append( vle );
      }
      m = m.nextSibling();
   }
   normalize();
}

void PMViewLayout::saveData( QDomElement& e, QDomDocument& doc ) const
{
   QList< PMViewLayoutEntry>::const_iterator it;

   e.setAttribute( "name", m_name );
   for( it = m_entries.constBegin(); it != m_entries.constEnd() ; ++it )
   {
      QDomElement a;
      a = doc.createElement( "unknown" );
      ( *it ).saveData( a );
      e.appendChild( a );
   }
}

void PMViewLayout::setName( const QString& n )
{
   m_name = n;
}

void PMViewLayout::addEntry( const PMViewLayoutEntry& e, int index )
{
   if( index == -1 )
      m_entries.append( e );
   else
      m_entries.insert( index, e );
}

void PMViewLayout::removeEntry( int index )
{
   m_entries.removeAt( index );
}

void PMViewLayout::displayLayout( PMShell* shell )
{
   PMDockWidget* lastWidget = nullptr;
   QList< PMViewLayoutEntry>::const_iterator it;
   bool isViewSet = false;
   int lastWidth = 0, width = 100;
   int lastHeight = 0, height = 100;

   for( it = m_entries.constBegin(); it != m_entries.constEnd() ; ++it )
   {
      PMDockWidget* dock = shell->createView( ( *it ).viewType(),
                                              ( *it ).customOptions(),
                                              false );
      // Dock the widget
      if( ( *it ).dockPosition() == PMDockWidget::DockNone )
      {
         // the specified target is the desktop
         dock->manualDock( nullptr, PMDockWidget::DockDesktop, 50,
                           QPoint( ( *it ).floatingPositionX(),
                                   ( *it ).floatingPositionY() ) );
         dock->resize( ( *it ).floatingWidth(), ( *it ).floatingHeight() );
         dock->show();
      }
      else
      {
         // the first dockwidget is not docked but placed on all the window
         // through setView()
         if( !isViewSet )
         {
            shell->setView( dock );
            isViewSet = true;
            lastWidget = dock;
         }
         else
         {
            switch( ( *it ).dockPosition() )
            {
               case PMDockWidget::DockRight:
                  dock->manualDock( lastWidget, ( *it ).dockPosition(),
                                    lastWidth * 100 / width );

                  width -= lastWidth;
                  if( width < 1 ) width = 1;
                  height = 100;
                  lastWidget = dock;
                  break;
               case PMDockWidget::DockBottom:
                  dock->manualDock( lastWidget, ( *it ).dockPosition(),
                                    lastHeight * 100 / height );
                  height -= lastHeight;
                  if( height < 1 ) height = 1;
                  lastWidget = dock;
                  break;
               case PMDockWidget::DockCenter:
                  dock->manualDock( lastWidget, ( *it ).dockPosition(), 100 );
                  break;
               default:
                  dock->manualDock( nullptr, PMDockWidget::DockDesktop, 100 );
                  break;
            }
         }

         switch( ( *it ).dockPosition() )
         {
            case PMDockWidget::DockRight:
               lastWidth = ( *it ).columnWidth();
               lastHeight = ( *it ).height();
               break;
            case PMDockWidget::DockBottom:
               lastHeight = ( *it ).height();
               break;
            default:
               break;
         }
      }
   }
}

void PMViewLayout::normalize()
{
   iterator it;
   int sumColWidth = 0;

   it = m_entries.begin();
   // the first entry has to be a new column
   if( it != m_entries.end() )
      if( ( *it ).dockPosition() != PMDockWidget::DockRight )
         ( *it ).setDockPosition( PMDockWidget::DockRight );

   // find negative or zero widths and heights
   for( it = m_entries.begin(); it != m_entries.end(); ++it )
   {
      if( ( *it ).columnWidth() < 1 )
         ( *it ).setColumnWidth( 1 );
      if( ( *it ).height() < 1 )
         ( *it ).setHeight( 1 );
   }

   // normalize the column widths
   for( it = m_entries.begin(); it != m_entries.end(); ++it )
      if( ( *it ).dockPosition() == PMDockWidget::DockRight )
         sumColWidth += ( *it ).columnWidth();
   if( sumColWidth == 0 )
      sumColWidth = 1;

   for( it = m_entries.begin(); it != m_entries.end(); ++it )
   {
      if( ( *it ).dockPosition() == PMDockWidget::DockRight )
      {
         ( *it ).setColumnWidth( ( int ) ( ( *it ).columnWidth() * 100.0
                                           / sumColWidth + 0.5 ) );
         // normalize each column
         iterator it2 = it;
         int height = ( *it ).height();

         for( it2++; it2 != m_entries.end() &&
             ( *it2 ).dockPosition() != PMDockWidget::DockRight; ++it2 )
            if( ( *it2 ).dockPosition() == PMDockWidget::DockBottom )
               height += ( *it2 ).height();
         if( height == 0 )
            height = 1;
         ( *it ).setHeight( ( int ) ( ( *it ).height() * 100.0
                                      / height + 0.5 ) );
         it2 = it;
         for( it2++; it2 != m_entries.end() &&
             ( *it2 ).dockPosition() != PMDockWidget::DockRight; ++it2 )
            if( ( *it2 ).dockPosition() == PMDockWidget::DockBottom )
               ( *it2 ).setHeight( ( int ) ( ( *it2 ).height() * 100.0
                                             / height + 0.5 ) );
      }
   }
}

PMViewLayout PMViewLayout::extractViewLayout( PMShell* shell )
{
   PMViewLayout layout;

   QList< QList< PMViewLayoutEntry > > cols;
   cols.append( QList< PMViewLayoutEntry >() );

   // find docked widgets
   recursiveExtractColumns( cols, cols.begin(), 100,
                            shell->centralWidget() );

   QList< QList< PMViewLayoutEntry > >::iterator cit;
   QList< PMViewLayoutEntry >::iterator eit;

   for( cit = cols.begin(); cit != cols.end(); ++cit )
      for( eit = ( *cit ).begin(); eit != ( *cit ).end(); ++eit )
         layout.addEntry( *eit );

   // find floating widgets
	QList<PMDockWidget*> list = shell->manager()->findFloatingWidgets();
	QList<PMDockWidget*>::ConstIterator it;

   for(it = list.constBegin() ; it != list.constEnd(); ++it )
   {
      QWidget* w = (*it)->getWidget();
      if( w )
      {
         bool invalid = false;
         PMViewLayoutEntry e;
         e.setDockPosition( PMDockWidget::DockNone );
         QPoint p = (*it)->pos();
         e.setFloatingPositionX( p.x() );
         e.setFloatingPositionY( p.y() );
         e.setFloatingWidth( (*it)->width() );
         e.setFloatingHeight( (*it)->height() );

         if( w->inherits( "PMViewBase" ) )
         {
            PMViewBase* v = ( PMViewBase* ) w;
            e.setViewType( v->viewType() );
            PMViewOptions* vo =
               PMViewFactory::theFactory()->newOptionsInstance( v->viewType() );
            if( vo )
            {
               v->saveViewConfig( vo );
               e.setCustomOptions( vo );
            }
         }
         else
            invalid = true;

         if( !invalid )
            layout.addEntry( e );
      }
   }

   return layout;
}

void PMViewLayout::recursiveExtractColumns(
   QList< QList< PMViewLayoutEntry > >& cols,
   QList< QList< PMViewLayoutEntry > >::iterator cit,
   int width, QWidget* widget )
{
   if( !widget )
      return;

   if( widget->inherits( "PMDockWidget" ) )
   {
      PMDockWidget* dw = ( PMDockWidget* ) widget;
      QWidget* w = dw->getWidget();
      if( w )
      {
         bool colStart = true;
         if( w->inherits( "PMDockSplitter" ) )
         {
            PMDockSplitter* sp = ( PMDockSplitter* ) w;
            if( sp->splitterOrientation() == Qt::Vertical )
            {
               colStart = false;
               // vertical splitter, split up the current column
               int w1 = ( int ) ( width * 0.01 * sp->separatorPos() + 0.5 );
               int w2 = width - w1;
               if( w1 == 0 ) w1++;
               if( w2 == 0 ) w2++;

               QList< QList< PMViewLayoutEntry > >::iterator cit1
                  = cols.insert( cit, QList< PMViewLayoutEntry >() );

               recursiveExtractColumns( cols, cit1, w1, sp->getFirst() );
               recursiveExtractColumns( cols, cit, w2, sp->getLast() );
            }
         }
         if( colStart )
         {
            // widget is a view, a horizontal splitter or a tab widget
            // a new column starts
            PMViewLayoutEntry e;
            e.setColumnWidth( width );
            ( *cit ).append( e );
            recursiveExtractOneColumn( *cit, ( *cit ).begin(), 100, dw );
         }
      }
   }
}

void PMViewLayout::recursiveExtractOneColumn(
   QList< PMViewLayoutEntry >& entries,
   QList< PMViewLayoutEntry >::iterator eit,
   int height, QWidget* widget )
{
   if( !widget )
      return;

   if( widget->inherits( "PMDockWidget" ) )
   {
      PMDockWidget* dw = ( PMDockWidget* ) widget;
      QWidget* w = dw->getWidget();
      if( w )
      {
         if( w->inherits( "PMDockSplitter" ) )
         {
            PMDockSplitter* sp = ( PMDockSplitter* ) w;
            // splitter, split up the current column
            int h1 = ( int ) ( height * 0.01 * sp->separatorPos() + 0.5 );
            int h2 = height - h1;
            if( h1 == 0 ) h1++;
            if( h2 == 0 ) h2++;

            ( *eit ).setHeight( h1 );
            ( *eit ).setDockPosition( PMDockWidget::DockRight );

            PMViewLayoutEntry e;
            e.setHeight( h2 );
            e.setDockPosition( PMDockWidget::DockBottom );
            QList< PMViewLayoutEntry >::iterator eit1 = eit;
            eit1 = entries.insert( ++eit1, e );

            recursiveExtractOneColumn( entries, eit, h1, sp->getFirst() );
            recursiveExtractOneColumn( entries, eit1, h2, sp->getLast() );
         }
         else if( w->inherits( "PMDockTabGroup" ) )
         {
            PMDockTabGroup* g = ( PMDockTabGroup* ) w;
            int num = g->count();
            QWidget* tw;
            int i;
            for( i = 0; i < num; i++ )
            {
               tw = g->widget( i );
               if( i == 0 )
                  recursiveExtractOneColumn( entries, eit, height, tw );
               else
               {
                  PMViewLayoutEntry e;
                  e.setHeight( height );
                  e.setDockPosition( PMDockWidget::DockCenter );

                  eit++;
                  eit = entries.insert( eit, e );
                  recursiveExtractOneColumn( entries, eit, height, tw );
               }
            }
         }
         else
         {
            // a povmodeler view???
            if( w->inherits( "PMViewBase" ) )
            {
               PMViewBase* v = ( PMViewBase* ) w;
               ( *eit ).setViewType( v->viewType() );
               PMViewOptions* vo =
                  PMViewFactory::theFactory()->newOptionsInstance( v->viewType() );
               if( vo )
               {
                  v->saveViewConfig( vo );
                  ( *eit ).setCustomOptions( vo );
               }
            }
         }
      }
   }
}

//=============== PMViewLayoutManager ===================

PMViewLayoutManager::PMViewLayoutManager()
{
   m_layoutsLoaded = false;
   m_layoutDisplayed = false;
   loadData();
}

PMViewLayoutManager::~PMViewLayoutManager()
{
}

void PMViewLayoutManager::setDefaultLayout( const QString& name )
{
   m_defaultLayout = name;
}

QStringList PMViewLayoutManager::availableLayouts()
{
   QStringList result;
   QList<PMViewLayout>::iterator it;

   for( it = m_layouts.begin(); it != m_layouts.end(); ++it )
      result.push_back( ( *it ).name() );

   return result;
}

void PMViewLayoutManager::loadData()
{
   if( m_layoutsLoaded )
      m_layouts.clear();

   m_layoutsLoaded = true;

   QString fileName = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "povmodeler/viewlayouts.xml" );
   if( fileName.isEmpty() )
   {
      // Generate a default layout
      // I have a feeling this shouldn't be here but hey, it works for now
      // TODO Must find a way to move this cleanly to PMShell
      PMViewLayout a;
      a.setName( tr( "Default" ) );
      PMViewLayoutEntry p;
      p.setViewType( "treeview" );
      p.setDockPosition( PMDockWidget::DockRight );
      p.setHeight( 50 );
      p.setColumnWidth( 33 );
      a.addEntry( p );
      p.setViewType( "dialogview" );
      p.setDockPosition( PMDockWidget::DockBottom );
      p.setHeight( 50 );
      a.addEntry( p );
      p.setViewType( "glview" );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewPosX ) );
      p.setDockPosition( PMDockWidget::DockRight );
      p.setHeight( 50 );
      p.setColumnWidth( 33 );
      a.addEntry( p );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewNegY ) );
      p.setDockPosition( PMDockWidget::DockBottom );
      p.setHeight( 50 );
      a.addEntry( p );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewPosZ ) );
      p.setDockPosition( PMDockWidget::DockRight );
      p.setHeight( 50 );
      p.setColumnWidth( 33 );
      a.addEntry( p );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewCamera ) );
      p.setDockPosition( PMDockWidget::DockBottom );
      p.setHeight( 50 );
      a.addEntry( p );

      m_layouts.append( a );
      m_defaultLayout = a.name();

      return;
   }

   QFile file( fileName );
   if( !file.open( QIODevice::ReadOnly ) )
   {
      qCritical(  ) << tr( "Could not open the view layouts file." )
                        << Qt::endl;
      return;
   }

   QDomDocument doc( "VIEWLAYOUTS" );
   doc.setContent( &file );

   QDomElement e = doc.documentElement();
   m_defaultLayout = e.attribute( "default", "empty" );

   QDomNode c = e.firstChild();

   QString str;

   while( !c.isNull() )
   {
      if( c.isElement() )
      {
         QDomElement ce = c.toElement();
         PMViewLayout v;
         v.loadData( ce );
         m_layouts.append( v );
      }
      c = c.nextSibling();
   }
}

void PMViewLayoutManager::saveData()
{
   QString fileName = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + "povmodeler/viewlayouts.xml" ;
   if( fileName.isEmpty() )
   {
      qCritical(  ) << tr( "View layouts not found." ) << Qt::endl;
      return;
   }
   QFile file( fileName );
   if( !file.open( QIODevice::WriteOnly ) )
   {
      qCritical(  ) << tr( "Could not open the view layouts file." )
                        << Qt::endl;
      return;
   }
   QDomDocument doc( "VIEWLAYOUTS" );
   QDomElement e = doc.createElement( "viewlist" );
   e.setAttribute( "default", m_defaultLayout );

   QList<PMViewLayout>::iterator it;

   for( it = m_layouts.begin(); it != m_layouts.end(); ++it )
   {
      QDomElement l;

      l = doc.createElement( "viewlayout" );
      ( *it ).saveData( l, doc );
      e.appendChild( l );
   }
   doc.appendChild( e );
   QTextStream str( &file );
   str.setCodec( QTextCodec::codecForName( "UTF-8" ) );
   str << doc.toString();
   file.close();
}

PMViewLayoutManager* PMViewLayoutManager::theManager()
{
   if( !s_pInstance )
      //s_staticDeleter.setObject( s_pInstance, new PMViewLayoutManager() );
       s_pInstance = new PMViewLayoutManager() ;
   return s_pInstance;
}

void PMViewLayoutManager::displayLayout( const QString& name, PMShell* shell )
{
   PMViewLayout* v_layout = findLayout( name );
	if( v_layout ) displayLayout( v_layout, shell );
}

void PMViewLayoutManager::displayLayout( int i, PMShell* shell )
{
	if(i >= 0 && m_layouts.count() > i) displayLayout( &m_layouts[i], shell );
}

void PMViewLayoutManager::displayLayout( PMViewLayout* v_layout, PMShell* shell ){
   if( v_layout )
   {
      // Destroy the existing dock widgets
      if( m_layoutDisplayed )
      {
            QWidgetList* lst = new QWidgetList();

         if( shell->centralWidget() )
            shell->manager()->findChildDockWidget( shell->centralWidget(), lst );
         while( lst->count() > 0 )
         {
            ( ( PMDockWidget* )lst->first() )->undock();
            ( ( PMDockWidget* )lst->first() )->close();
            lst->erase( lst->begin() );
         }
			delete lst;

			QList<PMDockWidget*> flist = shell->manager()->findFloatingWidgets();
			QList<PMDockWidget*>::ConstIterator it;

            for(it = flist.constBegin(); it != flist.constEnd(); ++it )
         {
            ( *it )->undock();
            ( *it )->close();
         }
      }
      // Create the new layout
      v_layout->displayLayout( shell );
      m_layoutDisplayed = true;
   }
}

void PMViewLayoutManager::displayDefaultLayout( PMShell* shell )
{
   displayLayout( m_defaultLayout, shell );
}

PMViewLayout* PMViewLayoutManager::findLayout( const QString& name )
{
   QList<PMViewLayout>::iterator it;
   for( it = m_layouts.begin(); it != m_layouts.end() &&
        ( *it ).name() != name; ++it )
       ;

   if( it == m_layouts.end() )
      return nullptr;
   return &( *it );
}

void PMViewLayoutManager::fillPopupMenu( QMenu* pMenu )
{
   QStringList lst = availableLayouts();
   QStringList::ConstIterator it = lst.constBegin();
	int i = 0;

   pMenu->clear();
   if( it != lst.constEnd() )
   {
      for( ; it != lst.constEnd(); ++it )
		{
         QAction* act = pMenu->addAction( ( *it ) );
			act->setData( i );
			i++;
		}
   }
}

void PMViewLayoutManager::addLayout( const QString& name )
{
   PMViewLayout a;

   if( m_layouts.isEmpty() )
      m_defaultLayout = name;
   a.setName( name );
   m_layouts.append( a );
}

void PMViewLayoutManager::removeLayout( const QString& name )
{
   QList<PMViewLayout>::iterator it;
   for( it = m_layouts.begin(); it != m_layouts.end() &&
        ( *it ).name() != name; ++it )
       ;

   if( it != m_layouts.end() )
      m_layouts.erase( it );
}


PMSaveViewLayoutDialog::PMSaveViewLayoutDialog( PMShell* parent )
      : QDialogButtonBox( parent)
{
	setWindowTitle( tr( "Save View Layout" ) );
    QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QWidget *mainWidget = new QWidget( this );
	QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout( mainLayout );
    mainLayout->addWidget( mainWidget );

    QPushButton *okButton = buttonBox->button( QDialogButtonBox::Ok );
	okButton->setDefault(true);
    okButton->setShortcut( Qt::CTRL | Qt::Key_Return );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

   //PORTING SCRIPT: WARNING mainLayout->addWidget(buttonBox) must be last item in layout. Please move it.
   mainLayout->addWidget( buttonBox );

   m_pShell = parent;

   //KGuiItem::assign(okButton, KStandardGuiItem::save());
   okButton->setEnabled( false );

   QWidget* w = new QWidget( this );
   QVBoxLayout* vl = new QVBoxLayout( w );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );
   vl->setMargin( 0 );

   QLabel* l = new QLabel( tr( "Enter view layout name:" ), w );
   vl->addWidget( l );

   m_pLayoutName = new QLineEdit( w );
   vl->addWidget( m_pLayoutName );
   connect( m_pLayoutName, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotNameChanged( const QString& ) ) );

   QListWidget* lb = new QListWidget( w );
   vl->addWidget( lb );
   connect( lb, SIGNAL( currentTextChanged( const QString& ) ),
            SLOT( slotNameSelected( const QString& ) ) );
   lb->addItems( PMViewLayoutManager::theManager()->availableLayouts() );

   connect( okButton,SIGNAL( clicked() ),this,SLOT( slotOk() ) );
//PORTING: Verify that widget was added to mainLayout:    setMainWidget( w );
// Add mainLayout->addWidget(w); if necessary
   mainLayout->addWidget( w );
   resize( QSize( 300, 200 ) );
}

PMSaveViewLayoutDialog::~PMSaveViewLayoutDialog()
{
}

void PMSaveViewLayoutDialog::slotOk()
{
   QString name = m_pLayoutName->text();

   PMViewLayoutManager* m = PMViewLayoutManager::theManager();
   PMViewLayout* layout = m->findLayout( name );

   PMViewLayout newLayout = PMViewLayout::extractViewLayout( m_pShell );
   newLayout.setName( name );

   if( layout )
      *layout = newLayout;
   else
      m->addLayout( newLayout );

   m->saveData();

   //QDialog::exec();
   QDialogButtonBox::show();
}

void PMSaveViewLayoutDialog::slotNameChanged( const QString& s )
{
   //okButton->setEnabled( !s.isEmpty() );
    PMSaveViewLayoutDialog::button( QDialogButtonBox::Ok )->setEnabled( !s.isEmpty() );
}

void PMSaveViewLayoutDialog::slotNameSelected( const QString& s )
{
   m_pLayoutName->setText( s );
}

#include "pmviewlayoutmanager.moc"
