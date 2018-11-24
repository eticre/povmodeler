//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2003 by Andreas Zehender
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


#ifndef PMOUTPUTDEVICE_H
#define PMOUTPUTDEVICE_H



#include "pmserializer.h"

#include <qstring.h>
//Added by qt3to4:
#include <QTextStream>

class QTextStream;
class PMPovrayFormat;

/**
 * Output class for povray code.
 *
 * The class @ref PMPovrayFormat or a base class has factory methods
 * to create an instance of this class. The output device uses the
 * registered serialization methods of this povray format to serialize
 * objects.
 *
 * This class handles the indentation and position of brackets
 */

class PMOutputDevice : public PMSerializer
{
public:
   /**
    * Creates an PMOutputDevice that serializes the povray code
    * to the device
    */
   PMOutputDevice( QIODevice* dev, PMPovrayFormat* format );
   /** */
   virtual ~PMOutputDevice();

   /** */
   virtual QString description() const;
   /** */
   virtual void serialize( PMObject* o );
   /** */
   virtual void close();

   /**
    * Writes the povray object type, an open bracket to the text stream
    * and indents the next lines
    */
   void objectBegin( const QString& type );
   /**
    * Begins a declare with the identifier id
    */
   void declareBegin( const QString& id );
   /**
    * Writes an closing bracket to the text stream
    * and decreases the indentation
    */
   void objectEnd();
   /**
    * Writes a single line to the text stream. Don't include
    * newlines in the string or indentation will not work properly.
    *
    * Adds a newline before the string.
    */
   void writeLine( const QString& str );
   /**
    * Writes the string to the text stream
    */
   void write( const QString& str );
   /**
    * Writes a new line to the text stream and indents the next line.
    */
   void newLine();
   /**
    * Writes a comment string to the text stream
    */
   void writeComment( const QString& text );
   /**
    * Writes a semicolon after a call to objectEnd()
    */
   void writeSemicolon();
   /**
    * Writes a special name comment to the text stream, if the
    * name is not empty
    */
   void writeName( const QString& name );

   /**
    * Returns the basic indentation offset
    */
   static unsigned int indentationOffset() { return s_indentOffset; }
   /**
    * Sets the basic indentation offset
    */
   static void setIndentationOffset( unsigned int offset )
   { s_indentOffset = offset; }

   /**
    * If set to true, the open bracket after an object begin will be
    * written behind the object type, otherwise in a new line.
    */
   static void setBracketBehindType( bool yes ) { s_bracketBehindType = yes; }
   /**
    * Returns true if the open bracket after an object begin will be
    * written behind the object type
    */
   static bool bracketBehindType() { return s_bracketBehindType; }

   /**
    * Correctly escapes the string and puts quotation marks at the begin
    * and end of the string.
    *
    * Escapes only not escaped characters. "\"" and "\\" in the string
    * are not escaped.
    */
   static QString escapeAndQuoteString( const QString& s );

   /**
    * Calls the serialization method for the object o and class
    * given by the meta object.
    *
    * If no method for this class was registered in the corresponding
    * @ref PMPovrayFormat, an error is added the message list.
    */
   void callSerialization( const PMObject* o, const PMMetaObject* mo );

private:
   static unsigned int s_indentOffset;
   static bool s_bracketBehindType;

   PMPovrayFormat* m_pFormat;
   unsigned int m_indentation;
   QString m_indentString;
   QTextStream m_stream;
   bool m_lastWasComment;
   bool m_pendingNewLine;
   bool m_objectSeparation;
};


#endif
