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
   QWidget* parent )
  : QDialog( parent )
{
    setWindowTitle( tr( "Insert Errors" ) );

    QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Help);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    QPushButton *user1Button = new QPushButton();
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

   //QLabel m_pNumberLabel;
   //m_pNumberLabel = new QLabel( ( "%2 of 1 object could not be inserted.", "%2 of %1 objects could not be inserted.",numObj, numErrors ) );

   m_pDetailsLabel = new QLabel(tr("Objects not inserted:"));
   mainLayout->addWidget(m_pDetailsLabel);

   m_pDetails = new QListWidget();
   m_pDetails->addItems(details);
   m_pDetails->setMinimumHeight(150);
   mainLayout->addWidget(m_pDetails);
   user1Button->setText(tr("Close"));
   mainLayout->addWidget(buttonBox);

   connect( user1Button,SIGNAL( clicked() ),this,SLOT(slotUser1() ) );

}

void PMInsertErrorDialog::slotUser1()
{
   this->close();
   m_pDetailsLabel->show();
   m_pDetails->show();
}
#include "pminserterrordialog.moc"
