//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002-2006 by Andreas Zehender
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


#ifndef PMPOLYNOMEDIT_H
#define PMPOLYNOMEDIT_H



#include "pmsolidobjectedit.h"
#include <QList>

class PMPolynom;
class PMFloatEdit;
class QSpinBox;
class QCheckBox;

/**
 * Dialog edit class for @ref PMPolynom
 */
class PMPolynomEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMPolynomEdit with parent and name
    */
   PMPolynomEdit( QWidget* parent );

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
   void slotOrderChanged( int );

private:
   void displayCoefficients( const PMVector& co, int cOrder, int dOrder );
   PMVector coefficients() const;
   
   PMPolynom* m_pDisplayedObject;
   
   int m_currentOrder;
   
   QWidget* m_pPolyWidget;
   QList<QWidget*> m_labels;
   QList<PMFloatEdit*> m_edits;
   QCheckBox* m_pSturm;
   QSpinBox* m_pOrder;
   bool m_readOnly;
};


#endif
