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


#include "pmdeclareedit.h"
#include "pmdeclare.h"
#include "pmpart.h"
#include "pmsymboltable.h"
#include "pmscanner.h"
#include "pmobjectselect.h"

#include <qlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtextstream.h>
#include <qpushbutton.h>
#include <QListWidget>
//Added by qt3to4:
#include <QHBoxLayout>


#include <QMessageBox>

PMDeclareEdit::PMDeclareEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
   m_pSelectedObject = nullptr;
}

void PMDeclareEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();

   QHBoxLayout* layout = new QHBoxLayout();
   tl->addLayout( layout );
   m_pNameEdit = new QLineEdit( this );
   m_pNameEdit->setMaxLength( 40 );
   QLabel* label = new QLabel( tr( "Identifier:" ), this );

   layout->addWidget( label );
   layout->addWidget( m_pNameEdit );

   connect( m_pNameEdit, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotNameChanged( const QString& ) ) );
}

void PMDeclareEdit::createBottomWidgets()
{
   QBoxLayout* tl = topLayout();
   QLabel* l = new QLabel( tr( "Linked objects:" ), this );
   tl->addWidget( l );

   m_pLinkedObjects = new QListWidget( this );
   m_pLinkedObjects->setMinimumHeight( 100 );
   connect( m_pLinkedObjects, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ),
            SLOT( slotItemSelected( QListWidgetItem* ) ) );
   tl->addWidget( m_pLinkedObjects, 1 );

   QHBoxLayout* layout = new QHBoxLayout();
   tl->addLayout( layout );
   m_pSelectButton = new QPushButton( tr( "Select..." ), this );
   m_pSelectButton->setEnabled( false );

   connect( m_pSelectButton, SIGNAL( clicked() ), SLOT( slotSelect() ) );
   layout->addStretch();
   layout->addWidget( m_pSelectButton );

   Base::createBottomWidgets();
}

void PMDeclareEdit::displayObject( PMObject* o )
{
   if( o->isA( "Declare" ) )
   {
      m_pDisplayedObject = ( PMDeclare* ) o;
      m_pNameEdit->setText( QString( m_pDisplayedObject->id() ) );

      m_pNameEdit->setReadOnly( m_pDisplayedObject->isReadOnly() );

		const PMObjectList& linked = m_pDisplayedObject->linkedObjects();
		PMObjectList::const_iterator it;
      m_pLinkedObjects->clear();

      for( it = linked.begin(); it != linked.end(); ++it )
         m_pLinkedObjects->addItem( new PMListBoxObject( *it ) );

      m_pSelectButton->setEnabled( false );
      m_pSelectedObject = nullptr;

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMDeclareEdit: Can't display object\n";
}

void PMDeclareEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      m_pDisplayedObject->setID( m_pNameEdit->text() );
   }
}

bool PMDeclareEdit::isDataValid()
{
   if( !Base::isDataValid() )
      return false;

   QString text = m_pNameEdit->text();
   if( text.length() == 0 )
   {
      QMessageBox::warning( this, "Please enter an identifier.",
                          "Error" );
      return false;
   }

   if( text == m_pDisplayedObject->id() )
      return true;

   QTextStream str( &text, QIODevice::ReadOnly );
   QChar c;
   int ac;
   bool ok = true;
   int i = 0;
   while( !str.atEnd() && ok )
   {
      str >> c;
      ac = c.toLatin1();
      // QChar::category can't be used because umlauts are not allowed
      if( i == 0 )
         ok = ( ( ( ac >= 'a' ) && ( ac <= 'z' ) ) ||
                ( ( ac >= 'A' ) && ( ac <= 'Z' ) ) ||
                ( ac == '_' ) );
      else
         ok  = ( ( ( ac >= 'a' ) && ( ac <= 'z' ) ) ||
                 ( ( ac >= 'A' ) && ( ac <= 'Z' ) ) ||
                 ( ( ac >= '0' ) && ( ac <= '9' ) ) ||
                 ( ac == '_' ) );
      i++;
   }
   if( !ok )
   {
      QMessageBox::warning( this, tr( "An identifier may consist of letters,"
                                      " digits and the underscore character"
                                      " ('_').\n"
                                      "The first character must be a letter"
                                      " or the underscore character." ),
                          tr( "Error" ) );
      return false;
   }
   // valid identifer!

   PMReservedWordDict* dict = PMScanner::reservedWords();
   if( dict->find( text ) != -1 )
   {
      QMessageBox::warning( this, tr( "You can not use a povray reserved word"
                                      " as an identifier." ), tr( "Error" ) );
      return false;
   }
   dict = PMScanner::directives();
   if( dict->find( text ) != -1 )
   {
      QMessageBox::warning( this, tr( "You can not use a povray directive"
                                      " as an identifier." ), tr( "Error" ) );
      return false;
   }

   // no reserved word
   PMSymbolTable* st = part()->symbolTable();
   if( st->contains( text ) )
   {
      QMessageBox::warning( this, tr( "Please enter a unique identifier." ),
                          tr( "Error" ) );
      return false;
   }
   return true;
}

void PMDeclareEdit::slotNameChanged( const QString& )
{
   emit dataChanged();
}

void PMDeclareEdit::slotItemSelected( QListWidgetItem* item )
{
	if(item)
	{
		m_pSelectedObject = ( ( PMListBoxObject* ) item )->object();
		m_pSelectButton->setEnabled( true );
	}
	else
	{
        m_pSelectedObject = nullptr;
		m_pSelectButton->setEnabled( false );
	}
}

void PMDeclareEdit::slotSelect()
{
   if( m_pSelectedObject )
      part()->slotObjectChanged( m_pSelectedObject, PMCNewSelection, this );
}
#include "pmdeclareedit.moc"
