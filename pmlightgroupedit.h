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


#ifndef PMLIGHTGROUPEDIT_H
#define PMLIGHTGROUPEDIT_H



#include "pmsolidobjectedit.h"

class PMLightGroup;
class QCheckBox;

/**
 * Dialog edit class for @ref PMLightGroup
 */
class PMLightGroupEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMLightGroupEdit with parent and name
    */
   PMLightGroupEdit( QWidget* parent );

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

private:
   PMLightGroup* m_pDisplayedObject;

   QCheckBox* m_pGlobalLights;
};


#endif
