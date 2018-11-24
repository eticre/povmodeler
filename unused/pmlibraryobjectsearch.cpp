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

#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qimage.h>
#include <QFrame>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <kurl.h>
#include <klocale.h>
#include <kdialog.h>
#include <k3listview.h>

PMLibraryObjectSearch::PMLibraryObjectSearch( QWidget* parent ) :
   QWidget( parent, "" )
{
   setMinimumSize( 780, 300 );
   setMaximumSize( 800, 400 );
   QVBoxLayout* vl = new QVBoxLayout( this );
   vl->setSpacing( KDialog::spacingHint( ) );

   // Search parameters
   QFrame* frame = new QFrame( this );
   QHBoxLayout* fhl = new QHBoxLayout( frame );
   fhl->setSpacing( KDialog::spacingHint( ) );
   QGridLayout* grid = new QGridLayout( fhl );
   QLabel *lbl = new QLabel( i18n( "Search for:" ), frame );
   m_pSearch = new QLineEdit( frame );
   grid->addWidget( lbl, 0, 0 );
   grid->addWidget( m_pSearch, 0, 1 );

   QVBoxLayout* fvl = new QVBoxLayout( fhl );
   m_pSearchButton = new QPushButton( i18n( "&Search" ), frame );
   fvl->addWidget( m_pSearchButton );
   fvl->addStretch( 1 );

   vl->addWidget( frame );

   // Search results
   frame = new QFrame( this );
   QHBoxLayout* hl = new QHBoxLayout( frame );
   hl->setSpacing( KDialog::spacingHint( ) );
   m_pFileList = new K3ListView( frame );
   m_pFileList->addColumn( i18n( "File" ) );
   m_pFileList->addColumn( i18n( "Path" ) );
   m_pFileList->setFullWidth( true );
   m_pPreview = new PMLibraryEntryPreview( frame );
   hl->addWidget( m_pFileList, 1 );
   hl->addWidget( m_pPreview );
   vl->addWidget( frame );

   // Connect signals and slots
   connect( m_pSearchButton, SIGNAL( clicked( ) ), SLOT( slotSearchButtonPressed( ) ) );
}

void PMLibraryObjectSearch::slotSearchButtonPressed( )
{
//   QStringList::Iterator it( s_libraryPath );
   // For each of the defined libraries
   //
   // Open recursively each library file
   // Check if any of the strings contains the search words
   // If it does add to the list
}

#include "pmlibraryobjectsearch.moc"
