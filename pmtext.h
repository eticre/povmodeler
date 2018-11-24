//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMTEXT_H
#define PMTEXT_H



#include "pmsolidobject.h"
#include "pmvector.h"


/**
 * Class for povray truetype texts.
 */

class PMText : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Creates an empty PMText
    */
   PMText( PMPart* part );
   /**
    * Copy constructor
    */
   PMText( const PMText& t );
   /**
    * deletes the PMText
    */
   virtual ~PMText();

   /** */
   virtual PMObject* copy() const { return new PMText( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMTextEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmtext" ); }

   /**
    * Returns the font file
    */
   QString font() const { return m_font; }
   /**
    * Sets the font file
    */
   void setFont( const QString& f );
   /**
    * Returns the text
    */
   QString text() const { return m_text; }
   /**
    * Sets the text
    */
   void setText( const QString& t );
   /**
    * Returns the thickness
    */
   double thickness() const { return m_thickness; }
   /**
    * Sets the thickness
    */
   void setThickness( double t );
   /**
    * Returns the additional offset (2D vector)
    */
   PMVector offset() const { return m_offset; }
   /**
    * Sets the offset (2D vector)
    */
   void setOffset( const PMVector& o );

   /** */
   virtual void restoreMemento( PMMemento* s );

   /**
    * Call this if the povray library paths have changed
    */
   static void povrayLibraryPathsChanged() { s_parameterKey++; }
   /**
    * Returns the number of lines for rendering
    */
   static int steps() { return s_steps; }
   /**
    * Sets the number of lines for rendering
    */
   static void setSteps( int s );

protected:
   /** */
   virtual void createViewStructure();
   /** */
   virtual int viewStructureParameterKey() const { return s_parameterKey; }

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMTextMementoID { PMFontID, PMTextID, PMThicknessID, PMOffsetID };
   QString m_font, m_text;
   double m_thickness;
   PMVector m_offset;

   static int s_steps;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
