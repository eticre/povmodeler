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

#define KPM_WITH_OBJECT_LIBRARY
#include "pmviewfactory.h"
#include "pmglview.h"
#include "pmtreeview.h"
#include "pmdialogview.h"
#include "pmdebug.h"
#include "pmviewbase.h"
#include "pmglview.h"
#include "pmdialogview.h"
#ifdef KPM_WITH_OBJECT_LIBRARY
#include "pmlibrarybrowser.h"
#endif


//Added by qt3to4:

PMViewFactory* PMViewFactory::s_pInstance = nullptr;
//K3StaticDeleter<PMViewFactory> PMViewFactory::s_staticDeleter;

PMViewFactory::PMViewFactory()
{
   addViewType( new PMTreeViewFactory() );
   addViewType( new PMDialogViewFactory() );
   addViewType( new PMGLViewFactory() );
#ifdef KPM_WITH_OBJECT_LIBRARY
   addViewType( new PMLibraryBrowserViewFactory() );
#endif
}

PMViewFactory::~PMViewFactory()
{
   while( !m_viewTypes.isEmpty() )
      delete m_viewTypes.takeFirst();
}

PMViewFactory* PMViewFactory::theFactory()
{
   if( !s_pInstance )
      //s_staticDeleter.setObject( s_pInstance, new PMViewFactory() );
      s_pInstance = new PMViewFactory();
   return s_pInstance;
}

void PMViewFactory::addViewType( PMViewTypeFactory* vt )
{
   if( vt )
   {
      m_viewTypes.append( vt );
      m_dict.insert( vt->viewType(), vt );
   }
}

PMViewBase* PMViewFactory::newViewInstance( const QString& viewType,
                                            QWidget* parent, PMPart* part ) const
{
   PMViewTypeFactory* f = m_dict.value( viewType );
   if( f )
      return f->newInstance( parent, part );

   qCritical() << "Unknown view type \"" << viewType << "\"" << endl;
   return nullptr;
}

PMViewOptions* PMViewFactory::newOptionsInstance( const QString& viewType ) const
{
   PMViewTypeFactory* f = m_dict.value( viewType );
   if( f )
      return f->newOptionsInstance();

   qCritical() << "Unknown view type \"" << viewType << "\"" << endl;
   return nullptr;
}

PMViewTypeFactory* PMViewFactory::viewFactory( const QString& viewType ) const
{
   return m_dict.value( viewType );
}

const QList< PMViewTypeFactory* >& PMViewFactory::viewTypes() const
{
   return m_viewTypes;
}

