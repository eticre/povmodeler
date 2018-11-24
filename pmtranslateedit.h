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


#ifndef PMMOVEEDIT_H
#define PMMOVEEDIT_H



#include "pmdialogeditbase.h"

class PMTranslate;
class PMVectorEdit;

/**
 * Dialog edit class for @ref PMTranslate
 */
class PMTranslateEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMTranslateEdit with parent and name
    */
   PMTranslateEdit( QWidget* parent );

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
   PMTranslate* m_pDisplayedObject;
   PMVectorEdit* m_pVector;
};


#endif
