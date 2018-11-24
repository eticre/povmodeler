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


#ifndef PMPLANEEDIT_H
#define PMPLANEEDIT_H



#include "pmsolidobjectedit.h"

class PMVectorEdit;
class PMFloatEdit;
class PMPlane;

class PMPlaneEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMPlaneEdit with parent and name
    */
   PMPlaneEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

protected slots:
   /**
    * Normalizes the vector
    */
   void slotNormalize();
   
private:
   PMPlane* m_pDisplayedObject;
   PMVectorEdit* m_pNormal;
   PMFloatEdit* m_pDistance;
};	
#endif
