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


#ifndef PMINTERIORTEXTUREEDIT_H
#define PMINTERIORTEXTUREEDIT_H



#include "pmtexturebaseedit.h"

class PMInteriorTexture;

/**
 * Dialog edit class for @ref PMTexture
 */
class PMInteriorTextureEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMTextureEdit with parent and name
    */
   PMInteriorTextureEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
//   virtual void createTopWidgets();
   /** */
//   virtual void saveContents();

private:
   PMInteriorTexture* m_pDisplayedObject;
};


#endif
