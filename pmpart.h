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


#ifndef PMPART_H
#define PMPART_H

#include "pmobject.h"
#include "pmcommandmanager.h"
#include <QUrl>
#include <QStringList>

class PMView;
class PMShell;
class PMScene;
class PMCamera;
class PMPovrayWidget;
class PMParser;

class PMSymbolTable;
class PMDeclare;

class PMPrototypeManager;
class PMInsertRuleSystem;
class PMIOManager;

class QMimeData;
class QAction;
class QActionGroup;
class QComboBox;
class QWidget;
class QWidgetAction;
class QSpinBox;

/**
 * The part for povmodeler (povray document)
 */
class PMPart : public QObject
{
   Q_OBJECT
public:
   /**
    * construtor of PMPart, calls all init functions to create the
    * application. QActionGroup* detail
    */
   PMPart( QWidget* parentWidget,
           QObject* parent, bool readWrite,
           PMShell* shell = nullptr );

   /**
    * construtor of PMPart, calls all init functions to create the
    * application. It does not create the main widget.
    */
   PMPart( QWidget* parentWidget,
           QObject* parent, bool readWrite,
           bool onlyCutPaste, PMShell* shell = nullptr );

   /**
    * Destructor
    */
   ~PMPart();
   /**
    * deletes the document's contents
    */
   void deleteContents();

   bool saveFileQt( QUrl ur );

   QUrl url(){ return m_pUrl; }

   void setReadWrite( bool rw ){ m_readWrite = rw; }
   bool isReadWrite(){ return m_readWrite; }
   QObject* parent(){ return m_pparent; }

   void setUrl( QUrl url ){ m_pUrl = url; }
   bool closeUrl(){ return true; }

   QMenu* getEditMenu(){ return editMenu;  }

   QMenu* getMenu( QString name );

   void setWidget( QWidget* qw ){ m_pwidget = qw; }
   QWidget* widget(){ return m_pwidget; }
   //void setModified(){ m_bModified = true; }
   //void setModified( bool b ){ m_bModified = b; }
   /**
    * initializes the document generally
    */
   bool newDocument();
   /**
    * closes the actual document
    */
   void closeDocument();
   /**
    * Exports the scene as povray code
    */
   bool exportPovray( const QUrl& url );

   /**
    * returns the toplevel object
    */
   PMScene* scene() const { return m_pScene; }
   /**
    * Returns a pointer to the shell if there is one
    */
   PMShell* shell() const { return m_pShell; }
   /**
    * saves settings
    */
   void saveConfig();
   /**
    * loads settings
    */
   void restoreConfig();

   /**
    * Updates the sorted list of selected objects if necessary and
    * returns a reference to the list.
    */
   const PMObjectList& selectedObjects();

   /**
    * Executes the command cmd.
    *
    * All changes to the document have to be made
    * with @ref PMCommand objects for undo/redo. Each slot that changes the
    * document creates a PMCommand object and calls this function.
    *
    * If this function returns false, the command was not executed
    * and the command object was deleted.
    */
   bool executeCommand( PMCommand* cmd );
   /**
    * Checks, where the objects in list can be inserted, as child
    * or as sibling of obj and asks the user if there is more than one
    * possibility
    *
    * Returns PMIFirstChild, PMILastChild, PMISibling or 0, if the objects
    * should not be inserted.*/
   int whereToInsert( PMObject* obj, const PMObjectList& list );
   /**
    * Checks, where the objects in list can be inserted, as child
    * or as sibling of obj and asks the user if there is more than one
    * possibility
    *
    * Returns PMIFirstChild, PMILastChild, PMISibling or 0, if the objects
    * should not be inserted.*/
   int whereToInsert( PMObject* obj, const QStringList& list );
   /**
    * Asks the user, where to insert new objects.
    *
    * Returns PMIFirstChild, PMILastChild, PMISibling or 0, if the objects
    * should not be inserted.*/
   int whereToInsert( PMObject* obj );

   /**
    * Returns the symbol table of this document
    */
   PMSymbolTable* symbolTable() const { return m_pSymbolTable; }
   /**
    * Returns the first camera
    */
   PMCamera* firstCamera();
   /**
    * Returns an iterator to the list of cameras
    */
   const QList< PMCamera* > cameras();
   /**
    * The active object
    */
   PMObject* activeObject() const { return m_pActiveObject; }
   /**
    * The active object's name
    */
   virtual QString activeObjectName();
   /**
    * Set the active object
    */
   virtual bool setActiveObject( const QString& name );
   /**
    * Get the valid properties of the currently active object
    */
   virtual QStringList getProperties();
   /**
    * set a property on the active object
    */
   virtual bool setProperty( const QString& name, const PMVariant& value );
   /**
    * set a property on the active object
    */
   virtual bool setProperty( const QString& name, const QString& value );
   /**
    * get a property on the active object
    */
   virtual const PMVariant getProperty( const QString& name );
   /**
    * get a property on the active object
    */
   virtual const QString getPropertyStr( const QString& name );
   /**
    * List of control points of the active object
    */
   PMControlPointList activeControlPoints() const { return m_controlPoints; }

   /**
    * Called when the user moves some objects in the tree view.
    * If obj is 0, the user moved the objects to another document (remove)
    *
    * Returns true if successful.
    */
   bool dragMoveSelectionTo( PMObject* obj );
   /**
    * Called when the user drags some objects onto the tree view
    *
    * Returns true if successful.
    */
   bool drop( PMObject* obj, const QMimeData* e );
   /**
    * Tries to parse the data and insert the parsed objects
    * as child or siblings of object obj
    *
    * Type is the actions description for the undo/redo menu.
    */
   bool insertFromParser( const QString& type, PMParser* parser, PMObject* obj );

   /**
    * Returns a pointer to the prototype manager
    */
   PMPrototypeManager* prototypeManager() const { return m_pPrototypeManager; }
   /**
    * Returns a pointer to the insert rules system
    */
   PMInsertRuleSystem* insertRuleSystem() const { return m_pInsertRuleSystem; }
   /**
    * Returns a pointer to the IO formats manager for this part
    */
   PMIOManager* ioManager() const { return m_pIOManager; }

   /**
    * eticre refresh visual tree
   */
   void refreshView();
   /**
    * eticre try radiosity not in pov
    */
   bool get_radYN();
   void set_radYN( bool yn ) { radYN = yn;}
   QAction* copyaction() const { return m_pCopyAction; }

   bool openFileQt( QUrl url );

   bool ismodified;

public slots:
   /**
    * Opens the import file dialog
    */
   void slotFileImport();
   /**
    * Opens the export file dialog
    */
   void slotFileExport();

   /**
    * puts the marked text/object into the clipboard and removes the objects
    */
   void slotEditCut();
   /**
    * removes the selected object
    */
   void slotEditDelete();
   /**
   *hide-show object in treeview
   */
   void slotEditHide();
   void slotEditShow();
   /**
    * puts the marked text/object into the clipboard
    */
   void slotEditCopy();
   /**
    * paste the clipboard into the document
    */
   void slotEditPaste();
   /**
    * undoes the last change
    */
   void slotEditUndo();
   /**
    * redoes the last undone command
    */
   void slotEditRedo();

   /**
    * Called when an object is changed.
    * Mode is a bit combination of @ref PMChange constants
    */
   void slotObjectChanged( PMObject* obj, const int mode, QObject* sender );
   /**
    * Called when an id is changed
    */
   void slotIDChanged( PMObject* obj, const QString& oldID );

   /**
    * Inserts a new PMObject of type type
    */
   void slotNewObject( const QString& type );
   /**
    * Inserts a new PMObject of type type at position pos.
    * pos can be one of:
    *     FirstChild
    *     LastChild
    *     Sibling
    */
   void slotNewObject( const QString& type, const QString& pos );
   /**
    * Inserts the new PMObject. The object will be deleted if it can't be
    * inserted or the user aborts the action
    */
   void slotNewObject( PMObject* newObject, int insertAs = 0 );

   /**
    * List the known object types
    */
   virtual QStringList getObjectTypes();

   /**
    * Adds transformations to the object and calls slotNewObject
    * for it
    */
   void slotNewTransformedObject( PMObject* o );

   /**
    * Inserts a new PMGlobalSettings
    */
   void slotNewGlobalSettings();
   /**
    * Inserts a new PMSkySphere
    */
   void slotNewSkySphere();
   /**
    * Inserts a new PMRainbow
    */
   void slotNewRainbow();
   /**
    * Inserts a new PMFog
    */
   void slotNewFog();
   /**
    * Inserts a new PMInterior
    */
   void slotNewInterior();
   /**
    * Inserts a new PMMedia
    */
   void slotNewMedia();
   /**
    * Inserts a new PMDensity
    */
   void slotNewDensity();
   /**
    * Inserts a new PMMaterial
    */
   void slotNewMaterial();
   /**
    * Inserts a new PMBox
    */
   void slotNewBox();
   /**
    * Inserts a new PMSphere
    */
   void slotNewSphere();
   /**
    * Inserts a new PMCylinder
    */
   void slotNewCylinder();
   /**
    * Inserts a new PMCone
    */
   void slotNewCone();
   /**
    * Inserts a new PMTorus
    */
   void slotNewTorus();
   /**
    * Inserts a new PMLathe
    */
   void slotNewLathe();
   /**
    * Inserts a new PMPrism
    */
   void slotNewPrism();
   /**
    * Inserts a new PMSurfaceOfRevolution
    */
   void slotNewSurfaceOfRevolution();
   /**
    * Inserts a new PMSuperquadricEllipsoid
    */
   void slotNewSuperquadricEllipsoid();
   /**
    * Inserts a new PMJuliaFractal
    */
   void slotNewJuliaFractal();
   /**
    * Inserts a new PMHeightField
    */
   void slotNewHeightField();
   /**
    * Inserts a new text object
    */
   void slotNewText();

   /**
    * Inserts a new PMBlob
    */
   void slotNewBlob();
   /**
    * Inserts a new PMBlobSphere
    */
   void slotNewBlobSphere();
   /**
    * Inserts a new PMBlobCylinder
    */
   void slotNewBlobCylinder();

   /**
    * Inserts a new PMPlane
    */
   void slotNewPlane();
   /**
    * Inserts a new PMPolynom
    */
   void slotNewPolynom();
   /**
    * Inserts a new PMDeclare
    */
   void slotNewDeclare();
   /**
    * Inserts a new PMDeclare as function()
    */
   void slotNewDeclareFunction();
   /**
    * Inserts a new PMObjectLink
    */
   void slotNewObjectLink();

   /**
    * Inserts a new PMCSG ( union )
    */
   void slotNewUnion();
   /**
    * Inserts a new PMCSG ( intersection )
    */
   void slotNewIntersection();
   /**
    * Inserts a new PMCSG ( difference )
    */
   void slotNewDifference();
   /**
    * Inserts a new PMCSG ( merge )
    */
   void slotNewMerge();

   /**
    * Inserts a new PMDisc
    */
   void slotNewDisc();
   /**
    * Inserts a new PMBicubicPatch
    */
   void slotNewBicubicPatch();
   /**
    * Inserts a new PMTriangle
    */
   void slotNewTriangle();

   /**
    * Inserts a new PMBoundedBy
    */
   void slotNewBoundedBy();
   /**
    * Inserts a new PMClippedBy
    */
   void slotNewClippedBy();

   /**
    * Inserts a new PMLight object
    */
   void slotNewLight();
   /**
    * Inserts a new PMLooksLike object
    */
   void slotNewLooksLike();
   /**
    * Inserts a new PMProjectedThrough object
    */
   void slotNewProjectedThrough();

   /**
    * Inserts a new PMCamera
    */
   void slotNewCamera();

   /**
    * Inserts a new PMTexture
    */
   void slotNewTexture();
   /**
    * Inserts a new PMPigment
    */
   void slotNewPigment();
   /**
    * Inserts a new PMNormal
    */
   void slotNewNormal();
   /**
    * Inserts a new PMSolidColor
    */
   void slotNewSolidColor();
   /**
    * Inserts a new PMTextureList
    */
   void slotNewTextureList();
   /**
    * Inserts a new PMColorList
    */
   void slotNewColorList();
   /**
    * Inserts a new PMPigmentList
    */
   void slotNewPigmentList();
   /**
    * Inserts a new PMNormalList
    */
   void slotNewNormalList();
   /**
    * Inserts a new PMDensityList
    */
   void slotNewDensityList();
   /**
    * Inserts a new PMFinish
    */
   void slotNewFinish();
   /**
    * Inserts a new PMPattern
    */
   void slotNewPattern();
   /**
    * Inserts a new PMBlendMapModifiers
    */
   void slotNewBlendMapModifiers();
   /**
    * Inserts a new PMTextureMap
    */
   void slotNewTextureMap();
   /**
    * Inserts a new PMMaterialMap
    */
   void slotNewMaterialMap();
   /**
    * Inserts a new PMColorMap
    */
   void slotNewColorMap();
   /**
    * Inserts a new PMPigmentMap
    */
   void slotNewPigmentMap();
   /**
    * Inserts a new PMNormalMap
    */
   void slotNewNormalMap();
   /**
    * Inserts a new PMBumpMap
    */
   void slotNewBumpMap();
   /**
    * Inserts a new PMSlopeMap
    */
   void slotNewSlopeMap();
   /**
    * Inserts a new PMDensityMap
    */
   void slotNewDensityMap();
   /**
    * Inserts a new PMSlope
    */
   void slotNewSlope();
   /**
    * Inserts a new PMWarp
    */
   void slotNewWarp();
   /**
    * Inserts a new PMImageMap
    */
   void slotNewImageMap();
   /**
    * Inserts a new PMQuickColor
    */
   void slotNewQuickColor();

   /**
    * Inserts a new PMTranslate
    */
   void slotNewTranslate();
   /**
    * Inserts a new PMRotate
    */
   void slotNewRotate();
   /**
    * Inserts a new PMScale
    */
   void slotNewScale();
   /**
    * Inserts a new PMPovrayMatrix
    */
   void slotNewMatrix();
   /**
    * Inserts a new PMComment
    */
   void slotNewComment();
   /**
    * Inserts a new PMRaw
    */
   void slotNewRaw();

   // POV-Ray 3.5 objects
   /**
    * Inserts a new PMIsoSurfate
    */
   void slotNewIsoSurface();
   /**
    * Inserts a new PMRadiosity
    */
   void slotNewRadiosity();
   /**
    * Inserts a new PMGlobalPhotons
    */
   void slotNewGlobalPhotons();
   /**
    * Inserts a new PMPhotons
    */
   void slotNewPhotons();
   /**
    * Inserts a new PMLightGroup
    */
   void slotNewLightGroup();
   /**
    * Inserts a new PMInteriorTexture
    */
   void slotNewInteriorTexture();
   /**
    * Inserts a new PMSphereSweep
    */
   void slotNewSphereSweep();
   /**
    * Inserts a new PMMesh
    */
   void slotNewMesh();


   /**
    * Called when the clipboard contents changed
    */
   void slotClipboardDataChanged();
   /**
    * updates the undo/redo menu items
    */
   void slotUpdateUndoRedo( const QString& undo, const QString& redo );

   /**
    * Starts rendering with povray
    */
   virtual void slotRender();
   /**
    * Opens the render settings
    */
   void slotRenderSettings();
   /**
    * Called when a render mode is selected
    */
   void slotRenderMode( int index );
   /**
    * Shows the render window
    */
   void slotViewRenderWindow();
   /**
    * Called when the visibility level has changed
    */
   void slotVisibilityLevelChanged( int );
   /**
    * Called when the global detail level has changed
    */
   void slotGlobalDetailLevelChanged( QAction *act );

   /**
    * Opens the search library object dialog
    */
   void slotSearchLibraryObject();

   /** */
   virtual void setModified();
   /** */
   virtual void setModified( bool modified );

   /** Set the scene object. Must be used with extreme care. */
   void setScene( PMScene* scene );

	/**
	 * Emits the aboutToSave signal
	 */
	void slotAboutToSave();

signals:
   /**
    * Signal that is emitted when an object is changed.
    * Mode is a bit combination of @ref PMChange constants.
    */
   void objectChanged( PMObject* obj, const int mode, QObject* sender );
   /**
    * Signal that is emitted when the views have to be refreshed.
    * Usually on newDocument or openDocument
    */
   void refresh();
   void refreshList();//eticre refresh and view not_in_tree
   /**
    * Emitted when all views should delete all data
    */
   void clear();
   /**
    * Emitted, when the modified flag changes
    */
   void modified();
   /**
    * Emitted when the mouse is over a control point
    */
   void controlPointMessage( const QString& msg );
   /**
    * Emitted when the active render mode has changed
    */
   void activeRenderModeChanged();
   /**
    * Emitted before the scene is rendered.
    *
    * Views should ask the user to save pending changes.
    */
   void aboutToRender();
   /**
    * Emitted before the scene is saved or exported
    *
    * Views should ask the user to save pending changes.
    */
   void aboutToSave();

protected:
   /**
    *
    */
   //virtual bool openFile();
   /**
    *
    */
   virtual bool saveFile() { return false; } //use saveFileQt

   /**
    * Inits all actions
    */
   void initActions();
   /**
    * Inits only actions related to copy&paste.
    * Required by the library browser.
    */
   void initCopyPasteActions();

   /**
    * creates the widget of the part instance and sets
    * it as the view
    */
   void initView( QWidget* parent );
   /**
    * initializes the documents contents
    */
   void initDocument();
   /**
    * clears the selection
    */
   void clearSelection();

private:
   /**
    * Disables all actions, that modify the part
    */
   QUrl m_pUrl;
   QWidget* m_pwidget;
   QObject* m_pparent;

   bool m_readWrite;

   void disableReadWriteActions( bool b );
   QAction* actionCollection(QString name , QMenu *menu);
   /**
    * Updates all "new object" actions
    */
   void updateNewObjectActions();
   /**
    * Finds a free id of the format <prefix><number>.
    *
    * Adds the object to the symbol table.
    *
    * Returns the number.
    */
   unsigned int findNewID( const QString& prefix, unsigned int firstNumber,
                           PMDeclare* obj );
   /**
    * Updates the list of cameras
    */
   void updateCameraList();

   /**
    * Generic drop/paste function
    */
   bool pasteOrDrop( const QString& type, const QMimeData* mime, PMObject* obj );
   /**
    * Generic cut/delete/remove function
    */
   bool removeSelection( const QString& type );
      bool hideSelection( const QString& type );
         bool showSelection( const QString& type );
   /**
    * Updates the render mode combo action
    */
   void updateRenderModes();
   /**
    * Updates the visibility level action
    */
   void updateVisibilityLevel();
   /**
    * Updates the control point list
    */
   void updateControlPoints( PMObject* oldActive );

   PMView* m_pView;
   PMShell* m_pShell;
   //PMBrowserExtension* m_pExtension;

   /**
    * the selected objects, unsorted!
    */
   PMObjectList m_selectedObjects;
   /**
    * the selected objects, sorted. This list is only created if necessary.
    */
   PMObjectList m_sortedSelectedObjects;
   /**
    * true if the list m_sortedSelectedObjects is up to date
    */
   bool m_sortedListUpToDate;
   /**
    * the active object
    */
   PMObject* m_pActiveObject;
   /**
    * the new selection after a command was executed
    */
   PMObject* m_pNewSelection;
   /**
    * List of all cameras
    */
   QList< PMCamera* > m_cameras;
   /**
    * true if the m_cameras list is up to date
    */
   bool m_bCameraListUpToDate;
   /**
    * true if the clipboard data can be decoded
    */
   bool m_canDecode;
   /**
    * Commands stack for undo and redo
    */
   PMCommandManager m_commandManager;
   /**
    * The povray scene, top level object
    */
   PMScene* m_pScene;
   /**
    * Number of added objects during the last executed command
    */
   unsigned int m_numAddedObjects;
   /**
    * Number of insert errors during the last executed command
    */
   unsigned int m_numInsertErrors;
   /**
    * Details of insert errors
    */
   QStringList m_insertErrorDetails;
   /**
    * The symbol table for this document
    */
   PMSymbolTable* m_pSymbolTable;
   /**
    * The povray render window
    */
   PMPovrayWidget* m_pPovrayWidget;
   /**
    * true if the new object actions have to be updated
    */
   bool m_updateNewObjectActions;
   /**
    * Control points of the active object
    */
   PMControlPointList m_controlPoints;
   /**
    * true if only copy'n'paste actions are available
    */
   bool m_onlyCopyPaste;

   bool m_bModified;

   // the actions
   QMenu* menuRenderModes;
   QMenu* menu_gdl;
   QMenu* menuSolidPri;
   QMenu* menuFinitePatch;
   QMenu* menuInfiniteSolid;
   QMenu* menuCsg;
   QMenu* menuMaterial;
   QMenu* menuInterior;
   QMenu* menuTexture;
   QMenu* menuPhotons;
   QMenu* menuAthmo;
   QMenu* menuTrans;
   QActionGroup* renderModes;
   QActionGroup* detail;
   QAction* verylow;
   QAction* low;
   QAction* medium;
   QAction* high;
   QAction* veryhigh;

   QAction* m_pImportAction;
   QAction* m_pExportAction;

   QAction* m_pCutAction;
   QAction* m_pCopyAction;
   QAction* m_pPasteAction;
   QAction* m_pUndoAction;
   QAction* m_pRedoAction;
   QAction * m_pDeleteAction;
   QAction * m_pHideAction;//eticre hide obj in tree by attribute
   QAction * m_pShowAction;//eticre show obj in tree by attribute

   QAction* m_pNewGlobalSettingsAction;
   QAction* m_pNewSkySphereAction;
   QAction* m_pNewRainbowAction;
   QAction* m_pNewFogAction;
   QAction* m_pNewInteriorAction;
   QAction* m_pNewMediaAction;
   QAction* m_pNewDensityAction;
   QAction* m_pNewMaterialAction;
   QAction* m_pNewBoxAction;
   QAction* m_pNewSphereAction;
   QAction* m_pNewCylinderAction;
   QAction* m_pNewConeAction;
   QAction* m_pNewTorusAction;
   QAction* m_pNewLatheAction;
   QAction* m_pNewPrismAction;
   QAction* m_pNewSurfaceOfRevolutionAction;
   QAction* m_pNewSuperquadricEllipsoidAction;
   QAction* m_pNewJuliaFractalAction;
   QAction* m_pNewHeightFieldAction;
   QAction* m_pNewTextAction;

   QAction* m_pNewBlobAction;
   QAction* m_pNewBlobSphereAction;
   QAction* m_pNewBlobCylinderAction;

   QAction* m_pNewPlaneAction;
   QAction* m_pNewPolynomAction;

   QAction* m_pNewDeclareAction;
   QAction* m_pNewObjectLinkAction;

   QAction* m_pNewUnionAction;
   QAction* m_pNewDifferenceAction;
   QAction* m_pNewIntersectionAction;
   QAction* m_pNewMergeAction;

   QAction* m_pNewBoundedByAction;
   QAction* m_pNewClippedByAction;

   QAction* m_pNewBicubicPatchAction;
   QAction* m_pNewDiscAction;
   QAction* m_pNewTriangleAction;

   QAction* m_pNewLightAction;
   QAction* m_pNewLooksLikeAction;
   QAction* m_pNewProjectedThroughAction;

   QAction* m_pNewCameraAction;

   QAction* m_pNewTextureAction;
   QAction* m_pNewPigmentAction;
   QAction* m_pNewNormalAction;
   QAction* m_pNewSolidColorAction;
   QAction* m_pNewTextureListAction;
   QAction* m_pNewColorListAction;
   QAction* m_pNewPigmentListAction;
   QAction* m_pNewNormalListAction;
   QAction* m_pNewDensityListAction;
   QAction* m_pNewFinishAction;
   QAction* m_pNewPatternAction;
   QAction* m_pNewBlendMapModifiersAction;
   QAction* m_pNewTextureMapAction;
   QAction* m_pNewMaterialMapAction;
   QAction* m_pNewPigmentMapAction;
   QAction* m_pNewColorMapAction;
   QAction* m_pNewNormalMapAction;
   QAction* m_pNewBumpMapAction;
   QAction* m_pNewSlopeMapAction;
   QAction* m_pNewDensityMapAction;
   QAction* m_pNewSlopeAction;
   QAction* m_pNewWarpAction;
   QAction* m_pNewImageMapAction;
   QAction* m_pNewQuickColorAction;

   QAction* m_pNewTranslateAction;
   QAction* m_pNewScaleAction;
   QAction* m_pNewRotateAction;
   QAction* m_pNewMatrixAction;

   QAction* m_pNewCommentAction;
   QAction* m_pNewRawAction;

   // POV-Ray 3.5 objects
   QAction* m_pNewIsoSurfaceAction;
   QAction* m_pNewRadiosityAction;
   QAction* m_pNewGlobalPhotonsAction;
   QAction* m_pNewPhotonsAction;
   QAction* m_pNewLightGroupAction;
   QAction* m_pNewInteriorTextureAction;
   QAction* m_pNewSphereSweepAction;
   QAction* m_pNewMeshAction;

   QAction* m_pSearchLibraryObjectAction;

   QComboBox* m_pRenderComboAction;
   QAction* m_pRenderSettingsAction;
   QAction* m_pViewRenderWindowAction;
   QAction* m_pRenderAction;
   //PMSpinBoxAction* m_pVisibilityLevelAction;
   QWidgetAction* m_pVisibilityLevelAction;
   QSpinBox* spbx;
   //KToolBarLabelAction* m_pVisibilityLabelAction;
   //KSelectAction* m_pGlobalDetailAction;
   //KToolBarLabelAction* m_pGlobalDetailLabelAction;

   QList< QAction* > m_readWriteActions;
   QHash<QString, QAction*> *m_hash_readWriteActions;

   PMPrototypeManager* m_pPrototypeManager;
   PMInsertRuleSystem* m_pInsertRuleSystem;
   PMIOManager* m_pIOManager;
   bool radYN;
   QMenu* fileMenu;
   QMenu* viewMenu;
   QMenu* insertMenu;
   QMenu* editMenu;
};


#endif
