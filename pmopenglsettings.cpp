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

#include "pmopenglsettings.h"

#include "pmglview.h"
#include "pmdefaults.h"

#include <qlayout.h>
#include <qcheckbox.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QVBoxLayout>

PMOpenGLSettings::PMOpenGLSettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QVBoxLayout* vlayout = new QVBoxLayout( this );
//TODO PORT QT5    vlayout->setSpacing( QDialog::spacingHint() );
   vlayout->setMargin( 0 );

   m_pDirect = new QCheckBox( tr( "Direct rendering" ), this );
   vlayout->addWidget( new QLabel( tr( "PovModeler must be restarted for the changes to take effect" ), this ) );
   vlayout->addWidget( m_pDirect );
   vlayout->addStretch( 1 );
}

void PMOpenGLSettings::displaySettings()
{
   m_pDirect->setChecked( PMGLView::isDirectRenderingEnabled() );
}

void PMOpenGLSettings::displayDefaults()
{
   m_pDirect->setChecked( true );
}

bool PMOpenGLSettings::validateData()
{
   return true;
}

void PMOpenGLSettings::applySettings()
{
   PMGLView::enableDirectRendering( m_pDirect->isChecked() );
}

#include "pmopenglsettings.moc"
