/*
**************************************************************************
                                 description
                             --------------------
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

#ifndef PMLAYOUTSETTINGS_H
#define PMLAYOUTSETTINGS_H



#include "pmsettingsdialog.h"
#include "pmviewlayoutmanager.h"

#include <QList>
#include <QLabel>
#include <QTableView>
#include <QStandardItemModel>

class PMIntEdit;
class PMViewOptionsWidget;
class QComboBox;
class QListWidget;
class QPushButton;
class QLabel;

/**
 * View layout configuration dialog page
 */
class PMLayoutSettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMLayoutSettings( QWidget* parent = 0 );
   /** */
   virtual void displaySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void applySettings();
   /** */
   virtual void displayDefaults();
   
protected slots:
   /**
    * Called when the browse add layout button is clicked
    */
   void slotAddLayout();
   /**
    * Called when the remove layout button is clicked
    */
   void slotRemoveLayout();
   /**
    * Called when the selected layout changes
    */
   void slotLayoutSelected(int layoutIndex );
   /**
    * Called when the layout name changes
    */
   void slotLayoutNameChanged( const QString& text );
   /**
    * Called when the selected view entry changes
    */
   void slotViewEntrySelected( QModelIndex mindex );
   /**
    * Called when the view type field changes value
    */
   void slotViewTypeChanged( int index );
   /**
    * Called when the gl view type field changes value
    */
   //void slotGLViewTypeChanged( int index );
   /**
    * Called when the dock position field changes value
    */
   void slotDockPositionChanged( int index );
   /**
    * Called when the view height field changes value
    */
   void slotViewHeightChanged( const QString& text );
   /**
    * Called when the column width field changes value
    */
   void slotColumnWidthChanged( const QString& text );
   /**
    * Called when the floating height changes value
    */
   void slotFloatingHeightChanged( const QString& text );
   /**
    * Called when the floating width changes value
    */
   void slotFloatingWidthChanged( const QString& text );
   /**
    * Called when the floating position x changes value
    */
   void slotFloatingPosXChanged( const QString& text );
   /**
    * Called when the floating position y changes value
    */
   void slotFloatingPosYChanged( const QString& text );
   /**
    * Called when the add view entry button is clicked
    */
   void slotAddViewEntryClicked();
   /**
    * Called when the remove view entry button is clicked
    */
   void slotRemoveViewEntryClicked();
   /**
    * Called when the move up view entry button is clicked
    */
   void slotMoveUpViewEntryClicked();
   /**
    * Called when the move down view entry button is clicked
    */
   void slotMoveDownViewEntryClicked();
   /**
    * Called when the view type description has changed
    */
   void slotViewTypeDescriptionChanged();

private:
   void displayLayoutList();
   void displayCustomOptions();
   
   QComboBox* m_pDefaultLayout;
   QListWidget* m_pViewLayouts;
   QPushButton* m_pAddLayout;
   QPushButton* m_pRemoveLayout;
   QLineEdit* m_pViewLayoutName;
   QTableView* m_pViewEntries;
   QStandardItemModel* m_model;
   QPushButton* m_pAddEntry;
   QPushButton* m_pRemoveEntry;
   QPushButton* m_pMoveUpEntry;
   QPushButton* m_pMoveDownEntry;
   QComboBox* m_pViewTypeEdit;
   QComboBox* m_pDockPositionEdit;
   PMIntEdit* m_pColumnWidthEdit;
   QLabel* m_pColumnWidthLabel;
   PMIntEdit* m_pViewHeightEdit;
   QLabel* m_pViewHeightLabel;
   
   PMIntEdit* m_pFloatingHeight;
   PMIntEdit* m_pFloatingWidth;
   QLabel* m_pFloatingHeightLabel;
   QLabel* m_pFloatingWidthLabel;
   PMIntEdit* m_pFloatingPosX;
   PMIntEdit* m_pFloatingPosY;
   QLabel* m_pFloatingPosXLabel;
   QLabel* m_pFloatingPosYLabel;

   PMViewOptionsWidget* m_pCustomOptionsWidget;
   QWidget* m_pCustomOptionsHolder;
   
   int m_currentLayoutIndex;
   QList<PMViewLayout> m_viewLayouts;
   QList<PMViewLayout>::iterator m_currentViewLayout;
   QList<PMViewLayout>::iterator m_defaultViewLayout;
   QList<PMViewLayoutEntry>::iterator m_currentViewEntry;
};

#endif
