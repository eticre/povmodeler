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


#include "pmcsgedit.h"
#include "pmcsg.h"

#include <qlayout.h>

#include <qcombobox.h>
//Added by qt3to4:
#include <QHBoxLayout>


PMCSGEdit::PMCSGEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMCSGEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout(); 
   QHBoxLayout* layout;
   m_pTypeCombo = new QComboBox( this );
   m_pTypeCombo->addItem( tr( "Union" ) );
   m_pTypeCombo->addItem( tr( "Intersection" ) );
   m_pTypeCombo->addItem( tr( "Difference" ) );
   m_pTypeCombo->addItem( tr( "Merge" ) );
   
   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( new QLabel( tr( "Type:" ), this ) );
   layout->addWidget( m_pTypeCombo );
   layout->addStretch( 1 );

   connect( m_pTypeCombo, SIGNAL( activated( int ) ), SLOT( slotTypeSelected( int ) ) );
}

void PMCSGEdit::displayObject( PMObject* o )
{
   if( o->isA( "CSG" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMCSG* ) o;

      switch( m_pDisplayedObject->csgType() )
      {
         case PMCSG::CSGUnion:
            m_pTypeCombo->setCurrentIndex( 0 );
            break;
         case PMCSG::CSGIntersection:
            m_pTypeCombo->setCurrentIndex( 1 );
            break;
         case PMCSG::CSGDifference:
            m_pTypeCombo->setCurrentIndex( 2 );
            break;
         case PMCSG::CSGMerge:
            m_pTypeCombo->setCurrentIndex( 3 );
            break;
      }
      
      m_pTypeCombo->setEnabled( !readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMCSGEdit: Can't display object\n";
}

void PMCSGEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      switch( m_pTypeCombo->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setCSGType( PMCSG::CSGUnion );
            break;
         case 1:
            m_pDisplayedObject->setCSGType( PMCSG::CSGIntersection );
            break;
         case 2:
            m_pDisplayedObject->setCSGType( PMCSG::CSGDifference );
            break;
         case 3:
            m_pDisplayedObject->setCSGType( PMCSG::CSGMerge );
            break;
         default:
            m_pDisplayedObject->setCSGType( PMCSG::CSGUnion );
            break;
      }
   }
}

bool PMCSGEdit::isDataValid()
{
   return Base::isDataValid();
}

void PMCSGEdit::slotTypeSelected( int )
{
   emit dataChanged();
}
#include "pmcsgedit.moc"
