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


#ifndef PMOBJECTLINKEDIT_H
#define PMOBJECTLINKEDIT_H



#include "pmsolidobjectedit.h"

class PMObjectLink;
class PMLinkEdit;

/**
 * Dialog edit class for @ref PMObjectLink.
 */
class PMObjectLinkEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMObjectLinkEdit with parent and name
    */
   PMObjectLinkEdit( QWidget* parent );

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
   PMObjectLink* m_pDisplayedObject;
   PMLinkEdit* m_pLinkEdit;
};


#endif
