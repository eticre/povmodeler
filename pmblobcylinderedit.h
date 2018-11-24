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


#ifndef PMBLOBCYLINDEREDIT_H
#define PMBLOBCYLINDEREDIT_H



#include "pmdetailobjectedit.h"

class PMVectorEdit;
class PMFloatEdit;
class PMBlobCylinder;

class PMBlobCylinderEdit : public PMDetailObjectEdit
{
   Q_OBJECT
   typedef PMDetailObjectEdit Base;
public:
   /**
    * Creates a PMBlobCylinderEdit with parent and name
    */
   PMBlobCylinderEdit( QWidget* parent );

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
   PMBlobCylinder* m_pDisplayedObject;
   PMVectorEdit* m_pEnd1;
   PMVectorEdit* m_pEnd2;
   PMFloatEdit* m_pRadius;
   PMFloatEdit* m_pStrength;
};
#endif
