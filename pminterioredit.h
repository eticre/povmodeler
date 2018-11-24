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


#ifndef PMINTERIOREDIT_H
#define PMINTERIOREDIT_H



#include "pmtexturebaseedit.h"
//Added by qt3to4:
#include <QLabel>

class PMInterior;
class PMIntEdit;
class PMFloatEdit;
class QCheckBox;
class QLabel;

/**
 * Dialog edit class for @ref PMInterior
 */
class PMInteriorEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMInteriorEdit with parent and name
    */
   PMInteriorEdit( QWidget* parent );

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
   void slotIorClicked();
   void slotCausticsClicked();
   void slotDispersionClicked();
   void slotDispSamplesClicked();
   void slotFadePowerClicked();
   void slotFadeDistanceClicked();

private:
   PMInterior* m_pDisplayedObject;
   PMFloatEdit* m_pIorEdit;
   PMFloatEdit* m_pCausticsEdit;
   PMFloatEdit* m_pDispersionEdit;
   PMIntEdit* m_pDispSamplesEdit;
   PMFloatEdit* m_pFadeDistanceEdit;
   PMFloatEdit* m_pFadePowerEdit;
   QCheckBox* m_pEnableIorEdit;
   QCheckBox* m_pEnableCausticsEdit;
   QCheckBox* m_pEnableDispersionEdit;
   QCheckBox* m_pEnableDispSamplesEdit;
   QCheckBox* m_pEnableFadeDistanceEdit;
   QCheckBox* m_pEnableFadePowerEdit;
};


#endif
