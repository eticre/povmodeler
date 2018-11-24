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

#include "pmpluginmanager.h"
#include "pmpart.h"
#include "pmshell.h"
#include "pmfactory.h"
#include "pmdebug.h"

#include <qmap.h>
#include <qapplication.h>
#include <QList>


PMPluginManager* PMPluginManager::s_pInstance = nullptr;
//K3StaticDeleter<PMPluginManager> PMPluginManager::s_staticDeleter;

using namespace KParts;

// workaround for protected Plugin::pluginInfos
class PMPluginWorkaround : public Plugin
{
public:
   PMPluginWorkaround() : Plugin( nullptr ) { }
   /*static QList<Plugin::PluginInfo> installedPlugins( const KComponentData &instance )
   {
      return pluginInfos( instance );
   }*/
};

PMPluginManager::PMPluginManager()
{
   // find installed plugins
   // eticre no factory KConfigGroup cfgGroup( PMFactory::componentData().config(), "KParts Plugins" );
   /*
    QList<Plugin::PluginInfo> plugins
      = PMPluginWorkaround::installedPlugins( PMFactory::componentData() );
   QList<Plugin::PluginInfo>::ConstIterator pIt = plugins.constBegin();
   QList<Plugin::PluginInfo>::ConstIterator pEnd = plugins.constEnd();
   
   for( ; pIt != pEnd; ++pIt )
   {
      QDomElement docElem = ( *pIt ).m_document.documentElement();
      QString name = docElem.attribute( "name" );
      QString description = docElem.attribute( "description" );
      if( !description.isEmpty() )
         description = tr( description.toLatin1() );
      bool enabled = cfgGroup.readEntry<bool>( name + "Enabled", false );
      m_plugins.append( new PMPluginInfo( name, description, enabled ) );
   }*/
}

PMPluginManager::~PMPluginManager()
{
	foreach(PMPluginInfo* info, m_plugins) delete info;
   m_plugins.clear();
}

PMPluginManager* PMPluginManager::theManager()
{
   if( !s_pInstance )
        static_cast<void>(s_pInstance), new PMPluginManager();
      //s_staticDeleter.setObject( s_pInstance, new PMPluginManager() );
   return s_pInstance;
}

void PMPluginManager::registerPart( PMPart* p )
{
   if( !m_parts.contains( p ) )
   {
      m_parts.append( p );
      //Plugin::loadPlugins( p, p, PMFactory::componentData(), false );
   }
}

void PMPluginManager::removePart( PMPart* p )
{
   m_parts.removeAll( p );
}

void PMPluginManager::updatePlugins()
{  /*
   KConfigGroup cfgGroup( PMFactory::componentData().config(),
                          "KParts Plugins" );
   QList<PMPluginInfo*>::iterator pit;
   for( pit = m_plugins.begin(); pit != m_plugins.end(); ++pit )
      cfgGroup.writeEntry( ( *pit )->name() + "Enabled",
                           ( *pit )->enabled() );
   cfgGroup.sync();

   QList<PMPart*>::iterator it;
   for( it = m_parts.begin(); it != m_parts.end(); ++it )
   {
      Plugin::loadPlugins( *it, *it, PMFactory::componentData(), false );
      PMShell* shell = ( *it )->shell();
      if( shell )
         shell->updateGUI();
      // TODO find a solution to update the gui without using the shell
   }*/
}
