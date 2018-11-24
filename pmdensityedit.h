//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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


#ifndef PMDENSITYEDIT_H
#define PMDENSITYEDIT_H



#include "pmtexturebaseedit.h"

class PMDensity;

/**
 * Dialog edit class for @ref PMDensity
 */
class PMDensityEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMDensityEdit with parent and name
    */
   PMDensityEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
//   virtual void createTopWidgets();
   /** */
//   virtual void saveContents();

private:
   PMDensity* m_pDisplayedObject;
};


#endif
