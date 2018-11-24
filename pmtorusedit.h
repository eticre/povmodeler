/*
 **************************************************************************
                          pmtorusedit.h  -  description
                             -------------------
    begin                : Sat Jun 30 2001
    copyright            : (C) 2001 Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PMTORUSEDIT_H
#define PMTORUSEDIT_H



#include "pmsolidobjectedit.h"

class PMTorus;
class PMFloatEdit ;
class QCheckBox;

/**
 * Dialog edit class for @ref PMTorus
 */
class PMTorusEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMSphereEdit with parent and name
    */
   PMTorusEdit( QWidget* parent );

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
   PMTorus* m_pDisplayedObject;
   PMFloatEdit* m_pMinorRadius;
   PMFloatEdit* m_pMajorRadius;
   QCheckBox * m_pSturm;

};


#endif
