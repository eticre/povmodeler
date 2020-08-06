/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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


#include "pmobjectdrag.h"
#include "pmobject.h"
#include "pmpart.h"
#include "pmpovrayparser.h"
#include "pmxmlparser.h"
#include "pmxmlhelper.h"
#include "pmdocumentformat.h"
#include "pmiomanager.h"
#include "pmserializer.h"

#include <QBuffer>
#include <QList>
#include <QTextStream>

const char* const c_kpmDocumentMimeFormat = "application/x-povmodeler";

PMObjectDrag::PMObjectDrag( PMPart* part, PMObject* object )
      : QMimeData()
{
   QByteArray modelerData;

   QTextStream s2( &modelerData, QIODevice::WriteOnly );
   QDomDocument doc( "POVMODELER" );
   QDomElement top = doc.createElement( "objects" );
   doc.appendChild( top );
   top.setAttribute( "majorFormat", c_majorDocumentFormat );
   top.setAttribute( "minorFormat", c_minorDocumentFormat );

   if( object->type() == "Scene" )
   {
      PMObject* o = object->firstChild();
      for( ; o; o = o->nextSibling() )
      {
         QDomElement e = o->serialize( doc );
         top.appendChild( e );
      }
   }
   else
   {
      QDomElement e = object->serialize( doc );
      top.appendChild( e );
   }
   s2 << doc;
    s2.flush();

	setData( c_kpmDocumentMimeFormat, modelerData );
    qDebug() << "Added mime type " << c_kpmDocumentMimeFormat
                          << " " << modelerData.size() << " bytes" << Qt::endl;

   const QList< PMIOFormat* >& formats = part->ioManager()->formats();
   foreach( PMIOFormat* it, formats )
   {
      if( it->services() & PMIOFormat::Export )
      {
         QByteArray data;
         QBuffer buffer( &data );
         buffer.open( QIODevice::WriteOnly );

         PMSerializer* ser = it->newSerializer( &buffer );

         if( ser )
         {
            ser->serialize( object );
            ser->close();
            delete ser;
            buffer.close();

                setData( it->mimeType(), data );

            qDebug() << "Added mime type " << it->mimeType()
                              << " " << data.size() << " bytes" << Qt::endl;
         }
         else
            qCritical() << "Format claims to support exporting, but doesn't provide a serializer" << Qt::endl;

         buffer.close();
      }
   }
}

PMObjectDrag::PMObjectDrag( PMPart* part, const PMObjectList& objList )
      : QMimeData()
{
   QByteArray modelerData;

   QTextStream s2( &modelerData, QIODevice::WriteOnly );
   QDomDocument doc( "POVMODELER" );
   QDomElement top = doc.createElement( "objects" );
   doc.appendChild( top );
   top.setAttribute( "majorFormat", c_majorDocumentFormat );
   top.setAttribute( "minorFormat", c_minorDocumentFormat );

	PMObjectList::const_iterator it;
   for( it = objList.begin(); it != objList.end(); ++it )
   {
      if( ( *it )->type() == "Scene" )
      {
         PMObject* o = ( *it )->firstChild();
         for( ; o; o = o->nextSibling() )
         {
            QDomElement e = o->serialize( doc );
            top.appendChild( e );
         }
      }
      else
      {
         QDomElement e = ( *it )->serialize( doc );
         top.appendChild( e );
      }
   }
   s2 << doc;
	s2.flush();

	setData( c_kpmDocumentMimeFormat, modelerData );
    qDebug() << "Added mime type " << c_kpmDocumentMimeFormat
                          << " " << modelerData.size() << " bytes" << Qt::endl;

   const QList< PMIOFormat* >& formats = part->ioManager()->formats();
   foreach( PMIOFormat* fit, formats )
   {
      if( fit->services() & PMIOFormat::Export )
      {
         QByteArray data;
         QBuffer buffer( &data );
         buffer.open( QIODevice::WriteOnly );

         PMSerializer* ser = fit->newSerializer( &buffer );

         if( ser )
         {
            ser->serializeList( objList );
            ser->close();
            delete ser;
            buffer.close();

                setData( fit->mimeType(), data );

            qDebug() << "Added mime type " << fit->mimeType()
                              << " " << data.size() << " bytes" << Qt::endl;
         }
         else
            qCritical() << "Format claims to support exporting, but doesn't provide a serializer" << Qt::endl;

         buffer.close();
      }
   }
}

PMObjectDrag::~PMObjectDrag()
{
}

bool PMObjectDrag::canDecode( const QMimeData* e, PMPart* part ) 
{ 
   if( e->hasFormat( c_kpmDocumentMimeFormat ) ) 
      return true; 
 
    const QList< PMIOFormat* >& formats = part->ioManager()->formats();
    foreach( PMIOFormat* fit, formats )
      if( fit->services() & PMIOFormat::Import && e->hasFormat( fit->mimeType() ) )
         return true; 
 
   return false; 
} 

PMParser* PMObjectDrag::newParser( const QMimeData* e, PMPart* part )
{
   if( e->hasFormat( c_kpmDocumentMimeFormat ) )
      return new PMXMLParser( part, e->data( c_kpmDocumentMimeFormat ) );

   const QList< PMIOFormat* >& formats = part->ioManager()->formats();
   foreach( PMIOFormat* fit, formats )
   {
      QString str = fit->mimeType();
      if( fit->services() & PMIOFormat::Import && e->hasFormat( str ) )
         return fit->newParser( part, e->data( str ) );
   }

   return 0;
}


