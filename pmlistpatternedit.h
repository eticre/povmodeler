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


#ifndef PMLISTPATTERNEDIT_H
#define PMLISTPATTERNEDIT_H



#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>

class PMListPattern;
class PMVectorEdit;
class QComboBox;
class PMFloatEdit;
class QLabel;

/**
 * Dialog edit class for @ref PMListPattern.
 */
class PMListPatternEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMListPatternEdit with parent and name
    */
   PMListPatternEdit( QWidget* parent );

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
   /**
    * This slot is called when the list type is changed
    */
   void slotComboChanged( int c );

private:
   PMListPattern*  m_pDisplayedObject;
   QComboBox*      m_pTypeCombo;
   PMFloatEdit*    m_pMortar;
   PMVectorEdit*   m_pBrickSize;
   QLabel*         m_pBrickSizeLabel;
   QLabel*         m_pMortarLabel;

   QLabel*         m_pDepthLabel;
   PMFloatEdit*    m_pDepth;
};


#endif
