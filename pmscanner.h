//-*-C++-*-
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


#ifndef PMSCANNER_H
#define PMSCANNER_H

#include <qiodevice.h>
#include <qstring.h>
#include <qmap.h>

/**
 * Dictionary of reserved words for fast lookup
 *
 * The class @ref PMScanner has two static dictionaries: one for reserved
 * words and one for directives. The constructor will insert the items.
 */
class PMReservedWordDict : protected QMap< QString, int >
{
public:
   /**
    * Mode for constructor.
    */
   enum PMDictMode { PMDReservedWords, PMDDirectives };
   /**
    * Creates a dictionary for povray reserved words or directives.
    */
   PMReservedWordDict( PMDictMode mode );
   /**
    * Deletes the dictionary
    */
   ~PMReservedWordDict();

   /**
    * Returns the token constant for the key if found, otherwise -1
    */
   int operator[] ( QString key ) const { return find( key ); }
   /**
    * Returns the token constant for the key if found, otherwise -1
    */
   int find( QString key ) const { return value( key , -1 ); }
};


/**
 * Scanner that scans povray tokens out of a QIODevice
 */
class PMScanner
{
public:
   /**
    * Creates a scanner that scans the QIODevice device
    */
   PMScanner( QIODevice* device );
   /**
    * Deletes the scanner
    */
   ~PMScanner();

   /**
    * Scans the device for the next token. Returns a value of @ref PMToken
    * ( > 0xFF ) or a single character
    */
   int nextToken();
   /**
    * Returns the current token
    */
   int currentToken() const { return m_token; }

   /**
    * Returns the integer value of the current token if currentToken
    * is INTEGER_TOK
    */
   int iValue() const { return m_ivalue; }
   /**
    * Returns the double value of the current token if currentToken
    * is FLOAT_TOK
    */
   double fValue() const { return m_fvalue; }
   /**
    * Returns the string value of the current token if currentToken
    * is ID_TOK, COMMENT_TOK, STRING_TOK
    */
   const char* sValue() const { return m_svalue; }
   /**
    * Returns the current line number
    */
   int currentLine() const { return m_line; }
   /**
    * Returns the error string if current token is SCANNER_ERROR_TOK
    */
   QString error() const { return m_error; }
   /**
    * Special parse method for a function statement
    */
   void scanFunction();
   /**
    * Returns a pointer to a dictionary with reserved words
    */
   static PMReservedWordDict* reservedWords() { return &m_reservedWords; }
   /**
    * Returns a pointer to a dictionary with directives
    */
   static PMReservedWordDict* directives() { return &m_directives; }
private:
   /**
    * returns true if c is one of the following characters:
    * space, tab, newline, '{', '}', '<', '>', '+', '-', '*', '/', ',',
    * '(', ')', '=', '[', ']', ';'
    */
   inline bool isseparation( int c );
   /**
    * Called on unexpected character
    */
   void scanError( int c );
   /**
    * Reads the next character out of the device
    */
   inline void nextChar();
   /**
    * Adds the char to m_svalue
    */
   inline void addChar( char c );
   /**
    * Clears m_svalue
    */
   inline void clearSValue();

   static PMReservedWordDict m_reservedWords;
   static PMReservedWordDict m_directives;

   /**
    * States for the state machine of the scanner
    */
   enum PMScanStates
   {
      START_ST = 0, ID_ENDST, INTEGER_ENDST,
      FLOAT1_ST, FLOAT_ENDST, FLOAT_EXP1_ST, FLOAT_EXP2_ST, FLOAT_EXP_ENDST,
      POINT_ST,
      DIRECTIVE1_ST, DIRECTIVE_ENDST,
      STRING1_ST, STRINGBS_ST,
      SLASH_ST, LINE_COMMENT_FIRST_ST, LINE_COMMENT_ST,
      C_COMMENT_ST, COMMENT_ST_ST, COMMENT_SL_ST, COMMENT_NEW_LINE_ST,
      PMNAME_ST,
      RAW_POVRAY_FIRST_ST, RAW_POVRAY_LB_ST, RAW_POVRAY_ST, RAW_POVRAY_END_ST,
      RAW_POVRAY_END_END_ST,
      TOKEN_END_ST
   };

   QIODevice* m_pDevice;

   int m_char;
   int m_token;
   int m_ivalue;
   double m_fvalue;
   char* m_svalue;
   char* m_lastChar;
   char* m_lastAlloc;
   unsigned int m_svalueAlloc;
   int m_indentation;
   int m_rawIndentation;
   int m_rawPovrayEnd;
   bool m_bFunctionMode;

   int m_line;
   QString m_error;
};


#endif
