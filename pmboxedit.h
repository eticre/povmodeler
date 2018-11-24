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


#ifndef PMBOXEDIT_H
#define PMBOXEDIT_H



#include "pmsolidobjectedit.h"

class PMBox;
class PMVectorEdit;

/**
 * Dialog edit class for @ref PMBox
 */
class PMBoxEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMBoxEdit with parent and name
    */
   PMBoxEdit( QWidget* parent );

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
   PMBox* m_pDisplayedObject;
   PMVectorEdit* m_pCorner1;
   PMVectorEdit* m_pCorner2;
};


#endif
