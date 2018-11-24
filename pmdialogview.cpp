/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2003 by Andreas Zehender
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


#include "pmdialogview.h"
#include "pmdialogeditbase.h"
#include "pmallobjects.h"
#include "pmdebug.h"
//#include "pmfactory.h"
#include "pmdatachangecommand.h"
#include "pmpart.h"
#include "pmdocumentationmap.h"

#include <QMessageBox>

#include <QDesktopServices>
#include <QUrl>
#include <QGroupBox>
#include <QScrollArea>
#include <QScrollBar>

#include <QVBoxLayout>
#include <QHBoxLayout>



PMDialogEditContent::PMDialogEditContent( QWidget* parent )
      : QScrollArea( parent )
{
   m_pContents = nullptr;
   setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
   setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
   setFrameStyle( Panel | Sunken );
   setLineWidth( 1 );
   setWidgetResizable(true);
}

void PMDialogEditContent::setContents( QWidget* wid )
{
   if( m_pContents )
      takeWidget();

   m_pContents = wid;

   if( m_pContents )
   {
        m_pContents->setSizePolicy( QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
      setWidget( m_pContents );
   }
}

PMDialogView::PMDialogView( PMPart* part, QWidget* parent )
      : PMViewBase( parent )
{
   m_pDialogEditBase = nullptr;
   m_unsavedData = false;
   m_pHelper = new PMDialogEditContent( this );
   m_pHelper->show();
   m_pPart = part;

   m_pLayout = new QVBoxLayout( this );
   QHBoxLayout* labelLayout = new QHBoxLayout();
   m_pLayout->addLayout( labelLayout );
   m_pPixmapLabel = new QLabel( this );
   m_pObjectTypeLabel = new QLabel( this );
   labelLayout->addWidget( m_pPixmapLabel );
   labelLayout->addWidget( m_pObjectTypeLabel );
   labelLayout->addStretch();

   m_pLayout->addWidget( m_pHelper, 2 );
   m_pLayout->addStretch();

   QHBoxLayout* buttonLayout = new QHBoxLayout();
   m_pLayout->addLayout( buttonLayout );
   m_pHelpButton = new QPushButton( "help", this );
   buttonLayout->addWidget( m_pHelpButton );
   connect( m_pHelpButton, SIGNAL( clicked() ), this, SLOT( slotHelp() ) );
   m_pHelpButton->setEnabled( false );

   m_pApplyButton = new QPushButton( "apply", this );
   buttonLayout->addWidget( m_pApplyButton );
   connect( m_pApplyButton, SIGNAL( clicked() ), this, SLOT( slotApply() ) );
   m_pApplyButton->setEnabled( false );

   buttonLayout->addStretch();

   m_pCancelButton = new QPushButton( "cancel", this );;
   buttonLayout->addWidget( m_pCancelButton );
   connect( m_pCancelButton, SIGNAL( clicked() ), this, SLOT( slotCancel() ) );
   m_pCancelButton->setEnabled( false );

   m_pLayout->activate();

   connect( part, SIGNAL( refresh() ),
                  SLOT( slotRefresh() ) );

   connect( part, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
                  SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   connect( part, SIGNAL( clear() ),
                  SLOT( slotClear() ) );

   connect( part, SIGNAL( aboutToRender() ),
                  SLOT( slotAboutToRender() ) );

   connect( part, SIGNAL( aboutToSave() ),
                  SLOT( slotAboutToRender() ) );

   connect( this, SIGNAL( objectChanged( PMObject*, const int, QObject* ) ),
            part, SLOT( slotObjectChanged( PMObject*, const int, QObject* ) ) );

   displayObject( m_pPart->activeObject() );
}



PMDialogView::~PMDialogView()
{
   emit destroyed( this );
}

void PMDialogView::slotObjectChanged( PMObject* obj, const int mode, QObject* sender )
{
   if( sender == this )
      return;
   if( mode & PMCNewSelection )
   {
      if( m_pDialogEditBase )
         if( m_pDialogEditBase->displayedObject() )
            if( m_unsavedData )
               if( shouldSaveData() )
                  slotApply();

      displayObject( obj );
   }
   if( mode & ( PMCSelected | PMCDeselected ) )
   {
      if( m_pDialogEditBase )
         if( m_pDialogEditBase->displayedObject() )
            if( m_unsavedData )
               if( shouldSaveData() )
                  slotApply();

      displayObject( nullptr );
   }
   if( mode & PMCRemove )
   {
      if( m_pDialogEditBase )
      {
         if( m_pDialogEditBase->displayedObject() == obj )
         {
            displayObject( nullptr );
         }
      }
   }
   if( mode & ( PMCData | PMCDescription ) )
   {
      if( m_pDialogEditBase )
      {
         if( m_pDialogEditBase->displayedObject() )
         {
            if( m_pDialogEditBase->displayedObject() == obj )
            {
               displayObject( obj, mode & PMCDescription );
               m_unsavedData = false;
               m_pApplyButton->setEnabled( false );
               m_pCancelButton->setEnabled( false );
            }
         }
      }
   }
   if( mode & PMCControlPointSelection )
   {
      if( m_pDialogEditBase )
         m_pDialogEditBase->updateControlPointSelection();
   }
}

bool PMDialogView::shouldSaveData()
{
    bool ret = false;
    QMessageBox msgBox;
    msgBox.setText( "The document has been modified." );
    msgBox.setInformativeText( "Do you want to save your changes?" );
    msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard );
    msgBox.setDefaultButton( QMessageBox::Save );
    if( msgBox.exec() == QMessageBox::Save ) ret = true;
    return ret;

   //return ( KMessageBox::questionYesNo(
   //   this, tr( "This object was modified.\n\nSave changes?" ),
    //  tr( "Unsaved Changes" ), KStandardGuiItem::save(), KStandardGuiItem::discard() ) == KMessageBox::Yes );
}

void PMDialogView::slotRefresh()
{
   if( m_pDialogEditBase )
      m_pDialogEditBase->redisplay();
}

void PMDialogView::slotClear()
{
   displayObject( nullptr );
}

void PMDialogView::slotApply()
{
   if( m_pDialogEditBase )
   {
      PMObject* obj = m_pDialogEditBase->displayedObject();
      if( obj )
      {
         if( m_pDialogEditBase->isDataValid() )
         {
            PMDataChangeCommand* cmd;

            obj->createMemento();
            m_pDialogEditBase->saveData();
            cmd = new PMDataChangeCommand( obj->takeMemento() );
            m_pPart->executeCommand( cmd );

            m_pApplyButton->setEnabled( false );
            m_pCancelButton->setEnabled( false );
            m_unsavedData = false;
         }
      }
   }
}

void PMDialogView::slotHelp()
{
   if( m_pDialogEditBase && m_pDialogEditBase->displayedObject() )
   {
      QString url = PMDocumentationMap::theMap()->documentation( m_pDialogEditBase->displayedObject()->className() );
      if( !url.isEmpty() )
      {
         QDesktopServices::openUrl( QUrl( url ) );
      }
   }
}

void PMDialogView::slotCancel()
{
   m_pCancelButton->setEnabled( false );
   m_pApplyButton->setEnabled( false );
   m_unsavedData = false;

   if( m_pDialogEditBase )
   {
      bool b = m_pDialogEditBase->signalsBlocked();
      m_pDialogEditBase->blockSignals( true );
      m_pDialogEditBase->redisplay();
      m_pDialogEditBase->blockSignals( b );
      slotSizeChanged();
   }
}

void PMDialogView::slotDataChanged()
{
//   qDebug() << "PMDialogView::slotDataChanged\n";
   m_pApplyButton->setEnabled( true ) ;
   m_pCancelButton->setEnabled( true );
   m_unsavedData = true;
}

void PMDialogView::slotSizeChanged()
{
   // force recalculating of the layout //eticre fail return at default preview size!
   if( m_pDialogEditBase )
      if( m_pDialogEditBase->layout() )
         m_pDialogEditBase->layout()->activate();
}

void PMDialogView::slotControlPointSelectionChanged()
{
   if( m_pDialogEditBase && m_pDialogEditBase->displayedObject() )
      emit objectChanged( m_pDialogEditBase->displayedObject(),
                          PMCControlPointSelection, this );
}

void PMDialogView::displayObject( PMObject* obj, bool updateDescription )
{
   PMDialogEditBase* old = nullptr;

   if( !obj )
   {
      if( m_pDialogEditBase )
         old = m_pDialogEditBase;

      m_pDialogEditBase = new PMDialogEditBase( m_pHelper->widget() );
      m_pDialogEditBase->createWidgets();
      m_pHelper->setContents( m_pDialogEditBase );
      m_pPixmapLabel->setText( "" );
      m_pObjectTypeLabel->setText( "" );
   }
   else
   {
      bool newWidget = true;
      if( m_pDialogEditBase )
      {
         if( m_pDialogEditBase->displayedObject() )
         {
            if( obj->type() == m_pDialogEditBase->displayedObject()->type())// &&
                    //m_pDialogEditBase->displayedObject()->type() != "Pattern")
            {
               // current widget displays object of the same type
               // reuse the widget except for Pattern because the pigment_pattern
               // is used only with Normal
                newWidget = false;
            }
         }
      }

      if( newWidget )
      {
         // first create the new widget, then delete the old one.
         if( m_pDialogEditBase )
            old = m_pDialogEditBase;
         m_pDialogEditBase = obj->editWidget( m_pHelper->widget() );
         m_pDialogEditBase->setPart( m_pPart );
         m_pDialogEditBase->createWidgets();
         m_pHelper->setContents( m_pDialogEditBase );

         if( m_pDialogEditBase )
         {
            connect( m_pDialogEditBase,
                     SIGNAL( dataChanged() ),
                     this, SLOT( slotDataChanged() ) );

            connect( m_pDialogEditBase,
                     SIGNAL( sizeChanged() ),
                     this, SLOT( slotSizeChanged() ) );

            connect( m_pDialogEditBase,
                     SIGNAL( aboutToRender() ),
                     this, SLOT( slotAboutToRender() ) );

            connect( m_pDialogEditBase,
                     SIGNAL( controlPointSelectionChanged() ),
                     SLOT( slotControlPointSelectionChanged() ) );
         }
      }
   }

   if( m_pDialogEditBase )
   {
      bool b = m_pDialogEditBase->signalsBlocked();
      m_pDialogEditBase->blockSignals( true );
      m_pDialogEditBase->displayObject( obj );
      m_pDialogEditBase->blockSignals( b );

      m_pHelpButton->setEnabled( !m_pDialogEditBase->helpTopic().isNull() );

      if( !m_pDialogEditBase->isVisible() )
         m_pDialogEditBase->show();
      if( obj && updateDescription )
      {
         m_pPixmapLabel->setPixmap( QIcon::fromTheme( obj->pixmap() ).pixmap( 16, 16) );
         m_pObjectTypeLabel->setText( obj->description() );
      }
      slotSizeChanged();
   }

   if( old )
      delete old;

   m_pApplyButton->setEnabled( false );
   m_pCancelButton->setEnabled( false );
   m_pHelpButton->setEnabled( !PMDocumentationMap::theMap()->povrayDocumentationPath().isEmpty() );
   m_unsavedData = false;
}

void PMDialogView::keyPressEvent( QKeyEvent* ev )
{
   if( ( ev->key() == Qt::Key_Return ) || ( ev->key() == Qt::Key_Enter ) )
      slotApply();
}

void PMDialogView::slotAboutToRender()
{
   if( m_unsavedData )
      if( shouldSaveData() )
         slotApply();
}

QString PMDialogView::description() const
{
   return "Object Properties";
}

QString PMDialogViewFactory::description() const
{
   return "Object Properties";
}

#include "pmdialogview.moc"
