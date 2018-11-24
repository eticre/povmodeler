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


#ifndef PMDECLAREEDIT_H
#define PMDECLAREEDIT_H



#include "pmdialogeditbase.h"

class PMDeclare;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QPushButton;

/**
 * Dialog edit class for @ref PMDeclare.
 */
class PMDeclareEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMDeclareEdit with parent and name
    */
   PMDeclareEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void createBottomWidgets();
   /** */
   virtual void saveContents();

private slots:
   void slotNameChanged( const QString& );
   void slotItemSelected( QListWidgetItem* );
   void slotSelect();
private:
   PMDeclare* m_pDisplayedObject;
   QLineEdit* m_pNameEdit;
   QListWidget* m_pLinkedObjects;
   QPushButton* m_pSelectButton;
   PMObject* m_pSelectedObject;
};


#endif
