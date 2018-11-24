/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#include "pmtriangleedit.h"
#include "pmtriangle.h"
#include "pmvectoredit.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>

#include <QMessageBox>

PMTriangleEdit::PMTriangleEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = 0;
}

void PMTriangleEdit::createTopWidgets()
{
   Base::createTopWidgets();

   int i;
   QBoxLayout* tl = topLayout();

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pSmooth = new QCheckBox( tr( "Smooth" ), this );
   hl->addWidget( m_pSmooth );
   connect( m_pSmooth, SIGNAL( toggled( bool ) ),
            SLOT( slotSmoothChecked( bool ) ) );

   m_pUVEnabled = new QCheckBox( tr( "UV vectors" ), this );
   hl->addWidget( m_pUVEnabled );
   connect( m_pUVEnabled, SIGNAL( toggled( bool ) ),
            SLOT( slotUVVectorsChecked( bool ) ) );

   QGridLayout* gl = new QGridLayout();
   tl->addLayout( gl );

   for( i = 0; i < 3; i++ )
   {
      m_pPoint[i] = new PMVectorEdit( "x", "y", "z", this );
      gl->addWidget( new QLabel( ( "Point :" + QString::number( i+1 ) ), this ),
                     i * 3, 0 );
      gl->addWidget( m_pPoint[i], i * 3, 1 );
      connect( m_pPoint[i], SIGNAL( dataChanged() ),
               SIGNAL( dataChanged() ) );

      m_pNormal[i] = new PMVectorEdit( "x", "y", "z", this );
      m_pNormalLabel[i] = new QLabel( ( "Normal :" + QString::number( i+1 ) ), this );
      gl->addWidget( m_pNormalLabel[i], i * 3 + 1, 0 );
      gl->addWidget( m_pNormal[i], i * 3 + 1, 1 );
      connect( m_pNormal[i], SIGNAL( dataChanged() ),
               SIGNAL( dataChanged() ) );

      m_pUVVector[i] = new PMVectorEdit( "u", "v", this );
      m_pUVVectorLabel[i] = new QLabel( ( "UV vector :" + QString::number( i+1 ) ), this );
      gl->addWidget( m_pUVVectorLabel[i], i * 3 + 2,  0 );
      gl->addWidget( m_pUVVector[i], i * 3 + 2, 1 );
      connect( m_pUVVector[i], SIGNAL( dataChanged() ),
               SIGNAL( dataChanged() ) );
   }
   hl = new QHBoxLayout();
   tl->addLayout( hl );
   m_pMirror = new QPushButton( tr( "Invert Normal Vectors" ), this );
   hl->addWidget( m_pMirror );
   hl->addStretch( 1 );
   connect( m_pMirror, SIGNAL( clicked() ), SLOT( slotInvertNormals() ) );
}

void PMTriangleEdit::displayObject( PMObject* o )
{
   if( o->isA( "Triangle" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = ( PMTriangle* ) o;
      bool smooth = m_pDisplayedObject->isSmoothTriangle();
      bool uvVector = m_pDisplayedObject->isUVEnabled();
      int i;

      for( i = 0; i < 3; i++ )
      {
         m_pPoint[i]->setVector( m_pDisplayedObject->point( i ) );
         m_pPoint[i]->setReadOnly( readOnly );

         m_pNormal[i]->setVector( m_pDisplayedObject->normal( i ) );
         m_pNormal[i]->setReadOnly( readOnly );

         m_pUVVector[i]->setVector( m_pDisplayedObject->uvVector( i ) );
         m_pUVVector[i]->setReadOnly( readOnly );

         m_pSmooth->setChecked( smooth );
         if( smooth )
         {
            m_pNormal[i]->show();
            m_pNormalLabel[i]->show();
            m_pMirror->show();
         }
         else
         {
            m_pNormal[i]->hide();
            m_pNormalLabel[i]->hide();
            m_pMirror->hide();
         }

         m_pUVEnabled->setChecked( uvVector );
         if( uvVector )
         {
            m_pUVVector[i]->show();
            m_pUVVectorLabel[i]->show();
         }
         else
         {
            m_pUVVector[i]->hide();
            m_pUVVectorLabel[i]->hide();
         }

         emit sizeChanged();
      }

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMTriangleEdit: Can't display object\n";
}

void PMTriangleEdit::slotSmoothChecked( bool on )
{
   int i;
   for( i = 0; i < 3; i++ )
   {
      if( on )
      {
         m_pNormal[i]->show();
         m_pNormalLabel[i]->show();
         m_pMirror->show();
      }
      else
      {
         m_pNormal[i]->hide();
         m_pNormalLabel[i]->hide();
         m_pMirror->hide();
      }
   }
   emit sizeChanged();
   emit dataChanged();
}

void PMTriangleEdit::slotUVVectorsChecked( bool on )
{
   int i;
   for( i = 0; i < 3; ++i )
   {
      if( on )
      {
         m_pUVVector[i]->show();
         m_pUVVectorLabel[i]->show();
      }
      else
      {
         m_pUVVector[i]->hide();
         m_pUVVectorLabel[i]->hide();
      }
   }
   emit sizeChanged();
   emit dataChanged();
}

void PMTriangleEdit::slotInvertNormals()
{
   int i;
   for( i = 0; i < 3; i++ )
      if( !m_pNormal[i]->isDataValid() )
         return;

   for( i = 0; i < 3; i++ )
      m_pNormal[i]->setVector( -( m_pNormal[i]->vector() ) );
}

void PMTriangleEdit::saveContents()
{
   int i;
   if( m_pDisplayedObject )
   {
      Base::saveContents();
      for( i = 0; i < 3; i++ )
         m_pDisplayedObject->setPoint( i, m_pPoint[i]->vector() );

      if( m_pSmooth->isChecked() )
      {
         m_pDisplayedObject->setSmoothTriangle( true );
         for( i = 0; i < 3; i++ )
            m_pDisplayedObject->setNormal( i, m_pNormal[i]->vector() );
      }
      else
         m_pDisplayedObject->setSmoothTriangle( false );

      if( m_pUVEnabled->isChecked() )
      {
         m_pDisplayedObject->enableUV( true );
         for( i = 0; i < 3; ++i )
            m_pDisplayedObject->setUVVector( i, m_pUVVector[i]->vector() );
      }
      else
         m_pDisplayedObject->enableUV( false );
   }
}

bool PMTriangleEdit::isDataValid()
{
   int i;
   for( i = 0; i < 3; i++ )
      if( !m_pPoint[i]->isDataValid() )
         return false;

   PMVector p0 = m_pPoint[0]->vector(),
            p1 = m_pPoint[1]->vector(),
            p2 = m_pPoint[2]->vector();

   if( p0.approxEqual( p1 ) || p1.approxEqual( p2 ) || p0.approxEqual( p2 ) )
   {
      QMessageBox::warning( this, tr( "Error" ), tr( "Please enter a valid triangle." ) );
      return false;
   }

   if( m_pSmooth->isChecked() )
   {
      for( i = 0; i < 3; i++ )
         if( !m_pNormal[i]->isDataValid() )
            return false;

      PMVector n0 = m_pNormal[0]->vector(),
               n1 = m_pNormal[1]->vector(),
               n2 = m_pNormal[2]->vector();
      PMVector tn = PMVector::cross( p1 - p0, p2 - p0 );
      double c0 = PMVector::dot( tn, n0 ),
             c1 = PMVector::dot( tn, n1 ),
             c2 = PMVector::dot( tn, n2 );
      if( ( ( c0 * c1 ) < 0 ) || ( ( c0 * c2 ) < 0 ) )
      {
         QMessageBox::warning( this, tr( "All normal vectors have to point"
                                         " to the same side of the triangle." ),
                             tr( "Error" ) );
         return false;
      }
   }

   if( m_pUVEnabled->isChecked() )
   {
      for( i = 0; i < 3; ++i )
         if( !m_pUVVector[i]->isDataValid() )
            return false;
   }

   return Base::isDataValid();
}

#include "pmtriangleedit.moc"
