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


#include "pmtext.h"

#include "pmxmlhelper.h"
#include "pmtextedit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pmresourcelocator.h"
#include "pmtruetypecache.h"
#include "pmdefaults.h"


//Added by qt3to4:
#include <QTextStream>

const QString c_defaultFont = QString( "" );
const QString c_defaultText = QString( "" );
const double c_defaultThickness = 1.0;
const PMVector c_defaultOffset = PMVector( 0.0, 0.0 );

int PMText::s_parameterKey = 0;
int PMText::s_steps = c_defaultTextSteps;

PMDefinePropertyClass( PMText, PMTextProperty );

PMMetaObject* PMText::s_pMetaObject = nullptr;
PMObject* createNewText( PMPart* part )
{
   return new PMText( part );
}

PMText::PMText( PMPart* part )
      : Base( part )
{
   m_text = c_defaultText;
   m_font = c_defaultFont;
   m_thickness = c_defaultThickness;
   m_offset = c_defaultOffset;
}

PMText::PMText( const PMText& t )
      : Base( t )
{
   m_text = t.m_text;
   m_font = t.m_font;
   m_thickness = t.m_thickness;
   m_offset = t.m_offset;
}

PMText::~PMText()
{
}

QString PMText::description() const
{
   return ( "text" );
}

void PMText::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "font", m_font );
   e.setAttribute( "text", m_text );
   e.setAttribute( "thickness", m_thickness );
   e.setAttribute( "offset", m_offset.serializeXML() );
   Base::serialize( e, doc );
}

void PMText::readAttributes( const PMXMLHelper& h )
{
   m_font = h.stringAttribute( "font", c_defaultFont );
   m_text = h.stringAttribute( "text", c_defaultText );
   m_thickness = h.doubleAttribute( "thickness", c_defaultThickness );
   m_offset = h.vectorAttribute( "offset", c_defaultOffset );
   Base::readAttributes( h );
}

PMMetaObject* PMText::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Text", Base::metaObject(),
                                        createNewText );
      s_pMetaObject->addProperty(
         new PMTextProperty( "font", &PMText::setFont, &PMText::font ) );
      s_pMetaObject->addProperty(
         new PMTextProperty( "text", &PMText::setText, &PMText::text ) );
      s_pMetaObject->addProperty(
         new PMTextProperty( "thickness", &PMText::setThickness, &PMText::thickness ) );
      s_pMetaObject->addProperty(
         new PMTextProperty( "offset", &PMText::setOffset, &PMText::offset ) );
   }
   return s_pMetaObject;
}

void PMText::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMText::setFont( const QString& f )
{
   if( f != m_font )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFontID, m_font );
      m_font = f;
      setViewStructureChanged();
   }
}

void PMText::setText( const QString& t )
{
   if( t != m_text )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMTextID, m_text );
      m_text = t;
      setViewStructureChanged();
   }
}

void PMText::setThickness( double t )
{
   if( t != m_thickness )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMThicknessID, m_thickness );
      m_thickness = t;
      setViewStructureChanged();
   }
}

void PMText::setOffset( const PMVector& o )
{
   if( o != m_offset )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMOffsetID, m_offset );
      m_offset = o;
      m_offset.resize( 2 );
      setViewStructureChanged();
   }
}

PMDialogEditBase* PMText::editWidget( QWidget* parent ) const
{
   return new PMTextEdit( parent );
}

void PMText::restoreMemento( PMMemento* s )
{
	const QList<PMMementoData*>& changes = s->changes();
	QList<PMMementoData*>::const_iterator it;
   PMMementoData* data;
	for( it = changes.begin(); it != changes.end(); ++it )
	{
		data = *it;
      if( data->objectType() == s_pMetaObject )
      {
         switch( data->valueID() )
         {
            case PMFontID:
               setFont( data->stringData() );
               break;
            case PMTextID:
               setText( data->stringData() );
               break;
            case PMThicknessID:
               setThickness( data->doubleData() );
               break;
            case PMOffsetID:
               setOffset( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMText::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMText::createViewStructure()
{
   // calculate needed points and lines
   int nlines = 0, npoints = 0;

   QString file = PMResourceLocator::findFile( m_font );
   PMTrueTypeFont* font = PMTrueTypeCache::font( file );

   if( font && font->isValid() )
   {
      QTextStream str( &m_text, QIODevice::ReadOnly );
      QChar c;
      PMTrueTypeOutline* ol;

      while( !str.atEnd() )
      {
         str >> c;
         ol = font->outline( c );
         if( ol )
         {
            npoints += ol->segments() * 2 * s_steps;
            nlines += ol->segments() * ( 2 * s_steps + 1 );
         }
      }
   }

   if( !m_pViewStructure )
      m_pViewStructure = new PMViewStructure( npoints, nlines );
   else
   {
      if( m_pViewStructure->points().size() != npoints )
         m_pViewStructure->points().resize( npoints );
      if( m_pViewStructure->lines().size() != nlines )
         m_pViewStructure->lines().resize( nlines );
   }

   if( ( nlines > 0 ) && ( npoints > 0 ) && font )
   {
      // create the view structure
      QTextStream str( &m_text, QIODevice::ReadOnly );
      QChar c, oldc;
      PMTrueTypeOutline* ol;
      double dp = 1.0 / s_steps;
      int i;
      int hnpoints = npoints / 2;
      int pbase = 0;
      int lbase = 0;
      PMVector v2( 2 );
      PMVector v3( 3 );
      int firstPoint = 0;
      PMVector coffset( 0.0, 0.0, 0.0 );
      double kerning = 0;

      PMPointArray& points = m_pViewStructure->points();
      PMLineArray& lines = m_pViewStructure->lines();

      while( !str.atEnd() )
      {
         // iterate over all characters with valid outline

         str >> c;
         ol = font->outline( c );
         if( ol )
         {
            // kerning offset
            kerning = font->kerning( oldc, c );
            coffset[0] += kerning;

            const PMSegmentListList& out = ol->outline();
            PMSegmentListList::ConstIterator oit;
            for( oit = out.begin(); oit != out.end(); ++oit )
            {
               // iterate over all contours

               PMSegmentList::ConstIterator sit;
               PMSegmentList::ConstIterator eit = ( *oit ).end();
               eit--;

               firstPoint = pbase;
               for( sit = ( *oit ).begin(); sit != ( *oit ).end(); ++sit )
               {
                  // iterate over all segments for the current contour

                  lines[lbase] = PMLine( pbase, pbase + hnpoints );
                  lbase++;

                  for( i = 0; i < s_steps; i++ )
                  {
                     v2 = ( *sit ).point( i * dp );
                     v3[0] = v2[0];
                     v3[1] = v2[1];
                     v3[2] = 0.0;
                     v3 += coffset;
                     points[pbase] = PMPoint( v3 );
                     v3[2] = m_thickness;
                     points[pbase + hnpoints] = PMPoint( v3 );

                     if( ( i != ( s_steps - 1 ) ) || ( sit != eit ) )
                     {
                        lines[lbase] = PMLine( pbase, pbase + 1 );
                        lbase++;
                        lines[lbase] = PMLine( pbase + hnpoints,
                                               pbase + hnpoints + 1 );
                        lbase++;
                     }
                     else
                     {
                        lines[lbase] = PMLine( firstPoint, pbase );
                        lbase++;
                        lines[lbase] = PMLine( firstPoint + hnpoints,
                                               pbase + hnpoints );
                        lbase++;
                     }

                     pbase++;
                  }
               }
            }
            coffset[0] -= kerning;
            coffset[0] += ol->advance();
            coffset += m_offset;
         }
         oldc = c;
      }
      if( ( lbase != nlines ) || ( pbase != hnpoints ) )
         qCritical(  ) << "PMText::createViewStructure is buggy!\n";
   }
}

void PMText::setSteps( int s )
{
   if( s >= 1 )
      s_steps = s;
   else
      qDebug() << "PMText::setSteps: S must be greater than 0\n";
   s_parameterKey++;
}
