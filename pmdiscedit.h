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


#ifndef PMDISCEDIT_H
#define PMDISCEDIT_H



#include "pmgraphicalobjectedit.h"

class PMVectorEdit;
class PMFloatEdit;
class PMDisc;

class PMDiscEdit : public PMGraphicalObjectEdit
{
   Q_OBJECT
   typedef PMGraphicalObjectEdit Base;
public:
   /**
    * Creates a PMPlaneEdit with parent and name
    */
   PMDiscEdit( QWidget* parent );

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
   PMDisc* m_pDisplayedObject;
   PMVectorEdit* m_pCenter;
   PMVectorEdit* m_pNormal;
   PMFloatEdit* m_pRadius;
   PMFloatEdit* m_pHRadius;
};
#endif
