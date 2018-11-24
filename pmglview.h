//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2007 by Andreas Zehender
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


#ifndef PMGLVIEW_H
#define PMGLVIEW_H



#include <QWidget>
#include <QPoint>
#include <QDateTime>
#include <QTimer>
#include <QPixmap>
//Added by qt3to4:
#include <QWheelEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QList>
#include <QLabel>
#include "libkpovmodeler_export.h"
#include "pmviewbase.h"
#include "pmviewfactory.h"
#include "pmcontrolpoint.h"
#include "pmvector.h"
#include "pmmatrix.h"
#include "pmobjectaction.h"

const int controlPointSize = 7;

class PMObject;
class PMPart;
class PMCamera;
class QComboBox;
class QAction;

/**
 * Widget for rendering the scene with OpenGL
 */
class LKPOVMODELER_EXPORT PMGLView : public PMViewBase
{
   Q_OBJECT
public:
   /**
    * Type of the view
    */
   enum PMViewType { PMViewPosX, PMViewNegX, PMViewPosY, PMViewNegY,
                     PMViewPosZ, PMViewNegZ, PMViewCamera };
   /**
    * Constructor
    */
   PMGLView(PMPart* part, PMViewType t,
             QWidget* parent = nullptr, Qt::WindowFlags f = nullptr );
   /**
    * Destructor
    */
   ~PMGLView();

   /** */
   virtual QString viewType() const { return QString( "glview" ); }
   /** */
   virtual QString description() const;
   /** */
   virtual void restoreViewConfig( PMViewOptions* );
   /** */
   virtual void saveViewConfig( PMViewOptions* ) const;

   /**
    * Enables/disables translating the view with the mouse
    */
   void enableTranslateMode( bool yes = true );
   /**
    * Enables/disables scaling the view with the mouse
    */
   void enableScaleMode( bool yes = true );

   /**
    * Returns true if the opengl projection is up to date
    */
   bool isProjectionUpToDate() const { return m_projectionUpToDate; }
   /**
    * Sets the projection up to date flag
    */
   void setProjectionUpToDate( bool yes ) { m_projectionUpToDate = yes; }

   /**
    * Sets the scale
    */
   void setScale( double scale );
   /**
    * Returns the scale
    */
   double scale() const { return m_dScale; }
   /**
    * Sets the views translation in x direction
    */
   void setTranslationX( double d );
   /**
    * Returns the views translation in x direction
    */
   double translationX() const { return m_dTransX; }
   /**
    * Sets the views translation in y direction
    */
   void setTranslationY( double d );
   /**
    * Returns the views translation in y direction
    */
   double translationY() const { return m_dTransY; }

   /**
    * Returns the 2D control points position in the view
    */
   const QList<PMVector*>& controlPointsPosition() const
   {
      return m_controlPointsPosition;
   }
   /**
    * Returns the last right mouse click position
    */
   PMVector contextClickPosition() const { return m_contextClickPosition; }

   /**
    * Returns the view type
    */
   PMViewType type() const { return m_type; }
   /**
    * Sets the view type
    */
   void setType( PMViewType t );
   /**
    * Sets the camera
    */
   void setCamera( PMCamera* c );
   /**
    * Returns the camera
    */
   PMCamera* camera() const { return m_pCamera; }

   /**
    * Saves the configuration
    */
   static void saveConfig();
   /**
    * Restores the configuration
    */
   static void restoreConfig();
   /**
    * Returns the view type as string
    */
   static QString viewTypeAsString( PMViewType t );
   /**
    * Sets the direct rendering flag
    */
   static void enableDirectRendering( bool yes ) { s_bDirect = yes; }
   /**
    * Returns the direct rendering flag
    */
   static bool isDirectRenderingEnabled() { return s_bDirect; }
	/**
	 * used by the render manager to set the rendered scene
	 */
    void setRenderedScene( const QPixmap& pix ) {
        m_renderedScene = pix;
        update();
	}
	
public slots:
   /**
    * Sets the view normal vector to the positive x-axes
    */
   void slotSetTypePosX() { setType( PMViewPosX ); }
   /**
    * Sets the view normal vector to the negative x-axes
    */
   void slotSetTypeNegX() { setType( PMViewNegX ); }
   /**
    * Sets the view normal vector to the positive y-axes
    */
   void slotSetTypePosY() { setType( PMViewPosY ); }
   /**
    * Sets the view normal vector to the negative y-axes
    */
   void slotSetTypeNegY() { setType( PMViewNegY ); }
   /**
    * Sets the view normal vector to the positive z-axes
    */
   void slotSetTypePosZ() { setType( PMViewPosZ ); }
   /**
    * Sets the view normal vector to the negative z-axes
    */
   void slotSetTypeNegZ() { setType( PMViewNegZ ); }

   /**
    * Called when an object is changed.
    * @see PMPart::objectChanged()
    */
   void slotObjectChanged( PMObject* obj, const int mode, QObject* sender );
   /**
    * Restarts rendering
    */
   void slotRefresh();
   /**
    * Clears all data
    */
   void slotClear();
   /**
    * Stops rendering
    */
   void slotStopRendering();
   /**
    * Repaints the view if it is a camera view
    */
   void slotActiveRenderModeChanged();

   /**
    * Connected to the render manager
    */
   void slotRenderingStarted( PMGLView* view );
   /**
    * Connected to the render manager
    */
   void slotAboutToUpdate( PMGLView* view );
   /**
    * Connected to the render manager
    */
   void slotRenderingFinished( PMGLView* view );

protected slots:
   /**
    * Sets the view type to camera view
    */
   void slotCameraView( QAction* );
   /**
    * Called when an object action was selected in the context menu
    */
   void slotObjectAction( QAction* );
   /**
    * Called when a control point was selected in the context menu
    */
   void slotControlPoint( QAction* );
   /**
    * Aligns the selected control points to the grid
    */
   void slotSnapToGrid();

   void slotMouseChangeTimer();
   void slotAutoScroll();

signals:
   /**
    * Emitted when rendering has to be restarted
    */
   void refresh( PMGLView* w );
   /**
    * Emitted when an object is changed
    */
   void objectChanged( PMObject* obj, const int mode, QObject* sender );
   /**
    * Emitted when the mouse is over a control point
    */
   void controlPointMessage( const QString& msg );
   /**
    * Emitted in the destructor
    */
   void destroyed( PMGLView* v );
   /**
    * Emitted when the view type changes
    */
   void viewTypeChanged( const QString& str );

protected:
   /** */
   virtual void resizeEvent( QResizeEvent* e );
   /** */
   virtual void paintEvent( QPaintEvent* e );
   /** */
   virtual void mousePressEvent( QMouseEvent* e );
   /** */
   virtual void mouseReleaseEvent( QMouseEvent* e );
   /** */
   virtual void mouseMoveEvent( QMouseEvent* e );
   /** */
   virtual void keyPressEvent( QKeyEvent* e );
   /**
    * Event to zoom in / zoom out the viewport by mouse wheel
    */
   virtual void wheelEvent( QWheelEvent* e );

private:

   /**
    * Updates the control points
    */
   void updateControlPoints();
   /**
    * Recalculates the position of the control points on the screen
    */
   void recalculateControlPointPosition();
   /**
    * Recalculates m_controlPointsTransformation and
    * m_inversePointsTransformation
    */
   void recalculateTransformations();
   /**
    * Returns the mouse 3D position, when the control point cp is selected
    *
    * x and y are the screen coordinates of the mouse.
    */
   PMVector mousePosition( PMControlPoint* cp, int x, int y );
   /**
    * Checks if a control point is under the mouse
    */
   void checkUnderMouse( int x, int y );
   /**
    * Repaints the view
    */
   void rerender( bool graphicalChange = false );
   /**
    * Starts a graphical change
    */
   void startChange( const QPoint& mousePos );
   /**
    * Graphical Change
    */
   void graphicalChange( const QPoint& mousePos );
   /**
    * Selects/deselecs the control point. If cp is 0, all control points are
    * selected/deselected.
    */
   void selectControlPoint( PMControlPoint* cp, bool select, bool deselectOthers = true );
   /**
    * Invalidates the projection and repaints the view
    */
   void invalidateProjection( bool graphicaChange = false );

   /**
    * Starts multiple selection mode
    */
   void startSelection();
   /**
    * Restores the widget under the selection rect
    */
   void restoreSelectionBox();
   /**
    * Paints the selection box
    */
   void paintSelectionBox();
   /**
    * Calculates the selection box
    */
   void calculateSelectionBox( int& sx, int& sy, int& ex, int& ey, int& w, int& h );

   double screenToInternalX( int x ) const;
   double screenToInternalY( int y ) const;

   /**
    * Returns the top level object for rendering (a declaration or the scene)
    */
   PMObject* topLevelRenderingObject( PMObject* obj ) const;
   /**
    * Displays the context menu
    */
   void contextMenu();

   /**
    * Type of the view (camera, xy, ... )
    */
   PMViewType m_type;
   /**
    * Pointer to the part
    */
   PMPart* m_pPart;
   /**
    * True if "scale view" is active
    */
   bool m_bScaleMode;
   double m_scaleIntX, m_scaleIntY;
   /**
    * true if "translate view" is active
    */
   bool m_bTranslateMode;
   /**
     * True if "scale view" or "translate view" is active and the left
     * mouse button is pressed.
     */
   bool m_bMousePressed;
   /**
    * MidButton pressed
    */
   bool m_bMidMousePressed;
   /**
    * True if a graphical change is active
    */
   bool m_bGraphicalChangeMode;
   bool m_bMementoCreated;
   /**
    * The old mouse position
    */
   QPoint m_mousePos;
   QPoint m_changeStartPos;
   QPoint m_currentMousePos;
   QTimer m_startTimer;
   QTime m_changeStartTime;
   bool m_bDeselectUnderMouse;
   bool m_bSelectUnderMouse;

   QPoint m_selectionStart, m_selectionEnd;
   bool m_bMultipleSelectionMode;
   bool m_bSelectionStarted;
   QWidget* m_pDragOverlay;

   /**
    * Member variables for autoscroll
    */
   bool m_bAutoScroll;
   double m_autoScrollSpeed;
   QTimer m_autoScrollTimer;
   QTime m_lastAutoScrollUpdate;
   int m_autoScrollDirectionX, m_autoScrollDirectionY;
   /**
    * Rendering
    */
   bool m_bAboutToUpdate;

   /**
    * Scale of the view
    */
   double m_dScale;
   /**
    * X-translation of the view
    */
   double m_dTransX;
   /**
    * Y-translation of the view
    */
   double m_dTransY;

   /**
    * Control points of the active object
    */
   PMControlPointList m_controlPoints;
   /**
    * Control point under the mouse
    */
   PMControlPoint* m_pUnderMouse;
   /**
    * Position of the control points on the screen
    */
   QList<PMVector*> m_controlPointsPosition;
   /**
    * Position of the last right mouse click
    */
   PMVector m_contextClickPosition;
   /**
    * Transformation of the control points
    *
    * Always m_viewTransformation * m_objectsTransformation.
    */
   PMMatrix m_controlPointsTransformation;
   /**
    * Inverse of m_controlPointsTransformation
    */
   PMMatrix m_inversePointsTransformation;
   /**
    * True if m_inversePointsTransformation is valid
    */
   bool m_bInverseValid;
   /**
    * Normal vector of the view
    */
   PMVector m_normal;
   /**
    * Transformation of the view (scale and translation)
    */
   PMMatrix m_viewTransformation;
   /**
    * Transformation of the active object
    */
   PMMatrix m_objectsTransformation;
   /**
    * The camera
    */
   PMCamera* m_pCamera;
   PMObject* m_pActiveObject;
   /**
    * true if the opengl projection is up to date
    */
   bool m_projectionUpToDate;
   int m_visibilityLevel;
   /**
    * The current object actions
    */
   QList<PMObjectAction*> m_objectActions;
   static bool s_bDirect;
   QPixmap m_renderedScene;
};

/**
 * Options class for the opengl view
 */
class PMGLViewOptions : public PMViewOptions
{
public:
	virtual ~PMGLViewOptions(){}
   PMGLViewOptions()
   {
      m_glViewType = PMGLView::PMViewPosX;
   }
   PMGLViewOptions( PMGLView::PMViewType t )
   {
      m_glViewType = t;
   }
   virtual PMViewOptions* copy() const { return new PMGLViewOptions( *this ); }
   virtual QString viewType() const { return QString( "glview" ); }
   PMGLView::PMViewType glViewType() const { return m_glViewType; }
   void setGLViewType( PMGLView::PMViewType t ) { m_glViewType = t; }
   virtual void loadData( QDomElement& e );
   virtual void saveData( QDomElement& e );

private:
   PMGLView::PMViewType m_glViewType;
};

/**
 * Factory class for 3d views
 */
class PMGLViewFactory : public PMViewTypeFactory
{
public:
   PMGLViewFactory() { }
   virtual QString viewType() const { return QString( "glview" ); }
   virtual QString description() const;
   virtual QString description( PMViewOptions* ) const;
   virtual QString iconName() const { return QString( "pmglview" ); }
   virtual PMViewBase* newInstance( QWidget* parent, PMPart* part ) const
   {
      return new PMGLView( part, PMGLView::PMViewPosX, parent );
   }
   virtual PMViewOptions* newOptionsInstance() const;
   virtual PMViewOptionsWidget* newOptionsWidget( QWidget*, PMViewOptions* );
};

/**
 * Configuration widget for the view layout settings dialog
 */
class PMGLViewOptionsWidget : public PMViewOptionsWidget
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMGLViewOptionsWidget( QWidget* parent, PMViewOptions* o );

protected slots:
   void slotGLViewTypeChanged( int );

private:
   PMGLViewOptions* m_pOptions;
   QComboBox* m_pGLViewType;
};

#endif
