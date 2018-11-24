/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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

#include "pmlinkedit.h"
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include <QPushButton>

#include "pmdeclare.h"
#include "pmobjectselect.h"

PMLinkEdit::PMLinkEdit( const QString& declareType,
                        QWidget* parent )
      : QWidget( parent )
{
   m_declareTypes.append( declareType );
   init();
}

PMLinkEdit::PMLinkEdit( const QStringList& declareTypes,
                        QWidget* parent )
      : QWidget( parent )
{
   m_declareTypes = declareTypes;
   init();
}

PMLinkEdit::PMLinkEdit( QWidget* parent )
      : QWidget( parent )
{
   init();
}

void PMLinkEdit::init()
{
   m_pDeclare = 0;
   m_pDisplayedObject = 0;
   m_bReadOnly = false;

   QGridLayout* grid = new QGridLayout( this );
//TODO PORT QT5    grid->setSpacing( QDialog::spacingHint() );
   grid->setMargin( 0 );

   grid->addWidget( new QLabel( tr( "Prototype:" ), this ), 0, 0 );
   grid->setColumnStretch( 0, 0 );
   grid->setColumnStretch( 1, 1 );
   m_pIDEdit = new QLineEdit( this );
   m_pIDEdit->setReadOnly( true );
   grid->addWidget( m_pIDEdit, 0, 1 );

   QHBoxLayout* layout = new QHBoxLayout();
   grid->addLayout( layout, 1, 1 );
   m_pSelectButton = new QPushButton( tr( "Select..." ), this );
   layout->addWidget( m_pSelectButton );
   m_pClearButton = new QPushButton( this );
   //KGuiItem::assign( m_pClearButton, KStandardGuiItem::clear() );
   layout->addWidget( m_pClearButton );

   connect( m_pSelectButton, SIGNAL( clicked() ), SLOT( slotSelectClicked() ) );
   connect( m_pClearButton, SIGNAL( clicked() ), SLOT( slotClearClicked() ) );
}

void PMLinkEdit::setDisplayedObject( PMObject* obj )
{
   m_pDisplayedObject = obj;
   m_pDeclare = obj->linkedObject();

   if( m_pDeclare )
   {
      m_pIDEdit->setText( m_pDeclare->id() );
      if( !m_bReadOnly )
         m_pClearButton->setEnabled( true );
   }
   else
   {
      m_pIDEdit->clear();
      if( !m_bReadOnly )
         m_pClearButton->setEnabled( false );
   }
}

void PMLinkEdit::setLinkPossibility( const QString& t )
{
   m_declareTypes.clear();
   m_declareTypes.append( t );
}


void PMLinkEdit::setLinkPossibilities( const QStringList& t )
{
   m_declareTypes = t;
}

void PMLinkEdit::setReadOnly( bool yes )
{
   m_bReadOnly = yes;
   m_pClearButton->setEnabled( !m_bReadOnly && m_pDeclare );
   m_pSelectButton->setEnabled( !m_bReadOnly );
}

void PMLinkEdit::slotSelectClicked()
{
   if( m_pDisplayedObject )
   {
      PMObject* obj = 0;
      int result;

      if( m_declareTypes.count() == 1 )
      {
         result = PMObjectSelect::selectDeclare(
         m_pDisplayedObject, m_declareTypes.first(), obj, this );
      }
      else
      {
         result = PMObjectSelect::selectDeclare(
         m_pDisplayedObject, m_declareTypes, obj, this );
      }

      if( ( result == QDialog::Accepted ) && obj )
      {
         m_pDeclare = ( PMDeclare* ) obj;
         m_pIDEdit->setText( m_pDeclare->id() );
         m_pClearButton->setEnabled( true );
         emit dataChanged();
      }
   }
}

void PMLinkEdit::slotClearClicked()
{
   m_pDeclare = 0;
   m_pIDEdit->clear();
   emit dataChanged();
}

#include "pmlinkedit.moc"
