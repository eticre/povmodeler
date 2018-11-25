/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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


#include "pmerrordialog.h"
#include "pmerrorflags.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QTextEdit>
#include <QResizeEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSettings>

QSize PMErrorDialog::s_size = QSize( 350, 400 );

PMErrorDialog::PMErrorDialog( const PMMessageList& messages, int errorFlags,
                              QWidget* parent )
      : QDialog( parent )
{
    setWindowTitle( tr( "Messages" ) );
    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    setLayout( vLayout );
    QPushButton *okButton = new QPushButton( "Ok" );
    QPushButton *cancelButton = new QPushButton( "Cancel" );
    hLayout->addWidget( okButton );
    hLayout->addWidget( cancelButton );
    okButton->setDefault( true );
    okButton->setShortcut( Qt::CTRL | Qt::Key_Return );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

   QLabel* text = new QLabel( QString( "" ) );
   vLayout->addWidget( text );


   m_pTextView = new QTextEdit();
   vLayout->addWidget( m_pTextView );
   m_pTextView->setReadOnly( true );

	m_messages = messages;
   displayMessages();

   if( errorFlags & PMEWarning )
   {
      if( ( errorFlags & PMEError ) || ( errorFlags & PMEFatal ) )
         text->setText( tr( "There were warnings and errors:" ) );
      else
         text->setText( tr( "There were warnings:" ) );
   }
   else
   {
      text->setText( tr( "There were errors:" ) );
   }

   if( errorFlags & PMEFatal )
   {
       okButton->setVisible(false);
   }
   else
   {
      QLabel* l = new QLabel( "Still try to proceed?" );
      vLayout->addWidget( l );
   }

   vLayout->addLayout( hLayout );
   resize( s_size );
}

void PMErrorDialog::displayMessages()
{
	PMMessageList::ConstIterator it;
   QString text;

   text = "<qt>\n";

   for(it = m_messages.constBegin(); it != m_messages.constEnd(); ++it )
      text += "<p>" + (*it).text() + "</p>\n";

   text += "</qt>";

   m_pTextView->setText( text );
}

void PMErrorDialog::saveConfig()
{
   QSettings qset;
   qset.setValue( "Appearance/ErrorDialogSize", s_size );
}

void PMErrorDialog::restoreConfig()
{
   QSettings qset;
   s_size.setHeight( 350 );
   s_size.setWidth( 400 );
   qset.value( "Appearance/ErrorDialogSize", s_size ).value<QSize>();
}

void PMErrorDialog::resizeEvent( QResizeEvent* ev )
{
   s_size = ev->size();
}
#include "pmerrordialog.moc"
