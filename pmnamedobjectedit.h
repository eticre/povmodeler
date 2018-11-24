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


#ifndef PMNAMEDOBJECTEDIT_H
#define PMNAMEDOBJECTEDIT_H



#include "pmdialogeditbase.h"

class PMNamedObject;
class QLineEdit;

/**
 * Dialog edit class for @ref PMNamedObject.
 */
class PMNamedObjectEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMNamedObjectEdit with parent and name
    */
   PMNamedObjectEdit( QWidget* parent );

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
   void slotNameChanged( const QString& );
private:
   PMNamedObject* m_pDisplayedObject;
   QLineEdit* m_pNameEdit;
};


#endif
