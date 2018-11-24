//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMSOREDIT_H
#define PMSOREDIT_H



#include "pmsolidobjectedit.h"
#include "pmvectoredit.h"
#include <QList>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>

class PMSurfaceOfRevolution;
class QVBoxLayout;
class QCheckBox;
class QPushButton;
class QLabel;
class PMVectorListEdit;

/**
 * Dialog edit class for @ref PMSurfaceOfRevolution
 */
class PMSurfaceOfRevolutionEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMSurfaceOfRevolutionEdit with parent and name
    */
   PMSurfaceOfRevolutionEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );
   /** */
   virtual void updateControlPointSelection();

   /** */
   virtual bool isDataValid();
   
protected:
   /** */
   virtual void createBottomWidgets();
   /** */
   virtual void saveContents();

protected slots:
   void slotAddPointAbove();
   void slotAddPointBelow();
   void slotRemovePoint();
   void slotSelectionChanged();
   
private:
   void updatePointButtons();
   
   PMSurfaceOfRevolution* m_pDisplayedObject;
   PMVectorListEdit* m_pPoints;
   QCheckBox* m_pOpen;
   QCheckBox* m_pSturm;
   QPushButton* m_pAddAbove;
   QPushButton* m_pAddBelow;
   QPushButton* m_pRemove;   
};


#endif
