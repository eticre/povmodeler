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


#include "pmcomment.h"
#include "pmxmlhelper.h"

#include "pmcommentedit.h"
#include "pmmemento.h"

#include <QTextStream>


PMDefinePropertyClass( PMComment, PMCommentProperty );

PMMetaObject* PMComment::s_pMetaObject = 0;
PMObject* createNewComment( PMPart* part )
{
   return new PMComment( part );
}

const int c_maxDescriptionLength = 25;

PMComment::PMComment( PMPart* part )
      : Base( part )
{
}

PMComment::PMComment( PMPart* part, const QString& t )
   : Base( part )
{
   m_text = t;
}

PMComment::PMComment( const PMComment& c )
      : Base( c )
{
   m_text = c.m_text;
}

PMComment::~PMComment()
{
}

QString PMComment::description() const
{
   if( !m_text.isEmpty() )
   {
      QString copy = m_text;
      QTextStream str( &copy, QIODevice::ReadOnly );
      QString tmp, desc;
      bool stop = false;
      bool truncated = false;

      while( !str.atEnd() && !stop )
      {
         str >> tmp;
         if( desc.length() + tmp.length() + 1 <= c_maxDescriptionLength )
         {
            if( !desc.isEmpty() )
               desc += ' ';
            desc += tmp;
         }
         else
         {
            if( desc.isEmpty() )
            {
               desc = tmp.left( c_maxDescriptionLength );
               if( tmp.length() > c_maxDescriptionLength )
                  truncated = true;
            }
            else
               truncated = true;
            stop = true;
         }
      }
      if( truncated )
         desc += "...";
      return desc;
   }
   return ( "comment" );
}

void PMComment::setText( const QString& text )
{
   if( text != m_text )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMTextID, m_text );
         m_pMemento->setDescriptionChanged();
      }
      m_text = text;
   }
}

PMMetaObject* PMComment::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Comment", Base::metaObject(),
                                        createNewComment );
      s_pMetaObject->addProperty(
         new PMCommentProperty( "text", &PMComment::setText, &PMComment::text ) );
   }
   return s_pMetaObject;
}

void PMComment::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMComment::serialize( QDomElement& e, QDomDocument& doc ) const
{
   QDomText t = doc.createTextNode( m_text );
   e.appendChild( t );
}

void PMComment::readAttributes( const PMXMLHelper& h )
{
   QDomNode e = h.element().firstChild();
   if( e.isText() )
      m_text = e.toText().data();
}

PMDialogEditBase* PMComment::editWidget( QWidget* parent ) const
{
   return new PMCommentEdit( parent );
}

void PMComment::restoreMemento( PMMemento* s )
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
            case PMTextID:
               setText( data->stringData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMComment::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

