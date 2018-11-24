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


#ifndef PMFORMULALABEL_H
#define PMFORMULALABEL_H



#include <qwidget.h>
//Added by qt3to4:
#include <QPaintEvent>

class PMPolynomExponents;

/**
 * QLabel with a rich text to display a polynom for the quadric,
 * cubic, quartic and polynom objects.
 */
class PMFormulaLabel : public QWidget
{
public:
   /**
    * Displays the exponents of the @ref PMPolynomExponents
    */
   PMFormulaLabel( const PMPolynomExponents& exp, QWidget* parent );
   /**
    * Displays the given exponents
    */
   PMFormulaLabel( int x, int y, int z, QWidget* parent );
   /**
    * Destructor
    */
   ~PMFormulaLabel();
   virtual QSize sizeHint() const;
   virtual QSize minimumSizeHint() const;
   
protected:
   virtual void drawContents( QPainter* p );
   virtual void paintEvent( QPaintEvent* e );
   virtual void fontChange( const QFont& oldFont );
   
private:
   QFont exponentFont() const;
   void calculateSizeHint();
   
   QSize m_sizeHint;
   int m_exponents[3];
   
   static QString s_xyz[3];
   static QString s_digit[10];
   static QString s_nullString;
};

#endif
