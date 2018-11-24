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


#ifndef PMMATERIALEDIT_H
#define PMMATERIALEDIT_H



#include "pmtexturebaseedit.h"

class PMMaterial;

/**
 * Dialog edit class for @ref PMMaterial
 */
class PMMaterialEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMMaterialEdit with parent and name
    */
   PMMaterialEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
//   virtual void createTopWidgets();
   /** */
//   virtual void saveContents();

private:
   PMMaterial* m_pDisplayedObject;
};


#endif
