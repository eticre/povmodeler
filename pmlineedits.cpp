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

#include "pmlineedits.h"
#include <QMessageBox>


PMFloatEdit::PMFloatEdit( QWidget* parent )
      : QLineEdit( parent )
{
   m_bCheckLower = false;
   m_bCheckUpper = false;
   m_lowerValue = 0;
   m_upperValue = 0;
   m_lowerOp = OpGreaterEqual;
   m_upperOp = OpLessEqual;

   connect( this, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotEditTextChanged( const QString& ) ) );
}

void PMFloatEdit::setValidation( bool checkLower, double lowerValue,
                                 bool checkUpper, double upperValue )
{
   m_bCheckLower = checkLower;
   m_bCheckUpper = checkUpper;
   m_lowerValue = lowerValue;
   m_upperValue = upperValue;
}

void PMFloatEdit::setValidationOperator( ValidationOp l, ValidationOp u )
{
   m_lowerOp = l;
   m_upperOp = u;
}

bool PMFloatEdit::isDataValid()
{
   bool ok = true;
   double d;
   d = text().toDouble( &ok );
   
   if( ok )
   {
      if( m_bCheckLower )
         ok = ok && ( m_lowerOp == OpGreaterEqual ?
                      d >= m_lowerValue : d > m_lowerValue );
      if( m_bCheckUpper )
         ok = ok && ( m_upperOp == OpLessEqual ?
                      d <= m_upperValue : d < m_upperValue );
      if( !ok )
      {
         if( m_bCheckLower && m_bCheckUpper )
            QMessageBox::warning( this, "Error", "Please enter a float value "
                                            "between " + QString::number( m_lowerValue ) + " and " +
                                            QString::number( m_upperValue ) );
         else if( m_bCheckLower )
         {
            if( m_lowerOp == OpGreaterEqual )
               QMessageBox::warning( this, ( "Please enter a float value "
                                               ">= " + QString::number( m_lowerValue ) ),
                                               ( "Error" ) );
            else
                QMessageBox::warning( this, "Error", "Please enter a float value "
                                                "> " + QString::number( m_lowerValue ) );
         }
         else
         {
            if( m_upperOp == OpLessEqual )
                QMessageBox::warning( this, "Error", "Please enter a float value "
                                                "<= " + QString::number( m_upperValue ) );
            else
                QMessageBox::warning( this, "Error", "Please enter a float value "
                                                "< " + QString::number( m_upperValue ) );
         }
      }
   }
   else
   {
      QMessageBox::warning( this, "Error", "Please enter a valid float value." );
   }

   if( !ok )
   {
      setFocus();
      selectAll();
   }
   return ok;
}

double PMFloatEdit::value() const
{
   return text().toDouble();
}

void PMFloatEdit::setValue( double d, int precision )
{
   QString str;

   str.setNum( d, 'g', precision );
   setText( str );
}

void PMFloatEdit::slotEditTextChanged( const QString& /*t*/ )
{
   emit dataChanged();
}





PMIntEdit::PMIntEdit( QWidget* parent )
      : QLineEdit( parent )
{
   m_bCheckLower = false;
   m_bCheckUpper = false;
   m_lowerValue = 0;
   m_upperValue = 0;

   connect( this, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotEditTextChanged( const QString& ) ) );
}

void PMIntEdit::setValidation( bool checkLower, int lowerValue,
                                 bool checkUpper, int upperValue )
{
   m_bCheckLower = checkLower;
   m_bCheckUpper = checkUpper;
   m_lowerValue = lowerValue;
   m_upperValue = upperValue;
}

bool PMIntEdit::isDataValid()
{
   bool ok = true;
   int i;
   double d;
   
   i = text().toInt( &ok );
   if( !ok )
   {
      d = text().toDouble( &ok );
      if( ok )
      {
         i = ( int ) d;
         QString str;
         bool b = signalsBlocked();
         blockSignals( true );
         str.setNum( i );
         setText( str );
         blockSignals( b );
      }
   }
   
   if( ok )
   {
      if( m_bCheckLower )
         ok = ok && ( i >= m_lowerValue );
      if( m_bCheckUpper )
         ok = ok && ( i <= m_upperValue );
      if( !ok )
      {
         if( m_bCheckLower && m_bCheckUpper )
            QMessageBox::warning( this, "Error", ( "Please enter an integer value "
                                            "between " + QString::number( m_lowerValue ) + " and " + QString::number( m_upperValue ) ) );
         else if( m_bCheckLower )
            QMessageBox::warning( this, "Error", ( "Please enter an integer value "
                                            ">= " + QString::number( m_lowerValue ) ) );
         else
            QMessageBox::warning( this, "Error", ( "Please enter an integer value "
                                            "<= " + QString::number( m_upperValue ) ) );
      }
   }
   else
   {
      QMessageBox::warning( this, "Error", "Please enter a valid integer value." );
   }

   if( !ok )
   {
      setFocus();
      selectAll();
   }
   return ok;
}

int PMIntEdit::value() const
{
   return text().toInt();
}

void PMIntEdit::setValue( int i )
{
   QString str;

   str.setNum( i );
   setText( str );
}

void PMIntEdit::slotEditTextChanged( const QString& /*t*/ )
{
   emit dataChanged();
}
#include "pmlineedits.moc"
