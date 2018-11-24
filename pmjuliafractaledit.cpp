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


#include "pmjuliafractaledit.h"
#include "pmjuliafractal.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>

#include <QMessageBox>

PMJuliaFractalEdit::PMJuliaFractalEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
}

void PMJuliaFractalEdit::createTopWidgets()
{
   Base::createTopWidgets();
   
   QHBoxLayout* hl;
   QGridLayout* gl;
   QBoxLayout* tl = topLayout();

   tl->addWidget( new QLabel( tr( "Julia parameter:" ), this ) );
   m_pJuliaParameter = new PMVectorEdit( "", "i", "j", "k", this );
   tl->addWidget( m_pJuliaParameter );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Algebra type:" ), this ) );
   m_pAlgebraType = new QComboBox( this );
   m_pAlgebraType->addItem( tr( "Quaternion" ) );
   m_pAlgebraType->addItem( tr( "Hypercomplex" ) );
   hl->addWidget( m_pAlgebraType );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Function type:" ), this ) );
   m_pFunctionType = new QComboBox( this );
   m_pFunctionType->addItem( "sqr" );
   m_pFunctionType->addItem( "cube" );
   m_pFunctionType->addItem( "exp" );
   m_pFunctionType->addItem( "reciprocal" );
   m_pFunctionType->addItem( "sin" );
   m_pFunctionType->addItem( "asin" );
   m_pFunctionType->addItem( "sinh" );
   m_pFunctionType->addItem( "asinh" );
   m_pFunctionType->addItem( "cos" );
   m_pFunctionType->addItem( "acos" );
   m_pFunctionType->addItem( "cosh" );
   m_pFunctionType->addItem( "acosh" );
   m_pFunctionType->addItem( "tan" );
   m_pFunctionType->addItem( "atan" );
   m_pFunctionType->addItem( "tanh" );
   m_pFunctionType->addItem( "atanh" );
   m_pFunctionType->addItem( "log" );
   m_pFunctionType->addItem( "pwr" );
   hl->addWidget( m_pFunctionType );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pExponentsLabel = new QLabel( tr( "Exponent:" ), this );
   hl->addWidget( m_pExponentsLabel );
   m_pExponents = new PMVectorEdit( "", "i", this );
   hl->addWidget( m_pExponents );
   hl->addStretch( 1 );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Maximum iterations:" ), this ), 0, 0 );
   m_pMaxIterations = new PMIntEdit( this );
   m_pMaxIterations->setValidation( true, 1, false, 0 );
   gl->addWidget( m_pMaxIterations, 0, 1 );
   gl->addWidget( new QLabel( tr( "Precision:" ), this ), 1, 0 );
   m_pPrecision = new PMFloatEdit( this );
   m_pPrecision->setValidation( true, 1.0, false, 0.0 );
   gl->addWidget( m_pPrecision, 1, 1 );
   hl->addStretch( 1 );
   
   tl->addWidget( new QLabel( tr( "Slice normal:" ), this ) );
   m_pSliceNormal = new PMVectorEdit( "", "i", "j", "k", this );
   tl->addWidget( m_pSliceNormal );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Slice distance:" ), this ) );
   m_pSliceDistance = new PMFloatEdit( this );
   hl->addWidget( m_pSliceDistance );
   hl->addStretch( 1 );

   connect( m_pJuliaParameter, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pAlgebraType, SIGNAL( activated( int ) ),
            SLOT( slotAlgebraTypeSelected( int ) ) );
   connect( m_pFunctionType, SIGNAL( activated( int ) ),
            SLOT( slotFunctionTypeSelected( int ) ) );
   connect( m_pExponents, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pMaxIterations, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pPrecision, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSliceNormal, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   connect( m_pSliceDistance, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );   
}

void PMJuliaFractalEdit::displayObject( PMObject* o )
{
   if( o->isA( "JuliaFractal" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMJuliaFractal* ) o;

      m_pJuliaParameter->setVector( m_pDisplayedObject->juliaParameter() );
      switch( m_pDisplayedObject->algebraType() )
      {
         case PMJuliaFractal::Quaternion:
            m_pAlgebraType->setCurrentIndex( 0 );
            break;
         case PMJuliaFractal::Hypercomplex:
            m_pAlgebraType->setCurrentIndex( 1 );
            break;
      }
      switch( m_pDisplayedObject->functionType() )
      {
         case PMJuliaFractal::FTsqr:
            m_pFunctionType->setCurrentIndex( 0 );
            break;
         case PMJuliaFractal::FTcube:
            m_pFunctionType->setCurrentIndex( 1 );
            break;
         case PMJuliaFractal::FTexp:
            m_pFunctionType->setCurrentIndex( 2 );
            break;
         case PMJuliaFractal::FTreciprocal:
            m_pFunctionType->setCurrentIndex( 3 );
            break;
         case PMJuliaFractal::FTsin:
            m_pFunctionType->setCurrentIndex( 4 );
            break;
         case PMJuliaFractal::FTasin:
            m_pFunctionType->setCurrentIndex( 5 );
            break;
         case PMJuliaFractal::FTsinh:
            m_pFunctionType->setCurrentIndex( 6 );
            break;
         case PMJuliaFractal::FTasinh:
            m_pFunctionType->setCurrentIndex( 7 );
            break;
         case PMJuliaFractal::FTcos:
            m_pFunctionType->setCurrentIndex( 8 );
            break;
         case PMJuliaFractal::FTacos:
            m_pFunctionType->setCurrentIndex( 9 );
            break;
         case PMJuliaFractal::FTcosh:
            m_pFunctionType->setCurrentIndex( 10 );
            break;
         case PMJuliaFractal::FTacosh:
            m_pFunctionType->setCurrentIndex( 11 );
            break;
         case PMJuliaFractal::FTtan:
            m_pFunctionType->setCurrentIndex( 12 );
            break;
         case PMJuliaFractal::FTatan:
            m_pFunctionType->setCurrentIndex( 13 );
            break;
         case PMJuliaFractal::FTtanh:
            m_pFunctionType->setCurrentIndex( 14 );
            break;
         case PMJuliaFractal::FTatanh:
            m_pFunctionType->setCurrentIndex( 15 );
            break;
         case PMJuliaFractal::FTlog:
            m_pFunctionType->setCurrentIndex( 16 );
            break;
         case PMJuliaFractal::FTpwr:
            m_pFunctionType->setCurrentIndex( 17 );
            break;
      }
      m_pExponents->setVector( m_pDisplayedObject->exponent() );
      if( m_pDisplayedObject->functionType() == PMJuliaFractal::FTpwr )
      {
         m_pExponents->show();
         m_pExponentsLabel->show();
      }
      else
      {
         m_pExponents->hide();
         m_pExponentsLabel->hide();
      }
      
      m_pMaxIterations->setValue( m_pDisplayedObject->maximumIterations() );
      m_pPrecision->setValue( m_pDisplayedObject->precision() );
      m_pSliceNormal->setVector( m_pDisplayedObject->sliceNormal() );
      m_pSliceDistance->setValue( m_pDisplayedObject->sliceDistance() );

      m_pJuliaParameter->setReadOnly( readOnly );
      m_pAlgebraType->setEnabled( !readOnly );
      m_pFunctionType->setEnabled( !readOnly );
      m_pExponents->setReadOnly( readOnly );
      m_pMaxIterations->setReadOnly( readOnly );
      m_pPrecision->setReadOnly( readOnly );
      m_pSliceNormal->setReadOnly( readOnly );
      m_pSliceDistance->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMJuliaFractalEdit: Can't display object\n";
}

void PMJuliaFractalEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      PMVector p( 4 );

      m_pDisplayedObject->setJuliaParameter( m_pJuliaParameter->vector() );
      switch( m_pAlgebraType->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setAlgebraType( PMJuliaFractal::Quaternion );
            break;
         case 1:
            m_pDisplayedObject->setAlgebraType( PMJuliaFractal::Hypercomplex );
            break;
         default:
            m_pDisplayedObject->setAlgebraType( PMJuliaFractal::Quaternion );
            break;
      }
      switch( m_pFunctionType->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTsqr );
            break;
         case 1:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTcube );
            break;
         case 2:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTexp );
            break;
         case 3:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTreciprocal );
            break;
         case 4:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTsin );
            break;
         case 5:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTasin );
            break;
         case 6:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTsinh );
            break;
         case 7:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTasinh );
            break;
         case 8:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTcos );
            break;
         case 9:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTacos );
            break;
         case 10:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTcosh );
            break;
         case 11:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTacosh );
            break;
         case 12:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTtan );
            break;
         case 13:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTatan );
            break;
         case 14:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTtanh );
            break;
         case 15:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTatanh );
            break;
         case 16:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTlog );
            break;
         case 17:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTpwr );
            break;
         default:
            m_pDisplayedObject->setFunctionType( PMJuliaFractal::FTsqr );
            break;
      }
      if( m_pExponents->isVisible() )
         m_pDisplayedObject->setExponent( m_pExponents->vector() );
      m_pDisplayedObject->setMaximumIterations( m_pMaxIterations->value() );
      m_pDisplayedObject->setPrecision( m_pPrecision->value() );
      m_pDisplayedObject->setSliceNormal( m_pSliceNormal->vector() );
      m_pDisplayedObject->setSliceDistance( m_pSliceDistance->value() );
   }
}

bool PMJuliaFractalEdit::isDataValid()
{
   if( !m_pJuliaParameter->isDataValid() )
      return false;
   if( m_pExponents->isVisible() )
      if( !m_pExponents->isDataValid() )
         return false;
   if( !m_pMaxIterations->isDataValid() )
      return false;
   if( !m_pPrecision->isDataValid() )
      return false;
   if( !m_pSliceNormal->isDataValid() )
      return false;
   PMVector v = m_pSliceNormal->vector();
   if( approxZero( v.abs() ) )
   {
      QMessageBox::warning( this, tr( "The slice normal vector may not be a null vector." ),
                          tr( "Error" ) );
      return false;
   }
   if( approxZero( v[3] ) )
   {
      QMessageBox::warning( this, tr( "The 'k' component of the slice normal vector may not be zero." ),
                          tr( "Error" ) );
      return false;
   }
   
   if( !m_pSliceDistance->isDataValid() )
      return false;

   if( m_pAlgebraType->currentIndex() == 0 )
   {
      if( m_pFunctionType->currentIndex() > 1 )
      {
         QMessageBox::warning( this, tr( "Only the functions 'sqr' and 'cube' "
                                         "are defined in the quaternion algebra." ),
                             tr( "Error" ) );
         return false;
      }
   }
         
   
   return Base::isDataValid();
}

void PMJuliaFractalEdit::slotAlgebraTypeSelected( int )
{
   emit dataChanged();
}

void PMJuliaFractalEdit::slotFunctionTypeSelected( int index )
{
   if( index == 17 )
   {
      m_pExponents->show();
      m_pExponentsLabel->show();
   }
   else
   {
      m_pExponents->hide();
      m_pExponentsLabel->hide();
   }
   emit dataChanged();
}

#include "pmjuliafractaledit.moc"
