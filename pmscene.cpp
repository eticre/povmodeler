/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#include "pmscene.h"

#include <QtXml/qdom.h>
#include "pmxmlhelper.h"
#include "pmdocumentformat.h"

const int c_defaultVisibilityLevel = 10;
PMMetaObject* PMScene::s_pMetaObject = nullptr;
PMObject* createNewScene( PMPart* part )
{
   return new PMScene( part );
}

PMScene::PMScene( PMPart* part )
      : Base( part )
{
   m_visibilityLevel = c_defaultVisibilityLevel;
	m_activeMode = -1;
}

PMScene::PMScene( const PMScene& s )
      : Base( s )
{
   m_visibilityLevel = s.m_visibilityLevel;
}

PMScene::~PMScene()
{
}

QString PMScene::description() const
{
   return ( "scene" );
}

PMMetaObject* PMScene::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Scene", Base::metaObject(),
                                        createNewScene );
   }
   return s_pMetaObject;
}

void PMScene::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMScene::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "majorFormat", c_majorDocumentFormat );
   e.setAttribute( "minorFormat", c_minorDocumentFormat );
   e.setAttribute( "visibility_level", m_visibilityLevel );
   QDomElement data = doc.createElement( "extra_data" );
   QDomElement rm;

	PMRenderModeList::const_iterator it;
   for(it = m_renderModes.begin(); it != m_renderModes.end(); ++it )
   {
      rm = doc.createElement( "rendermode" );
      (*it).serialize( rm );
      data.appendChild( rm );
   }

   e.appendChild( data );
   Base::serialize( e, doc );
}

void PMScene::readAttributes( const PMXMLHelper& h )
{
   m_visibilityLevel = h.intAttribute( "visibility_level", c_defaultVisibilityLevel );
   QDomElement e = h.extraData();
   if( !e.isNull() )
   {
      QDomNode c = e.firstChild();
      while( !c.isNull() )
      {
         if( c.isElement() )
         {
            QDomElement ce = c.toElement();
            if( ce.tagName() == "rendermode" )
               m_renderModes.append( PMRenderMode( ce ) );
         }
         c = c.nextSibling();
      }
   }

   Base::readAttributes( h );
   setRenderModes( m_renderModes );
}

void PMScene::setRenderModes( const PMRenderModeList& list )
{
	m_renderModes = list;
	if( list.count() > 0 )
		m_activeMode = 0;
	else
		m_activeMode = -1;
}

void PMScene::setActiveRenderMode( int i )
{
	if( i >= 0 && i < m_renderModes.count() ) m_activeMode = i;
}

const PMRenderMode* PMScene::activeRenderMode() const
{
	if( m_activeMode >= 0 && m_activeMode < m_renderModes.count() )
		return &m_renderModes[m_activeMode];
	return 0;
}
