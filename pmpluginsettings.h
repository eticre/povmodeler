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

#ifndef PMPLUGINSETTINGS_H
#define PMPLUGINSETTINGS_H



#include "pmsettingsdialog.h"

class QFrame;
class QTableWidget;
class QPushButton;

/**
 * Plugins configuration dialog page
 */
class PMPluginSettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMPluginSettings( QWidget* parent = 0 );
   /** */
   virtual void displaySettings();
   /** */
   virtual void applySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void displayDefaults();
   
protected slots:
   void slotToggle();
   void slotSelectionChanged();
   
private:
   QFrame* m_pPluginOptions;
   QTableWidget* m_pPluginsList;
   QPushButton* m_pToggle;
};


#endif
