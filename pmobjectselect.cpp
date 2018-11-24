/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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

#include "pmobjectselect.h"
#include <QBuffer>
#include <QImage>
#include "pmdefaults.h"
#include <QDebug>
#include <QVBoxLayout>

//eticre add preview


PMListBoxObject::PMListBoxObject( QListWidget* listbox, PMObject* obj, QListWidgetItem* after )
   : QListWidgetItem( QIcon::fromTheme( obj->pixmap() ), checkName( obj->name() ) )
{
   listbox->insertItem( listbox->row( after ) + 1, this );
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( QListWidget* listbox, PMObject* obj )
   : QListWidgetItem( QIcon::fromTheme( obj->pixmap() ), checkName( obj->name() ), listbox )
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( PMObject* obj )
   : QListWidgetItem( QIcon::fromTheme( obj->pixmap() ), checkName( obj->name() ) )
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( QListWidget* listbox, PMObject* obj,
                                  const QString& text, QListWidgetItem* after )
      : QListWidgetItem( QIcon::fromTheme( obj->pixmap() ), text )
{
   listbox->insertItem( listbox->row( after ) + 1, this );
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( QListWidget* listbox, PMObject* obj,
                                  const QString& text )
      : QListWidgetItem( QIcon::fromTheme( obj->pixmap() ).pixmap( 16, 16 ), text, listbox)
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( PMObject* obj, const QString& text )
      : QListWidgetItem( QIcon::fromTheme( obj->pixmap() ).pixmap( 16, 16 ), text )
{
   m_pObject = obj;
}

QString PMListBoxObject::checkName( const QString& text )
{
   if( text.isEmpty() )
      return "(unnamed)";
   return text;
}

PMListBoxObject::~PMListBoxObject()
{
}


QSize PMObjectSelect::s_size = QSize( 200, 300 );


PMObjectSelect::PMObjectSelect( QWidget* parent, bool modal )
    : QDialog( parent )
{
   //m_pWidget = new QWidget( this );
   setWindowTitle( tr( "Choose Object" ) );
   m_pSelectedObject = nullptr;
   m_pWidget = new QWidget();
   m_pListWidget = new QListWidget( m_pWidget );
   m_pListWidget->adjustSize();

   int h = m_pListWidget->size().height();
   int w = m_pListWidget->size().width();
   if( h<300 )
       h = h + ( 300-h );

   m_pLabel = new QLabel ( m_pWidget );
   m_image = QImage( QSize( 300 , 300 ) , QImage::Format_Mono );
   m_image.fill( QColor( Qt::white ).rgb() );

   this->setGeometry( 250 , 75 , w + 320 , h + 0 );
   m_pWidget->setGeometry( 1 , 1 , w + 316 , h + 48 );
   m_pLabel->setGeometry( w+5 , 0 , 300 , 300 );
   m_pLabel->setPixmap( QPixmap::fromImage( m_image ) );

   //eticre create a preview, save qimage and visualize in m_pLabel
   //m_pPart = new PMPart( this , this ,  false , true ); error
   m_pRenderPreviewSelect = new PMPovrayRenderWidget();
   m_pEditBase = new PMDialogEditBase( m_pRenderPreviewSelect );
   m_pEditBase->b_onlyPreview = true;

   buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok|QDialogButtonBox::Cancel );

   QVBoxLayout *mainLayout = new QVBoxLayout;
   setLayout( mainLayout );
   //mainLayout->addWidget(mainWidget);
   //okButton = buttonBox->button(QDialogButtonBox::Ok);
   //okButton->setDefault(true);
   //okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
   user1Button = new QPushButton;
   buttonBox->addButton( user1Button, QDialogButtonBox::ActionRole );
   user1Button->setVisible( false );
   connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
   connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
   //PORTING SCRIPT: WARNING mainLayout->addWidget(buttonBox) must be last item in layout. Please move it.
   mainLayout->addWidget( m_pWidget );
   mainLayout->addWidget( buttonBox );
   setModal( modal );
   user1Button->setText( "Preview" );

   i_listRows = m_pListWidget->count();
   i_previousRow = 0;



   /**eticre preview**/
   connect( this , SIGNAL( user1Clicked() ) , this , SLOT( slotPreview() ) );

   connect( this->m_pRenderPreviewSelect , SIGNAL( progress100( QImage ) ),
            this , SLOT( slotProgress( QImage ) ) );

   connect( m_pListWidget, SIGNAL( itemSelectionChanged() ),
            SLOT( slotSelectionChanged() ) );

   connect( m_pListWidget, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ),
            SLOT( slotDoubleClicked( QListWidgetItem* ) ) );

   //okButton->setEnabled(false);
   buttonBox->button( QDialogButtonBox::Ok )->setDisabled( true );
}

PMObjectSelect::~PMObjectSelect()
{
   s_size = size();
   m_pPart = nullptr;
   m_pEditBase = nullptr;
   m_pRenderPreviewSelect = nullptr;
   m_pListWidget = nullptr;
   m_pWidget = nullptr;
}

void PMObjectSelect::addObject( PMObject* obj )
{
   m_pListWidget->addItem( new PMListBoxObject( obj ) );
}

int PMObjectSelect::selectObject( PMObject* link, const QString& t,
                                  PMObject* & obj, QWidget* parent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;

   do
   {
      scene = last->parent();
      if( scene )
      {
         if( scene->type() == "Scene" )
         {
            last = last->prevSibling();
            stop = true;
            found = true;
         }
         else
            last = last->parent();
      }
      else
         stop = true;
   }
   while( !stop );

   if( found )
   {
      PMObjectSelect s( parent );

      PMObject* o = scene->firstChild();
      bool l = false;

      while( o && !l && last )
      {
         if( o->type() == t )
            s.m_pListWidget->addItem( new PMListBoxObject( o ) );

         if( o == last )
            l = true;
         else
            o = o->nextSibling();
      }

      int result = s.exec();
      if( result == Accepted )
         obj = s.selectedObject();

      return result;
   }
   else
      qCritical() << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

int PMObjectSelect::selectObject( PMObject* link,
                                  const QStringList& t,
                                  PMObject* & obj, QWidget* parent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;

   do
   {
      scene = last->parent();
      if( scene )
      {
         if( scene->type() == "Scene" )
         {
            last = last->prevSibling();
            stop = true;
            found = true;
         }
         else
            last = last->parent();
      }
      else
         stop = true;
   }
   while( !stop );

   if( found )
   {
      PMObjectSelect s( parent );

      PMObject* o = scene->firstChild();
      bool l = false;

      while( o && !l && last )
      {
         if( t.indexOf( o->type() ) >= 0 )
            s.m_pListWidget->addItem( new PMListBoxObject( o ) );

         if( o == last )
            l = true;
         else
            o = o->nextSibling();
      }

      int result = s.exec();
      if( result == Accepted )
         obj = s.selectedObject();

      return result;
   }
   else
      qCritical() << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

int PMObjectSelect::selectDeclare( PMObject* link, const QString& declareType,
                                   PMObject* & obj, QWidget* parent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;

   do
   {
      scene = last->parent();
      if( scene )
      {
         if( scene->type() == "Scene" )
         {
            last = last->prevSibling();
            stop = true;
            found = true;
         }
         else
            last = last->parent();
      }
      else
         stop = true;
   }
   while( !stop );

   if( found )
   {
      PMObjectSelect s( parent );

      PMObject* o = scene->firstChild();
      PMDeclare* decl;

      bool l = false;

      while( o && !l && last )
      {
         if( o->type() == "Declare" )
         {
            //decl = ( PMDeclare* ) o;
            decl = static_cast<PMDeclare*>(o);

            if( decl->declareType() == declareType )
               s.m_pListWidget->addItem( new PMListBoxObject( o ) );
         }

         if( o == last )
            l = true;
         else
            o = o->nextSibling();
      }

      int result = s.exec();
      if( result == Accepted )
         obj = s.selectedObject();

      return result;
   }
   else
      qCritical() << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

int PMObjectSelect::selectDeclare( PMObject* link, const QStringList& declareTypes,
                                   PMObject* & obj, QWidget* parent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;

   do
   {
      scene = last->parent();
      if( scene )
      {
         if( scene->type() == "Scene" )
         {
            last = last->prevSibling();
            stop = true;
            found = true;
         }
         else
            last = last->parent();
      }
      else
         stop = true;
   }
   while( !stop );

   if( found )
   {
      PMObjectSelect s( parent );

      PMObject* o = scene->firstChild();
      PMDeclare* decl;

      bool l = false;

      while( o && !l && last )
      {
         if( o->type() == "Declare" )
         {
            //decl = ( PMDeclare* ) o;
            decl = static_cast<PMDeclare*>(o);
            if( declareTypes.indexOf( decl->declareType() ) >= 0 )
               s.m_pListWidget->addItem( new PMListBoxObject( o ) );
         }

         if( o == last )
            l = true;
         else
            o = o->nextSibling();
      }

      int result = s.exec();
      if( result == Accepted )
         obj = s.selectedObject();

      return result;
   }
   else
      qCritical() << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

QStringList PMObjectSelect::selectFunctions( PMObject* link )
{
   PMObject* last = link;
   PMObject* scene;
   QStringList functionsList;
   bool stop = false;
   bool found = false;

   do
   {
      scene = last->parent();
      if( scene )
      {
         if( scene->type() == "Scene" )
         {
            last = last->prevSibling();
            stop = true;
            found = true;
         }
         else
            last = last->parent();
      }
      else
         stop = true;
   }
   while( !stop );

   if( found )
   {

      PMObject* o = scene->firstChild();
      PMDeclare* decl;

      bool l = false;

      while( o && !l && last )
      {
         if( o->type() == "Declare" )
         {
            //decl = ( PMDeclare* ) o;
            decl = static_cast<PMDeclare*>(o);
            if( decl->is_a_function )
                functionsList.append( decl->id() );
         }

         if( o == last )
            l = true;
         else
            o = o->nextSibling();
      }

      return functionsList;
   }
   else
      qCritical() << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return functionsList;
}


void PMObjectSelect::slotSelectionChanged()
{
   QList<QListWidgetItem*> selected = m_pListWidget->selectedItems();
   if ( selected.isEmpty() )
       return;

   //m_pSelectedObject = ( ( PMListBoxObject* ) selected.at( 0 ) )->object();
   m_pSelectedObject = ( static_cast<PMListBoxObject*>( selected.at( 0 ) ) )->object();

   buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );

   QImage img = mapPreview[m_pListWidget->currentRow()];
   if( !img.isNull() )
       m_pLabel->setPixmap( QPixmap::fromImage( img ) );
   else
   {
       m_pLabel->clear();
       m_pLabel->setText( "No Preview Image" );
   }

   PMObject* global;
   PMObject* local;
   m_pEditBase->m_pDisplayedObject = m_pSelectedObject;
   m_pEditBase->findTextures( global , local );
   if( global )
       user1Button->setVisible( true );
   else
       user1Button->setVisible( false );

}

void PMObjectSelect::slotDoubleClicked( QListWidgetItem* )
{
   QList<QListWidgetItem*> selected = m_pListWidget->selectedItems();
   if ( selected.isEmpty() )
       return;

   //m_pSelectedObject = ( ( PMListBoxObject* ) selected.at(0) )->object();
   m_pSelectedObject = ( static_cast<PMListBoxObject*>( selected.at( 0 ) ) )->object();

   buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
   accept();
}


void PMObjectSelect::slotPreview()
{
    m_pEditBase->m_pDisplayedObject = m_pSelectedObject;
    m_pEditBase->setPreviewSize( 300 );
    m_pEditBase->slotTexturePreview();
    if ( m_pEditBase->qba_scene_link_preview.length() != 0 )//eticre + .length()
        m_pRenderPreviewSelect->render(
                 m_pEditBase->qba_scene_link_preview ,
                 m_pEditBase->p_mode_link_preview, m_pPart->url() );

    //m_pEditBase->setPreviewSize( c_defaultTPSize );
}

void PMObjectSelect::slotProgress( QImage img )
{
    if( !img.isNull() )
    {
        m_pLabel->setPixmap( QPixmap::fromImage( img ) );
        if( m_pListWidget->currentRow() >= 0 )
            mapPreview[m_pListWidget->currentRow()] = img;//internal use
        else
            emit ok_image( img );//external request of preview image without m_pListwidget
    }
}

#include "pmobjectselect.moc"
