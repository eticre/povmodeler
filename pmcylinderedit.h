//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorinaez
    email                : skoriane@nce.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMCYLINDEREDIT_H
#define PMCYLINDEREDIT_H



#include "pmsolidobjectedit.h"

class PMVectorEdit;
class PMFloatEdit;
class PMCylinder;
class QCheckBox;

class PMCylinderEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMCylinderEdit with parent and name
    */
   PMCylinderEdit( QWidget* parent );

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
   PMCylinder* m_pDisplayedObject;
   PMVectorEdit* m_pEnd1;
   PMVectorEdit* m_pEnd2;
   PMFloatEdit* m_pRadius;
   QCheckBox* m_pOpen;

};	
#endif
