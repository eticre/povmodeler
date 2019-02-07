//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2006 by Andreas Zehender
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

#ifndef PMRENDERMODESDIALOG_H
#define PMRENDERMODESDIALOG_H

#include "pmrendermode.h"
#include <QDialog>
//Added by qt3to4:
#include <QResizeEvent>
#include <QPushButton>
#include <QDialogButtonBox>

class QListWidget;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QTabWidget;
class PMIntEdit;
class PMFloatEdit;

/**
 * Dialog for editing a list of render modes.
 * @see PMRenderMode
 */
class PMRenderModesDialog : public QDialog
{
   Q_OBJECT
public:
   /**
    * Creates a dialog for the modes list
    */
   PMRenderModesDialog( const PMRenderModeList& modes, int index, QWidget* parent = 0 );
   QDialogButtonBox* buttonBox;
   static void saveConfig();
   static void restoreConfig();

	/**
	 * Returns the last selected mode
	 */
	int activeRenderMode() const { return m_selectionIndex; }
	/**
	 * Returns the configured modes
	 */
	PMRenderModeList renderModes() const { return m_workingModes; }
	
protected slots:
   /**
    * Called when a mode is selected in the list view
    */
   void slotModeSelected( int index );

   /**
    * Called when the add button is klicked
    */
   void slotAdd();
   /**
    * Called when the remove button is klicked
    */
   void slotRemove();
   /**
    * Called when the up button is klicked
    */
   void slotUp();
   /**
    * Called when the down button is klicked
    */
   void slotDown();
   /**
    * Called when the edit button is klicked
    */
   void slotEdit();
   /**
    * Called when the modes are changed
    */
   void slotChanged();
   virtual void slotOk();

protected:
   virtual void resizeEvent( QResizeEvent* ev );

private:
   void displayList();
   void checkButtons();

   PMRenderModeList m_workingModes;
   int m_selectionIndex;

   QListWidget* m_pListBox;
   QPushButton* m_pAddButton;
   QPushButton* m_pRemoveButton;
   QPushButton* m_pUpButton;
   QPushButton* m_pDownButton;
   QPushButton* m_pEditButton;
   static QSize s_size;
};

/**
 * Dialog for editing one render mode
 * @see PMRenderMode
 */
class PMRenderModeDialog : public QDialog
{
   Q_OBJECT
public:
   /**
    * Creates a dialog for the mode
    */
   PMRenderModeDialog( const PMRenderMode& mode, QWidget* parent = 0 );
	/**
	 * Returns the configured mode
	 */
   PMRenderMode mode() const { return m_mode; }
   QDialogButtonBox* modeButtonBox;
   static void saveConfig();
   static void restoreConfig();
   void setButton( QPushButton* btn, bool b );

protected:
   virtual void resizeEvent( QResizeEvent* ev );


protected slots:
   virtual void slotOk();
   void slotChanged();
   void slotTextChanged( const QString& );
   void slotActivated( int );
   void slotSubsectionToggled( bool );
   void slotAntialiasingToggled( bool );
   void slotJitterToggled( bool );
   void slotToggled( bool );

private:
   /**
    * Saves the current changes. Returns true if successful.
    */
   bool saveChanges();
   /**
    * Returns true if the data is valid
    */
   bool validate();
   /**
    * Displays the selected mode
    */
   void displayMode();

   void enableSubsection( bool yes );
   void enableAntialiasing( bool yes );
   void enableJitter( bool yes );
   int qualityToIndex( int quality );
   int indexToQuality( int index );

   PMRenderMode m_mode;

   QTabWidget* m_pTabWidget;
   QLineEdit* m_pDescriptionEdit;
   PMIntEdit* m_pHeightEdit;
   PMIntEdit* m_pWidthEdit;
   QCheckBox* m_pSubsectionBox;
   PMFloatEdit* m_pStartRowEdit;
   PMFloatEdit* m_pEndRowEdit;
   PMFloatEdit* m_pStartColumnEdit;
   PMFloatEdit* m_pEndColumnEdit;
   // quality
   QComboBox* m_pQualityCombo;
   QCheckBox* m_pRadiosityBox;
   QCheckBox* m_pAntialiasingBox;
   QComboBox* m_pSamplingCombo;
   PMFloatEdit* m_pThresholdEdit;
   QCheckBox* m_pJitterBox;
   PMFloatEdit* m_pJitterAmountEdit;
   PMIntEdit* m_pAntialiasDepthEdit;
   // output
   QCheckBox* m_pAlphaBox;

   static QSize s_size;
};

#endif
