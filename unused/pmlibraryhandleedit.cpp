/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmlibraryhandleedit.h"

#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <QTextEdit>
#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>

#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

#include "pmlineedits.h"
#include "pmdialogeditbase.h"
#include "pmlibraryhandle.h"

QSize PMLibraryHandleEdit::s_size = QSize( 600, 400 );

PMLibraryHandleEdit::PMLibraryHandleEdit( PMLibraryHandle* lib, QWidget* parent )
      : KDialogBase( parent, name, true, i18n( "Create Library" ),
                     Ok | Cancel, Ok )
{
   m_pLibrary = lib;

   resize( s_size );
   QWidget* page = new QWidget( this );
   setMainWidget( page );
   QVBoxLayout* vl = new QVBoxLayout( page );
   vl->setSpacing( KDialog::spacingHint( ) );

   QGridLayout* grid = new QGridLayout( vl );
   QLabel* lbl = new QLabel( i18n( "Name: " ), page );
   m_pNameEdit = new QLineEdit( page );
   grid->addWidget( lbl, 0, 0 );
   grid->addWidget( m_pNameEdit, 0, 1 );

   lbl = new QLabel( i18n( "Author: " ), page );
   m_pAuthorEdit = new QLineEdit( page );
   grid->addWidget( lbl, 1, 0 );
   grid->addWidget( m_pAuthorEdit, 1, 1 );

   lbl = new QLabel( i18n( "Description: " ), page );
   m_pDescriptionEdit = new QTextEdit( page );
   m_pDescriptionEdit->setMaximumHeight( 120 );
   grid->addWidget( lbl, 2, 0 );
   grid->addMultiCellWidget( m_pDescriptionEdit, 2, 3, 1, 1 );
   grid->setRowStretch( 3, 1 );
   
   m_pReadOnlyEdit = new QCheckBox( i18n( "Allow changes to the library?" ), page );
   grid->addMultiCellWidget( m_pReadOnlyEdit, 4, 4, 0, 1 );

   // Load the fields with values
   m_pNameEdit->setText( lib->name( ) );
   m_pDescriptionEdit->setText( lib->description( ) );
   m_pAuthorEdit->setText( lib->author( ) );
   m_pReadOnlyEdit->setChecked( !lib->isReadOnly( ) ); 
   
   // Setup the signals
   connect( m_pNameEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotEditsChanged( const QString& ) ) );
   connect( m_pAuthorEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotEditsChanged( const QString& ) ) );
   connect( m_pDescriptionEdit, SIGNAL( textChanged( ) ), SLOT( slotDescriptionChanged( ) ) );
   connect( m_pReadOnlyEdit, SIGNAL( clicked( ) ), SLOT( slotReadOnlyChanged( ) ) );

   // On startup you can only cancel
   enableButtonOk( false );
}

void PMLibraryHandleEdit::slotReadOnlyChanged( )
{
   enableButtonOk( true );
}

void PMLibraryHandleEdit::slotEditsChanged( const QString& /*str*/ )
{
   enableButtonOk( true );
}

void PMLibraryHandleEdit::slotDescriptionChanged( )
{
   enableButtonOk( true );
}

void PMLibraryHandleEdit::saveConfig( KConfig* cfg )
{
   cfg->setGroup( "Appearance" );
   cfg->writeEntry( "LibraryHandleEditSize", s_size );
}

void PMLibraryHandleEdit::restoreConfig( KConfig* cfg )
{
   cfg->setGroup( "Appearance" );

   QSize defaultSize( 300, 200 );
   s_size = cfg->readEntry<QSize>( "LibraryHandleEditSize", defaultSize );
}

void PMLibraryHandleEdit::resizeEvent( QResizeEvent* ev )
{
   s_size = ev->size( );
}

void PMLibraryHandleEdit::slotOk( )
{
   m_pLibrary->setName( m_pNameEdit->text( ) );
   m_pLibrary->setAuthor( m_pAuthorEdit->text( ) );
   m_pLibrary->setDescription( m_pDescriptionEdit->text( ) );
   m_pLibrary->setReadOnly( !m_pReadOnlyEdit->isChecked( ) );
   
   accept( );
}

#include "pmlibraryhandleedit.moc"
