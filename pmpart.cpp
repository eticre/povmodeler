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


// include files for QT

// application specific includes
#include "pmpart.h"
#include "pmshell.h"
#include "pmview.h"
#include "pmglview.h"
#include "pmallcommands.h"
#include "pmpovraywidget.h"
#include "pmpovrayrenderwidget.h"
#include "pmactions.h"
#include "pmallobjects.h"
#include "pmcommandmanager.h"
#include "pmobjectdrag.h"
#include "pmxmlparser.h"
#include "pmpovrayparser.h"
#include "pmerrordialog.h"
#include "pmsettingsdialog.h"
#include "pminserterrordialog.h"
#include "pminsertpopup.h"
#include "pmpovray35format.h"
#include "pmserializer.h"
#include "pmdefaults.h"
#include "pmsymboltable.h"
#include "pmrendermodesdialog.h"
#include "pmrendermode.h"
#include "pmpovrayoutputwidget.h"
#include "pmrendermanager.h"
#include "pmdialogeditbase.h"
#include "pmdocumentationmap.h"
//#ifdef KPM_WITH_OBJECT_LIBRARY
#include "pmlibrarymanager.h"
#include "pmlibraryhandleedit.h"
#include "pmlibraryobject.h"
#include "pmlibrarybrowser.h"
#include "pmlibraryobjectsearch.h"
#include "pmlibraryiconview.h"
#include "pmlibraryentrypreview.h"
#include "pmlibraryhandle.h"
//#endif
#include "pmscene.h"
#include "pmpluginmanager.h"
#include "pminsertrulesystem.h"
#include "pmprototypemanager.h"
#include "pmiomanager.h"
#include "pmrecursiveobjectiterator.h"
#include "pmerrorflags.h"
#include "pmfiledialog.h"
//#include "kstandardaction.h"
//#include "kactioncollection.h"
//#include "ktoolbarlabelaction.h"
#include "povmodeleradaptator.h"
#include <QApplication>
#include <QtGui>
#include <QDir>
#include <QClipboard>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QLabel>
#include <QDateTime>
#include <QStack>
#include <QStandardPaths>
#include <QComboBox>
// include files for KDE
#include <QIcon>
#include <QTemporaryFile>
#include <QtDBus>
#include <QSpinBox>
#include <QAction>
#include <QWidgetAction>
//#include <QtGui/private/qzipreader_p.h>

#ifdef PMEnableSimpleProfiling
QTime PMDebugTime;
#endif

/*mainwidget*/
PMPart::PMPart( QWidget* parentWidget,
                QObject* parent,
                bool readwrite,
                PMShell* shell )
    : m_commandManager( this )
{
   m_pActiveObject = nullptr;
   m_canDecode = false;
   m_pScene = nullptr;
   m_pNewSelection = nullptr;
   m_sortedListUpToDate = false;
   m_numAddedObjects = 0;
   m_numInsertErrors = 0;
   m_pSymbolTable = nullptr;
   m_bCameraListUpToDate = true;
   m_updateNewObjectActions = false;
   m_pPovrayWidget = nullptr;
   m_pView = nullptr;
   m_pShell = shell;
   m_onlyCopyPaste = true;
   radYN = false;
   ismodified = false;
   m_readWrite = false;
   m_pwidget = parentWidget;
   m_pparent = parent;

   // call inits to invoke all other construction parts
   setReadWrite( readwrite );

   m_pPrototypeManager = new PMPrototypeManager( this );
   m_pInsertRuleSystem = new PMInsertRuleSystem( this );
   m_pIOManager = new PMIOManager( this );
   m_pInsertRuleSystem->loadRules( "baseinsertrules.xml" );
   initActions();
   initDocument();
   initView( parentWidget );
   restoreConfig();

   connect( qApp->clipboard(), SIGNAL( dataChanged() ),
                                SLOT( slotClipboardDataChanged() ) );

   slotClipboardDataChanged();

   connect( &m_commandManager, SIGNAL( updateUndoRedo( const QString&, const QString& ) ),
            SLOT( slotUpdateUndoRedo( const QString&, const QString& ) ) );

   connect( &m_commandManager, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
            SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   connect( &m_commandManager, SIGNAL( idChanged( PMObject*, const QString& ) ),
            SLOT( slotIDChanged( PMObject*, const QString& ) ) );

   new povmodelerAdaptor( this );
   QDBusConnection::sessionBus().registerObject("/PMPart", this);
   // eticre no plugin PMPluginManager::theManager()->registerPart( this );

   emit refresh();
   slotObjectChanged( m_pScene, PMCNewSelection, this );
}

/**no mainwidget**/
PMPart::PMPart( QWidget* parentWidget,
                QObject* parent,
                bool readwrite,
                bool /*onlyCutPaste*/,
                PMShell* shell
              )
      : m_commandManager( this )
{
   m_pActiveObject = nullptr;
   m_canDecode = false;
   m_pNewSelection = nullptr;
   m_sortedListUpToDate = false;
   m_numAddedObjects = 0;
   m_numInsertErrors = 0;
   m_pSymbolTable = nullptr;
   m_bCameraListUpToDate = true;
   m_updateNewObjectActions = false;
   m_pPovrayWidget = nullptr;
   m_pView = nullptr;
   m_pCutAction = nullptr;
   m_pCopyAction = nullptr;
   m_pPasteAction = nullptr;
   m_pUndoAction = nullptr;
   m_pRedoAction = nullptr;
   m_pDeleteAction = nullptr;
   m_pHideAction = nullptr;
   m_pShowAction = nullptr;
   editMenu = nullptr;
   m_pShell = shell;
   m_pScene = new PMScene( this );
   m_onlyCopyPaste = true;
   radYN = false;
   ismodified = false;
   m_pwidget = parentWidget;
   m_pparent = parent;

   // call inits to invoke all other construction parts
   setReadWrite( readwrite );
   m_pPrototypeManager = new PMPrototypeManager( this );
   m_pInsertRuleSystem = new PMInsertRuleSystem( this );
   m_pIOManager = new PMIOManager( this );
   m_pInsertRuleSystem->loadRules( "baseinsertrules.xml" );
   m_pSymbolTable = new PMSymbolTable();

   if(m_pShell)
   {
       initActions();
       initDocument();
   }
   initCopyPasteActions();

   connect( &m_commandManager, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
            SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   new povmodelerAdaptor(this);
   QDBusConnection::sessionBus().registerObject("/LibraryBrowser", this);
   emit refresh();
}


PMPart::~PMPart()
{
   //deleteContents();
   //qDebug(PMArea) << "pmpart01";
   for (int i = 0; i < m_readWriteActions.size(); ++i) {
        delete m_readWriteActions.at(i);
   }
   delete m_pparent;
   delete m_pwidget;
   delete m_pView;
   m_pView = nullptr;
   delete m_pCutAction;
   delete m_pCopyAction;
   delete m_pPasteAction;
   delete m_pUndoAction;
   delete m_pRedoAction;
   delete m_pDeleteAction;
   delete m_pHideAction;
   delete m_pShowAction;
   m_pCutAction = nullptr;
   m_pCopyAction = nullptr;
   m_pPasteAction = nullptr;
   m_pUndoAction = nullptr;
   m_pRedoAction = nullptr;
   m_pDeleteAction = nullptr;
   m_pHideAction = nullptr;
   m_pShowAction = nullptr;
   delete menuSolidPri;
   delete menuFinitePatch;
   delete menuInfiniteSolid;
   delete menuCsg;
   delete menuMaterial;
   delete menuInterior;
   delete menuTexture;
   delete menuPhotons;
   delete menuAthmo;
   delete menuTrans;
   menuSolidPri = nullptr;
   menuFinitePatch = nullptr;
   menuInfiniteSolid = nullptr;
   menuCsg = nullptr;
   menuMaterial = nullptr;
   menuInterior = nullptr;
   menuTexture = nullptr;
   menuPhotons = nullptr;
   menuAthmo = nullptr;
   menuTrans = nullptr;
   delete editMenu;
   editMenu = nullptr;
   delete menu_gdl;
   menu_gdl = nullptr;
   delete menuRenderModes;
   delete m_pRenderComboAction;
   if( m_pSymbolTable )
       delete m_pSymbolTable;

   if( m_pPovrayWidget )
       delete m_pPovrayWidget;
}

QAction* PMPart::actionCollection( QString name, QMenu* menu=nullptr )
{
  QAction* a;
  QString s;
  s = name.remove(0,4);
  s.prepend(":/smallicon/icons/povicons/small/pm");
  if(!menu) menu = insertMenu;
  a = menu->addAction(name);
  a->setIcon( QIcon( s ) );
  m_hash_readWriteActions[name] = a;
  return a;
}

bool PMPart::get_radYN()
{
     return 0;
}

void PMPart::initCopyPasteActions()
{
   // setup edit menu
   if ( !editMenu )
       editMenu = new QMenu();

   //if( m_pCutAction ) return;

   m_pCutAction = editMenu->addAction( "Cut" );
   connect( m_pCutAction, &QAction::triggered, this, &PMPart::slotEditCut );

   m_pCopyAction = editMenu->addAction( "Copy" );
   connect( m_pCopyAction, &QAction::triggered, this, &PMPart::slotEditCopy );

   m_pPasteAction = editMenu->addAction( "Paste" );
   connect( m_pPasteAction, &QAction::triggered, this, &PMPart::slotEditPaste );


   //m_pDeleteAction = new QAction( tr( "Delete" ), "edittrash", Qt::Key_Delete, this, SLOT( slotEditDelete() ), actionCollection(), "edit_delete" );
   m_pDeleteAction = editMenu->addAction( "Delete" );
   m_pDeleteAction->setShortcut( Qt::Key_Delete );
   m_pDeleteAction->setIcon( QIcon::fromTheme( "user-trash" ) );
   connect( m_pDeleteAction, SIGNAL( triggered( bool ) ), this, SLOT( slotEditDelete() ) );

   editMenu->addSeparator();
   //eticre hide show menu
   m_pHideAction = editMenu->addAction( "hide" );
   connect( m_pHideAction, SIGNAL( triggered( bool ) ), this, SLOT( slotEditHide() ) );

   m_pShowAction = editMenu->addAction( "show" );
   connect( m_pShowAction, SIGNAL( triggered( bool ) ), this, SLOT( slotEditShow() ) );

   m_pCutAction->setEnabled( false );
   m_pCopyAction->setEnabled( false );
   m_pPasteAction->setEnabled( false );
   m_pDeleteAction->setEnabled( false );


}

void PMPart::initActions()
{
   fileMenu = m_pShell->get_fileMenu();
   viewMenu = m_pShell->get_viewMenu();
   insertMenu = m_pShell->get_insertMenu();
   editMenu = m_pShell->get_editMenu();
   menuSolidPri = new QMenu;
   menuFinitePatch = new QMenu;
   menuInfiniteSolid = new QMenu;
   menuCsg = new QMenu;
   menuMaterial = new QMenu;
   menuInterior = new QMenu;
   menuTexture = new QMenu;
   menuPhotons = new QMenu;
   menuAthmo = new QMenu;
   menuTrans = new QMenu;

   initCopyPasteActions();
   m_onlyCopyPaste = false;

   fileMenu->addSeparator();

   m_pImportAction = new QAction("Import");//m_pShell->get_fileMenu()->addAction( "Import" );
   fileMenu->insertAction( fileMenu->actions().at(5) , m_pImportAction );
   connect( m_pImportAction, SIGNAL( triggered( bool ) ), this, SLOT( slotFileImport() ) );

   m_pExportAction = new QAction("Export");
   fileMenu->insertAction( m_pShell->get_fileMenu()->actions().at(6) , m_pExportAction );
   connect( m_pExportAction, SIGNAL( triggered( bool ) ), this, SLOT( slotFileExport() ) );

   viewMenu->addSeparator();
   m_pRenderComboAction = new QComboBox;
   updateRenderModes();
   QWidgetAction* qw = new QWidgetAction(nullptr);
   menuRenderModes = new QMenu;
   qw->setDefaultWidget( m_pRenderComboAction );
   menuRenderModes->addAction(qw);
   menuRenderModes->setTitle("Render Modes");
   viewMenu->addMenu(menuRenderModes);

   viewMenu->addSeparator();
   m_pRenderAction = viewMenu->addAction( "Render" );
   m_pRenderAction->setIcon( QIcon::fromTheme( "pmrender" ) );
   connect(m_pRenderAction, SIGNAL( triggered( bool) ), this, SLOT( slotRender() ));

   m_pRenderSettingsAction = viewMenu->addAction( "view_render_settings" );
   m_pRenderSettingsAction->setText( tr( "Render Modes..." ) );
   m_pRenderSettingsAction->setIcon( QIcon(":/smallicon/icons/povicons/small/pmrendersettings") );
   connect( m_pRenderSettingsAction, SIGNAL(triggered(bool)), SLOT( slotRenderSettings() ));

   m_pViewRenderWindowAction = viewMenu->addAction( "view_render_window" );
   m_pViewRenderWindowAction->setText( tr( "Render Window" ) );
   connect( m_pViewRenderWindowAction, SIGNAL( triggered( bool ) ), this, SLOT( slotViewRenderWindow() ) );

   viewMenu->addSeparator();
   QMenu* mn = new QMenu;
   spbx = new QSpinBox;
   spbx->setMinimum(20);
   spbx->setValue(20);
   m_pVisibilityLevelAction = new QWidgetAction(nullptr);
   m_pVisibilityLevelAction->setDefaultWidget(spbx);
   mn->addAction(m_pVisibilityLevelAction);
   mn->setTitle( tr( "Visibility Level" ) );
   connect(m_pVisibilityLevelAction, SIGNAL(valueChanged(int) ), SLOT( slotVisibilityLevelChanged(int) ) );
   viewMenu->addMenu( mn );

   detail = new QActionGroup( this );
   verylow = detail->addAction( "verylow" );
   verylow->setCheckable(true);
   verylow->setText( tr( "Very Low" ) );
   verylow->setData(0);
   low = detail->addAction("Low");
   low->setCheckable(true);
   low->setData(1);
   medium = detail->addAction( "Medium" );
   medium->setCheckable(true);
   medium->setData(2);
   high = detail->addAction( "High" );
   high->setCheckable(true);
   high->setData(3);
   veryhigh = detail->addAction( "very high" );
   veryhigh->setCheckable(true);
   veryhigh->setText( tr( "Very High" ) );
   veryhigh->setData(4);
   // "Global Detail Level" menu
   menu_gdl = new QMenu;
   menu_gdl->setTitle( "Global Detail Level" );
   menu_gdl->addActions( detail->actions() );

   viewMenu->addMenu( menu_gdl );
   connect( detail, &QActionGroup::triggered, this, &PMPart::slotGlobalDetailLevelChanged );

   // new objects
   if( isReadWrite() )
   {
      m_pNewGlobalSettingsAction = actionCollection( "new_globalsettings" );
      m_pNewGlobalSettingsAction->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmglobalsettings" ) );
      m_pNewGlobalSettingsAction->setText( tr( "Global Settings" ) );
      connect(m_pNewGlobalSettingsAction, SIGNAL(triggered(bool)), SLOT(slotNewGlobalSettings()));
      m_readWriteActions.append( m_pNewGlobalSettingsAction );
      m_pNewRadiosityAction = actionCollection( "new_radiosity" );
      //m_pNewRadiosityAction->setIcon( QIcon::fromTheme("pmradiosity") );
      m_pNewRadiosityAction->setText( tr( "Radiosity" ) );
      connect(m_pNewRadiosityAction, SIGNAL(triggered(bool)), SLOT( slotNewRadiosity() ));
      m_readWriteActions.append( m_pNewRadiosityAction );
      m_pNewGlobalPhotonsAction = actionCollection( "new_globalphotons" );
      //m_pNewGlobalPhotonsAction->setIcon( QIcon::fromTheme("pmglobalphotons") );
      m_pNewGlobalPhotonsAction->setText( tr( "Global Photons" ) );
      connect(m_pNewGlobalPhotonsAction, SIGNAL(triggered(bool)), SLOT( slotNewGlobalPhotons() ));
      m_readWriteActions.append( m_pNewGlobalPhotonsAction );
      insertMenu->addSeparator();

      m_pNewDeclareAction = actionCollection( "new_declare" );
      //m_pNewDeclareAction->setIcon( QIcon::fromTheme("pmdeclare") );
      m_pNewDeclareAction->setText( tr( "Declaration" ) );
      connect(m_pNewDeclareAction, SIGNAL(triggered(bool)), SLOT( slotNewDeclare() ));
      m_pNewDeclareAction = actionCollection( "new_declare_function" );
      //m_pNewDeclareAction->setIcon( QIcon::fromTheme("pmdeclare-f") );
      m_pNewDeclareAction->setText( tr( "Declare Function" ) );
      connect(m_pNewDeclareAction, SIGNAL(triggered(bool)), SLOT( slotNewDeclareFunction() ));
      m_readWriteActions.append( m_pNewDeclareAction );
      insertMenu->addSeparator();

      m_pNewBoundedByAction = actionCollection( "new_boundedby" );
      //m_pNewBoundedByAction->setIcon( QIcon::fromTheme("pmboundedby") );
      m_pNewBoundedByAction->setText( tr( "Bounded By" ) );
      connect(m_pNewBoundedByAction, SIGNAL(triggered(bool)), SLOT( slotNewBoundedBy() ));
      m_readWriteActions.append( m_pNewBoundedByAction );
      m_pNewClippedByAction = actionCollection( "new_clippedby" );
      //m_pNewClippedByAction->setIcon( QIcon::fromTheme("pmclippedby") );
      m_pNewClippedByAction->setText( tr( "Clipped By" ) );
      connect(m_pNewClippedByAction, SIGNAL(triggered(bool)), SLOT( slotNewClippedBy() ));
      m_readWriteActions.append( m_pNewClippedByAction );

      insertMenu->addSeparator();

      m_pNewLightAction = actionCollection( "new_light" );
      //m_pNewLightAction->setIcon( QIcon::fromTheme("pmlight") );
      m_pNewLightAction->setText( tr( "Light" ) );
      connect(m_pNewLightAction, SIGNAL(triggered(bool)), SLOT( slotNewLight() ));
      m_readWriteActions.append( m_pNewLightAction );
      m_pNewLightGroupAction = actionCollection( "new_lightgroup" );
      //m_pNewLightGroupAction->setIcon( QIcon::fromTheme("pmlightgroup") );
      m_pNewLightGroupAction->setText( tr( "Light Group" ) );
      connect(m_pNewLightGroupAction, SIGNAL(triggered(bool)), SLOT( slotNewLightGroup() ));
      m_readWriteActions.append( m_pNewLightGroupAction );
      m_pNewLooksLikeAction = actionCollection( "new_lookslike" );
      //m_pNewLooksLikeAction->setIcon( QIcon::fromTheme("pmlookslike") );
      m_pNewLooksLikeAction->setText( tr( "Looks Like" ) );
      connect(m_pNewLooksLikeAction, SIGNAL(triggered(bool)), SLOT( slotNewLooksLike() ));
      m_readWriteActions.append( m_pNewLooksLikeAction );
      m_pNewProjectedThroughAction = actionCollection( "new_projectedthrough" );
      //m_pNewProjectedThroughAction->setIcon( QIcon::fromTheme("pmprojectedthrough") );
      m_pNewProjectedThroughAction->setText( tr( "Projected Through" ) );
      connect(m_pNewProjectedThroughAction, SIGNAL(triggered(bool)), SLOT( slotNewProjectedThrough() ));
      m_readWriteActions.append( m_pNewProjectedThroughAction );
      m_pNewCameraAction = actionCollection( "new_camera" );
      //m_pNewCameraAction->setIcon( QIcon::fromTheme("pmcamera") );
      m_pNewCameraAction->setText( tr( "Camera" ) );
      connect(m_pNewCameraAction, SIGNAL(triggered(bool)), SLOT( slotNewCamera() ));
      m_readWriteActions.append( m_pNewCameraAction );
      insertMenu->addSeparator();

      m_pNewCommentAction = actionCollection( "new_comment" );
      //m_pNewCommentAction->setIcon( QIcon::fromTheme("pmcomment") );
      m_pNewCommentAction->setText( tr( "Comment" ) );
      connect(m_pNewCommentAction, SIGNAL(triggered(bool)), SLOT( slotNewComment() ));
      m_readWriteActions.append( m_pNewCommentAction );
      m_pNewRawAction = actionCollection( "new_raw" );
      //m_pNewRawAction->setIcon( QIcon::fromTheme("pmraw") );
      m_pNewRawAction->setText( tr( "Raw Povray" ) );
      connect(m_pNewRawAction, SIGNAL(triggered(bool)), SLOT( slotNewRaw() ));
      m_readWriteActions.append( m_pNewRawAction );
      insertMenu->addSeparator();

      menuSolidPri->setTitle( "Finite Solid Primitives" );
      insertMenu->addMenu( menuSolidPri );

      m_pNewBoxAction = actionCollection( "new_box", menuSolidPri );
      //m_pNewBoxAction->setIcon( QIcon::fromTheme("pmbox") );
      m_pNewBoxAction->setText( tr( "Box" ) );
      connect(m_pNewBoxAction, SIGNAL(triggered(bool)), SLOT( slotNewBox() ));
      m_readWriteActions.append( m_pNewBoxAction );
      m_pNewSphereAction = actionCollection( "new_sphere", menuSolidPri );
      //m_pNewSphereAction->setIcon( QIcon::fromTheme("pmsphere") );
      m_pNewSphereAction->setText( tr( "Sphere" ) );
        connect(m_pNewSphereAction, SIGNAL(triggered(bool)), SLOT( slotNewSphere() ));
      m_readWriteActions.append( m_pNewSphereAction );
      m_pNewCylinderAction = actionCollection( "new_cylinder", menuSolidPri );
      //m_pNewCylinderAction->setIcon( QIcon::fromTheme("pmcylinder") );
      m_pNewCylinderAction->setText( tr( "Cylinder" ) );
      connect(m_pNewCylinderAction, SIGNAL(triggered(bool)), SLOT( slotNewCylinder() ));
      m_readWriteActions.append( m_pNewCylinderAction );
      m_pNewConeAction = actionCollection( "new_cone", menuSolidPri );
      //m_pNewConeAction->setIcon( QIcon::fromTheme("pmcone") );
      m_pNewConeAction->setText( tr( "Cone" ) );
      connect(m_pNewConeAction, SIGNAL( triggered( bool ) ), this, SLOT( slotNewCone() ) );
      m_readWriteActions.append( m_pNewConeAction );
      m_pNewTorusAction = actionCollection( "new_torus" , menuSolidPri);
      //m_pNewTorusAction->setIcon( QIcon::fromTheme("pmtorus") );
      m_pNewTorusAction->setText( tr( "Torus" ) );
      connect(m_pNewTorusAction, SIGNAL(triggered(bool)), SLOT( slotNewTorus() ));
      m_readWriteActions.append( m_pNewTorusAction );
      m_pNewSuperquadricEllipsoidAction = actionCollection( "new_superquadricellipsoid", menuSolidPri );
      //m_pNewSuperquadricEllipsoidAction->setIcon( QIcon::fromTheme("pmsqe") );
      m_pNewSuperquadricEllipsoidAction->setText( tr( "Superquadric Ellipsoid" ) );
      connect(m_pNewSuperquadricEllipsoidAction, SIGNAL(triggered(bool)), SLOT( slotNewSuperquadricEllipsoid() ));
      m_readWriteActions.append( m_pNewSuperquadricEllipsoidAction );
      m_pNewHeightFieldAction = actionCollection( "new_heightfield", menuSolidPri );
      //m_pNewHeightFieldAction->setIcon( QIcon::fromTheme("pmheightfield") );
      m_pNewHeightFieldAction->setText( tr( "Height Field" ) );
      connect(m_pNewHeightFieldAction, SIGNAL(triggered(bool)), SLOT( slotNewHeightField() ));
      m_readWriteActions.append( m_pNewHeightFieldAction );
      m_pNewTextAction = actionCollection( "new_text", menuSolidPri );
      //m_pNewTextAction->setIcon( QIcon::fromTheme("pmtext") );
      m_pNewTextAction->setText( tr( "Text" ) );
      connect(m_pNewTextAction, SIGNAL(triggered(bool)), SLOT( slotNewText() ));
      m_pNewJuliaFractalAction = actionCollection( "new_juliafractal", menuSolidPri );
      //m_pNewJuliaFractalAction->setIcon( QIcon::fromTheme("pmjuliafractal") );
      m_pNewJuliaFractalAction->setText( tr( "Julia Fractal" ) );
      connect(m_pNewJuliaFractalAction, SIGNAL(triggered(bool)), SLOT( slotNewJuliaFractal() ));
      m_readWriteActions.append( m_pNewJuliaFractalAction );
      m_readWriteActions.append( m_pNewTextAction );
      m_pNewIsoSurfaceAction = actionCollection( "new_isosurface", menuSolidPri );
      //m_pNewIsoSurfaceAction->setIcon( QIcon::fromTheme("pmisosurface") );
      m_pNewIsoSurfaceAction->setText( tr( "Iso Surface" ) );
      connect(m_pNewIsoSurfaceAction, SIGNAL(triggered(bool)), SLOT( slotNewIsoSurface() ));
      m_readWriteActions.append( m_pNewIsoSurfaceAction );
      menuSolidPri->addSeparator();
      m_pNewBlobAction = actionCollection( "new_blob", menuSolidPri );
      //m_pNewBlobAction->setIcon( QIcon::fromTheme("pmblob") );
      m_pNewBlobAction->setText( tr( "Blob" ) );
      connect(m_pNewBlobAction, SIGNAL(triggered(bool)), SLOT( slotNewBlob() ));
      m_readWriteActions.append( m_pNewBlobAction );
      m_pNewBlobSphereAction = actionCollection( "new_blobsphere", menuSolidPri );
      //m_pNewBlobSphereAction->setIcon( QIcon::fromTheme("pmblobsphere") );
      m_pNewBlobSphereAction->setText( tr( "Blob Sphere" ) );
      connect(m_pNewBlobSphereAction, SIGNAL(triggered(bool)), SLOT( slotNewBlobSphere() ));
      m_readWriteActions.append( m_pNewBlobSphereAction );
      m_pNewBlobCylinderAction = actionCollection( "new_blobcylinder", menuSolidPri );
      //m_pNewBlobCylinderAction->setIcon( QIcon::fromTheme("pmblobcylinder") );
      m_pNewBlobCylinderAction->setText( tr( "Blob Cylinder" ) );
      connect(m_pNewBlobCylinderAction, SIGNAL(triggered(bool)), SLOT( slotNewBlobCylinder() ));
      m_readWriteActions.append( m_pNewBlobCylinderAction );
      menuSolidPri->addSeparator();
      m_pNewLatheAction = actionCollection( "new_lathe", menuSolidPri );
      //m_pNewLatheAction->setIcon( QIcon::fromTheme("pmlathe") );
      m_pNewLatheAction->setText( tr( "Lathe" ) );
      connect(m_pNewLatheAction, SIGNAL(triggered(bool)), SLOT( slotNewLathe() ));
      m_readWriteActions.append( m_pNewLatheAction );
      m_pNewPrismAction = actionCollection( "new_prism", menuSolidPri );
      //m_pNewPrismAction->setIcon( QIcon::fromTheme("pmprism") );
      m_pNewPrismAction->setText( tr( "Prism" ) );
      connect(m_pNewPrismAction, SIGNAL(triggered(bool)), SLOT( slotNewPrism() ));
      m_readWriteActions.append( m_pNewPrismAction );
      m_pNewSurfaceOfRevolutionAction = actionCollection( "new_surfaceofrevolution", menuSolidPri );
      //m_pNewSurfaceOfRevolutionAction->setIcon( QIcon::fromTheme("pmsor") );
      m_pNewSurfaceOfRevolutionAction->setText( tr( "Surface of Revolution" ) );
      connect(m_pNewSurfaceOfRevolutionAction, SIGNAL(triggered(bool)), SLOT( slotNewSurfaceOfRevolution() ));
      m_readWriteActions.append( m_pNewSurfaceOfRevolutionAction );
      m_pNewSphereSweepAction = actionCollection( "new_spheresweep", menuSolidPri );
      //m_pNewSphereSweepAction->setIcon( QIcon::fromTheme("pmspheresweep") );
      m_pNewSphereSweepAction->setText( tr( "Sphere Sweep" ) );
      connect(m_pNewSphereSweepAction, SIGNAL(triggered(bool)), SLOT( slotNewSphereSweep() ));
      m_readWriteActions.append( m_pNewSphereSweepAction );
      menuSolidPri->addSeparator();
      m_pNewObjectLinkAction = actionCollection( "new_objectlink", menuSolidPri );
      //m_pNewObjectLinkAction->setIcon( QIcon::fromTheme("pmobjectlink") );
      m_pNewObjectLinkAction->setText( tr( "Object Link" ) );
      connect(m_pNewObjectLinkAction, SIGNAL(triggered(bool)), SLOT( slotNewObjectLink() ));
      m_readWriteActions.append( m_pNewObjectLinkAction );
      //end submenu

      menuFinitePatch->setTitle( "Finite Patch Primitives" );
      insertMenu->addMenu( menuFinitePatch );

      m_pNewDiscAction = actionCollection( "new_disc", menuFinitePatch );
      //m_pNewDiscAction->setIcon( QIcon::fromTheme("pmdisc") );
      m_pNewDiscAction->setText( tr( "Disc" ) );
      connect(m_pNewDiscAction, SIGNAL(triggered(bool)), SLOT( slotNewDisc() ));
      m_readWriteActions.append( m_pNewDiscAction );
      m_pNewBicubicPatchAction = actionCollection( "new_bicubicpatch", menuFinitePatch );
      //m_pNewBicubicPatchAction->setIcon( QIcon::fromTheme("pmbicubicpatch") );
      m_pNewBicubicPatchAction->setText( tr( "Bicubic Patch" ) );
      connect(m_pNewBicubicPatchAction, SIGNAL(triggered(bool)), SLOT( slotNewBicubicPatch() ));
      m_readWriteActions.append( m_pNewBicubicPatchAction );
      m_pNewTriangleAction = actionCollection( "new_triangle", menuFinitePatch );
      //m_pNewTriangleAction->setIcon( QIcon::fromTheme("pmtriangle") );
      m_pNewTriangleAction->setText( tr( "Triangle" ) );
      connect(m_pNewTriangleAction, SIGNAL(triggered(bool)), SLOT( slotNewTriangle() ));
      m_readWriteActions.append( m_pNewTriangleAction );
      m_pNewMeshAction = actionCollection( "new_mesh", menuFinitePatch );
      //m_pNewMeshAction->setIcon( QIcon::fromTheme("pmmesh") );
      m_pNewMeshAction->setText( tr( "Mesh" ) );
      connect(m_pNewMeshAction, SIGNAL(triggered(bool)), SLOT( slotNewMesh() ));
      m_readWriteActions.append( m_pNewMeshAction );
      //end submenu

      menuInfiniteSolid->setTitle( "Infinite Primitives" );
      insertMenu->addMenu( menuInfiniteSolid );

      m_pNewPlaneAction = actionCollection( "new_plane", menuInfiniteSolid );
      //m_pNewPlaneAction->setIcon( QIcon::fromTheme("pmplane") );
      m_pNewPlaneAction->setText( tr( "Plane" ) );
      connect(m_pNewPlaneAction, SIGNAL(triggered(bool)), SLOT( slotNewPlane() ));
      m_readWriteActions.append( m_pNewPlaneAction );

      menuCsg->setTitle( "Constructive Solid Geometry" );
      insertMenu->addMenu( menuCsg );

      m_pNewUnionAction = actionCollection( "new_union", menuCsg );
      //m_pNewUnionAction->setIcon( QIcon::fromTheme("pmunion") );
      m_pNewUnionAction->setText( tr( "Union" ) );
      connect(m_pNewUnionAction, SIGNAL(triggered(bool)), SLOT( slotNewUnion() ));
      m_readWriteActions.append( m_pNewUnionAction );
      m_pNewIntersectionAction = actionCollection( "new_intersection", menuCsg );
      //m_pNewIntersectionAction->setIcon( QIcon::fromTheme("pmintersection") );
      m_pNewIntersectionAction->setText( tr( "Intersection" ) );
      connect(m_pNewIntersectionAction, SIGNAL(triggered(bool)), SLOT( slotNewIntersection() ));
      m_readWriteActions.append( m_pNewIntersectionAction );
      m_pNewDifferenceAction = actionCollection( "new_difference", menuCsg );
      //m_pNewDifferenceAction->setIcon( QIcon::fromTheme("pmdifference") );
      m_pNewDifferenceAction->setText( tr( "Difference" ) );
      connect(m_pNewDifferenceAction, SIGNAL(triggered(bool)), SLOT( slotNewDifference() ));
      m_readWriteActions.append( m_pNewDifferenceAction );
      m_pNewMergeAction = actionCollection( "new_merge", menuCsg );
      //m_pNewMergeAction->setIcon( QIcon::fromTheme("pmmerge") );
      m_pNewMergeAction->setText( tr( "Merge" ) );
      connect(m_pNewMergeAction, SIGNAL(triggered(bool)), SLOT( slotNewMerge() ));
      m_readWriteActions.append( m_pNewMergeAction );
      //end menu
      insertMenu->addSeparator();

      menuMaterial->setTitle( "Material" );
      insertMenu->addMenu( menuMaterial );
      m_pNewMaterialAction = actionCollection( "new_material", menuMaterial );
      //m_pNewMaterialAction->setIcon( QIcon::fromTheme("pmmaterial") );
      m_pNewMaterialAction->setText( tr( "Material" ) );
      connect(m_pNewMaterialAction, SIGNAL(triggered(bool)), SLOT( slotNewMaterial() ));
      m_readWriteActions.append( m_pNewMaterialAction );

      menuInterior->setTitle( "Interior" );
      insertMenu->addMenu( menuInterior );

      m_pNewInteriorAction = actionCollection( "new_interior", menuInterior );
      //m_pNewInteriorAction->setIcon( QIcon::fromTheme("pminterior") );
      m_pNewInteriorAction->setText( tr( "Interior" ) );
      connect(m_pNewInteriorAction, SIGNAL(triggered(bool)), SLOT( slotNewInterior() ));
      m_readWriteActions.append( m_pNewInteriorAction );
      m_pNewMediaAction = actionCollection( "new_media", menuInterior );
      //m_pNewMediaAction->setIcon( QIcon::fromTheme("pmmedia") );
      m_pNewMediaAction->setText( tr( "Media" ) );
      connect(m_pNewMediaAction, SIGNAL(triggered(bool)), SLOT( slotNewMedia() ));
      m_readWriteActions.append( m_pNewMediaAction );
      m_pNewDensityAction = actionCollection( "new_density", menuInterior );
      //m_pNewDensityAction->setIcon( QIcon::fromTheme("pmdensity") );
      m_pNewDensityAction->setText( tr( "Density" ) );
      connect(m_pNewDensityAction, SIGNAL(triggered(bool)), SLOT( slotNewDensity() ));
      m_readWriteActions.append( m_pNewDensityAction );
      m_pNewDensityListAction = actionCollection( "new_densitylist", menuInterior );
      //m_pNewDensityListAction->setIcon( QIcon::fromTheme("pmdensit, menuInteriorylist") );
      m_pNewDensityListAction->setText( tr( "Density List" ) );
      connect(m_pNewDensityListAction, SIGNAL(triggered(bool)), SLOT( slotNewDensityList() ));
      m_readWriteActions.append( m_pNewDensityListAction );
      m_pNewDensityMapAction = actionCollection( "new_densitymap", menuInterior );
      //m_pNewDensityMapAction->setIcon( QIcon::fromTheme("pmdensitymap") );
      m_pNewDensityMapAction->setText( tr( "Density Map" ) );
      connect(m_pNewDensityMapAction, SIGNAL(triggered(bool)), SLOT( slotNewDensityMap() ));
      m_readWriteActions.append( m_pNewDensityMapAction );

      menuTexture->setTitle( "Texture" );
      insertMenu->addMenu( menuTexture );

      m_pNewPatternAction = actionCollection( "new_pattern", menuTexture );
      //m_pNewPatternAction->setIcon( QIcon::fromTheme("pmpattern") );
      m_pNewPatternAction->setText( tr( "Pattern" ) );
      connect(m_pNewPatternAction, SIGNAL(triggered(bool)), SLOT( slotNewPattern() ));
      m_readWriteActions.append( m_pNewPatternAction );
      m_pNewBlendMapModifiersAction = actionCollection( "new_blendmapmodifiers", menuTexture );
      //m_pNewBlendMapModifiersAction->setIcon( QIcon::fromTheme("pmblendmapmodifiers") );
      m_pNewBlendMapModifiersAction->setText( tr( "Blend Map Modifiers" ) );
      connect(m_pNewBlendMapModifiersAction, SIGNAL(triggered(bool)), SLOT( slotNewBlendMapModifiers() ));
      m_readWriteActions.append( m_pNewBlendMapModifiersAction );
      m_pNewWarpAction = actionCollection( "new_warp", menuTexture );
      //m_pNewWarpAction->setIcon( QIcon::fromTheme("pmwarp") );
      m_pNewWarpAction->setText( tr( "Warp" ) );
      connect(m_pNewWarpAction, SIGNAL(triggered(bool)), SLOT( slotNewWarp() ));
      m_readWriteActions.append( m_pNewWarpAction );

      menuTexture->addSeparator();

      m_pNewTextureAction = actionCollection( "new_texture", menuTexture );
      //m_pNewTextureAction->setIcon( QIcon::fromTheme("pmtexture") );
      m_pNewTextureAction->setText( tr( "Texture" ) );
      connect(m_pNewTextureAction, SIGNAL(triggered(bool)), SLOT( slotNewTexture() ));
      m_readWriteActions.append( m_pNewTextureAction );
      m_pNewInteriorTextureAction = actionCollection( "new_interiortexture", menuTexture );
      //m_pNewInteriorTextureAction->setIcon( QIcon::fromTheme("pminteriortexture") );
      m_pNewInteriorTextureAction->setText( tr( "Interior Texture" ) );
      connect(m_pNewInteriorTextureAction, SIGNAL(triggered(bool)), SLOT( slotNewInteriorTexture() ));
      m_readWriteActions.append( m_pNewInteriorTextureAction );
      m_pNewPigmentAction = actionCollection( "new_pigment", menuTexture );
      //m_pNewPigmentAction->setIcon( QIcon::fromTheme("pmpigment") );
      m_pNewPigmentAction->setText( tr( "Pigment" ) );
      connect(m_pNewPigmentAction, SIGNAL(triggered(bool)), SLOT( slotNewPigment() ));
      m_readWriteActions.append( m_pNewPigmentAction );
      m_pNewNormalAction = actionCollection( "new_normal", menuTexture );
      //m_pNewNormalAction->setIcon( QIcon::fromTheme("pmnormal") );
      m_pNewNormalAction->setText( tr( "Normal" ) );
      connect(m_pNewNormalAction, SIGNAL(triggered(bool)), SLOT( slotNewNormal() ));
      m_readWriteActions.append( m_pNewNormalAction );
      m_pNewSolidColorAction = actionCollection( "new_solidcolor", menuTexture );
      //m_pNewSolidColorAction->setIcon( QIcon::fromTheme("pmsolidcolor") );
      m_pNewSolidColorAction->setText( tr( "Solid Color" ) );
      connect(m_pNewSolidColorAction, SIGNAL(triggered(bool)), SLOT( slotNewSolidColor() ));
      m_readWriteActions.append( m_pNewSolidColorAction );
      m_pNewTextureListAction = actionCollection( "new_texturelist", menuTexture );
      //m_pNewTextureListAction->setIcon( QIcon::fromTheme("pmtexturelist") );
      m_pNewTextureListAction->setText( tr( "Texture List" ) );
      connect(m_pNewTextureListAction, SIGNAL(triggered(bool)), SLOT( slotNewTextureList() ));
      m_readWriteActions.append( m_pNewTextureListAction );
      m_pNewColorListAction = actionCollection( "new_colorlist", menuTexture );
      //m_pNewColorListAction->setIcon( QIcon::fromTheme("pmcolorlist") );
      m_pNewColorListAction->setText( tr( "Color List" ) );
      connect(m_pNewColorListAction, SIGNAL(triggered(bool)), SLOT( slotNewColorList() ));
      m_readWriteActions.append( m_pNewColorListAction );
      m_pNewPigmentListAction = actionCollection( "new_pigmentlist", menuTexture );
      //m_pNewPigmentListAction->setIcon( QIcon::fromTheme("pmpigmentlist") );
      m_pNewPigmentListAction->setText( tr( "Pigment List" ) );
      connect(m_pNewPigmentListAction, SIGNAL(triggered(bool)), SLOT( slotNewPigmentList() ));
      m_readWriteActions.append( m_pNewPigmentListAction );
      m_pNewNormalListAction = actionCollection( "new_normallist", menuTexture );
      //m_pNewNormalListAction->setIcon( QIcon::fromTheme("pmnormallist") );
      m_pNewNormalListAction->setText( tr( "Normal List" ) );
      connect(m_pNewNormalListAction, SIGNAL(triggered(bool)), SLOT( slotNewNormalList() ));
      m_readWriteActions.append( m_pNewNormalListAction );
      m_pNewTextureMapAction = actionCollection( "new_texturemap", menuTexture );
      //m_pNewTextureMapAction->setIcon( QIcon::fromTheme("pmtexturemap") );
      m_pNewTextureMapAction->setText( tr( "Texture Map" ) );
      connect(m_pNewTextureMapAction, SIGNAL(triggered(bool)), SLOT( slotNewTextureMap() ));
      m_readWriteActions.append( m_pNewTextureMapAction );
      m_pNewMaterialMapAction = actionCollection( "new_materialmap", menuTexture );
      //m_pNewMaterialMapAction->setIcon( QIcon::fromTheme("pmmaterialmap") );
      m_pNewMaterialMapAction->setText( tr( "Material Map" ) );
      connect(m_pNewMaterialMapAction, SIGNAL(triggered(bool)), SLOT( slotNewMaterialMap() ));
      m_readWriteActions.append( m_pNewMaterialMapAction );
      m_pNewPigmentMapAction = actionCollection( "new_pigmentmap", menuTexture );
      //m_pNewPigmentMapAction->setIcon( QIcon::fromTheme("pmpigmentmap") );
      m_pNewPigmentMapAction->setText( tr( "Pigment Map" ) );
      connect(m_pNewPigmentMapAction, SIGNAL(triggered(bool)), SLOT( slotNewPigmentMap() ));
      m_readWriteActions.append( m_pNewPigmentMapAction );
      m_pNewImageMapAction = actionCollection( "new_imagemap", menuTexture );
      //m_pNewImageMapAction->setIcon( QIcon::fromTheme("pmimagemap") );
      m_pNewImageMapAction->setText( tr( "Image Map" ) );
      connect(m_pNewImageMapAction, SIGNAL(triggered(bool)), SLOT( slotNewImageMap() ));
      m_readWriteActions.append( m_pNewImageMapAction );
      m_pNewColorMapAction = actionCollection( "new_colormap", menuTexture );
      //m_pNewColorMapAction->setIcon( QIcon::fromTheme("pmcolormap") );
      m_pNewColorMapAction->setText( tr( "Color Map" ) );
      connect(m_pNewColorMapAction, SIGNAL(triggered(bool)), SLOT( slotNewColorMap() ));
      m_readWriteActions.append( m_pNewColorMapAction );
      m_pNewNormalMapAction = actionCollection( "new_normalmap", menuTexture );
      //m_pNewNormalMapAction->setIcon( QIcon::fromTheme("pmnormalmap") );
      m_pNewNormalMapAction->setText( tr( "Normal Map" ) );
      connect(m_pNewNormalMapAction, SIGNAL(triggered(bool)), SLOT( slotNewNormalMap() ));
      m_readWriteActions.append( m_pNewNormalMapAction );
      m_pNewBumpMapAction = actionCollection( "new_bumpmap", menuTexture );
      //m_pNewBumpMapAction->setIcon( QIcon::fromTheme("pmbumpmap") );
      m_pNewBumpMapAction->setText( tr( "Bump Map" ) );
      connect(m_pNewBumpMapAction, SIGNAL(triggered(bool)), SLOT( slotNewBumpMap() ));
      m_readWriteActions.append( m_pNewBumpMapAction );
      m_pNewSlopeAction = actionCollection( "new_slope", menuTexture );
      //m_pNewSlopeAction->setIcon( QIcon::fromTheme("pmslope") );
      m_pNewSlopeAction->setText( tr( "Slope" ) );
      connect(m_pNewSlopeAction, SIGNAL(triggered(bool)), SLOT( slotNewSlope() ));
      m_readWriteActions.append( m_pNewSlopeAction );
      m_pNewSlopeMapAction = actionCollection( "new_slopemap", menuTexture );
      //m_pNewSlopeMapAction->setIcon( QIcon::fromTheme("pmslopemap") );
      m_pNewSlopeMapAction->setText( tr( "Slope Map" ) );
      connect(m_pNewSlopeMapAction, SIGNAL(triggered(bool)), SLOT( slotNewSlopeMap() ));
      m_readWriteActions.append( m_pNewSlopeMapAction );
      m_pNewFinishAction = actionCollection( "new_finish", menuTexture );
      //m_pNewFinishAction->setIcon( QIcon::fromTheme("pmfinish") );
      m_pNewFinishAction->setText( tr( "Finish" ) );
      connect(m_pNewFinishAction, SIGNAL(triggered(bool)), SLOT( slotNewFinish() ));
      m_readWriteActions.append( m_pNewFinishAction );
      menuTexture->addSeparator();
      m_pNewQuickColorAction = actionCollection( "new_quickcolor", menuTexture );
      //m_pNewQuickColorAction->setIcon( QIcon::fromTheme("pmquickcolor") );
      m_pNewQuickColorAction->setText( tr( "QuickColor" ) );
      connect(m_pNewQuickColorAction, SIGNAL(triggered(bool)), SLOT( slotNewQuickColor() ));
      m_readWriteActions.append( m_pNewQuickColorAction );
      menuPhotons->setTitle( "Photons" );
      insertMenu->addMenu( menuPhotons );

      m_pNewPhotonsAction = actionCollection( "new_photons", menuPhotons );
      //m_pNewPhotonsAction->setIcon( QIcon::fromTheme("pmphotons") );
      m_pNewPhotonsAction->setText( tr( "Photons" ) );
      connect(m_pNewPhotonsAction, SIGNAL(triggered(bool)), SLOT( slotNewPhotons() ));
      m_readWriteActions.append( m_pNewPhotonsAction );

      menuAthmo->setTitle( "Athmospheric Effects" );
      insertMenu->addMenu( menuAthmo );

      m_pNewSkySphereAction = actionCollection( "new_skysphere", menuAthmo );
      //m_pNewSkySphereAction->setIcon( QIcon::fromTheme("pmskysphere") );
      m_pNewSkySphereAction->setText( tr( "Sky Sphere" ) );
      connect(m_pNewSkySphereAction, SIGNAL(triggered(bool)), SLOT( slotNewSkySphere() ));
      m_readWriteActions.append( m_pNewSkySphereAction );
      m_pNewRainbowAction = actionCollection( "new_rainbow", menuAthmo );
      //m_pNewRainbowAction->setIcon( QIcon::fromTheme("pmrainbow") );
      m_pNewRainbowAction->setText( tr( "Rainbow" ) );
      connect(m_pNewRainbowAction, SIGNAL(triggered(bool)), SLOT( slotNewRainbow() ));
      m_readWriteActions.append( m_pNewRainbowAction );
      m_pNewFogAction = actionCollection( "new_fog", menuAthmo );
      //m_pNewFogAction->setIcon( QIcon::fromTheme("pmfog") );
      m_pNewFogAction->setText( tr( "Fog" ) );
      connect(m_pNewFogAction, SIGNAL(triggered(bool)), SLOT( slotNewFog() ));
      m_readWriteActions.append( m_pNewFogAction );
      //end menu

      insertMenu->addSeparator();
      menuTrans->setTitle( "Transformation" );
      insertMenu->addMenu( menuTrans );

      m_pNewTranslateAction = actionCollection( "new_translate", menuTrans );
      //m_pNewTranslateAction->setIcon( QIcon::fromTheme("pmtranslate") );
      m_pNewTranslateAction->setText( tr( "Translate" ) );
      connect(m_pNewTranslateAction, SIGNAL(triggered(bool)), SLOT( slotNewTranslate() ));
      m_readWriteActions.append( m_pNewTranslateAction );
      m_pNewScaleAction = actionCollection( "new_scale", menuTrans );
      //m_pNewScaleAction->setIcon( QIcon::fromTheme("pmscale") );
      m_pNewScaleAction->setText( tr( "Scale" ) );
      connect(m_pNewScaleAction, SIGNAL(triggered(bool)), SLOT( slotNewScale() ));
      m_readWriteActions.append( m_pNewScaleAction );
      m_pNewRotateAction = actionCollection( "new_rotate", menuTrans );
      //m_pNewRotateAction->setIcon( QIcon::fromTheme("pmrotate") );
      m_pNewRotateAction->setText( tr( "Rotate" ) );
      connect(m_pNewRotateAction, SIGNAL(triggered(bool)), SLOT( slotNewRotate() ));
      m_readWriteActions.append( m_pNewRotateAction );
      m_pNewMatrixAction = actionCollection( "new_povraymatrix", menuTrans );
      //m_pNewMatrixAction->setIcon( QIcon::fromTheme("pmmatrix") );
      m_pNewMatrixAction->setText( tr( "Matrix" ) );
      connect(m_pNewMatrixAction, SIGNAL(triggered(bool)), SLOT( slotNewMatrix() ));
      m_readWriteActions.append( m_pNewMatrixAction );



      // POV-Ray 3.5 objects



#ifdef KPM_WITH_OBJECT_LIBRARY
      m_pSearchLibraryObjectAction = actionCollection( "search_library_object" );
      //m_pSearchLibraryObjectAction->setIcon( QIcon::fromTheme("pmsearchlibrary") );
      m_pSearchLibraryObjectAction->setText( tr( "Search Object" ) );
		connect(m_pSearchLibraryObjectAction, SIGNAL(triggered(bool)), SLOT( slotSearchLibraryObject() ));
      m_readWriteActions.append( m_pSearchLibraryObjectAction );
#endif

      m_pUndoAction = actionCollection( "Undo", editMenu );
      m_pRedoAction = actionCollection( "Redo", editMenu );
      m_pUndoAction->setEnabled( false );
      m_pRedoAction->setEnabled( false );
      connect( m_pUndoAction, &QAction::triggered, this, &PMPart::slotEditUndo );
      connect( m_pRedoAction, &QAction::triggered, this, &PMPart::slotEditRedo );

      disableReadWriteActions(false);
   }
   else
   {
      m_pNewGlobalSettingsAction = nullptr;
      m_pNewSkySphereAction = nullptr;
      m_pNewRainbowAction = nullptr;
      m_pNewFogAction = nullptr;

      m_pNewInteriorAction = nullptr;
      m_pNewMediaAction = nullptr;
      m_pNewDensityAction = nullptr;
      m_pNewMaterialAction = nullptr;
      m_pNewBoxAction = nullptr;
      m_pNewSphereAction = nullptr;
      m_pNewCylinderAction = nullptr;
      m_pNewConeAction = nullptr;
      m_pNewTorusAction = nullptr;
      m_pNewLatheAction = nullptr;
      m_pNewPrismAction = nullptr;
      m_pNewSurfaceOfRevolutionAction = nullptr;
      m_pNewSuperquadricEllipsoidAction = nullptr;
      m_pNewJuliaFractalAction = nullptr;
      m_pNewHeightFieldAction = nullptr;
      m_pNewTextAction = nullptr;

      m_pNewBlobAction = nullptr;
      m_pNewBlobSphereAction = nullptr;
      m_pNewBlobCylinderAction = nullptr;

      m_pNewPlaneAction = nullptr;
      m_pNewPolynomAction = nullptr;

      m_pNewDeclareAction = nullptr;
      m_pNewObjectLinkAction = nullptr;

      m_pNewDiscAction = nullptr;
      m_pNewBicubicPatchAction = nullptr;
      m_pNewTriangleAction = nullptr;

      m_pNewUnionAction = nullptr;
      m_pNewDifferenceAction = nullptr;
      m_pNewIntersectionAction = nullptr;
      m_pNewMergeAction = nullptr;

      m_pNewBoundedByAction = nullptr;
      m_pNewClippedByAction = nullptr;

      m_pNewLightAction = nullptr;
      m_pNewLooksLikeAction = nullptr;
      m_pNewProjectedThroughAction = nullptr;

      m_pNewCameraAction = nullptr;

      m_pNewTextureAction = nullptr;
      m_pNewPigmentAction = nullptr;
      m_pNewNormalAction = nullptr;
      m_pNewSolidColorAction = nullptr;
      m_pNewFinishAction = nullptr;
      m_pNewTextureListAction = nullptr;
      m_pNewColorListAction = nullptr;
      m_pNewPigmentListAction = nullptr;
      m_pNewNormalListAction = nullptr;
      m_pNewDensityListAction = nullptr;

      m_pNewPatternAction = nullptr;
      m_pNewBlendMapModifiersAction = nullptr;
      m_pNewTextureMapAction = nullptr;
      m_pNewMaterialMapAction = nullptr;
      m_pNewPigmentMapAction = nullptr;
      m_pNewColorMapAction = nullptr;
      m_pNewNormalMapAction = nullptr;
      m_pNewBumpMapAction = nullptr;
      m_pNewSlopeMapAction = nullptr;
      m_pNewDensityMapAction = nullptr;

      m_pNewWarpAction = nullptr;
      m_pNewImageMapAction = nullptr;
      m_pNewSlopeAction = nullptr;

      m_pNewTranslateAction = nullptr;
      m_pNewScaleAction = nullptr;
      m_pNewRotateAction = nullptr;
      m_pNewMatrixAction = nullptr;
      m_pNewCommentAction = nullptr;
      m_pNewRawAction = nullptr;

      m_pNewIsoSurfaceAction = nullptr;
      m_pNewRadiosityAction = nullptr;
      m_pNewGlobalPhotonsAction = nullptr;
      m_pNewPhotonsAction = nullptr;
      m_pNewLightGroupAction = nullptr;
      m_pNewInteriorTextureAction = nullptr;
      m_pNewSphereSweepAction = nullptr;
      m_pNewMeshAction = nullptr;

      // POV-Ray

      m_pUndoAction = nullptr;
      m_pRedoAction = nullptr;
   }
}

void PMPart::updateNewObjectActions()
{
   if( isReadWrite() && !m_onlyCopyPaste )
   {
      const QList<PMMetaObject*>& prototypes = m_pPrototypeManager->prototypes();
      QList<PMMetaObject*>::const_iterator it;
      QAction* action;
      bool enable;
      bool readWriteParent = false;
      if( m_pActiveObject )
         if( m_pActiveObject->parent() )
            if( !m_pActiveObject->parent()->isReadOnly() )
               readWriteParent = true;

      for( it = prototypes.begin(); it != prototypes.end(); ++it )
      {
         // get the action object for that type of PMObject
         // action names are "new_" + povray name
         // (like new_box, new_sphere ...)

         QString actionName = "new_" + (*it)->className().toLower();
         action = m_hash_readWriteActions[actionName];
         //action = actionCollection()->action( actionName );
         if( action )
         {
            if( m_pActiveObject )
            {
               QString insertName = (*it)->className();
               enable = m_pActiveObject->canInsert( insertName, nullptr );
               if( !enable )
                  if( m_pActiveObject->lastChild() )
                     enable = m_pActiveObject->canInsert( insertName, m_pActiveObject->lastChild() );
               if( !enable )
                  if( readWriteParent )
                     enable |= m_pActiveObject->parent()->canInsert( insertName, m_pActiveObject );
            }
            else
               enable = false;
            action->setEnabled( enable );
         }
      }
      // special treatment for csg actions
      if( m_pActiveObject )
      {
         enable = m_pActiveObject->canInsert( QString( "CSG" ), nullptr );
         if( !enable )
            if( m_pActiveObject->lastChild() )
               enable = m_pActiveObject->canInsert( QString( "CSG" ), m_pActiveObject->lastChild() );
         if( !enable )
            if( readWriteParent )
               enable = m_pActiveObject->parent()->canInsert( QString( "CSG" ), m_pActiveObject );
      }
      else
         enable = false;

      m_pNewUnionAction->setEnabled( enable );
      m_pNewIntersectionAction->setEnabled( enable );
      m_pNewDifferenceAction->setEnabled( enable );
      m_pNewMergeAction->setEnabled( enable );
   }
   m_updateNewObjectActions = false;
}

void PMPart::disableReadWriteActions( bool b = false )
{
   foreach ( QAction* act, m_readWriteActions )
   {
       act->setDisabled(b);
   }
}

void PMPart::initDocument()
{
   newDocument();
}

void PMPart::initView( QWidget* parent )
{
   if( !m_pShell )
   {
      // a part inside konqueror
      // simple layout
      m_pView = new PMView( this, parent );
      m_pView->show();
      setWidget( m_pView );
   }
   else
   {
      // inside a PMShell
      // the shell will create the view
   }
}

void PMPart::saveConfig()
{
   if( m_pView )
      m_pView->saveConfig();
   PMErrorDialog::saveConfig();
   PMRenderModesDialog::saveConfig();
   PMRenderModeDialog::saveConfig();
   PMPovrayOutputWidget::saveConfig();
   PMRenderManager::theManager()->saveConfig();
   PMGLView::saveConfig();
   PMDialogEditBase::saveConfig();
   PMControlPoint::saveConfig();
   PMPovrayRenderWidget::saveConfig();
   PMSettingsDialog::saveConfig();
   PMDocumentationMap::theMap()->saveConfig();
#ifdef KPM_WITH_OBJECT_LIBRARY
   PMLibraryHandleEdit::saveConfig();
   PMLibraryManager::theManager()->saveConfig();
#endif

   QSettings qset;
   qset.beginGroup( "Rendering" );
   qset.setValue( "SphereUSteps", PMSphere::uSteps() );
   qset.setValue( "SphereVSteps", PMSphere::vSteps() );
   qset.setValue( "CylinderSteps", PMCylinder::steps() );
   qset.setValue( "ConeSteps", PMCone::steps() );
   qset.setValue( "DiscSteps", PMDisc::steps() );
   qset.setValue( "BlobSphereUSteps", PMBlobSphere::uSteps() );
   qset.setValue( "BlobSphereVSteps", PMBlobSphere::vSteps() );
   qset.setValue( "BlobCylinderUSteps", PMBlobCylinder::uSteps() );
   qset.setValue( "BlobCylinderVSteps", PMBlobCylinder::vSteps() );
   qset.setValue( "TorusUSteps", PMTorus::uSteps() );
   qset.setValue( "TorusVSteps", PMTorus::vSteps() );
   qset.setValue( "LatheSSteps", PMLathe::sSteps() );
   qset.setValue( "LatheRSteps", PMLathe::rSteps() );
   qset.setValue( "SorSSteps", PMSurfaceOfRevolution::sSteps() );
   qset.setValue( "SorRSteps", PMSurfaceOfRevolution::rSteps() );
   qset.setValue( "PrismSSteps", PMPrism::sSteps() );
   qset.setValue( "PlaneSize", PMPlane::planeSize() );
   qset.setValue( "SqeUSteps", PMSuperquadricEllipsoid::uSteps() );
   qset.setValue( "SqeVSteps", PMSuperquadricEllipsoid::vSteps() );
   qset.setValue( "SphereSweepRSteps", PMSphereSweep::rSteps() );
   qset.setValue( "SphereSweepSSteps", PMSphereSweep::sSteps() );
   qset.setValue( "HeightFieldVariance", PMHeightField::variance() );
   qset.setValue( "GlobalDetailLevel", PMDetailObject::globalDetailLevel() );
   qset.setValue( "DirectRendering", PMGLView::isDirectRenderingEnabled() );
}

void PMPart::restoreConfig()
{
   if( m_pView )
      m_pView->restoreConfig();
   PMErrorDialog::restoreConfig();
   PMRenderModesDialog::restoreConfig();
   PMRenderModeDialog::restoreConfig();
   PMPovrayOutputWidget::restoreConfig();
   PMRenderManager::theManager()->restoreConfig();
   PMGLView::restoreConfig();
   PMDialogEditBase::restoreConfig();
   PMControlPoint::restoreConfig();
   PMPovrayRenderWidget::restoreConfig();
   PMSettingsDialog::restoreConfig();
   PMDocumentationMap::theMap()->restoreConfig();
#ifdef KPM_WITH_OBJECT_LIBRARY
   PMLibraryHandleEdit::restoreConfig();
   PMLibraryManager::theManager()->restoreConfig();
#endif

   QSettings qset;
   qset.beginGroup( "Rendering" );
   PMSphere::setUSteps( qset.value( "SphereUSteps" ).value<int>() );
   PMSphere::setVSteps( qset.value( "SphereVSteps" ).value<int>() );
   PMCylinder::setSteps( qset.value( "CylinderSteps" ).value<int>() );
   PMCone::setSteps( qset.value( "ConeSteps" ).value<int>() );
   PMTorus::setUSteps( qset.value( "TorusUSteps" ).value<int>() );
   PMTorus::setVSteps( qset.value( "TorusVSteps" ).value<int>() );
   PMLathe::setSSteps( qset.value( "LatheSSteps" ).value<int>() );
   PMLathe::setRSteps( qset.value( "LatheRSteps" ).value<int>() );
   PMSurfaceOfRevolution::setSSteps( qset.value( "SorSSteps" ).value<int>() );
   PMSurfaceOfRevolution::setRSteps( qset.value( "SorRSteps" ).value<int>() );
   PMPrism::setSSteps( qset.value( "PrismSSteps" ).value<int>() );
   PMPlane::setPlaneSize( qset.value( "PlaneSize" ).value<double>() );
   PMDisc::setSteps( qset.value( "DiscSteps" ).value<int>() );
   PMBlobSphere::setUSteps( qset.value( "BlobSphereUSteps" ).value<int>() );
   PMBlobSphere::setVSteps( qset.value( "BlobSphereVSteps" ).value<int>() );
   PMBlobCylinder::setUSteps( qset.value( "BlobCylinderUSteps" ).value<int>() );
   PMBlobCylinder::setVSteps( qset.value( "BlobCylinderVSteps" ).value<int>() );
   PMSuperquadricEllipsoid::setUSteps( qset.value( "SqeUSteps" ).value<int>() );
   PMSuperquadricEllipsoid::setVSteps( qset.value( "SqeVSteps" ).value<int>() );
   PMSphereSweep::setRSteps( qset.value( "SphereSweepRSteps" ).value<int>() );
   PMSphereSweep::setSSteps( qset.value( "SphereSweepSSteps" ).value<int>() );
   PMHeightField::setVariance( qset.value( "HeightFieldVariance" ).value<int>() );
   PMDetailObject::setGlobalDetailLevel( qset.value( "GlobalDetailLevel" ).value<int>() );
   detail->actions().at( PMDetailObject::globalDetailLevel() )->setChecked( true );


}

bool PMPart::openFileQt( QUrl url )
{
   this->setUrl( url );
   QIODevice* dev = new QFile( url.path() );
   bool success = true;
   PMObjectList list;

   deleteContents();

   setModified( false );

   if( dev && dev->open( QIODevice::ReadOnly ) )
   {
      PMXMLParser parser( this, dev );
      parser.parse( &list, nullptr, nullptr );//step1

      if( parser.errors() || parser.warnings() )
      {
         PMErrorDialog dlg( parser.messages(), parser.errorFlags() );
         // still try to insert the correct parsed objects?
         success = ( dlg.exec() == QDialog::Accepted );
      }

      if( success )
      {
         PMObject* obj = list.first();
         if( obj )
         {
            if( obj->type() == "Scene" )
               m_pScene = static_cast<PMScene*>(obj);
            else
               success = false;
         }
         else
            success = false;
      }
   }
   else
      success = false;

   if( !success )
   {
      setUrl( QUrl() );
      newDocument();
   }

   m_pScene->setReadOnly( !isReadWrite() );
   if( !isReadWrite() )
      disableReadWriteActions( true );
   m_bCameraListUpToDate = false;

   emit refresh();
   updateRenderModes();
   updateVisibilityLevel();
   slotObjectChanged( m_pScene, PMCNewSelection, this );

   if( dev )
      delete dev;

   return success;
}

/*
bool PMPart::saveFile()
{
   bool success = false;

   QIODevice* dev = new QFile( localFilePath() );
   if( dev && dev->open( QIODevice::WriteOnly ) )
   {
      QDomDocument doc( "POVMODELER" );
      QDomElement e = static_cast<PMObject*>(m_pScene)->serialize( doc );
      doc.appendChild( e );

      QTextStream str( dev );
      str << doc;
      dev->close();
      setModified( false );
      success = true;
   }

   if( dev )
      delete dev;

   return success;
}*/


bool PMPart::saveFileQt( QUrl ur )
{
   bool success = false;

   QIODevice* dev = new QFile( ur.path() );
   if( dev && dev->open( QIODevice::WriteOnly ) )
   {
      QDomDocument doc( "POVMODELER" );
      QDomElement e = static_cast<PMObject*>(m_pScene)->serialize( doc );
      doc.appendChild( e );

      QTextStream str( dev );
      str << doc;
      dev->close();
      setModified( false );
      success = true;
   }

   if( dev )
      delete dev;

   return success;
}

bool PMPart::exportPovray(const QUrl &url )
{
   QFile* file = nullptr;
   bool ok = true;

   if( !url.isValid() )
      return false;

   if( url.isLocalFile() )
   {
      // Local file
      file = new QFile( url.path() );
      if( !file->open( QIODevice::WriteOnly ) )
         ok = false;
   }
   else
   {
      // Remote file
      // provide a temp file
      QTemporaryFile *tempFile = new QTemporaryFile();
      if( !tempFile->open() )
         ok = false;
      file = tempFile;
   }

   if( ok )
   {
      PMPovray35Format format;
      PMSerializer* dev = format.newSerializer( file );
      dev->serialize( m_pScene );
      delete dev;

      if( dynamic_cast<QTemporaryFile*>(file) )
      {
         //ok = KIO::NetAccess::upload( file->fileName(), url, (QWidget*) 0 );//KIO::file_copy()
        //ok = file->copy(url);//eticre todo
      }
   }

   delete file;

   return ok;
}

QString PMPart::activeObjectName()
{
   QString result = "";
   PMObject* tmpObj;
   PMObject* testSib;
   int idx = 0;

   tmpObj = activeObject();
   while( tmpObj != m_pScene && tmpObj )
   {
      // count previous siblings of the same type (for array like entries)
      testSib = tmpObj;
      while( ( testSib = testSib->prevSibling() ) )
         if( testSib->type() == tmpObj->type() )
            idx++;

      // prepend to result
      if( idx != 0 )
         result = tmpObj->type() + '[' + QString::number( idx ) + "]/" + result;
      else
         result = tmpObj->type() + '/' + result;

      // go up in the scene
      tmpObj = tmpObj->parent();
      idx = 0;
   }

   // Make the object name an absolute name
   result = '/' + result;

   return result;
}

bool PMPart::setActiveObject( const QString& name )
{
   PMObject* tmpObj;
   PMObject* tmpSibling;
   int pos, siblingIndex, objIndex, firstBracket, lastBracket;
   QString objPath;
   QString pathElem;

   // check if it's a absolute object name
   if( name[0] == '/' )
   {
      tmpObj = m_pScene;
      objPath = name.mid( 1 );  // clear that first character
   }
   else
      tmpObj = activeObject();

   // get the first element
   pos = objPath.indexOf( '/' );
   if( pos != -1 )
   {
      pathElem = objPath.mid( 0, pos );
      objPath = objPath.mid( pos + 1 );
   }
   else
   {
      pathElem = objPath;
      objPath.clear();
   }

   while( !pathElem.isNull() )
   {
      if( !pathElem.isEmpty() )
      {
         // Special treatment for brackets
         firstBracket = pathElem.indexOf( '[' );
         if( firstBracket != -1 )
         {
            lastBracket = pathElem.lastIndexOf( ']' );
            objIndex = pathElem.mid( firstBracket + 1, lastBracket - firstBracket - 1).toInt();
            pathElem = pathElem.left( firstBracket );
         }
         else
            objIndex = 0;

         // Iterate the children for this element. We stop when there are no more siblings
         // or the object is of the correct type and it's index count is also correct
         siblingIndex = 0;
         tmpSibling = tmpObj->firstChild();
         for( ; tmpSibling && ( tmpSibling->type() != pathElem || siblingIndex != objIndex );
                                                                tmpSibling = tmpSibling->nextSibling() )
         {
            // Found an object of the type we are looking for
            if( tmpSibling->type() == pathElem )
               siblingIndex++;
         }
         if( tmpSibling )
            tmpObj = tmpSibling;
         else
            // No correct sibling
            return false;

      }

      // Get the next element
      pos = objPath.indexOf( '/' );
      if( pos != -1 )
      {
         pathElem = objPath.mid( 0, pos );
         objPath = objPath.mid( pos + 1 );
      }
      else
      {
         pathElem = objPath;
         objPath.clear();
      }
   }
   if( tmpObj )
   {
      slotObjectChanged( tmpObj, PMCNewSelection, this );
      return true;
   }
   else
      return false;
}

QStringList PMPart::getProperties()
{
   PMObject* curObj = activeObject();

   // Ensure that there is an active object
   if( !curObj )
      return QStringList();

   return curObj->properties();
}

bool PMPart::setProperty( const QString& name, const PMVariant& value )
{
   PMObject* curObj = activeObject();

   // Ensure that there is an active object
   if( !curObj )
      return false;

   curObj->setProperty( name, value );
   slotObjectChanged( curObj, PMCNewSelection, this );
   return true;
}

bool PMPart::setProperty( const QString& name, const QString& value )
{
   PMObject* curObj = activeObject();
   PMVariant variant;

   // Ensure that there is an active object
   if( !curObj )
      return false;

   variant.fromString( curObj->property( name ).dataType(), value );
   curObj->setProperty( name, variant );
   slotObjectChanged( curObj, PMCNewSelection, this );
   return true;
}

const PMVariant PMPart::getProperty( const QString& name )
{
   PMObject* curObj = activeObject();

   // Ensure that there is an active object
   if( !curObj )
      return PMVariant();

   return curObj->property( name );
}

const QString PMPart::getPropertyStr( const QString& name )
{
   PMObject* curObj = activeObject();

   // Ensure that there is an active object
   if( !curObj )
      return PMVariant().asString();

   return curObj->property( name ).asString();
}

const PMObjectList& PMPart::selectedObjects()
{
   int numObjects = m_selectedObjects.count();
   int numOrdered = 0;
   bool stop = false;

   PMObject* tmp;
   QStack<PMObject*> stack;

   if( !m_sortedListUpToDate )
   {
      m_sortedSelectedObjects.clear();

      if( numObjects == 1 )
         m_sortedSelectedObjects.append( m_selectedObjects.first() );
      else if( numObjects > 1 )
      {
         tmp = m_pScene;
         do
         {
            if( !tmp )
            {
               if( !stack.isEmpty() )
               {
                  tmp = stack.pop();
                  if( tmp == m_pScene )
                     stop = true;
                  else
                     tmp = tmp->nextSibling();
               }
               else
                  stop = true;
            }
            else if( tmp->isSelected() )
            {
               m_sortedSelectedObjects.append( tmp );
               numOrdered++;
               tmp = tmp->nextSibling();
            }
            else if( tmp->selectedChildren() > 0 )
            {
               stack.push( tmp );
               tmp = tmp->firstChild();
            }
            else
            {
               tmp = tmp->nextSibling();
            }
         }
         while( !stop && ( numOrdered < numObjects ) );
      }
      m_sortedListUpToDate = true;
   }

   return m_sortedSelectedObjects;
}

int PMPart::whereToInsert( PMObject* obj, const PMObjectList& list )
{
   // if you change this function, change
   // whereToInsert( PMObject* obj, const QStringList& ), too

   int canInsertAsFirstChild = 0;
   int canInsertAsLastChild = 0;
   int canInsertAsSibling = 0;

   int insertAs = 0;
   int insertPossibilities = 0;

   if( !obj->isReadOnly() )
   {
      canInsertAsFirstChild = obj->canInsert( list, nullptr );//eticre declare function
      if( obj->lastChild() )
         canInsertAsLastChild = obj->canInsert( list, obj->lastChild() );

      if( canInsertAsFirstChild > 0 )
      {
         // some objects can be inserted as child
         insertAs |= PMInsertPopup::PMIFirstChild;
         insertPossibilities++;
      }
      if( canInsertAsLastChild > 0 )
      {
         insertAs |= PMInsertPopup::PMILastChild;
         insertPossibilities++;
      }
   }

   if( obj->parent() )
   {
      PMObject* p = obj->parent();
      if( !p->isReadOnly() )
      {
         canInsertAsSibling = p->canInsert( list, obj );
         if( canInsertAsSibling > 0 )
         {
            // some objects can be inserted as siblings
            insertAs |= PMInsertPopup::PMISibling;
            insertPossibilities++;
         }
      }
   }

   if( insertPossibilities > 1 )
   {
      int count = list.count();//eticre
      // more than one possibilities, ask user
      insertAs = PMInsertPopup::choosePlace(
         widget(), count > 1, insertAs,
         canInsertAsFirstChild == count,
         canInsertAsLastChild == count,
         canInsertAsSibling == count );
   }
   else if( insertPossibilities == 0 )
      insertAs = PMInsertPopup::PMIFirstChild;
   return insertAs;
}

int PMPart::whereToInsert( PMObject* obj, const QStringList& list )
{
   // if you change this function, change
   // whereToInsert( PMObject* obj, const PMObjectList ), too

   int canInsertAsFirstChild = 0;
   int canInsertAsLastChild = 0;
   int canInsertAsSibling = 0;

   int insertAs = 0;
   int insertPossibilities = 0;

   if( !obj->isReadOnly() )
   {
      canInsertAsFirstChild = obj->canInsert( list, nullptr );
      if( obj->lastChild() )
         canInsertAsLastChild = obj->canInsert( list, obj->lastChild() );

      if( canInsertAsFirstChild > 0 )
      {
         // some objects can be inserted as child
         insertAs |= PMInsertPopup::PMIFirstChild;
         insertPossibilities++;
      }
      if( canInsertAsLastChild > 0 )
      {
         insertAs |= PMInsertPopup::PMILastChild;
         insertPossibilities++;
      }
   }

   if( obj->parent() )
   {
      PMObject* p = obj->parent();
      if( !p->isReadOnly() )
      {
         canInsertAsSibling = p->canInsert( list, obj );
         if( canInsertAsSibling > 0 )
         {
            // some objects can be inserted as siblings
            insertAs |= PMInsertPopup::PMISibling;
            insertPossibilities++;
         }
      }
   }

   if( insertPossibilities > 1 )
   {
      int count = list.count();//eticre
      // more than one possibilities, ask user
      insertAs = PMInsertPopup::choosePlace(
         widget(), count > 1, insertAs,
         canInsertAsFirstChild == count,
         canInsertAsLastChild == count,
         canInsertAsSibling == count );
   }
   else if( insertPossibilities == 0 )
      insertAs = PMInsertPopup::PMIFirstChild;
   return insertAs;
}

int PMPart::whereToInsert( PMObject* obj )
{
   int insertAs = 0;
   int insertPossibilities = 0;

   if( obj->parent() )
   {
      insertAs |= PMInsertPopup::PMISibling;
      insertPossibilities++;
   }
   if( obj->isA( "CompositeObject" ) )
   {
      insertAs |= PMInsertPopup::PMIFirstChild;
      insertPossibilities++;
      if( obj->firstChild() )
      {
         insertAs |= PMInsertPopup::PMILastChild;
         insertPossibilities++;
      }
   }
   if( insertAs && ( insertPossibilities > 1 ) )
      insertAs = PMInsertPopup::choosePlace( widget(), true, insertAs );

   return insertAs;
}

void PMPart::slotFileImport()
{
   QString fileName;
   PMIOFormat* selectedFormat = nullptr;

   fileName = PMFileDialog::getImportFileName( nullptr, this, selectedFormat );

   if( !fileName.isEmpty() && selectedFormat )
   {
      QFile file( fileName );
      if( file.open( QIODevice::ReadOnly ) )
      {
         PMParser* newParser = selectedFormat->newParser( this, &file );
         if( newParser )
         {
            if( m_pActiveObject )
               insertFromParser( ( "Import %1" + selectedFormat->description() ),
                                 newParser, m_pActiveObject );
            else
               insertFromParser( ( "Import %1" + selectedFormat->description() ),
                                 newParser, m_pScene );
            delete newParser;
         }
      }
      else
      {
         QMessageBox::warning( nullptr, "Couldn't open the selected file\n",
                                    "Permission denied!");
      }
   }
}

void PMPart::slotFileExport()
{
	emit aboutToSave();

   QString fileName, filter;
   PMIOFormat* selectedFormat = nullptr;
   PMPovray35Format format;


   fileName = PMFileDialog::getExportFileName( nullptr, this, selectedFormat, filter );

   if( !fileName.isEmpty() )//&& selectedFormat )
   {
      QByteArray baData;
      QBuffer buffer( &baData );
      buffer.open( QIODevice::WriteOnly );

      PMSerializer* newSer = format.newSerializer( &buffer );
      if( newSer )
      {
         newSer->serialize( m_pScene );
         newSer->close();
         bool success = !( newSer->warnings() || newSer->errors() );

         if( !success )
         {
            // there were errors, display them
            PMErrorDialog dlg( newSer->messages(), newSer->errorFlags() );
            // still try to export?
            success = ( dlg.exec() == QDialog::Accepted );
         }
         if( success )
         {
            QFileInfo info( fileName );
            if( info.completeSuffix().isEmpty() )
               fileName += filter.right( filter.length() - 1 ); // remove '*'

            QFile file( fileName );
            if( file.open( QIODevice::WriteOnly ) )
            {
               file.write( baData );
               file.close();
            }
            else
            {
               QMessageBox::warning( nullptr, "Couldn't export to the selected file\n",
                                          "Permission denied!" );
            }
         }

         delete newSer;
      }
   }
}

void PMPart::slotEditCut()
{
   //emit setStatusBarText( tr( "Cutting selection..." ) );

   const PMObjectList& sortedList = selectedObjects();

   if( sortedList.count() > 0 )
   {
      QApplication::clipboard()->setMimeData( new PMObjectDrag( this, sortedList ) );
      removeSelection( tr( "Cut" ) );
   }

   //emit setStatusBarText( "" );
}

void PMPart::slotEditDelete()
{
   //emit setStatusBarText( tr( "Deleting selection..." ) );

   removeSelection( tr( "Delete" ) );

   //emit setStatusBarText( "" );
}

void PMPart::slotEditHide()
{
   //emit setStatusBarText( tr( "Hide selection..." ) );
   if( !m_selectedObjects.isEmpty() )
        foreach ( PMObject* o, m_selectedObjects )
        {
           o->setTreeVisibility( true );
           o->getTreeItem()->setHidden( true );
        }
   //emit setStatusBarText( "" );
   emit refresh();
}

void PMPart::slotEditShow()
{
    //emit setStatusBarText( tr( "Show selection..." ) );
    if( !m_selectedObjects.isEmpty() )
         foreach (PMObject* o, m_selectedObjects)
         {
            o->setTreeVisibility( false );
            o->getTreeItem()->setHidden( false );
         }
    //emit setStatusBarText( "" );
    emit refresh();
}


void PMPart::slotEditCopy()
{
   //emit setStatusBarText( tr( "Copying selection to clipboard..." ) );
   const PMObjectList& sortedList = selectedObjects();

   if( sortedList.count() > 0 )
      QApplication::clipboard()->setMimeData( new PMObjectDrag( this, sortedList ), QClipboard::Clipboard );

   //emit setStatusBarText( "" );
}

bool PMPart::dragMoveSelectionTo( PMObject* obj )
{
   if( obj == nullptr )
   {
      return removeSelection( tr( "Drag" ) );
   }
   else
   {
      const PMObjectList& sortedList = selectedObjects();
      PMMoveCommand* command = nullptr;
      int insertAs = whereToInsert( obj, sortedList );

      if( insertAs > 0 )
      {
         PMObject* hlp;
         bool stop;

         switch( insertAs )
         {
            case PMInsertPopup::PMIFirstChild:
               command = new PMMoveCommand( sortedList, obj, nullptr );
               break;
            case PMInsertPopup::PMILastChild:
               hlp = obj->lastChild();
               stop = false;

               while( hlp && !stop )
               {
                  if( hlp->isSelected() )
                     hlp = hlp->prevSibling();
                  else
                     stop = true;
               }
               command = new PMMoveCommand( sortedList, obj, hlp );
               break;
            case PMInsertPopup::PMISibling:
               command = new PMMoveCommand( sortedList, obj->parent(), obj );
               break;
         }
      }
      if( command )
      {
         command->setText( tr( "Drag" ) );
         return executeCommand( command );
      }
   }
   return false;
}

bool PMPart::removeSelection( const QString& type )
{
   PMDeleteCommand* cmd = nullptr;
   const PMObjectList& sortedList = selectedObjects();

   if( sortedList.count() > 0 )
   {
      cmd = new PMDeleteCommand( sortedList );
      cmd->setText( type );
      return executeCommand( cmd );
   }
   return false;
}

bool PMPart::drop( PMObject* obj, const QMimeData* mime )
{
   return pasteOrDrop( tr( "Drop" ), mime, obj );
}

void PMPart::slotEditPaste()
{
   //emit setStatusBarText( tr( "Inserting clipboard contents..." ) );

   pasteOrDrop( tr( "Paste" ), qApp->clipboard()->mimeData(),
                m_pActiveObject );

   //emit setStatusBarText( "" );
}

bool PMPart::pasteOrDrop( const QString& type, const QMimeData* mime, PMObject* obj )
{
   bool success = false;

   if( mime && obj)
   {
      PMParser* parser = PMObjectDrag::newParser( mime, this );

      if( parser )
         success = insertFromParser( type, parser, obj );
   }
   return success;
}

bool PMPart::insertFromParser( const QString& type, PMParser* parser,
                               PMObject* obj )
{
   PMObjectList list;
   bool success = true;
   int insertAs = 0;
   PMAddCommand* command = nullptr;

   // try to parse
   if( parser->canQuickParse() )
   {
      QStringList types;
      parser->quickParse( types );

      success = !( parser->warnings() || parser->errors() );

      if( !success )
      {
         // there were errors, display them
         PMErrorDialog dlg( parser->messages(), parser->errorFlags() );
         // still try to insert the correct parsed objects?
         success = ( dlg.exec() == QDialog::Accepted );
      }
      if( success && ( types.count() > 0 ) )
         insertAs = whereToInsert( obj, types );
   }
   else
      insertAs = whereToInsert( obj );

   if( success && insertAs )
   {
      PMObject* parent = nullptr;
      PMObject* after = nullptr;

      switch( insertAs )
      {
         case PMInsertPopup::PMIFirstChild:
            parent = obj;
            after = nullptr;
            break;
         case PMInsertPopup::PMILastChild:
            parent = obj;
            after = obj->lastChild();
            break;
         case PMInsertPopup::PMISibling:
            parent = obj->parent();
            after = obj;
            break;
         default:
            parent = obj;
            after = nullptr;
            break;
      }

      parser->parse( &list, parent, after );

      success = !( parser->warnings() || parser->errors() );

      if( !success )
      {
         // there were errors, display them
         PMErrorDialog dlg( parser->messages(), parser->errorFlags() );
         // still try to insert the correct parsed objects?
         success = ( dlg.exec() == QDialog::Accepted );
      }

      if( list.count() > 0 )
      {
         if( success )
         {
            // parsing was successful
            command = new PMAddCommand( list, parent, after );

            command->setText( type );
            success = executeCommand( command );
         }
         else
         {
            // parsed objects will not be inserted
            // remove all links
            PMDeclare* decl = nullptr;

            foreach( PMObject* obj, list )
            {
               PMRecursiveObjectIterator rit( obj );
               for( ; rit.current(); ++rit )
               {
                  decl = rit.current()->linkedObject();
                  if( decl )
                     decl->removeLinkedObject( rit.current() );
               }
            }
         }
      }
   }
   if( !command )
   {
      // delete all parsed objects
		foreach( PMObject* obj, list ) delete obj;
      list.clear();
   }

   return success && insertAs;
}

void PMPart::slotEditUndo()
{
   //emit setStatusBarText( tr( "Undo last change..." ) );
   m_pNewSelection = nullptr;
   m_updateNewObjectActions = false;

   m_commandManager.undo();

   if( m_pNewSelection )
      slotObjectChanged( m_pNewSelection, PMCNewSelection, this );
   if( !ismodified )
      setModified( true );
   if( m_updateNewObjectActions )
      updateNewObjectActions();

   //emit setStatusBarText( "" );
}

void PMPart::slotEditRedo()
{
   //emit setStatusBarText( tr( "Redo last change..." ) );
   m_pNewSelection = nullptr;
   m_updateNewObjectActions = false;

   m_commandManager.redo();
   if( m_pNewSelection )
      slotObjectChanged( m_pNewSelection, PMCNewSelection, this );
   if( !ismodified )
      setModified( true );
   if( m_updateNewObjectActions )
      updateNewObjectActions();

   //emit setStatusBarText( "" );
}

bool PMPart::executeCommand( PMCommand* cmd )
{
   m_pNewSelection = nullptr;
   m_numAddedObjects = 0;
   m_numInsertErrors = 0;
   m_insertErrorDetails.clear();
   m_updateNewObjectActions = false;

   if( isReadWrite() && cmd )
   {
      bool execute = true;
      int flags = cmd->errorFlags( this );

      if( flags )
      {
         PMErrorDialog dlg( cmd->messages(), flags );
         execute = ( dlg.exec() == QDialog::Accepted );
      }

      if( execute )
      {
         m_commandManager.execute( cmd );
         if( m_pNewSelection )
            slotObjectChanged( m_pNewSelection, PMCNewSelection, this );
         if( !ismodified )
            setModified( true );

         if( m_numInsertErrors > 0 )
         {
            m_insertErrorDetails.sort();
            PMInsertErrorDialog dlg( m_numAddedObjects, m_numInsertErrors,
                                     m_insertErrorDetails );
            dlg.exec();
         }
         if( m_updateNewObjectActions )
            updateNewObjectActions();

         return true;
      }
   }

   delete cmd;
   return false;
}

void PMPart::slotObjectChanged( PMObject* obj, const int m,
                                QObject* sender )
{
   int mode = m;
   bool selectionChanged = false;
   bool changeControlPoints = false;
   PMObject* oldActive = m_pActiveObject;

   if( mode & PMCNewSelection )
   {
      if( !obj )
      {
         clearSelection();
         selectionChanged = true;
         m_pActiveObject = nullptr;
      }
      else
      {
         clearSelection();
         obj->setSelected( true );
         m_selectedObjects.append( obj );
         selectionChanged = true;
         m_pActiveObject = obj;
      }
   }
   else if( ( mode & PMCSelected ) && !obj->isSelected() )
   {
      if( obj->isSelectable() )
      {
         if( obj->selectedChildren() > 0 )
         {
            QStack<PMObject*> stack;
            PMObject* tmp = obj->firstChild();
            bool stop = false;

            do
            {
               if( !tmp )
               {
                  if( !stack.isEmpty() )
                  {
                     tmp = stack.pop();
                     if( tmp == obj )
                        stop = true;
                     else
                        tmp = tmp->nextSibling();
                  }
                  else
                     stop = true;
               }
               else if( tmp->isSelected() )
               {
                  tmp->setSelected( false );
                  m_selectedObjects.removeAll( tmp );
                  emit objectChanged( tmp, PMCDeselected, this );
                  tmp = tmp->nextSibling();
               }
               else if( tmp->selectedChildren() > 0 )
               {
                  stack.push( tmp );
                  tmp = tmp->firstChild();
               }
               else
               {
                  tmp = tmp->nextSibling();
               }
            }
            while( !stop );
         }

         obj->setSelected( true );
         m_selectedObjects.append( obj );
         selectionChanged = true;
         m_sortedListUpToDate = false;
         m_sortedSelectedObjects.clear();
         m_pActiveObject = nullptr;
      }
      else
      {
         qCritical() << "(PMPart::slotObjectChanged) object is not selectable!" << "\n";
         mode = mode & ( ~( PMCSelected | PMCNewSelection ) );
      }
   }
   else if( mode & PMCDeselected )
   {
      // no problems here
      m_selectedObjects.removeAll( obj );
      obj->setSelected( false );
      m_sortedListUpToDate = false;
      m_sortedSelectedObjects.clear();
      selectionChanged = true;
      m_pActiveObject = nullptr;
   }

   if( mode & PMCRemove )
   {
      if( obj->parent() )
         if( obj->parent() == m_pActiveObject )
            m_updateNewObjectActions = true;
      if( m_pNewSelection == obj )
      {
         if( obj->nextSibling() )
            m_pNewSelection = obj->nextSibling();
         else if( obj->prevSibling() )
            m_pNewSelection = obj->nextSibling();
         else if( obj->parent() )
            m_pNewSelection = obj->parent();
         else
            m_pNewSelection = nullptr;
      }
      if( m_selectedObjects.contains( obj ) )
      {
         m_selectedObjects.removeAll( obj );
         if( m_selectedObjects.isEmpty() )
         {
            if( obj->nextSibling() )
               m_pNewSelection = obj->nextSibling();
            else if( obj->prevSibling() )
               m_pNewSelection = obj->prevSibling();
            else if( obj->parent() )
               m_pNewSelection = obj->parent();
            else
               m_pNewSelection = nullptr;
         }
         m_sortedListUpToDate = false;
         m_sortedSelectedObjects.clear();
         selectionChanged = true;
      }
      if( m_pActiveObject == obj )
         m_pActiveObject = nullptr;

      if( obj->isA( "Declare" ) )
      {
         PMDeclare* decl = static_cast<PMDeclare*>(obj);
         m_pSymbolTable->remove( decl->id() );
      }

      if( obj->type() == "Camera" ) {
          m_cameras.removeAll( static_cast<PMCamera*>(obj) );
      }
   }

   if( mode & PMCAdd )
   {
      if( !( mode & PMCInsertError ) )
      {
         m_pNewSelection = obj;
         if( obj->isA( "Declare" ) )
         {
            PMDeclare* decl = static_cast<PMDeclare*>(obj);
            PMSymbolTable::const_iterator sit = m_pSymbolTable->constFind( decl->id() );
            if( sit == m_pSymbolTable->constEnd() )
               m_pSymbolTable->insert( decl->id(), new PMSymbol( decl->id(), decl ) );
         }
         if( obj->type() == "Camera" )
            m_bCameraListUpToDate = false;
      }
      if( obj->parent() )
         if( obj->parent() == m_pActiveObject )
            m_updateNewObjectActions = true;
      m_numAddedObjects++;
   }

   if( mode & PMCData )
   {
      m_updateNewObjectActions = true;
   }

   if( mode & PMCViewStructure )
   {
      changeControlPoints = true;
   }

   if( mode & PMCInsertError )
   {
      m_numInsertErrors++;
      QString detail;
      detail = obj->description() + QString( " " ) + obj->name();
      m_insertErrorDetails.append( detail );

      if( obj->isA( "Declare" ) )
      {
         PMDeclare* decl = static_cast<PMDeclare*>(obj);
         m_pSymbolTable->remove( decl->id() );
      }
   }

   if( selectionChanged )
   {
      m_sortedListUpToDate = false;
      m_sortedSelectedObjects.clear();

      int c = m_selectedObjects.count();

      if( m_pScene->isSelected() )
         c = m_pScene->countChildren();

      m_pCopyAction->setEnabled( c > 0 );

      if( isReadWrite() )
      {
         m_pCutAction->setEnabled( c > 0 );
         m_pDeleteAction->setEnabled( c > 0 );
         m_pPasteAction->setEnabled( m_pActiveObject && m_canDecode );
         updateNewObjectActions();
      }
   }

   if( ( oldActive != m_pActiveObject ) || changeControlPoints )
   {
      updateControlPoints( oldActive );
      emit objectChanged( m_pActiveObject, PMCNewControlPoints, this );
      mode |= ( PMCNewControlPoints | PMCControlPointSelection );
   }

   emit objectChanged( obj, mode, sender );
}

void PMPart::slotIDChanged( PMObject* obj, const QString& oldID )
{
   if( obj->isA( "Declare" ) )
   {
      PMDeclare* d = static_cast<PMDeclare*>(obj);
		PMSymbolTable::const_iterator sit = m_pSymbolTable->constFind( oldID );
      if( sit != m_pSymbolTable->constEnd() )
      {
         if( (*sit)->type() == PMSymbol::Object )
         {
            if( (*sit)->object() == obj )
            {
               m_pSymbolTable->take( oldID );
               (*sit)->setId( d->id() );
               m_pSymbolTable->insert( (*sit)->id(), *sit );
            }
            else
               qCritical() << "PMPart::slotIDChanged: Symbol "
                                 << oldID << " points to wrong object.\n";
         }
         else
            qCritical() << "PMPart::slotIDChanged: Symbol "
                              << oldID << " has wrong type.\n";
      }
      else
         qCritical() << "PMPart::slotIDChanged: Symbol "
                           << oldID << " not found.\n";
   }
}

void PMPart::slotNewObject( PMObject* newObject, int insertAs )
{
   PMObjectList list;
   list.append( newObject );
   PMCommand* command = nullptr;

   if( m_pActiveObject )
   {
      // If no position was specified ask the user
      if( insertAs <= 0 )
         insertAs = whereToInsert( m_pActiveObject, list );
      // If either through a parameter or by user action a position was selected
      if( insertAs > 0 )
      {
         // insert the object in the position indicated
         switch( insertAs )
         {
            case PMInsertPopup::PMIFirstChild:
               command = new PMAddCommand( list, m_pActiveObject, nullptr );
               break;
            case PMInsertPopup::PMILastChild:
               command = new PMAddCommand( list, m_pActiveObject,
                                           m_pActiveObject->lastChild() );
               break;
            case PMInsertPopup::PMISibling:
               command = new PMAddCommand( list,
                                           m_pActiveObject->parent(),
                                           m_pActiveObject );
               break;
            default:
               command = new PMAddCommand( list, m_pActiveObject, nullptr );
               break;
         }
         executeCommand( command );
      }
      else
      {
         list.clear();
         delete newObject;
      }
   }
   else
   {
      list.clear();
      delete newObject;
   }
}

void PMPart::slotNewObject( const QString& type )
{
   PMObject* newObject = m_pPrototypeManager->newObject( type );
   if( newObject )
      slotNewObject( newObject );
}

void PMPart::slotNewObject( const QString& type, const QString& pos )
{
   PMObject* newObject = m_pPrototypeManager->newObject( type );
   if( newObject )
   {
      if( pos == "FirstChild" )
         slotNewObject( newObject, PMInsertPopup::PMIFirstChild );
      else if( pos == "LastChild" )
         slotNewObject( newObject, PMInsertPopup::PMILastChild );
      else if( pos == "Sibling" )
         slotNewObject( newObject, PMInsertPopup::PMISibling );
      else
         slotNewObject( newObject );
   }
}

QStringList PMPart::getObjectTypes()
{
   QStringList result;
	const QList<PMMetaObject*>& prototypes = m_pPrototypeManager->prototypes();
	QList<PMMetaObject*>::const_iterator it;
	for( it = prototypes.begin(); it != prototypes.end(); ++it )
   {
      result.append( (*it)->className() );
   }
   return result;
}

void PMPart::slotNewTransformedObject( PMObject* o )
{
   if( o )
   {
      if( o->canInsert( QString( "Scale" ), o->lastChild() ) )
         o->appendChild( new PMScale( this ) );
      if( o->canInsert( QString( "Rotate" ), o->lastChild() ) )
         o->appendChild( new PMRotate( this ) );
      if( o->canInsert( QString( "Translate" ), o->lastChild() ) )
         o->appendChild( new PMTranslate( this ) );
      slotNewObject( o );
   }
}

void PMPart::slotNewGlobalSettings()
{
   slotNewObject( new PMGlobalSettings( this ) );
}

void PMPart::slotNewSkySphere()
{
   slotNewObject( new PMSkySphere( this ) );
}

void PMPart::slotNewRainbow()
{
   slotNewObject( new PMRainbow( this ) );
}

void PMPart::slotNewFog()
{
   slotNewObject( new PMFog( this ) );
}

void PMPart::slotNewInterior()
{
   slotNewObject( new PMInterior( this ) );
}

void PMPart::slotNewMedia()
{
   slotNewObject( new PMMedia( this ) );
}

void PMPart::slotNewDensity()
{
   slotNewObject( new PMDensity( this ) );
}

void PMPart::slotNewMaterial()
{
   slotNewObject( new PMMaterial( this ) );
}

void PMPart::slotNewBox()
{
   slotNewTransformedObject( new PMBox( this ) );
}

void PMPart::slotNewSphere()
{
   slotNewTransformedObject( new PMSphere( this ) );
}

void PMPart::slotNewCylinder()
{
   slotNewTransformedObject( new PMCylinder( this ) );
}

void PMPart::slotNewPlane()
{
   slotNewTransformedObject( new PMPlane( this ) );
}

void PMPart::slotNewPolynom()
{
   slotNewTransformedObject( new PMPolynom( this ) );
}

void PMPart::slotNewCone()
{
   slotNewTransformedObject( new PMCone( this ) );
}

void PMPart::slotNewTorus()
{
   slotNewTransformedObject( new PMTorus( this ) );
}

void PMPart::slotNewLathe()
{
   slotNewTransformedObject( new PMLathe( this ) );
}

void PMPart::slotNewPrism()
{
   slotNewTransformedObject( new PMPrism( this ) );
}

void PMPart::slotNewSurfaceOfRevolution()
{
   slotNewTransformedObject( new PMSurfaceOfRevolution( this ) );
}

void PMPart::slotNewSuperquadricEllipsoid()
{
   slotNewTransformedObject( new PMSuperquadricEllipsoid( this ) );
}

void PMPart::slotNewJuliaFractal()
{
   slotNewTransformedObject( new PMJuliaFractal( this ) );
}

void PMPart::slotNewHeightField()
{
   slotNewTransformedObject( new PMHeightField( this ) );
}

void PMPart::slotNewText()
{
   slotNewTransformedObject( new PMText( this ) );
}

void PMPart::slotNewBlob()
{
   slotNewTransformedObject( new PMBlob( this ) );
}

void PMPart::slotNewBlobSphere()
{
   slotNewObject( new PMBlobSphere( this ) );
}

void PMPart::slotNewBlobCylinder()
{
   slotNewObject( new PMBlobCylinder( this ) );
}

void PMPart::slotNewDeclare()
{
   PMDeclare* obj = new PMDeclare( this );
   m_pSymbolTable->findNewID( "Declare", obj );
   slotNewObject( obj );
}

void PMPart::slotNewDeclareFunction()
{
   PMDeclare* obj = new PMDeclare( this );
   m_pSymbolTable->findNewID( "Function", obj );
   obj->is_a_function = 1;
   slotNewObject( obj );
}

void PMPart::slotNewObjectLink()
{
   slotNewTransformedObject( new PMObjectLink( this ) );
}

void PMPart::slotNewUnion()
{
   slotNewObject( new PMCSG( this, PMCSG::CSGUnion ) );
}

void PMPart::slotNewDifference()
{
   slotNewObject( new PMCSG( this, PMCSG::CSGDifference ) );
}

void PMPart::slotNewIntersection()
{
   slotNewObject( new PMCSG( this, PMCSG::CSGIntersection ) );
}

void PMPart::slotNewMerge()
{
   slotNewObject( new PMCSG( this, PMCSG::CSGMerge ) );
}

void PMPart::slotNewBoundedBy()
{
   slotNewObject( new PMBoundedBy( this ) );
}

void PMPart::slotNewClippedBy()
{
   slotNewObject( new PMClippedBy( this ) );
}

void PMPart::slotNewLight()
{
   slotNewObject( new PMLight( this ) );
}

void PMPart::slotNewLooksLike()
{
   slotNewObject( new PMLooksLike( this ) );
}

void PMPart::slotNewProjectedThrough()
{
   slotNewObject( new PMProjectedThrough( this ) );
}

void PMPart::slotNewDisc()
{
   slotNewTransformedObject( new PMDisc( this ) );
}

void PMPart::slotNewBicubicPatch()
{
   slotNewTransformedObject( new PMBicubicPatch( this ) );
}

void PMPart::slotNewTriangle()
{
   slotNewObject( new PMTriangle( this ) );
}

void PMPart::slotNewCamera()
{
   PMCamera* c = new PMCamera( this );
   c->setAngle( 45.0 );
   c->setLocation( PMVector( 5.0, 5.0, -5.0 ) );
   c->setLookAt( PMVector( 0.0, 0.0, 0.0 ) );

   slotNewObject( c );
}

void PMPart::slotNewTexture()
{
   slotNewObject( new PMTexture( this ) );
}

void PMPart::slotNewPigment()
{
   slotNewObject( new PMPigment( this ) );
}

void PMPart::slotNewNormal()
{
   slotNewObject( new PMNormal( this ) );
}

void PMPart::slotNewSolidColor()
{
   slotNewObject( new PMSolidColor( this ) );
}

void PMPart::slotNewTextureList()
{
   slotNewObject( new PMTextureList( this ) );
}

void PMPart::slotNewColorList()
{
   slotNewObject( new PMColorList( this ) );
}

void PMPart::slotNewPigmentList()
{
   slotNewObject( new PMPigmentList( this ) );
}

void PMPart::slotNewNormalList()
{
   slotNewObject( new PMNormalList( this ) );
}

void PMPart::slotNewDensityList()
{
   slotNewObject( new PMDensityList( this ) );
}

void PMPart::slotNewFinish()
{
   slotNewObject( new PMFinish( this ) );
}

void PMPart::slotNewWarp()
{
   slotNewObject( new PMWarp( this ) );
}

void PMPart::slotNewImageMap()
{
   slotNewObject( new PMImageMap( this ) );
}

void PMPart::slotNewPattern()
{
   slotNewObject( new PMPattern( this ) );
}

void PMPart::slotNewBlendMapModifiers()
{
   slotNewObject( new PMBlendMapModifiers( this ) );
}

void PMPart::slotNewTextureMap()
{
   slotNewObject( new PMTextureMap( this ) );
}

void PMPart::slotNewMaterialMap()
{
   slotNewObject( new PMMaterialMap( this ) );
}

void PMPart::slotNewColorMap()
{
   slotNewObject( new PMColorMap( this ) );
}

void PMPart::slotNewPigmentMap()
{
   slotNewObject( new PMPigmentMap( this ) );
}

void PMPart::slotNewNormalMap()
{
   slotNewObject( new PMNormalMap( this ) );
}

void PMPart::slotNewBumpMap()
{
   slotNewObject( new PMBumpMap( this ) );
}

void PMPart::slotNewSlopeMap()
{
   slotNewObject( new PMSlopeMap( this ) );
}

void PMPart::slotNewDensityMap()
{
   slotNewObject( new PMDensityMap( this ) );
}

void PMPart::slotNewSlope()
{
   slotNewObject( new PMSlope( this ) );
}

void PMPart::slotNewQuickColor()
{
   slotNewObject( new PMQuickColor( this ) );
}

void PMPart::slotNewTranslate()
{
   slotNewObject( new PMTranslate( this ) );
}

void PMPart::slotNewScale()
{
   slotNewObject( new PMScale( this ) );
}

void PMPart::slotNewRotate()
{
   slotNewObject( new PMRotate( this ) );
}

void PMPart::slotNewMatrix()
{
   slotNewObject( new PMPovrayMatrix( this ) );
}

void PMPart::slotNewComment()
{
   slotNewObject( new PMComment( this ) );
}

void PMPart::slotNewRaw()
{
   slotNewObject( new PMRaw( this ) );
}

// POV-Ray 3.5 objects

void PMPart::slotNewIsoSurface()
{
   slotNewObject( new PMIsoSurface( this ) );
}

void PMPart::slotNewRadiosity()
{
   slotNewObject( new PMRadiosity( this ) );
}

void PMPart::slotNewGlobalPhotons()
{
   slotNewObject( new PMGlobalPhotons( this ) );
}

void PMPart::slotNewPhotons()
{
   slotNewObject( new PMPhotons( this ) );
}

void PMPart::slotNewLightGroup()
{
   slotNewObject( new PMLightGroup( this ) );
}

void PMPart::slotNewInteriorTexture()
{
   slotNewObject( new PMInteriorTexture( this ) );
}

void PMPart::slotNewSphereSweep()
{
   slotNewObject( new PMSphereSweep( this ) );
}

void PMPart::slotNewMesh()
{
   slotNewObject( new PMMesh( this ) );
}

void PMPart::slotSearchLibraryObject()
{
#ifdef KPM_WITH_OBJECT_LIBRARY
   PMLibraryObjectSearch* aux = new PMLibraryObjectSearch( NULL );
   aux->show();
#endif
}

void PMPart::slotClipboardDataChanged()
{
   if( isReadWrite() )
   {
      m_canDecode = PMObjectDrag::canDecode( qApp->clipboard()->mimeData(), this );
      m_pPasteAction->setEnabled( m_canDecode && m_pActiveObject );
   }
   else
      m_pPasteAction->setEnabled( false );
}

void PMPart::clearSelection()
{
	PMObjectList::iterator it = m_selectedObjects.begin();

	if( it != m_selectedObjects.end() )
   {
      if( ( *it )->nextSibling() )
         m_pNewSelection = ( *it )->nextSibling();
      else if( ( *it )->prevSibling() )
         m_pNewSelection = ( *it )->prevSibling();
      else if( ( *it )->parent() )
         m_pNewSelection = ( *it )->parent();

		for( ; it != m_selectedObjects.end(); ++it )
      {
         ( *it )->setSelected( false );
         if( m_pNewSelection == ( *it ) )
         {
            if( ( *it )->nextSibling() )
               m_pNewSelection = ( *it )->nextSibling();
            else if( ( *it )->prevSibling() )
               m_pNewSelection = ( *it )->prevSibling();
            else if( ( *it )->parent() )
               m_pNewSelection = ( *it )->parent();
         }
      }
   }

   m_selectedObjects.clear();
   m_sortedListUpToDate = true;
}

bool PMPart::newDocument()
{
   deleteContents();
   setModified( false );

   m_pScene = new PMScene( this );
   
   PMRaw* raw = new PMRaw ( this );
   raw->setCode( "#version 3.7;" );
   raw->setName( "VERSION" );
   m_pScene->appendChild( raw );

   PMGlobalSettings* gs = new PMGlobalSettings( this );
   gs->setAssumedGamma( 1 );
   m_pScene->appendChild( gs );

   PMBox* b = new PMBox( this );
   m_pScene->appendChild( b );
   PMPigment* p = new PMPigment( this );
   b->appendChild( p );
   PMSolidColor* c = new PMSolidColor( this );
   c->setColor( PMColor( 0.3, 1.0, 0.3 ) );
   p->appendChild( c );
   PMScale* s = new PMScale( this );
   b->appendChild( s );
   PMRotate* r = new PMRotate( this );
   b->appendChild( r );
   PMTranslate* t = new PMTranslate( this );
   t->setTranslation( PMVector( 0, 0.5, 0 ) );
   b->appendChild( t );

   PMLight* l = new PMLight( this );
   l->setLocation( PMVector( 4.0, 5.0, -5.0 ) );
   m_pScene->appendChild( l );
   PMCamera* ca = new PMCamera( this );
   ca->setAngle( 45.0 );
   ca->setLocation( PMVector( 5.0, 5.0, -5.0 ) );
   ca->setLookAt( PMVector( 0.0, 0.0, 0.0 ) );
   m_pScene->appendChild( ca );
   m_bCameraListUpToDate = false;

   m_pScene->setReadOnly( !isReadWrite() );
	PMRenderMode mode;
   mode.setDescription( tr( "Default" ) );
   PMRenderModeList modes;
   modes.append( mode );
   m_pScene->setRenderModes( modes );
   updateRenderModes();

   emit refresh();
   //updateRenderModes();
   //updateVisibilityLevel();
   slotObjectChanged( m_pScene, PMCNewSelection, this );

   return true;
}

void PMPart::closeDocument()
{
   setUrl( QUrl() );
}

void PMPart::deleteContents()
{
   emit clear();
   if( isReadWrite() )
      m_commandManager.clear();
   m_selectedObjects.clear();
   m_sortedSelectedObjects.clear();
   m_sortedListUpToDate = true;
   m_pActiveObject = nullptr;
   m_pNewSelection = nullptr;

   if( m_pScene )
   {
      delete m_pScene;
      m_pScene = nullptr;
   }
   if( m_pSymbolTable )
      delete m_pSymbolTable;

   m_pSymbolTable = new PMSymbolTable();
   m_cameras.clear();
   m_bCameraListUpToDate = true;
}

void PMPart::slotUpdateUndoRedo( const QString& undo, const QString& redo )
{
   if( isReadWrite() )
   {
      if( m_pUndoAction )
      {
         if( undo.isNull() )
         {
            m_pUndoAction->setText( tr( "Undo" ) );
            m_pUndoAction->setEnabled( false );
         }
         else
         {
            m_pUndoAction->setText( "Undo the specified action " +  undo );
            m_pUndoAction->setEnabled( true );
         }
      }
      if( m_pRedoAction )
      {
         if( redo.isNull() )
         {
            m_pRedoAction->setText( "Redo" );
            m_pRedoAction->setEnabled( false );
         }
         else
         {
            m_pRedoAction->setText( "Redo the specified action" + redo );
            m_pRedoAction->setEnabled( true );
         }
      }
   }
}

/**eticre refresh visual tree**/
void PMPart::refreshView()
{
   emit refreshList();
}

void PMPart::setScene( PMScene* scene )
{
   deleteContents();
   m_pScene = scene;
   emit refresh();
   slotObjectChanged( m_pScene, PMCNewSelection, this );
}

void PMPart::setModified()
{
   //setModified();
   ismodified = true;
   emit modified();
}

void PMPart::setModified( bool m )
{
   //setModified( m );
   ismodified = m;
   emit modified();
}

PMCamera* PMPart::firstCamera()
{
   if( !m_bCameraListUpToDate )
      updateCameraList();
   return m_cameras.isEmpty() ? nullptr : m_cameras.first();
}

const QList< PMCamera* > PMPart::cameras()
{
   if( !m_bCameraListUpToDate )
      updateCameraList();
   return m_cameras;
}

void PMPart::updateCameraList()
{
   m_cameras.clear();
   PMObject* obj;
   for( obj = m_pScene->firstChild(); obj; obj = obj->nextSibling() )
      if( obj->type() == "Camera" )
         m_cameras.append( static_cast<PMCamera*>(obj) );
   m_bCameraListUpToDate = true;
}

void PMPart::slotRender()
{
   const PMRenderMode* m = m_pScene->activeRenderMode();
   if( m )
   {
      emit aboutToRender();

      if( m_pScene->firstChild()->name() != "VERSION" )
      {
         int c = m_pScene->countChildren();
         int i;
         for(  i = 0 ; i < c ; ++i )
         {
             if( m_pScene->childAt( i )->name() == "VERSION" )
             {
                PMObject* version = m_pScene->childAt( i )->copy();
                slotObjectChanged( m_pScene->childAt( i ), PMCNewSelection, this );
                removeSelection("Delete");
                slotObjectChanged( m_pScene, PMCNewSelection, this );
                slotNewObject( version , 1 );
                break;
             }
         }
      }
      QByteArray a;
      QBuffer buffer( &a );
      buffer.open( QIODevice::WriteOnly );
      PMPovray35Format format;
      PMSerializer* dev = format.newSerializer( &buffer );
      dev->serialize( m_pScene );
      delete dev;

      if( m_pPovrayWidget )
         m_pPovrayWidget->close();

       m_pPovrayWidget = new PMPovrayWidget();
      if( m_pPovrayWidget->render( a, *m, url() ) )
      {
         m_pPovrayWidget->show();
         m_pPovrayWidget->resize( 640, 480 );
         m_pPovrayWidget->raise();
      }
   }
}

void PMPart::slotRenderSettings()
{
   PMRenderModesDialog dlg( m_pScene->renderModes(), m_pScene->activeRenderModeIndex(), widget() );
   int result = dlg.exec();

   if( result == QDialog::Accepted )
   {
		m_pScene->setRenderModes( dlg.renderModes() );
		m_pScene->setActiveRenderMode( dlg.activeRenderMode() );
      if( isReadWrite() )
         setModified( true );
      updateRenderModes();
   }
}

void PMPart::slotViewRenderWindow()
{
   if( !m_pPovrayWidget )
      m_pPovrayWidget = new PMPovrayWidget();
   m_pPovrayWidget->show();
   m_pPovrayWidget->raise();
}

void PMPart::slotRenderMode( int index )
{
	m_pScene->setActiveRenderMode( index );
   emit activeRenderModeChanged();
}

void PMPart::updateRenderModes()
{
   if( m_pScene )
   {
      PMRenderModeList list = m_pScene->renderModes();
		PMRenderModeList::iterator it;
		QStringList entries;
		for( it = list.begin(); it != list.end(); ++it )
			entries.append( (*it).description() );
        m_pRenderComboAction->clear();
        m_pRenderComboAction->addItems( entries );
        m_pRenderComboAction->setCurrentIndex( m_pScene->activeRenderModeIndex() );
      emit activeRenderModeChanged();
   }
}

void PMPart::slotVisibilityLevelChanged( int l )
{
   if( m_pScene->visibilityLevel() != l )
   {
      m_pScene->setVisibilityLevel( l );
      if( isReadWrite() )
         setModified( true );
      emit objectChanged( m_pScene, PMCViewStructure, this );
   }
}

void PMPart::updateVisibilityLevel()
{
   if( m_pVisibilityLevelAction && m_pScene )
       spbx->setValue( m_pScene->visibilityLevel() );
}

void PMPart::slotGlobalDetailLevelChanged( QAction* act )
{
   PMDetailObject::setGlobalDetailLevel( act->data().toInt() );
   emit objectChanged( m_pScene, PMCViewStructure, this );
}

void PMPart::updateControlPoints( PMObject* oldActive )
{
   PMControlPointList newCPs;

   if( m_pActiveObject )
   {
      m_pActiveObject->controlPoints( newCPs );

      if( m_pActiveObject == oldActive )
      {
         // check if the control points are the same
         bool same = true;
			PMControlPointList::iterator oit = m_controlPoints.begin();
			PMControlPointList::iterator nit = newCPs.begin();
         while( same && oit != m_controlPoints.end() && nit != newCPs.end() )
         {
            if( ( *oit )->id() != ( *nit )->id() )
               same = false;
            ++oit;
            ++nit;
         }
         if( oit != m_controlPoints.end() || nit != newCPs.end() )
            same = false;
         if( same )
         {
            // set the old selection
				for(oit = m_controlPoints.begin(), nit = newCPs.begin(); oit != m_controlPoints.end() && nit != newCPs.end(); ++oit, ++nit)
               ( *nit )->setSelected( ( *oit )->selected() );
         }
      }
   }

	foreach(PMControlPoint* p, m_controlPoints) delete p;
   m_controlPoints.clear();
   m_controlPoints = newCPs;

}

void PMPart::slotAboutToSave()
{
	emit aboutToSave();
}

#include "pmpart.moc"
