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


#ifndef PMPHOTONSEDIT_H
#define PMPHOTONSEDIT_H



#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>

class PMPhotons;
class PMFloatEdit;
class QCheckBox;
class QLabel;

/**
 * Dialog edit class for @ref PMPhotons.
 */
class PMPhotonsEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMPhotonsEdit with parent and name
    */
   PMPhotonsEdit( QWidget* parent );

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
    * Slot Called whenever target is clicked
    */
   void slotTargetClicked();

private:
   PMPhotons* m_pDisplayedObject;

   QWidget* m_pLayoutWidget;
   QCheckBox* m_pTarget;
   PMFloatEdit* m_pSpacingMulti;
   QLabel* m_pSpacingMultiLabel;
   QCheckBox* m_pRefraction;
   QCheckBox* m_pReflection;
   QCheckBox* m_pCollect;
   QCheckBox* m_pPassThrough;
   QCheckBox* m_pAreaLight;
};


#endif
