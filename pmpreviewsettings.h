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

#ifndef PMPREVIEWSETTINGS_H
#define PMPREVIEWSETTINGS_H



#include "pmsettingsdialog.h"

class PMIntEdit;
class PMFloatEdit;
class QCheckBox;
class QPushButton;

/**
 * Texture preview configuration dialog page
 */
class PMPreviewSettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMPreviewSettings( QWidget* parent = 0 );
   /** */
   virtual void displaySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void applySettings();
   /** */
   virtual void displayDefaults();

   QColor getStyleColor( QString str )
   {
       str.remove( 0, 12 );
       str.remove( " " );
       return QColor ( str );
   }

private:
   PMIntEdit* m_pPreviewSize;
   QCheckBox* m_pPreviewSphere;
   QCheckBox* m_pPreviewCylinder;
   QCheckBox* m_pPreviewBox;
   QCheckBox* m_pPreviewAA;
   PMIntEdit* m_pPreviewAALevel;
   PMFloatEdit* m_pPreviewAAThreshold;
   QCheckBox* m_pPreviewWall;
   QCheckBox* m_pPreviewFloor;
   QPushButton* m_pFloorColor1;
   QPushButton* m_pFloorColor2;
   QPushButton* m_pWallColor1;
   QPushButton* m_pWallColor2;
   PMFloatEdit* m_pPreviewGamma;
};

#endif
