//-*-C++-*-
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


#ifndef PMINSERTERRORDIALOG_H
#define PMINSERTERRORDIALOG_H



#include <QDialog>
#include <qstringlist.h>

class QListWidget;
class QLabel;

/**
 * Dialog that is shown if not all objects could be inserted
    during the execution of a command */
class PMInsertErrorDialog : public QDialog
{
   Q_OBJECT
public:
   /**
    * Creates a modal PMInsertErrorDialog with parent and name.
    */
    PMInsertErrorDialog( int NumberOfObjects, int NumberOfInsertErrors,
                        const QStringList& details,
                        QWidget* parent = nullptr );
protected slots:
   void slotUser1();

private:
   QListWidget* m_pDetails;
   QLabel* m_pDetailsLabel;
};

#endif
