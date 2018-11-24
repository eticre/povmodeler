/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002-2006 by Andreas Zehender
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


#include "pmpolynomedit.h"
#include "pmpolynom.h"
#include "pmpolynomexponents.h"
#include "pmlineedits.h"
#include "pmformulalabel.h"

#include <qlayout.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <QList>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>


PMPolynomEdit::PMPolynomEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
   m_currentOrder = 0;
   m_readOnly = false;
}

void PMPolynomEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Order" ), this ) );
   m_pOrder = new QSpinBox( this );
   m_pOrder->setMinimum( 2 );
   m_pOrder->setMaximum( 7 );
   hl->addWidget( m_pOrder );
   hl->addStretch( 1 );
   connect( m_pOrder, SIGNAL( valueChanged( int ) ), SLOT( slotOrderChanged( int ) ) );
   
   tl->addWidget( new QLabel( tr( "Formula:" ), this ) );
   m_pPolyWidget = new QWidget( this );
   tl->addWidget( m_pPolyWidget );
   m_pSturm = new QCheckBox( tr( "Sturm" ), this );
   tl->addWidget( m_pSturm );
   connect( m_pSturm, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
}

void PMPolynomEdit::displayObject( PMObject* o )
{
   if( o->isA( "Polynom" ) )
   {
      bool readOnly = o->isReadOnly();
      m_readOnly = readOnly;
      m_pDisplayedObject = ( PMPolynom* ) o;

      displayCoefficients( m_pDisplayedObject->coefficients(),
                           m_pDisplayedObject->polynomOrder(),
                           m_pDisplayedObject->polynomOrder() );
      
      m_pSturm->setChecked( m_pDisplayedObject->sturm() );
      m_pSturm->setEnabled( !readOnly );
      if( m_pDisplayedObject->polynomOrder() == 2 )
         m_pSturm->hide();
      else
         m_pSturm->show();

      bool sb = m_pOrder->signalsBlocked();
      m_pOrder->blockSignals( true );
      m_pOrder->setValue( m_pDisplayedObject->polynomOrder() );
      m_pOrder->blockSignals( sb );
      
      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMPolynomEdit: Can't display object\n";
}

void PMPolynomEdit::displayCoefficients( const PMVector& co, int cOrder,
                                         int dOrder )
{
   QList<PMPolynomExponents>& polynom
      = PMPolynomExponents::polynom( dOrder );
      
   if( dOrder != m_currentOrder )
   {
		hide();
      if( m_currentOrder > 0 )
      {
         if( m_pPolyWidget->layout() )
            delete m_pPolyWidget->layout();

			foreach(QWidget* w, m_labels) delete w;
         m_labels.clear();
			foreach(PMFloatEdit* w, m_edits) delete w;
         m_edits.clear();
      }

      int nedit = polynom.count();
      int nr = ( nedit + 2 ) / 3;
      int i;
      
      QGridLayout* gl = new QGridLayout( m_pPolyWidget );
      gl->setSpacing( 0 );
      QLabel* l = 0;
      PMFloatEdit* edit;
      PMFormulaLabel* fl;
      
      QList<PMPolynomExponents>::ConstIterator it = polynom.constBegin();
      QString text;
      int row, col;
      QString plus( "+" );
      
      for( i = 0; it != polynom.constEnd(); i++, ++it )
      {
         row = ( i / 3 ) * 2;
         col = ( i % 3 ) * 3;

         if( i != 0 )
         {
            l = new QLabel( plus, m_pPolyWidget );
            m_labels.append( l );
            gl->addWidget( l, row, col );
            l->show();
         }
         
         edit = new PMFloatEdit( m_pPolyWidget );
         connect( edit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
         m_edits.append( edit );
         gl->addWidget( edit, row, col + 1 );
         edit->show();
         edit->setReadOnly( m_readOnly );

         fl = new PMFormulaLabel( *it, m_pPolyWidget );
         
         m_labels.append( fl );
         gl->addWidget( fl, row, col + 2 );
         fl->show();
      }
      for( i = 0; i < ( nr - 1 ); i++ )
//TODO PORT QT5          gl->addItem( new QSpacerItem( 0, QDialog::spacingHint() ), i * 2 + 1, 0 );
		show();
      
      emit sizeChanged();
   }
   m_currentOrder = dOrder;

   
   if( dOrder == cOrder )
   {
		QList<PMFloatEdit*>::Iterator eit;
      int i;
      for( i = 0, eit = m_edits.begin(); eit != m_edits.end(); ++eit, ++i )
         ( *eit )->setValue( co[i] );
   }
   else if( dOrder > cOrder )
   {
      QList<PMPolynomExponents>::ConstIterator dit = polynom.constBegin();
      QList<PMPolynomExponents>& cpoly
         = PMPolynomExponents::polynom( cOrder );
      QList<PMPolynomExponents>::ConstIterator cit = cpoly.constBegin();
		QList<PMFloatEdit*>::Iterator eit = m_edits.begin();
      int i = 0;
      
      for( ; ( eit != m_edits.end() ) && ( dit != polynom.constEnd() ) && ( cit != cpoly.constEnd() ); ++dit, ++eit )
      {
         if( *dit == *cit )
         {
            ( *eit )->setValue( co[i] );
            i++;
            cit++;
         }
         else
            ( *eit )->setValue( 0.0 );
      }
      if( ( dit != polynom.constEnd() ) || ( cit != cpoly.constEnd() ) )
         qCritical(  ) << "displayExponents is buggy!\n";
   } 
   else // dOrder < cOrder
   {
      QList<PMPolynomExponents>::ConstIterator dit = polynom.constBegin();
      QList<PMPolynomExponents>& cpoly
         = PMPolynomExponents::polynom( cOrder );
      QList<PMPolynomExponents>::ConstIterator cit = cpoly.constBegin();
		QList<PMFloatEdit*>::Iterator eit = m_edits.begin();
      int i = 0;
      
      for( ; ( eit != m_edits.end() ) && ( dit != polynom.constEnd() ) && ( cit != cpoly.constEnd() ); ++cit, ++i )
      {
         if( *dit == *cit )
         {
            ( *eit )->setValue( co[i] );
            ++eit;
            ++dit;
         }
      }
      if( ( dit != polynom.constEnd() ) || ( cit != cpoly.constEnd() ) )
         qCritical(  ) << "displayExponents is buggy!\n";
   }
}

PMVector PMPolynomEdit::coefficients() const
{
	QList<PMFloatEdit*>::ConstIterator eit;
   int num = m_edits.count();
   PMVector v( num );
   int i;
   
   for( eit = m_edits.begin(), i = 0 ; eit != m_edits.end(); ++eit, ++i )
      v[i] = ( *eit )->value();
   return v;
}

void PMPolynomEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setPolynomOrder( m_pOrder->value() );
      m_pDisplayedObject->setCoefficients( coefficients() );
      
      Base::saveContents();

      m_pDisplayedObject->setSturm( m_pSturm->isChecked() );
   }
}

bool PMPolynomEdit::isDataValid()
{
	QList<PMFloatEdit*>::Iterator eit;
   for( eit = m_edits.begin(); eit != m_edits.end(); ++eit )
      if( !( *eit )->isDataValid() )
         return false;
   return Base::isDataValid();
}

void PMPolynomEdit::slotOrderChanged( int order )
{
   if( order == 2 )
      m_pSturm->hide();
   else
      m_pSturm->show();
   
   displayCoefficients( coefficients(), m_currentOrder, order );
}

#include "pmpolynomedit.moc"
