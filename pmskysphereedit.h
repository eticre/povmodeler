//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMSKYSPHEREEDIT_H
#define PMSKYSPHEREEDIT_H



#include "pmtexturebaseedit.h"

class PMSkySphere;

/**
 * Dialog edit class for @ref PMSkySphere
 */
class PMSkySphereEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMSkySphereEdit with parent and name
    */
   PMSkySphereEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
//   virtual void createTopWidgets();
   /** */
//   virtual void saveContents();

private:
   PMSkySphere* m_pDisplayedObject;
};


#endif
