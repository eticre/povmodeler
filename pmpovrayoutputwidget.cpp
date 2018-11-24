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

#include "pmpovrayoutputwidget.h"

#include <QTextEdit>
#include <QLayout>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QPushButton>
#include <QPushButton>
#include <QSettings>

#include "pmdebug.h"

QSize PMPovrayOutputWidget::s_size = QSize( 800, 600 );

PMPovrayOutputWidget::PMPovrayOutputWidget()
{
    QVBoxLayout* topLayout = new QVBoxLayout( this );
    topLayout->setMargin( 0 );

    m_pTextView = new QTextEdit();
    topLayout->addWidget( m_pTextView );

    m_pTextView->setReadOnly( true );

    QPushButton *closeButton = new QPushButton( "Close" );
    topLayout->addWidget( closeButton );

    connect( closeButton, SIGNAL( clicked() ), SLOT( slotClose() ) );
    setWindowTitle( "Povray Output" );
    resize( s_size );

    m_startOfLastLine = 0;
}

PMPovrayOutputWidget::~PMPovrayOutputWidget()
{
}

void PMPovrayOutputWidget::slotClear()
{
   m_output.clear();
   m_startOfLastLine = 0;
   m_pTextView->clear();
}

void PMPovrayOutputWidget::slotText( const QString& output )
{
   int i;

   for( i = 0; i < output.length(); i++ )
   {
      QChar c = output[i];
      if( c == '\r' )
         m_output.truncate( m_startOfLastLine );
      else if( c == '\n' )
      {
         m_output += c;
         m_startOfLastLine = m_output.length();
         //qDebug() << m_startOfLastLine;
      }
      else if( c.isPrint() )
         m_output += c;
   }

   m_pTextView->setPlainText( m_output );
}

void PMPovrayOutputWidget::slotClose()
{
   hide();
}


void PMPovrayOutputWidget::saveConfig()
{
   QSettings qset;
   qset.beginGroup( "Appearance" );
   qset.setValue( "PovrayOutputWidgetSize", s_size );
}

void PMPovrayOutputWidget::restoreConfig()
{
   QSettings qset;
   qset.beginGroup( "Appearance" );
   QSize defaultSize( 500, 400 );
   s_size = qset.value( "PovrayOutputWidgetSize", defaultSize ).value<QSize>();
}

void PMPovrayOutputWidget::resizeEvent( QResizeEvent* ev )
{
   s_size = ev->size();
}

#include "pmpovrayoutputwidget.moc"
