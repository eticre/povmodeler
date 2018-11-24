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


#ifndef PMHEIGHTFIELDEDIT_H
#define PMHEIGHTFIELDEDIT_H



#include "pmsolidobjectedit.h"

class PMHeightField;
class PMFloatEdit;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QPushButton;

/**
 * Dialog edit class for @ref PMHeightField
 */
class PMHeightFieldEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMHeightFieldEdit with parent and name
    */
   PMHeightFieldEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();

protected slots:
   /**
    * Slot called when the height field type is changed
    */
   void slotTypeChanged( int t );
   /**
    * Slot called when the file name is changed
    */
   void slotFileNameChanged( const QString& s );
   /**
    * Slot called when the choose file button is pressed
    */
   void slotFileNameClicked();

protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

private:
   PMHeightField* m_pDisplayedObject;
   QComboBox* m_pHeightFieldType;
   QLineEdit* m_pFileName;
   QPushButton* m_pChooseFileName;
   QCheckBox* m_pSmooth;
   QCheckBox* m_pHierarchy;
   PMFloatEdit* m_pWaterLevel;

};


#endif
