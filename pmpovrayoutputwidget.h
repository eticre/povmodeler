//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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

#ifndef PMPOVRAYOUTPUTWIDGET_H
#define PMPOVRAYOUTPUTWIDGET_H




#include <QDialog>
//Added by qt3to4:
#include <QResizeEvent>
class QTextEdit;

/**
 * Widget to display the povray text output
 */
class PMPovrayOutputWidget : public QDialog
{
   Q_OBJECT
public:
   /**
    * Standard constructor
    */
   PMPovrayOutputWidget();
   /**
    * Destructor
    */
   ~PMPovrayOutputWidget();

   static void saveConfig();
   static void restoreConfig();

public slots:
   /**
    * Clears the text
    */
   void slotClear();
   /**
    * Adds the text to the output
    */
   void slotText( const QString& output );

protected slots:
   /**
    * Called when the close button is clicked
    */
   void slotClose();

protected:
   virtual void resizeEvent( QResizeEvent* ev );

private:
   QTextEdit* m_pTextView;
   int m_startOfLastLine;
   QString m_output;
   static QSize s_size;
};


#endif
