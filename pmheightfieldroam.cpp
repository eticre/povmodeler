/*
**************************************************************************
                               description
                           --------------------
  copyright            : (C) 2003 by Leon Pennington
  email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/
#include "pmheightfieldroam.h"

#include <cstdlib>
#include <new>

#include <qstring.h>
#include <qimage.h>
#include <qcolor.h>
#include <QVector>


PMHeightFieldROAM::PMHeightFieldROAM( const QString &fileName )
{
   m_size = 0;
   m_numLines = m_usedPoints = 0;
   m_waterLevel = m_displayDetail = 0;
   m_mapMod = m_levelMod = true;
   m_fail = false;
   m_pPoints = nullptr;
   m_pTree = nullptr;

   if ( !imageToData( fileName ) )
   {
      if ( m_pPoints )
         delete [] m_pPoints;
      if ( m_pTree )
         delete [] m_pTree;
      m_pPoints = nullptr;
      m_pTree = nullptr;
      m_pNextNode = nullptr;
      m_size = m_numPoints = m_numNodes = 0;
      m_fail = true;
      return;
   }

   calcLevel();
}

PMHeightFieldROAM::~PMHeightFieldROAM()
{
   delete [] m_pPoints;
   delete [] m_pTree;
}

void PMHeightFieldROAM::updateModel()
{
   if ( m_fail )
      return;
   int z = m_size - 1;
   if ( m_mapMod )
   {
      m_mapMod = false;
      m_levelMod = false;
      clearPoints();
      clearNodes( true );

      m_pNextNode = m_pTree + 1;
      m_pNextNode->base = m_pTree;
      m_pTree->base = m_pNextNode++;

      varNode(m_pTree, z, 0, 0, 0, 0, z, 0);
      varNode(m_pTree + 1, 0, z, z, z, z, 0, 0);

      sptNode(m_pTree, 0);
      sptNode(m_pTree + 1, 0);

      pntNode(m_pTree, z, 0, 0, 0, 0, z);
      pntNode(m_pTree + 1, 0, z, z, z, z, 0);
   }
   else if ( m_levelMod )
   {
      m_levelMod = false;
      clearPoints();
      clearNodes();

      sptNode( m_pTree, 0 );
      sptNode( m_pTree + 1, 0 );

      pntNode( m_pTree, z, 0, 0, 0, 0, z );
      pntNode( m_pTree + 1, 0, z, z, z, z, 0 );
   }
}

void PMHeightFieldROAM::setDisplayDetail( int detail )
{
   if ( detail != m_displayDetail )
   {
      m_displayDetail = detail;
      m_levelMod = true;
   }
}

void PMHeightFieldROAM::setWaterLevel( double waterLevel )
{
   int waterConv = (int)( waterLevel * 65535 );
   if ( waterConv != m_waterLevel )
   {
      m_waterLevel = waterConv;
      m_levelMod = true;
   }
}

double PMHeightFieldROAM::waterLevel() const
{
   double waterConv = m_waterLevel / 65535.0;
   return waterConv;
}

unsigned short PMHeightFieldROAM::height( int x, int y, bool waterLevel ) const
{
   unsigned short hgt = m_pPoints[ x + ( y * m_size ) ].hgt;
   if ( waterLevel )
   {
      if ( hgt <= m_waterLevel )
         return m_waterLevel;
      else
         return hgt;
   }
   else
      return hgt;
}

bool PMHeightFieldROAM::lineExist( int x, int y, int line ) const
{
   if ( m_pPoints[ x + ( y * m_size ) ].lines[ line ] ) return true;
   else return false;
}

bool PMHeightFieldROAM::imageToData(const QString &fileName)
{
   QImage scaledMap;
   QImage mapFile( fileName );

   if ( mapFile.isNull() )
      return false;

   if ( mapFile.width() > 192 || mapFile.height() > 192 )
   {
      scaledMap = mapFile.scaled( 257, 257 );
   }
   else if ( mapFile.width() > 96 || mapFile.height() > 96 )
   {
      scaledMap = mapFile.scaled( 129, 129 );
   }
   else if ( mapFile.width() > 48 || mapFile.height() > 48)
   {
      scaledMap = mapFile.scaled( 65, 65 );
   }
   else
   {
      scaledMap = mapFile.scaled( 33, 33 );
   }

   if ( scaledMap.isNull() )
      return false;

   m_size = scaledMap.width();
   if ( !createPoints() || !createNodes() )
      return false;

   bool colourIndex;

   if ( mapFile.depth() > 8 )
      colourIndex = false;
   else
      colourIndex = true;

   for ( int y = 0, y2 = ( m_size - 1 ) ; y < m_size ; ++y, --y2 )
   {
      for ( int x = 0 ; x < m_size ; ++x )
      {
         if ( colourIndex )
            setHeight( x, y2, scaledMap.pixelIndex( x, y ) * 256 );
         else
            setHeight( x, y2, ( 256 * qRed( scaledMap.pixel( x, y ) ) ) +
                             qGreen( scaledMap.pixel( x, y ) ) );
      }
   }

   return true;
}

void PMHeightFieldROAM::calcLevel()
{
   int i = 0;
   int j = m_size;

   while( j != 1)
   {
      j /= 2;
      i++;
   }
   m_maxLevel = i * 2;
}

void PMHeightFieldROAM::varNode ( triNodeStructure* current,
                                int x1, int y1,
                                int x2, int y2,
                                int x3, int y3,
                                int level )
{
   int xm = (x1 + x3) >> 1;
   int ym = (y1 + y3) >> 1;

   if ( level >= m_maxLevel )
   {
      unsigned short z1 = height( x1, y1 );
      unsigned short z3 = height( x3, y3 );

      unsigned short zm = ( ( z3 - z1 ) / 2 ) + z1;
      unsigned short hgt = height( xm, ym );

      current->vari = abs( zm - hgt );
      return;
   }

   current->lchd = m_pNextNode++;
   current->rchd = m_pNextNode++;

   varNode(current->lchd, x3, y3, xm, ym, x2, y2, level + 1);
   varNode(current->rchd, x2, y2, xm, ym, x1, y1, level + 1);

   current->vari = current->lchd->vari + current->rchd->vari;
}

void PMHeightFieldROAM::sptNode ( triNodeStructure* current, int level )
{
   if ( !current->split )
   {
      if ( level >= m_maxLevel ) return;

      if (current->vari > m_displayDetail) split(current);
      else return;
   }

   sptNode(current->lchd, level + 1);
   sptNode(current->rchd, level + 1);
}

void PMHeightFieldROAM::split( triNodeStructure* current )
{
   current->split = true;

   if ( current->base )
   {
      if ( current->base->base != current ) split( current->base );
   }

   triNodeStructure* child;

   //left child
   child = current->lchd;
   child->base = current->lnbr;
   if ( current->lnbr )
   {
      if ( current->lnbr->rnbr == current ) current->lnbr->rnbr = child;
      else current->lnbr->base = child;
   }
   child->lnbr = current->rchd;

   //rightchild
   child = current->rchd;
   child->base = current->rnbr;
   if ( current->rnbr )
   {
      if ( current->rnbr->lnbr == current ) current->rnbr->lnbr = child;
      else current->rnbr->base = child;
   }
   child->rnbr = current->lchd;

   if ( current->base )
   {
      if ( !current->base->split ) split( current->base );
      current->lchd->rnbr = current->base->rchd;
      current->rchd->lnbr = current->base->lchd;
   }
}

void PMHeightFieldROAM::pntNode( triNodeStructure* current,
                               int x1, int y1,
                               int x2, int y2,
                               int x3, int y3 )
{
   if (current->split)
   {
      int xm = (x1 + x3) >> 1;
      int ym = (y1 + y3) >> 1;
      pntNode( current->lchd, x3, y3, xm, ym, x2, y2 );
      pntNode( current->rchd, x2, y2, xm, ym, x1, y1 );
   }
   else
   {
      pointStructure* pts[3];
      pts[0] = &m_pPoints[ x1 + ( y1 * m_size ) ];
      pts[1] = &m_pPoints[ x2 + ( y2 * m_size ) ];
      pts[2] = &m_pPoints[ x3 + ( y3 * m_size ) ];

      if ( m_waterLevel != 0 )
      {
         if ( pts[0]->hgt <= m_waterLevel &&
              pts[1]->hgt <= m_waterLevel &&
              pts[2]->hgt <= m_waterLevel )
         return;
      }

      for ( int i = 0 ; i < 3 ; ++i )
      {
         if ( !pts[i]->used )
         {
            pts[i]->pos = m_usedPoints++;
            pts[i]->used = true;
         }
      }

      addLine( pts[0], pts[1] );
      addLine( pts[1], pts[2] );
      addLine( pts[2], pts[0] );
   }
}

void PMHeightFieldROAM::addLine( pointStructure* pts1, pointStructure* pts2 )
{
   for ( int i = 0 ; i < 8 ; ++i )
   {
      if ( pts1->lines[i] )
      {
         if ( pts1->lines[i] == pts2 ) return;
      }
      else
      {
         for ( int j = 0 ; pts2->lines[j] ; ++j )
         {
            if ( pts2->lines[j] == pts1 ) return;
         }
         pts1->lines[i] = pts2;
         m_numLines++;
         return;
      }
   }
}

bool PMHeightFieldROAM::createPoints()
{
   m_numPoints = m_size * m_size;
   m_pPoints = new( std::nothrow ) pointStructure[ m_numPoints ];
   if ( !m_pPoints )
      return false;
   else
   {
      clearPoints( true );
      return true;
   }
}

void PMHeightFieldROAM::clearPoints( bool all )
{
   int i, j;
   for ( i = 0 ; i < m_numPoints ; ++i )
   {
      if ( all )
      {
         m_pPoints[i].hgt = 0;
         m_pPoints[i].pos = 0;
      }
      for ( j = 0 ; j < 8 ; ++j )
         m_pPoints[i].lines[j] = nullptr;
      m_pPoints[i].used = false;
   }

   m_usedPoints = m_numLines = 0;
}

bool PMHeightFieldROAM::createNodes()
{
   m_numNodes = ( ( m_size - 1 ) * ( 4 * ( m_size - 1 ) ) ) - 2;
   m_pTree = new( std::nothrow ) triNodeStructure[ m_numNodes ];
   if ( !m_pTree )
      return false;
   else
   {
      clearNodes( true );
      return true;
   }
}

void PMHeightFieldROAM::clearNodes( bool all )
{
   m_pNextNode = m_pTree;
   for ( int i = 0; i < m_numNodes; ++i )
   {
      if ( all )
      {
         m_pNextNode->lchd = nullptr;
         m_pNextNode->rchd = nullptr;
         m_pNextNode->base = nullptr;
         m_pNextNode->lnbr = nullptr;
         m_pNextNode->rnbr = nullptr;
         m_pNextNode->vari = 0;
      }
      m_pNextNode->split = false;
      m_pNextNode++;
   }
}
