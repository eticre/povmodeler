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


#ifndef PMPRISMEDIT_H
#define PMPRISMEDIT_H



#include "pmsolidobjectedit.h"
#include "pmvectoredit.h"
#include <QList>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>


class PMPrism;
class PMFloatEdit;
class PMVectorListEdit;
class QVBoxLayout;
class QComboBox;
class QCheckBox;
class QPushButton;
class QLabel;

/**
 * Dialog edit class for @ref PMPrism
 */
class PMPrismEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMPrismEdit with parent and name
    */
   PMPrismEdit( QWidget* parent );
   /**
    * Destructor
    */
   virtual ~PMPrismEdit();
   /** */
   virtual void displayObject( PMObject* o );
   /** */
   void updateControlPointSelection();

   /** */
   virtual bool isDataValid();
   
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void createBottomWidgets();
   /** */
   virtual void saveContents();

private:
   /**
    * Displays the spline points
    */
   void displayPoints( const QList< QList<PMVector> >& list );
   /**
    * Returns the spline points from the vector edits
    */
   QList< QList<PMVector> > splinePoints();
   /**
    * Deletes the spline point edits
    */
   void deleteEdits();
   /**
    * Creates the edits for the points
    */
   void createEdits( const QList< QList<PMVector> >& points );

protected slots:
   void slotTypeChanged( int );
   void slotSweepChanged( int );
   void slotAddSubPrism();
   void slotRemoveSubPrism();
   void slotAddPointAbove();
   void slotAddPointBelow();
   void slotRemovePoint();
   void slotSelectionChanged();
   
private:   
   PMPrism* m_pDisplayedObject;
   QList< QLabel* > m_labels;
   QList< QPushButton* > m_subPrismAddButtons;
   QList< QPushButton* > m_subPrismRemoveButtons;
   QList< QPushButton* > m_addAboveButtons;
   QList< QPushButton* > m_addBelowButtons;
   QList< QPushButton* > m_removeButtons;
   QList< PMVectorListEdit* > m_points;
   QWidget* m_pEditWidget;
   QComboBox* m_pSplineType;
   QComboBox* m_pSweepType;
   QCheckBox* m_pSturm;
   QCheckBox* m_pOpen;
   PMFloatEdit* m_pHeight1;
   PMFloatEdit* m_pHeight2;
   int m_lastSplineType;
};


#endif
