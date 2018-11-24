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


#include "pmoutputdevice.h"
#include "pmpovrayformat.h"
#include <qtextstream.h>


unsigned int PMOutputDevice::s_indentOffset = 3;
bool PMOutputDevice::s_bracketBehindType = true;

PMOutputDevice::PMOutputDevice( QIODevice* dev, PMPovrayFormat* format )
      : PMSerializer( dev ), m_stream( dev )
{
   m_pFormat = format;
   m_indentation = 0;
   m_lastWasComment = false;
   m_pendingNewLine = false;
   m_objectSeparation = false;
}

PMOutputDevice::~PMOutputDevice()
{
}

QString PMOutputDevice::description() const
{
   return QString( "POV-Ray" );
}

void PMOutputDevice::callSerialization( const PMObject* o, const PMMetaObject* mo )
{
   if( !mo )
      return;

   const PMPovraySerializeMethodInfo* info =
      m_pFormat->serializationMethod( mo->className() );

   if( info )
      info->call( o, mo, this );
   else
   {
      if( mo == o->metaObject() )
      {
         printError( ( "The object " + o->description() + " does not support " + description() ) );
      }
      else
      {
         printError( ( "The class " + o->description() + " does not support " + mo->className() ) );
      }
   }
}

void PMOutputDevice::serialize( PMObject* o )
{
   callSerialization( o, o->metaObject() );
	m_stream.flush();
}

void PMOutputDevice::close()
{
//   m_stream << ( char ) 0;
}

void PMOutputDevice::objectBegin( const QString& type )
{
   if( m_pendingNewLine )
      newLine();
   if( m_objectSeparation )
      newLine();
   m_stream << type;
   if( s_bracketBehindType )
      m_stream << " ";
   else
      newLine();

   m_stream << "{";
   m_indentation++;
   m_indentString.fill( ' ', s_indentOffset * m_indentation );
   m_pendingNewLine = true;
   m_objectSeparation = false;
}

void PMOutputDevice::declareBegin( const QString& id )
{
   if( m_pendingNewLine )
      newLine();
   if( m_objectSeparation )
      newLine();
   m_stream << "#declare " << id;

   m_objectSeparation = false;
}


void PMOutputDevice::objectEnd()
{
   m_indentation--;
   m_indentString.fill( ' ', s_indentOffset * m_indentation );
   newLine();
   m_stream << "}";
   m_pendingNewLine = true;
   m_objectSeparation = true;
}

void PMOutputDevice::writeLine( const QString& str )
{
   if( m_pendingNewLine )
      newLine();
   m_stream << str;
   m_pendingNewLine = true;
   m_objectSeparation = true;
}

void PMOutputDevice::write( const QString& str )
{
   if( m_pendingNewLine )
      newLine();
   m_stream << str;
   m_objectSeparation = false;
}

void PMOutputDevice::newLine()
{
   m_lastWasComment = false;
   m_pendingNewLine = false;
   m_stream << '\n' << m_indentString;
}

void PMOutputDevice::writeComment( const QString& text )
{
   QString s( text );
   QTextStream str( &s, QIODevice::ReadOnly );

   bool lwc = m_lastWasComment;
   if( m_pendingNewLine )
      newLine();
   if( lwc )
      newLine();
   if( m_objectSeparation )
      newLine();
   if( str.atEnd() )
      writeLine( "//" );
   else
      while( !str.atEnd() )
         writeLine( QString( "// " ) + str.readLine() );
   m_lastWasComment = true;
   m_objectSeparation = false;
}

void PMOutputDevice::writeSemicolon()
{
   // does not change m_pendingNewLine!
   m_stream << ';';
}

void PMOutputDevice::writeName( const QString& name )
{
   if( !name.isEmpty() )
      writeLine( QString( "//*PMName " ) + name );
}

QString PMOutputDevice::escapeAndQuoteString( const QString& s )
{
   QString result = "\"";
   QString tmp = s;
   QTextStream stream( &tmp, QIODevice::ReadOnly );
   QChar current, last;

   while( !stream.atEnd() )
   {
      stream >> current ;
      // not escaped quotation mark
      if( ( current == '"' ) && ( last != '\\' ) )
         result += '\\';
      result += current;
      // correctly quoted backslash
      if( ( last == '\\' ) && ( current == '\\' ) )
         current = QChar( 0 ); // clear the last char
      last = current;
   }
   // backslash at the end
   if( last == '\\' )
      result += '\\';
   result += '"';

   return result;
}
