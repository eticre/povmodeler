//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
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


#ifndef PMNORMALEDIT_H
#define PMNORMALEDIT_H



#include "pmtexturebaseedit.h"

class PMNormal;
class QCheckBox;
class PMFloatEdit;

/**
 * Dialog edit class for @ref PMNormal
 */
class PMNormalEdit : public PMTextureBaseEdit
{
   Q_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMNormalEdit with parent and name
    */
   PMNormalEdit( QWidget* parent );

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
   void slotBumpSizeClicked();

private:
   PMNormal* m_pDisplayedObject;

   QCheckBox* m_pBumpSizeCheck;
   PMFloatEdit* m_pBumpSizeEdit;
   PMFloatEdit* m_pAccuracy;
   QCheckBox* m_pUVMapping;
};


#endif
