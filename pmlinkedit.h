/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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

#ifndef PMLINKEDIT_H
#define PMLINKEDIT_H

#include <qwidget.h>
#include "pmobject.h"
#include "pmdeclare.h"

#include <QList>

class QString;
class QLineEdit;
class QPushButton;

/**
 * Edit widget for links with a QLineEdit, a select and a clear button.
 */
class PMLinkEdit : public QWidget
{
   Q_OBJECT
public:
   /**
    * Creates a link edit widget with parent and name.
    *
    * Allows the selection of declares of type declareType.
    */
   PMLinkEdit( const QString& declareType, QWidget* parent );
   /**
    * Creates a link edit widget with parent and name.
    *
    * Allows the selection of declares of type declares.
    */
   PMLinkEdit( const QStringList& declares, QWidget* parent );
   /**
    * Creates a link edit widget with parent and name.
    */
   PMLinkEdit( QWidget* parent );

   /**
    * Sets the displayed object and displays the link
    */
   void setDisplayedObject( PMObject* obj );
   /**
    * Returns the selected link
    */
   PMDeclare* link() const { return m_pDeclare; }
   /**
    * Sets the selection possibilities
    */
   void setLinkPossibility( const QString& t );
   /**
    * Sets the selection possibilities
    */
   void setLinkPossibilities( const QStringList& t );
   
   /**
    * Enables or disables read only mode
    */
   void setReadOnly( bool yes = true );
signals:
   /**
    * Emitted when the link is changed
    */
   void dataChanged();

public slots:
   /**
    * Called when the select button is clicked
    */
   void slotSelectClicked();
   /**
    * Called when the clear button is clicked
    */
   void slotClearClicked();

private:
   void init();
   PMDeclare* m_pDeclare;
   PMObject* m_pDisplayedObject;
   QStringList m_declareTypes;

   QLineEdit* m_pIDEdit;
   QPushButton* m_pSelectButton;
   QPushButton* m_pClearButton;
   bool m_bReadOnly;
};

#endif
