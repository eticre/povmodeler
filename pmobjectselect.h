//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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

#ifndef PMOBJECTSELECT_H
#define PMOBJECTSELECT_H



#include <QDialog>
#include <QListWidget>
#include <QSize>
#include <QList>
#include <QStringList>
#include <QDialogButtonBox>
#include <QPushButton>
#include "pmobject.h"
#include "pmdeclare.h"
#include "pmdialogeditbase.h"
#include "pmpart.h"
#include "pmpovrayrenderwidget.h"
#include <QLabel>
#include <QMap>


/**
 * QListBoxItem for @ref PMObject
 */
class PMListBoxObject : public QListWidgetItem
{
public:
   /**
    * Constructs a list box item in listbox listbox showing the
    * object obj. The item gets inserted after the item after
    */
   PMListBoxObject( QListWidget* listbox, PMObject* obj, QListWidgetItem* after );
   /**
    * Constructs a list box item in listbox listbox showing the object obj
    */
   PMListBoxObject( QListWidget* listbox, PMObject* obj );
   /**
    * Constructs a list box item showing the object obj
    */
   PMListBoxObject( PMObject* obj );
   /**
    * Constructs a list box item in listbox listbox showing the
    * text text. The item gets inserted after the item after
    */
   PMListBoxObject( QListWidget* listbox, PMObject* obj, const QString& text,
                    QListWidgetItem* after );
   /**
    * Constructs a list box item in listbox listbox showing the text text
    */
   PMListBoxObject( QListWidget* listbox, PMObject* obj, const QString& text );
   /**
    * Constructs a list box item showing the text text
    */
   PMListBoxObject( PMObject* obj, const QString& text );
   /**
    * Deletes the item
    */
   ~PMListBoxObject();

   /**
    * Returns a pointer to the object
    */
   PMObject* object() const { return m_pObject; }

private:
   static QString checkName( const QString& text );
   PMObject* m_pObject;
};



class PMDialogEditBase;

/**
 * A PMObject selection widget
 */
class PMObjectSelect : public QDialog
{
   Q_OBJECT
public:
   /**
    * Creates a selection widget with parent and name
    */
   PMObjectSelect( QWidget* parent = nullptr,
                   bool modal = true );
   /**
    * Deletes the dialog
    */
   ~PMObjectSelect();

   /**
    * Appends the object to the list of choosable objects
    */
   void addObject( PMObject* obj );

   /**
    * Returns the currently selected object
    */
   PMObject* selectedObject() const { return m_pSelectedObject; }
   /**
    * Set the currently selected object
    */
   void setSelectedObject( PMObject* selobj )  {  m_pSelectedObject = selobj; }
   /**
    * get the image selected preview
    */
   QImage imagePreview() { return mapPreview[m_pListWidget->currentRow()]; }
   /**
    * Create a modal dialog and let the user choose an declare object
    * of type t.
    *
    * Only objects above the object link  are listed.
    */
   static int selectObject( PMObject* link, const QString& t, PMObject* & obj,
                            QWidget* parent = nullptr );
   static int selectObject( PMObject* link, const QStringList& t,
                            PMObject* & obj, QWidget* parent = nullptr );
   static int selectDeclare( PMObject* link, const QString& declareType,
                             PMObject* & obj, QWidget* parent = nullptr );
   static int selectDeclare( PMObject* link, const QStringList& dt,
                             PMObject* & obj, QWidget* parent = nullptr );
   QStringList selectFunctions( PMObject* link );

signals:

   /**
    * Emit when preview image is recived from povray(PMPovrayRenderWidget)
    */
   void ok_image( QImage );

public slots:
   /**
    * request previev image from povray(PMPovrayRenderWidget)
    */
   void slotPreview();
   /**
    * recive previev image from povray(PMPovrayRenderWidget)
    */
   void slotProgress( QImage img );

protected slots:

   void slotSelectionChanged();
   void slotDoubleClicked( QListWidgetItem* );
private:
   QListWidget* m_pListWidget;
   PMObject* m_pSelectedObject;
   static QSize s_size;
   PMDialogEditBase* m_pEditBase;
   QWidget* m_pWidget;
   QLabel* m_pLabel;
   PMPart* m_pPart;
   PMPovrayRenderWidget* m_pRenderPreviewSelect;
   QImage m_image;
   QLabel* lbl;
   int i_previousRow;
   int i_listRows;
   QMap<int, QImage> mapPreview;
   QDialogButtonBox* buttonBox;
   QPushButton* user1Button;
};

#endif
