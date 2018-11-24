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


#ifndef PMISOSURFACEEDIT_H
#define PMISOSURFACEEDIT_H



#include "pmsolidobjectedit.h"
#include "pmobjectselect.h"
//Added by qt3to4:
#include <QLabel>
#include <QPushButton>

class PMIsoSurface;
class PMVectorEdit;
class PMFloatEdit;
class PMIntEdit;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;

/**
 * Dialog edit class for @ref PMIsoSurface
 */
class PMIsoSurfaceEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMIsoSurfaceEdit with parent and name
    */
   PMIsoSurfaceEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

private slots:
   void textChanged( const QString& );
   void currentChanged( int i );
   void evaluateToggled( bool );
   void allToggled( bool );
   void toggled( bool );
   void insertFunction();

private:
   PMIsoSurface* m_pDisplayedObject;
   QLineEdit* m_pFunction;
   QComboBox* m_pListFunction;
   QPushButton* m_pSelectFunction;
   QComboBox* m_pContainedBy;
   PMVectorEdit* m_pCorner1;
   PMVectorEdit* m_pCorner2;
   PMVectorEdit* m_pCenter;
   PMFloatEdit* m_pRadius;
   QLabel* m_pCorner1Label;
   QLabel* m_pCorner2Label;
   QLabel* m_pCenterLabel;
   QLabel* m_pRadiusLabel;
   PMFloatEdit* m_pThreshold;
   PMFloatEdit* m_pAccuracy;
   PMFloatEdit* m_pMaxGradient;
   QCheckBox* m_pEvaluate;
   PMFloatEdit* m_pEvaluateValue[3];
   QCheckBox* m_pOpen;
   PMIntEdit* m_pMaxTrace;
   QCheckBox* m_pAllIntersections;
   PMObjectSelect* m_pObjectSelect;
   QStringList m_pGetList;
};


#endif
