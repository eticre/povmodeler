/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2003 by Andreas Zehender
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

#include "pmpovrayrenderwidget.h"
#include "pmdefaults.h"
#include "pmdebug.h"
#include "pmdragwidget.h"

#include <QInputDialog>

#include <QUrl>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QColor>
#include <QPainter>
#include <QTextStream>
#include <QMimeData>
#include <QPaintEvent>
#include <QMessageBox>
#include <QDrag>
#include <QDir>
#include <QByteArray>
#include <QSettings>

QString PMPovrayRenderWidget::s_povrayCommand = c_defaultPovrayCommand;
//QString PMPovrayRenderWidget::s_povrayCommand = "/usr/bin/povray";

QStringList PMPovrayRenderWidget::s_libraryPaths;

PMPovrayRenderWidget::PMPovrayRenderWidget( QWidget* parent )
      : PMDragWidget( parent )
{
   m_pProcess = nullptr;
   m_bSuspended = false;
   m_rcvHeader = false;
   m_skipBytes = 0;
   m_bPixmapUpToDate = false;
   m_pTempFile = nullptr;
   is_preview = false;

   QPalette palette;
   palette.setColor( backgroundRole(), QColor( 0, 0, 0 ) );
   setPalette( palette );
   setAutoFillBackground( true );
}

PMPovrayRenderWidget::~PMPovrayRenderWidget()
{
   cleanup();
}

bool PMPovrayRenderWidget::render( const QByteArray& scene,
                                   const PMRenderMode& m,
                                   const QUrl &documentURL )
{
   cleanup();

   m_povrayOutput = "";
   m_renderMode = m;
   is_preview = m_renderMode.m_preview;

   if( !scene.data() )
   {
      QMessageBox::information( this, "warning", "Cannot render an empty scene.\n",
                   QMessageBox::Ok, QMessageBox::NoButton);
      return false;
   }

   // output to tmp file
   m_pTempFile = new QTemporaryFile( QDir::tempPath() + "/XXXXXX.pov"  );
   if( !m_pTempFile->open() )
   {
      QMessageBox::information( this, "sorry", "Could not write the scene to a temp file.\n",
                   QMessageBox::Ok, QMessageBox::NoButton);
      return false;
   }

   QDataStream dstr ( m_pTempFile );
   dstr.writeRawData( scene.data(), scene.size() );
   m_pTempFile->flush();

   m_pProcess = new QProcess( this );

   connect( m_pProcess, SIGNAL( finished( int, QProcess::ExitStatus ) ),
                        SLOT( slotRenderingFinished() ) );

   QStringList::ConstIterator it;
   QStringList args = m_renderMode.commandLineSwitches();

   for( it = args.constBegin(); it != args.constEnd(); ++it )
   { m_args = m_args + *it + " "; }

   m_args = m_args + "\n";

   for( it = s_libraryPaths.constBegin(); it != s_libraryPaths.constEnd(); ++it )
   {
      QString path = *it;
      if( path != QString( "/" ) )
         if( path.right( 1 ) == QString( "/" ) )
            path.truncate( path.length() - 1 );
     m_args = m_args + ( QString( "+L" ) + path ) + " ";
     m_args = m_args + "\n";
   }

   //eticre add +D +P for preview  png default format +FP
   m_args =  m_args + QString( "+I" ) + m_pTempFile->fileName() + " +O-" + " +FP"
               + "\n" +
               + " +K0.0" + " +KFI1" + " +KFF1" + " +KI0.0" + " +KF0.0"
               + "\n" +
               + " +SF1" + " +EF1"+ " -KC" + " +D" + " +P";

   QFileInfo file_info( documentURL.toString() );
	       
   if( !documentURL.isEmpty() && documentURL.isLocalFile() )
   {
      m_pProcess->setWorkingDirectory( file_info.path().remove( 0, 6 ) );
   }

   m_progress = 0;
   m_bPixmapUpToDate = false;

   
    //get user input modify args
    QInputDialog qi;
    qi.setInputMode( QInputDialog::TextInput );
    qi.setOptions( QInputDialog::UsePlainTextEditForTextInput );
    m_args = qi.getMultiLineText( this, "povray", "                             ______________________cmdline____________________                                   ",
                                   m_args, nullptr, Qt::Widget, Qt::ImhMultiLine );

    m_args.replace( "\n", " " );
    m_args.replace( "  ", " " );
    m_list = m_args.split( " " );
    // povray execution
    m_pProcess->start( s_povrayCommand, m_list );
   if( !m_pProcess->waitForStarted( 6000 ) )
   {
       QMessageBox::warning( this, "warning" ,
                   "Could not call povray.\n"
                   "Please check your installation "
                   "or set another povray command." ,
                   QMessageBox::Ok, QMessageBox::NoButton);
      //m_pProcess->close();
      //delete m_pProcess;
      m_pProcess = nullptr;
      return false;
   }
   m_image.fill( qRgb( 0, 0, 0 ) );
   repaint();
   m_bSuspended = false;
   return true;
}

void PMPovrayRenderWidget::killRendering()
{
   if( m_pProcess )
   {
      if( m_bSuspended )
         m_bSuspended = false;
      m_pProcess->terminate();
   }
}

void PMPovrayRenderWidget::suspendRendering()
{
   if( m_pProcess )
   {
      m_bSuspended = true;
      m_pProcess->close();
   }
}

void PMPovrayRenderWidget::resumeRendering()
{
   if( m_pProcess )
   {
      m_pProcess->close(  );
      m_bSuspended = false;
   }
}

/*
void PMPovrayRenderWidget::slotPovrayMessage( QProcess*, char* buffer, int buflen )
{
   QString str = QString::fromLatin1( buffer, buflen );
   m_povrayOutput += str;
   emit povrayMessage( str );
}
*/

void PMPovrayRenderWidget::slotRenderingFinished()
{
   if( !m_pProcess->exitStatus() )
   {
         m_image.loadFromData( m_pProcess->readAllStandardOutput() );
         emit povrayMessage( QString( m_pProcess->readAllStandardError() ) );
         repaint();
         emit( finished( 0 ) );
   }
   else
   {
      emit povrayMessage( QString( m_pProcess->readAllStandardError() ) );
      emit( finished( -1000 ) ); // call void PMDialogEditBase::slotPreviewFinished( int exitStatus )
   }

   cleanup();
}

void PMPovrayRenderWidget::paintEvent( QPaintEvent* ev )
{
   //if( is_preview ) return;
   QPainter painter( this );
   painter.drawImage( QPoint( 0, 0 ), m_image );

   painter.drawTiledPixmap( ev->rect().left(), ev->rect().top(),
                          ev->rect().width(), ev->rect().height(),
                    m_pixmap, ev->rect().left(), ev->rect().top() );
 //  bitBlt( this, ev->rect().left(), ev->rect().top(),
   //        &m_pixmap, ev->rect().left(), ev->rect().top(),
     //      ev->rect().width(), ev->rect().height()/*, CopyROP*/ );
}

void PMPovrayRenderWidget::cleanup()
{
   m_args = "";
   if( m_pProcess )
      delete m_pProcess;
   m_pProcess = nullptr;
   if( m_pTempFile ) { delete m_pTempFile; }
   m_pTempFile = nullptr;
}

QSize PMPovrayRenderWidget::sizeHint() const
{
   QSize s;
   if( m_image.isNull() )
      s = QSize( 200, 200 );
   else
      s = m_image.size();

   return s.expandedTo( minimumSize() );
}

void PMPovrayRenderWidget::saveConfig()
{
   QSettings qset;
   qset.beginGroup( "Povray" );
   qset.setValue( "PovrayCommand", s_povrayCommand );
   qset.setValue( "LibraryPaths", s_libraryPaths );
}

void PMPovrayRenderWidget::restoreConfig()
{
   QSettings qset;
   qset.beginGroup( "Povray" );
   s_povrayCommand = qset.value( "PovrayCommand", c_defaultPovrayCommand ).toString();
   s_libraryPaths = qset.value( "LibraryPaths", s_libraryPaths ).toStringList();
}

void PMPovrayRenderWidget::startDrag()
{
   QDrag *drag = new QDrag( this );
   QMimeData *mimeData = new QMimeData;
   mimeData->setImageData( m_image );
   drag->setMimeData( mimeData );
   drag->exec();
}

#include "pmpovrayrenderwidget.moc"
