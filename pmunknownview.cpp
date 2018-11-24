/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmunknownview.h"

#include <QLayout>
//Added by qt3to4:
#include <QLabel>
#include <QHBoxLayout>

PMUnknownView::PMUnknownView( const QString& viewType,
                              QWidget* parent )
      : PMViewBase( parent )
{
   QHBoxLayout* hl = new QHBoxLayout( this );
   QLabel* l;
   QString s;
   s = ( "Unknown view type \"%1\"" + viewType );
   l = new QLabel( this );
   l->setText( s );
   l->setAlignment( Qt::AlignCenter );
   hl->addWidget( l );
   m_viewType = viewType;
}

QString PMUnknownView::description() const
{
   return tr( "Unknown" );
}
