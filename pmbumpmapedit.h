//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Passos Carvalho
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


#ifndef PMBUMPMAPEDIT_H
#define PMBUMPMAPEDIT_H



#include "pmpalettevalueedit.h"
#include "pmdialogeditbase.h"
//Added by qt3to4:
#include <QLabel>

class PMBumpMap;
class QComboBox;
class PMFloatEdit;
class QLabel;
class QCheckBox;
class QWidget;
class QLineEdit;
class QPushButton;

/**
 * Dialog edit class for @ref PMBumpMap.
 */
class PMBumpMapEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMBumpMapEdit with parent and name
    */
   PMBumpMapEdit( QWidget* parent );
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
   /** */
   void slotImageFileTypeChanged( int a );
   /** */
   void slotMapTypeChanged( int a );
   /** */
   void slotInterpolateTypeChanged( int a );
   /** */
   void slotImageFileNameChanged( const QString& a );
   /** */
   void slotImageFileBrowseClicked();
private:
   PMBumpMap*   m_pDisplayedObject;
   QComboBox*   m_pImageFileTypeEdit;
   QLineEdit*   m_pImageFileNameEdit;
   QPushButton* m_pImageFileNameBrowse;
   QCheckBox*   m_pOnceEdit;
   QComboBox*   m_pMapTypeEdit;
   QComboBox*   m_pInterpolateTypeEdit;
   QCheckBox*   m_pUseIndexEdit;
   PMFloatEdit* m_pBumpSizeEdit;
};

#endif
