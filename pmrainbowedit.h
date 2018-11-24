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


#ifndef PMRAINBOWEDIT_H
#define PMRAINBOWEDIT_H



#include "pmtexturebaseedit.h"
//Added by qt3to4:
#include <QLabel>

class PMRainbow;
class PMFloatEdit;
class PMVectorEdit;
class QCheckBox;
class QLabel;

/**
 * Dialog edit class for @ref PMRainbow
 */
class PMRainbowEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMRainbowEdit with parent and name
    */
   PMRainbowEdit( QWidget* parent );

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
   void slotDirectionClicked();
   void slotAngleClicked();
   void slotWidthClicked();
   void slotDistanceClicked();
   void slotJitterClicked();
   void slotUpClicked();
   void slotArcAngleClicked();
   void slotFalloffAngleClicked();

private:
   PMRainbow* m_pDisplayedObject;
   PMVectorEdit* m_pDirectionEdit; 
   PMFloatEdit* m_pAngleEdit;
   PMFloatEdit* m_pWidthEdit;
   PMFloatEdit* m_pDistanceEdit;
   PMFloatEdit* m_pJitterEdit;
   PMVectorEdit* m_pUpEdit;
   PMFloatEdit* m_pArcAngleEdit;
   PMFloatEdit* m_pFalloffAngleEdit;
   QCheckBox* m_pEnableDirectionEdit;
   QCheckBox* m_pEnableAngleEdit;
   QCheckBox* m_pEnableWidthEdit;
   QCheckBox* m_pEnableDistanceEdit;
   QCheckBox* m_pEnableJitterEdit;
   QCheckBox* m_pEnableUpEdit;
   QCheckBox* m_pEnableArcAngleEdit;
   QCheckBox* m_pEnableFalloffAngleEdit;
};


#endif
