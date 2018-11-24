//-*-C++-*-
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

#ifndef PM_LINEEDITS_H
#define PM_LINEEDITS_H



#include <qlineedit.h>

/**
 * Lineedit for float input
 */

class PMFloatEdit : public QLineEdit
{
   Q_OBJECT
public:
   enum ValidationOp { OpGreater, OpGreaterEqual,
                       OpLess, OpLessEqual };
   /**
    * Simple constructor
    */
   PMFloatEdit( QWidget* parent );
   /**
    * Sets the validation for the lineedit.
    *
    * If checkLower is true, the value has to be >= the lowerValue.
    *
    * If checkUpper is true, the value has to be <= the upperValue.
    *
    * By default no range check is made.
    */
   void setValidation( bool checkLower, double lowerValue,
                       bool checkUpper, double upperValue );
   /**
    * Sets the validation operators for the lower and upper value.
    *
    * Valid values for lower are OpGreater and OpGreaterEqual,
    * valid values for upper are OpLess and OpLessEqual.
    */
   void setValidationOperator( ValidationOp lower, ValidationOp upper );
   /**
    * Returns true, if the text is a valid float in the valid range
    */
   bool isDataValid();
   /**
    * Returns the float value
    */
   double value() const;
   /**
    * Sets the value
    */
   void setValue( double d, int precision = 5 );
signals:
   /**
    * emitted if the text is changed
    */
   void dataChanged();
public slots:
   void slotEditTextChanged( const QString& t );
private:
   bool m_bCheckLower, m_bCheckUpper;
   double m_lowerValue, m_upperValue;
   ValidationOp m_lowerOp, m_upperOp;
};


/**
 * Lineedit for int input
 */

class PMIntEdit : public QLineEdit
{
   Q_OBJECT
public:
   /**
    * Simple constructor
    */
   PMIntEdit( QWidget* parent );
   /**
    * Sets the validation for the lineedit.
    *
    * If checkLower is true, the value has to be >= the lowerValue.
    *
    * If checkUpper is true, the value has to be <= the upperValue.
    *
    * By default no range check is made.
    */
   void setValidation( bool checkLower, int lowerValue,
                       bool checkUpper, int upperValue );
   /**
    * Returns true, if the text is a valid integer in the valid range
    */
   bool isDataValid();
   /**
    * Returns the integer value
    */
   int value() const;
   /**
    * Sets the value
    */
   void setValue( int i );
signals:
   /**
    * emitted if the text is changed
    */
   void dataChanged();
public slots:
   void slotEditTextChanged( const QString& t );
private:
   bool m_bCheckLower, m_bCheckUpper;
   int m_lowerValue, m_upperValue;
};

#endif
