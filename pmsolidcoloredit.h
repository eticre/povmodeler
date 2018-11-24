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


#ifndef PMSOLIDCOLOREDIT_H
#define PMSOLIDCOLOREDIT_H



#include "pmdialogeditbase.h"

class PMSolidColor;
class PMColorEdit;

/**
 * Dialog edit class for @ref PMSolidColor.
 */
class PMSolidColorEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMSolidColorEdit with parent and name
    */
   PMSolidColorEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

private:
   PMSolidColor* m_pDisplayedObject;
   PMColorEdit* m_pColorEdit;
};


#endif
