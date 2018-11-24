/*
**************************************************************************

                          pmsphereedit.h  -  description
                             -------------------
    begin                : Wed Jun 6 2001
    copyright            : (C) 2001 by Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMSPHEREEDIT_H
#define PMSPHEREEDIT_H



#include "pmsolidobjectedit.h"

class PMSphere;
class PMVectorEdit;
class PMFloatEdit ;

/**
 * Dialog edit class for @ref PMSphere
 */
class PMSphereEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMSphereEdit with parent and name
    */
   PMSphereEdit( QWidget* parent );

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
   PMSphere* m_pDisplayedObject;
   PMVectorEdit* m_pCentre;
   PMFloatEdit* m_pRadius;
};


#endif
