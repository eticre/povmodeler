/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmpovraysettings.h"

#include "pmdocumentationmap.h"
#include "pmpovrayrenderwidget.h"
#include "pmdefaults.h"
#include "pmresourcelocator.h"
#include "pmtext.h"

#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QList>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

PMPovraySettings::PMPovraySettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   m_selectionIndex = 0;

   QHBoxLayout* hlayout;
   QVBoxLayout* vlayout;
   QVBoxLayout* gvl;
   QGroupBox* gb;

   vlayout = new QVBoxLayout( this );

   vlayout->setMargin( 0 );

   gb = new QGroupBox( "Povray Command", this );
   gvl = new QVBoxLayout( gb );

   gvl->addSpacing( 10 );
   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( tr( "Command:" ), gb ) );
   m_pPovrayCommand = new QLineEdit( gb );
   hlayout->addWidget( m_pPovrayCommand );
   m_pBrowsePovrayCommand = new QPushButton( gb );
   m_pBrowsePovrayCommand->setIcon( QIcon::fromTheme( "document-open" ) );

   connect( m_pBrowsePovrayCommand,
            SIGNAL( clicked() ),
            SLOT( slotBrowsePovrayCommand() ) );

   hlayout->addWidget( m_pBrowsePovrayCommand );
   vlayout->addWidget( gb );

   gb = new QGroupBox( tr( "Povray User Documentation" ), this );
   gvl = new QVBoxLayout( gb );

   gvl->addSpacing( 10 );
   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( tr( "Path:" ), gb ) );
   m_pDocumentationPath = new QLineEdit( gb );
   hlayout->addWidget( m_pDocumentationPath );
   m_pBrowseDocumentationPath = new QPushButton( gb );
   m_pBrowseDocumentationPath->setIcon( QIcon::fromTheme( "document-open" ) );

   connect( m_pBrowseDocumentationPath,
            SIGNAL( clicked() ),
            SLOT( slotBrowsePovrayDocumentation() ) );

   hlayout->addWidget( m_pBrowseDocumentationPath );
   vlayout->addWidget( gb );
   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   hlayout->addWidget( new QLabel( "Version:", gb ) );
   m_pDocumentationVersion = new QComboBox( gb );
   QList<QString> versions = PMDocumentationMap::theMap()->availableVersions();
   QList<QString>::iterator it;

   for( it = versions.begin(); it != versions.end(); ++it )
      m_pDocumentationVersion->addItem( *it );

   hlayout->addWidget( m_pDocumentationVersion );
   hlayout->addStretch();

   gb = new QGroupBox( "Library Paths", this );
   gvl = new QVBoxLayout( gb );

   gvl->addSpacing( 10 );
   hlayout = new QHBoxLayout();
   gvl->addLayout( hlayout );
   m_pLibraryPaths = new QListWidget( gb );

   connect( m_pLibraryPaths, SIGNAL( currentRowChanged( int ) ),
                             SLOT( slotPathSelected( int ) ) );

   hlayout->addWidget( m_pLibraryPaths );

   QVBoxLayout* bl = new QVBoxLayout();
   hlayout->addLayout( bl );
   m_pAddLibraryPath = new QPushButton( "Add...", gb );

   connect( m_pAddLibraryPath, SIGNAL( clicked() ), SLOT( slotAddPath() ) );

   bl->addWidget( m_pAddLibraryPath );
   m_pRemoveLibraryPath = new QPushButton( "Remove", gb );

   connect( m_pRemoveLibraryPath, SIGNAL( clicked() ),
                                  SLOT( slotRemovePath() ) );

   bl->addWidget( m_pRemoveLibraryPath );
   m_pChangeLibraryPath = new QPushButton( tr( "Edit..." ), gb );

   connect( m_pChangeLibraryPath, SIGNAL( clicked() ),
                                  SLOT( slotEditPath() ) );

   bl->addWidget( m_pChangeLibraryPath );
   m_pLibraryPathUp = new QPushButton( tr( "Up" ), gb );

   connect( m_pLibraryPathUp, SIGNAL( clicked() ), SLOT( slotPathUp() ) );

   bl->addWidget( m_pLibraryPathUp );
   m_pLibraryPathDown = new QPushButton( tr( "Down" ), gb );

   connect( m_pLibraryPathDown, SIGNAL( clicked() ),
                                SLOT( slotPathDown() ) );

   bl->addWidget( m_pLibraryPathDown );
   bl->addStretch( 1 );
   vlayout->addWidget( gb );
    vlayout->addStretch( 1 );
}

void PMPovraySettings::displaySettings()
{
   m_pPovrayCommand->setText( PMPovrayRenderWidget::povrayCommand() );
   m_pDocumentationPath->setText( PMDocumentationMap::theMap()->povrayDocumentationPath() );
   int c = m_pDocumentationVersion->count();
   QString s = PMDocumentationMap::theMap()->documentationVersion();
   int i;
   for( i = 0; i < c; i++ )
      if( m_pDocumentationVersion->itemText( i ) == s )
         m_pDocumentationVersion->setCurrentIndex( i );

   bool sb = m_pLibraryPaths->signalsBlocked();
   m_pLibraryPaths->blockSignals( true );
   m_pLibraryPaths->clear();
   //const QStringList plist = PMPovrayRenderWidget::povrayLibraryPaths();
   m_pLibraryPaths->addItems( PMPovrayRenderWidget::povrayLibraryPaths() );
   /*QStringList::ConstIterator it = plist.constBegin();
   m_selectionIndex = -1;
   for( ; it != plist.constEnd(); ++it )
   {
      m_pLibraryPaths->addItem( *it );
      m_selectionIndex++;
   }*/

   m_pRemoveLibraryPath->setEnabled( false );
   m_pChangeLibraryPath->setEnabled( false );
   m_pLibraryPathUp->setEnabled( false );
   m_pLibraryPathDown->setEnabled( false );
   m_pLibraryPaths->blockSignals( sb );
}

void PMPovraySettings::displayDefaults()
{
   m_pPovrayCommand->setText( c_defaultPovrayCommand );
   m_pDocumentationVersion->setCurrentIndex( 0 );
   m_pDocumentationPath->setText( QString() );
   m_pLibraryPaths->clear();

}

bool PMPovraySettings::validateData()
{
   return true;
}

void PMPovraySettings::applySettings()
{
   PMPovrayRenderWidget::setPovrayCommand( m_pPovrayCommand->text() );
   PMDocumentationMap::theMap()->setPovrayDocumentationPath( m_pDocumentationPath->text() );
   PMDocumentationMap::theMap()->setDocumentationVersion( m_pDocumentationVersion->currentText() );
   QStringList plist;
   int num = ( signed ) m_pLibraryPaths->count();
   int i;
   for( i = 0; i < num; i++ )
       plist.append( m_pLibraryPaths->item( i )->text() );

   if( PMPovrayRenderWidget::povrayLibraryPaths() != plist )
   {
      PMPovrayRenderWidget::setPovrayLibraryPaths( plist );
      PMResourceLocator::clearCache();
      PMText::povrayLibraryPathsChanged();
      emit repaintViews();
   }
}

void PMPovraySettings::slotAddPath()
{
   if( m_pLibraryPaths->count() >= 20 )
   {
       QMessageBox::information( this, tr( "warning" ) ,
                    tr( "Povray only supports up to 20 library paths." ),
                    QMessageBox::Ok, QMessageBox::NoButton);
   }
   else
   {
      QUrl url;
      QString path = QFileDialog::getExistingDirectory( this ,"dir" , QString() );
      if( !path.isEmpty() )
      {
         QList<QListWidgetItem *>  item = m_pLibraryPaths->findItems( path, Qt::MatchExactly );
         if( item.count() < 1 )
         {
            m_pLibraryPaths->insertItem( m_selectionIndex + 1, path );
            m_pLibraryPaths->setCurrentRow( m_selectionIndex + 1 );
         }
         else
         {
             QMessageBox::information( this, tr( "warning" ) ,
                          tr( "The list of library paths already contains this path." ),
                          QMessageBox::Ok, QMessageBox::NoButton);
         }
      }
   }
}

void PMPovraySettings::slotRemovePath()
{
   m_pLibraryPaths->takeItem( m_selectionIndex );
   if( ( unsigned ) m_selectionIndex >= m_pLibraryPaths->count() )
      m_selectionIndex--;
   m_pLibraryPaths->setCurrentRow( m_selectionIndex );
}

void PMPovraySettings::slotPathUp()
{
   QListWidgetItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      QString text = lbi->text();
      m_pLibraryPaths->takeItem( m_selectionIndex );
      if( m_selectionIndex > 0 )
         m_selectionIndex--;
      m_pLibraryPaths->insertItem( m_selectionIndex, text );
      m_pLibraryPaths->setCurrentRow( m_selectionIndex );
   }
}

void PMPovraySettings::slotPathDown()
{
   QListWidgetItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      QString text = lbi->text();
      m_pLibraryPaths->takeItem( m_selectionIndex );
      if( ( unsigned ) m_selectionIndex < m_pLibraryPaths->count() )
         m_selectionIndex++;
      m_pLibraryPaths->insertItem( m_selectionIndex, text );
      m_pLibraryPaths->setCurrentRow( m_selectionIndex );
   }
}

void PMPovraySettings::slotEditPath()
{
   QListWidgetItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      QString text = lbi->text();
      QString path = QFileDialog::getExistingDirectory(this , QString(), text);
      if( !path.isEmpty() )
      {
         QList<QListWidgetItem *> item = m_pLibraryPaths->findItems( path, Qt::MatchExactly );
         if( item.count() < 1 )
            m_pLibraryPaths->item( m_selectionIndex )->setText( path );
         else if( item.first() != lbi )
         {
             QMessageBox::information( this, "warning",
                          "The list of library paths already contains this path.",
                          QMessageBox::Ok, QMessageBox::NoButton);
         }
      }
   }
}

void PMPovraySettings::slotPathSelected( int index )
{
   m_selectionIndex = index;
   QListWidgetItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      m_pRemoveLibraryPath->setEnabled( true );
      m_pChangeLibraryPath->setEnabled( true );
      m_pLibraryPathUp->setEnabled( index > 0 );
      m_pLibraryPathDown->setEnabled( index < ( ( signed ) m_pLibraryPaths->count() - 1 ) );
   }
   else
   {
      m_pRemoveLibraryPath->setEnabled( false );
      m_pChangeLibraryPath->setEnabled( false );
      m_pLibraryPathUp->setEnabled( false );
      m_pLibraryPathDown->setEnabled( false );
   }
}

void PMPovraySettings::slotBrowsePovrayCommand()
{
   //QUrl url;
   QString str = QFileDialog::getOpenFileName( this, "file", QString() );

   if( !str.isEmpty() )
   {
      m_pPovrayCommand->setText( str );
   }
}

void PMPovraySettings::slotBrowsePovrayDocumentation()
{
   QString str = QFileDialog::getExistingDirectory( 0, QString() );

   if( !str.isEmpty() )
      m_pDocumentationPath->setText( str );
}

#include "pmpovraysettings.moc"
