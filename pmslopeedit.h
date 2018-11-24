//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMSLOPEEDIT_H
#define PMSLOPEEDIT_H



#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>

class PMSlope;
class PMFloatEdit;
class QLabel;

/**
 * Dialog edit class for @ref PMSlope.
 */
class PMSlopeEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMSlopeEdit with parent and name
    */
   PMSlopeEdit( QWidget* parent );

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
   PMSlope* m_pDisplayedObject;
   PMFloatEdit*  m_pHeightEdit;
   PMFloatEdit*  m_pSlopeEdit;
};


#endif
