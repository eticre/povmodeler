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


#ifndef PMBLENDMAPMODIFIERSEDIT_H
#define PMBLENDMAPMODIFIERSEDIT_H



#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>

class PMBlendMapModifiers;
class QComboBox;
class PMFloatEdit;
class QLabel;
class QCheckBox;
class QWidget;

/**
 * Dialog edit class for @ref PMBlendMapModifiers.
 */
class PMBlendMapModifiersEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMBlendMapModifiersEdit with parent and name
    */
   PMBlendMapModifiersEdit( QWidget* parent );

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
    * Slot called whenever a new Wave Form type is selected.
    */
   void slotTypeComboChanged( int c );
   /**
    * Slot called when frequency is activated/deactivated.
    */
   void slotFrequencyClicked();
   /**
    * Slot called when phase is activated/deactivated.
    */
   void slotPhaseClicked();
   /**
    * Slot called when wave form is activated/deactivated.
    */
   void slotWaveFormClicked();

private:
   /**
    * Set's the combo box and enables/disables widgets.
    */
   void setBlendMapModifiersType( int i );
   PMBlendMapModifiers*     m_pDisplayedObject;

   QComboBox*     m_pWaveTypeCombo;
   PMFloatEdit*   m_pFrequencyEdit;
   PMFloatEdit*   m_pPhaseEdit;
   PMFloatEdit*   m_pWaveExponentEdit;
   QLabel*        m_pWaveExponentLabel;
   QCheckBox*     m_pEnableFrequencyEdit;
   QCheckBox*     m_pEnablePhaseEdit;
   QCheckBox*     m_pEnableWaveFormEdit;
};


#endif
