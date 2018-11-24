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

#ifndef PMPOVRAYSETTINGS_H
#define PMPOVRAYSETTINGS_H



#include "pmsettingsdialog.h"

class QLineEdit;
class QPushButton;
class QComboBox;
class QListWidget;

/**
 * Povray configuration dialog page
 */
class PMPovraySettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMPovraySettings( QWidget* parent = 0 );
   /** */
   virtual void displaySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void applySettings();
   /** */
   virtual void displayDefaults();
   
protected slots:
   void slotAddPath();
   /**
    * Called when the remove button is clicked
    */
   void slotRemovePath();
   /**
    * Called when the up button is clicked
    */
   void slotPathUp();
   /**
    * Called when the down button is clicked
    */
   void slotPathDown();
   /**
    * Called when the edit button is clicked
    */
   void slotEditPath();
   /**
    * Called when a path is selected in the list view
    */
   void slotPathSelected( int index );
   /**
    * Called when the browse button for the povray command is clicked
    */
   void slotBrowsePovrayCommand();
   /**
    * Called when the browse button for the povray documentation is clicked
    */
   void slotBrowsePovrayDocumentation();
   
private:
   QLineEdit* m_pPovrayCommand;
   QPushButton* m_pBrowsePovrayCommand;
   QLineEdit* m_pDocumentationPath;
   QPushButton* m_pBrowseDocumentationPath;
   QComboBox* m_pDocumentationVersion;
   QListWidget* m_pLibraryPaths;
   QPushButton* m_pAddLibraryPath;
   QPushButton* m_pRemoveLibraryPath;
   QPushButton* m_pChangeLibraryPath;
   QPushButton* m_pLibraryPathUp;
   QPushButton* m_pLibraryPathDown;
   int m_selectionIndex;
};

#endif
