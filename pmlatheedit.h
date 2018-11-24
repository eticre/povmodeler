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


#ifndef PMLATHEEDIT_H
#define PMLATHEEDIT_H



#include "pmsolidobjectedit.h"
#include "pmvectoredit.h"
#include <QList>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>

class PMLathe;
class PMVectorListEdit;
class QVBoxLayout;
class QComboBox;
class QCheckBox;
class QPushButton;
class QLabel;

/**
 * Dialog edit class for @ref PMLathe
 */
class PMLatheEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMLatheEdit with parent and name
    */
   PMLatheEdit( QWidget* parent );

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
   
   PMLathe* m_pDisplayedObject;
   QComboBox* m_pSplineType;
   QCheckBox* m_pSturm;
   PMVectorListEdit* m_pPoints;
   QPushButton* m_pAddAbove;
   QPushButton* m_pAddBelow;
   QPushButton* m_pRemove;   
};


#endif
