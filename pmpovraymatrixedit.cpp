/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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


#include "pmpovraymatrixedit.h"
#include "pmpovraymatrix.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QGridLayout>



PMPovrayMatrixEdit::PMPovrayMatrixEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMPovrayMatrixEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();

   int i, r, c;
   QGridLayout* gl = new QGridLayout();
   tl->addLayout( gl );

   for( i = 0; i < 12; i++ )
   {
      m_pValue[i] = new PMFloatEdit( this );
      connect( m_pValue[i], SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   }

   for( r = 0; r < 4; r++ )
      for( c = 0; c < 3; c++ )
         gl->addWidget( m_pValue[r*3+c], r, c );
   gl->addWidget( new QLabel( "0.0", this ), 0, 3 );
   gl->addWidget( new QLabel( "0.0", this ), 1, 3 );
   gl->addWidget( new QLabel( "0.0", this ), 2, 3 );
   gl->addWidget( new QLabel( "1.0", this ), 3, 3 );
}

void PMPovrayMatrixEdit::displayObject( PMObject* o )
{
   if( o->isA( "PovrayMatrix" ) )
   {
      bool readOnly = o->isReadOnly();
      int i;
      m_pDisplayedObject = ( PMPovrayMatrix* ) o;
      PMVector v = m_pDisplayedObject->values();

      for( i = 0; i < 12; i++ )
      {
         m_pValue[i]->setValue( v[i] );
         m_pValue[i]->setReadOnly( readOnly );
      }

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMPovrayMatrixEdit: Can't display object\n";
}

void PMPovrayMatrixEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      PMVector v( 12 );
      int i;

      for( i = 0; i < 12; i++ )
         v[i] = m_pValue[i]->value();
      m_pDisplayedObject->setValues( v );
   }
}

bool PMPovrayMatrixEdit::isDataValid()
{
   int i;

   for( i = 0; i < 12; i++ )
      if( !m_pValue[i]->isDataValid() )
         return false;

   return Base::isDataValid();
}

#include "pmpovraymatrixedit.moc"
