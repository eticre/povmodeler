//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMLIGHTEDIT_H
#define PMLIGHTEDIT_H



#include "pmnamedobjectedit.h"
//Added by qt3to4:
#include <QLabel>

class PMLight;
class PMVectorEdit;
class PMColorEdit;
class QComboBox;
class PMFloatEdit;
class PMIntEdit;
class QLabel;
class QCheckBox;

/**
 * Dialog edit class for @ref PMLight
 */
class PMLightEdit : public PMNamedObjectEdit
{
   Q_OBJECT
   typedef PMNamedObjectEdit Base;
public:
   /**
    * Creates a PMLightEdit with parent and name
    */
   PMLightEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();
public slots:
   void slotTypeActivated( int index );
   void slotAreaClicked();
   void slotOrientCheck();
   void slotFadingClicked();
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

private:
   /**
    * Returns true if orient should be enabled
    */
   bool orientEnabled( bool readOnly );

   PMLight* m_pDisplayedObject;
   PMVectorEdit* m_pLocation;
   PMColorEdit* m_pColor;
   QComboBox* m_pType;
   
   PMFloatEdit* m_pRadius;
   QLabel* m_pRadiusLabel;
   PMFloatEdit* m_pFalloff;
   QLabel* m_pFalloffLabel;
   PMFloatEdit* m_pTightness;
   QLabel* m_pTightnessLabel;
   
   PMVectorEdit* m_pPointAt;
   QLabel* m_pPointAtLabel;
   QCheckBox* m_pParallel;

   QCheckBox* m_pAreaLight;
   QLabel* m_pAreaTypeLabel;
   QComboBox* m_pAreaType;
   PMVectorEdit* m_pAxis1;
   PMIntEdit* m_pSize1;
   PMVectorEdit* m_pAxis2;
   PMIntEdit* m_pSize2;
   QLabel* m_pAxis1Label;
   QLabel* m_pAxis2Label;
   QLabel* m_pSize1Label;
   QLabel* m_pSize2Label;
   PMIntEdit* m_pAdaptive;
   QLabel* m_pAdaptiveLabel;
   QCheckBox* m_pOrient;
   QCheckBox* m_pJitter;

   QCheckBox* m_pFading;
   PMFloatEdit* m_pFadeDistance;
   QLabel* m_pFadeDistanceLabel;
   PMIntEdit* m_pFadePower;
   QLabel* m_pFadePowerLabel;
   QCheckBox* m_pMediaInteraction;
   QCheckBox* m_pMediaAttenuation;
};


#endif
