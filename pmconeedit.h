//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorinaez
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


#ifndef PMCONEEDIT_H
#define PMCONEEDIT_H



#include "pmsolidobjectedit.h"

class QCheckBox;

class PMVectorEdit;
class PMFloatEdit;
class PMCone;

class PMConeEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMConeEdit with parent and name
    */
   PMConeEdit( QWidget* parent );

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
   PMCone* m_pDisplayedObject;
   PMVectorEdit* m_pEnd1;
   PMVectorEdit* m_pEnd2;
   PMFloatEdit* m_pRadius1;
   PMFloatEdit* m_pRadius2;
   QCheckBox* m_pOpen;
};	
#endif
