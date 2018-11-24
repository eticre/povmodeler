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

#ifndef PMPOVRAYRENDERWIDGET_H
#define PMPOVRAYRENDERWIDGET_H


#include "pmrendermode.h"
#include "pmdragwidget.h"

#include <QImage>
#include <QBuffer>
#include <QPixmap>
#include <QPaintEvent>
#include <QProcess>

class QUrl;
class QTemporaryFile;

/**
 * Widget that calls povray to render a scene and
 * displays the output.
 */
class PMPovrayRenderWidget : public PMDragWidget
{
   Q_OBJECT
public:
   /**
    * Standard constructor
    */
   PMPovrayRenderWidget( QWidget* parent = nullptr );
   /**
    * destructor
    */
   virtual ~PMPovrayRenderWidget();

   /**
    * Starts rendering for the povray code in the byte array with
    * render mode m.
    * @see PMRenderMode
    */
   bool render( const QByteArray& scene, const PMRenderMode& m,
                const QUrl &documentURL );

   /**
    * Returns the povray text output
    */
   QString povrayOutput() const { return m_povrayOutput; }
   /**
    * Returns the rendered image
    */
   QImage image() const { return m_image; }
   
   static void saveConfig();
   static void restoreConfig();

   /**
    * Returns the povray command
    */
   static QString povrayCommand() { return s_povrayCommand; }
   /**
    * Sets the povray command
    */
   static void setPovrayCommand( const QString& c ) { s_povrayCommand = c; }
   /**
    * Returns the library paths
    */
   static QStringList povrayLibraryPaths() { return s_libraryPaths; }
   /**
    * Sets the library paths
    */
   static void setPovrayLibraryPaths( const QStringList& slist )
   { s_libraryPaths = slist; }
   virtual QSize sizeHint() const;

   virtual void startDrag();
   
signals:
   /**
    * Emitted when rendering has finished
    */
   void finished( int exitStatus );
   /**
    * Provides progress information
    */
   void progress( int percent );
   /**
    * Provides progress imformation
    */
   void lineFinished( int line );
   /**
    * The povray output text
    */
   void povrayMessage( const QString& msg );
   /**
    * The preview output image
    */
   void progress100( QImage q_img );

public slots:
   /**
    * Kills rendering
    */
   void killRendering();
   /**
    * Suspends rendering
    */
   void suspendRendering();
   /**
    * Resumes rendering
    */
   void resumeRendering();

protected slots:
   /**
    * Receive povray messages
    */
   //void slotPovrayMessage( QProcess* proc, char* buffer, int buflen );
   /**
    * Receive rendered image
    */
   //void slotPovrayImage(const char* buffer, QByteArray *btr);
   /**
    * Called when output has been written to the povray process
    */
   //void slotWroteStdin( K3Process* proc );
   /**
    * Called when the process has finished
    */
   void slotRenderingFinished();
   
protected:
   virtual void paintEvent( QPaintEvent* );
      
private:
   void setPixel( int x, int y, uint c );
   void cleanup();
   
   QProcess* m_pProcess;
   
   bool m_bSuspended;
   PMRenderMode m_renderMode;
   QImage m_image;
   bool m_rcvHeader;
   unsigned char m_header[18];
   int m_rcvHeaderBytes;
   int m_skipBytes;
   int m_bytespp;
   int m_rcvPixels;
   int m_progress;
   unsigned char m_restBytes[4];
   int m_numRestBytes;
   int m_line;
   int m_column;
   QPixmap m_pixmap;
   bool m_bPixmapUpToDate;
   QString m_povrayOutput;
   QTemporaryFile* m_pTempFile;
   QString m_args;
   QString m_split_args;
   //const QList<QByteArray>& K3Process::args();
   //QList<QByteArray> povray_command_args;
   QStringList m_list;
   bool is_preview;
   static QString s_povrayCommand;
   static QStringList s_libraryPaths;
};

#endif
