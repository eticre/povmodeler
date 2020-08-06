/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2004 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmdragwidget.h"
//Added by qt3to4:
#include <QMouseEvent>

PMDragWidget::PMDragWidget(QWidget* parent, QFlag f )
      : QWidget( parent, f )
{
   dragging = false;
}

void PMDragWidget::mousePressEvent( QMouseEvent* )
{
   dragging = true;
}

void PMDragWidget::mouseReleaseEvent( QMouseEvent* )
{
   dragging = false;
}

void PMDragWidget::mouseMoveEvent( QMouseEvent* )
{
   if( dragging )
   {
      startDrag();
      dragging = false;
   }
}

void PMDragWidget::startDrag()
{
}

#include "pmdragwidget.moc"
