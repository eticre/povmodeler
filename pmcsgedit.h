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


#ifndef PMCSGEDIT_H
#define PMCSGEDIT_H



#include "pmsolidobjectedit.h"

class PMCSG;
class QComboBox;

/**
 * Dialog edit class for @ref PMCSG
 */
class PMCSGEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMCSGEdit with parent and name
    */
   PMCSGEdit( QWidget* parent );

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
   void slotTypeSelected( int index );
private:
   PMCSG* m_pDisplayedObject;
   QComboBox* m_pTypeCombo;
};


#endif
