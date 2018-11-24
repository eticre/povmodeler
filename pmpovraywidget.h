/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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

#ifndef PMPOVRAYWIDGET_H
#define PMPOVRAYWIDGET_H



#include <QDialog>
#include <QDateTime>
//Added by qt3to4:
#include <QLabel>

class PMPovrayRenderWidget;
class PMPovrayOutputWidget;
class PMRenderMode;

class QProgressBar;
class QPushButton;
class QLabel;
class QScrollArea;
class QUrl;

/**
 * Widget with toolbar, statusbar and a @ref PMPovrayRenderWidget
 */
class PMPovrayWidget : public QDialog
{
   Q_OBJECT
public:
   /**
    * Standard constructor
    */
   PMPovrayWidget();
   /**
    * Destructor
    */
   virtual ~PMPovrayWidget();
   /**
    * Starts rendering for the povray code in the byte array with
    * render mode m.
    * @see PMRenderMode
    */
   bool render( const QByteArray& scene, const PMRenderMode& m,
                const QUrl &documentURL );

public slots:
   void slotStop();
   void slotSuspend();
   void slotResume();
   void slotClose();
   void slotSave();
   void slotPovrayOutput();

protected slots:
   void slotRenderingFinished( int returnStatus );
   void slotProgress( int i );
   void slotLineFinished( int line );
   void slotUpdateSpeed();
   
protected:
   void showSpeed( double pps );
   
private:
   PMPovrayRenderWidget* m_pRenderWidget;
   PMPovrayOutputWidget* m_pPovrayOutputWidget;
   QPushButton* m_pStopButton;
   QPushButton* m_pSuspendButton;
   QPushButton* m_pResumeButton;
   QPushButton* m_pSaveButton;
   QPushButton* m_pPovrayOutputButton;
   QProgressBar* m_pProgressBar;
   QLabel* m_pProgressLabel;
   QScrollArea* m_pScrollView;
   int m_height, m_width;
   bool m_bRunning;
   QTime m_lastSpeedTime;
   QTimer* m_pProgressTimer;
   bool m_speedInfo;
   bool m_immediateUpdate;
   float m_speed;
   int m_line;
   bool m_stopped;
};

#endif
