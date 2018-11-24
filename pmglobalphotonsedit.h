//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMGLOBALPHOTONSEDIT_H
#define PMGLOBALPHOTONSEDIT_H



#include "pmdialogeditbase.h"

class PMGlobalPhotons;
class PMFloatEdit;
class PMIntEdit;
class QComboBox;
class QCheckBox;

/**
 * Dialog edit class for @ref PMGlobalPhotons.
 */
class PMGlobalPhotonsEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMGlobalPhotonsEdit with parent and name
    */
   PMGlobalPhotonsEdit( QWidget* parent );

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
    * Slot Called whenever NumberType is altered
    */
   void slotNumberTypeActivated( int index );
   /**
    * slot Called whenever MaxTraceLevelGlobal is Clicked
    */
   void slotMaxTraceLevelGlobalClicked();
   /**
    * slot Called whenever AdcBailoutGlobal is Clicked
    */
   void slotAdcBailoutGlobalClicked();

private:
   PMGlobalPhotons* m_pDisplayedObject;

   QComboBox* m_pNumberType;
   PMFloatEdit* m_pSpacing;
   PMIntEdit* m_pCount;
   PMIntEdit* m_pGatherMin;
   PMIntEdit* m_pGatherMax;
   PMIntEdit* m_pMediaMaxSteps;
   PMFloatEdit* m_pMediaFactor;
   PMFloatEdit* m_pJitter;
   QCheckBox* m_pMaxTraceLevelGlobal;
   PMIntEdit* m_pMaxTraceLevel;
   QCheckBox* m_pAdcBailoutGlobal;
   PMFloatEdit* m_pAdcBailout;
   PMFloatEdit* m_pAutostop;
   PMFloatEdit* m_pExpandIncrease;
   PMIntEdit* m_pExpandMin;
   PMFloatEdit* m_pRadiusGather;
   PMFloatEdit* m_pRadiusGatherMulti;
   PMFloatEdit* m_pRadiusMedia;
   PMFloatEdit* m_pRadiusMediaMulti;
};


#endif
