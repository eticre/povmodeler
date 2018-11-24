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


#include "pmblobedit.h"
#include "pmblob.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QHBoxLayout>


PMBlobEdit::PMBlobEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMBlobEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   QBoxLayout* tl = topLayout();
   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Threshold:" ), this ) );
   m_pThreshold = new PMFloatEdit( this );
   hl->addWidget( m_pThreshold );
   m_pThreshold->setValidation( true, 0.0, false, 0 );
   m_pThreshold->setValidationOperator( PMFloatEdit::OpGreater,
                                        PMFloatEdit::OpLess );
   hl->addStretch( 1 );

   m_pSturm = new QCheckBox( tr( "Sturm" ), this );
   tl->addWidget( m_pSturm );
   m_pHierarchy = new QCheckBox( tr( "Hierarchy" ), this );
   tl->addWidget( m_pHierarchy );
   
   connect( m_pThreshold, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pHierarchy, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pSturm, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMBlobEdit::displayObject( PMObject* o )
{
   if( o->isA( "Blob" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMBlob* ) o;

      m_pThreshold->setValue( m_pDisplayedObject->threshold() );
      m_pSturm->setChecked( m_pDisplayedObject->sturm() );
      m_pHierarchy->setChecked( m_pDisplayedObject->hierarchy() );

      m_pThreshold->setReadOnly( readOnly );
      m_pSturm->setEnabled( !readOnly );
      m_pHierarchy->setEnabled( !readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMBlobEdit: Can't display object\n";
}

void PMBlobEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setThreshold( m_pThreshold->value() );
      m_pDisplayedObject->setSturm( m_pSturm->isChecked() );
      m_pDisplayedObject->setHierarchy( m_pHierarchy->isChecked() );
   }
}

bool PMBlobEdit::isDataValid()
{
   if( m_pThreshold->isDataValid() )
      return Base::isDataValid();
   return false;
}

#include "pmblobedit.moc"
