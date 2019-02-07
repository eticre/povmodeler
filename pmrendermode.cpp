/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2006 by Andreas Zehender
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

#include "pmrendermode.h"


#include "pmxmlhelper.h"


PMRenderMode::PMRenderMode()
{
   init();
}

void PMRenderMode::init()
{
   m_width = 640;
   m_height = 480;

   m_subSection = false;
   m_startRow = 1;
   m_endRow = m_height;
   m_startColumn = 1;
   m_endColumn = m_width;

   m_quality = 9;
   
   m_radiosity = false;
   
   m_antialiasing = false;
   m_samplingMethod = AntialiasingNonRecursive;
   m_antialiasThreshold = 0.3;
   m_antialiasJitter = false;
   m_antialiasJitterAmount = 1.0;
   m_antialiasDepth = 2;

   m_description = ( "New mode" );

   m_alpha = false;
   m_preview = false;
}

PMRenderMode::PMRenderMode( const QDomElement& e )
{
   init();

   PMXMLHelper hlp( e, nullptr, nullptr, 0, 0 );
   m_description = hlp.stringAttribute( "description", m_description );
   m_height = hlp.intAttribute( "height", m_height );
   m_width = hlp.intAttribute( "width", m_width );
   m_subSection = hlp.boolAttribute( "subsection", m_subSection );
   m_startColumn = hlp.doubleAttribute( "start_column", m_startColumn );
   m_endColumn = hlp.doubleAttribute( "end_column", m_endColumn );
   m_startRow = hlp.doubleAttribute( "start_row", m_startRow );
   m_endRow = hlp.doubleAttribute( "end_row", m_endRow );
   m_quality = hlp.intAttribute( "quality", m_quality );
   m_radiosity = hlp.boolAttribute( "radiosity", m_radiosity );
   m_antialiasing = hlp.boolAttribute( "antialiasing", m_antialiasing );
   m_samplingMethod = hlp.intAttribute( "sampling_method", m_samplingMethod );
   m_antialiasThreshold = hlp.doubleAttribute( "aa_threshold", m_antialiasThreshold );
   m_antialiasJitter = hlp.boolAttribute( "aa_jitter", m_antialiasJitter );
   m_antialiasJitterAmount = hlp.doubleAttribute( "aa_jitter_amount", m_antialiasJitterAmount );
   m_antialiasDepth = hlp.intAttribute( "aa_depth", m_antialiasDepth );
   m_alpha = hlp.boolAttribute( "alpha", m_alpha );
   //emit s_radYN();
}

void PMRenderMode::serialize( QDomElement& e ) const
{
   e.setAttribute( "description", m_description );
   e.setAttribute( "height", m_height );
   e.setAttribute( "width", m_width );
   e.setAttribute( "subsection", m_subSection );
   e.setAttribute( "start_row", m_startRow );
   e.setAttribute( "end_row", m_endRow );
   e.setAttribute( "start_column", m_startColumn );
   e.setAttribute( "end_column", m_endColumn );
   e.setAttribute( "quality", m_quality );
   e.setAttribute( "radiosity", m_radiosity );
   e.setAttribute( "antialiasing", m_antialiasing );
   e.setAttribute( "sampling_method", m_samplingMethod );
   e.setAttribute( "aa_threshold", m_antialiasThreshold );
   e.setAttribute( "aa_jitter", m_antialiasJitter );
   e.setAttribute( "aa_jitter_amount", m_antialiasJitterAmount );
   e.setAttribute( "aa_depth", m_antialiasDepth );
   e.setAttribute( "alpha", m_alpha );
}

void PMRenderMode::setHeight( int height )
{
   if( height >= 1 )
      m_height = height;
}

void PMRenderMode::setWidth( int width )
{
   if( width >= 1 )
      m_width = width;
}

void PMRenderMode::setStartRow( double startRow )
{
   if( startRow >= 0 )
      m_startRow = startRow;
}

void PMRenderMode::setEndRow( double endRow )
{
   if( endRow >= 0 )
      m_endRow = endRow;
}

void PMRenderMode::setStartColumn( double startColumn )
{
   if( startColumn >= 0 )
      m_startColumn = startColumn;
}

void PMRenderMode::setEndColumn( double endColumn )
{
   if( endColumn >= 0 )
      m_endColumn = endColumn;
}

void PMRenderMode::setQuality( int quality )
{
   if( ( quality >= 0 ) && ( quality <= 11 ) )
      m_quality = quality;
}

void PMRenderMode::setAntialiasingDepth( int depth )
{
   if( ( depth >= 1 ) && ( depth <= 9 ) )
      m_antialiasDepth = depth;
}

void PMRenderMode::setSamplingMethod( int method )
{
   if( ( method == AntialiasingNonRecursive )
       || ( method ==  AntialiasingRecursive ) )
      m_samplingMethod = method;
   else
      m_samplingMethod = AntialiasingNonRecursive;
}

QStringList PMRenderMode::commandLineSwitches() const
{
   QStringList cl;
   QString tmp;

   cl.append( QString( "+W%1" ).arg( m_width ) );
   cl.append( QString( "+H%1" ).arg( m_height ) );
   if( m_subSection )
   {
      if( m_startRow < 1.0 )
         tmp.sprintf( "+SR%4.2f", m_startRow );
      else
         tmp = QString( "+SR%1" ).arg( ( int ) ( m_startRow + 0.5 ) );
      cl.append( tmp );
      if( m_endRow < 1.0 )
         tmp.sprintf( "+ER%4.2f", m_endRow );
      else
         tmp = QString( "+ER%1" ).arg( ( int ) ( m_endRow + 0.5 ) );
      cl.append( tmp );
      
      if( m_startColumn < 1.0 )
         tmp.sprintf( "+SC%4.2f", m_startColumn );
      else
         tmp = QString( "+SC%1" ).arg( ( int ) ( m_startColumn + 0.5 ) );
      cl.append( tmp );
      if( m_endColumn < 1.0 )
         tmp.sprintf( "+EC%4.2f", m_endColumn );
      else
         tmp = QString( "+EC%1" ).arg( ( int ) ( m_endColumn + 0.5 ) );
      cl.append( tmp );
   }
   cl.append( QString( "+Q%1" ).arg( m_quality ) );
/* eticre mod for povray 3.7
   if( m_radiosity )
      cl.append( QString( "+QR" ) );
   else
      cl.append( QString( "-QR" ) );
*/
   if( m_antialiasing )
   {
      cl.append( QString( "+A" ) );
      cl.append( QString( "+AM%1" ).arg( m_samplingMethod ) );
      tmp.sprintf( "+A%5.3f", m_antialiasThreshold );
      cl.append( tmp );
      if( m_antialiasJitter )
      {
         tmp.sprintf( "+J%5.3f", m_antialiasJitterAmount );
         cl.append( tmp );
      }
      else
         cl.append( QString( "-J" ) );
      cl.append( QString( "+R%1" ).arg( m_antialiasDepth ) );
   }
   else
      cl.append( QString( "-A" ) );

   if( m_alpha )
      cl.append( QString( "+UA" ) );
   else
      cl.append( QString( "-UA" ) );

   return cl;
}

