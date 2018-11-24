/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2006 by Andreas Zehender
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


#include "pmtexturemapedit.h"
#include "pmtexturemap.h"
#include "pmlineedits.h"

#include <qlayout.h>
#include <qlabel.h>
#include <QVBoxLayout>
#include <QList>
#include <QHBoxLayout>

#include <QMessageBox>
#include <pmpattern.h>


PMTextureMapEdit::PMTextureMapEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
   m_numValues = 0;
}

void PMTextureMapEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();
   tl->addWidget( new QLabel( tr( "Map values:" ), this ) );
   m_pNoChildLabel = new QLabel( tr( "(No Child Objects)" ), this );
   m_pPureLinkLabel = new QLabel( tr( "(Pure Link)" ), this );
   tl->addWidget( m_pNoChildLabel );
   tl->addWidget( m_pPureLinkLabel );
   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pEditLayout = new QVBoxLayout();
   hl->addLayout( m_pEditLayout );
   hl->addStretch( 1 );
}

void PMTextureMapEdit::displayObject( PMObject* o )
{
   //QString str;

   if( o->isA( "TextureMapBase" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMTextureMapBase* ) o;
      QList<double> mv = m_pDisplayedObject->mapValues();
      QList<double>::Iterator vit = mv.begin();
      //QList<PMFloatEdit*>::Iterator eit = m_edits.begin();
      PMFloatEdit* edit;
		int i;

      m_numValues = mv.count();
		if( m_edits.count() < mv.count() )
		{
			for( i = m_edits.count(); i < mv.count(); i++ )
			{
            edit = new PMFloatEdit( this );
            m_pEditLayout->addWidget( edit );
            m_edits.append( edit );
            PMPattern* p =  ( PMPattern* ) o->prevSibling();
            if( p && p->patternType() == PMPattern::PatternAverage )
               edit->setValidation( true, 0.0, true, 9999.0 );
            else
               edit->setValidation( true, 0.0, true, 1.0 );
            connect( edit, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
			}
		}
		if( m_edits.count() > mv.count() )
			for( i = mv.count(); i < m_edits.count(); i++ )
				m_edits[i]->hide();

      for( i = 0; vit != mv.end(); ++vit, i++ )
      {
			m_edits[i]->setValue( *vit );
			m_edits[i]->show();
			m_edits[i]->setReadOnly( readOnly );
		}
      if( m_numValues == 0 )
      {
         if( o->linkedObject() )
         {
            m_pPureLinkLabel->show();
            m_pNoChildLabel->hide();
         }
         else
         {
            m_pPureLinkLabel->hide();
            m_pNoChildLabel->show();
         }
      }
      else
      {
         m_pNoChildLabel->hide();
         m_pPureLinkLabel->hide();
      }
   }
   else
      qCritical(  ) << "PMTextureMapEdit: Can't display object\n";
   Base::displayObject( o );
   enableLinkEdit( m_numValues == 0 );
}

void PMTextureMapEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      if( m_numValues > 0 )
      {
         QList<PMFloatEdit*>::Iterator it;
         int i = 0;
         QList<double> values;
         
         for( it = m_edits.begin(); ( i < m_numValues ) && it != m_edits.end(); ++i, ++it )
            values.append( ( *it )->value() );
         m_pDisplayedObject->setMapValues( values );
      }
      Base::saveContents();
   }
}

bool PMTextureMapEdit::isDataValid()
{
   int i = 0;
   double last = 0.0;
   PMPattern* p =  ( PMPattern* ) m_pDisplayedObject->prevSibling();
   QList<PMFloatEdit*>::Iterator it;
   for( it = m_edits.begin(); ( i < m_numValues ) && it != m_edits.end(); ++i, ++it )
   {
      if( !( *it )->isDataValid() )
         return false;
      if( ( *it )->value() < last && p->patternType() != PMPattern::PatternAverage )
      {
         QMessageBox::warning( this, tr( "The map values have to be increasing." ),
                          tr( "Error" ) );
         ( *it )->setFocus();
         return false;
      }
      last = ( *it )->value();
   }
   return Base::isDataValid();
}

#include "pmtexturemapedit.moc"
