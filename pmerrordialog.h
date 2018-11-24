//-*-C++-*-
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


#ifndef PMERRORDIALOG_H
#define PMERRORDIALOG_H



#include <QDialog>
#include <QList>
#include <QResizeEvent>

class QTextEdit;

#include "pmmessage.h"

/**
 * Dialog that is shown if some errors or warnings occurred during
 * parsing or execution of commands.
 */
class PMErrorDialog : public QDialog
{
   Q_OBJECT
public:
   /**
    * Creates a modal PMErrorDialog with parent and name.
    *
    * messages is the message list. If the list contains a message of type
    * FatalError, the 'Proceed" button will not be shown.
    *
    * PMErrorDialog::exec() returns QDialog::Accepted if the command
    * should be continued.
    */
   PMErrorDialog( const PMMessageList& messages, int errorFlags,
                  QWidget* parent = nullptr );

   static void saveConfig();
   static void restoreConfig();

protected:
   virtual void resizeEvent( QResizeEvent* ev );

private:
   void displayMessages();

   static QSize s_size;
   QTextEdit* m_pTextView;
   PMMessageList m_messages;
};

#endif
