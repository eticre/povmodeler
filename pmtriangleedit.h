//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMTRIANGLEEDIT_H
#define PMTRIANGLEEDIT_H



#include "pmgraphicalobjectedit.h"
//Added by qt3to4:
#include <QLabel>

class PMTriangle;
class PMVectorEdit;
class QCheckBox;
class QLabel;
class QPushButton;

/**
 * Dialog edit class for @ref PMTriangle
 */
class PMTriangleEdit : public PMGraphicalObjectEdit
{
   Q_OBJECT
   typedef PMGraphicalObjectEdit Base;
public:
   /**
    * Creates a PMTriangleEdit with parent and name
    */
   PMTriangleEdit( QWidget* parent );

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
   void slotSmoothChecked( bool on );
   void slotUVVectorsChecked( bool on );
   void slotInvertNormals();

private:
   PMTriangle* m_pDisplayedObject;
   PMVectorEdit* m_pPoint[3];
   PMVectorEdit* m_pNormal[3];
   QLabel* m_pNormalLabel[3];
   QCheckBox* m_pSmooth;
   QLabel* m_pUVVectorLabel[3];
   PMVectorEdit* m_pUVVector[3];
   QCheckBox* m_pUVEnabled;
   QPushButton* m_pMirror;
};


#endif
