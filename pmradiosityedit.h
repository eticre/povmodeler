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


#ifndef PMRADIOSITYEDIT_H
#define PMRADIOSITYEDIT_H



#include "pmdialogeditbase.h"

class PMRadiosity;
class PMFloatEdit;
class PMIntEdit;
class QCheckBox;

/**
 * Dialog edit class for @ref PMRadiosity.
 */
class PMRadiosityEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMRadiosityEdit with parent and name
    */
   PMRadiosityEdit( QWidget* parent );

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

private:
   PMRadiosity* m_pDisplayedObject;

   PMFloatEdit* m_pAdcBailout;
   QCheckBox* m_pAlwaysSample;
   PMFloatEdit* m_pBrightness;
   PMIntEdit* m_pCount;
   PMFloatEdit* m_pErrorBound;
   PMFloatEdit* m_pGrayThreshold;
   PMFloatEdit* m_pLowErrorFactor;
   PMFloatEdit* m_pMaxSample;
   QCheckBox* m_pMedia;
   PMFloatEdit* m_pMinimumReuse;
   PMIntEdit* m_pNearestCount;
   QCheckBox* m_pNormal;
   PMFloatEdit* m_pPretraceStart;
   PMFloatEdit* m_pPretraceEnd;
   PMIntEdit* m_pRecursionLimit;
};


#endif
