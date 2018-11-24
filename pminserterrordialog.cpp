/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2007 by Andreas Zehender
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


#include "pminserterrordialog.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

PMInsertErrorDialog::PMInsertErrorDialog(
   int numObj, int numErrors,
   const QStringList& details,
   QWidget* parent /*= 0 */ )
      : QDialog( parent )
{
    setWindowTitle( tr( "Insert Errors" ) );
	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Help);
	QWidget *mainWidget = new QWidget(this);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	mainLayout->addWidget(mainWidget);
	QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
	okButton->setDefault(true);
	okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
	QPushButton *user1Button = new QPushButton;
	buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	//PORTING SCRIPT: WARNING mainLayout->addWidget(buttonBox) must be last item in layout. Please move it.
	mainLayout->addWidget(buttonBox);
	okButton->setDefault(true);
    user1Button->setText(tr( "Details"  ));
   QWidget* page = new QWidget( this );
   QVBoxLayout *pageVBoxLayout = new QVBoxLayout(page);
   pageVBoxLayout->setMargin(0);
//PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout:    setMainWidget( page );
// Add mainLayout->addWidget(page); if necessary
// Add mainLayout->addWidget(page); if necessary
   //new QLabel( ( "%2 of 1 object could not be inserted.", "%2 of %1 objects could not be inserted.",
                 //numObj, numErrors ), page );

   m_pDetailsLabel = new QLabel( tr( "Objects not inserted:" ), page );
   pageVBoxLayout->addWidget( m_pDetailsLabel );
   m_pDetailsLabel->hide();

   m_pDetails = new QListWidget( page );
   pageVBoxLayout->addWidget( m_pDetails );
   m_pDetails->addItems( details );
   m_pDetails->setMinimumHeight( 150 );
   m_pDetails->hide();
   connect( user1Button,SIGNAL( clicked() ),this,SLOT(slotUser1() ) );

}

void PMInsertErrorDialog::slotUser1()
{
   m_pDetailsLabel->show();
   m_pDetails->show();
}
#include "pminserterrordialog.moc"
