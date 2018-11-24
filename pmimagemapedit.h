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


#ifndef PMIMAGEMAPEDIT_H
#define PMIMAGEMAPEDIT_H



#include "pmpalettevalueedit.h"
#include "pmdialogeditbase.h"
#include <QLabel>
#include <QList>

class PMImageMap;
class PMPaletteValue;
class QComboBox;
class PMFloatEdit;
class QLabel;
class QCheckBox;
class QWidget;
class QLineEdit;
class QPushButton;

/**
 * Dialog edit class for @ref PMImageMap.
 */
class PMImageMapEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMImageMapEdit with parent and name
    */
   PMImageMapEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void createBottomWidgets();
   /** */
   virtual void saveContents();
   /** */
   QList<PMPaletteValue> filters();
   /** */
   QList<PMPaletteValue> transmits();
   /** */
   void displayPaletteEntries( const QList<PMPaletteValue>& filters,
                               const QList<PMPaletteValue>& transmits );

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
   /** */
   void slotFilterAllClicked();
   /** */
   void slotTransmitAllClicked();
   /** */
   void slotAddFilterEntry();
   /** */
   void slotRemoveFilterEntry();
   /** */
   void slotAddTransmitEntry();
   /** */
   void slotRemoveTransmitEntry();
private:
   PMImageMap*  m_pDisplayedObject;
   QComboBox*   m_pImageFileTypeEdit;
   QLineEdit*   m_pImageFileNameEdit;
   QPushButton* m_pImageFileNameBrowse;
   QCheckBox*   m_pOnceEdit;
   QComboBox*   m_pMapTypeEdit;
   QComboBox*   m_pInterpolateTypeEdit;
   QCheckBox*   m_pEnableFilterAllEdit;
   QCheckBox*   m_pEnableTransmitAllEdit;
   PMFloatEdit* m_pFilterAllEdit;
   PMFloatEdit* m_pTransmitAllEdit;
   QWidget*     m_pFiltersWidget;
   QWidget*     m_pTransmitsWidget;

   QList< PMPaletteValueEdit* > m_filterEntries;
   QList< QPushButton* >        m_filterAddButtons;
   QList< QPushButton* >        m_filterRemoveButtons;
   QList< PMPaletteValueEdit* > m_transmitEntries;
   QList< QPushButton* >        m_transmitAddButtons;
   QList< QPushButton* >        m_transmitRemoveButtons;
};

#endif
