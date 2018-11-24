/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#include "pmraw.h"
#include "pmxmlhelper.h"

#include "pmrawedit.h"
#include "pmmemento.h"


#include <qtextstream.h>

PMDefinePropertyClass( PMRaw, PMRawProperty );

PMMetaObject* PMRaw::s_pMetaObject = 0;
PMObject* createNewRaw( PMPart* part )
{
   return new PMRaw( part );
}

PMRaw::PMRaw( PMPart* part )
      : Base( part )
{
}

PMRaw::PMRaw( const PMRaw& r )
      : Base( r )
{
   m_code = r.m_code;
}

PMRaw::PMRaw( PMPart* part, const QString& t )
   : Base( part )
{
   m_code = t;
}

PMRaw::~PMRaw()
{
}

QString PMRaw::description() const
{
   return ( "raw povray" );
}

void PMRaw::setCode( const QString& code )
{
   if( code != m_code )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCodeID, m_code );
      m_code = code;
   }
}

PMMetaObject* PMRaw::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Raw", Base::metaObject(),
                                        createNewRaw );
      s_pMetaObject->addProperty(
         new PMRawProperty( "code", &PMRaw::setCode, &PMRaw::code ) );
   }
   return s_pMetaObject;
}

void PMRaw::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMRaw::serialize( QDomElement& e, QDomDocument& doc ) const
{
   QDomText t = doc.createTextNode( m_code );
   e.appendChild( t );
      Base::serialize( e, doc );
}

void PMRaw::readAttributes( const PMXMLHelper& h )
{
   QDomNode e = h.element().firstChild();
   if( e.isText() )
      m_code = e.toText().data();
      Base::readAttributes( h );
}

PMDialogEditBase* PMRaw::editWidget( QWidget* parent ) const
{
   return new PMRawEdit( parent );
}

void PMRaw::restoreMemento( PMMemento* s )
{
	const QList<PMMementoData*>& changes = s->changes();
	QList<PMMementoData*>::const_iterator it;
   PMMementoData* data;
	for( it = changes.begin(); it != changes.end(); ++it )
	{
		data = *it;
      if( data->objectType() == s_pMetaObject )
      {
         switch( data->valueID() )
         {
            case PMCodeID:
               setCode( data->stringData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMRaw::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

