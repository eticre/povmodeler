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


#ifndef PMCLIPPEDBYEDIT_H
#define PMCLIPPEDBYEDIT_H



#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>

class PMClippedBy;
class QLabel;

/**
 * Dialog edit class for @ref PMClippedBy
 */
class PMClippedByEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMClippedByEdit with parent and name
    */
   PMClippedByEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
   virtual void createTopWidgets();
   
private:
   PMClippedBy* m_pDisplayedObject;
   QLabel* m_pChildLabel;
   QLabel* m_pBoundedByLabel;
};


#endif
