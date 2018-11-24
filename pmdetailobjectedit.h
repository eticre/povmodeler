//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2004 by Leon Pennington
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
#ifndef PMDETAILOBJECTEDIT_H
#define PMDETAILOBJECTEDIT_H



#include "pmnamedobjectedit.h"
//Added by qt3to4:
#include <QLabel>

class PMDetailObject;
class QCheckBox;
class QComboBox;

/**
 * Dialog edit class for @ref PMNamedObject.
 */
class PMDetailObjectEdit : public PMNamedObjectEdit
{
   Q_OBJECT
   typedef PMNamedObjectEdit Base;
public:
   /**
    * Creates a PMNamedObjectEdit with parent and name
    */
   PMDetailObjectEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();

protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

private slots:
   void slotGlobalDetailClicked();

private:
   PMDetailObject* m_pDisplayedObject;
   QCheckBox* m_pGlobalDetail;
   QLabel* m_pLocalDetailLevelLabel;
   QComboBox* m_pLocalDetailLevel;
};


#endif
