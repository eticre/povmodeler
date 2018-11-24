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

#ifndef PMIOMANAGER
#define PMIOMANAGER



#include <qstring.h>
#include <qstringlist.h>
#include <QHash>

class PMParser;
class PMSerializer;
class PMRenderer;
class PMPart;

class QIODevice;

/**
 * Description class for input and output formats.
 *
 * A format may provide the following services:
 *
 * Import: The class can provide a parser to load and import data
 *
 * Export: The class can provide a output device to export and save data
 *
 * Renderer: A renderer exists to render the exported data
 *
 * The class @ref PMIOManager stores a list of instances
 * of this class
 */
class PMIOFormat
{
public:
   /**
    * Format type enum
    */
   enum Services { Import = 1, Export = 2, Renderer = 4,
                   AllServices = Import | Export | Renderer };
   /**
    * Default constructor
    */
   PMIOFormat();
   /**
    * Destructor
    */
   virtual ~PMIOFormat();

   /**
    * Returns an unique name of this format.
    */
   virtual QString name() const = 0;
   /**
    * Returns a translated description of this format
    */
   virtual QString description() const = 0;
   /**
    * Returns the supported services
    * (a bitwise combination of the Services enum values)
    */
   virtual int services() const = 0;
   /**
    * Returns a parser to parse the io device.
    *
    * The caller is responsible to delete the returned parser.
    */
   virtual PMParser* newParser( PMPart*, QIODevice* ) const
   {
      return 0;
   }
   /**
    * Returns a parser to parse the byte array.
    *
    * The caller is responsible to delete the returned parser.
    */
   virtual PMParser* newParser( PMPart*, const QByteArray& ) const
   {
      return 0;
   }
   /**
    * Returns an output device to export objects or the complete
    * scene to the given io device.
    *
    * The caller is responsible to delete the returned device
    */
   virtual PMSerializer* newSerializer( QIODevice* )
   {
      return 0;
   }
   /**
    * Returns a new renderer
    */
   virtual PMRenderer* newRenderer( PMPart* ) const
   {
      return 0;
   }
   /**
    * Returns the mime type for this format
    */
   virtual QString mimeType() const
   {
      return QString();
   }
   /**
    * Returns a list of patterns for the import file dialog
    */
   virtual QStringList importPatterns() const
   {
      return QStringList();
   }
   /**
    * Returns a list of patterns for the export file dialog
    */
   virtual QStringList exportPatterns() const
   {
      return QStringList();
   }
};

/**
 * Manager class that handles all available input and output formats
 * as well as renderers for one part
 */
class PMIOManager
{
public:
   /**
    * Creates an io manager for the part
    */
   PMIOManager( PMPart* part );
   /**
    * Deletes the io manager
    */
   ~PMIOManager();

   /**
    * Adds a new format
    */
   void addFormat( PMIOFormat* format );
   /**
    * Removes a format by name
    */
   void removeFormat( const QString& name );

   /**
    * Returns the list of registered io formats
    */
   const QList< PMIOFormat* >& formats() const { return m_formats; }
   /**
    * Returns a view type by name
    */
   PMIOFormat* format( const QString& name ) const;
   /**
    * Returns the first io format that can handle the mime type
    * or 0 if there is none
    */
   PMIOFormat* formatForMimeType( const QString& mime ) const;

private:
   QList< PMIOFormat* > m_formats;
   QHash<QString, PMIOFormat*> m_dict;
   PMPart* m_pPart;
};

#endif
