//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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


#ifndef PMCAMERAEDIT_H
#define PMCAMERAEDIT_H



#include "pmnamedobjectedit.h"
#include <QList>
#include <QLabel>

class PMCamera;
class PMVectorEdit;
class PMFloatEdit;
class PMIntEdit;
class QComboBox;
class QLabel;
class QCheckBox;

/**
 * Dialog edit class for @ref PMCamera
 */
class PMCameraEdit : public PMNamedObjectEdit
{
   Q_OBJECT
   typedef PMNamedObjectEdit Base;
public:
   /**
    * Creates a PMCameraEdit with parent and name
    */
   PMCameraEdit( QWidget* parent );

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
   void slotCameraTypeActivated( int index );
   void slotCylinderTypeActivated( int index );
   void slotFocalBlurToggled( bool on );
   void slotAngleToggled( bool on );
   void slotAngleChanged();
   void slotDirectionChanged();
   void slotRightChanged();
   
private:
   void enableFocalWidgets( bool yes );
   void calculateCameraAngle();
   
   PMCamera* m_pDisplayedObject;
   PMVectorEdit* m_pLocation;
   PMVectorEdit* m_pDirection;
   PMVectorEdit* m_pRight;
   PMVectorEdit* m_pUp;
   PMVectorEdit* m_pSky;
   PMVectorEdit* m_pLookAt;
   QCheckBox* m_pEnableAngle;
   PMFloatEdit* m_pAngle;
   QComboBox* m_pCameraType;
   QComboBox* m_pCylinderType;
   QLabel* m_pCylinderTypeLabel;
   QCheckBox* m_pFocalBlur;
   PMFloatEdit* m_pAperture;
   PMIntEdit* m_pBlurSamples;
   PMVectorEdit* m_pFocalPoint;
   PMFloatEdit* m_pConfidence;
   PMFloatEdit* m_pVariance;
   QList<QWidget*> m_focalWidgets;
   QCheckBox* m_pExport;
};


#endif
