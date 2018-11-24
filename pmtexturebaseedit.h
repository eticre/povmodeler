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


#ifndef PMTEXTUREBASEEDIT_H
#define PMTEXTUREBASEEDIT_H



#include "pmnamedobjectedit.h"

class PMTextureBase;
class PMLinkEdit;

/**
 * Dialog edit class for @ref PMTextureBase
 */
class PMTextureBaseEdit : public PMNamedObjectEdit
{
   Q_OBJECT
   typedef PMNamedObjectEdit Base;
public:
   /**
    * Creates a PMTextureBaseEdit with parent and name
    */
   PMTextureBaseEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();
   /**
    * Enables/disables the link edit widget
    */
   void enableLinkEdit( bool enable );

private:
   PMTextureBase* m_pDisplayedObject;
   PMLinkEdit* m_pLinkEdit;
};


#endif
