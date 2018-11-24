/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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

// conflicting types for INT32 in qt and glx
#ifndef QT_CLEAN_NAMESPACE
#define QT_CLEAN_NAMESPACE
#endif

#include "pmrendermanager.h"
#include "pmviewstructure.h"
#include "pmobject.h"
#include "pmdeclare.h"
#include "pmcamera.h"
#include "pmquickcolor.h"
#include "pmdefaults.h"
#include "pmgraphicalobject.h"
#include "pmmath.h"

#include <QApplication>
#include <QBitmap>
#include <QPainter>
//Added by qt3to4:
#include <QTimerEvent>
#include <QSettings>
#include <time.h>
#include <stdio.h>
#include <math.h>


#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h> // Only needed for gluPerspective
#include <QtX11Extras/QX11Info>
#endif
#include <QAbstractEventDispatcher>
#include <QSurfaceFormat>
#include <QResizeEvent>
#include "pmglview.h"

const GLdouble dA = 0.75;
const GLdouble dB = 0.15;

const GLdouble viewVolumeZ = 1e5;

// Size has to be controlPointSize (see pmglview.h)
const GLubyte PointBitmap[7] = { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE };
const GLubyte CrossBitmap[7] = { 0x10, 0x10, 0x10, 0xFE, 0x10, 0x10, 0x10 };

const int maxEventTime = 0;
const int maxSubdivisions = 32;
const double subdivisionDistance = 0.05;



PMRenderManager* PMRenderManager::s_pManager = nullptr;

PMRenderManager* PMRenderManager::theManager()
{
   if( !s_pManager )
   {
      s_pManager = new PMRenderManager;
   }

   return s_pManager;
}

PMRenderManager::PMRenderManager( QWidget *parent )
      : QOpenGLWidget( parent )
{
   int i;

   m_bStartTask = false;
   m_bStopTask = false;
   m_bTaskIsRunning = false;

   m_graphicalObjectColor[0] = c_defaultGraphicalObjectColor0;
   m_graphicalObjectColor[1] = c_defaultGraphicalObjectColor1;
   m_textureColor[0] = c_defaultTextureColor0;
   m_textureColor[1] = c_defaultTextureColor1;
   m_axesColor[0] = c_defaultAxesColorX;
   m_axesColor[1] = c_defaultAxesColorY;
   m_axesColor[2] = c_defaultAxesColorZ;
   m_controlPointColor[0] = c_defaultControlPointColor0;
   m_controlPointColor[1] = c_defaultControlPointColor1;
   m_backgroundColor = c_defaultBackgroundColor;
   m_fieldOfViewColor = c_defaultFieldOfViewColor;
   m_highDetailCameraView = c_defaultHighDetailCameraView;
   m_nMaxRenderedLines = 3000;//eticre complex object fail GL rendering (1000)
   m_gridDistance = c_defaultGridDistance;
   m_gridColor = c_defaultGridColor;
   m_axesViewStructureCreated = false;
   m_currentVisibility = 0;
   m_nViews = 0;

   m_subdivisionViewStructure = PMViewStructure( maxSubdivisions + 1,
                                                 maxSubdivisions );
   PMLineArray& lines = m_subdivisionViewStructure.lines();
   for( i = 0; i < maxSubdivisions; i++ )
      lines[i] = PMLine( i, i + 1 );

   m_pResize = new QResizeEvent( QSize( 1, 1 ), QSize( 1, 1 ) );
   this->resizeEvent( m_pResize );


    QSurfaceFormat format;
    format.setProfile( QSurfaceFormat::CoreProfile );
    this->setFormat( format );
}

PMRenderManager::~PMRenderManager()
{
   s_pManager = nullptr;
    foreach( PMRenderTask* t, m_renderTasks ) delete t;
}

void PMRenderManager::addView( PMGLView* view, PMObject* active, PMObject* top,
                               PMControlPointList* controlPoints,
                               double aspectRatio, int visibilityLevel,
                               bool graphicalChange )
{
   QList<PMRenderTask*>::iterator it;
   PMRenderTask* task = nullptr;
   bool restart = false;
   bool first = true;

   for( it = m_renderTasks.begin(); it != m_renderTasks.end() && !task; ++it )
   {
      if( (*it)->view() == view )
         task = *it;
      else
         first = false;
   }

   if( task )
   {
      if( first )
         restart = true;
      else if( graphicalChange )
      {
         m_renderTasks.removeAll( task );
         m_renderTasks.prepend( task );
         restart = true;
      }
      task->setActiveObject( active );
      task->setTopLevelObject( top );
      task->setControlPoints( controlPoints );
      task->setAspectRatio( aspectRatio );
      task->setVisibilityLevel( visibilityLevel );
   }
   else
   {
      task = new PMRenderTask( view, active, top, controlPoints, aspectRatio,
                               visibilityLevel );
      if( graphicalChange )
      {
         m_renderTasks.prepend( task );
         restart = true;
      }
      else
      {
         m_renderTasks.append( task );
         if( m_renderTasks.count() == 1 )
            restart = true;
      }
   }
   if( restart )
      restartRendering();
}

void PMRenderManager::removeView( PMGLView* view )
{
	QList<PMRenderTask*>::iterator it;
   PMRenderTask* task = nullptr;
   bool restart = false;

   for( it = m_renderTasks.begin(); it != m_renderTasks.end() && !task; ++it )
      if( (*it)->view() == view )
         task = (*it);

   if( task )
   {
      if( task == m_renderTasks.first() )
      {
         restart = true;
         if( m_bTaskIsRunning )
            emit renderingFinished( task->view() );
      }
      m_renderTasks.removeAll( task );
   }

   if( restart )
      restartRendering();
}

bool PMRenderManager::containsTask( PMGLView* view ) const
{
	QList<PMRenderTask*>::const_iterator it;
   bool contains = false;

   for( it = m_renderTasks.begin(); it != m_renderTasks.end() && !contains; ++it )
      if( (*it)->view() == view )
         contains = true;
   return contains;
}

QColor PMRenderManager::controlPointColor( int i ) const
{
   if( ( i >= 0 ) && ( i <= 1 ) )
      return m_controlPointColor[i];
   return QColor( 0, 0, 0 );
}

void PMRenderManager::setControlPointColor( int i, const QColor& c )
{
   if( ( i >= 0 ) && ( i <= 1 ) )
      m_controlPointColor[i] = c;
}

QColor PMRenderManager::graphicalObjectColor( int i ) const
{
   if( ( i >= 0 ) && ( i <= 1 ) )
      return m_graphicalObjectColor[i];
   return QColor( 0, 0, 0 );
}

void PMRenderManager::setGraphicalObjectColor( int i, const QColor& c )
{
   if( ( i >= 0 ) && ( i <= 1 ) )
      m_graphicalObjectColor[i] = c;
}

QColor PMRenderManager::axesColor( int i ) const
{
   if( ( i >= 0 ) && ( i <= 2 ) )
      return m_axesColor[i];
   return QColor( 0, 0, 0 );
}

void PMRenderManager::setAxesColor( int i, const QColor& c )
{
   if( ( i >= 0 ) && ( i <= 2 ) )
      m_axesColor[i] = c;
}

void PMRenderManager::setGridDistance( int d )
{
   if( d >= 20 )
      m_gridDistance = d;
}

void PMRenderManager::restartRendering()
{
   if( !m_bTaskIsRunning && !m_bStartTask )
      startTimer( 0 );

   m_bStartTask = true;
   m_bStopTask = false;
}

void PMRenderManager::slotStopRendering()
{
   m_bStopTask = true;
   m_bStartTask = false;

   if( m_bTaskIsRunning )
      if( m_pCurrentTask )
         emit renderingFinished( m_pCurrentTask->view() );
	foreach(PMRenderTask* t, m_renderTasks) delete t;
   m_renderTasks.clear();
}

void PMRenderManager::timerEvent( QTimerEvent* )
{
   QAbstractEventDispatcher::instance()->unregisterTimers( this );
   renderTask();
}

void PMRenderManager::renderTask()
{
   //emit renderingStarted();
   int r, g, b;
   bool disableView = false;

   while( m_bStartTask && !m_bStopTask )
   {
      m_bStartTask = false;


      // render the views sequential
      while( !m_renderTasks.isEmpty() && !m_bStopTask && !m_bStartTask )
      {
         // reset the member variables for rendering
         m_pCurrentTask = m_renderTasks.first();
         m_pCurrentGlView = m_pCurrentTask->view();
         emit renderingStarted( m_pCurrentGlView );
         this->setGeometry( m_pCurrentGlView->geometry() );
         *m_pResize = QResizeEvent( m_pCurrentGlView->size(), this->size() );
         this->resizeEvent( m_pResize );
         m_bTaskIsRunning = true;
         this->makeCurrent();

         m_renderedLines = 0;
         m_selected = false;
         m_pDeselectObject = nullptr;
         m_matrixStack.clear();
         m_quickColorObjects.clear();
         m_quickColors.clear();
         m_currentColor = m_graphicalObjectColor[0];
         m_specialCameraMode = false;
         m_currentVisibility = 0;
         m_visibilityStack.clear();

         if( m_bStopTask || m_bStartTask ) break;

         m_backgroundColor.getRgb( &r, &g, &b );
         glClearColor( r/255, g/255, b/255, 1.0 );
         glPointSize( controlPointSize );
         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  );
         glEnable( GL_DEPTH_TEST );
         glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

         //glViewport( 0, 0, ( GLint ) m_pCurrentGlView->width(),
           //          ( GLint ) m_pCurrentGlView->height() );

         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

         disableView = false;

         if( m_pCurrentGlView->type() == PMGLView::PMViewCamera )
         {
            if( m_pCurrentGlView->camera() )
            {
               if( m_pCurrentGlView->camera()->cameraType()
                   == PMCamera::Omnimax )
                  disableView = true;
            }
            else
               disableView = true;
         }

         if( !disableView )
            setProjection();

         glLoadIdentity();

         glDisable( GL_DEPTH_TEST );

         if( m_pCurrentGlView->type() == PMGLView::PMViewCamera )
            renderFieldOfView();
         else
            renderGrid();

         renderDescription();
         glEnable( GL_DEPTH_TEST );

         if( !disableView )
         {
            renderAxes();

            renderObject( m_pCurrentTask->topLevelObject() );

            if( !m_bStopTask && !m_bStartTask ) renderControlPoints();
         }

         if( !m_bStopTask && !m_bStartTask )
         {
            emit aboutToUpdate( m_pCurrentGlView );

            if( m_bStopTask || m_bStartTask ) break;

            QImage image = this->grabFramebuffer();
            //image.save("/home/alberto/xxx.png");
            m_pCurrentGlView->setRenderedScene( QPixmap::fromImage( image ) );
            //this->context()->swapBuffers( this->context()->surface() );
            //glFinish();

            emit renderingFinished( m_pCurrentGlView );

            if( m_bStopTask || m_bStartTask ) break;

            QCoreApplication::instance()->processEvents( QEventLoop::AllEvents, maxEventTime );

            if( m_bStopTask || m_bStartTask ) break;

            m_renderTasks.removeFirst();
         }
      }
   }
   emit renderingFinished();

   m_bStopTask = false;
   m_bStartTask = false;
   m_bTaskIsRunning = false;
}

void PMRenderManager::renderObject( PMObject* objectToRender )
{
   bool children = false;
   PMGraphicalObject* go = nullptr;

   m_objectToRenderStack.append( objectToRender );

   if( objectToRender->isA( "GraphicalObject" ) )
   {
      go = static_cast<PMGraphicalObject*>(objectToRender);
      m_visibilityStack.push( m_currentVisibility );
      if( go->isVisibilityLevelRelative() )
         m_currentVisibility += go->visibilityLevel();
      else
         m_currentVisibility = go->visibilityLevel();
   }

   if( !m_selected )
   {
      if( objectToRender->isSelected() )
      {
         m_selected = true;
         m_pDeselectObject = objectToRender;
         if( objectToRender->hasTransformationMatrix() )
            if( objectToRender->parent() )
               m_pDeselectObject = objectToRender->parent();
      }
   }

   if( ( m_pCurrentGlView->type() != PMGLView::PMViewCamera )
       || ( objectToRender != static_cast<PMObject*>(m_pCurrentGlView->camera()) ) )
     {
      PMObject* obj = nullptr;

      children = objectToRender->lastChild() || objectToRender->linkedObject();
      if( children )
      {
         bool stop;

         PMMatrix matrix;
         if( m_specialCameraMode )
            matrix = m_viewTransformation;
         else
            matrix = PMMatrix::modelviewMatrix();
         m_matrixStack.push( matrix );

         // render the children and the linked object
         obj = objectToRender->lastChild();
         while( obj && !m_bStopTask && !m_bStartTask )
         {
            if( !obj->isA( "Declare" ) )
               renderObject( obj );
            if( !m_bStopTask && !m_bStartTask )
            {
               do
               {
                  // Do not render declares
                  obj = obj->prevSibling();
                  if( !obj )
                     stop = true;
                  else
                     stop = !obj->isA( "Declare" );
               }
               while( !stop );
            }
         }
         if( !m_bStopTask && !m_bStartTask )
         {
            obj = objectToRender->linkedObject();
            if( obj )
               renderObject( obj );
         }
      }


      if( !m_bStopTask && !m_bStartTask )
      {
         // children of the object are rendered
         // render the object

         if( objectToRender == m_pCurrentTask->activeObject() )
         {
            if( m_specialCameraMode )
               m_controlPointTransformation = m_viewTransformation;
            else
               m_controlPointTransformation = PMMatrix::modelviewMatrix();
         }

         if( objectToRender->type() == "QuickColor" )
         {
            PMQuickColor* qc = static_cast<PMQuickColor*>(objectToRender);
				PMObjectList::const_iterator it = m_objectToRenderStack.constEnd();
            bool pofound = false;

            while( it != m_objectToRenderStack.constBegin() && !pofound )
            {
					--it;
               if( ( *it )->isA( "GraphicalObject" ) )
                  pofound = true;
            }
            if( pofound )
            {
               if( !m_quickColorObjects.isEmpty() && m_quickColorObjects.top() != *it )
               {
                  m_quickColorObjects.push( *it );
                  m_quickColors.push( m_currentColor );
                  m_currentColor = qc->color().toQColor();
               }
            }
         }

         PMViewStructure* vs = objectToRender->viewStructure();
         if( objectToRender->hasTransformationMatrix() || vs )
         {
            // object has transformation or view structure
            if( vs )
            {
               if( ( m_currentVisibility <= m_pCurrentTask->visibilityLevel() )
                   || ( objectToRender == m_pCurrentTask->activeObject() ) )
               {
                  // render the view structure.
                  // call qApp->processEvents() each m_nMaxRenderedLines rendered
                  // lines
                  if( m_selected )
                     setGLColor( m_graphicalObjectColor[ 1 ] );
                  else
                     setGLColor( m_currentColor );

                  renderViewStructure( *vs );
               }
            }
            else if( objectToRender->hasTransformationMatrix() )
            {
               if( m_specialCameraMode )
                  m_viewTransformation = m_viewTransformation * objectToRender->transformationMatrix();
               else
                  glMultMatrixd( objectToRender->transformationMatrix().data() );
            }
         }
         // end rendering
      }
   }
   if( !m_bStopTask && !m_bStartTask )
   {
      if( children )
      {
            if( !m_matrixStack.isEmpty() )
            {
				PMMatrix matrix = m_matrixStack.pop();

              if( m_specialCameraMode )
                 m_viewTransformation = matrix;
              else
                 glLoadMatrixd( matrix.data() );
            }
      }
      if( m_selected )
      {
         if( m_pDeselectObject == objectToRender )
         {
            m_selected = false;
            m_pDeselectObject = nullptr;
         }
      }
      if( !m_quickColorObjects.isEmpty() && m_quickColorObjects.top() == objectToRender )
      {
         m_quickColorObjects.pop();
			if( !m_quickColors.isEmpty() )
			{
				m_currentColor = m_quickColors.pop();
         }
      }
   }

   if( go )
      m_currentVisibility = m_visibilityStack.pop();

   m_objectToRenderStack.removeLast();
}

void PMRenderManager::renderViewStructure( PMViewStructure& vs )
{
   if( m_specialCameraMode )
   {
      PMPointArray points = vs.points();
      points.detach();
      transformProjection( points.data(), points.size(),
                           m_pCurrentGlView->camera() );

      if( m_highDetailCameraView )
      {
         // subdivide line
         PMLineArray& lines = vs.lines();
         PMPointArray& utPoints = vs.points();
         int numLines = lines.size();
         PMPointArray& sdPoints = m_subdivisionViewStructure.points();
         PMLineArray& sdLines = m_subdivisionViewStructure.lines();
         int i, li, sd;
         double distance;
         PMPoint start, end, dir;

         for( i = 0; ( i < numLines ) && !m_bStopTask && !m_bStartTask; i++ )
         {
            // calculate screen distance
            start = points[lines[i].startPoint()];
            end = points[lines[i].endPoint()];
            dir[0] = ( end[0] - start[0] ) / m_anglex;
            dir[1] = ( end[1] - start[1] ) / m_angley;
            distance = sqrt( dir[0] * dir[0] + dir[1] * dir[1] );

            // calculate number of subdivisions
            sd = ( int ) ( distance / subdivisionDistance );
            if( sd > 1 )
            {
               // calculate subdivision
               if( sd > maxSubdivisions )
                  sd = maxSubdivisions;

               sdPoints[0] = start;
               sdPoints[sd] = end;

               start = utPoints[lines[i].startPoint()];
               end = utPoints[lines[i].endPoint()];
               dir[0] = ( end[0] - start[0] ) / sd;
               dir[1] = ( end[1] - start[1] ) / sd;
               dir[2] = ( end[2] - start[2] ) / sd;

               for( li = 1; li < sd; li++ )
               {
                  sdPoints[li][0] = start[0] + li * dir[0];
                  sdPoints[li][1] = start[1] + li * dir[1];
                  sdPoints[li][2] = start[2] + li * dir[2];
               }
               // transform points (first and last are already transformed)
               transformProjection( sdPoints.data() + 1, sd - 1,
                                    m_pCurrentGlView->camera() );
               renderViewStructureSimple( sdPoints, sdLines, sd );
            }
            else
            {
               sdPoints[0] = start;
               sdPoints[1] = end;
               renderViewStructureSimple( sdPoints, sdLines, 1 );
            }
         }
      }
      else
         renderViewStructureSimple( points, vs.lines() );
   }
   else
      renderViewStructureSimple( vs.points(), vs.lines() );
}

void PMRenderManager::renderViewStructureSimple( PMPointArray& points,
                                                 PMLineArray& lines,
                                                 int numberOfLines )
{
   GLuint* linesData = ( GLuint* ) ( lines.data() );
   int vsLines = 0;
   int rl = 0;

   if( numberOfLines < 0 )
      vsLines = lines.size();
   else
      vsLines = numberOfLines;

   glEnableClientState( GL_VERTEX_ARRAY );
   glVertexPointer( 3, GL_DOUBLE, 0, points.data() );

   while( ( vsLines > 0 ) && !m_bStopTask && !m_bStartTask )
   {
      rl = m_nMaxRenderedLines - m_renderedLines;
      if( rl > vsLines )
         rl = vsLines;

      glDrawElements( GL_LINES, GLsizei( rl * 2 ), GL_UNSIGNED_INT, linesData );

      m_renderedLines += rl;
      if( m_renderedLines >= m_nMaxRenderedLines )
      {
         m_renderedLines = 0;
         qApp->processEvents( QEventLoop::AllEvents, maxEventTime );
         if( !m_bStopTask && !m_bStartTask )
            this->makeCurrent();
      }

      vsLines -= rl;
      linesData = linesData + ( rl * 2 );
   }

   glDisableClientState( GL_VERTEX_ARRAY );
}

void PMRenderManager::transformProjection( PMPoint* points, int size,
                                           PMCamera* camera )
{
   int i;
   PMPoint* data = points;
   PMPoint p;
   double cameraAngle = camera->angle() * M_PI / 180.0;
   double rad, phi, h;

   if( approxZero( cameraAngle ) )
      cameraAngle = M_PI;

   switch( camera->cameraType() )
   {
      case PMCamera::Perspective:
      case PMCamera::Orthographic:
         break;

      case PMCamera::UltraWideAngle:
         for( i = 0; i < size; i++ )
         {
            p = m_viewTransformation * (*data);

            // reverse povray's calculations
            p[0] /= m_rightLength;
            p[1] /= m_upLength;
            p[2] /= m_directionLength;
            h = sqrt( p[0] * p[0] + p[1] * p[1] + p[2] * p[2] );
            if( !approxZero( h ) )
            {
               p[0] /= h;
               p[1] /= h;
            }

            (*data)[0] = asin( p[0] );
            (*data)[1] = asin( p[1] );
            if( p[2] > 0 )
            {
               (*data)[0] = M_PI - (*data)[0];
               (*data)[1] = M_PI - (*data)[1];
            }
            (*data)[2] = -h;

            data++;
         }
         break;

      case PMCamera::FishEye:
         for( i = 0; i < size; i++ )
         {
            p = m_viewTransformation * (*data);

            // reverse povray's calculations
            phi = atan2( p[1], p[0] );
            rad = atan2( sqrt( p[0] * p[0] + p[1] * p[1] ), -p[2] );

            (*data)[0] = rad * cos( phi );
            (*data)[1] = rad * sin( phi );
            (*data)[2] = -sqrt( p[0] * p[0] + p[1] * p[1] + p[2] * p[2] );

            data++;
         }
         break;

      case PMCamera::Panoramic:
         for( i = 0; i < size; i++ )
         {
            p = m_viewTransformation * (*data);

            // reverse povray's calculations
            p[0] /= m_rightLength;
            p[1] /= m_upLength;
            p[2] /= m_directionLength;

            (*data)[0] = atan2( p[0], -p[2] );
            (*data)[1] = atan2( p[1], sqrt( p[0] * p[0] + p[2] * p[2] ) );
            (*data)[2] = -sqrt( p[0] * p[0] + p[1] * p[1] + p[2] * p[2] );

            data++;
         }
         break;

      case PMCamera::Cylinder:
         switch( camera->cylinderType() )
         {
            case 1:
               for( i = 0; i < size; i++ )
               {
                  p = m_viewTransformation * (*data);

                  // reverse povray's calculations
                  p[0] /= m_rightLength;
                  p[1] /= m_upLength;
                  p[2] /= m_directionLength;

                  h = sqrt( p[0] * p[0] + p[2] * p[2] );
                  if( approxZero( h ) )
                     h = 1e-5;

                  (*data)[0] = atan2( p[0], -p[2] ) / cameraAngle;
                  (*data)[1] = p[1] / h;
                  (*data)[2] = -h;

                  data++;
               }
               break;

            case 2:
               for( i = 0; i < size; i++ )
               {
                  p = m_viewTransformation * (*data);

                  // reverse povray's calculations
                  p[0] /= m_rightLength;
                  p[1] /= m_upLength;
                  p[2] /= m_directionLength;

                  h = sqrt( p[1] * p[1] + p[2] * p[2] );
                  if( approxZero( h ) )
                     h = 1e-5;

                  (*data)[0] = p[0] / h;
                  (*data)[1] = atan2( p[1], -p[2] ) / cameraAngle;
                  (*data)[2] = -h;

                  data++;
               }
               break;

            case 3:
               for( i = 0; i < size; i++ )
               {
                  p = m_viewTransformation * (*data);

                  // reverse povray's calculations
                  p[0] /= m_rightLength;
                  p[1] /= m_upLength;
                  p[2] /= m_directionLength;

                  h = sqrt( p[0] * p[0] + p[2] * p[2] );
                  if( approxZero( h ) )
                     h = 1e-5;

                  (*data)[0] = atan2( p[0], -p[2] ) / cameraAngle;
                  (*data)[1] = p[1];
                  (*data)[2] = -h;

                  data++;
               }
               break;

            case 4:
               for( i = 0; i < size; i++ )
               {
                  p = m_viewTransformation * (*data);

                  // reverse povray's calculations
                  p[0] /= m_rightLength;
                  p[1] /= m_upLength;
                  p[2] /= m_directionLength;

                  h = sqrt( p[1] * p[1] + p[2] * p[2] );
                  if( approxZero( h ) )
                     h = 1e-5;

                  (*data)[0] = p[0];
                  (*data)[1] = atan2( p[1], -p[2] ) / cameraAngle;
                  (*data)[2] = -h;

                  data++;
               }
               break;
         }
         break;

      case PMCamera::Omnimax:
         break;

   }
}

void PMRenderManager::renderControlPoints()
{
   if( ( m_pCurrentGlView->type() == PMGLView::PMViewCamera )
       && ( m_pCurrentGlView->camera() == m_pCurrentTask->activeObject() ) )
      return;

   if( m_specialCameraMode )
      m_viewTransformation = m_controlPointTransformation;
   else
      glLoadMatrixd( m_controlPointTransformation.data() );

   PMControlPointList* cplist = m_pCurrentTask->controlPoints();
   if( cplist->count() > 0 )
   {
      PMControlPointList::iterator it = cplist->begin();
      PMControlPoint* cp;
      PMPoint v;
      const GLubyte* bitmap = nullptr;

      // draw extra control point lines
      setGLColor( m_graphicalObjectColor[1] );
      for( it = cplist->begin(); it != cplist->end(); ++it )
      {
         cp = *it;
         if( cp->hasExtraLine() )
         {
            PMPoint s = PMPoint( cp->extraLineStart() );
            PMPoint e = PMPoint( cp->extraLineEnd() );

            if( m_specialCameraMode )
            {
               transformProjection( &s, 1, m_pCurrentGlView->camera() );
               transformProjection( &e, 1, m_pCurrentGlView->camera() );
            }

            glBegin( GL_LINES );
            glVertex3d( s[0], s[1], s[2] );
            glVertex3d( e[0], e[1], e[2] );
            glEnd();
         }
      }

      glDisable( GL_DEPTH_TEST );
      // draw not selected control points
      setGLColor( m_controlPointColor[0] );
      for( it = cplist->begin(); it != cplist->end(); ++it )
      {
         cp = *it;

         if( cp->display() )
         {
            v = PMPoint( cp->position() );
            if( m_specialCameraMode )
               transformProjection( &v, 1, m_pCurrentGlView->camera() );
            switch( cp->displayType() )
            {
               case PMControlPoint::CPPoint:
                  if( !cp->selected() )
                     bitmap = PointBitmap;
                  break;
               case PMControlPoint::CPCross:
                  bitmap = CrossBitmap;
                  break;
            }

            glRasterPos3d( v[0], v[1], v[2] );
            if( bitmap )
               glBitmap( controlPointSize, controlPointSize,
                         controlPointSize/2, controlPointSize/2,
                         0, 0, bitmap );
         }
      }
      // draw selected control points
      setGLColor( m_controlPointColor[1] );
      for( it = cplist->begin(); it != cplist->end(); ++it )
      {
         cp = *it;

         if( cp->selected() && cp->display() )
         {
            v = PMPoint( cp->position() );
            if( m_specialCameraMode )
               transformProjection( &v, 1, m_pCurrentGlView->camera() );
            if( cp->displayType() == PMControlPoint::CPPoint )
               bitmap = PointBitmap;

            glRasterPos3d( v[0], v[1], v[2] );
            if( bitmap )
               glBitmap( controlPointSize, controlPointSize,
                         controlPointSize/2, controlPointSize/2,
                         0, 0, bitmap );
         }
      }
   }
}

void PMRenderManager::renderAxes()
{
   int i;

   if( !m_axesViewStructureCreated )
   {
      m_axesViewStructure[0] = PMViewStructure( 6, 9 );
      PMPointArray& points = m_axesViewStructure[0].points();
      PMLineArray& lines = m_axesViewStructure[0].lines();

      lines[0] = PMLine( 0, 1 );
      lines[1] = PMLine( 1, 2 );
      lines[2] = PMLine( 1, 3 );
      lines[3] = PMLine( 1, 4 );
      lines[4] = PMLine( 1, 5 );
      lines[5] = PMLine( 2, 3 );
      lines[6] = PMLine( 3, 4 );
      lines[7] = PMLine( 4, 5 );
      lines[8] = PMLine( 5, 2 );

      points[0] = PMPoint( 0.0, 0.0, 0.0 );
      points[1] = PMPoint( 1.0, 0.0, 0.0 );
      points[2] = PMPoint(  dA,  dB,  dB );
      points[3] = PMPoint(  dA, -dB,  dB );
      points[4] = PMPoint(  dA, -dB, -dB );
      points[5] = PMPoint(  dA,  dB, -dB );

      m_axesViewStructure[1] = m_axesViewStructure[0];
      PMPointArray& points1 = m_axesViewStructure[1].points();
      points1.detach();

      points1[0] = PMPoint( 0.0, 0.0, 0.0 );
      points1[1] = PMPoint( 0.0, 1.0, 0.0 );
      points1[2] = PMPoint(  dB,  dA,  dB );
      points1[3] = PMPoint( -dB,  dA,  dB );
      points1[4] = PMPoint( -dB,  dA, -dB );
      points1[5] = PMPoint(  dB,  dA, -dB );

      m_axesViewStructure[2] = m_axesViewStructure[0];
      PMPointArray& points2 = m_axesViewStructure[2].points();
      points2.detach();

      points2[0] = PMPoint( 0.0, 0.0, 0.0 );
      points2[1] = PMPoint( 0.0, 0.0, 1.0 );
      points2[2] = PMPoint(  dB,  dB,  dA );
      points2[3] = PMPoint( -dB,  dB,  dA );
      points2[4] = PMPoint( -dB, -dB,  dA );
      points2[5] = PMPoint(  dB, -dB,  dA );

      m_axesViewStructureCreated = true;
   }

   glEnable( GL_DEPTH_TEST );
   for( i = 0; i < 3; i++ )
   {
      setGLColor( m_axesColor[i] );
      renderViewStructure( m_axesViewStructure[i] );
   }
}

PMMatrix PMRenderManager::viewTransformation( PMCamera* c ) const
{
   PMVector location, lookAt, sky;
   PMMatrix m;

   sky = c->sky();
   location = c->location();
   lookAt = c->lookAt();

   if( approxZero( sky.abs() ) )
      sky = PMVector( 0.0, 1.0, 0.0 );
   if( approxZero( ( location - lookAt ).abs() ) )
      lookAt = location + PMVector( 0.0, 0.0, 1.0 );

   m = c->transformedWith();
   if( m.canBuildInverse() )
      return PMMatrix::viewTransformation( location, lookAt, sky ) * m.inverse();
   return PMMatrix::viewTransformation( location, lookAt, sky );
}

void PMRenderManager::setProjection()
{
   PMGLView::PMViewType type = m_pCurrentGlView->type();
   PMCamera* camera = m_pCurrentGlView->camera();
   int width = m_pCurrentGlView->width();
   int height = m_pCurrentGlView->height();

   if( type == PMGLView::PMViewCamera )
   {
      if( camera )
         setCameraProjection();
   }
   else
   {
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();

      double d = m_pCurrentGlView->scale();

      // TODO calculating the z clipping plane
      glOrtho( -width/2, width/2, -height/2, height/2,
               -viewVolumeZ, viewVolumeZ );
      glScaled( d, d, d );
      glTranslated( m_pCurrentGlView->translationX(),
                    m_pCurrentGlView->translationY(), 0 );

      switch( type )
      {
         case PMGLView::PMViewPosZ:
            break;
         case PMGLView::PMViewNegZ:
            glRotated( 180.0, 0.0, 1.0, 0.0 );
            break;
         case PMGLView::PMViewNegY:
            glRotated( 90.0, 1.0, 0.0, 0.0 );
            break;
         case PMGLView::PMViewPosY:
            glRotated( -90.0, 1.0, 0.0, 0.0 );
            break;
         case PMGLView::PMViewPosX:
            glRotated( 90.0, 0.0, 1.0, 0.0 );
            break;
         case PMGLView::PMViewNegX:
            glRotated( -90.0, 0.0, 1.0, 0.0 );
            break;
         default:
            break;
      }

      glScaled( 1.0, 1.0, -1.0 );

      glMatrixMode( GL_MODELVIEW );
      m_pCurrentGlView->setProjectionUpToDate( true );
   }
}

void PMRenderManager::setCameraProjection()
{
   PMCamera* camera = m_pCurrentGlView->camera();
   int width = m_pCurrentGlView->width();
   int height = m_pCurrentGlView->height();
   double angle = M_PI / 2.0;
   double modeAspect, viewAspect, cameraAspect;

   m_viewTransformation = viewTransformation( camera );
   m_upLength = camera->up().abs();
   if( approxZero( m_upLength ) )
      m_upLength = 1.0;
   m_rightLength = camera->right().abs();
   if( approxZero( m_rightLength ) )
      m_rightLength = 1.0;
   m_directionLength = camera->direction().abs();
   if( approxZero( m_directionLength ) )
      m_directionLength = 1.0;

   if( camera->isAngleEnabled() )
      angle = camera->angle() * M_PI / 180.0;

   m_anglex = 0.5;
   m_angley = 0.5;
   if( ( angle <= 0.0 ) || ( angle > 2 * M_PI ) )
      angle = M_PI;

   switch( camera->cameraType() )
   {
      case PMCamera::Perspective:
         //  If angle wasn't specified determine one from right and direction
         if( !camera->isAngleEnabled() )
            angle = 2 * atan2( 0.5 * m_rightLength, m_directionLength );
         break;
      case PMCamera::UltraWideAngle:
         m_anglex = angle / ( 2.0 * M_PI );
         m_angley = angle / ( 2.0 * M_PI );
         m_specialCameraMode = true;
         break;
      case PMCamera::FishEye:
         m_anglex = angle / 2.0;
         m_angley = angle / 2.0;
         m_specialCameraMode = true;
         break;
      case PMCamera::Panoramic:
         m_anglex = M_PI / 2.0;
         m_angley = M_PI / 2.0;
         m_specialCameraMode = true;
         break;
      case PMCamera::Cylinder:
         m_anglex = 0.5;
         m_angley = 0.5;
         m_specialCameraMode = true;
         break;
      case PMCamera::Omnimax:
         m_specialCameraMode = true;
         break;
      default:
         break;
   }

   modeAspect = m_pCurrentTask->aspectRatio();

   if( approxZero( modeAspect ) )
      modeAspect = 1.0;

   cameraAspect = camera->aspect();

   if( approxZero( cameraAspect ) )
      cameraAspect = 1.0;

   viewAspect = ( double ) width / ( double ) height;

   if( approxZero( viewAspect ) )
      viewAspect = 1.0;

   if( viewAspect > modeAspect )
      m_anglex *= viewAspect / modeAspect;
   else
      m_angley *= modeAspect / viewAspect;


   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();


   PMVector up, right, direction;
   double handedness;
   PMMatrix m;

   up = camera->up();
   right = camera->right();
   direction = camera->direction();

   if( approxZero( m_upLength ) )
      up = PMVector( 0.0, 1.0, 0.0 );

   if( approxZero( m_rightLength ) )
      right = PMVector( 1.0, 0.0, 0.0 );

   if( approxZero( m_directionLength ) )
      direction = PMVector( 0.0, 0.0, 1.0 );

   handedness = PMVector::dot( PMVector::cross( up, direction ), right );

   switch( camera->cameraType() )
   {
      case PMCamera::Perspective:
         if( ( angle <= 0.0 ) || ( angle >= M_PI ) )
            angle = M_PI / 2.0;

         // opengl needs the vertical angle
         if( viewAspect < modeAspect )
            angle = atan( tan( angle / 2.0 ) / cameraAspect * modeAspect
                          / viewAspect ) * 360.0 / M_PI;
         else
            angle = atan( tan( angle / 2.0 ) / cameraAspect )
                    * 360.0 / M_PI;

         gluPerspective( angle, cameraAspect * viewAspect / modeAspect,
                         0.001, viewVolumeZ );

         if( handedness > 0 )
            glScaled( -1.0, 1.0, 1.0 );

         glMultMatrixd( m_viewTransformation.data() );
         break;

      case PMCamera::Orthographic:
         m_anglex = m_rightLength / 2.0;
         m_angley = m_upLength / 2.0;

         if( viewAspect > modeAspect )
            m_anglex *= viewAspect / modeAspect;
         else
            m_angley *= modeAspect / viewAspect;

         glOrtho( -m_anglex, m_anglex, -m_angley, m_angley,
                  0, viewVolumeZ );

         if( handedness > 0 )
            glScaled( -1.0, 1.0, 1.0 );

         glMultMatrixd( m_viewTransformation.data() );
         break;

      case PMCamera::UltraWideAngle:
      case PMCamera::FishEye:
      case PMCamera::Panoramic:
      case PMCamera::Cylinder:
      case PMCamera::Omnimax:
         glOrtho( -m_anglex, m_anglex, -m_angley, m_angley,
                  -viewVolumeZ, viewVolumeZ );

         if( handedness > 0 )
            glScaled( -1.0, 1.0, 1.0 );
         break;
   }

   glMatrixMode( GL_MODELVIEW );
   m_pCurrentGlView->setProjectionUpToDate( true );
}

void PMRenderManager::renderFieldOfView()
{
   if( m_pCurrentGlView->type() == PMGLView::PMViewCamera )
   {
      PMCamera* camera = m_pCurrentGlView->camera();

      if( camera )
      {
         int width = m_pCurrentGlView->width();
         int height = m_pCurrentGlView->height();
         double modeAspect, viewAspect;
         int d, vx1, vx2, vy1, vy2;

         modeAspect = m_pCurrentTask->aspectRatio(); //camera->aspect();
         if( approxZero( modeAspect ) )
            modeAspect = 1.0;
         viewAspect = ( double ) width / ( double ) height;

         if( viewAspect < modeAspect )
         {
            vx1 = 0;
            vx2 = width - 1;

            d = ( int ) ( height - width / modeAspect + 0.5 ) / 2;

            vy1 = d;
            vy2 = height - d - 1;
         }
         else
         {
            vy1 = 0;
            vy2 = height - 1;

            d = ( int ) ( height * modeAspect );

            d = ( width - d ) / 2;
            vx1 = d;
            vx2 = width - d - 1;
         }

         glMatrixMode( GL_PROJECTION );
         glPushMatrix();
         glLoadIdentity();
         glOrtho( 0, width, 0, height, -2, 2 );
         glMatrixMode( GL_MODELVIEW );
         glPushMatrix();
         glLoadIdentity();

         setGLColor( m_fieldOfViewColor );
         glDisable( GL_DEPTH_TEST );

         if( camera->cameraType() == PMCamera::Omnimax )
            renderString( tr( "not supported" ), 5.0,
                          height - qApp->fontMetrics().height() * 2 - 2 );
         else if( m_specialCameraMode && !m_highDetailCameraView )
            renderString( tr( "approximated" ), 5.0,
                          height - qApp->fontMetrics().height() * 2 - 2 );

         glBegin( GL_LINE_LOOP );

         glVertex2d( vx1, vy1 );
         glVertex2d( vx2, vy1 );
         glVertex2d( vx2, vy2 );
         glVertex2d( vx1, vy2 );

         glEnd();
         glEnable( GL_DEPTH_TEST );

         glMatrixMode( GL_PROJECTION );
         glPopMatrix();
         glMatrixMode( GL_MODELVIEW );
         glPopMatrix();
      }
   }
}

void PMRenderManager::renderGrid()
{
   double scale = m_pCurrentGlView->scale();

   if( scale >= 0 )
   {
      // calculate the views grid distance
      double viewGridDistance = pow( 10.0, ceil( log10( ( double ) m_gridDistance / scale ) ) );
      int sd = ( int ) ( viewGridDistance * scale + 0.5 );

      if( ( sd * 0.2 ) > m_gridDistance )
         viewGridDistance *= 0.2;
      else if( ( sd * 0.5 ) > m_gridDistance )
         viewGridDistance *= 0.5;

      // draw the grid
      double x1, x2, y1, y2, sx, sy;
      double x, y;
      int gi;
      double screenx, screeny;
      double signx = 1.0, signy = 1.0;

      int height = m_pCurrentGlView->height();
      int width = m_pCurrentGlView->width();
      double transX = m_pCurrentGlView->translationX();
      double transY = m_pCurrentGlView->translationY();
      int fontHeight = qApp->fontMetrics().height();

      glMatrixMode( GL_PROJECTION );
      glPushMatrix();
      glLoadIdentity();
      glOrtho( -width/2, width/2, -height/2, height/2, -2, 2 );

      glMatrixMode( GL_MODELVIEW );
      glPushMatrix();
      glLoadIdentity();

      setGLColor( m_gridColor );
      glDisable( GL_DEPTH_TEST );

      switch( m_pCurrentGlView->type() )
      {
         case PMGLView::PMViewPosX:
            signx = -1.0;
            break;
         case PMGLView::PMViewPosY:
            signy = -1.0;
            break;
         case PMGLView::PMViewNegZ:
            signx = -1.0;
            break;
         default:
            break;
      }

      sx = width / scale;
      sy = height / scale;
      x1 = -transX - sx / 2;
      x2 = -transX + sx / 2;
      y1 = -transY - sy / 2;
      y2 = -transY + sy / 2;

      sx = ceil( x1 / viewGridDistance ) * viewGridDistance;
      gi = 0;
      x = sx;
      while( x < x2 )
      {
         screenx = ( x + transX ) * scale;
         glBegin( GL_LINES );
         glVertex2d( screenx, -height/2 );
         glVertex2d( screenx, height/2 );
         glEnd();

         QString label = QString( "%1" ).arg( x * signx, 0, 'g', 4 );
         if( approxZero( x ) && label.indexOf( "e-" ) >= 0 )
            label = "0";

         renderString( label, screenx + 3, height / 2 - fontHeight - 2 );

         gi++;
         x = sx + viewGridDistance * gi;
      }

      sy = ceil( y1 / viewGridDistance ) * viewGridDistance;
      gi = 0;
      y = sy;
      while( y < y2 )
      {
         screeny = ( y + transY ) * scale;
         glBegin( GL_LINES );
         glVertex2d( -width/2, screeny );
         glVertex2d( width/2, screeny );
         glEnd();

         QString label = QString( "%1" ).arg( y * signy, 0, 'g', 4 );
         if( approxZero( y ) && label.indexOf( "e-" ) >= 0 )
            label = "0";

         renderString( label, -width / 2 + 3, screeny + 2 );

         gi++;
         y = sy + viewGridDistance * gi;
      }


      setGLColor( axesColor( 0 ) );
      switch( m_pCurrentGlView->type() )
      {
         case PMGLView::PMViewPosY:
         case PMGLView::PMViewPosZ:
         case PMGLView::PMViewNegY:
            renderString( "x", width / 2 - qApp->fontMetrics().boundingRect( "x" ).width() - 4, -3 );
            break;
         case PMGLView::PMViewNegZ:
            renderString( "x", -width / 2 + 3, -3 );
            break;
         default:
            break;
      }
      setGLColor( axesColor( 1 ) );
      switch( m_pCurrentGlView->type() )
      {
         case PMGLView::PMViewPosX:
         case PMGLView::PMViewNegX:
         case PMGLView::PMViewPosZ:
         case PMGLView::PMViewNegZ:
            renderString( "y", -3, height / 2 - fontHeight );
            break;
         default:
            break;
      }
      setGLColor( axesColor( 2 ) );
      switch( m_pCurrentGlView->type() )
      {
         case PMGLView::PMViewPosX:
            renderString( "z", -width / 2 + 3, -3 );
            break;
         case PMGLView::PMViewNegX:
            renderString( "z", width / 2 - qApp->fontMetrics().boundingRect( "z" ).width() - 4, -3 );
            break;
         case PMGLView::PMViewNegY:
            renderString( "z", -3, height / 2 - fontHeight );
            break;
         case PMGLView::PMViewPosY:
            renderString( "z", -3, -height / 2 );
            break;
         default:
            break;
      }

      glEnable( GL_DEPTH_TEST );
      glMatrixMode( GL_PROJECTION );
      glPopMatrix();
      glMatrixMode( GL_MODELVIEW );
      glPopMatrix();
   }
}

void PMRenderManager::renderDescription()
{
   int height = m_pCurrentGlView->height();
   int width = m_pCurrentGlView->width();
   int fontHeight = qApp->fontMetrics().height();

   glMatrixMode( GL_PROJECTION );
   glPushMatrix();
   glLoadIdentity();
   glOrtho( 0, width, 0, height, -2, 2 );
   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
   glLoadIdentity();

   setGLColor( m_fieldOfViewColor );

   switch( m_pCurrentGlView->type() )
   {
      case PMGLView::PMViewPosX:
         renderString( tr( "left" ), 5.0, height - fontHeight - 2 );
         break;
      case PMGLView::PMViewNegX:
         renderString( tr( "right" ), 5.0, height - fontHeight - 2 );
         break;
      case PMGLView::PMViewPosY:
         renderString( tr( "bottom" ), 5.0, height - fontHeight - 2 );
         break;
      case PMGLView::PMViewNegY:
         renderString( tr( "top" ), 5.0, height - fontHeight - 2 );
         break;
      case PMGLView::PMViewPosZ:
         renderString( tr( "front" ), 5.0, height - fontHeight - 2 );
         break;
      case PMGLView::PMViewNegZ:
         renderString( tr( "back" ), 5.0, height - fontHeight - 2 );
         break;
      case PMGLView::PMViewCamera:
      {
         PMCamera* c = m_pCurrentGlView->camera();
         if( c )
         {
            QString name( "-" );
            if( !c->name().isEmpty() )
               name = c->name();
            else
               name = tr( "(unnamed)" );

            renderString( tr( "camera" ) + ": " + name,
                          5.0, height - fontHeight - 2 );
         }
         else
            renderString( "camera", 5.0, height - fontHeight - 2 );
         break;
      }
   }

   glEnable( GL_DEPTH_TEST );

   glMatrixMode( GL_PROJECTION );
   glPopMatrix();
   glMatrixMode( GL_MODELVIEW );
   glPopMatrix();
}

void PMRenderManager::renderString( const QString& str, double x, double y )
{
   int width  = qApp->fontMetrics().boundingRect( str ).width();
   int height = qApp->fontMetrics().height();

   // GL wants word aligned bitmap
   QBitmap bm( ( ( width + 32 ) % 32 ) * 32, height );
   bm.clear();

   QPainter p( &bm );
   p.setFont( qApp->font() );
   p.drawText( bm.rect(), Qt::AlignLeft | Qt::AlignBottom, str );
   p.end();

   // Transform to GL bitmap
   QImage img = bm.toImage().mirrored( false, true ).convertToFormat( QImage::Format_Mono );

   glRasterPos2d( x, y );
   glBitmap( img.width(), img.height(), 0, 0, 0, 0, img.bits() );
}

void PMRenderManager::setGLColor( const QColor& c )
{
   int r, g, b;

   c.getRgb( &r, &g, &b );
   glColor3ub( ( GLubyte ) r, ( GLubyte ) g, ( GLubyte ) b );
}

void PMRenderManager::slotRenderingSettingsChanged()
{
   emit renderingSettingsChanged();
}

void PMRenderManager::saveConfig()
{
   QSettings qset;
   qset.beginGroup( "Rendering" );
   qset.setValue( "BackgroundColor", m_backgroundColor );
   qset.setValue( "RGraphicalObjectColor0", m_graphicalObjectColor[0] );
   qset.setValue( "GraphicalObjectColor1", m_graphicalObjectColor[1] );
   qset.setValue( "ControlPointColor0", m_controlPointColor[0] );
   qset.setValue( "ControlPointColor1", m_controlPointColor[1] );
   qset.setValue( "AxesColorX", m_axesColor[0] );
   qset.setValue( "AxesColorY", m_axesColor[1] );
   qset.setValue( "AxesColorZ", m_axesColor[2] );
   qset.setValue( "GridColor", m_gridColor );
   qset.setValue( "GridDistance", m_gridDistance );
   qset.setValue( "FieldOfViewColor", m_fieldOfViewColor );
   qset.setValue( "HighDetailCameraViews", m_highDetailCameraView );
}

void PMRenderManager::restoreConfig()
{
   QSettings qset;
   qset.beginGroup( "Rendering" );
   m_backgroundColor = qset.value( "BackgroundColor", m_backgroundColor ).value<QColor>();
   m_graphicalObjectColor[0] = qset.value( "GraphicalObjectColor0", m_graphicalObjectColor[0] ).value<QColor>();
   m_graphicalObjectColor[1] = qset.value( "GraphicalObjectColor1", m_graphicalObjectColor[1] ).value<QColor>();
   m_controlPointColor[0] = qset.value( "ControlPointColor0", m_controlPointColor[0] ).value<QColor>();
   m_controlPointColor[1] = qset.value( "ControlPointColor1", m_controlPointColor[1] ).value<QColor>();
   m_axesColor[0] = qset.value( "AxesColorX", m_axesColor[0] ).value<QColor>();
   m_axesColor[1] = qset.value( "AxesColorY", m_axesColor[1] ).value<QColor>();
   m_axesColor[2] = qset.value( "AxesColorZ", m_axesColor[2] ).value<QColor>();
   m_gridColor = qset.value( "GridColor", m_gridColor ).value<QColor>();
   m_gridDistance = qset.value( "GridDistance", m_gridDistance ).value<int>();
   m_fieldOfViewColor = qset.value( "FieldOfViewColor", m_fieldOfViewColor ).value<QColor>();
   m_highDetailCameraView = qset.value( "HighDetailCameraViews", m_highDetailCameraView ).value<bool>();
}

#include "pmrendermanager.moc"
