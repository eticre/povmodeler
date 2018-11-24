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


#ifndef PMMEDIAEDIT_H
#define PMMEDIAEDIT_H



#include "pmtexturebaseedit.h"
//Added by qt3to4:
#include <QLabel>

class PMMedia;
class PMIntEdit;
class PMFloatEdit;
class PMColorEdit;
class QCheckBox;
class QLabel;
class QComboBox;

/**
 * Dialog edit class for @ref PMMedia
 */
class PMMediaEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMMediaEdit with parent and name
    */
   PMMediaEdit( QWidget* parent );

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
   void slotMethodChanged( int c );
   void slotAbsorptionClicked();
   void slotEmissionClicked();
   void slotScatteringClicked();
   void slotScatteringTypeChanged( int c );

private:
   PMMedia* m_pDisplayedObject;

   QComboBox* m_pMethodEdit;
   PMIntEdit* m_pIntervalsEdit;
   PMIntEdit* m_pSamplesMinEdit;
   QLabel* m_pSamplesMaxLabel;
   PMIntEdit* m_pSamplesMaxEdit;
   PMFloatEdit* m_pConfidenceEdit;
   PMFloatEdit* m_pVarianceEdit;
   PMFloatEdit* m_pRatioEdit;

   QWidget* m_pAAWidget;
   PMIntEdit* m_pAALevelEdit;
   PMFloatEdit* m_pAAThresholdEdit;

   QCheckBox* m_pEnableAbsorptionEdit;
   PMColorEdit* m_pAbsorptionEdit;
   QLabel* m_pAbsorptionLabel;

   QCheckBox* m_pEnableEmissionEdit;
   PMColorEdit* m_pEmissionEdit;
   QLabel* m_pEmissionLabel;

   QCheckBox* m_pEnableScatteringEdit;
   QWidget* m_pScatteringWidget;
   QComboBox* m_pScatteringTypeEdit;
   PMColorEdit* m_pScatteringColorEdit;
   QLabel* m_pScatteringEccentricityLabel;
   PMFloatEdit* m_pScatteringEccentricityEdit;
   PMFloatEdit* m_pScatteringExtinctionEdit;
};


#endif
