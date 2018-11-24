/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmobjectsettings.h"

#include "pmlineedits.h"
#include "pmrendermanager.h"
#include "pmdefaults.h"

#include "pmsphere.h"
#include "pmcone.h"
#include "pmcylinder.h"
#include "pmtorus.h"
#include "pmplane.h"
#include "pmdisc.h"
#include "pmblobsphere.h"
#include "pmblobcylinder.h"
#include "pmlathe.h"
#include "pmsor.h"
#include "pmprism.h"
#include "pmsqe.h"
#include "pmspheresweep.h"
#include "pmheightfield.h"
#include "pmtext.h"

#include <qlayout.h>
#include <qcheckbox.h>
#include <QGroupBox>
#include <qlabel.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


PMObjectSettings::PMObjectSettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QHBoxLayout* hlayout;
   QVBoxLayout* vlayout;
   QVBoxLayout* gvl;
   QGridLayout* grid;
   QGroupBox* gb;

   setContentsMargins(0,0,0,0);
   vlayout = new QVBoxLayout( this );

   vlayout->setMargin( 0 );
   vlayout->setContentsMargins( 0, 0, 0, 0 );

   gb = new QGroupBox( ( "Subdivisions" ) );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );
   gvl->setContentsMargins( 0, 0, 0, 0 );
   gvl->setMargin( 0 );

   gvl->addSpacing( 10 );
   grid = new QGridLayout();
   grid->setSpacing(0);
   gvl->addLayout( grid );

   grid->addWidget( new QLabel( ( "Sphere:" ), gb ), 0, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 0, 2 );
   grid->addWidget( new QLabel( "u", gb ), 0, 1 );
   m_pSphereUSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSphereUSteps );
   m_pSphereUSteps->setValidation( true, 2, true, 32 );
   hlayout->addWidget( new QLabel( "v", gb ) );
   m_pSphereVSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSphereVSteps );
   m_pSphereVSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Cylinder:" ), gb ), 1, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 1, 2 );
   m_pCylinderSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pCylinderSteps );
   m_pCylinderSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Cone:" ), gb ), 2, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 2, 2 );
   m_pConeSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pConeSteps );
   m_pConeSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Torus:" ), gb ), 3, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 3, 2 );
   grid->addWidget( new QLabel( "u", gb ), 3, 1 );
   m_pTorusUSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pTorusUSteps );
   m_pTorusUSteps->setValidation( true, 2, true, 16 );
   hlayout->addWidget( new QLabel( "v", gb ) );
   m_pTorusVSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pTorusVSteps );
   m_pTorusVSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Disc:" ), gb ), 4, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 4, 2 );
   m_pDiscSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pDiscSteps );
   m_pDiscSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Blob sphere:" ), gb ), 5, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 5, 2 );
   grid->addWidget( new QLabel( "u", gb ), 5, 1 );
   m_pBlobSphereUSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pBlobSphereUSteps );
   m_pBlobSphereUSteps->setValidation( true, 2, true, 32 );
   hlayout->addWidget( new QLabel( "v", gb ) );
   m_pBlobSphereVSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pBlobSphereVSteps );
   m_pBlobSphereVSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Blob cylinder:" ), gb ), 6, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 6, 2 );
   grid->addWidget( new QLabel( "u", gb ), 6, 1 );
   m_pBlobCylinderUSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pBlobCylinderUSteps );
   m_pBlobCylinderUSteps->setValidation( true, 2, true, 32 );
   hlayout->addWidget( new QLabel( "v", gb ) );
   m_pBlobCylinderVSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pBlobCylinderVSteps );
   m_pBlobCylinderVSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Lathe:" ), gb ), 7, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 7, 2 );
   grid->addWidget( new QLabel( "u", gb ), 7, 1 );
   m_pLatheUSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pLatheUSteps );
   m_pLatheUSteps->setValidation( true, 1, true, 16 );
   hlayout->addWidget( new QLabel( "v", gb ) );
   m_pLatheRSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pLatheRSteps );
   m_pLatheRSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Surface of revolution:" ), gb ), 8, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 8, 2 );
   grid->addWidget( new QLabel( "u", gb ), 8, 1 );
   m_pSorUSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSorUSteps );
   m_pSorUSteps->setValidation( true, 1, true, 16 );
   hlayout->addWidget( new QLabel( "v", gb ) );
   m_pSorRSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSorRSteps );
   m_pSorRSteps->setValidation( true, 4, true, 64 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Prism:" ), gb ), 9, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 9, 2 );
   m_pPrismSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pPrismSteps );
   m_pPrismSteps->setValidation( true, 1, true, 16 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Superquadric ellipsoid:" ), gb ), 10, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 10, 2 );
   grid->addWidget( new QLabel( "u", gb ), 10, 1 );
   m_pSqeUSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSqeUSteps );
   m_pSqeUSteps->setValidation( true, 2, true, 8 );
   hlayout->addWidget( new QLabel( "v", gb ) );
   m_pSqeVSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSqeVSteps );
   m_pSqeVSteps->setValidation( true, 2, true, 8 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Sphere sweep:" ), gb ), 11, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 11, 2 );
   grid->addWidget( new QLabel( "r", gb ), 11, 1 );
   m_pSphereSweepRSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSphereSweepRSteps );
   m_pSphereSweepRSteps->setValidation( true, 4, true, 64 );
   hlayout->addWidget( new QLabel( "s", gb ) );
   m_pSphereSweepSSteps = new PMIntEdit( gb );
   hlayout->addWidget( m_pSphereSweepSSteps );
   m_pSphereSweepSSteps->setValidation( true, 1, true, 16 );
   hlayout->addStretch( 1 );

   grid->addWidget( new QLabel( ( "Heightfield:" ), gb ), 12, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 12, 2 );
   m_pHeightFieldVariance = new PMIntEdit( gb );
   hlayout->addWidget( m_pHeightFieldVariance );
   m_pHeightFieldVariance->setValidation( true, 1, true, 16 );
   hlayout->addStretch( 1 );

   gb = new QGroupBox( ( "Sizes" ), this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );
   gvl->setMargin( 0 );
   gvl->setContentsMargins( 0, 0, 0, 0 );
   gvl->addSpacing( 1 );

   grid = new QGridLayout();
   gvl->addLayout( grid );

   grid->addWidget( new QLabel( ( "Plane:" ), gb ), 0, 0 );
   hlayout = new QHBoxLayout();
   grid->addLayout( hlayout, 0, 1 );
   m_pPlaneSize = new PMFloatEdit( gb );
   hlayout->addWidget( m_pPlaneSize );
   m_pPlaneSize->setValidation( true, 0.1, false, 0.0 );
   hlayout->addStretch( 1 );

   gb = new QGroupBox( ( "Camera Views" ), this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );
   gvl->setMargin( 0 );
   gvl->setContentsMargins( 0, 0, 0, 0 );
   gvl->addSpacing( 1 );
   gvl->addSpacing( 1 );

   m_pHighDetailCameraViews = new QCheckBox( tr( "High detail for enhanced projections" ), gb );
   gvl->addWidget( m_pHighDetailCameraViews );

   vlayout->addStretch( 1 );
}

void PMObjectSettings::displaySettings()
{
   m_pSphereUSteps->setValue( PMSphere::uSteps() );
   m_pSphereVSteps->setValue( PMSphere::vSteps() );
   m_pCylinderSteps->setValue( PMCylinder::steps() );
   m_pConeSteps->setValue( PMCone::steps() );
   m_pTorusUSteps->setValue( PMTorus::uSteps() );
   m_pTorusVSteps->setValue( PMTorus::vSteps() );
   m_pDiscSteps->setValue( PMDisc::steps() );
   m_pBlobSphereUSteps->setValue( PMBlobSphere::uSteps() );
   m_pBlobSphereVSteps->setValue( PMBlobSphere::vSteps() );
   m_pBlobCylinderUSteps->setValue( PMBlobCylinder::uSteps() );
   m_pBlobCylinderVSteps->setValue( PMBlobCylinder::vSteps() );
   m_pPlaneSize->setValue( PMPlane::planeSize() );
   m_pLatheUSteps->setValue( PMLathe::sSteps() );
   m_pLatheRSteps->setValue( PMLathe::rSteps() );
   m_pSorUSteps->setValue( PMSurfaceOfRevolution::sSteps() );
   m_pSorRSteps->setValue( PMSurfaceOfRevolution::rSteps() );
   m_pPrismSteps->setValue( PMPrism::sSteps() );
   m_pSqeUSteps->setValue( PMSuperquadricEllipsoid::uSteps() );
   m_pSqeVSteps->setValue( PMSuperquadricEllipsoid::vSteps() );
   m_pSphereSweepRSteps->setValue( PMSphereSweep::rSteps() );
   m_pSphereSweepSSteps->setValue( PMSphereSweep::sSteps() );
   m_pHeightFieldVariance->setValue( PMHeightField::variance() );
   PMRenderManager* rm = PMRenderManager::theManager();
   m_pHighDetailCameraViews->setChecked( rm->highDetailCameraViews() );
}

void PMObjectSettings::displayDefaults()
{
   m_pSphereUSteps->setValue( c_defaultSphereUSteps );
   m_pSphereVSteps->setValue( c_defaultSphereVSteps );
   m_pCylinderSteps->setValue( c_defaultCylinderSteps );
   m_pConeSteps->setValue( c_defaultConeSteps );
   m_pTorusUSteps->setValue( c_defaultTorusUSteps );
   m_pTorusVSteps->setValue( c_defaultTorusVSteps );
   m_pDiscSteps->setValue( c_defaultDiscSteps );
   m_pBlobSphereUSteps->setValue( c_defaultBlobSphereUSteps );
   m_pBlobSphereVSteps->setValue( c_defaultBlobSphereVSteps );
   m_pBlobCylinderUSteps->setValue( c_defaultBlobCylinderUSteps );
   m_pBlobCylinderVSteps->setValue( c_defaultBlobCylinderVSteps );
   m_pLatheUSteps->setValue( c_defaultLatheSSteps );
   m_pLatheRSteps->setValue( c_defaultLatheRSteps );
   m_pSorUSteps->setValue( c_defaultSurfaceOfRevolutionSSteps );
   m_pSorRSteps->setValue( c_defaultSurfaceOfRevolutionRSteps );
   m_pPrismSteps->setValue( c_defaultPrismSSteps );
   m_pSqeUSteps->setValue( c_defaultSuperquadricEllipsoidUSteps );
   m_pSqeVSteps->setValue( c_defaultSuperquadricEllipsoidVSteps );
   m_pSphereSweepRSteps->setValue( c_defaultSphereSweepRSteps );
   m_pSphereSweepSSteps->setValue( c_defaultSphereSweepSSteps );
   m_pHeightFieldVariance->setValue( c_defaultHeightFieldVariance );
   m_pPlaneSize->setValue( c_defaultPlaneSize );
   m_pHighDetailCameraViews->setChecked( c_defaultHighDetailCameraView );
}

bool PMObjectSettings::validateData()
{
   if( !m_pSphereUSteps->isDataValid() )
   {
      emit showMe();
      m_pSphereUSteps->setFocus();
      return false;
   }
   if( !m_pSphereVSteps->isDataValid() )
   {
      emit showMe();
      m_pSphereVSteps->setFocus();
      return false;
   }
   if( !m_pCylinderSteps->isDataValid() )
   {
      emit showMe();
      m_pCylinderSteps->setFocus();
      return false;
   }
   if( !m_pConeSteps->isDataValid() )
   {
      emit showMe();
      m_pConeSteps->setFocus();
      return false;
   }
   if( !m_pTorusUSteps->isDataValid() )
   {
      emit showMe();
      m_pTorusUSteps->setFocus();
      return false;
   }
   if( !m_pTorusVSteps->isDataValid() )
   {
      emit showMe();
      m_pTorusVSteps->setFocus();
      return false;
   }
   if( !m_pDiscSteps->isDataValid() )
   {
      emit showMe();
      m_pDiscSteps->setFocus();
      return false;
   }
   if( !m_pBlobSphereUSteps->isDataValid() )
   {
      emit showMe();
      m_pBlobSphereUSteps->setFocus();
      return false;
   }
   if( !m_pBlobSphereVSteps->isDataValid() )
   {
      emit showMe();
      m_pBlobSphereVSteps->setFocus();
      return false;
   }
   if( !m_pBlobCylinderUSteps->isDataValid() )
   {
      emit showMe();
      m_pBlobCylinderUSteps->setFocus();
      return false;
   }
   if( !m_pBlobCylinderVSteps->isDataValid() )
   {
      emit showMe();
      m_pBlobCylinderVSteps->setFocus();
      return false;
   }
   if( !m_pLatheUSteps->isDataValid() )
   {
      emit showMe();
      m_pLatheUSteps->setFocus();
      return false;
   }
   if( !m_pLatheRSteps->isDataValid() )
   {
      emit showMe();
      m_pLatheRSteps->setFocus();
      return false;
   }
   if( !m_pSorUSteps->isDataValid() )
   {
      emit showMe();
      m_pSorUSteps->setFocus();
      return false;
   }
   if( !m_pSorRSteps->isDataValid() )
   {
      emit showMe();
      m_pSorRSteps->setFocus();
      return false;
   }
   if( !m_pPrismSteps->isDataValid() )
   {
      emit showMe();
      m_pPrismSteps->setFocus();
      return false;
   }
   if( !m_pSqeUSteps->isDataValid() )
   {
      emit showMe();
      m_pSqeUSteps->setFocus();
      return false;
   }
   if( !m_pSqeVSteps->isDataValid() )
   {
      emit showMe();
      m_pSqeVSteps->setFocus();
      return false;
   }
   if( !m_pSphereSweepRSteps->isDataValid() )
   {
      emit showMe();
      m_pSphereSweepRSteps->setFocus();
      return false;
   }
   if( !m_pSphereSweepSSteps->isDataValid() )
   {
      emit showMe();
      m_pSphereSweepSSteps->setFocus();
      return false;
   }
   if( !m_pHeightFieldVariance->isDataValid() )
   {
      emit showMe();
      m_pHeightFieldVariance->setFocus();
      return false;
   }
   if( !m_pPlaneSize->isDataValid() )
   {
      emit showMe();
      m_pPlaneSize->setFocus();
      return false;
   }
   return true;
}

void PMObjectSettings::applySettings()
{
   bool repaint = false;

   PMRenderManager* rm = PMRenderManager::theManager();
   if( rm->highDetailCameraViews() != m_pHighDetailCameraViews->isChecked() )
   {
      rm->setHighDetailCameraViews( m_pHighDetailCameraViews->isChecked() );
      repaint = true;
   }
   if( PMSphere::uSteps() != m_pSphereUSteps->value() )
   {
      PMSphere::setUSteps( m_pSphereUSteps->value() );
      repaint = true;
   }
   if( PMSphere::vSteps() != m_pSphereVSteps->value() )
   {
      PMSphere::setVSteps( m_pSphereVSteps->value() );
      repaint = true;
   }
   if( PMCylinder::steps() != m_pCylinderSteps->value() )
   {
      PMCylinder::setSteps( m_pCylinderSteps->value() );
      repaint = true;
   }
   if( PMCone::steps() != m_pConeSteps->value() )
   {
      PMCone::setSteps( m_pConeSteps->value() );
      repaint = true;
   }
   if( PMTorus::uSteps() != m_pTorusUSteps->value() )
   {
      PMTorus::setUSteps( m_pTorusUSteps->value() );
      repaint = true;
   }
   if( PMTorus::vSteps() != m_pTorusVSteps->value() )
   {
      PMTorus::setVSteps( m_pTorusVSteps->value() );
      repaint = true;
   }
   if( PMDisc::steps() != m_pDiscSteps->value() )
   {
      PMDisc::setSteps( m_pDiscSteps->value() );
      repaint = true;
   }
   if( PMBlobSphere::uSteps() != m_pBlobSphereUSteps->value() )
   {
      PMBlobSphere::setUSteps( m_pBlobSphereUSteps->value() );
      repaint = true;
   }
   if( PMBlobSphere::vSteps() != m_pBlobSphereVSteps->value() )
   {
      PMBlobSphere::setVSteps( m_pBlobSphereVSteps->value() );
      repaint = true;
   }
   if( PMBlobCylinder::uSteps() != m_pBlobCylinderUSteps->value() )
   {
      PMBlobCylinder::setUSteps( m_pBlobCylinderUSteps->value() );
      repaint = true;
   }
   if( PMBlobCylinder::vSteps() != m_pBlobCylinderVSteps->value() )
   {
      PMBlobCylinder::setVSteps( m_pBlobCylinderVSteps->value() );
      repaint = true;
   }
   if( PMPlane::planeSize() != m_pPlaneSize->value() )
   {
      PMPlane::setPlaneSize( m_pPlaneSize->value() );
      repaint = true;
   }
   if( PMLathe::sSteps() != m_pLatheUSteps->value() )
   {
      PMLathe::setSSteps( m_pLatheUSteps->value() );
      repaint = true;
   }
   if( PMLathe::rSteps() != m_pLatheRSteps->value() )
   {
      PMLathe::setRSteps( m_pLatheRSteps->value() );
      repaint = true;
   }
   if( PMSurfaceOfRevolution::sSteps() != m_pSorUSteps->value() )
   {
      PMSurfaceOfRevolution::setSSteps( m_pSorUSteps->value() );
      repaint = true;
   }
   if( PMSurfaceOfRevolution::rSteps() != m_pSorRSteps->value() )
   {
      PMSurfaceOfRevolution::setRSteps( m_pSorRSteps->value() );
      repaint = true;
   }
   if( PMPrism::sSteps() != m_pPrismSteps->value() )
   {
      PMPrism::setSSteps( m_pPrismSteps->value() );
      repaint = true;
   }
   if( PMSuperquadricEllipsoid::uSteps() != m_pSqeUSteps->value() )
   {
      PMSuperquadricEllipsoid::setUSteps( m_pSqeUSteps->value() );
      repaint = true;
   }
   if( PMSuperquadricEllipsoid::vSteps() != m_pSqeVSteps->value() )
   {
      PMSuperquadricEllipsoid::setVSteps( m_pSqeVSteps->value() );
      repaint = true;
   }
   if( PMSphereSweep::rSteps() != m_pSphereSweepRSteps->value() )
   {
      PMSphereSweep::setRSteps( m_pSphereSweepRSteps->value() );
      repaint = true;
   }
   if( PMSphereSweep::sSteps() != m_pSphereSweepSSteps->value() )
   {
      PMSphereSweep::setSSteps( m_pSphereSweepSSteps->value() );
      repaint = true;
   }
   if( PMHeightField::variance() != m_pHeightFieldVariance->value() )
   {
      PMHeightField::setVariance( m_pHeightFieldVariance->value() );
      repaint = true;
   }
   if( repaint )
      emit repaintViews();
}

#include "pmobjectsettings.moc"
