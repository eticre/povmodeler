/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
    email                : zehender@kde.org
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMOBJECTLIBRARYSETTINGS_H
#define PMOBJECTLIBRARYSETTINGS_H



#include "pmsettingsdialog.h"
//Added by qt3to4:
#include <QLabel>

class QPushButton;
class Q3ListBox;

/**
 * Object library configuration dialog page
 */
class PMObjectLibrarySettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMObjectLibrarySettings( QWidget* parent = 0 );
   /** */
   virtual void displaySettings();
   /** */
   virtual void applySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void displayDefaults();
protected slots:
   /** Called when the object library create button is clicked */
   void slotCreateObjectLibrary();
   /** Called when the object library properties button is clicked */
   void slotModifyObjectLibrary();
   /** Called when the object library remove button is clicked */
   void slotRemoveObjectLibrary();
   /** Called when the object library import button is clicked */
   void slotImportObjectLibrary();
   /** Called when the selected library is changed */
   void slotObjectLibraryChanged();
private:
   int m_objectLibraryIndex;
   int m_selectionIndex;
   Q3ListBox* m_pObjectLibraries;
   QPushButton* m_pCreateObjectLibrary;
   QPushButton* m_pModifyObjectLibrary;
   QPushButton* m_pRemoveObjectLibrary;
   QPushButton* m_pImportObjectLibrary;

   QLabel* m_pLibraryPath;
   QLabel* m_pLibraryAuthor;
   QLabel* m_pLibraryDescription;
   QLabel* m_pReadOnlyText;
};


#endif
