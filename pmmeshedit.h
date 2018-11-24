//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMMESHEDIT_H
#define PMMESHEDIT_H



#include "pmsolidobjectedit.h"

class PMMesh;
class QCheckBox;
class PMVectorEdit;

/**
 * Dialog edit class for @ref PMMesh
 */
class PMMeshEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMMeshEdit with parent and name
    */
   PMMeshEdit( QWidget* parent );

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
   void slotInsideVectorClicked();
private:
   PMMesh* m_pDisplayedObject;

   QCheckBox* m_pHierarchy;
   QCheckBox* m_pEnableInsideVector;
   PMVectorEdit* m_pInsideVector;
};


#endif
