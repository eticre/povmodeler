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

#include "pmpart.h"
#include "pmdefaults.h"
#include "pmpovrayrenderwidget.h"
#include "pmpovrayoutputwidget.h"
#include "pmpovray35format.h"
#include "pmserializer.h"
#include "pmcolor.h"
#include "pmrecursiveobjectiterator.h"
#include "pmdeclare.h"
#include "pmdialogeditbase.h"


#include <QCheckBox>
#include <QBuffer>

//Added by qt3to4:
#include <QVBoxLayout>
//#include <QFrame>
#include <QHBoxLayout>
#include <QTextStream>
#include <QBoxLayout>

#include <QSettings>
#include <QMessageBox>

int PMDialogEditBase::s_previewSize = c_defaultTPSize;
bool PMDialogEditBase::s_showSphere = c_defaultTPShowSphere;
bool PMDialogEditBase::s_showCylinder = c_defaultTPShowCylinder;
bool PMDialogEditBase::s_showBox = c_defaultTPShowBox;
bool PMDialogEditBase::s_previewAA = c_defaultTPAA;
int PMDialogEditBase::s_previewAADepth = c_defaultTPAADepth;
double PMDialogEditBase::s_previewAAThreshold = c_defaultTPAAThreshold;
bool PMDialogEditBase::s_showFloor = c_defaultTPShowFloor;
bool PMDialogEditBase::s_showWall = c_defaultTPShowWall;
QColor PMDialogEditBase::s_wallColor1 = c_defaultTPWallColor1;
QColor PMDialogEditBase::s_wallColor2 = c_defaultTPWallColor2;
QColor PMDialogEditBase::s_floorColor1 = c_defaultTPFloorColor1;
QColor PMDialogEditBase::s_floorColor2 = c_defaultTPFloorColor2;
bool PMDialogEditBase::s_previewLocal = false;
double PMDialogEditBase::s_previewGamma = c_defaultTPGamma;


/** Constants for texture preview */
const QString c_wallCode = QString(
   "plane { <0, 0, -1>, -2\n"
   "  pigment { checker %1 %2\n"
   "    scale <0.5, 0.5, 0.5>\n"
   "    translate <0.5, 0, 0>\n"
   "  }\n"
   "}\n" );
const QString c_floorCode = QString(
   "plane { <0, 1, 0>, 0\n"
   "  pigment { checker %1 %2\n"
   "    scale <0.5, 0.5, 0.5>\n"
   "  }\n"
   "}\n" );
const QString c_lightCode = QString(
   "light_source { <-2.5, 3, -1.5>, rgb <1, 1, 1> }\n"
   "light_source { <3, 3, -3>, rgb <0.6, 0.6, 0.6> shadowless }\n" );


const QString c_cameraCode[3] = {
   QString( "camera { location <-1, 1.25, -2> right <1, 0, 0>\n"
            "  look_at <0.0, 0.5, 0> angle 45 }\n" ),
   QString( "camera { location <-1, 2, -3> right <1, 0, 0>\n"
            "  look_at <0.0, 1, 0> angle 45 }\n" ),
   QString( "camera { location <-2, 2.5, -4> right <1, 0, 0>\n"
            "  look_at <0.0, 1.5, 0> angle 45 }\n" )
};
const QString c_sphereCode = QString(
   "sphere { <0, 0.5, 0>, 0.5 translate <0, %1, 0> }\n" );
const QString c_cylinderCode = QString(
   "cylinder { <0, 0, 0>, <0, 1, 0>, 0.5 translate <0, %1, 0> }\n" );
const QString c_boxCode = QString(
   "box { <-0.5, 0, -0.5>, <0.5, 1, 0.5> translate <0, %1, 0> }\n" );
const QString c_globalSettingsCode = QString(
   "global_settings { assumed_gamma %1 }\n" );

PMDialogEditBase::PMDialogEditBase( QWidget* parent )
      : QWidget( parent )
{
   m_pDisplayedObject = nullptr;
   m_pPart = nullptr;
   m_pTexturePreviewWidget = nullptr;
   m_pOutputWidget = nullptr;
   m_pRenderWidget = nullptr;
   m_pRenderFrame = nullptr;
   m_pPreviewLocalBox = nullptr;
   m_pPreviewButton = nullptr;
   m_pOutputButton = nullptr;
   b_onlyPreview = false;
}

PMDialogEditBase::~PMDialogEditBase()
{
   if( m_pOutputWidget )
      delete m_pOutputWidget;
}

void PMDialogEditBase::createWidgets()
{
   m_pTopLayout = new QVBoxLayout( this );
   createTopWidgets();
   createBottomWidgets();

   m_pTopLayout->addStretch();
   m_pTopLayout->activate();
}

void PMDialogEditBase::createBottomWidgets()
{
   m_pTexturePreviewWidget = new QWidget( this );
   m_pTexturePreviewWidget->hide();
   if( !b_onlyPreview )
       m_pTopLayout->addWidget( m_pTexturePreviewWidget );
}

bool PMDialogEditBase::saveData()
{
   if( isDataValid() )
   {
      saveContents();
      return true;
   }
   return false;
}

void PMDialogEditBase::saveContents()
{
}

void PMDialogEditBase::displayObject( PMObject* o )
{
   if( !o ) return;

   bool newObject = ( m_pDisplayedObject != o );
   m_pDisplayedObject = o;

   if( m_pDisplayedObject->isA( "Declare" ) )
   {
       if( reinterpret_cast<PMDeclare*>( m_pDisplayedObject )->is_a_function )
       {
           return;
       }
   }

   PMObject* global;
   PMObject* local;

   // Is object a full texture or inside a full texture
   findTextures( global, local );
   if( global )
   {
      // is the preview widget created?
      if( !m_pRenderWidget )
      {
         QVBoxLayout* vl = new QVBoxLayout( m_pTexturePreviewWidget );
         vl->setSpacing( 1 );
         vl->addSpacing( 10 );
         QFrame* hline = new QFrame( m_pTexturePreviewWidget );
         hline->setFrameStyle( QFrame::HLine | QFrame::Plain );
         hline->setLineWidth( 1 );
         vl->addWidget( hline );
         vl->addWidget( new QLabel( "Texture preview:", m_pTexturePreviewWidget ) );
         m_pRenderFrame = new QWidget( m_pTexturePreviewWidget );
         QVBoxLayout *pRenderFrameVBoxLayout = new QVBoxLayout( m_pRenderFrame );
         pRenderFrameVBoxLayout->setMargin( 0 );
         m_pRenderFrame->show();
         labelPreviewImage = new QLabel(); // visualize preview image
         pRenderFrameVBoxLayout->addWidget( labelPreviewImage );
         m_pRenderFrame->hide();
         vl->addWidget( m_pRenderFrame );
         m_pRenderWidget = new PMPovrayRenderWidget();
         m_pRenderWidget->setHidden( true );// preview in labelPreviewImage

         connect( m_pRenderWidget, SIGNAL( finished( int ) ),
                                   SLOT( slotPreviewFinished( int ) ) );

         m_pPreviewLocalBox = new QCheckBox( tr( "local" ), m_pTexturePreviewWidget );
         m_pPreviewLocalBox->setChecked( s_previewLocal );

         connect( m_pPreviewLocalBox, SIGNAL( toggled( bool ) ),
                                      SLOT( slotPreviewLocal( bool ) ) );

         vl->addWidget( m_pPreviewLocalBox );
         QHBoxLayout* hl = new QHBoxLayout();
         vl->addLayout( hl );
         m_pPreviewButton = new QPushButton( tr( "&Preview" ), m_pTexturePreviewWidget );
         hl->addWidget( m_pPreviewButton );

         connect( m_pPreviewButton, SIGNAL( clicked() ),
                                    SLOT( slotTexturePreview() ) );

         m_pOutputButton = new QPushButton( tr( "Povray Output" ), m_pTexturePreviewWidget );
         hl->addWidget( m_pOutputButton );

         connect( m_pOutputButton, SIGNAL( clicked() ),
                                   SLOT( slotPovrayOutput() ) );

         hl->addStretch( 1 );
         m_pOutputWidget = new PMPovrayOutputWidget();

         connect( m_pRenderWidget, SIGNAL( povrayMessage( const QString& ) ),
                  m_pOutputWidget, SLOT( slotText( const QString& ) ) );

         //connect( this->m_pRenderWidget , SIGNAL( progress100( QImage ) ),
                  //this , SLOT( slotPreviewImage( QImage ) ) );
      }

      m_pTexturePreviewWidget->show();

      if( newObject )
         m_pRenderFrame->hide();

      m_pPreviewLocalBox->setChecked( false );
      m_pPreviewLocalBox->setEnabled( local != global );
      m_pPreviewLocalBox->setChecked( local != global );
      m_pPreviewButton->setEnabled( true );

      if ( b_onlyPreview )
      {
          m_pPreviewButton->setHidden( true );
          m_pPreviewLocalBox->setHidden( true );
          m_pOutputButton->setHidden( true );
      }

   }
   else
   {
      // is the preview widget created?
      if( m_pPreviewButton )
         m_pPreviewButton->setEnabled( false );

      m_pTexturePreviewWidget->hide();
   }

   if( o && !o->getPreviewImage().isNull() )
   {
       labelPreviewImage->setHidden( false );
       m_pRenderFrame->setHidden( false );
       labelPreviewImage->setPixmap( QPixmap::fromImage( o->getPreviewImage() ) );
   }
}

void PMDialogEditBase::setHelp( const QString& anchor )
{
   m_helpTopic = anchor;
}

void PMDialogEditBase::redisplay()
{
   if( m_pDisplayedObject )
      displayObject( m_pDisplayedObject );
}

void PMDialogEditBase::setCheckBox( QCheckBox* box, PMThreeState state )
{
   switch( state )
   {
      case PMTrue:
         box->setChecked( true );
         break;
      case PMFalse:
         box->setChecked( false );
         break;
      case PMUnspecified:
         box->setCheckState( Qt::PartiallyChecked );
         break;
   }
}

PMThreeState PMDialogEditBase::checkBoxState( QCheckBox* box )
{
   PMThreeState st = PMTrue;
   switch( box->checkState() )
   {
      case Qt::Checked:
         st = PMTrue;
         break;
      case Qt::Unchecked:
         st = PMFalse;
         break;
      case Qt::PartiallyChecked:
         st = PMUnspecified;
         break;
   }
   return st;
}

void PMDialogEditBase::setPreviewSize( int size )
{
   if( ( size >= 10 ) && ( size <= 400 ) )
      s_previewSize = size;
}

void PMDialogEditBase::setPreviewAADepth( int d )
{
   if( ( d >= 1 ) && ( d <= 9 ) )
      s_previewAADepth = d;
}

void PMDialogEditBase::saveConfig()
{
   QSettings qset;
   qset.beginGroup( "TexturePreview" );
   qset.setValue( "Size", s_previewSize );
   qset.setValue( "showSphere", s_showSphere );
   qset.setValue( "showCylinder", s_showCylinder );
   qset.setValue( "showBox", s_showBox );
   qset.setValue( "AA", s_previewAA );
   qset.setValue( "AADepth", s_previewAADepth );
   qset.setValue( "AAThreshold", s_previewAAThreshold );
   qset.setValue( "showWall", s_showWall );
   qset.setValue( "showFloor", s_showFloor );
   qset.setValue( "WallColor1", s_wallColor1 );
   qset.setValue( "WallColor2", s_wallColor2 );
   qset.setValue( "FloorColor1", s_floorColor1 );
   qset.setValue( "FloorColor2", s_floorColor2 );
   qset.setValue( "Gamma", s_previewGamma );
}

void PMDialogEditBase::restoreConfig()
{
   QSettings qset;
   qset.beginGroup( "TexturePreview" );
   setPreviewSize( qset.value( "Size", s_previewSize ).toInt() );
   s_showSphere = qset.value( "showSphere", s_showSphere ).toBool();
   s_showCylinder = qset.value( "showCylinder", s_showCylinder ).toBool();
   s_showBox = qset.value( "showBox", s_showBox ).toBool();
   s_previewAA = qset.value( "AA", s_previewAA ).toBool();
   setPreviewAADepth( qset.value( "AADepth", s_previewAADepth ).toInt() );
   s_previewAAThreshold = qset.value( "AAThreshold", s_previewAAThreshold ).toDouble();
   s_showWall = qset.value( "showWall", s_showWall ).toBool();
   s_showFloor = qset.value( "showFloor", s_showFloor ).toBool();
   s_wallColor1 = qset.value( "WallColor1", s_wallColor1 ).value<QColor>();
   s_wallColor2 = qset.value( "WallColor2", s_wallColor2 ).value<QColor>();
   s_floorColor1 = qset.value( "FloorColor1", s_floorColor1 ).value<QColor>();
   s_floorColor2 = qset.value( "FloorColor2", s_floorColor2 ).value<QColor>();
   s_previewGamma = qset.value( "Gamma", s_previewGamma ).toDouble();
}

void PMDialogEditBase::findTextures( PMObject*& global, PMObject*& local ) const
{
   PMObject* o;
   global = nullptr;
   local = nullptr;

   for( o = m_pDisplayedObject; o; o = o->parent() )
   {
      if( o->type() == "Material" || o->type() == "Interior" ||
          o->type() == "Texture" || o->type() == "Pigment" ||
          o->type() == "InteriorTexture" )
      {
         if( !local )
            local = o;
         global = o;
      }
      else if( o->type() == "Declare" )
      {
         PMDeclare* d = reinterpret_cast<PMDeclare*>( o );
         if( d->declareType() == "Interior" ||
             d->declareType() == "Pigment" ||
             d->declareType() == "Material" )
         {
            if( !local || ( local == global ) )
               local = o;
            global = o;
         }
         else if( d->declareType() == "Texture" ||
                  d->declareType() == "InteriorTexture" )
         {
            if( o->countChildren() > 1 )
            {
               if( !local )
                  local = o;
            }
            else
               if( !local || ( local == global ) )
                  local = o;
            global = o;
         }
      }
   }
}


void PMDialogEditBase::slotTexturePreview()//eticre error
{
   PMObject* o = nullptr;
   PMObject* local = nullptr;
   PMObject* global = nullptr;
   QString parentType;

   qba_scene_link_preview = nullptr;

   if( !m_pRenderWidget && !b_onlyPreview )
      return;

   findTextures( global, local );
   if( local )
   {
      emit aboutToRender();// save data if there is change

      if( global == local )
      {
         o = global;
      }
      else
      {
         if( m_pPreviewLocalBox )
         {
            if( m_pPreviewLocalBox->isChecked() )
            {
                o = local;
                parentType = o->parent()->type();
            }
            else
            {
                o = global;
            }
         }
         else
         {
             o = local;
         }
      }

      // ensure at least one object will be rendered.
      if( !( s_showSphere || s_showCylinder || s_showBox ) )
         s_showSphere = true;

      int numObjects = 0;
      QByteArray scene;
      QBuffer buffer( &scene );
      buffer.open( QIODevice::WriteOnly );
      QTextStream str( &buffer );
      PMPovray35Format format;
      PMSerializer* dev = format.newSerializer( &buffer );
      PMRenderMode mode;
      PMObjectList neededDeclares, objectsToCheck;
      QHash<PMObject*, bool> objectsToSerialize;
      PMObject* link;

      str << c_globalSettingsCode.arg( s_previewGamma );

      // find needed declares
      objectsToCheck.append( o );
      do
      {
         PMObjectList::iterator it;
         for( it = objectsToCheck.begin(); it != objectsToCheck.end(); ++it )
         {
            PMRecursiveObjectIterator rit( *it );
            for( ; rit.current(); ++rit )
            {
               link = rit.current()->linkedObject();
               if( link )
                  if( !neededDeclares.contains( link ) )
                     if( !objectsToSerialize.contains( link ) )
                     {
                        neededDeclares.append( link );
                        objectsToSerialize.insert( link, true );
                     }
            }
            //objectsToSerialize.insert( *it, true );
         }

         objectsToCheck = neededDeclares;
         neededDeclares.clear();
      }
      while( objectsToCheck.count() > 0 );

      // serialize all needed declares in the right order
      int numDeclares = objectsToSerialize.count();
      if( numDeclares > 0 )
      {
         PMObject* otr = o;

         // find the scene
         while( otr->parent() ) otr = otr->parent();

         for( otr = otr->firstChild(); otr && ( numDeclares > 0 );
              otr = otr->nextSibling() )
         {
            if( otr->type() == "Declare" )
            {
               if( objectsToSerialize.contains( otr ) )
               {
                  str.flush();
                  dev->serialize( otr );
                  numDeclares--;
               }
            }
         }
         str << "\n\n";
      }
      // if the previewed texture is a declare, serialize it
      if( o->type() == "Declare" )
      {
          str.flush();
          PMDeclare* tmpdeclare =  reinterpret_cast<PMDeclare*>( o );
          if( tmpdeclare->is_a_function )
          {
              tmpdeclare->is_a_function = false;
              dev->serialize( o );
              tmpdeclare->is_a_function = true;
          }
          else
          dev->serialize( o );
      }

      // build the preview scene
      str << "\n union {\n";
      if( s_showBox )
      {
         str << c_boxCode.arg( numObjects );
         numObjects++;
      }
      if( s_showCylinder )
      {
         str << c_cylinderCode.arg( numObjects );
         numObjects++;
      }
      if( s_showSphere )
      {
         str << c_sphereCode.arg( numObjects );
         numObjects++;
      }

      // serialize the texture write the id declare
      if( o->type() == "Declare" )
      {
         PMDeclare* dec = reinterpret_cast<PMDeclare*>( o );
         if( dec->declareType() == "Interior" )
            str << "interior { ";
         else if( dec->declareType() == "Texture" )
            str << "texture { ";
         else if( dec->declareType() == "Pigment" )
            str << "pigment { ";
         else if( dec->declareType() == "InteriorTexture" )
            str << "interior_texture { ";
         else if( dec->declareType() == "Material" )
            str << "material { ";
         else
            qCritical(  ) << "Unhandled declare type in "
               "PMDialogEditBase::slotTexturePreview\n";

         str << dec->id();
         str << " }\n";
      }
      else
      {
         str.flush();
         if( parentType == "PigmentMap" )
         {
             str << "pigment{\n";
             str.flush();
             dev->serialize( o );
             str << "}\n";
         }
         else if( parentType == "TextureMap" )
         {
             str << "texture{\n";
             str.flush();
             dev->serialize( o );
             str << "}\n";
         }
         else
             dev->serialize( o );
         //if( parentType != "" )//eticre-preview
            // str << "}//1\n";
      }
      str << "}\n";//union end
      PMColor c1, c2;
      if( s_showWall )
      {
         c1 = PMColor( s_wallColor1 );
         c2 = PMColor( s_wallColor2 );
         str << c_wallCode.arg( c1.serialize( true ) ).arg( c2.serialize( true ) );
      }
      if( s_showFloor )
      {
         c1 = PMColor( s_floorColor1 );
         c2 = PMColor( s_floorColor2 );
         str << c_floorCode.arg( c1.serialize( true ) ).arg( c2.serialize( true ) );
      }

      str << c_lightCode;
      str << c_cameraCode[numObjects-1];
      //eticre moved at first line to avoid err. using srgb
      //str << c_globalSettingsCode.arg( s_previewGamma );
      str.flush();

      // Set the render mode
      mode.setHeight( s_previewSize );
      mode.setWidth( s_previewSize );
      mode.setAntialiasing( s_previewAA );
      mode.setAntialiasingThreshold( s_previewAAThreshold );
      mode.setAntialiasingDepth( s_previewAADepth );
      mode.m_preview = true;

      if( !b_onlyPreview )
      {
          m_pRenderFrame->setHidden( false );
          m_pRenderFrame->setFixedSize( s_previewSize + 4, s_previewSize + 4);
          m_pRenderFrame->updateGeometry();
          m_pTexturePreviewWidget->layout()->activate();
          emit sizeChanged();
          m_pOutputWidget->slotClear();
          m_pRenderWidget->render( scene, mode, m_pPart->url() );
      }
      else
      {
          qba_scene_link_preview = scene;
          p_mode_link_preview = mode;
      }
      delete dev;
      mode.m_preview = false;
   }
}

void PMDialogEditBase::slotPreviewLocal( bool on )
{
   s_previewLocal = on;
}

void PMDialogEditBase::slotPreviewFinished( int exitStatus )
{
   if( exitStatus != 0 )
   {
     QMessageBox::information( this, "warning",
                   "Povray exited abnormally with exit code " + QString::number( exitStatus ) + "\n" +
                   "See the povray output for details.",
                   QMessageBox::Ok, QMessageBox::NoButton);
   }
   else if( m_pRenderWidget->povrayOutput().contains( "ERROR" ) )
   {
      QMessageBox::information( this, "warning",
                   "There were errors while rendering.\n See the povray output for details.",
                   QMessageBox::Ok, QMessageBox::NoButton);
   }
   else
   {
       labelPreviewImage->setPixmap( QPixmap::fromImage( m_pRenderWidget->image() ) );
       m_pDisplayedObject->setPreviewImage( m_pRenderWidget->image() );
   }
}

void PMDialogEditBase::slotPovrayOutput()
{
   if( m_pOutputWidget )
      m_pOutputWidget->show();
}

QSize PMDialogEditBase::sizeHint() const {
    return QWidget::minimumSizeHint();
}

void PMDialogEditBase::slotPreviewImage( QImage img )
{
  //m_pDisplayedObject->setPreviewImage( img );
  //labelPreviewImage->clear();
  //labelPreviewImage->setPixmap( QPixmap::fromImage( img ) );
}

#include "pmdialogeditbase.moc"
