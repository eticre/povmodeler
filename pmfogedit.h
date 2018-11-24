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


#ifndef PMFOGEDIT_H
#define PMFOGEDIT_H



#include "pmtexturebaseedit.h"
//Added by qt3to4:
#include <QLabel>

class PMFog;
class PMVectorEdit;
class PMFloatEdit;
class PMIntEdit;
class PMColorEdit;
class QCheckBox;
class QComboBox;
class QWidget;
class QLabel;

/**
 * Dialog edit class for @ref PMFog
 */
class PMFogEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMFogEdit with parent and name
    */
   PMFogEdit( QWidget* parent );

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
    * Slot called whenever turbulence is activated/deactivated
    */
   void slotTurbulenceClicked();
   /**
    * Slot called whenever a new fog type is selected
    */
   void slotFogTypeChanged( int val );

private:
   PMFog* m_pDisplayedObject;

   QComboBox* m_pFogTypeEdit;
   PMFloatEdit* m_pDistance;
   PMColorEdit* m_pColor;
   QCheckBox* m_pTurbulenceCheck;
   QWidget* m_pTurbulenceWidget;
   PMVectorEdit* m_pTurbulenceVector;
   PMIntEdit* m_pOctaves;
   PMFloatEdit* m_pOmega;
   PMFloatEdit* m_pLambda;
   PMFloatEdit* m_pDepth;
   QLabel* m_pFogOffsetLabel;
   PMFloatEdit* m_pFogOffset;
   QLabel* m_pFogAltLabel;
   PMFloatEdit* m_pFogAlt;
   QLabel* m_pUpLabel;
   PMVectorEdit* m_pUp;
};


#endif
