//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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


#ifndef PMPOVRAYMATRIXEDIT_H
#define PMPOVRAYMATRIXEDIT_H



#include "pmdialogeditbase.h"

class PMPovrayMatrix;
class PMFloatEdit;

/**
 * Dialog edit class for @ref PMPovrayMatrix
 */
class PMPovrayMatrixEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMPovrayMatrixEdit with parent and name
    */
   PMPovrayMatrixEdit( QWidget* parent );

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
   PMPovrayMatrix* m_pDisplayedObject;
   PMFloatEdit* m_pValue[12];
};


#endif
