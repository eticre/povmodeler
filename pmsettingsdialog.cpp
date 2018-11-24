/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2003 by Andreas Zehender
    email                : zehender@kde.org
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmsettingsdialog.h"

#define KPM_WITH_OBJECT_LIBRARY

#include "pmpovraysettings.h"
#include "pmcolorsettings.h"
#include "pmgridsettings.h"
#include "pmobjectsettings.h"
#include "pmpreviewsettings.h"
#include "pmlayoutsettings.h"
#ifdef  KPM_WITH_OBJECT_LIBRARY
#include "pmobjectlibrarysettings.h"
#endif
#include "pmpluginsettings.h"
#include "pmopenglsettings.h"

#include "pmrendermanager.h"
#include "pmdebug.h"

#include <QVBoxLayout>
#include <QResizeEvent>
#include <QIcon>
#include <QSettings>
#include <QStandardPaths>

PMSettingsDialogPage::PMSettingsDialogPage( QWidget* parent )
      : QWidget( parent )
{
}

QSize PMSettingsDialog::s_size = QSize( 640, 400 );

PMSettingsDialog::PMSettingsDialog( PMPart* part, QWidget* parent )
      : QDialog( parent )
{
    tbw = new QTabWidget;

    setWindowTitle( "Configure" );

    buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel
                                   | QDialogButtonBox::Reset
                                   | QDialogButtonBox::Apply );

    PMSettingsDialogPage* page;
    m_pPart = part;
    QStringList strpath;
    strpath << QStandardPaths::HomeLocation + "/.local/share/povmodeler/icons/povicons";
    QIcon::setThemeSearchPaths( strpath );
    QIcon::setThemeName("povicons");
    page = new PMPovraySettings();
    int ipovray = tbw->addTab( page, "Povray" );
    connect( page, SIGNAL( repaintViews() ), this, SLOT( slotRepaint() ) );
    tbw->setTabIcon( 0, QIcon::fromTheme( "pmconfigurepovray" ) );
    //ipovray->setHeader( "Povray Options" );
    m_pages.append( PMRegisteredSettingsPage( ipovray, page ) );

    page = new PMOpenGLSettings();
    int igl = tbw->addTab( page, "OpenGL" );
    //igl->setIcon( QIcon::fromTheme( "pmconfigureopengl" ) );
    //igl->setHeader( "OpenGL Display Settings" );
    m_pages.append( PMRegisteredSettingsPage( igl, page ) );

    page = new PMColorSettings();
    int icolor = tbw->addTab( page, "Colors" );
    connect( page, SIGNAL( repaintViews() ), this, SLOT( slotRepaint() ) );
    //icolor->setIcon( QIcon::fromTheme( "pmconfiguregraphicalview" ) );
    //icolor->setHeader( "Color Settings" );
    m_pages.append( PMRegisteredSettingsPage( icolor, page ) );

    page = new PMGridSettings();
    int igrid = tbw->addTab( page, "Grid" );
    connect( page, SIGNAL( repaintViews() ), this, SLOT( slotRepaint() ) );
    //igrid->setIcon( QIcon::fromTheme( "pmconfiguregrid" ) );
    //igrid->setHeader( "Grid Settings" );
    m_pages.append( PMRegisteredSettingsPage( igrid, page ) );

    page = new PMObjectSettings();
    int iobjects = tbw->addTab( page, "Objects" );
    connect( page, SIGNAL( repaintViews() ), this, SLOT( slotRepaint() ) );
    //iobjects->setIcon( QIcon::fromTheme( "pmconfigureobjects" ) );
    //iobjects->setHeader( "Display Settings for Objects" );
    m_pages.append( PMRegisteredSettingsPage( iobjects, page ) );

    page = new PMPreviewSettings();
    int ipreview = tbw->addTab( page, "Texture Preview" );
    //ipreview->setIcon( QIcon::fromTheme( "pmconfiguretexturepreview" ) );
    //ipreview->setHeader( tr( "Display Settings for Texture Previews" ) );
    m_pages.append( PMRegisteredSettingsPage( ipreview, page ) );

    page = new PMLayoutSettings();
    int ilayout = tbw->addTab( page, "View Layout" );
    //ilayout->setIcon( QIcon::fromTheme( "pmconfigureviewlayout" ) );
    //ilayout->setHeader( "Display Settings for View layouts" );
    m_pages.append( PMRegisteredSettingsPage( ilayout, page ) );

#ifdef KPM_WITH_OBJECT_LIBRARY_todo
/*	page = new PMObjectLibrarySettings();
    int ilibrary = tbw->addPage( page, "Object Library" );
    //ilibrary->setIcon( QIcon::fromTheme( "pmconfigureobjectlibrary" ) );
    //ilibrary->setHeader( "Display Settings for Texture Previews" );
    m_pages.push_back( PMRegisteredSettingsPage( ilibrary, page ) );*/
#endif

#ifdef KPM_WITH_PLUGINS
    page = new PMPluginSettings();
    QWidget* iplugins = addPage( page, "Plugins" );
    iplugins->setIcon( QIcon::fromTheme( "pmconfigureplugins" ) );
    iplugins->setHeader( "Plugin Settings" );
    m_pages.push_back( PMRegisteredSettingsPage( iplugins, page ) );
#endif

   displaySettings();
   resize( s_size );
   connect( buttonBox->button( QDialogButtonBox::Ok ), SIGNAL ( clicked() ), this, SLOT( slotOk() ) );
   connect( buttonBox->button( QDialogButtonBox::Cancel ), SIGNAL ( clicked() ), this, SLOT( slotCancel() ) );
   connect( buttonBox->button( QDialogButtonBox::Reset ), SIGNAL ( clicked() ), this, SLOT( slotDefault() ) );
   connect( buttonBox->button( QDialogButtonBox::Apply ), SIGNAL ( clicked() ), this, SLOT( slotApply() ) );

   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addWidget( tbw );
   mainLayout->addWidget( buttonBox );
   setLayout( mainLayout );
}

void PMSettingsDialog::displaySettings()
{
   QList<PMRegisteredSettingsPage>::const_iterator it;
   for( it = m_pages.constBegin(); it != m_pages.constEnd(); ++it )
      ( *it ).page->displaySettings();
}

void PMSettingsDialog::slotCancel()
{
   QDialog::reject();
}

void PMSettingsDialog::slotApply()
{
   if( validateData() )
      saveSettings();
}

void PMSettingsDialog::slotOk()
{
   if( validateData() )
   {
      saveSettings();
      QDialog::accept();
   }
}

void PMSettingsDialog::slotDefault()
{
   int current = tbw->currentIndex();
   PMSettingsDialogPage* page = nullptr;
   QList<PMRegisteredSettingsPage>::const_iterator it;
   for( it = m_pages.constBegin(); it != m_pages.constEnd() && !page; ++it )
      if( ( *it ).topPage == current )
         page = ( *it ).page;
   if( page )
      page->displayDefaults();
}

bool PMSettingsDialog::validateData()
{
   bool valid = true;
   QList<PMRegisteredSettingsPage>::const_iterator it;
   for( it = m_pages.constBegin(); it != m_pages.constEnd() && valid; ++it )
      valid = ( *it ).page->validateData();
   return valid;
}

void PMSettingsDialog::saveSettings()
{
   m_repaint = false;

   QList<PMRegisteredSettingsPage>::const_iterator it;
   for( it = m_pages.constBegin(); it != m_pages.constEnd(); ++it )
      ( *it ).page->applySettings();

   if( m_repaint )
   {
      PMRenderManager* rm = PMRenderManager::theManager();
      rm->slotRenderingSettingsChanged();
   }
}

void PMSettingsDialog::saveConfig()
{
   QSettings qset;
   qset.setValue( "Appearance/SettingsDialogSize", s_size );
}

void PMSettingsDialog::restoreConfig()
{
   QSettings qset;
   QSize defaultSize( 640, 400 );
   s_size = qset.value( "Appearance/SettingsDialogSize", defaultSize ).value<QSize>();
}

void PMSettingsDialog::resizeEvent( QResizeEvent* ev )
{
   s_size = ev->size();
}

void PMSettingsDialog::slotRepaint()
{
   m_repaint = true;
}

#include "pmsettingsdialog.moc"
