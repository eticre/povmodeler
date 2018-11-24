//-*-C++-*-
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


#ifndef PMPARSER_H
#define PMPARSER_H



#include <QUrl>
#include <qstring.h>
#include <qstringlist.h>
#include <QList>
#include <QHash>
#include <QTextStream>

#include "pmobject.h"
#include "pmsymboltable.h"
#include "pmvalue.h"
#include "pmerrordialog.h"

class QTextStream;
class QIODevice;

class PMPart;
class PMSymbolTable;

enum PMPMessage { PMMClockDefault = 1, PMMClockDeltaDefault = 2,
                  PMMSpecialRawComment = 4 };

/**
 * Base class for kpovmodeler parsers.
 */
class PMParser
{
public:
   /**
    * Parser that parses the device
    */
   PMParser( PMPart* part, QIODevice* device );
   /**
    * Parser that parses the byte array
    */
   PMParser( PMPart* part, const QByteArray& array );
   /**
    * Deletes the parser
    */
   virtual ~PMParser();

   /**
    * Quickly scans the top level objects. Appends all top level object
    * types to the list.
    */
   virtual void quickParse( QStringList& /*list*/ ) { }
   /**
    * Returns true, if the parser can quickly scan the top level objects.
    */
   virtual bool canQuickParse() const { return false; }

   /**
    * Parses the device.
    *
    * Appends all parsed objects to the list.
    *
    * parent is the object where the parsed objects will be inserted as
    * children behind the object after. These parameters are used to check
    * the consistency of declares and links.
    *
    * Set parent and after to 0 if and only if a document is parsed/opened.
    */
   void parse( PMObjectList* list, PMObject* parent,
               PMObject* after );
   
   /**
    * Returns the messages of the parser
    */
   PMMessageList messages() const { return m_messages; }
   /**
    * Returns true if there were errors during parsing
    */
   bool errors() const { return m_errors > 0; }
   /**
    * Returns true if there were warnings during parsing
    */
   bool warnings() const { return m_warnings > 0; }
   /**
    * Returns true, if a fatal error occurred or no object could be
    * successfully parsed and it doesn't make sense to continue
    */
   bool fatal() const { return m_bFatalError; }
   /**
    * Returns a bitwise combination of @ref PMErrorFlags constants
    */
   int errorFlags() const;

   /**
    * returns the maximum number of errors
    */
   static unsigned maxErrors() { return s_maxErrors; }
   /**
    * sets the maximum number of errors to m
    */
   static void setMaxErrors( unsigned m ) { s_maxErrors = m; }
   /**
    * returns the maximum number of warnings
    */
   static unsigned maxWarnings() { return s_maxWarnings; }
   /**
    * sets the maximum number of warnings to m
    */
   static void setMaxWarnings( unsigned m ) { s_maxWarnings = m; }

   /**
    * Adds an error to the message string
    */
   void printError( const QString& msg );
   /**
    * Adds the error "'<str>' expected, found <token> instead" to the message string
    */
   void printExpected( const QString& str, const char* token );
   /**
    * Adds the error "'<c>' expected" to the message string
    */
   void printExpected( const char c, const char* token );
   /**
    * Adds the error "Unexpected token '<str>', found <token> instead" to the message string
    */
   void printUnexpected( const QString& str );
   /**
    * Adds a warning to the message string
    */
   void printWarning( const QString& msg );
   /**
    * Adds an info to the message string
    */
   void printInfo( const QString& msg );
   /**
    * Adds the message to the message string. Type is "error", "warning",
    * "info" or "scanner error"
    */
   void printMessage( const QString& type, const QString& msg );
   /**
    * Prints a messages that should only reported once
    */
   void printMessage( const PMPMessage messageNum );

   /**
    * Sets the fatal error flag
    */
   void setFatalError() { m_bFatalError = true; }
   
   /**
    * Sets the current line number
    */
   void setCurrentLine( int lineNumber ) { m_lineNum = lineNumber; }

   /**
    * Returns the declare object with id, if it exists and is declared at
    * the current parse position, otherwise 0
    */
   PMDeclare* checkLink( const QString& id );
   /**
    * Checks the id of the declare. If there is already a declare with
    * the same id, the id of the object is renamed.
    *
    * Inserts the object into the symbol table.*/
   void checkID( PMDeclare* obj );
   /**
     eticre "declare" not in tree
   **/
   bool checkID_lib( PMDeclare* obj );
   /**
    * Checks the id of the value declare (constant, vector or color).
    * If there is already a declare with
    * the same id, the id of the value is renamed.
    *
    * Inserts the object into the symbol table.*/
   void checkID( const QString& id, const PMValue& v );
   /**
    * Returns the symbol with id id or 0 if the id is undeclared
    */
   PMSymbol* getSymbol( const QString& id ) const;
   /**
    * Tries to insert obj as child of parent. If parent is 0, the object
    * will be inserted in the list of top level objects.
    *
    * Returns true if the object could be inserted.
    */
   bool insertChild( PMObject* obj, PMObject* parent );

protected:
   /**
    * Top level parse function.
    *
    * Overwrite this function in a specific parser.
    */
   virtual void topParse() = 0;
   
protected:
   /**
    * Pointer to the part
    */
   PMPart* m_pPart;
   /**
    * parent object where the parsed top level objects
    * will be inserted _later_ (not from the parser) as children
    */
   const PMObject* m_pTopParent;
   const PMObject* m_pAfter;
   /**
    * The list where the parsed objects are stored
    */
   PMObjectList* m_pResultList;
   /**
    * The QIODevice
    */
   QIODevice* m_pDevice;
   /**
    * True, if the device was created by the parser
    */
   bool m_bDeviceCreated;
   
private:
   /**
    * Initializes the parser
    */
   void init();
   
   /**
    * The parser output (errors, warnings...)
    */
   PMMessageList m_messages;
   /**
    * Number of warnings during parsing
    */
   unsigned int m_warnings;
   /**
    * Number of errors during parsing
    */
   unsigned int m_errors;
   /**
    * Flag for fatal errors
    */
   bool m_bFatalError;

   /**
    * maximum number of errors
    */
   static unsigned int s_maxErrors;
   /**
    * maximum number of warnings
    */
   static unsigned int s_maxWarnings;
   /**
    * Already shown warnings or errors, that are reported only once
    */
   int m_shownMessages;
   /**
    * The current line number
    */
   int m_lineNum;
   /**
    * List of renamed declares
    */
   QList<PMSymbol*> m_renamedObjectSymbols;
   /**
    * Dictionary of symbol names, that can be linked
    */
   QHash<QString, bool> m_okDeclares;
   PMObject* m_pNextCheckDeclare;
   /**
    * Symbol table used during parsing. The symbol table of the part
    * will not be changed.
    */
   PMSymbolTable m_pLocalST;
};



#endif
