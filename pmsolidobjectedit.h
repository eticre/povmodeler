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


#ifndef PMSOLIDOBJECTEDIT_H
#define PMSOLIDOBJECTEDIT_H



#include "pmgraphicalobjectedit.h"

class PMSolidObject;
class QCheckBox;

/**
 * Dialog edit class for @ref PMSolidObject.
 */
class PMSolidObjectEdit : public PMGraphicalObjectEdit
{
   Q_OBJECT
   typedef PMGraphicalObjectEdit Base;
public:
   /**
    * Creates a PMSolidObjectEdit with parent and name
    */
   PMSolidObjectEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();
protected:
   /** */
   virtual void createBottomWidgets();
   /** */
   virtual void saveContents();

private:
   PMSolidObject* m_pDisplayedObject;
   QCheckBox* m_pHollowButton;
   QCheckBox* m_pInverseButton;
};


#endif
