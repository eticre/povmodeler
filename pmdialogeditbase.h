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


#ifndef PMDIALOGEDITBASE_H
#define PMDIALOGEDITBASE_H


#include "pmobject.h"
#include "pmobjectselect.h"
#include "pmrendermode.h"
#include "pmtexture.h"
#include "pmdeclare.h"
#include "pmpigment.h"
#include "pmdialogview.h"


class QBoxLayout;
class QCheckBox;
class QPushButton;

class PMPart;
class PMPovrayRenderWidget;
class PMPovrayOutputWidget;

/**
 * Base class for all widgets for editing object attributes.
 *
 * Ensures a consistent layout for all widgets. Widgets should not
 * created within the constructor, but with the functions @ref createTopWidgets
 * and @ref createBottomWidgets.
 * 
 * Each subclass uses the functionality of the base class. For example all
 * widgets for solid objects have the same base class that shows attributes
 * of solid objects. Subclasses like the widget for the box object add their
 * object specific controls.
 */
class PMDialogEditBase : public QWidget
{
   friend class PMObjectSelect;

   Q_OBJECT
public:
   /**
    * Creates a new PMDialogEditBase widget objectType is
    * @ref PMObject->description().
    *
    * No widgets are created within the constructor! You have to call
    * @ref createWidgets after creating a new edit widget.
    */
   PMDialogEditBase( QWidget* parent );
   /**
    * Destructor
    */
   virtual ~PMDialogEditBase();

   /**
    * Creates child widgets.
    *
    * This function is necessary because virtual functions do not work
    * properly inside the constructor.
    *
    * Calls @ref createTopWidgets and @ref createBottomWidgets
    */
   void createWidgets();

   /**
    * Displays the object o.
    * Always call displayObject() of the base class after displaying the
    * objects data
    */
   virtual void displayObject( PMObject* o );
   
   /**
    * returns a pointer to the displayed object
    */
   PMObject* displayedObject() { return m_pDisplayedObject; }
   
   /**
    * Function that is called, when the "Apply" button is pressed.
    *
    * Returns true if successful
    */
   bool saveData();
   
   /**
    * Called, when the contents have to be checked.
    *
    * Display an error message and return false, if the data is invalid.
    * Otherwise return isDataValid() of the base class.
    */
   virtual bool isDataValid() { return true; }
   
   /**
    * Returns the help topic
    */
   const QString& helpTopic() { return m_helpTopic; }

   /**
    * Discards changes and redisplays the object
    */
   void redisplay();
   
   /**
    * Called when the control point selection has changed
    */
   virtual void updateControlPointSelection() { }

   /**
    * Sets the check box to state s
    */
   static void setCheckBox( QCheckBox* box, PMThreeState s );
   /**
    * Gets the state of the checkbox
    */
   static PMThreeState checkBoxState( QCheckBox* box );

   /**
    * Sets the part
    */
   void setPart( PMPart* p ) { m_pPart = p; }
   /**
    * Returns the part
    */
   PMPart* part() const { return m_pPart; }

   /**
    * Returns the size of the texture preview widget
    */
   static int previewSize() { return s_previewSize; }
   /**
    * Sets the texture preview size
    */
   static void setPreviewSize( int size );
   /**
    * Returns true if a sphere should be renderend
    */
   static bool previewShowSphere() { return s_showSphere; }
   /**
    * Enable/disable the sphere in the texture preview widget
    */
   static void previewShowSphere( bool show ) { s_showSphere = show; }
   /**
    * Returns true if a cylinder should be renderend
    */
   static bool previewShowCylinder() { return s_showCylinder; }
   /**
    * Enable/disable the cylinder in the texture preview widget
    */
   static void previewShowCylinder( bool show ) { s_showCylinder = show; }
   /**
    * Returns true if a box should be renderend
    */
   static bool previewShowBox() { return s_showBox; }
   /**
    * Enable/disable the box in the texture preview widget
    */
   static void previewShowBox( bool show ) { s_showBox = show; }

   /**
    * Returns true if AA is enabled
    */
   static bool isPreviewAAEnabled() { return s_previewAA; }
   /**
    * Enables/disables AA
    */
   static void setPreviewAAEnabled( bool enable ) { s_previewAA = enable; }
   /**
    * Returns the AA depth
    */
   static int previewAADepth() { return s_previewAADepth; }
   /**
    * Sets the AA depth
    */
   static void setPreviewAADepth( int d );
   /**
    * Returns the AA threshold
    */
   static double previewAAThreshold() { return s_previewAAThreshold; }
   /**
    * Sets the AA threshold
    */
   static void setPreviewAAThreshold( double t ) { s_previewAAThreshold = t; }
   
   /**
    * Returns true if the floor should be rendered
    */
   static bool previewShowFloor() { return s_showFloor; }
   /**
    * Enables/disables the floor
    */
   static void previewShowFloor( bool show ) { s_showFloor = show; }
   /**
    * Returns true if the wall should be rendered
    */
   static bool previewShowWall() { return s_showWall; }
   /**
    * Enables/disables the wall
    */
   static void previewShowWall( bool show ) { s_showWall = show; }
   /**
    * Returns the first wall color
    */
   static QColor previewWallColor1() { return s_wallColor1; }
   /**
    * Sets the first wall color
    */
   static void setPreviewWallColor1( const QColor& c ) { s_wallColor1 = c; }
   /**
    * Returns the second wall color
    */
   static QColor previewWallColor2() { return s_wallColor2; }
   /**
    * Sets the second wall color
    */
   static void setPreviewWallColor2( const QColor& c ) { s_wallColor2 = c; }
   /**
    * Returns the first floor color
    */
   static QColor previewFloorColor1() { return s_floorColor1; }
   /**
    * Sets the first floor color
    */
   static void setPreviewFloorColor1( const QColor& c ) { s_floorColor1 = c; }
   /**
    * Returns the second floor color
    */
   static QColor previewFloorColor2() { return s_floorColor2; }
   /**
    * Sets the second floor color
    */
   static void setPreviewFloorColor2( const QColor& c ) { s_floorColor2 = c; }
   /**
    * Returns the local flag for texture preview
    */
   static bool previewLocal() { return s_previewLocal; }
   /**
    * Sets the local flag
    */
   static void setPreviewLocal( bool l ) { s_previewLocal = l; }
   /**
    * Returns the gamma value for the texture preview
    */
   static double previewGamma() { return s_previewGamma; }
   /**
    * Sets the gamma value for the texture preview
    */
   static void setPreviewGamma( double g ) { s_previewGamma = g; }
   /**
    *eticre preview from object link
    */
   bool b_onlyPreview;

   static void saveConfig();
   static void restoreConfig();
   virtual QSize sizeHint() const;
protected:
   /**
    * Sets the help topic
    * @param anchor Defined anchor in your docbook sources
    */
   void setHelp( const QString& anchor );

   /**
    * Create widgets here, that should be placed on top of the widgets
    * of the sub class.
    *
    * First call the function of the base class, then create and append
    * the widgets to the top layout.
    */
   virtual void createTopWidgets() { }

   /**
    * Create widgets here, that should be placed under the widgets
    * of the sub class.
    *
    * First create and append the widgets to the top layout, then
    * call the function of the base class
    */
   virtual void createBottomWidgets();
   
   /**
    * Save here the class specific data and call saveContents()
    * of the base class
    */
   virtual void saveContents();

   /**
    * Returns a pointer to the top layout
    */
   QBoxLayout* topLayout() const { return m_pTopLayout; }

protected slots:
   void slotTexturePreview();
   void slotPreviewLocal( bool on );
   void slotPreviewFinished( int exitStatus );
   void slotPovrayOutput();
   void slotPreviewImage( QImage img );
   
signals:
   /**
    * Emit this, if data has changed
    */
   void dataChanged();
   /**
    * Emit this, if the size of the widget has changed
    */
   void sizeChanged();
   /**
    * Emit this, if the control point selection has changed
    */
   void controlPointSelectionChanged();
   /**
    * Emit this signal, before the displayed object or texture is rendered
    */
   void aboutToRender();
private:
   void findTextures( PMObject*& global, PMObject*& local ) const;
   
   PMObject* m_pDisplayedObject;
   PMObject* m_pTmpObject;
   QBoxLayout* m_pTopLayout;
   QString m_helpTopic;
   PMPart* m_pPart;
   QWidget* m_pTexturePreviewWidget;
   PMPovrayRenderWidget* m_pRenderWidget;
   PMPovrayOutputWidget* m_pOutputWidget;
   QWidget* m_pRenderFrame;
   QCheckBox* m_pPreviewLocalBox;
   QPushButton* m_pPreviewButton;
   QPushButton* m_pOutputButton;
   QByteArray qba_scene_link_preview;
   PMRenderMode p_mode_link_preview;
   QImage previewImage;
   QLabel* labelPreviewImage;

   static int s_previewSize;
   static bool s_showSphere;
   static bool s_showCylinder;
   static bool s_showBox;
   static bool s_previewAA;
   static int s_previewAADepth;
   static double s_previewAAThreshold;
   static bool s_showFloor;
   static bool s_showWall;
   static QColor s_wallColor1, s_wallColor2;
   static QColor s_floorColor1, s_floorColor2;
   static bool s_previewLocal;
   static double s_previewGamma;
};


#endif

