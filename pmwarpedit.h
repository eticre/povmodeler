//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
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


#ifndef PMWARPEDIT_H
#define PMWARPEDIT_H



#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>

class PMWarp;
class PMVectorEdit;
class PMIntEdit;
class PMFloatEdit;
class QComboBox;
class QCheckBox;
class QLabel;

/**
 * Dialog edit class for @ref PMWarp.
 */
class PMWarpEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMWarpEdit with parent and name
    */
   PMWarpEdit( QWidget* parent );

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
   /** */
   void slotComboChanged( int c );

private:
   PMWarp* m_pDisplayedObject;
   QComboBox*    m_pWarpTypeEdit;
   PMVectorEdit* m_pDirectionEdit;
   QLabel*       m_pDirectionLabel;
   PMVectorEdit* m_pOffsetEdit;
   QLabel*       m_pOffsetLabel;
   PMVectorEdit* m_pFlipEdit;
   QLabel*       m_pFlipLabel;

   PMVectorEdit* m_pLocationEdit;
   QLabel*       m_pLocationLabel;
   PMFloatEdit*  m_pRadiusEdit;
   QLabel*       m_pRadiusLabel;
   PMFloatEdit*  m_pStrengthEdit;
   QLabel*       m_pStrengthLabel;
   PMFloatEdit*  m_pFalloffEdit;
   QLabel*       m_pFalloffLabel;
   QCheckBox*    m_pInverseEdit;
   PMVectorEdit* m_pRepeatEdit;
   QLabel*       m_pRepeatLabel;
   PMVectorEdit* m_pTurbulenceEdit;
   QLabel*       m_pTurbulenceLabel;

   PMVectorEdit* m_pValueVectorEdit;
   QLabel*       m_pValueVectorLabel;
   PMIntEdit*    m_pOctavesEdit;
   QLabel*       m_pOctavesLabel;
   PMFloatEdit*  m_pOmegaEdit;
   QLabel*       m_pOmegaLabel;
   PMFloatEdit*  m_pLambdaEdit;
   QLabel*       m_pLambdaLabel;

   PMVectorEdit* m_pOrientationEdit;
   PMFloatEdit*  m_pDistExpEdit;
   PMFloatEdit*  m_pMajorRadiusEdit;
   QLabel*       m_pMajorRadiusLabel;

   QWidget* m_pRepeatWidget;
   QWidget* m_pBlackHoleWidget;
   QWidget* m_pTurbulenceWidget;
   QWidget* m_pMappingWidget;
};


#endif
