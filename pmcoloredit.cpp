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

#include "pmcoloredit.h"
#include "pmlineedits.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qcolor.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QColorDialog>

void PMColorEdit::selectColor( QPushButton* b )
{
   QColor c;
   c = getStyleColor( b->styleSheet() );
   QColorDialog colordialog( this );
   c = colordialog.getColor( c );
   if( c.isValid() )
   {
      b->setStyleSheet( "background: " + c.name() + "; border:none;" );
      b->repaint();
      slotColorChanged( c );
   }
}

QColor PMColorEdit::getStyleColor( QString str )
{
    str.remove( 0, 12 );
    str.remove( " " );
<<<<<<< HEAD
    str.truncate(7);
    return QColor ( str );
=======
    return QColor ( str.left(7) );
>>>>>>> 017e873b166b295367db1c3ed517c78e2c5f992c
}

PMColorEdit::PMColorEdit( bool filterAndTransmit, QWidget* parent )
      : QWidget( parent )
{
   m_bFilterAndTransmit = filterAndTransmit;
   m_pSrgb = nullptr;

   m_edits[0] = new PMFloatEdit( this );
   m_edits[1] = new PMFloatEdit( this );
   m_edits[2] = new PMFloatEdit( this );
   if( filterAndTransmit )
   {
      m_edits[3] = new PMFloatEdit( this );
      m_edits[4] = new PMFloatEdit( this );
   }
   else
   {
      m_edits[3] = nullptr;
      m_edits[4] = nullptr;
   }
   m_pButton = new QPushButton();

   QVBoxLayout* topLayout = new QVBoxLayout( this );
   topLayout->setMargin( 0 );
   QHBoxLayout* l = new QHBoxLayout();

   topLayout->addLayout( l );
   l->addWidget( m_pButton );
   //l = new QHBoxLayout();
   //topLayout->addLayout( l );
   l->addWidget( new QLabel( tr( "red:" ), this ) );
   l->addWidget( m_edits[0] );
   l->addWidget( new QLabel( tr( "green:" ), this ) );
   l->addWidget( m_edits[1] );
   l->addWidget( new QLabel( tr( "blue:" ), this ) );
   l->addWidget( m_edits[2] );

   l = new QHBoxLayout();
   topLayout->addLayout( l );
   //eticre use srgb instead rgb
   m_pSrgb = new QCheckBox( tr( "sRGB   " ), this );
   //eticre to do set after load color
   m_pSrgb->setChecked( s_Srgb );
   l->addWidget( m_pSrgb );

   connect( m_pSrgb, SIGNAL( toggled( bool ) ), SLOT( slotSrgbChanged() ) );

   if( filterAndTransmit )
   {
      l->addWidget( new QLabel( tr( "filter" ), this ) );
      l->addWidget( m_edits[3] );
      l->addWidget( new QLabel( tr( "transmit" ), this ) );
      l->addWidget( m_edits[4] );
   }

   connect( m_edits[0], SIGNAL( dataChanged() ), SLOT( slotEditChanged() ) );
   connect( m_edits[1], SIGNAL( dataChanged() ), SLOT( slotEditChanged() ) );
   connect( m_edits[2], SIGNAL( dataChanged() ), SLOT( slotEditChanged() ) );
   if( filterAndTransmit )
   {
      connect( m_edits[3], SIGNAL( dataChanged() ), SLOT( slotEditChanged() ) );
      connect( m_edits[4], SIGNAL( dataChanged() ), SLOT( slotEditChanged() ) );
   }
   connect( m_pButton, &QPushButton::clicked, [=]() { PMColorEdit::selectColor( m_pButton ); } );
   //SLOT( slotColorChanged( const QColor& ) ) );
}

//eticre srgb
void PMColorEdit::slotSrgbChanged()
{
    bool b;
    b = m_pSrgb->isChecked();
    //set bool var useSrgb
    m_color.setSrgb( b );
    //set fifth element in array color
    m_color.setUseSrgb( b );
    emit dataChanged();

}

void PMColorEdit::setColor( const PMColor& c )
{
   bool blocked[5];
   int i;
   int num = m_bFilterAndTransmit ? 5 : 3;
   
   for( i = 0; i < num; i++ )
   {
      blocked[i] = m_edits[i]->signalsBlocked();
      m_edits[i]->blockSignals( true );
   }

   m_color = c;
   m_pSrgb->setChecked( m_color.getSrgb() );
   m_edits[0]->setValue( c.red() );
   m_edits[1]->setValue( c.green() );
   m_edits[2]->setValue( c.blue() );
   if( m_bFilterAndTransmit )
   {
      m_edits[3]->setValue( c.filter() );
      m_edits[4]->setValue( c.transmit() );
   }
   updateButton();

   for( i = 0; i < num; i++ )
      m_edits[i]->blockSignals( blocked[i] );
}

void PMColorEdit::updateButton()
{
   bool b = m_pButton->signalsBlocked();
   m_pButton->blockSignals( true );
   m_pButton->setStyleSheet( "background: " + m_color.toQColor().name() + "; border:none;"  );
   m_pButton->blockSignals( b );
}

bool PMColorEdit::isDataValid()
{
   if( !m_edits[0]->isDataValid() )
      return false;
   if( !m_edits[1]->isDataValid() )
      return false;
   if( !m_edits[2]->isDataValid() )
      return false;
   if( m_bFilterAndTransmit )
   {
      if( !m_edits[3]->isDataValid() )
         return false;
      if( !m_edits[4]->isDataValid() )
         return false;
   }
   return true;
}

void PMColorEdit::setReadOnly( bool yes )
{
   m_edits[0]->setReadOnly( yes );
   m_edits[1]->setReadOnly( yes );
   m_edits[2]->setReadOnly( yes );
   if( m_bFilterAndTransmit )
   {
      m_edits[3]->setReadOnly( yes );
      m_edits[4]->setReadOnly( yes );
   }
   m_pButton->setEnabled( !yes );
}

void PMColorEdit::slotColorChanged( const QColor& c )
{
   int i;
   bool blocked[3];
   
   for( i = 0; i < 3; i++ )
   {
      blocked[i] = m_edits[i]->signalsBlocked();
      m_edits[i]->blockSignals( true );
   }
   
   m_color.setRed( c.red() / 255.0 );
   m_color.setGreen( c.green() / 255.0 );
   m_color.setBlue( c.blue() / 255.0 );

   m_edits[0]->setValue( m_color.red() );
   m_edits[1]->setValue( m_color.green() );
   m_edits[2]->setValue( m_color.blue() );
   
   for( i = 0; i < 3; i++ )
      m_edits[i]->blockSignals( blocked[i] );

   emit dataChanged();
}

void PMColorEdit::slotEditChanged()
{
   bool ok;

   m_edits[0]->text().toDouble( &ok );
   if( ok )
      m_edits[1]->text().toDouble( &ok );
   if( ok )
      m_edits[2]->text().toDouble( &ok );
   if( m_bFilterAndTransmit )
   {
      if( ok )
         m_edits[3]->text().toDouble( &ok );
      if( ok )
         m_edits[4]->text().toDouble( &ok );
   }

   if( ok )
   {
      m_color.setRed( m_edits[0]->value() );
      m_color.setGreen( m_edits[1]->value() );
      m_color.setBlue( m_edits[2]->value() );
      if( m_bFilterAndTransmit )
      {
         m_color.setFilter( m_edits[3]->value() );
         m_color.setTransmit( m_edits[4]->value() );
      }
      updateButton();
   }
   emit dataChanged();
}

#include "pmcoloredit.moc"
