/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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

#include <stdio.h>

// application specific includes
#include "pmview.h"
#include "pmpart.h"
#include "pmtreeview.h"
#include "pmdialogview.h"
#include "pmglview.h"

#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QSplitter>
#include <QLayout>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSettings>


PMView::PMView( PMPart* part, QWidget* parent )
      : QWidget( parent )
{
   //setBackgroundMode( Qt::PaletteBase );

   QVBoxLayout* layout = new QVBoxLayout( this );
   m_pMainSplitter = new QSplitter( Qt::Horizontal, this );
   m_pMainSplitter->setObjectName( "MainSplitter" );
   m_pTreeEditSplitter = new QSplitter( Qt::Vertical, m_pMainSplitter );
   m_pTreeEditSplitter->setObjectName( "TreeEditSplitter" );

   
   m_pTreeView = new PMTreeView( part, m_pTreeEditSplitter );
   m_pTreeView->setObjectName( "TreeView" );
   m_pTreeView->show();
   
   m_pDialogView = new PMDialogView( part, m_pTreeEditSplitter );
   m_pDialogView->setObjectName( "EditView" );
   m_pDialogView->show();

   
   QWidget* glArea = new QWidget( m_pMainSplitter);
   glArea->setObjectName( "GLArea" );
   glArea->show();
   PMGLView* gl;
   
   QGridLayout* topLayout = new QGridLayout( glArea );
   topLayout->setSpacing( 1 );
   topLayout->setMargin( 1 );
   gl = new PMGLView( part, PMGLView::PMViewPosZ, glArea );
   topLayout->addWidget( gl, 0, 0 );
   gl = new PMGLView( part, PMGLView::PMViewPosX, glArea );
   topLayout->addWidget( gl, 0, 1 );
   gl = new PMGLView( part, PMGLView::PMViewNegY, glArea );
   topLayout->addWidget( gl, 1, 0 );
   gl = new PMGLView( part, PMGLView::PMViewCamera, glArea );
   topLayout->addWidget( gl, 1, 1 );
   
   m_pMainSplitter->show();

   layout->addWidget( m_pMainSplitter );
   layout->activate();

   m_pPart = part;
}

PMView::~PMView()
{
}

void PMView::print( QPrinter* pPrinter )
{
   QPainter printpainter;
   printpainter.begin( pPrinter );
	
   // TODO: add your printing code here

   printpainter.end();
}

void PMView::saveConfig()
{
   QSettings qset;
   qset.beginGroup( "Appearance" );
   qset.setValue( "MainSplitter", m_pMainSplitter->saveState() );
   qset.setValue( "TreeEditSplitter", m_pTreeEditSplitter->saveState() );
}

void PMView::restoreConfig()
{
   QSettings qset;
   qset.beginGroup( "Appearance" );
   m_pMainSplitter->restoreState( qset.value( "MainSplitter", m_pMainSplitter->saveState() ).toByteArray() );
   m_pTreeEditSplitter->restoreState( qset.value( "TreeEditSplitter", m_pTreeEditSplitter->saveState() ).toByteArray() );
}

/*QString PMView::toStr(QList<int> lst )
{
    QString str;
    for( int i=0; i < lst.size() ; i++ )
    {
       str += QString::number( lst[i] );
       if( i < lst.size()-1 )
       str += "," ;
    }
    return str;
}*/

#include "pmview.moc"
