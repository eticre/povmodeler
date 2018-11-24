//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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

#ifndef PMLIBRARYOBJECTPREVIEW_H
#define PMLIBRARYOBJECTPREVIEW_H

#include <qwidget.h>
#include <qimage.h>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QLabel>
#include <QDropEvent>
#include <QtXml/QDomDocument>
#include "pmobjectselect.h"


class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class PMPart;
class PMTreeView;

/**
 * Preview widget for Library Objects and sub Libs.
 */
class PMLibraryEntryPreview: public QWidget
{
   Q_OBJECT
public:
   /** Constructor */
   PMLibraryEntryPreview( QWidget *parent );
   /** Destructor */
   ~PMLibraryEntryPreview();

   /**
    * Called to show the file preview.
    * @param url The path to the file
    * @param readOnly Whether the top library is read only
    * @param subLib Whether this is a sub library to preview
    * @Return true if the previous file was saved to disk.
    */
   bool showPreview( QString lib_path, bool readOnly, bool subLib );
   /**
    * Clears the preview
    */
   virtual void clearPreview();

   /**
    * Save the object, if needed.
    * @param forceSave If true don't ask about changes just save them
    * @Return true if a save was performed.
    */
   bool saveIfNeeded( bool forceSave = false );

signals:
   /** Emitted when the object name has been changed */
   void objectChanged();

protected:
   virtual void dragEnterEvent( QDragEnterEvent* event );

   virtual void dropEvent( QDropEvent* event );

private slots:
   /** Called when description or keywords are changed */
   void slotTextChanged();
   /** Called when the name is changed */
   void slotTextChanged( const QString& s );
   /** Called when the set preview button is clicked */
   void slotPreviewClicked();
   /** Called when the apply button is clicked */
   void slotApplyClicked();
   /** Called when the cancel button is clicked */
   void slotCancelClicked();
   /** preview */
   void slotPovrayPreview();
   void slotRecivePovrayPreview( QImage img );
   void slotChangeSelected();

private:
   /** Called to load the object tree. */
   void loadObjects(QByteArray *obj );
   /** Sets whether the object is read only or not */
   void setReadOnly( bool b );
   /** Sets whether the object is modified or not */
   void setModified( bool modified );

   void slotEditCopy();

   PMPart*          m_pPart;
   QLineEdit*       m_pName;
   QTextEdit*       m_pDescription;
   QLabel*          m_pKeywordsLabel;
   QTextEdit*       m_pKeywords;
   QLabel*          m_pContentsLabel;
   PMTreeView*      m_pContentsPreview;
   QPushButton*     m_pSetPreviewImageButton;
   QPushButton*     m_pApplyChanges;
   QPushButton*     m_pCancelChanges;
   QPushButton*     m_pPovrayPreview;
   QImage           m_image;
   QString          m_currentPath;
   PMObjectSelect*  m_pObjectSelect;
   QLabel*          m_pLabelPovrayPreview;
   QMap<int, QImage> mapPreview;
   int              count_preview;

   bool m_modified;
   bool m_readOnly;
   bool m_subLib;
};

#endif
