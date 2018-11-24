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

#ifndef PMGRIDSETTINGS_H
#define PMGRIDSETTINGS_H



#include "pmsettingsdialog.h"

class QPushButton;
class PMIntEdit;
class PMFloatEdit;

/**
 * Grid configuration dialog page
 */
class PMGridSettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMGridSettings( QWidget* parent = nullptr );
   /** */
   virtual void displaySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void applySettings();
   /** */
   virtual void displayDefaults();
   
private:
   void selectColor();
   PMIntEdit* m_pGridDistance;
   QPushButton* m_pGridColor;
   PMFloatEdit* m_pMoveGrid;
   PMFloatEdit* m_pScaleGrid;
   PMFloatEdit* m_pRotateGrid;
};


#endif
