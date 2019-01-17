/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2007 by Andreas Zehender
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

#include "pmglview.h"
#include "pmpart.h"
#include "pmrendermanager.h"
#include "pmcamera.h"
#include "pmscene.h"
#include "pmdatachangecommand.h"
#include "pmdebug.h"
#include "pmdefaults.h"

#include <math.h>
#include <QMenu>
#include <QPainter>
#include <QCursor>
#include <QColor>
#include <QtGlobal>
#include <QLayout>
#include <QComboBox>
#include <QtXml/QDomDocument>
//Added by qt3to4:
#include <QWheelEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QMouseEvent>

//#include <kxmlguifactory.h>
#include <QAction>
#include <QDialog>

const double c_sizeFactor = log( 2.0 ) / 100.0;
const int c_mouseChangeDelayPixel = 3;
const int c_mouseChangeDelayMs = 300;
//const int c_multipleSelectDelayPixel = 3;
const double c_defaultAutoScrollSpeed = 200; // pixels per second
const int c_minAutoScrollUpdateTime = 30; //ms

const double keyMoveSpeed = 40.0;
const double keyScaleFactor = 1.4;

bool PMGLView::s_bDirect = true;


PMGLView::PMGLView( PMPart* part, PMViewType t,
                    QWidget* parent, Qt::WindowFlags f )
      : PMViewBase( parent, f /*| Qt::MSWindowsOwnDC | Qt::WA_OpaquePaintEvent*/ )
{
   //eticresetAttribute( Qt::WA_PaintOutsidePaintEvent, true );
   m_pPart = part;
   m_type = t;
   m_bScaleMode = false;
   m_scaleIntX = 0.0;
   m_scaleIntY = 0.0;
   m_bTranslateMode = false;
   m_bGraphicalChangeMode = false;
   m_bMousePressed = false;
   m_bMidMousePressed = false;
   m_dTransX = 0.0;
   m_dTransY = 0.0;
   m_dScale = 30.0;
   m_bInverseValid = false;
   m_pActiveObject = part->activeObject();
   m_bMementoCreated = false;
   m_bSelectUnderMouse = false;
   m_bDeselectUnderMouse = false;
   m_bMultipleSelectionMode = false;
   m_bSelectionStarted = false;
   m_bAutoScroll = false;
   m_autoScrollSpeed = c_defaultAutoScrollSpeed;
   m_bAboutToUpdate = false;
   m_projectionUpToDate = false;
   m_contextClickPosition = PMVector( 0.0, 0.0 );
   m_pUnderMouse = nullptr;
   m_startTimer.setSingleShot( true );
   m_autoScrollTimer.setSingleShot( true );

   setCamera( m_pPart->firstCamera() );

   setMouseTracking( true );
   setFocusPolicy( Qt::WheelFocus );

   PMRenderManager* rm = PMRenderManager::theManager();
   rm->viewCreated();
   
   QFrame* frame = new QFrame( this) ;
   //frame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
   frame->setBackgroundRole( QPalette::Highlight );
   frame->setAutoFillBackground(true);
   QPalette pal = frame->palette();
   QColor c = PMRenderManager::theManager()->controlPointColor( 1 );
   c.setAlpha( 80 );
   pal.setColor( QPalette::Highlight, c );
   frame->setPalette( pal );
   frame->hide();
   m_pDragOverlay = frame;   

   setMinimumSize( 50, 50 );

   connect( part, SIGNAL( refresh() ), SLOT( slotRefresh() ) );
   connect( part, SIGNAL( clear() ), SLOT( slotClear() ) );

   connect( this, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
            part, SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   connect( part, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
                  SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   connect( part, SIGNAL( activeRenderModeChanged() ),
                  SLOT( slotActiveRenderModeChanged() ) );

   connect( &m_startTimer, SIGNAL( timeout() ),
                           SLOT( slotMouseChangeTimer() ) );

   connect( &m_autoScrollTimer, SIGNAL( timeout() ),
                                SLOT( slotAutoScroll() ) );

   connect( rm, SIGNAL( renderingStarted( PMGLView* ) ),
                SLOT( slotRenderingStarted( PMGLView* ) ) );

   connect( rm, SIGNAL( aboutToUpdate( PMGLView* ) ),
                SLOT( slotAboutToUpdate( PMGLView* ) ) );

   connect( rm, SIGNAL( renderingFinished( PMGLView* ) ),
                SLOT( slotRenderingFinished( PMGLView* ) ) );

   connect( rm, SIGNAL( renderingSettingsChanged() ),
                SLOT( slotRefresh() ) );

   connect( this, SIGNAL( controlPointMessage( const QString& ) ),
         m_pPart, SIGNAL( controlPointMessage( const QString& ) ) );

   if( m_pActiveObject->isA( "Scene") )
      enableTranslateMode( true );

   updateControlPoints();
}

PMGLView::~PMGLView()
{
   PMRenderManager* rm = PMRenderManager::theManager();
   rm->removeView( this );
   rm->viewDeleted();
   emit destroyed( this );
}

void PMGLView::setScale( double scale )
{
   if( m_dScale > 0 )
   {
      m_dScale = scale;
      invalidateProjection();
   }
   else
      qCritical() << "Scale <= 0 in PMGLView::setScale\n";
}

void PMGLView::setTranslationX( double d )
{
   m_dTransX = d;
   invalidateProjection();
}

void PMGLView::setTranslationY( double d )
{
   m_dTransY = d;
   invalidateProjection();
}

void PMGLView::resizeEvent( QResizeEvent* )
{
   invalidateProjection();
}

void PMGLView::paintEvent( QPaintEvent* )
{
    QPainter p( this );
    p.drawPixmap( QPoint( 0, 0 ), m_renderedScene );
}

void PMGLView::invalidateProjection( bool graphicalChange )
{
   m_viewTransformation = PMMatrix::identity();


   if( m_type != PMViewCamera )
   {
      m_viewTransformation = m_viewTransformation * PMMatrix::scale( m_dScale, m_dScale, m_dScale );
      m_viewTransformation = m_viewTransformation * PMMatrix::translation( m_dTransX, m_dTransY, 0 );

      switch( m_type )
      {
         case PMViewPosZ:
            m_normal = PMVector( 0.0, 0.0, 1.0 );
            break;
         case PMViewNegZ:
            m_viewTransformation = m_viewTransformation * PMMatrix::rotation( 0.0, M_PI, 0.0 );
            m_normal = PMVector( 0.0, 0.0, -1.0 );
            break;
         case PMViewNegY:
            m_viewTransformation = m_viewTransformation * PMMatrix::rotation( M_PI_2, 0.0, 0.0 );
            m_normal = PMVector( 0.0, -1.0, 0.0 );
            break;
         case PMViewPosY:
            m_normal = PMVector( 0.0, 1.0, 0.0 );
            m_viewTransformation = m_viewTransformation * PMMatrix::rotation( -M_PI_2, 0.0, 0.0 );
            break;
         case PMViewPosX:
            m_viewTransformation = m_viewTransformation * PMMatrix::rotation( 0.0, M_PI_2, 0.0 );
            m_normal = PMVector( 1.0, 0.0, 0.0 );
            break;
         case PMViewNegX:
            m_viewTransformation = m_viewTransformation * PMMatrix::rotation( 0.0, -M_PI_2, 0.0 );
            m_normal = PMVector( -1.0, 0.0, 0.0 );
            break;
         default:
            break;
      }

      m_viewTransformation = m_viewTransformation * PMMatrix::scale( 1.0, 1.0, -1.0 );

      if( m_controlPoints.count() > 0 )
         recalculateTransformations();

      recalculateControlPointPosition();
   }
   m_projectionUpToDate = false;
   rerender( graphicalChange );
}

void PMGLView::enableTranslateMode( bool yes )
{
   if( m_type != PMViewCamera )
   {
      m_bScaleMode = false;
      m_bTranslateMode = yes;
      setCursor( yes ? Qt::CrossCursor : Qt::ArrowCursor );
   }
}

void PMGLView::enableScaleMode( bool yes )
{
   if( m_type != PMViewCamera )
   {
      m_bScaleMode = yes;
      m_bTranslateMode = false;
      setCursor( yes ? Qt::CrossCursor : Qt::ArrowCursor );
   }
}

void PMGLView::mousePressEvent( QMouseEvent* e )
{
   if( m_bScaleMode || m_bTranslateMode )
   {
      if( ( e->button() & Qt::LeftButton ) && ( e->modifiers() == 0 ) )
      {
         m_bMousePressed = true;
         m_mousePos = e->pos();
         m_scaleIntX = screenToInternalX( e->x() );
         m_scaleIntY = screenToInternalY( e->y() );
      }
   }
   else if( m_type != PMViewCamera )
   {
      if( ( e->button() & Qt::LeftButton ) && m_bInverseValid
          && m_pActiveObject )
      {
         if( m_pUnderMouse )
         {
            // check the control point selection
            if( m_pActiveObject->multipleSelectControlPoints() )
            {
               if( m_pUnderMouse->selected() )
               {
                  if( e->modifiers() & Qt::ControlModifier )
                     m_bDeselectUnderMouse = true;
                  else
                     m_bSelectUnderMouse = true;
               }
               else
               {
                  if( e->modifiers() & Qt::ControlModifier )
                     selectControlPoint( m_pUnderMouse, !m_pUnderMouse->selected(), false );
                  else
                     selectControlPoint( m_pUnderMouse, true );
               }
            }
            else
               selectControlPoint( m_pUnderMouse, true );

            // start the graphical change
            if( !m_bGraphicalChangeMode )
            {
               m_bGraphicalChangeMode = true;
               m_bMementoCreated = false;
               m_changeStartPos = e->pos();
               m_changeStartTime = QTime::currentTime();
               m_currentMousePos = m_changeStartPos;
               m_startTimer.start( c_mouseChangeDelayMs );
            }
         }
         else
         {
            if( m_pActiveObject->multipleSelectControlPoints() )
            {
               // multiple selection mode
               // start only when the view is rendered
               if( !PMRenderManager::theManager()->containsTask( this ) )
               {
                  m_bMultipleSelectionMode = true;
                  m_bSelectionStarted = false;
                  m_selectionStart = e->pos();
                  m_selectionEnd = m_selectionStart;
               }
            }
            else
               selectControlPoint( nullptr, false );
         }
      }
   }


   if( !( m_bGraphicalChangeMode || m_bMousePressed ) )
   {
      if( ( e->button() == Qt::RightButton ) && ( e->modifiers() == 0 ) )
      {
         m_contextClickPosition = PMVector( screenToInternalX( e->x() ),
                                            screenToInternalY( e->y() ) );

         if( m_pUnderMouse )
         {
            // check the control point selection
            if( m_pActiveObject && m_pActiveObject->multipleSelectControlPoints() )
            {
               if( !m_pUnderMouse->selected() )
                  selectControlPoint( m_pUnderMouse, true );
            }
            else
               selectControlPoint( m_pUnderMouse, true );
         }

         contextMenu();
      }
   }

   if( e->button() == Qt::MidButton )
   {
      m_bMidMousePressed = true;
      m_mousePos = e->pos();
   }
}

void PMGLView::mouseReleaseEvent( QMouseEvent* e )
{
   m_bMousePressed = false;
   if( m_bGraphicalChangeMode )
   {
      m_startTimer.stop();

      if( m_bMementoCreated )
      {
         PMDataChangeCommand* cmd;
         cmd = new PMDataChangeCommand( m_pActiveObject->takeMemento() );
         m_pPart->executeCommand( cmd );

         checkUnderMouse( ( int ) screenToInternalX( e->x() ),
                          ( int ) screenToInternalY( e->y() ) );
      }
      else
      {
         if( m_pUnderMouse )
         {
            if( m_bSelectUnderMouse )
               selectControlPoint( m_pUnderMouse, true );
            else if( m_bDeselectUnderMouse )
               selectControlPoint( m_pUnderMouse, false, false );
         }
      }
      m_bGraphicalChangeMode = false;
   }
   else if( m_bMultipleSelectionMode )
   {
      if( m_bSelectionStarted )
      {
         int sx, sy, ex, ey, w, h;
         double isx, isy, iex, iey;
         QList<PMVector*>::iterator pit = m_controlPointsPosition.begin();
         PMControlPointList::iterator cit = m_controlPoints.begin();
         PMVector p;

         calculateSelectionBox( sx, sy, ex, ey, w, h );
         isx = screenToInternalX( sx );
         iex = screenToInternalX( ex );
         isy = screenToInternalY( ey );
         iey = screenToInternalY( sy );
          m_pDragOverlay->hide();

         while( pit != m_controlPointsPosition.end() && cit != m_controlPoints.end() )
         {
            p = **pit;

            if( ( isx <= p[0] ) && ( iex >= p[0] ) && ( isy <= p[1] ) && ( iey >= p[1] ) )
               selectControlPoint( *cit, true, false );
            else if( !( e->modifiers() & Qt::ControlModifier ) )
               selectControlPoint( *cit, false, false );

            ++cit;
            ++pit;
         }
      }
      else
         selectControlPoint( nullptr, false );

      m_bMultipleSelectionMode = false;
   }

   if( m_bAutoScroll )
   {
      m_bAutoScroll = false;
      m_autoScrollTimer.stop();
   }

   if( e->button() & Qt::MidButton )
      m_bMidMousePressed = false;

   m_bSelectUnderMouse = false;
   m_bDeselectUnderMouse = false;
}

void PMGLView::mouseMoveEvent( QMouseEvent* e )
{
   if( m_bMousePressed )
   {
      if( m_bScaleMode )
      {
         int d = e->x() - m_mousePos.x();
         if( d != 0 )
         {
            double s = exp( d * c_sizeFactor );
            double c = 1.0 / ( m_dScale * s ) - 1.0 / m_dScale;
            m_dTransX += m_scaleIntX * c;
            m_dTransY += m_scaleIntY * c;
            m_dScale *= s;
            invalidateProjection();
         }
      }
      else if( m_bTranslateMode )
      {
         m_dTransX += ( e->x() - m_mousePos.x() ) / m_dScale;
         m_dTransY -= ( e->y() - m_mousePos.y() ) / m_dScale;
         invalidateProjection();
      }
      m_mousePos = e->pos();
   }
   else if( m_bMidMousePressed )
   {
      m_dTransX += ( e->x() - m_mousePos.x() ) / m_dScale;
      m_dTransY -= ( e->y() - m_mousePos.y() ) / m_dScale;
      invalidateProjection();

      m_mousePos = e->pos();
   }
   else if( m_bGraphicalChangeMode )
   {
      m_currentMousePos = e->pos();
      if( !m_bMementoCreated )
      {
         QPoint movement = e->pos() - m_changeStartPos;
         if( ( m_changeStartTime.msecsTo( QTime::currentTime() ) > c_mouseChangeDelayMs )
            || ( movement.manhattanLength() > c_mouseChangeDelayPixel ) )
         {
            m_startTimer.stop();
            startChange( m_changeStartPos );
         }
      }

      if( m_bMementoCreated )
         graphicalChange( e->pos() );
   }
   else if( m_bMultipleSelectionMode )
   {
      if( !m_bSelectionStarted )
      {
         m_selectionEnd = e->pos();
         startSelection();
      }
      else
      {
         m_selectionEnd = e->pos();
         paintSelectionBox();
      }
   }
   else if( !( m_bScaleMode || m_bTranslateMode ) )
   {
      checkUnderMouse( ( int ) screenToInternalX( e->x() ),
                       ( int ) screenToInternalY( e->y() ) );
   }

   if( m_bMultipleSelectionMode || m_bGraphicalChangeMode )
   {
      bool as = m_bAutoScroll;

      if( e->x() < 0 )
         m_autoScrollDirectionX = 1;
      else if( e->x() >= width() )
         m_autoScrollDirectionX = -1;
      else
         m_autoScrollDirectionX = 0;

      if( e->y() < 0 )
         m_autoScrollDirectionY = 1;
      else if( e->y() >= height() )
         m_autoScrollDirectionY = -1;
      else
         m_autoScrollDirectionY = 0;

      if( ( m_autoScrollDirectionX != 0 ) || ( m_autoScrollDirectionY != 0 ) )
         m_bAutoScroll = true;
      else
         m_bAutoScroll = false;

      if( m_bAutoScroll && !as )
      {
         m_lastAutoScrollUpdate = QTime::currentTime();
         m_autoScrollTimer.start( c_minAutoScrollUpdateTime );
      }
      if( !m_bAutoScroll && as )
         m_autoScrollTimer.stop();
   }
}

void PMGLView::wheelEvent( QWheelEvent* e )//eticre resize ok
{
   if( m_type != PMViewCamera )
   {
      double s = exp( e->delta() / 4 * c_sizeFactor );
      double deltaX = screenToInternalX( e->x() );
      double deltaY = screenToInternalY( e->y() );
      double c = 1.0 / ( m_dScale * s ) - 1.0 / m_dScale;
      m_dTransX += deltaX * c;
      m_dTransY += deltaY * c;
      m_dScale *= s;
      invalidateProjection();
   }
}

void PMGLView::keyPressEvent( QKeyEvent* e )
{
   bool accept = true;

   if( e->modifiers() == 0 )
   {
      if( m_type != PMViewCamera )
      {
         if( m_dScale > 0 )
         {
            switch( e->key() )
            {
               case Qt::Key_Left:
                  m_dTransX -= keyMoveSpeed / m_dScale;
                  break;
               case Qt::Key_Right:
                  m_dTransX += keyMoveSpeed / m_dScale;
                  break;
               case Qt::Key_Up:
                  m_dTransY += keyMoveSpeed / m_dScale;
                  break;
               case Qt::Key_Down:
                  m_dTransY -= keyMoveSpeed / m_dScale;
                  break;
               default:
                  accept = false;
            }
         }
         else
            qCritical() << "scale <= 0 in PMGLView::keyPressEvent\n";
      }
   }
   else if( e->modifiers() == Qt::ControlModifier )
   {
      if( m_type != PMViewCamera )
      {
         switch( e->key() )
         {
            case Qt::Key_Left:
            case Qt::Key_Down:
               m_dScale /= keyScaleFactor;
               break;
            case Qt::Key_Right:
            case Qt::Key_Up:
               m_dScale *= keyScaleFactor;
               break;
            default:
               accept = false;
         }
      }
   }
   else
      accept = false;

   if( accept )
      invalidateProjection();
   else
      e->ignore();
}

void PMGLView::slotAutoScroll()
{
   if( m_bAutoScroll )
   {
      QTime now = QTime::currentTime();
      int msecs = m_lastAutoScrollUpdate.msecsTo( now );
      int pixels = ( int ) ( m_autoScrollSpeed * msecs / 1000 );

      if( pixels < 1 )
         pixels = 1;
      if( pixels > ( width() * 3 / 4 ) )
         pixels = width() * 3 / 4;
      if( pixels > ( height() * 3 / 4 ) )
         pixels = height() * 3 / 4;

      if( m_bGraphicalChangeMode && !m_bMementoCreated )
         startChange( m_changeStartPos );

      m_dTransX += pixels * m_autoScrollDirectionX / m_dScale;
      m_dTransY -= pixels * m_autoScrollDirectionY / m_dScale;
      invalidateProjection();

   if( m_bGraphicalChangeMode )
   {
      if( m_bMultipleSelectionMode )
      {
         m_selectionStart += QPoint( pixels * m_autoScrollDirectionX,
                                     pixels * m_autoScrollDirectionY );

         paintSelectionBox();
      }
    }
      if( m_bGraphicalChangeMode )
         graphicalChange( mapFromGlobal( QCursor::pos() ) );
      else
         rerender();

      m_lastAutoScrollUpdate = now;
   }
}

void PMGLView::slotMouseChangeTimer()
{
   if( !m_bMementoCreated )
   {
      if( m_currentMousePos != m_changeStartPos )
      {
         startChange( m_changeStartPos );
         graphicalChange( m_currentMousePos );
      }
   }
}

void PMGLView::startChange( const QPoint& mousePos )//eticre resize?
{
   m_pActiveObject->createMemento();
   m_bMementoCreated = true;

   PMVector p = mousePosition( m_pUnderMouse, mousePos.x(), mousePos.y() );
   p.transform( m_inversePointsTransformation );

   if( m_pActiveObject->multipleSelectControlPoints() )
   {
		PMControlPointList::iterator it;
      for( it = m_controlPoints.begin(); it != m_controlPoints.end(); ++it )
         if( ( *it )->selected() )
            ( *it )->startChange( p, m_normal );
   }
   else
      m_pUnderMouse->startChange( p, m_normal );
}

void PMGLView::graphicalChange( const QPoint& mousePos )
{
   PMVector p = mousePosition( m_pUnderMouse, mousePos.x(), mousePos.y() );
   p.transform( m_inversePointsTransformation );
   if( m_pActiveObject->multipleSelectControlPoints() )
   {
		PMControlPointList::iterator it;
      for( it = m_controlPoints.begin(); it != m_controlPoints.end(); ++it )
         if( ( *it )->selected() )
            ( *it )->change( p );
   }
   else
      m_pUnderMouse->change( p );

   PMObjectList changedObjects;
   m_pActiveObject->controlPointsChangedList( m_controlPoints, changedObjects );

   if( changedObjects.isEmpty() )
      emit objectChanged( m_pActiveObject, PMCGraphicalChange, this );
   else
   {
		PMObjectList::iterator it;
      for( it = changedObjects.begin(); it != changedObjects.end(); ++it )
         emit objectChanged( *it, PMCGraphicalChange, this );
   }
}

void PMGLView::checkUnderMouse( int x, int y )
{
   // is cursor over a control point?
	PMControlPointList::iterator pit;
	QList<PMVector*>::iterator vit;
    PMControlPoint* old = m_pUnderMouse;

   if( m_bInverseValid && ( m_type != PMViewCamera ) )
   {
      m_pUnderMouse = nullptr;

      vit = m_controlPointsPosition.begin();
      pit = m_controlPoints.begin();

      while( pit != m_controlPoints.end() )
      {
         if( ( *pit )->displayType() == PMControlPoint::CPCross )
         {
            if( !m_pUnderMouse )
               m_pUnderMouse = *pit;
         }
         else
         {
            if( ( fabs( x - (**vit)[0] ) < ( controlPointSize / 2.0 + 0.1 ) )
                && ( fabs( y - (**vit)[1] ) < ( controlPointSize / 2.0 + 0.1 ) ) )
            {
               if( m_pUnderMouse )
               {
                  if( ( *pit )->selected() && !m_pUnderMouse->selected() )
                     m_pUnderMouse = *pit;
               }
               else
                  m_pUnderMouse = *pit;
            }
         }

         ++pit;
			++vit;
      }
   }
   else
      m_pUnderMouse = nullptr;

   setCursor( m_pUnderMouse ? Qt::CrossCursor : Qt::ArrowCursor );
   if( m_pUnderMouse != old )
   {
      if( m_pUnderMouse )
         emit controlPointMessage( m_pUnderMouse->description() );
      else
         emit controlPointMessage( "" );
   }
}

void PMGLView::updateControlPoints()
{
   m_controlPoints.clear();
   m_controlPoints = m_pPart->activeControlPoints();

   if( ( m_controlPoints.count() > 0 ) && m_pActiveObject )
   {
      m_objectsTransformation = m_pActiveObject->transformedWith();
      recalculateTransformations();
   }

   if( m_bGraphicalChangeMode )
      m_bGraphicalChangeMode = false;

   recalculateControlPointPosition();
}

void PMGLView::recalculateControlPointPosition()
{
   PMControlPointList::iterator it;

   foreach(PMVector* dv, m_controlPointsPosition) delete dv;

   m_controlPointsPosition.clear();

   PMVector* v;

   for( it = m_controlPoints.begin(); it != m_controlPoints.end(); ++it )
   {
      v = new PMVector( m_controlPointsTransformation * ( *it )->position() );
      m_controlPointsPosition.append( v );
   }
   if( !m_bGraphicalChangeMode )
   {
      m_pUnderMouse = nullptr;
      emit controlPointMessage( "" );
   }
}

PMVector PMGLView::mousePosition( PMControlPoint* cp, int x, int y )
{
   PMVector result;
   int index;
   PMVector* p;

   result[0] = screenToInternalX( x );
   result[1] = screenToInternalY( y );
   if( cp )
   {
      index = m_controlPoints.indexOf( cp );
      if( index >= 0 )
      {
         p = m_controlPointsPosition.at( ( uint ) index );
         if( p )
            result[2] = p->z();
      }
   }
   return result;
}

void PMGLView::recalculateTransformations()
{
   int r, c;

   m_controlPointsTransformation = m_viewTransformation * m_objectsTransformation;

   if( m_controlPointsTransformation.canBuildInverse() )
   {
      m_inversePointsTransformation = m_controlPointsTransformation.inverse();

      for( c = 0; c < 4; c++ )
         for( r = 0; r < 4; r++ )
            if( fabs( m_inversePointsTransformation[c][r] ) < 1e-8 )
               m_inversePointsTransformation[c][r] = 0.0;

      m_bInverseValid = true;
   }
   else
      m_bInverseValid = false;
}

void PMGLView::setType( PMViewType t )
{
   if( m_type != t ) { m_viewTransformation = PMMatrix::identity(); }

   m_type = t;
   invalidateProjection();

   emit viewTypeChanged( viewTypeAsString( t ) );
}

void PMGLView::setCamera( PMCamera* c )
{
   m_pCamera = c;
   invalidateProjection();
}

void PMGLView::slotRefresh()
{
   if( m_type == PMViewCamera )
      if( !m_pCamera )
         setCamera( m_pPart->firstCamera() );

   rerender();
}

void PMGLView::slotClear()
{
   foreach(PMVector* dv, m_controlPointsPosition) delete dv;
   m_controlPointsPosition.clear();
   m_controlPoints.clear();
   m_pUnderMouse = nullptr;
   m_pCamera = nullptr;
   m_pActiveObject = nullptr;

   slotStopRendering();
}

void PMGLView::slotActiveRenderModeChanged()
{
   if( ( m_type == PMViewCamera ) && m_pCamera )
      invalidateProjection();
}

void PMGLView::slotStopRendering()
{
   PMRenderManager* rm = PMRenderManager::theManager();
   rm->removeView( this );
}

void PMGLView::slotObjectChanged( PMObject* obj, const int mode,
                                  QObject* sender )
{
   bool redraw = false;

   if( mode & PMCNewSelection )
   {
      if( obj )
      {
          if( obj->isA("Scene") )
             {
                 if( !m_bTranslateMode )
                    enableTranslateMode( true );
             }
             else if( m_bTranslateMode )
             {
                 enableTranslateMode( false );
             }
         if( obj != m_pActiveObject )
         {
            m_pActiveObject = obj;
            redraw = true;
         }
      }
      else
      {
         m_pActiveObject = nullptr;
         redraw = true;
      }
   }
   if( mode & ( PMCSelected | PMCDeselected ) )
   {
      m_pActiveObject = nullptr;
      redraw = true;
   }
   if( mode & ( PMCViewStructure | PMCGraphicalChange ) )
   {
      if( m_type == PMGLView::PMViewCamera && obj )
      {
         if( obj->type() == "Camera" )
            if( m_pCamera == static_cast<PMCamera*>(obj) )
               invalidateProjection();

         if( obj->parent() )
            if( obj->parent()->type() == "Camera" )
               if( m_pCamera == static_cast<PMCamera*>(obj)->parent() )
                  if( obj->hasTransformationMatrix() )
                     invalidateProjection();
      }

      redraw = true;
   }
   if( mode & PMCNewControlPoints )
   {
      updateControlPoints();
      m_pActiveObject = m_pPart->activeObject();
      redraw = true;
   }
   if( mode & PMCControlPointSelection )
   {
      redraw = true;
   }
   if( mode & PMCDescription )
   {
      if( m_type == PMGLView::PMViewCamera && obj && obj == m_pCamera )
         redraw = true;
   }
   if( mode & PMCAdd )
   {
      if( m_type == PMGLView::PMViewCamera && obj )
      {
         if( obj->type() == "Camera" )
            if( !m_pCamera )
               setCamera( static_cast<PMCamera*>(obj) );
         if( obj->parent() )
            if( obj->parent()->type() == "Camera" )
               if( m_pCamera == static_cast<PMCamera*>(obj)->parent() )
                  if( obj->hasTransformationMatrix() )
                     invalidateProjection();
      }
      redraw = true;
   }

   if( (mode & PMCRemove) && obj )
   {
      if( obj->type() == "Camera" )
         if( m_pCamera == static_cast<PMCamera*>(obj) )
            setCamera( nullptr );

      if( m_type == PMGLView::PMViewCamera )
         if( obj->parent() )
            if( obj->parent()->type() == "Camera" )
               if( m_pCamera == static_cast<PMCamera*>(obj)->parent() )
                  if( obj->hasTransformationMatrix() )
                     invalidateProjection();

      redraw = true;
   }

   if( mode & PMCChildren )
      redraw = true;

   if( redraw )
      rerender( sender == this );
}

void PMGLView::rerender( bool graphicalChange )
{
	PMObject* obj = m_pActiveObject;

	if( obj )
		obj = topLevelRenderingObject( obj );
	else
	{
        const PMObjectList& selected = m_pPart->selectedObjects();
        PMObjectList::const_iterator it = selected.begin();
        if( it != selected.end() )
			obj = topLevelRenderingObject( *it );

        if( obj && ( obj->type() != "Scene" ) )
            for( ++it; it != selected.end() && obj; ++it )
				if( topLevelRenderingObject( *it ) != obj )
                    obj = nullptr;
	}

    if( !obj )
    {
        obj = m_pPart->scene();
    }
	if( obj )
	{
        double aspectRatio = 1.0;
        const PMRenderMode* mode = m_pPart->scene()->activeRenderMode();
		if( mode )
            if( mode->height() != 0 )
                aspectRatio = ( double ) mode->width() / ( double ) mode->height();

        PMRenderManager* rm = PMRenderManager::theManager();
        rm->addView( this, m_pActiveObject, obj,
						 &m_controlPoints, aspectRatio,
                         m_pPart->scene()->visibilityLevel(), graphicalChange );
	}
}

PMObject* PMGLView::topLevelRenderingObject( PMObject* o ) const
{
   PMObject* obj = o;
   bool stop = false;

   if( obj )
   {
      do
      {
         if( !obj )
            stop = true;
         else if( obj->isA( "Scene" ) || obj->isA( "Declare" ) )
            stop = true;
         else
            obj = obj->parent();
      }
      while( !stop );
   }
   else
      obj = m_pPart->scene();

   return obj;
}

void PMGLView::selectControlPoint( PMControlPoint* cp, bool select, bool deselectOthers )
{
   bool selectionChanged = false;

   if( cp )
   {
      if( deselectOthers )
      {
         PMControlPointList::iterator it;
         for( it = m_controlPoints.begin(); it != m_controlPoints.end(); ++it )
         {
            bool s;
            if( *it == cp )
               s = select;
            else
               s = false;

            if( s != ( *it )->selected() )
            {
               selectionChanged = true;
               ( *it )->setSelected( s );
            }
         }
      }
      else
      {
         if( select != cp->selected() )
         {
            selectionChanged = true;
            cp->setSelected( select );
         }
      }
   }
   else
   {
		PMControlPointList::iterator it;
        for( it = m_controlPoints.begin(); it != m_controlPoints.end(); ++it )
      {
         if( select != ( *it )->selected() )
         {
            selectionChanged = true;
            ( *it )->setSelected( select );
         }
      }
   }

   if( selectionChanged )
      emit objectChanged( m_pActiveObject, PMCControlPointSelection, this );
}

void PMGLView::startSelection()
{
   if( !m_bSelectionStarted )
   {
      paintSelectionBox();
      m_bSelectionStarted = true;
   }
}


void PMGLView::paintSelectionBox()
{
   if( !m_bAboutToUpdate )
   {
      int sx, sy, ex, ey, w, h;
      calculateSelectionBox( sx, sy, ex, ey, w, h );
      m_pDragOverlay->setGeometry(QRect(QPoint(sx, sy), QPoint(ex, ey)));
      m_pDragOverlay->raise();
      m_pDragOverlay->show();
      m_pDragOverlay->update();
   }
}

void PMGLView::calculateSelectionBox( int& sx, int& sy, int& ex, int& ey,
                                      int& w, int& h )
{
   if( m_selectionStart.x() < m_selectionEnd.x() )
   {
      sx = m_selectionStart.x();
      ex = m_selectionEnd.x();
   }
   else
   {
      ex = m_selectionStart.x();
      sx = m_selectionEnd.x();
   }

   if( m_selectionStart.y() < m_selectionEnd.y() )
   {
      sy = m_selectionStart.y();
      ey = m_selectionEnd.y();
   }
   else
   {
      ey = m_selectionStart.y();
      sy = m_selectionEnd.y();
   }

   w = ex - sx + 1;
   h = ey - sy + 1;
}

double PMGLView::screenToInternalX( int x ) const
{
   return rint( x - width() / 2.0 + 0.1 );
}

double PMGLView::screenToInternalY( int y ) const
{
   return rint( height() / 2.0 - y - 0.1 );
}

void PMGLView::slotRenderingStarted( PMGLView* )
{
}

void PMGLView::slotAboutToUpdate( PMGLView* view )
{
   if( view == this )
      m_bAboutToUpdate = true;
}

void PMGLView::slotRenderingFinished( PMGLView* view )
{
   if( view == this )
   {
      m_bAboutToUpdate = false;
      if( m_bMultipleSelectionMode )
      {
         paintSelectionBox();
      }

      if( m_bAutoScroll )
      {
         QTime now = QTime::currentTime();
         int msecs = m_lastAutoScrollUpdate.msecsTo( now );

         if( msecs < c_minAutoScrollUpdateTime )
            m_autoScrollTimer.start( c_minAutoScrollUpdateTime - msecs );
         else
            m_autoScrollTimer.start( 0 );
      }
   }
}

QString PMGLView::viewTypeAsString( PMViewType t )
{
   QString str;

   switch( t )
   {
      case PMViewPosX:
         str = tr( "Left" );
         break;
      case PMViewNegX:
         str = tr( "Right" );
         break;
      case PMViewPosY:
         str = tr( "Bottom" );
         break;
      case PMViewNegY:
         str = tr( "Top" );
         break;
      case PMViewPosZ:
         str = tr( "Front" );
         break;
      case PMViewNegZ:
         str = tr( "Back" );
         break;
      case PMViewCamera:
         str = tr( "Camera" );
         break;
   }
   return str;
}

void PMGLView::saveConfig()
{
}

void PMGLView::restoreConfig()
{
}

void PMGLView::contextMenu()
{
   QMenu* m = new QMenu();
   m->addAction( ( "Left View" ), this, SLOT( slotSetTypePosX() ) );
   m->addAction( ( "Right View" ), this, SLOT( slotSetTypeNegX() ) );
   m->addAction( ( "Top View" ), this, SLOT( slotSetTypeNegY() ) );
   m->addAction( ( "Bottom View" ), this, SLOT( slotSetTypePosY() ) );
   m->addAction( ( "Front View" ), this, SLOT( slotSetTypePosZ() ) );
   m->addAction( ( "Back View" ), this, SLOT( slotSetTypeNegZ() ) );
   int menuid = 0;

   QMenu* cm = m->addMenu( QIcon( ":/smallicon/icons/povicons/small/pmcamera" ), ( "Camera" ) );
   const QList< PMCamera* > cameras = m_pPart->cameras();
   QList< PMCamera* >::ConstIterator it = cameras.begin();
   QString name;
   if( it == cameras.end() )
      cm->addAction( ( "No Cameras" ) );
   else
   {
      menuid = 0;
      for( ; it != cameras.end(); ++it )
      {
         name = ( *it )->name();
         if( name.isEmpty() )
            name = ( "(unnamed)" );
         QAction* ac = cm->addAction( name );
	 ac->setData( menuid );
         menuid++;
      }
   }
   connect( cm, SIGNAL( triggered( QAction* ) ), SLOT( slotCameraView( QAction* ) ) );

   m->addSeparator();

   m->addAction( ( "Snap to Grid" ), this, SLOT( slotSnapToGrid() ) );
	foreach(PMObjectAction* da, m_objectActions) delete da;
   m_objectActions.clear();
   if( m_pActiveObject )
   {
      m_pActiveObject->addObjectActions( m_controlPoints, m_objectActions );
      if( !m_objectActions.isEmpty() )
      {
	 menuid = 0;
	 foreach( PMObjectAction* ait, m_objectActions )
         {
            QAction* act = m->addAction( ait->description() );
            act->setData( menuid + 1 );
            ait->setMenuID( menuid );
            menuid++;
         }
      }
   }
   connect( m, SIGNAL( triggered( QAction* ) ), SLOT( slotObjectAction( QAction* ) ) );

   m->addSeparator();

   QMenu* menu = m->addMenu( ( "Control Points" ) );
   if( m_controlPoints.count() == 0 )
      menu->addAction( ( "No Control Points" ) );
   else
   {
      menuid = 0;
      foreach( PMControlPoint* point, m_controlPoints )
      {
        QAction* ac = menu->addAction( point->description() );
        ac->setData( menuid );
        menuid++;
      }
   }
   connect( menu, SIGNAL( triggered( QAction* ) ), SLOT( slotControlPoint( QAction* ) ) );

   m->exec( QCursor::pos() );
   delete m;
}

void PMGLView::slotCameraView( QAction* ac )
{
   int id = ac->data().toInt();
   int i;
   const QList< PMCamera* > cameras = m_pPart->cameras();
   QList< PMCamera* >::ConstIterator it = cameras.begin();

   for( i = 0; i < id && it != cameras.end(); i++ )
      ++it;
   if( it != cameras.end() )
   {
      setCamera( *it );
      setType( PMGLView::PMViewCamera );
   }
}

void PMGLView::slotObjectAction( QAction* act )
{
   int id = act->data().toInt() - 1;
   if( id < 0 ) return;

   QList<PMObjectAction*>::iterator it;
   PMObjectAction* oa = nullptr;

   for( it = m_objectActions.begin(); it != m_objectActions.end() && !oa; ++it )
      if( ( *it )->menuID() == id )
         oa = *it;

   if( oa && m_pActiveObject )
   {
      // otherwise no object action was selected in the context menu

      m_pActiveObject->createMemento();
      m_pActiveObject->objectActionCalled( oa, m_controlPoints,
                                           m_controlPointsPosition,
                                           m_contextClickPosition );
      PMDataChangeCommand* cmd;
      cmd = new PMDataChangeCommand( m_pActiveObject->takeMemento() );
      cmd->setText( oa->description() );
      m_pPart->executeCommand( cmd );
   }
}

void PMGLView::slotControlPoint( QAction* act )
{
   int id = act->data().toInt();
   PMControlPoint* p = m_controlPoints.at( id );
   if( p )
   {
		PMControlPointList::iterator cit;
        for( cit = m_controlPoints.begin(); cit != m_controlPoints.end(); ++cit )
         ( *cit )->setSelected( p == *cit );
      emit objectChanged( m_pActiveObject, PMCControlPointSelection, this );
   }
}

void PMGLView::slotSnapToGrid()
{
   if( m_pActiveObject )
   {
      if( !m_pActiveObject->mementoCreated() )
         m_pActiveObject->createMemento();

		PMControlPointList::iterator it;
        for( it = m_controlPoints.begin(); it != m_controlPoints.end(); ++it )
         if( ( *it )->selected() )
            ( *it )->snapToGrid();

      m_pActiveObject->controlPointsChanged( m_controlPoints );

      PMDataChangeCommand* cmd;
      cmd = new PMDataChangeCommand( m_pActiveObject->takeMemento() );
      cmd->setText( tr( "Snap to Grid" ) );
      m_pPart->executeCommand( cmd );
   }
}

QString PMGLView::description() const
{
   return viewTypeAsString( m_type );
}

void PMGLView::restoreViewConfig( PMViewOptions* vo )
{
   if( vo && vo->viewType() == "glview" )
   {
      PMGLViewOptions* o = ( PMGLViewOptions* ) vo;
      m_type = o->glViewType();
   }
}

void PMGLView::saveViewConfig( PMViewOptions* vo ) const
{
   if( vo && vo->viewType() == "glview" )
   {
      PMGLViewOptions* o = ( PMGLViewOptions* ) vo;
      o->setGLViewType( m_type );
   }
}

void PMGLViewOptions::loadData( QDomElement& e )
{
   QString s = e.attribute( "type", "Camera" );
   if( s == "Camera" ) m_glViewType = PMGLView::PMViewCamera;
   else if( s == "X" ) m_glViewType = PMGLView::PMViewPosX;
   else if( s == "Y" ) m_glViewType = PMGLView::PMViewPosY;
   else if( s == "Z" ) m_glViewType = PMGLView::PMViewPosZ;
   else if( s == "NegX" ) m_glViewType = PMGLView::PMViewNegX;
   else if( s == "NegY" ) m_glViewType = PMGLView::PMViewNegY;
   else if( s == "NegZ" ) m_glViewType = PMGLView::PMViewNegZ;
}

void PMGLViewOptions::saveData( QDomElement& e )
{
   switch( m_glViewType )
   {
      case PMGLView::PMViewCamera:
         e.setAttribute( "type", "Camera" );
         break;
      case PMGLView::PMViewPosX:
         e.setAttribute( "type", "X" );
         break;
      case PMGLView::PMViewPosY:
         e.setAttribute( "type", "Y" );
         break;
      case PMGLView::PMViewPosZ:
         e.setAttribute( "type", "Z" );
         break;
      case PMGLView::PMViewNegX:
         e.setAttribute( "type", "NegX" );
         break;
      case PMGLView::PMViewNegY:
         e.setAttribute( "type", "NegY" );
         break;
      case PMGLView::PMViewNegZ:
         e.setAttribute( "type", "NegZ" );
         break;
      default:
         qCritical() << ( "Unknown GL view type." )
                           << endl;
         break;
   }
}

QString PMGLViewFactory::description() const
{
   return ( "3D View" );
}

QString PMGLViewFactory::description( PMViewOptions* vo ) const
{
   if( vo && vo->viewType() == "glview" )
   {
      PMGLViewOptions* o = ( PMGLViewOptions* ) vo;
      return ( "3D View (%1)" +
         PMGLView::viewTypeAsString( o->glViewType() ) );
   }
   return description();
}

PMViewOptionsWidget* PMGLViewFactory::newOptionsWidget( QWidget* parent,
                                                        PMViewOptions* o )
{
   return new PMGLViewOptionsWidget( parent, o );
}

PMViewOptions* PMGLViewFactory::newOptionsInstance() const
{
   PMGLViewOptions* o = new PMGLViewOptions();
   return o;
}

PMGLViewOptionsWidget::PMGLViewOptionsWidget( QWidget* parent,
                                              PMViewOptions* o )
      : PMViewOptionsWidget( parent )
{
   m_pOptions = ( PMGLViewOptions* ) o;

   QHBoxLayout* hl = new QHBoxLayout( this );
//TODO PORT QT5    hl->setSpacing( QDialog::spacingHint() );
   hl->setMargin( 0 );
   QLabel* l = new QLabel( tr( "3D view type:" ), this );
   hl->addWidget( l );

   m_pGLViewType = new QComboBox( this );
   m_pGLViewType->addItem( "Top" );
   m_pGLViewType->addItem( "Bottom" );
   m_pGLViewType->addItem( "Left" );
   m_pGLViewType->addItem( "Right" );
   m_pGLViewType->addItem( "Front" );
   m_pGLViewType->addItem( "Back" );
   m_pGLViewType->addItem( "Camera" );

   switch( m_pOptions->glViewType() )
   {
      case PMGLView::PMViewNegY:
         m_pGLViewType->setCurrentIndex( 0 );
         break;
      case PMGLView::PMViewPosY:
         m_pGLViewType->setCurrentIndex( 1 );
         break;
      case PMGLView::PMViewPosX:
         m_pGLViewType->setCurrentIndex( 2 );
         break;
      case PMGLView::PMViewNegX:
         m_pGLViewType->setCurrentIndex( 3 );
         break;
      case PMGLView::PMViewPosZ:
         m_pGLViewType->setCurrentIndex( 4 );
         break;
      case PMGLView::PMViewNegZ:
         m_pGLViewType->setCurrentIndex( 5 );
         break;
      case PMGLView::PMViewCamera:
         m_pGLViewType->setCurrentIndex( 6 );
         break;
   }

   connect( m_pGLViewType, SIGNAL( activated( int ) ),
                           SLOT( slotGLViewTypeChanged( int ) ) );
   hl->addWidget( m_pGLViewType );
}

void PMGLViewOptionsWidget::slotGLViewTypeChanged( int index )
{
   switch( index )
   {
      case 0:
         m_pOptions->setGLViewType( PMGLView::PMViewNegY );
         break;
      case 1:
         m_pOptions->setGLViewType( PMGLView::PMViewPosY );
         break;
      case 2:
         m_pOptions->setGLViewType( PMGLView::PMViewPosX );
         break;
      case 3:
         m_pOptions->setGLViewType( PMGLView::PMViewNegX );
         break;
      case 4:
         m_pOptions->setGLViewType( PMGLView::PMViewPosZ );
         break;
      case 5:
         m_pOptions->setGLViewType( PMGLView::PMViewNegZ );
         break;
      case 6:
         m_pOptions->setGLViewType( PMGLView::PMViewCamera );
         break;
   }
   emit viewTypeDescriptionChanged();
}

#include "pmglview.moc"
