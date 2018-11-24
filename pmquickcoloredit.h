//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMQUICKCOLOREDIT_H
#define PMQUICKCOLOREDIT_H



#include <QColorDialog>

#include "pmdialogeditbase.h"

class PMQuickColor;
class PMColorEdit;

/**
 * Dialog edit class for @ref PMQuickColor.
 */
class PMQuickColorEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMQuickColorEdit with parent and name
    */
   PMQuickColorEdit( QWidget* parent );

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
   PMQuickColor* m_pDisplayedObject;
   PMColorEdit* m_pColorEdit;
};


#endif
