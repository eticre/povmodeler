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

#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>

#include <QLocale>
#include <QSettings>

#include <QMessageBox>
#include <QPushButton>

#include "pmlineedits.h"
#include "pmdialogeditbase.h"
#include "pmlibraryhandle.h"


QSize PMLibraryHandleEdit::s_size = QSize( 600, 400 );

PMLibraryHandleEdit::PMLibraryHandleEdit( PMLibraryHandle* lib, QWidget* parent )
    : QDialog ( parent )
{

    m_buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok|QDialogButtonBox::Cancel );
    QWidget *mainWidget = new QWidget( this );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout( mainLayout );
    mainLayout->addWidget( mainWidget );
    m_buttonBox->button( QDialogButtonBox::Ok )->setDefault( true );
    m_buttonBox->button( QDialogButtonBox::Ok )->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect( m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect( m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    //PORTING SCRIPT: WARNING mainLayout->addWidget(buttonBox) must be last item in layout. Please move it.
    mainLayout->addWidget( m_buttonBox );
    mainWidget->setWindowTitle( tr( "Create Library" ) );
    m_pLibrary = lib;

   resize( s_size );
   QWidget* page = new QWidget( this );
//PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout:    setMainWidget( page );
// Add mainLayout->addWidget(page); if necessary
// Add mainLayout->addWidget(page); if necessary
   QVBoxLayout* vl = new QVBoxLayout( page );
//TODO PORT QT5    vl->setSpacing( QDialog::spacingHint() );

   QGridLayout* grid = new QGridLayout;//( vl );
   vl->addLayout( grid );
   QLabel* lbl = new QLabel( tr( "Name: " ), page );
   m_pNameEdit = new QLineEdit( page );
   grid->addWidget( lbl, 0, 0 );
   grid->addWidget( m_pNameEdit, 0, 1 );

   lbl = new QLabel( tr( "Author: " ), page );
   m_pAuthorEdit = new QLineEdit( page );
   grid->addWidget( lbl, 1, 0 );
   grid->addWidget( m_pAuthorEdit, 1, 1 );

   lbl = new QLabel( tr( "Description: " ), page );
   m_pDescriptionEdit = new QTextEdit( page );
   m_pDescriptionEdit->setMaximumHeight( 120 );
   grid->addWidget( lbl, 2, 0 );
   grid->addWidget( m_pDescriptionEdit, 2, 3, 1, 1 );
   grid->setRowStretch( 3, 1 );
   
   m_pReadOnlyEdit = new QCheckBox( tr( "Allow changes to the library?" ), page );
   grid->addWidget( m_pReadOnlyEdit, 4, 4, 0, 1 );

   // Load the fields with values
   m_pNameEdit->setText( lib->name() );
   m_pDescriptionEdit->setText( lib->description() );
   m_pAuthorEdit->setText( lib->author() );
   m_pReadOnlyEdit->setChecked( !lib->isReadOnly() ); 
   
   // Setup the signals
   connect( m_pNameEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotEditsChanged( const QString& ) ) );
   connect( m_pAuthorEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotEditsChanged( const QString& ) ) );
   connect( m_pDescriptionEdit, SIGNAL( textChanged() ), SLOT( slotDescriptionChanged() ) );
   connect( m_pReadOnlyEdit, SIGNAL( clicked() ), SLOT( slotReadOnlyChanged() ) );

   // On startup you can only cancel
   m_buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
}

void PMLibraryHandleEdit::slotReadOnlyChanged()
{
  //okButton->setEnabled( true );
  m_buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
}

void PMLibraryHandleEdit::slotEditsChanged( const QString& /*str*/ )
{
   m_buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
}

void PMLibraryHandleEdit::slotDescriptionChanged()
{
   m_buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
}

void PMLibraryHandleEdit::saveConfig()
{
   QSettings qset;
   qset.setValue( "Appearance/LibraryHandleEditSize", s_size );
   //cfg->group("Appearance");
   //cfg->setGroup( "Appearance" );
   //cfg->writeEntry( "LibraryHandleEditSize", s_size );
}

void PMLibraryHandleEdit::restoreConfig()
{
   QSettings qset;
   QSize defaultSize( 300, 200 );
   s_size = qset.value( "Appearance/LibraryHandleEditSize", defaultSize ).value<QSize>();
}

void PMLibraryHandleEdit::resizeEvent( QResizeEvent* ev )
{
   s_size = ev->size();
}

void PMLibraryHandleEdit::slotOk()
{
   m_pLibrary->setName( m_pNameEdit->text() );
   m_pLibrary->setAuthor( m_pAuthorEdit->text() );
   m_pLibrary->setDescription( m_pDescriptionEdit->toPlainText() );
   m_pLibrary->setReadOnly( !m_pReadOnlyEdit->isChecked() );
   
   accept();
}

#include "pmlibraryhandleedit.moc"
