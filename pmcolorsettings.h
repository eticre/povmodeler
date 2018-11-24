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

#ifndef PMCOLORSETTINGS_H
#define PMCOLORSETTINGS_H



#include "pmsettingsdialog.h"
#include <QPushButton>

/**
 * Colors configuration dialog page
 */
class PMColorSettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMColorSettings( QWidget* parent = 0 );
   /** */
   virtual void displaySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void applySettings();
   /** */
   virtual void displayDefaults();

private slots:
   void selectColor( QPushButton *b );
   
private:
   QColor getStyleColor( QString str );
   QPushButton* m_pBackgroundColor;
   QPushButton* m_pGraphicalObjectsColor[2];
   QPushButton* m_pControlPointsColor[2];
   QPushButton* m_pAxesColor[3];
   QPushButton* m_pFieldOfViewColor;

signals:
    void btn( QPushButton* sentBtn );


};


#endif
