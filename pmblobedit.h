//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMBLOBEDIT_H
#define PMBLOBEDIT_H



#include "pmsolidobjectedit.h"

class PMBlob;
class PMFloatEdit;
class QCheckBox;

/**
 * Dialog edit class for @ref PMBlob
 */
class PMBlobEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMBlobEdit with parent and name
    */
   PMBlobEdit( QWidget* parent );

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
   PMBlob* m_pDisplayedObject;
   PMFloatEdit* m_pThreshold;
   QCheckBox* m_pSturm;
   QCheckBox* m_pHierarchy;
};


#endif
