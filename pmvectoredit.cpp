/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#include "pmvectoredit.h"
#include "pmdebug.h"
#include <qstring.h>
#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QDialog>
#include <QMessageBox>

PMVectorEdit::PMVectorEdit( const QString& descriptionX,
                            const QString& descriptionY,
                            QWidget* parent )
      : QWidget( parent )
{
   int i;
   QHBoxLayout* layout;
   QLabel* label;

   for( i = 0; i < 2; i++ )
   {
      m_edits.append( new QLineEdit( this ) );
      connect( m_edits[i], SIGNAL( textChanged( const QString& ) ),
               SLOT( slotTextChanged( const QString& ) ) );
   }

   layout = new QHBoxLayout( this );

   if( !descriptionX.isEmpty() )
   {
      label = new QLabel( descriptionX, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[0] );
//TODO PORT QT5    layout->addSpacing( QDialog::spacingHint() );

   if( !descriptionY.isEmpty() )
   {
      label = new QLabel( descriptionY, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[1] );
}

PMVectorEdit::PMVectorEdit( const QString& descriptionX,
                            const QString& descriptionY,
                            const QString& descriptionZ,
                            QWidget* parent )
      : QWidget( parent )
{
   int i;
   QHBoxLayout* layout;
   QLabel* label;

   for( i = 0; i < 3; i++ )
   {
      m_edits.append( new QLineEdit( this ) );
      connect( m_edits[i], SIGNAL( textChanged( const QString& ) ),
               SLOT( slotTextChanged( const QString& ) ) );
   }

   layout = new QHBoxLayout( this );

   if( !descriptionX.isEmpty() )
   {
      label = new QLabel( descriptionX, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[0] );
//TODO PORT QT5    layout->addSpacing( QDialog::spacingHint() );

   if( !descriptionY.isEmpty() )
   {
      label = new QLabel( descriptionY, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[1] );
//TODO PORT QT5    layout->addSpacing( QDialog::spacingHint() );

   if( !descriptionZ.isEmpty() )
   {
      label = new QLabel( descriptionZ, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[2] );
}

PMVectorEdit::PMVectorEdit( const QString& descriptionA,
                            const QString& descriptionB,
                            const QString& descriptionC,
                            const QString& descriptionD,
                            QWidget* parent )
      : QWidget( parent )
{
   int i;
   QHBoxLayout* layout;
   QLabel* label;

   for( i = 0; i < 4; i++ )
   {
      m_edits.append( new QLineEdit( this ) );
      connect( m_edits[i], SIGNAL( textChanged( const QString& ) ),
               SLOT( slotTextChanged( const QString& ) ) );
   }

   layout = new QHBoxLayout( this );

   if( !descriptionA.isEmpty() )
   {
      label = new QLabel( descriptionA, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[0] );
//TODO PORT QT5    layout->addSpacing( QDialog::spacingHint() );

   if( !descriptionB.isEmpty() )
   {
      label = new QLabel( descriptionB, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[1] );
//TODO PORT QT5    layout->addSpacing( QDialog::spacingHint() );

   if( !descriptionC.isEmpty() )
   {
      label = new QLabel( descriptionC, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[2] );
//TODO PORT QT5    layout->addSpacing( QDialog::spacingHint() );

   if( !descriptionD.isEmpty() )
   {
      label = new QLabel( descriptionD, this );
      layout->addWidget( label );
//TODO PORT QT5       layout->addSpacing( QDialog::spacingHint() );
   }
   layout->addWidget( m_edits[3] );
}

void PMVectorEdit::setVector( const PMVector& v, int precision )
{
   int i;
   QString str;

   if( v.size() != m_edits.size() )
      qCritical(  ) << "Vector has wrong size in PMVectorEdit::setVector\n";

   for( i = 0; ( i < m_edits.size() ) && ( i < v.size() ); i++ )
   {
      str.setNum( v[i], 'g', precision );
      m_edits[i]->setText( str );
   }
}

PMVector PMVectorEdit::vector() const
{
   PMVector result( m_edits.size() );
   int i;

   for( i = 0; i < m_edits.size(); i++ )
      result[i] = m_edits[i]->text().toDouble();

   return result;
}

bool PMVectorEdit::isDataValid()
{
   bool ok = true;
   int i;

   for( i = 0; ( i < m_edits.size() ) && ok; i++ )
   {
      m_edits[i]->text().toDouble( &ok );
      if( !ok )
      {
         QMessageBox::warning( this, tr( "Error" ), tr( "Please enter a valid float value!" ) );
         m_edits[i]->setFocus();
         m_edits[i]->selectAll();
      }
   }

   return ok;
}

void PMVectorEdit::slotTextChanged( const QString& )
{
   emit dataChanged();
}

void PMVectorEdit::setReadOnly( bool yes )
{
   int i;
   for( i = 0; ( i < m_edits.size() ); i++ )
      m_edits[i]->setReadOnly( yes );
}

void PMVectorEdit::setEnabled( bool yes )
{
   int i;
   for( i = 0; ( i < m_edits.size() ); i++ )
      m_edits[i]->setEnabled( yes );
}

#include "pmvectoredit.moc"
