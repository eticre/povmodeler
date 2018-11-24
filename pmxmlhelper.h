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

#ifndef PMXMLHELPER_H
#define PMXMLHELPER_H



#include <QtXml/QDomDocument>
#include <qstring.h>

#include "pmobject.h"
#include "pmcolor.h"
#include "pmvector.h"
#include "pmmatrix.h"

class PMPart;
class PMParser;

/**
 * Class for reading attributes out of a @ref QDomElement
 */
class PMXMLHelper
{
public:
   /**
    * Creates a PMXMLHelper for the QDomElement& e
    */
   PMXMLHelper( const QDomElement& e, PMPart* p, PMParser* par,
                int majorDocumentFormat, int minorDocumentFormat );
   /**
    * Returns the QDomElement
    */
   QDomElement element() const { return m_e; }

   /**
    * Returns true if the element contains the attribute
    */
   bool hasAttribute( const QString& name ) const;
   /**
    * Reads an integer attribute
    */
   int intAttribute( const QString& name, int def ) const;
   /**
    * Reads a double attribute
    */
   double doubleAttribute( const QString& name, double def ) const;
   /**
    * Reads a bool attribute
    */
   bool boolAttribute( const QString& name, bool def ) const;
   /**
    * Reads a PMThreeState attribute
    */
   PMThreeState threeStateAttribute( const QString& name ) const;
   /**
    * Reads a string attribute
    */
   QString stringAttribute( const QString& name, const QString& def ) const;
   /**
    * Reads a vector attribute
    */
   PMVector vectorAttribute( const QString& name, const PMVector& def ) const;
   /**
    * Reads a matrix attribute
    */
   PMMatrix matrixAttribute( const QString& name, const PMMatrix& def ) const;
   /**
    * Reads a color attribute
    */
   PMColor colorAttribute( const QString& name, const PMColor& def ) const;
   /**
    * Returns the "extra_data" child element or a null element, if there
    * is no child element with tag name "extra_data"
    */
   QDomElement extraData() const;
       
   /**
    * Returns a pointer to the part
    */
   PMPart* part() const { return m_pPart; }
   /**
    * Returns a pointer to the parser
    */
   PMParser* parser() const { return m_pParser; }
   /**
    * Returns the documents major format number
    */
   int majorDocumentFormat() const { return m_major; }
   /**
    * Returns the documents minor format number
    */
   int minorDocumentFormat() const { return m_minor; }
   
private:
   QDomElement m_e;
   PMPart* m_pPart;
   PMParser* m_pParser;
   int m_major;
   int m_minor;
};

#endif
