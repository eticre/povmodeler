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

#ifndef PMTRUETYPECACHE_H
#define PMTRUETYPECACHE_H


#include "pmsplinesegment.h"

#include <QCache>

#include "config-povmodeler.h"

#ifdef HAVE_FREETYPE
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#endif

// TODO check if freetype can be replaced by QPainterPath

class PMTrueTypeFont;

/**
 * Cache for truetype fonts
 */
class PMTrueTypeCache
{
public:
   /**
    * Returns a pointer to the font with file name "file" or 0, if the
    * font does not exist or is no truetype font
    *
    * Returns 0 if freetype is not available.
    */
   static PMTrueTypeFont* font( const QString& file );
   
#ifdef HAVE_FREETYPE
   
   /**
    * Destructor
    */
   ~PMTrueTypeCache();
   
private:
   static PMTrueTypeCache* s_pInstance;
   //static K3StaticDeleter<PMTrueTypeCache> s_staticDeleter;

   /**
    * Lookup function
    */
   PMTrueTypeFont* lookUp( const QString& file );
   
   QCache<QString, PMTrueTypeFont> m_cache;
   FT_Library m_library;

#endif //HAVE_FREETYPE
   
private:
   /**
    * Standard constructor
    */
   PMTrueTypeCache();
};

/**
 * Class that represents a truetype character outline
 */
class PMTrueTypeOutline
{
public:
#ifdef HAVE_FREETYPE
   /**
    * Constructor that generates the outline for the glyph.
    * The outline is scaled to match the font height.
    */
   PMTrueTypeOutline( FT_OutlineGlyph glyph, FT_Face face );
#else
   /**
    * Standard constructor.
    *
    * Don't use this constructor. It is only added as dummy if freetype
    * is not installed.
    */   
   PMTrueTypeOutline()
   {
      m_contours = 0;
      m_segments = 0;
      m_advance = 0;
   }
#endif
   /**
    * Returns the outline
    */
   const PMSegmentListList& outline() const { return m_outline; }
   /**
    * Returns the number of contours
    */
   int contours() const { return m_contours; }
   /**
    * Returns the sum of the number of segments for all contours
    */
   int segments() const { return m_segments; }
   /**
    * Returns the offset for the next character
    */
   double advance() const { return m_advance; }
       
   PMSegmentListList m_outline;
   int m_contours;
   int m_segments;
   double m_advance;
};

/**
 * Class that represents a truetype font.
 *
 * This class caches the glyph outlines.
 */
class PMTrueTypeFont
{
public:
   /**
    * Returns the outline for the character
    *
    * Returns 0 if there is no glyph for the character or the font is
    * not a valid, scalable true type font.
    */
   PMTrueTypeOutline* outline( QChar c );
   /**
    * Returns true if the font is a valid, scalable true type font
    */
   bool isValid();
   /**
    * Returns the kerning offset for the two characters
    */
   double kerning( QChar c1, QChar c2 );
   
#ifdef HAVE_FREETYPE
   
public:
   /**
    * Creates a true type font
    */
   PMTrueTypeFont( FT_Library lib, FT_Face face );
   /**
    * Deletes the true type font
    */
   ~PMTrueTypeFont();

private:
   FT_UInt findGlyphIndex( QChar c );
   
   FT_Library m_library;
   FT_Face m_face;
   
   bool m_valid;
   bool m_validChecked;
   bool m_useKerning;

   QCache<QString, PMTrueTypeOutline> m_cache;
   
#else //!HAVE_FREETYPE
   
public:
   /**
    * Standard constructor.
    *
    * Don't use this constructor. It is only added as dummy if freetype
    * is not installed.
    */
   PMTrueTypeFont();
   
#endif //HAVE_FREETYPE
   
};

#endif
