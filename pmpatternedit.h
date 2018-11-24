//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
    copyright            : (C) 2001 by Andreas Zehender
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


#ifndef PMPATTERNEDIT_H
#define PMPATTERNEDIT_H



#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>
#include "pmlinkedit.h"

class PMPattern;
class PMVectorEdit;
class QComboBox;
class PMFloatEdit;
class PMIntEdit;
class QLabel;
class QCheckBox;
class QWidget;
class QLineEdit;
class QPushButton;

/**
 * Dialog edit class for @ref PMPattern.
 */
class PMPatternEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Sets the linked object. Returns true if successful
    */
   bool setLinkedObject( PMDeclare* o );
   /**
    * Creates a PMPatternEdit with parent and name
    */
   PMPatternEdit( QWidget* parent );

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
    * Slot called whenever a new pattern type is selected.
    */
   void slotComboChanged( int c );
   /**
    * Slot called when the browse button is pressed for selection of the
    * density file
    */
   void slotDensityFileBrowseClicked();
   /**
    * Slot called when fractal magnet clicked
    */
   void slotFractalMagnetClicked();
   /**
    * Slot called when the slope altitude flag is clicked
    */
   void slotSlopeAltFlagClicked();
   /**
    * Slot called when the point_at flag is clicked
    */
   void slotPointAtFlagClicked();
   /**
    * Slot called when turbulence is activated/deactivated.
    */
   void slotTurbulenceClicked();

private:
   /**
    * Set's the combo box and enables/disables widgets.
    */
   void setPatternType( int i );
   /**
    * m_noDepth is false is the editor must show the depth field
    */
   bool m_noDepth;
   PMPattern*     m_pDisplayedObject;
   QComboBox*     m_pTypeCombo;

   QLabel*        m_pAgateTurbulenceLabel;
   PMFloatEdit*   m_pAgateTurbulenceEdit;

   QWidget*       m_pCrackleWidget;
   PMVectorEdit*  m_pCrackleForm;
   PMIntEdit*     m_pCrackleMetric;
   PMFloatEdit*   m_pCrackleOffset;
   QCheckBox*     m_pCrackleSolid;

   QWidget*       m_pDensityWidget;
   QComboBox*     m_pDensityInterpolate;
   QLineEdit*     m_pDensityFile;
   QPushButton*   m_pDensityFileBrowse;

   QLabel*        m_pGradientLabel;
   PMVectorEdit*  m_pGradientEdit;

   QLabel*        m_pJuliaComplexLabel;
   PMVectorEdit*  m_pJuliaComplex;
   QWidget*       m_pFractalWidget;
   QCheckBox*     m_pFractalMagnet;
   QComboBox*     m_pFractalMagnetType;
   PMIntEdit*     m_pMaxIterationsEdit;
   QLabel*        m_pFractalExponentLabel;
   PMIntEdit*     m_pFractalExponent;
   QComboBox*     m_pFractalExtType;
   PMFloatEdit*   m_pFractalExtFactor;
   QComboBox*     m_pFractalIntType;
   PMFloatEdit*   m_pFractalIntFactor;

   QLabel*        m_pQuiltControlsLabel;
   PMFloatEdit*   m_pQuiltControl0Edit;
   PMFloatEdit*   m_pQuiltControl1Edit;

   QWidget*       m_pSlopeWidget;
   PMVectorEdit*  m_pSlopeDirection;
   PMVectorEdit*  m_pPointAt;
   QCheckBox*     m_pPointAtFlag;
   PMFloatEdit*   m_pSlopeLoSlope;
   PMFloatEdit*   m_pSlopeHiSlope;
   QCheckBox*     m_pSlopeAltFlag;
   PMVectorEdit*  m_pSlopeAltitude;
   QLabel*        m_pSlopeLoAltLabel;
   PMFloatEdit*   m_pSlopeLoAlt;
   QLabel*        m_pSlopeHiAltLabel;
   PMFloatEdit*   m_pSlopeHiAlt;

   QLabel*        m_pSpiralNumberLabel;
   PMIntEdit*     m_pSpiralNumberEdit;

   QLabel*        m_pTilingNumberLabel;
   PMIntEdit*     m_pTilingNumberEdit;

   QLabel*        m_pPigmentLabel;
   PMLinkEdit*    m_pPigmentPatternEdit;

   QLabel*        m_pNoiseGeneratorLabel;
   QComboBox*     m_pNoiseGenerator;

   QCheckBox*     m_pEnableTurbulenceEdit;
   QWidget*       m_pTurbulenceWidget;
   PMVectorEdit*  m_pValueVectorEdit;
   PMIntEdit*     m_pOctavesEdit;
   PMFloatEdit*   m_pOmegaEdit;
   PMFloatEdit*   m_pLambdaEdit;

   QLabel*        m_pDepthLabel;
   PMFloatEdit*   m_pDepthEdit;
};


#endif
