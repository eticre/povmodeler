/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmdiscedit.h"
#include "pmdisc.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <QLayout>
#include <QLabel>
#include <QPushButton>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>

#include <QMessageBox>

PMDiscEdit::PMDiscEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMDiscEdit::createTopWidgets()
{
   Base::createTopWidgets();

   QHBoxLayout* layout;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();

   m_pCenter = new PMVectorEdit( "x", "y", "z", this );
   m_pNormal = new PMVectorEdit( "x", "y", "z", this );
   m_pHRadius = new PMFloatEdit( this );
   m_pHRadius->setValidation( true, 0.0, false, 0.0 );
   m_pRadius = new PMFloatEdit( this );
   m_pRadius->setValidation( true, 0.0, false, 0.0 );

   gl = new QGridLayout();
   tl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Center:" ), this ), 0, 0 );
   gl->addWidget( m_pCenter, 0, 1 );
   gl->addWidget( new QLabel( tr( "Normal:" ), this ), 1, 0 );
   gl->addWidget( m_pNormal, 1, 1 );
   
   layout = new QHBoxLayout();
   tl->addLayout( layout );
   gl = new QGridLayout();
   layout->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Radius:" ), this ), 0, 0 );
   gl->addWidget( m_pRadius, 0, 1 );
   gl->addWidget( new QLabel( tr( "Hole radius:" ), this ), 1, 0 );
   gl->addWidget( m_pHRadius, 1, 1 );
   layout->addStretch( 1 );

   QPushButton* nb = new QPushButton( tr( "Normalize" ), this );
   layout = new QHBoxLayout();
   tl->addLayout( layout );
   layout->addWidget( nb );
   layout->addStretch( 1 );

   connect( m_pCenter, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pNormal, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pRadius, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pHRadius, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( nb, SIGNAL( clicked() ), SLOT( slotNormalize() ) );
}

void PMDiscEdit::slotNormalize()
{
   PMVector normal = m_pNormal->vector();
   double l = normal.abs();
   if( !approxZero( l ) )
      m_pNormal->setVector( normal / l );
}


void PMDiscEdit::displayObject( PMObject* o )
{
   if( o->isA( "Disc" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = static_cast<PMDisc*>(o);

      m_pCenter->setVector( m_pDisplayedObject->center() );
      m_pNormal->setVector( m_pDisplayedObject->normal() );
      m_pRadius->setValue( m_pDisplayedObject->radius() );
      m_pHRadius->setValue( m_pDisplayedObject->holeRadius() );

      m_pCenter->setReadOnly( readOnly );
      m_pNormal->setReadOnly( readOnly );
      m_pRadius->setReadOnly( readOnly );
      m_pHRadius->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMDiscEdit: Can't display object\n";
}

void PMDiscEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setCenter( m_pCenter->vector() );
      m_pDisplayedObject->setNormal( m_pNormal->vector() );
      m_pDisplayedObject->setRadius( m_pRadius->value() );
      m_pDisplayedObject->setHoleRadius( m_pHRadius->value() );
   }
}

bool PMDiscEdit::isDataValid()
{
   if( m_pNormal->isDataValid() )
   {
      if( approxZero( m_pNormal->vector().abs() ) )
      {
         QMessageBox::warning( this, tr( "The normal vector may not be a "
                                         "null vector." ),
                             tr( "Error" ) );
         return false;
      }
      if( m_pCenter->isDataValid() )
      {
         if( m_pRadius->isDataValid() )
         {
            if( m_pHRadius->isDataValid() )
            {
               if( m_pRadius->value() >= m_pHRadius->value() )
               {
                  return Base::isDataValid();
               }
               else
               {
                  QMessageBox::warning( this, tr( "The radius may not be smaller "
                                                  "than the hole radius." ),
                                      tr( "Error" ) );
                  m_pRadius->setFocus();
               }
            }
         }
      }
   }
   return false;
}

#include  "pmdiscedit.moc"
