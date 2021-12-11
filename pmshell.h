//-*-C++-*-
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


#ifndef PMSHELL_H
#define PMSHELL_H

#include "pmdockwidget.h"

//#include "pmpart.h"
#include "pmglview.h"
#include "version.h"

#include <QMenu>
#include <libpovmodeler_export.h>
#include <QToolBar>
//class KHelpMenu;
class QStatusBar;
class PMViewOptions;
class PMPart;

/**
 * Main view for PovModeler
 */
class LPOVMODELER_EXPORT PMShell : public PMDockMainWindow
{
   Q_OBJECT

public:
   /**
    * Standard constructor
    */
   PMShell( const QUrl& = QUrl() );
   /**
    * Destructor
    */
   virtual ~PMShell();

   QMenu* getLayouts(){ return layout_viewMenu_submenu; }

   QMenu* get_editMenu(){ return editMenu; }
   QMenu* get_viewMenu(){ return viewMenu; }
   QMenu* get_fileMenu(){ return fileMenu; }
   QMenu* get_insertMenu(){ return insertMenu; }
   /**
    * Creates the actions
    */
   void setupActions();
   /**
    * Creates the docking views
    */
   void setupView();
   /**
    * Updates the gui (menus and toolbars)
    */
   void updateGUI();

   /**
    * Checks if a file with that name exists and asks if
    * it should be overwritten.
    *
    * Returns true if the files is not a local file, the file does'n exist
    * or the file should be overwritten.
    */
   //static bool overwriteURL( const QUrl &u ); eticre unnecessary
   /***eticre view-hide Declare with name have "_" at first char***/
   /**
     eticre show hidden obj not_in_tree
   */
   bool show_list;
   //using KMainWindow::setWindowTitle;

public slots:
   void openUrl( const QUrl& );
   void slotOpenRecent( QAction* action );

   void slotFileNew();
   void slotFileOpen();
   void slotFileSave();
   void slotFileSaveAs();
   void slotFileRevert();
   void slotFilePrint();
   void slotFileNewWindow();
   void slotFileClose();

   void slotShowStatusbar( bool b );
   void slotShowPath();
   /***eticre show lib***/
   void slotShowList();
   void slotSettings();
   void slotNewGraphicalView( PMGLView::PMViewType );
   void slotNewTopView();
   void slotNewBottomView();
   void slotNewLeftView();
   void slotNewRightView();
   void slotNewFrontView();
   void slotNewBackView();
   void slotNewCameraView();
   void slotNewTreeView();
   void slotNewDialogView();
   void slotNewLibraryBrowserView();

   void saveOptions();
   void restoreOptions();

   void setWindowTitle( const QString& caption = QString() );
   void statusMsg( const QString& text = QString() );
   void slotControlPointMsg( const QString& msg = QString() );

   void slotModified();
   void slotDockWidgetClosed();
   void slotDeleteClosedObjects();
   void slotSelectedLayout( QAction* action );
   void slotSaveViewLayout();
   void slotViewsMenuAboutToShow();

protected:
    virtual bool queryClose();
	virtual void showEvent( QShowEvent* );
    void saveAs();
    void setSize();

public:
   /**
    * Creates a dock widget of view type t with custom config c.
    *
    * If initPosition is true, the widget is docked to the desktop.
    */
   PMDockWidget* createView( const QString& t, PMViewOptions* c = nullptr,
                             bool initPosition = true );

private:

   void closeEvent( QCloseEvent *event );
   void shellClose();

   void restoreRecent();
   QList<QAction*>recentFileAction;
   QList<QVariant>recent_urls;
   void add_recentFiles( QUrl url );

   //KRecentFilesAction* m_pRecent;
   QAction* m_pToolbarAction_sp;
   QAction* m_pToolbarAction_fp;
   QAction* m_pToolbarAction_csg;
   QAction* m_pToolbarAction_ip;
   QAction* m_pToolbarAction_gdl;
   QAction* m_pToolbarAction_material;
   QAction* m_pToolbarAction_interior;
   QAction* m_pToolbarAction_texture;
   QAction* m_pToolbarAction_photons;
   QAction* m_pToolbarAction_athmo;
   QAction* m_pToolbarAction_transform;

   QAction* m_pStatusbarAction;

   QAction* m_pPathAction;
   QAction* m_pListAction;
   QAction* m_pPreferAaction;
   QAction* m_pConfigToolbar;
   QAction* m_pNewTreeViewAction;
   QAction* m_pNewDialogViewAction;
   QAction* m_pNewTopViewAction;
   QAction* m_pNewBottomViewAction;
   QAction* m_pNewLeftViewAction;
   QAction* m_pNewRightViewAction;
   QAction* m_pNewFrontViewAction;
   QAction* m_pNewBackViewAction;
   QAction* m_pNewCameraViewAction;
   QAction* m_pNewLibraryBrowserAction;
   QAction* m_pSaveOptions;
   QAction* m_pSaveViewLayoutAction;
   QAction* m_pImportAction;
   QAction* m_pExportAction;

   QStatusBar* m_pStatusBar;
   QLabel* statusBarLabel;
   PMPart* m_pPart;
   QUrl m_openRecentURL;
   QUrl currentUrl;

   QList<QObject*> m_objectsToDelete;
   int m_viewNumber;
   QMenu* fileMenu;
   QMenu* editMenu;
   QMenu* viewMenu;
   QMenu* insertMenu;
   QMenu settingsMenu;
   QMenu* layout_viewMenu_submenu;
   QMenu* menu_open_recent;
   QMenuBar* menu_Bar;
   QToolBar* m_pToolBar;
   QToolBar* m_pToolbar_sp;
   QToolBar* m_pToolbar_fp;
   QToolBar* m_pToolbar_csg;
   QToolBar* m_pToolbar_gdl;
   QToolBar* m_pToolbar_ip;
   QToolBar* m_pToolbar_material;
   QToolBar* m_pToolbar_interior;
   QToolBar* m_pToolbar_texture;
   QToolBar* m_pToolbar_photons;
   QToolBar* m_pToolbar_athmo;
   QToolBar* m_pToolbar_transform;

};

#endif
