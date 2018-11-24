//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
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


#ifndef PMFINISHEDIT_H
#define PMFINISHEDIT_H



#include "pmtexturebaseedit.h"
//Added by qt3to4:
#include <QLabel>

class PMFinish;
class PMFloatEdit;
class PMColorEdit;
class QCheckBox;
class QLabel;
class QWidget;

/**
 * Dialog edit class for @ref PMFinish
 */
class PMFinishEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMFinishEdit with parent and name
    */
   PMFinishEdit( QWidget* parent );

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
   void slotIridClicked();
   void slotEmissionClicked();
   void slotAmbientClicked();
   void slotPhongClicked();
   void slotPhongSizeClicked();
   void slotBrillianceClicked();
   void slotDiffuseClicked();
   void slotMetallicClicked();
   void slotCrandClicked();
   void slotSpecularClicked();
   void slotRoughnessClicked();
   void slotReflectionClicked();
   void slotReflectionMinClicked();
   void slotRefFalloffClicked();
   void slotRefExponentClicked();
   void slotRefMetallicClicked();

private:
   PMFinish* m_pDisplayedObject;
   PMColorEdit* m_pEmissionColorEdit;
   QLabel* m_pEmissionColorLabel;
   PMColorEdit* m_pAmbientColorEdit;
   QLabel* m_pAmbientColorLabel;
   PMFloatEdit* m_pDiffuseEdit;
   PMFloatEdit* m_pBrillianceEdit;
   PMFloatEdit* m_pCrandEdit;
   QCheckBox* m_pConserveEnergyEdit;
   PMFloatEdit* m_pPhongEdit;
   PMFloatEdit* m_pPhongSizeEdit;
   PMFloatEdit* m_pMetallicEdit;
   PMFloatEdit* m_pSpecularEdit;
   PMFloatEdit* m_pRoughnessEdit;
   QCheckBox* m_pIridEdit;
   PMFloatEdit* m_pIridAmountEdit;
   PMFloatEdit* m_pIridThicknessEdit;
   PMFloatEdit* m_pIridTurbulenceEdit;
   QLabel* m_pIridAmountLabel;
   QLabel* m_pIridThicknessLabel;
   QLabel* m_pIridTurbulenceLabel;
   QCheckBox* m_pEnableEmissionEdit;
   QCheckBox* m_pEnableAmbientEdit;
   QCheckBox* m_pEnablePhongEdit;
   QCheckBox* m_pEnablePhongSizeEdit;
   QCheckBox* m_pEnableDiffuseEdit;
   QCheckBox* m_pEnableBrillianceEdit;
   QCheckBox* m_pEnableCrandEdit;
   QCheckBox* m_pEnableSpecularEdit;
   QCheckBox* m_pEnableRoughnessEdit;
   QCheckBox* m_pEnableMetallicEdit;

   QWidget* m_pReflectionWidget;
   PMColorEdit* m_pReflectionColorEdit;
   PMColorEdit* m_pReflectionMinColorEdit;
   QCheckBox* m_pReflectionFresnelEdit;
   PMFloatEdit* m_pReflectionFalloffEdit;
   PMFloatEdit* m_pReflectionExponentEdit;
   PMFloatEdit* m_pReflectionMetallicEdit;
   QCheckBox* m_pEnableReflectionEdit;
   QCheckBox* m_pEnableReflectionMinEdit;
   QCheckBox* m_pEnableRefFalloffEdit;
   QCheckBox* m_pEnableRefExponentEdit;
   QCheckBox* m_pEnableRefMetallicEdit;
};


#endif
