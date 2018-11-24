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


#ifndef PMTEXTUREEDIT_H
#define PMTEXTUREEDIT_H



#include "pmtexturebaseedit.h"

class PMTexture;
class QCheckBox;

/**
 * Dialog edit class for @ref PMTexture
 */
class PMTextureEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMTextureEdit with parent and name
    */
   PMTextureEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

private:
   PMTexture* m_pDisplayedObject;
   QCheckBox* m_pUVMapping;
};


#endif
