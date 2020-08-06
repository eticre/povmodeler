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

#include "pmformulalabel.h"
#include "pmpolynomexponents.h"
#include <qpainter.h>
//Added by qt3to4:
#include <QPaintEvent>

const int c_indent = 3;
const int c_dotSize = 3;

QString PMFormulaLabel::s_xyz[3] =
{
   QString( "x" ), QString( "y" ), QString( "z" )
};

QString PMFormulaLabel::s_digit[10] =
{
   QString( "0" ),
   QString( "1" ),
   QString( "2" ),
   QString( "3" ),
   QString( "4" ),
   QString( "5" ),
   QString( "6" ),
   QString( "7" ),
   QString( "8" ),
   QString( "9" )
};

QString PMFormulaLabel::s_nullString = QString( "= 0" );

PMFormulaLabel::PMFormulaLabel( const PMPolynomExponents& exp, QWidget* parent )
      : QWidget( parent )
{
   m_exponents[0] = exp.exponent( 0 );
   m_exponents[1] = exp.exponent( 1 );
   m_exponents[2] = exp.exponent( 2 );

   calculateSizeHint();
}

PMFormulaLabel::PMFormulaLabel( int x, int y, int z, QWidget* parent)
      : QWidget( parent )
{
   m_exponents[0] = x;
   m_exponents[1] = y;
   m_exponents[2] = z;

   calculateSizeHint();
}

PMFormulaLabel::~PMFormulaLabel()
{
}

void PMFormulaLabel::drawContents( QPainter* p )
{
   QRect cr = rect();
   int i;
   cr.setLeft( cr.left() + c_indent );
   
   int sum = m_exponents[0] + m_exponents[1] + m_exponents[2];
   if( sum == 0 )
      p->drawText( cr, Qt::AlignVCenter | Qt::AlignLeft, s_nullString );
   else
   {
      // draw dot
      int center = ( cr.top() + cr.bottom() ) / 2;
      int rad = c_dotSize / 2;
      p->setBrush( palette().brush( QPalette::Text ) );
      p->drawEllipse( cr.left(), center - rad, c_dotSize, c_dotSize );
      cr.setLeft( cr.left() + c_dotSize + c_indent );

      QFontMetrics m1( font() );
      QFont f2 = exponentFont();
      QFontMetrics m2( f2 );
      int up = m1.height() / 2;
      
      for( i = 0; i < 3; i++ )
      {
         
         if( m_exponents[i] > 0 )
         {
            p->drawText( cr, Qt::AlignVCenter | Qt::AlignLeft, s_xyz[i] );
            cr.setLeft( cr.left() + m1.horizontalAdvance( s_xyz[i] ) );
            if( m_exponents[i] > 1 )
            {
               cr.setBottom( cr.bottom() - up );
               p->setFont( f2 );
               p->drawText( cr, Qt::AlignVCenter | Qt::AlignLeft,
                            s_digit[m_exponents[i]] );
               cr.setLeft( cr.left() + m2.horizontalAdvance( s_digit[m_exponents[i]] ) + 1 );
               cr.setBottom( cr.bottom() + up );
               p->setFont( font() );
            }
         }
      }
   }
}

void PMFormulaLabel::paintEvent( QPaintEvent* ev )
{
   QPainter paint( this );
   if( ev->rect().intersects( rect() ) )
   {
      paint.setClipRegion( ev->region().intersected( rect() ) );
      drawContents( &paint );
   }
}

void PMFormulaLabel::calculateSizeHint()
{
   int sum = m_exponents[0] + m_exponents[1] + m_exponents[2];

   QFontMetrics m1( font() );
   if( sum == 0 )
      m_sizeHint.setWidth( m1.horizontalAdvance( s_nullString ) );
   else
   {
      QFontMetrics m2( exponentFont() );
      int width = c_indent * 3 + c_dotSize;
      int i;
      for( i = 0; i < 3; i++ )
      {
         if( m_exponents[i] > 0 )
         {
            width += m1.horizontalAdvance( s_xyz[i] );
            if( m_exponents[i] > 1 )
               width += m2.horizontalAdvance( s_digit[m_exponents[i]] ) + 1;
         }
      }
      m_sizeHint.setWidth( width );
   }
   m_sizeHint.setHeight( m1.height() + 7 );
}

QSize PMFormulaLabel::sizeHint() const
{
   return minimumSizeHint();
}

QSize PMFormulaLabel::minimumSizeHint() const
{
   return m_sizeHint;
}

void PMFormulaLabel::fontChange( const QFont& )
{
   calculateSizeHint();
}


QFont PMFormulaLabel::exponentFont() const
{
   QFont small = font();
   int fs = small.pointSize();
   if( fs > 0 )
   {
      fs = fs * 2 / 3;
      if( fs == 0 )
         fs = 1;
      small.setPointSize( fs );
   }
   else
   {
      fs = small.pixelSize();
      fs = fs * 2 / 3;
      if( fs == 0 )
         fs = 1;
      small.setPixelSize( fs );
   }
   return small;
}
