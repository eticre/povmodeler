//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003-2006 by Andreas Zehender
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

#ifndef PMSERIALIZER_H
#define PMSERIALIZER_H



class QIODevice;

#include "pmobject.h"
#include "pmerrordialog.h"

/**
 * Class to serialize an object or a list of objects.
 *
 * Normally you don't have to create instances of this type or subclasses,
 * the class @ref PMIOFormat has factory methods to create them.
 *
 * There is one sub class for each format.
 *
 * During serialization, errors can occur. These are returned
 * by the method @ref errors.
 */
class PMSerializer
{
public:
   /**
    * Default constructor
    *
    * The serialized data will be written to the io device
    */
   PMSerializer( QIODevice* dev );
   /**
    * Destructor
    */
   virtual ~PMSerializer();
   /**
    * Returns the translated description of the format. Should return
    * the same string as description() of the corresponding
    * IO format.
    */
   virtual QString description() const = 0;

   /**
    * Serializes one object to the device
    */
   virtual void serialize( PMObject* o ) = 0;
   /**
    * Serializes a list of objects. The default
    * implementation will call serialize( PMObject* ) for each object.
    */
   virtual void serializeList( const PMObjectList& objects );
   /**
    * Closes the serializer
    */
   virtual void close() = 0;

   /**
    * Returns the messages of the serializer
    */
   PMMessageList messages() const { return m_messages; }
   /**
    * Returns true if there were errors during serializing
    */
   bool errors() const { return m_errors > 0; }
   /**
    * Returns true if there were warnings during serializing
    */
   bool warnings() const { return m_warnings > 0; }
   /**
    * Returns true, if a fatal error occurred
    * and it doesn't make sense to continue
    */
   bool fatal() const { return m_bFatalError; }
   /**
    * Returns a bitwise combination of @ref PMErrorFlags constants
    */
   int errorFlags() const;

   /**
    * Adds an error to the message string
    */
   void printError( const QString& msg );
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
    * "info"
    */
   void printMessage( const QString& type, const QString& msg );

   /**
    * Sets the fatal error flag
    */
   void setFatalError() { m_bFatalError = true; }

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

protected:
   /**
    * The assigned IO device for serialization
    */
   QIODevice* m_pDev;

private:
   /**
    * The serializer output (errors, warnings...)
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
};

#endif
