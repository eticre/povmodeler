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

#ifndef PMLIBRARYOBJECTSEARCH_H
#define PMLIBRARYOBJECTSEARCH_H



#include <QWidget>
#include <QStandardItemModel>

class QLineEdit;
class QPushButton;
class PMLibraryEntryPreview;
class QTableView;

/**
 * Search widget for Library Objects.
 * It also provides drag. If the user doesn't have a clear idea
 * where the objects he wants are, this is the dialog to use.
 */
class PMLibraryObjectSearch: public QWidget
{
   Q_OBJECT
public:
   PMLibraryObjectSearch( QWidget *parent );

private slots:
   void slotSearchButtonPressed();

private:
   QLineEdit*   m_pSearch;
   QPushButton* m_pSearchButton;
   QTableView* m_pFileList;
   QStandardItemModel* m_model;
   PMLibraryEntryPreview* m_pPreview;
};

#endif
