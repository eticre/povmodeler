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

#ifndef PMOBJECTSETTINGS_H
#define PMOBJECTSETTINGS_H



#include "pmsettingsdialog.h"

class PMIntEdit;
class PMFloatEdit;
class QCheckBox;

/**
 * Object details configuration dialog page
 */
class PMObjectSettings : public PMSettingsDialogPage
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMObjectSettings( QWidget* parent = 0 );
   /** */
   virtual void displaySettings();
   /** */
   virtual bool validateData();
   /** */
   virtual void applySettings();
   /** */
   virtual void displayDefaults();

private:
   PMIntEdit* m_pSphereUSteps;
   PMIntEdit* m_pSphereVSteps;
   PMIntEdit* m_pCylinderSteps;
   PMIntEdit* m_pConeSteps;
   PMIntEdit* m_pTorusUSteps;
   PMIntEdit* m_pTorusVSteps;
   PMFloatEdit* m_pPlaneSize;
   PMIntEdit* m_pDiscSteps;
   PMIntEdit* m_pBlobSphereUSteps;
   PMIntEdit* m_pBlobSphereVSteps;
   PMIntEdit* m_pBlobCylinderUSteps;
   PMIntEdit* m_pBlobCylinderVSteps;
   PMIntEdit* m_pLatheUSteps;
   PMIntEdit* m_pLatheRSteps;
   PMIntEdit* m_pSorUSteps;
   PMIntEdit* m_pSorRSteps;
   PMIntEdit* m_pPrismSteps;
   PMIntEdit* m_pSqeUSteps;
   PMIntEdit* m_pSqeVSteps;
   PMIntEdit* m_pSphereSweepRSteps;
   PMIntEdit* m_pSphereSweepSSteps;
   PMIntEdit* m_pHeightFieldVariance;
   QCheckBox* m_pHighDetailCameraViews;
};


#endif
