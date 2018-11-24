//-*-C++-*-
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

#ifndef PMXMLPARSER_H
#define PMXMLPARSER_H



#include "pmparser.h"

/**
 * Parser that parses kpovmodeler xml code
 */
class PMXMLParser : public PMParser
{
public:
   /**
    * Parser that parses the device
    */
   PMXMLParser( PMPart* part, QIODevice* device );
   /**
    * Parser that parses the byte array
    */
   PMXMLParser( PMPart* part, const QByteArray& array );
   /**
    * Deletes the parser
    */
   virtual ~PMXMLParser();
   
   /**
    * Quickly scans the top level objects. Appends all top level object
    * types to the list.
    */
   virtual void quickParse( QStringList& list );
   /**
    * Returns true, if the parser can quickly scan the top level objects.
    */
   virtual bool canQuickParse() const { return true; }
protected:
   /**
    * Top level parse function
    */
   virtual void topParse();
   
private:
   /**
    * Inializes the parser
    */
   void init();
   /**
    * Initializes the QDomDocument. Returns true if successful.
    */
   bool initDocument();
   
   /**
    * Looks for child objects, parses them and appends them to the parent
    * object. If parent is 0, all objects are appended to the result list.
    */
   void parseChildObjects( QDomElement& e, PMObject* parent );

   QDomDocument* m_pDoc;
   int m_majorDocumentFormat;
   int m_minorDocumentFormat;
};


#endif
