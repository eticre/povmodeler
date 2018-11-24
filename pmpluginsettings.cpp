/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003-2006 by Andreas Zehender
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

#include "pmpluginsettings.h"

#include "pmpluginmanager.h"

#include <qlayout.h>
#include <QGroupBox>
#include <QTableWidget>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>

class PMPluginListViewItem : public QTableWidget //one table rows=1 instead of q3list
{
public:
   PMPluginListViewItem( QTableWidget* parent, PMPluginInfo* info )
       : QTableWidget( 1, 2, parent/*, info->description()*/ )
   {
      m_info = info;
      m_enabled = info->enabled();
      setStatus();
   }
   void toggleStatus()
   {
      m_enabled = !m_enabled;
      setStatus();
   }
   void setStatus()
   {
      if( m_enabled )
         item( 0, 2 )->setText( "loaded" );
      else
         item( 0, 2 )->setText( "deactivated" );
   }
   PMPluginInfo* m_info;
   bool m_enabled;
};

PMPluginSettings::PMPluginSettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QVBoxLayout* vlayout = new QVBoxLayout( this );
//TODO PORT QT5    vlayout->setSpacing( QDialog::spacingHint() );
   vlayout->setMargin( 0 );

   QGroupBox* gb = new QGroupBox( tr( "Installed Plugins" ), this );
   vlayout->addWidget( gb );

   QVBoxLayout* gvl = new QVBoxLayout( gb );
//TODO PORT QT5    gvl->setSpacing( QDialog::spacingHint() );
//TODO PORT QT5    gvl->setMargin( QDialog::marginHint() );
   gvl->addSpacing( 10 );

   m_pPluginsList = new QTableWidget( gb );//container table
   connect( m_pPluginsList, SIGNAL( selectionChanged() ),
            SLOT( slotSelectionChanged() ) );
   m_pPluginsList->setColumnCount( 3 );
   m_pPluginsList->horizontalHeaderItem( 0 )->setText( "name" );
   m_pPluginsList->horizontalHeaderItem( 1 )->setText( "Description"  );
   m_pPluginsList->horizontalHeaderItem( 2 )->setText( "Status" );
   gvl->addWidget( m_pPluginsList, 1 );

   QHBoxLayout* hl = new QHBoxLayout();
   gvl->addLayout( hl );
   m_pToggle = new QPushButton( tr( "Load" ), gb );
   m_pToggle->setEnabled( false );
   connect( m_pToggle, SIGNAL( clicked() ), SLOT( slotToggle() ) );
   hl->addWidget( m_pToggle );
   hl->addStretch( 1 );

   vlayout->addStretch( 1 );
}

void PMPluginSettings::displaySettings()
{
   QList<PMPluginInfo*> plugins = PMPluginManager::theManager()->plugins();
   QList<PMPluginInfo*>::const_iterator it;

   m_pPluginsList->clear();
   for(it = plugins.constBegin(); it != plugins.constEnd(); ++it )
      new PMPluginListViewItem( m_pPluginsList, *it );
}

void PMPluginSettings::applySettings()
{
   bool changes = false;
   /* todo
    *PMPluginListViewItem* item =
      ( PMPluginListViewItem* ) m_pPluginsList->firstChild();//first row?
   for( ; item; item = ( PMPluginListViewItem* ) item->nextSibling() )
   {
      if( item->m_enabled != item->m_info->enabled() )
      {
         item->m_info->enable( item->m_enabled );
         changes = true;
      }
   }
   if( changes )
      PMPluginManager::theManager()->updatePlugins();*/
}

bool PMPluginSettings::validateData()
{
   return true;
}

void PMPluginSettings::displayDefaults()
{
}

void PMPluginSettings::slotToggle()
{
   PMPluginListViewItem* item =
      ( PMPluginListViewItem* ) m_pPluginsList->currentItem();
   if( item )
   {
      item->toggleStatus();
   
      if( item->m_enabled )
         m_pToggle->setText( tr( "Deactivate" ) );
      else
         m_pToggle->setText( tr( "Load" ) );
   }
}

void PMPluginSettings::slotSelectionChanged()
{
   PMPluginListViewItem* item =
      ( PMPluginListViewItem* ) m_pPluginsList->currentItem();
   if( item )
   {
      m_pToggle->setEnabled( true );
      if( item->m_enabled )
         m_pToggle->setText( tr( "Deactivate" ) );
      else
         m_pToggle->setText( tr( "Load" ) );
   }
   else
      m_pToggle->setEnabled( false );
}

#include "pmpluginsettings.moc"
