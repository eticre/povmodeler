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


#include "pmlibraryobject.h"
#include "pmlibraryentrypreview.h"
#include "pmlibraryobjectsearch.h"
#include "pmdialogeditbase.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QPixmap>
#include <QImage>
#include <QFrame>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QUrl>
#include <QLocale>
#include <QDialog>
#include <QTableView>
#include <QHeaderView>
//#include <k3listview.h>


PMLibraryObjectSearch::PMLibraryObjectSearch( QWidget* parent ) :
   QWidget( parent )
{
   setMinimumSize( 780, 300 );
   setMaximumSize( 800, 400 );
   QVBoxLayout* vl = new QVBoxLayout( this );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );

   // Search parameters
   QFrame* frame = new QFrame( this );
   QHBoxLayout* fhl = new QHBoxLayout( frame );
//TODO PORT QT5    fhl->setSpacing( QDialog::spacingHint() );
   QGridLayout* grid = new QGridLayout();
   grid->addLayout( fhl, 0, 0 );

   QLabel *lbl = new QLabel( tr( "Search for:" ), frame );
   m_pSearch = new QLineEdit( frame );
   grid->addWidget( lbl, 0, 0 );
   grid->addWidget( m_pSearch, 0, 1 );

   QVBoxLayout* fvl = new QVBoxLayout(  );
   fvl->addLayout( fhl );
   m_pSearchButton = new QPushButton( tr( "&Search" ), frame );
   fvl->addWidget( m_pSearchButton );
   fvl->addStretch( 1 );

   vl->addWidget( frame );

   // Search results
   frame = new QFrame( this );
   QHBoxLayout* hl = new QHBoxLayout( frame );
//TODO PORT QT5    hl->setSpacing( QDialog::spacingHint() );
   m_pFileList = new QTableView( frame );
   m_model = new QStandardItemModel( 1, 2, this );//rows cols
   m_model->setHorizontalHeaderItem( 0, new QStandardItem( QString( "File" ) ) );
   m_model->setHorizontalHeaderItem( 1, new QStandardItem( QString( "Path" ) ) );
   m_pFileList->setModel( m_model );
   m_pFileList->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
   m_pFileList->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_pFileList->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
   m_pFileList->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
   m_pFileList->setEnabled( true );
   m_pFileList->setEditTriggers( QAbstractItemView::NoEditTriggers );

   m_pPreview = new PMLibraryEntryPreview( frame );
   hl->addWidget( m_pFileList, 1 );
   hl->addWidget( m_pPreview );
   vl->addWidget( frame );

   // Connect signals and slots
   connect( m_pSearchButton, SIGNAL( clicked() ), SLOT( slotSearchButtonPressed() ) );
}

void PMLibraryObjectSearch::slotSearchButtonPressed()
{
//   QStringList::Iterator it( s_libraryPath );
   // For each of the defined libraries
   //
   // Open recursively each library file
   // Check if any of the strings contains the search words
   // If it does add to the list
}

#include "pmlibraryobjectsearch.moc"
