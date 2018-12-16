/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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
#include "pmpart.h"
#include "pmsettingsdialog.h"
#include "pmdefaults.h"
#include "pmdockwidget.h"
#include "pmviewbase.h"
#include "pmunknownview.h"
#include "pmviewlayoutmanager.h"
#include <QAction>
#include <QStatusBar>
#include <QFileDialog>
#include <QMenuBar>
#include <QTimer>
#include <QSettings>
#include <QStandardPaths>
#include <QMessageBox>
#include <QShowEvent>

#define KPM_WITH_OBJECT_LIBRARY

PMShell::PMShell( const QUrl &url )
      : PMDockMainWindow( nullptr )
{
   setAttribute( Qt::WA_DeleteOnClose );
   statusBarLabel = new QLabel( this );
   currentUrl = nullptr;
   /***call pmpart with mainwidget***/

   m_viewNumber = 0;
   /***eticre view-hide Declare with name have "_" at first char***/
   show_list = false;

   QString ruleFile = QStandardPaths::locate( QStandardPaths::GenericDataLocation, QString( "povmodeler/povmodelershell.rc" ) );
   QList<QToolBar *> allToolBars = this->findChildren<QToolBar *>();
   foreach(QToolBar *tb, allToolBars) {
       // This does not delete the tool bar.
       this->removeToolBar(tb);
   }
   m_pToolBar = new QToolBar;
   m_pToolbar_sp = new QToolBar;//solid primitives toolbar
   m_pToolbar_fp = new QToolBar;//finite patch toolbar
   m_pToolbar_csg = new QToolBar;//construct soid geom. toolbar
   m_pToolbar_gdl = new QToolBar;
   m_pToolbar_ip = new QToolBar;
   m_pToolbar_material = new QToolBar;
   m_pToolbar_interior = new QToolBar;
   m_pToolbar_texture = new QToolBar;
   m_pToolbar_photons = new QToolBar;
   m_pToolbar_athmo = new QToolBar;
   m_pToolbar_transform = new QToolBar;

   menu_Bar = new QMenuBar;
   editMenu = new QMenu;
   viewMenu = new QMenu;
   fileMenu = new QMenu;
   insertMenu = new QMenu;
   menu_open_recent = new QMenu;
   fileMenu->setTitle("File");
   editMenu->setTitle(tr("Edit"));
   viewMenu->setTitle(tr("View"));
   insertMenu->setTitle(tr("Insert"));
   settingsMenu.setTitle(tr("Settings"));
   menu_open_recent->setTitle("Recent File");
   menu_open_recent->setToolTipsVisible(true);
   menu_Bar->addMenu(fileMenu);
   menu_Bar->addMenu(editMenu);
   menu_Bar->addMenu(viewMenu);
   menu_Bar->addMenu(insertMenu);
   menu_Bar->addMenu(&settingsMenu);
   setMenuBar(menu_Bar);

   setupActions();

   m_pPart = new PMPart( this, this, true, this );
   m_pToolbar_sp->addActions( m_pPart->getMenu( "menuSolidPri" )->actions() );
   m_pToolbar_fp->addActions( m_pPart->getMenu( "menuFinitePatch" )->actions() );
   m_pToolbar_ip->addActions( m_pPart->getMenu( "menuInfiniteSolid" )->actions() );
   m_pToolbar_csg->addActions( m_pPart->getMenu( "menuCsg" )->actions() );
   m_pToolbar_gdl->addActions( m_pPart->getMenu( "menu_gdl" )->actions() );
   m_pToolbar_material->addActions( m_pPart->getMenu( "menuMaterial" )->actions() );
   m_pToolbar_interior->addActions( m_pPart->getMenu( "menuInterior" )->actions() );
   m_pToolbar_texture->addActions( m_pPart->getMenu( "menuTexture" )->actions() );
   m_pToolbar_photons->addActions( m_pPart->getMenu( "menuPhotons" )->actions() );
   m_pToolbar_athmo->addActions( m_pPart->getMenu( "menuAthmo" )->actions() );
   m_pToolbar_transform->addActions( m_pPart->getMenu( "menuTrans" )->actions() );

   addToolBar( m_pToolbar_sp );
   addToolBar( m_pToolbar_fp );
   addToolBar( m_pToolbar_ip );
   addToolBar( m_pToolbar_csg );
   addToolBar( m_pToolbar_gdl );
   addToolBar( m_pToolbar_material );
   addToolBar( m_pToolbar_interior );
   addToolBar( m_pToolbar_texture );
   addToolBar( m_pToolbar_photons );
   addToolBar( m_pToolbar_athmo );
   addToolBar( m_pToolbar_transform );
   restoreRecent();
   //restoreOptions();
   setSize();
   setupView();

   m_pStatusBar = statusBar();
   m_pStatusBar->addWidget ( statusBarLabel, 1 );
   statusBarLabel->setText(" Status Bar ");
   //m_pStatusBar->showMessage( " ", c_statusBarInfo, 1 );
   //m_pStatusBar->showMessage( "" , c_statusBarControlPoints );

   if( !url.isEmpty() )
   {
      openUrl( url );
      currentUrl = url;
   }

   setWindowTitle( url.toDisplayString() );

   QSettings qset;
   restoreState( qset.value("mainwindow/state").toByteArray() );

   connect( m_pPart, SIGNAL( modified() ), SLOT( slotModified() ) );

   connect( m_pPart, SIGNAL( controlPointMessage( const QString& ) ),
                       SLOT( slotControlPointMsg( const QString& ) ) );
}

PMShell::~PMShell()
{
    QSettings qset;
    qset.setValue( "mainwindow/size", this->size() );
    qset.setValue( "mainwindow/fullscreen", this->isFullScreen() );
    qset.setValue( "mainwindow/state", saveState(0) );
    delete m_pToolBar;
    delete m_pToolbarAction_fp;
    delete m_pToolbarAction_sp;
    qDebug() << "pmshell desctructor";
    qDebug() << "pmshell01";
}

void PMShell::setSize()
{
    QSettings qset;
    if ( qset.value( "mainwindow/fullscreen" ).toBool() )
       this->showFullScreen();
    else if ( qset.contains( "mainwindow/size" ) )
        this->resize( qset.value( "mainwindow/size" ).toSize() );
    else
        this->resize( 1024, 800 );
}

void PMShell::setupActions()
{
    QAction* newFile = new QAction( "New" );
    fileMenu->addAction(newFile);
    connect( newFile, &QAction::triggered, this, &PMShell::slotFileNew );

   QAction* open = new QAction( "Open" );
   connect( open, &QAction::triggered, this, &PMShell::slotFileOpen );
   fileMenu->addAction(open);

   fileMenu->addMenu( menu_open_recent );
   connect( menu_open_recent, &QMenu::triggered, this, &PMShell::slotOpenRecent );

   //m_pRecent = KStandardAction::openRecent( this, SLOT( slotOpenRecent( const QUrl& ) ),
    //                                   actionCollection() );
   QAction* save = new QAction( "Save" );
   connect( save, &QAction::triggered, this, &PMShell::slotFileSave );
   fileMenu->addAction(save);

   QAction* saveAs = new QAction( "Save As" );
   connect( saveAs, &QAction::triggered, this, &PMShell::slotFileSaveAs );
   fileMenu->addAction(saveAs);

   QAction* revert = new QAction( "Revert" );
   connect( revert, &QAction::triggered, this, &PMShell::slotFileRevert );
   fileMenu->addAction(revert);

   fileMenu->addSeparator();

   QAction* print = new QAction( "Print" );
   connect( print, &QAction::triggered, this, &PMShell::slotFilePrint );

   fileMenu->addSeparator();
   fileMenu->addAction(print);

   fileMenu->addSeparator();

   QAction* close = new QAction( "Close" );
   connect( close, &QAction::triggered, this, &PMShell::slotFileClose );
   fileMenu->addAction(close);

   QAction* quit = new QAction( "Quit", this );
   connect( quit, &QAction::triggered, this, &PMShell::shellClose );
   fileMenu->addAction(quit);

   /***eticre add showlib***/
   m_pListAction = settingsMenu.addAction("options_show_lib");
   m_pListAction->setCheckable( true );
   m_pListAction->setText( tr( "Show &List" ) );
   connect( m_pListAction, SIGNAL( triggered() ), this, SLOT( slotShowList() ) );
   //m_pPathAction->setCheckedState( KGuiItem( tr( "Hide &List" ) ) );
   
   m_pPathAction = settingsMenu.addAction("options_show_path");
   m_pPathAction->setCheckable( true );
   m_pPathAction->setText( tr( "Show &Path" ) );
   connect( m_pPathAction, SIGNAL( triggered() ), this, SLOT( slotShowPath() ) );

   m_pStatusbarAction = settingsMenu.addAction("showStatusbar");
   m_pStatusbarAction->setCheckable( true );
   connect( m_pStatusbarAction, SIGNAL( triggered() ), this, SLOT( saveOptions() ) );
   settingsMenu.addAction("");
   settingsMenu.addSection("Toolbars");

   m_pToolbarAction_sp = m_pToolbar_sp->toggleViewAction();
   m_pToolbarAction_sp->setText("Solid Primitives");
   settingsMenu.addAction( m_pToolbarAction_sp );

   m_pToolbarAction_fp = m_pToolbar_fp->toggleViewAction();
   m_pToolbarAction_fp->setText( "Finite Patch" );
   settingsMenu.addAction( m_pToolbarAction_fp );

   m_pToolbarAction_ip = m_pToolbar_ip->toggleViewAction();
   m_pToolbarAction_ip->setText( "Infinite Solid" );
   settingsMenu.addAction( m_pToolbarAction_ip );

   m_pToolbarAction_csg = m_pToolbar_csg->toggleViewAction();
   m_pToolbarAction_csg->setText( "CSG" );
   settingsMenu.addAction( m_pToolbarAction_csg );

   m_pToolbarAction_material = m_pToolbar_material->toggleViewAction();
   m_pToolbarAction_material->setText( "Material" );
   settingsMenu.addAction( m_pToolbarAction_material );

   m_pToolbarAction_interior = m_pToolbar_interior->toggleViewAction();
   m_pToolbarAction_interior->setText( "Interior" );
   settingsMenu.addAction( m_pToolbarAction_interior );

   m_pToolbarAction_texture = m_pToolbar_texture->toggleViewAction();
   m_pToolbarAction_texture->setText( "Texture" );
   settingsMenu.addAction( m_pToolbarAction_texture );

   m_pToolbarAction_photons = m_pToolbar_photons->toggleViewAction();
   m_pToolbarAction_photons->setText( "Photons" );
   settingsMenu.addAction( m_pToolbarAction_photons );

   m_pToolbarAction_athmo = m_pToolbar_athmo->toggleViewAction();
   m_pToolbarAction_athmo->setText( "Athmospheric" );
   settingsMenu.addAction( m_pToolbarAction_athmo );

   m_pToolbarAction_transform = m_pToolbar_transform->toggleViewAction();
   m_pToolbarAction_transform->setText( "Transformation" );
   settingsMenu.addAction( m_pToolbarAction_transform );

   m_pToolbarAction_gdl = m_pToolbar_gdl->toggleViewAction();
   m_pToolbarAction_gdl->setText("Global Detail Level");
   settingsMenu.addAction(m_pToolbarAction_gdl);

   settingsMenu.addAction("");
   settingsMenu.addSection("App. Layout");
   m_pPreferAaction = settingsMenu.addAction("Preferences");
   connect( m_pPreferAaction, SIGNAL( triggered() ), this, SLOT( slotSettings() ) );

   m_pSaveOptions = settingsMenu.addAction("Save Option");
   connect( m_pSaveOptions, SIGNAL( triggered() ), this, SLOT( saveOptions() ) );

   m_pNewTreeViewAction = viewMenu->addAction("view_new_treeview");
   m_pNewTreeViewAction->setText( tr( "New Object Tree" ) );
   connect( m_pNewTreeViewAction, SIGNAL( triggered() ), this, SLOT( slotNewTreeView() ) );

   m_pNewDialogViewAction = viewMenu->addAction( "view_new_dialogview" );
   m_pNewDialogViewAction->setText( tr( "New Properties View" ) );
   connect( m_pNewDialogViewAction, SIGNAL( triggered() ), this, SLOT( slotNewDialogView() ) );

#ifdef KPM_WITH_OBJECT_LIBRARY
   m_pNewLibraryBrowserAction = viewMenu->addAction( "view_new_librarybrowser" );
   m_pNewLibraryBrowserAction->setText( tr( "New Library Browser" ) );
   connect( m_pNewLibraryBrowserAction, SIGNAL( triggered() ), this, SLOT( slotNewLibraryBrowserView() ) );
#endif

   viewMenu->addSeparator();

   m_pNewTopViewAction = viewMenu->addAction( "view_new_topview" );
   m_pNewTopViewAction->setText( tr( "New Top View" ) );
   connect( m_pNewTopViewAction, SIGNAL( triggered() ), this, SLOT( slotNewTopView() ) );

   m_pNewBottomViewAction = viewMenu->addAction( "view_new_bottomview" );
   m_pNewBottomViewAction->setText( tr ( "New Bottom View" ) );
   connect( m_pNewBottomViewAction, SIGNAL( triggered() ), this, SLOT( slotNewBottomView() ) );

   m_pNewLeftViewAction = viewMenu->addAction( "view_new_leftview" );
   m_pNewLeftViewAction->setText( tr( "New Left View" ) );
   connect( m_pNewLeftViewAction, SIGNAL( triggered() ), this, SLOT( slotNewLeftView() ) );

   m_pNewRightViewAction = viewMenu->addAction( "view_new_rightview" );
   m_pNewRightViewAction->setText( tr( "New Right View" ) );
   connect( m_pNewRightViewAction, SIGNAL( triggered() ), this, SLOT( slotNewRightView() ) );

   m_pNewFrontViewAction = viewMenu->addAction( "view_new_frontview" );
   m_pNewFrontViewAction->setText( tr( "New Front View" ) );
   connect( m_pNewFrontViewAction, SIGNAL( triggered() ), this, SLOT( slotNewRightView() ) );

   m_pNewBackViewAction = viewMenu->addAction( "view_new_backview" );
   m_pNewBackViewAction->setText( tr( "New Back View" ) );
   connect( m_pNewBackViewAction, SIGNAL( triggered() ), this, SLOT( slotNewBackView() ) );

   m_pNewCameraViewAction = viewMenu->addAction( "view_new_cameraview" );
   m_pNewCameraViewAction->setText( tr( "New Camera View" ) );
   connect( m_pNewCameraViewAction, SIGNAL( triggered() ), this, SLOT( slotNewCameraView() ) );

   viewMenu->addSeparator();

   layout_viewMenu_submenu = viewMenu->addMenu( tr( "View Layouts" ) );

   connect( layout_viewMenu_submenu, SIGNAL( aboutToShow() ), SLOT( slotViewsMenuAboutToShow() ) );
   PMViewLayoutManager::theManager()->fillPopupMenu( layout_viewMenu_submenu );
   connect( layout_viewMenu_submenu, SIGNAL( triggered( QAction* ) ), SLOT( slotSelectedLayout( QAction* ) ) );


   m_pSaveViewLayoutAction = viewMenu->addAction( "save_view_layout" );
   m_pSaveViewLayoutAction->setText( tr( "Save View Layout..." ) );
   connect( m_pSaveViewLayoutAction, SIGNAL( triggered() ), this, SLOT( slotSaveViewLayout() ) );
}

void PMShell::setupView()
{
   PMViewLayoutManager::theManager()->displayDefaultLayout( this );
}


PMDockWidget* PMShell::createView( const QString& t, PMViewOptions* o,
                                   bool initPosition )
{
   PMDockWidget* dock = nullptr;
   PMViewBase* contents = nullptr;

   PMViewTypeFactory* factory = PMViewFactory::theFactory()->viewFactory( t );

   m_viewNumber++;
   QString name = QString( "View (%1)" ).arg( m_viewNumber );

   if( factory )
   {
      QString desc;
      // Create the appropriate dock widget
      if( o )
         desc = factory->description( o );
      else
         desc = factory->description();

      dock = createDockWidget( QIcon::fromTheme( factory->iconName() ).pixmap( 16, 16), nullptr,
                               desc, desc );

      contents = factory->newInstance( dock, m_pPart );

      if( o ) contents->restoreViewConfig( o );
   }
   else
   {
      // unknown view type
      dock = createDockWidget( QIcon::fromTheme( "Unknown" ).pixmap( 16, 16), nullptr,
                               "Unknown", "Unknown" );
      contents = new PMUnknownView( t, dock );
   }

   dock->setWidget( contents );
   connect( dock, SIGNAL( headerCloseButtonClicked() ),
                  SLOT( slotDockWidgetClosed() ) );

   if( initPosition )
   {
      if( t == "librarybrowserview" )
          dock->resize( 800 , 600 );
      else
          dock->resize( 300 , 400 );

      dock->manualDock( nullptr, PMDockWidget::DockDesktop, 50,
                        mapToGlobal( QPoint( 50, 50 ) ) );
   }
   return dock;
}

void PMShell::slotNewGraphicalView( PMGLView::PMViewType t )
{
   PMGLViewOptions* o = new PMGLViewOptions( t );
   createView( "glview", o );
   delete o;
}

void PMShell::slotNewTopView()
{
   slotNewGraphicalView( PMGLView::PMViewNegY );
}

void PMShell::slotNewBottomView()
{
   slotNewGraphicalView( PMGLView::PMViewPosY );
}

void PMShell::slotNewLeftView()
{
   slotNewGraphicalView( PMGLView::PMViewPosX );
}

void PMShell::slotNewRightView()
{
   slotNewGraphicalView( PMGLView::PMViewNegX );
}

void PMShell::slotNewFrontView()
{
   slotNewGraphicalView( PMGLView::PMViewPosZ );
}

void PMShell::slotNewBackView()
{
   slotNewGraphicalView( PMGLView::PMViewNegZ );
}

void PMShell::slotNewCameraView()
{
   slotNewGraphicalView( PMGLView::PMViewCamera );
}

void PMShell::slotNewDialogView()
{
   createView( "dialogview" );
}

void PMShell::slotNewTreeView()
{
   createView( "treeview" );
}

void PMShell::slotNewLibraryBrowserView()
{
   createView( "librarybrowserview" );
}

void PMShell::slotDockWidgetClosed()
{
   QObject* o = sender();
   if( o && o->inherits( "PMDockWidget" ) )
   {
      if( !m_objectsToDelete.contains( o ) )
      {
         m_objectsToDelete.append( o );
         QTimer::singleShot( 0, this, SLOT( slotDeleteClosedObjects() ) );
      }
   }
}

void PMShell::slotDeleteClosedObjects()
{
	foreach(QObject* o, m_objectsToDelete) delete o;
   m_objectsToDelete.clear();
}

void PMShell::openUrl( const QUrl &url )
{
   if( !m_pPart->ismodified && m_pPart->url().isEmpty() )
   {
      m_pPart->openFileQt( url );
      setWindowTitle( m_pPart->url().toDisplayString() );
   }
   else
   {
      PMShell *shell = new PMShell();
      shell->show();
      shell->openUrl( url );
   }
}

void PMShell::slotFileNew()
{
   if( !m_pPart->ismodified && m_pPart->url().isEmpty() )
   {
      m_pPart->newDocument();
      setWindowTitle();
   }
   else
   {
      PMShell *shell = new PMShell();
      shell->show();
   }
}

void PMShell::slotFileOpen()
{
   QUrl url = QFileDialog::getOpenFileUrl( this, "File",
      QUrl(), QString( "*.kpm|" ) +
     "Povray Modeler Files ( *.kpm )" +
     "\n*|" + "All Files" );

   if( !url.isEmpty() && !recent_urls.contains( url ))
   {
      int i = recentFileAction.length();
      if( i >= 9 )
      {
          recentFileAction.removeFirst();
          recent_urls.removeFirst();
      }

      recent_urls.append( url );
      QSettings qset;
      qset.setValue( "recent/recenturls", recent_urls );
      restoreRecent();
   }
   if( !url.isEmpty() )
      openUrl( url );
}

void PMShell::restoreRecent()
{
    recentFileAction.clear();
    QSettings qset;
    recent_urls = qset.value("recent/recenturls").toList();
    if ( recent_urls.length()>0 )
        for ( int i = 0; i < recent_urls.size(); ++i )
        {
           QAction* a = new QAction(this);
           a->setText( recent_urls.at(i).toUrl().fileName() );
           a->setData( recent_urls.at(i).toUrl() );
           a->setToolTip( recent_urls.at(i).toString() );
           recentFileAction.insert( 0, a );
        }
    menu_open_recent->clear();
    menu_open_recent->addActions( recentFileAction );
}

void PMShell::slotOpenRecent( QAction* action )
{
   m_pPart->openFileQt( action->data().toUrl() );
}

void PMShell::slotFileSave()
{
    m_pPart->slotAboutToSave();

   if( m_pPart->ismodified )
   {
      if( !currentUrl.isEmpty() && m_pPart->isReadWrite() )
         m_pPart->saveFileQt( currentUrl );
      else
         saveAs();

      setWindowTitle( m_pPart->url().toDisplayString() );
   }
   else
      emit statusMsg( tr( "No changes need to be saved" ) );
}

void PMShell::slotFileSaveAs()
{
    m_pPart->slotAboutToSave();
    saveAs();
}

void PMShell::saveAs()
{
   QFileDialog dlg( this );
   dlg.setNameFilter( "kpov file (*.kpm)" );
   dlg.setAcceptMode( QFileDialog::AcceptSave );

   if( dlg.exec() )
   {
       currentUrl = dlg.selectedUrls().at( 0 );
       if( QFileInfo( currentUrl.path() ).completeSuffix().isEmpty() )
          currentUrl.setPath( currentUrl.path() + ".kpm" );

      if( m_pPart->saveFileQt( currentUrl ) )
          setWindowTitle( currentUrl.toDisplayString() );
       else
          QMessageBox::warning( this, "Error", tr( "Could not save the file." ) );
   }
}

void PMShell::slotFileRevert()
{
   QUrl url = m_pPart->url();

   if( !url.isEmpty() )
      m_pPart->openFileQt( url );
//   else
//    slotFileNew();
}

void PMShell::slotFilePrint()
{
   //TODO
   //   m_pPart->slotPrint();
}

void PMShell::slotFileNewWindow()
{
   PMShell* shell = new PMShell;
   shell->show();
}

void PMShell::slotFileClose()
{
   if( m_pPart->closeUrl() )
   {
      m_pPart->closeDocument();
      m_pPart->newDocument();
      setWindowTitle();
   }
}

void PMShell::slotShowStatusbar( bool b )
{
   if( !b )
      statusBar()->hide();
   else
      statusBar()->show();
}
/***eticre view-hide Declare with name have "_" at first char***/
/***eticre refresh show lib view hidden not_in_tree***/
void PMShell::slotShowList()
{
  if( m_pPathAction->isChecked() )
      m_pPathAction->setText( tr( "Hide &List" ) );
  else
      m_pPathAction->setText( tr( "Show &List" ) );

   m_pPart->refreshView();
}

void PMShell::slotShowPath()
{
    if( m_pPathAction->isChecked() )
        m_pPathAction->setText( tr( "Hide &Path" ) );
    else
        m_pPathAction->setText( tr( "Show &Path" ) );

   setWindowTitle( m_pPart->url().toDisplayString() );
}

void PMShell::slotSettings()
{
   PMSettingsDialog dlg( m_pPart );
   dlg.exec();
}

void PMShell::updateGUI()
{
   //createGUI( nullptr );
   //createShellGUI( false );
   //createGUI( m_pPart );
}

void PMShell::saveOptions()
{
   QSettings qset;
   qset.setValue( "Appearance/ShowStatusbar", m_pStatusbarAction->isChecked() );
   slotShowStatusbar( m_pStatusbarAction->isChecked() );
   // eticre save recent file m_pRecent->saveEntries( "RecentFile" );

   if( m_pPart )
       m_pPart->saveConfig();

   //config.sync();
}

void PMShell::restoreOptions()
{
   QSettings qset;
   bool show;

   show = qset.value( "Appearance/ShowStatusbar", true ).value<bool>();
   m_pStatusbarAction->blockSignals( true );
   m_pStatusbarAction->setChecked( show );
   m_pStatusbarAction->blockSignals( false );
   statusBar()->setVisible(show);

   // eticre save recent filem_pRecent->loadEntries( "RecentFile" );
}

void PMShell::setWindowTitle( const QString& caption )
{
   QString tmp;

   if( caption.isEmpty() )
      tmp = tr( "unknown" );
   else
   {
      if( !m_pPathAction->isChecked() )
         tmp = caption.right( caption.length() - caption.lastIndexOf( '/' ) - 1 );
      else
         tmp = caption;
   }

   QMainWindow::setWindowTitle( tmp );//, m_pPart->isModified() );
}

void PMShell::statusMsg( const QString& text )
{
   m_pStatusBar->showMessage( text, 5000 );
}

void PMShell::closeEvent( QCloseEvent *event )
{
    shellClose();
}

void PMShell::shellClose()
{
   queryClose();
   m_pPart->deleteContents();
   this->close();

}

bool PMShell::queryClose()
{
   //return m_pPart->closeUrl();
   if( m_pPart->ismodified )
   {
       int i = QMessageBox::question( this, "Save", tr("The document has been modified.\n"
                                                        "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
       if( i == QMessageBox::Save )
       {
           saveAs();
           return true;
       }
       if( i == QMessageBox::Discard )
       {
           this->close();
           m_pPart->setModified( false );
           return true;
        }

       return false;
   }

   return true;
}

void PMShell::showEvent( QShowEvent* ){
   activateDock();
}

void PMShell::slotModified()
{
   setWindowTitle( m_pPart->url().toDisplayString() );
}

void PMShell::slotControlPointMsg( const QString& msg )
{
   if( msg.isEmpty() )
     statusBarLabel->setText( msg );
   else
     statusBarLabel->setText( QString( " " ) + msg + QString( " " ));
}

bool PMShell::overwriteURL( const QUrl& u )
{
   if( u.isLocalFile() )
   {
      QFileInfo info;
      QString name( u.path() );
      info.setFile( name );

      if( info.exists() )
      {
         QMessageBox m;
         m.setText( "Overwrite file" );
         m.setInformativeText( "A file with this name already exists.\nDo you want to overwrite it?" );
         m.setStandardButtons( QMessageBox::Save | QMessageBox::Cancel );
         if ( m.exec() == QMessageBox::Save )
             return( true );
      }
   }
   return ( false );
}

void PMShell::slotSelectedLayout( QAction* action )
{
   PMViewLayoutManager::theManager()->displayLayout( action->data().toInt(), this );
}

void PMShell::slotSaveViewLayout()
{
   PMSaveViewLayoutDialog dlg( this );
   dlg.show();
}

void PMShell::slotViewsMenuAboutToShow()
{
   QMenu* m = getLayouts();
   PMViewLayoutManager::theManager()->fillPopupMenu( m );
}
