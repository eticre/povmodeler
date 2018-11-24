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

#include "pmtruetypecache.h"
#include "pmdebug.h"

//***********************************************************************
// Part with freetype support
//***********************************************************************

#ifdef HAVE_FREETYPE
#define PMFREETYPEDEBUG

PMTrueTypeCache* PMTrueTypeCache::s_pInstance = nullptr;
//K3StaticDeleter<PMTrueTypeCache> PMTrueTypeCache::s_staticDeleter;

PMTrueTypeCache::PMTrueTypeCache()
      : m_cache( 10 )
{
   bool error = FT_Init_FreeType( &m_library );
   if( error )
      qCritical(  ) << "Failed to initialize the freetype library\n";
   
#ifdef PMFREETYPEDEBUG   
   else
      qDebug() << "Freetype 2 initialized\n";
#endif
}

PMTrueTypeCache::~PMTrueTypeCache()
{
   m_cache.clear();  
   if( m_library )
      FT_Done_FreeType( m_library );
}

PMTrueTypeFont* PMTrueTypeCache::lookUp( const QString& file )
{
    if( !m_library )
    {
        return nullptr;
    }
   if( file.isEmpty() )
   {
       return nullptr;
   }
   
   PMTrueTypeFont* f = m_cache.object( file );
   if( !f )
   {
      FT_Face face;
      FT_New_Face( m_library, file.toLatin1(), 0, &face );
      f = new PMTrueTypeFont( m_library, face );
      
#ifdef PMFREETYPEDEBUG   
      if( face )
         qDebug() << "Successfully opened font " << file;

      if( f->isValid() )
         m_cache.insert( file, f, 1 );
      else
         m_cache.insert( file, f, 0 );
#endif
   }

   if( f->isValid() )
   {
      return f;
   }
   
   return nullptr;
}

PMTrueTypeFont* PMTrueTypeCache::font( const QString& file )
{
   if( !s_pInstance )
      s_pInstance = new PMTrueTypeCache();

       //s_staticDeleter.setObject( s_pInstance, new PMTrueTypeCache() );

   return s_pInstance->lookUp( file );
}

PMTrueTypeFont::PMTrueTypeFont( FT_Library lib, FT_Face face )
   : m_cache( 100 )
{
   m_library = lib;
   m_face = face;
   m_valid = false;
   m_validChecked = false;
   m_useKerning = false;
   if( m_face )
   {
      m_useKerning = FT_HAS_KERNING( m_face );
      // find the correct encoding
      int i;
      bool found = false;
      for( i = 0; ( i < m_face->num_charmaps ) && !found; i++ )
         if( m_face->charmaps[i]->platform_id == 3 ) // microsoft encodings
            FT_Set_Charmap( m_face, m_face->charmaps[i] );
      for( i = 0; ( i < m_face->num_charmaps ) && !found; i++ )
         if( m_face->charmaps[i]->platform_id == 1 ) // mac encodings
            FT_Set_Charmap( m_face, m_face->charmaps[i] );
   }
}

PMTrueTypeFont::~PMTrueTypeFont()
{
   if( m_face )
      FT_Done_Face( m_face );
   m_cache.clear();
}

bool PMTrueTypeFont::isValid()
{
   if( !m_validChecked )
   {
      if( !m_face )
         m_valid = false;
      else
         m_valid = m_face->face_flags & FT_FACE_FLAG_SCALABLE;

#ifdef PMFREETYPEDEBUG   
      if( m_valid )
         qDebug() << "Font: " << m_face->family_name
                           << " style " << m_face->style_name
                           << " units_per_EM " << m_face->units_per_EM
                           << " height " << m_face->height << endl;
#endif
            
      m_validChecked = true;
   }
   
   return m_valid;
}

PMTrueTypeOutline* PMTrueTypeFont::outline( QChar c )
{
   PMTrueTypeOutline* ol = nullptr;
   
   if( isValid() )
   {
      QString str( c );
      ol = m_cache.object( str );
      if( !ol )
      {
         FT_UInt glyphIndex = findGlyphIndex( c );
         
         bool error = !glyphIndex;
         FT_Glyph glyph = nullptr;
         
         if( !error )
            error = FT_Load_Glyph( m_face, glyphIndex,
                                   FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE );
         if( !error )
            error = FT_Get_Glyph( m_face->glyph, &glyph );
         
#ifdef PMFREETYPEDEBUG
         if( error )
            qDebug() << "Failed to load glyph for " << c.toLatin1() << "\n";
         else
         {
            FT_Glyph_Metrics* m = &( m_face->glyph->metrics );
            qDebug() << "Glyph w: " << m->width << " h: " << m->height
                              << " hbx: " << m->horiBearingX << " hby: " << m->horiBearingY
                              << " ha: " << m->horiAdvance << endl;
         }
#endif
         
         if( !error && glyph && ( glyph->format == ft_glyph_format_outline ) )
         {
            FT_OutlineGlyph outlineGlyph = ( FT_OutlineGlyph ) glyph;
            ol = new PMTrueTypeOutline( outlineGlyph, m_face );
         }

         if( glyph )
            FT_Done_Glyph( glyph );
         if( ol )
            m_cache.insert( str, ol );
      }
   }
   
   return ol;
}

double PMTrueTypeFont::kerning( QChar c1, QChar c2 )
{
   double k = 0.0;
   if( m_useKerning && !c1.isNull() && !c2.isNull() )
   {
      FT_UInt gi1 = findGlyphIndex( c1 );
      FT_UInt gi2 = findGlyphIndex( c2 );
      if( gi1 && gi2 )
      {
         FT_Vector delta;
         FT_Get_Kerning( m_face, gi1, gi2, ft_kerning_unscaled, &delta );
         k = ( double ) delta.x / ( double ) m_face->units_per_EM;
      }
   }

   return k;
}

FT_UInt PMTrueTypeFont::findGlyphIndex( QChar c )
{
   FT_UInt glyphIndex = 0;

   if( m_face )
   {
      // glyphIndex = FT_Get_Char_Index( m_face, c.unicode() );
      // if( !glyphIndex )
      char ch = c.toLatin1();
      if( !ch )
         ch = '?';
      glyphIndex = FT_Get_Char_Index( m_face, ch );
   }
   return glyphIndex;
}

PMTrueTypeOutline::PMTrueTypeOutline( FT_OutlineGlyph glyph, FT_Face face )
{
   int n = 0, p = 0, si;
   FT_Outline* ol = &( glyph->outline );
   
   PMVector v[4];
   bool onPoint[4] = { false, false, false, false };
   bool cubic[4] = { false, false, false, false };
   
   double dfh = ( double ) face->units_per_EM;
   double horiBearing = ( double ) face->glyph->metrics.horiBearingX / dfh;

   m_segments = 0;
   m_contours = ol->n_contours;
   m_advance = ( double ) face->glyph->metrics.horiAdvance / dfh;

#ifdef PMFREETYPEDEBUG

   /**
   qDebug() << "New outline:\n";
   int dn, dp = 0;
   for( dn = 0; dn < m_contours; dn++ )
   {
      qDebug() << "  Contour " << dn << ":\n";
      for( ; dp <= ol->contours[dn]; dp++ )
      {         
         qDebug() << "    <" << ol->points[dp].x << ", "
                           << ol->points[dp].y << ">, "
                           << ( ( ol->tags[dp] & 1 ) == 1 ) << " "
                           << ( ( ol->tags[dp] & 2 ) == 2 ) << endl;
      }
   }
   */
   
#endif
   
   for( n = 0; n < m_contours; n++ )
   {
      PMSegmentList os;
      PMSplineSegment s;
      bool segmentCreated = false;
      bool quadricSpecialCase = false;
      bool contourEnd = false;
      int firstPoint = p;
      
      si = 0;
      
      for( ; !contourEnd; p++, si++ )
      {
         segmentCreated = false;
         quadricSpecialCase = false;
         
         // last point = first point
         if( p > ol->contours[n] )
         {
            p = firstPoint;
            contourEnd = true;
         }

         // scale the point
         v[si] = PMVector( ( double ) ol->points[p].x / dfh - horiBearing,
                           ( double ) ol->points[p].y / dfh );
         // point type
         onPoint[si] = ( ( ol->tags[p] & 1 ) == 1 );
         cubic[si] = ( ( ol->tags[p] & 2 ) == 2 );

         if( onPoint[si] )
         {
            switch( si )
            {
               case 0:
                  break;
               case 1:
                  // line
                  s.calculateLinear( v[0], v[1] );
                  segmentCreated = true;
                  break;
               case 2:
                  // quadric bezier
                  s.calculateQuadricBezier( v[0], v[1], v[2] );
                  segmentCreated = true;
                  break;
               case 3:
                  // cubic bezier
                  s.calculateBezier( v[0], v[1], v[2], v[3] );
                  segmentCreated = true;
                  break;
               default:
                  qCritical(  ) << "Glyph outline seems incorrect. No on point.\n";
                  si = 0;
                  break;
            }
         }
         else if( ( si == 2 ) && ( !cubic[si] ) )
         {
            // two quadric off points
            // add an on point between them
            v[3] = v[2];
            onPoint[3] = onPoint[2];
            cubic[3] = cubic[2];
            v[2] = ( v[1] + v[3] ) / 2.0;
            onPoint[2] = true;
            
            s.calculateQuadricBezier( v[0], v[1], v[2] );
            segmentCreated = true;
            quadricSpecialCase = true;
         }

         if( segmentCreated )
         {
            os.append( s );
            v[0] = v[si];
            onPoint[0] = true;
            si = 0;
            
            if( quadricSpecialCase )
            {
               v[1] = v[3];
               onPoint[1] = onPoint[3];
               cubic[1] = onPoint[3];
               si++;
            }
         }
      }

      m_outline.append( os );
      m_segments += os.count();
      p = ol->contours[n] + 1;
   }
}

#else //!HAVE_FREETYPE

//***********************************************************************
// Part without freetype support
//***********************************************************************

PMTrueTypeCache::PMTrueTypeCache()
{
}

PMTrueTypeFont* PMTrueTypeCache::font( const QString& )
{
   return 0;
}

PMTrueTypeFont::PMTrueTypeFont()
{
}

bool PMTrueTypeFont::isValid()
{
   return false;
}

PMTrueTypeOutline* PMTrueTypeFont::outline( QChar )
{
   return 0;
}

double PMTrueTypeFont::kerning( QChar, QChar )
{
   return 0;
}

#endif //HAVE_FREETYPE
