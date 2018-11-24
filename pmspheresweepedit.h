//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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


#ifndef PMSPHERESWEEPEDIT_H
#define PMSPHERESWEEPEDIT_H



#include "pmsolidobjectedit.h"
#include "pmvectoredit.h"

class PMSphereSweep;
class PMVectorListEdit;
class PMFloatEdit;
class QVBoxLayout;
class QComboBox;
class QPushButton;
class QLabel;

/**
 * Dialog edit class for @ref PMSphereSweep
 */
class PMSphereSweepEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMSphereSweepEdit with parent and name
    */
   PMSphereSweepEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );
   /** */
   virtual void updateControlPointSelection();

   /** */
   virtual bool isDataValid();

protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void createBottomWidgets();
   /** */
   virtual void saveContents();

protected slots:
   void slotTypeChanged( int );
   void slotAddPointAbove();
   void slotAddPointBelow();
   void slotRemovePoint();
   void slotSelectionChanged();

private:
   void updatePointButtons();

   PMSphereSweep* m_pDisplayedObject;
   QComboBox* m_pSplineType;
   PMVectorListEdit* m_pPoints;
   PMFloatEdit* m_pTolerance;
   QPushButton* m_pAddAbove;
   QPushButton* m_pAddBelow;
   QPushButton* m_pRemove;
};


#endif
