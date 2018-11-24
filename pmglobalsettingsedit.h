//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMGLOBALSETTINGSEDIT_H
#define PMGLOBALSETTINGSEDIT_H



#include "pmdialogeditbase.h"

class PMGlobalSettings;
class PMColorEdit;
class PMFloatEdit;
class PMIntEdit;
class QCheckBox;
class QComboBox;

/**
 * Dialog edit class for @ref PMGlobalSettings.
 */
class PMGlobalSettingsEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMGlobalSettingsEdit with parent and name
    */
   PMGlobalSettingsEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();

protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

protected slots:
   /**
    * Slot called whenever Radiosity is toggled
    */
   void slotRadiosityClicked();

private:
   PMGlobalSettings* m_pDisplayedObject;

   PMFloatEdit* m_pAdcBailoutEdit;
   PMColorEdit* m_pAmbientLightEdit;
   PMFloatEdit* m_pAssumedGammaEdit;
   QCheckBox*   m_pHfGray16Edit;
   PMColorEdit* m_pIridWaveLengthEdit;
   PMIntEdit*   m_pMaxIntersectionsEdit;
   PMIntEdit*   m_pMaxTraceLevelEdit;
   PMIntEdit*   m_pNumberWavesEdit;
   QComboBox*   m_pNoiseGeneratorEdit;
   QCheckBox*   m_pRadiosityEdit;
   QWidget*     m_pRadiosityWidget;
   PMFloatEdit* m_pBrightnessEdit;
   PMIntEdit*   m_pCountEdit;
   PMFloatEdit* m_pDistanceMaximumEdit;
   PMFloatEdit* m_pErrorBoundEdit;
   PMFloatEdit* m_pGrayThresholdEdit;
   PMFloatEdit* m_pLowErrorFactorEdit;
   PMFloatEdit* m_pMinimumReuseEdit;
   PMIntEdit*   m_pNearestCountEdit;
   PMIntEdit*   m_pRecursionLimitEdit;
};


#endif
