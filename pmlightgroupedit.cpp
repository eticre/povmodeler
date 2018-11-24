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


#include "pmlightgroupedit.h"
#include "pmlightgroup.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>


PMLightGroupEdit::PMLightGroupEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMLightGroupEdit::createTopWidgets()
{
   Base::createTopWidgets();

   QHBoxLayout* layout;
   QBoxLayout* tl = topLayout();
   m_pGlobalLights = new QCheckBox( tr( "Global lights" ), this );

   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( m_pGlobalLights );
   layout->addStretch( 1 );

   connect( m_pGlobalLights, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMLightGroupEdit::displayObject( PMObject* o )
{
   if( o->isA( "LightGroup" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMLightGroup* ) o;

      m_pGlobalLights->setChecked( m_pDisplayedObject->globalLights() );
      m_pGlobalLights->setEnabled( !readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMLightGroupEdit: Can't display object\n";
}

void PMLightGroupEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();

      m_pDisplayedObject->setGlobalLights( m_pGlobalLights->isChecked() );
   }
}

bool PMLightGroupEdit::isDataValid()
{
   return Base::isDataValid();
}

#include "pmlightgroupedit.moc"
