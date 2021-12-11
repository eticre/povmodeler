/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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
//visualize render scene

#include "pmpovraywidget.h"
#include "pmpovrayrenderwidget.h"
#include "pmpovrayoutputwidget.h"
#include "pmshell.h"

#include <QLocale>
#include <QMessageBox>

#include <QTemporaryFile>
//#include <kglobalsettings.h>
#include <QScreen>
#include <QMimeType>
#include <QMimeDatabase>
#include <QFileDialog>
#include <QPushButton>
#include <QLayout>
#include <QProgressBar>
#include <QLabel>
#include <QScrollArea>
#include <QTimer>
#include <QApplication>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImageWriter>
#include <QPushButton>

const int timerIntervall = 1000;

PMPovrayWidget::PMPovrayWidget()
{
   QVBoxLayout* topLayout = new QVBoxLayout( this );
   topLayout->setMargin( 0 );
   topLayout->addStretch();

   QHBoxLayout* renderLayout = new QHBoxLayout();
   topLayout->addLayout( renderLayout, 2 );
   m_pScrollView = new QScrollArea( this );
   m_pScrollView->setWidgetResizable( true );
   renderLayout->addWidget( m_pScrollView, 2 );
   m_pRenderWidget = new PMPovrayRenderWidget( m_pScrollView->viewport() );
   m_pRenderWidget->setMinimumSize( 640, 480 );

   m_pScrollView->setWidget( m_pRenderWidget );
   m_pScrollView->adjustSize();
   topLayout->addStretch();

   QHBoxLayout* progressLayout = new QHBoxLayout();
   topLayout->addLayout( progressLayout );
   m_pProgressBar = new QProgressBar( this );
   m_pProgressBar->hide();
   progressLayout->addWidget( m_pProgressBar, 1 );
   m_pProgressLabel = new QLabel( this );
   progressLayout->addWidget( m_pProgressLabel, 2 );

   QHBoxLayout* buttonLayout = new QHBoxLayout();
   topLayout->addLayout( buttonLayout );
   m_pStopButton = new QPushButton(  "Stop", this );
   m_pStopButton->setEnabled( false );
   buttonLayout->addWidget( m_pStopButton );
   m_pSuspendButton = new QPushButton( "Suspend", this );
   m_pSuspendButton->setEnabled( false );
   buttonLayout->addWidget( m_pSuspendButton );
   m_pResumeButton = new QPushButton( "Resume", this );
   m_pResumeButton->setEnabled( false );
   buttonLayout->addWidget( m_pResumeButton );
   buttonLayout->addStretch( 1 );
   m_pPovrayOutputButton = new QPushButton( "Povray Output", this );
   buttonLayout->addWidget( m_pPovrayOutputButton );

   buttonLayout = new QHBoxLayout();
   topLayout->addLayout( buttonLayout );
   m_pSaveButton = new QPushButton( "Save", this );
   m_pSaveButton->setEnabled( false );
   buttonLayout->addWidget( m_pSaveButton );
   buttonLayout->addStretch( 1 );
   QPushButton* closeButton = new QPushButton( "Close", this );
   buttonLayout->addWidget( closeButton );
   m_pRenderWidget->resize( 640, 480 );
   this->adjustSize();
   this->setModal( false );
	
   //povray normal exit
   connect( m_pRenderWidget, SIGNAL( finished(int) ),
                             SLOT( slotRenderingFinished(int) ) );

   //connect( m_pRenderWidget, SIGNAL( progress(int) ),
   //                          SLOT( slotProgress(int) ) );

   //connect( m_pRenderWidget, SIGNAL( lineFinished(int) ),
   //                          SLOT( slotLineFinished(int) ) );

   connect( m_pStopButton, SIGNAL( clicked() ), SLOT( slotStop() ) );
   connect( m_pSuspendButton, SIGNAL( clicked() ), SLOT( slotSuspend() ) );
   connect( m_pResumeButton, SIGNAL( clicked() ), SLOT( slotResume() ) );
   connect( m_pSaveButton, SIGNAL( clicked() ), SLOT( slotSave() ) );
   connect( closeButton, SIGNAL( clicked() ), SLOT( slotClose() ) );
   connect( m_pPovrayOutputButton, SIGNAL( clicked() ), SLOT( slotPovrayOutput() ) );

   m_bRunning = false;

   //qtimer povray render image
   m_pProgressTimer = new QTimer( this );
   connect( m_pProgressTimer, SIGNAL( timeout() ), SLOT( slotUpdateSpeed() ) );
   m_pProgressTimer->setSingleShot( true );

   setWindowTitle( "Render Window" );

   m_height = m_width = 0;
   m_stopped = false;

   m_pPovrayOutputWidget = new PMPovrayOutputWidget();

   connect( m_pRenderWidget, SIGNAL( povrayMessage( const QString& ) ),
      m_pPovrayOutputWidget, SLOT( slotText( const QString& ) ) );
}

PMPovrayWidget::~PMPovrayWidget()
{
   delete m_pPovrayOutputWidget;
}

bool PMPovrayWidget::render( const QByteArray& scene, const PMRenderMode& m,
                             const QUrl &documentURL )
{
   bool updateSize = ( m_height != m.height() ) || ( m_width != m.width() );
   m_height = m.height();
   m_width = m.width();
   m_bRunning = false;
   m_pPovrayOutputWidget->slotClear();
   m_stopped = false;

   m_pRenderWidget->setFixedSize( m_width, m_height );
   QSize maxSize( m_width + m_pScrollView->frameWidth() * 2,
                  m_height + m_pScrollView->frameWidth() * 2 );
   m_pScrollView->setMaximumSize( maxSize );

   if( updateSize )
   {
      int w, h;

//TODO PORT QT    MainWindow mainWindow;
      //example
      //const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mainWindow);
     // mainWindow.resize(availableGeometry.width() / 3, availableGeometry.height() / 2);5
     // w = maxSize.width() + 2;
     // h = maxSize.height() + m_pSaveButton->sizeHint().height() * 8;
             // + QDialog::spacingHint() * 6;
//TODO PORT QT5
      if( m_pProgressLabel->sizeHint().height() > m_pProgressBar->sizeHint().height() )
         h = m_pProgressLabel->sizeHint().height();
      else
         h = m_pProgressBar->sizeHint().height();

      w = 16;
      h += 16;

      QScreen *screen = QGuiApplication::primaryScreen();
      QRect  dw = screen->geometry();

      ///QRect dw = KGlobalSettings::desktopGeometry(this);
      if( w > dw.width() ) w = dw.width();

      if( h > dw.height() ) h = dw.height();

      resize( w, h );
   }

   if( m_pRenderWidget->render( scene, m, documentURL ) )
   {
      m_bRunning = true;
      m_pProgressBar->setValue( 0 );
      m_pProgressBar->show();
      m_pProgressLabel->setText( "running" );
      m_pStopButton->setEnabled( true );
      m_pSuspendButton->setEnabled( true );
      m_pResumeButton->setEnabled( false );
      m_pSaveButton->setEnabled( false );

      m_lastSpeedTime = QTime();
      //m_pProgressTimer->setSingleShot( true );
      m_pProgressTimer->start( timerIntervall );
      m_speedInfo = false;
      m_speed = 0;
      m_line = 0;
      m_immediateUpdate = false;
      showSpeed( 0 );
   }

   return m_bRunning;
}

void PMPovrayWidget::slotStop()
{
   m_stopped = true;
   m_pRenderWidget->killRendering();
   m_pSaveButton->setEnabled( true );
}

void PMPovrayWidget::slotSuspend()
{
   m_pRenderWidget->suspendRendering();
   m_pSuspendButton->setEnabled( false );
   m_pResumeButton->setEnabled( true );
   m_pSaveButton->setEnabled( true );

   m_pProgressTimer->stop();
   m_lastSpeedTime = QTime();
   m_speedInfo = false;
   m_immediateUpdate = false;
   m_pProgressLabel->setText( tr( "suspended" ) );
}

void PMPovrayWidget::slotResume()
{
   m_pRenderWidget->resumeRendering();
   m_pSuspendButton->setEnabled( true );
   m_pResumeButton->setEnabled( false );
   m_pSaveButton->setEnabled( false );
   m_pProgressTimer->start( timerIntervall );
   showSpeed( m_speed );
}

void PMPovrayWidget::slotClose()
{
   hide();
}

void PMPovrayWidget::slotSave()
{
   QTemporaryFile* tempFile = 0;
   QFile* file = 0;
   bool ok = true;

   QUrl url = QFileDialog::getSaveFileUrl( this, tr( "Save Image File" ), QUrl() );  // QUrl(), KImageIO::pattern( KImageIO::Writing ) );
   if( url.isEmpty() )
      return;
   //if( !PMShell::overwriteURL( url ) )
     // return;

   if( !url.isValid() )
   {
      QMessageBox::warning( this, tr( "warning" ) ,
            tr( "Malformed URL" ),
            QMessageBox::Ok, QMessageBox::NoButton);
      return;
   }

   ///QMimeDatabase mimeDatabase;
   const QFileInfo fi( url.fileName() );
   //const QMimeType mime = mimeDatabase.mimeTypeForFile( fi );

   QByteArray format = fi.completeSuffix().toLatin1();
   //QString mimeType = mime.name();//QMimeType::findByUrl( url.fileName(), 0, url.isLocalFile(), true )->name();
   //QStringList formats = mime.suffixes();
   //if ( !formats.isEmpty() )
   //   format = formats.first().toLatin1();

   if( url.isLocalFile() )
   {
      // Local file
      file = new QFile( url.path() );
      if( !file->open( QIODevice::WriteOnly ) )
         ok = false;
   }
   else
   {
      // Remote file
      // provide a temp file
      tempFile = new QTemporaryFile();
      if( !tempFile->open() )
         ok = false;
      else
         file = tempFile;
   }

   if( ok )
   {
      QImageWriter iio( file, format );
      ok = iio.write( m_pRenderWidget->image() );

      if( ok )
      {
         if( tempFile )
         {
            tempFile->flush();
            // eticre todo ok = KIO::NetAccess::upload( tempFile->fileName(), url, this );
            file = 0;
         }
         else
            file->close();
      }
      else
         QMessageBox::warning( this, tr( "warning" ) ,
                  tr( "Could not correctly write the image.\n"
                  "Wrong image format?" ),
                  QMessageBox::Ok, QMessageBox::NoButton);
   }
   else
      QMessageBox::warning( this, tr( "warning" ) ,
            tr( "Could not write the image.\n"
                "Permission denied." ),
            QMessageBox::Ok, QMessageBox::NoButton);


   delete file;
   if (file != tempFile)
      delete tempFile;
}

void PMPovrayWidget::slotPovrayOutput()
{
   m_pPovrayOutputWidget->show();
}

void PMPovrayWidget::slotRenderingFinished( int returnStatus )
{
   qDebug() << "Povray exited with status " << returnStatus;
   m_bRunning = false;

   if( returnStatus == 0 ) m_pSaveButton->setEnabled( true );

   m_pStopButton->setEnabled( false );
   m_pSuspendButton->setEnabled( false );
   m_pResumeButton->setEnabled( false );
   m_pProgressLabel->setText( tr( "finished" ) );
   m_pProgressTimer->stop();

   if( ( returnStatus != 0 ) && !m_stopped )
   {
      QMessageBox::warning( this, tr( "warning" ) ,
            tr( "Povray exited abnormally (%1).\n"
                "See the povray output for details." ),
            QMessageBox::Ok, QMessageBox::NoButton);

   }
   else if( m_pRenderWidget->povrayOutput().contains( "ERROR" ) )
   {
      QMessageBox::warning( this, tr( "warning" ) ,
            tr( "There were errors while rendering.\n"
                "See the povray output for details." ),
            QMessageBox::Ok, QMessageBox::NoButton);
   }
}

void PMPovrayWidget::slotProgress( int i )
{
   m_pProgressBar->setValue( i );
}

void PMPovrayWidget::slotLineFinished( int line )
{
   m_speedInfo = true;
   QTime ct = QTime::currentTime();

   if( !m_lastSpeedTime.isNull() )
   {
      int msecs = m_lastSpeedTime.msecsTo( ct );
      if( msecs < 1 )
         msecs = 1;

      double g = 1.0 / ( ( double ) msecs / 500.0 + 1.0 );
      m_speed = g * m_speed + 1000 * ( 1.0 - g ) * m_width * ( line - m_line ) / msecs;
   }

   if( m_immediateUpdate )
   {
      m_immediateUpdate = false;
      showSpeed( m_speed );
      m_pProgressTimer->start( timerIntervall );
      m_speedInfo = false;
   }

   m_lastSpeedTime = ct;
   m_line = line;
}

void PMPovrayWidget::slotUpdateSpeed()
{
   if( m_speedInfo )
   {
      showSpeed( m_speed );
      m_pProgressTimer->start( timerIntervall );
      m_speedInfo = false;
   }
   else
      m_immediateUpdate = true;
}

void PMPovrayWidget::showSpeed( double pps )
{
   QString num;
   if( pps >= 1000000 )
   {
      num.setNum( pps / 100000, 'g', 3 );
      num += 'M';
   }
   else if( pps >= 1000 )
   {
      num.setNum( pps / 1000, 'g', 3 );
      num += 'K';
   }
   else
      num.setNum( pps, 'g', 3 );

   m_pProgressLabel->setText( "running, %1 pixels/second" + num );
}

#include "pmpovraywidget.moc"
