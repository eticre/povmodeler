//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMTEXTEDIT_H
#define PMTEXTEDIT_H



#include "pmsolidobjectedit.h"

class PMText;
class PMVectorEdit;
class PMFloatEdit;
class QLineEdit;
class QPushButton;

/**
 * Dialog edit class for @ref PMText
 */
class PMTextEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMTextEdit with parent and name
    */
   PMTextEdit( QWidget* parent );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid();

protected slots:
   void slotTextChanged( const QString& t );
   void slotChooseFont();
   
protected:
   /** */
   virtual void createTopWidgets();
   /** */
   virtual void saveContents();

private:
   PMText* m_pDisplayedObject;
   QLineEdit* m_pFont;
   QPushButton* m_pChooseFont;
   QLineEdit* m_pText;
   PMFloatEdit* m_pThickness;
   PMVectorEdit* m_pOffset;
};


#endif
