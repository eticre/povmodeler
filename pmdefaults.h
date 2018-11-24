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

#ifndef PMDEFAULTS_H
#define PMDEFAULTS_H



#include <qcolor.h>
#include <math.h>

// PMRenderManager
const QColor c_defaultGraphicalObjectColor0 = QColor( 148, 148, 148 );
const QColor c_defaultGraphicalObjectColor1 = QColor( 255, 255, 128 );
const QColor c_defaultTextureColor0 = QColor( 64, 192, 64 );
const QColor c_defaultTextureColor1 = QColor( 192, 255, 128 );
const QColor c_defaultAxesColorX = QColor( 255, 0, 0 );
const QColor c_defaultAxesColorY = QColor( 0, 255, 0 );
const QColor c_defaultAxesColorZ = QColor( 0, 0, 255 );
const QColor c_defaultControlPointColor0 = QColor( 255, 255, 148 );
const QColor c_defaultControlPointColor1 = QColor( 92, 255, 92 );
const QColor c_defaultBackgroundColor = QColor( 0, 0, 0 );
const QColor c_defaultFieldOfViewColor = QColor( 128, 128, 255 );
const bool c_defaultHighDetailCameraView = true;

// PMGLView
const int c_defaultGridDistance = 50;
const QColor c_defaultGridColor = QColor( 40, 120, 40 );

// PovrayRenderWidget
const QString c_defaultPovrayCommand = QString( "povray" );

// PMDetailObject
const int c_defaultDetailObjectGlobalDetailLevel = 1;

// PMSphere
const int c_defaultSphereUSteps = 8;
const int c_defaultSphereVSteps = 16;

// PMBlobSphere
const int c_defaultBlobSphereUSteps = 8;
const int c_defaultBlobSphereVSteps = 16;

// PMBlobCylinder
const int c_defaultBlobCylinderUSteps = 4;
const int c_defaultBlobCylinderVSteps = 16;

// PMCylinder
const int c_defaultCylinderSteps = 16;

// PMCone
const int c_defaultConeSteps = 16;

// PMTorus
const int c_defaultTorusVSteps = 16;
const int c_defaultTorusUSteps = 9;

// PMPlane
const double c_defaultPlaneSize = 2.0;

// PMDisc
const int c_defaultDiscSteps = 16;

// PMText
const int c_defaultTextSteps = 3;

// PMLathe
const int c_defaultLatheRSteps = 16;
const int c_defaultLatheSSteps = 4;

// PMPrism
const int c_defaultPrismSSteps = 4;

// PMSurfaceOfRevolution
const int c_defaultSurfaceOfRevolutionRSteps = 16;
const int c_defaultSurfaceOfRevolutionSSteps = 4;

// PMSuperquadricEllipsoid
const int c_defaultSuperquadricEllipsoidUSteps = 3;
const int c_defaultSuperquadricEllipsoidVSteps = 3;

// PMSphereSweep
const int c_defaultSphereSweepRSteps = 8;
const int c_defaultSphereSweepSSteps = 4;

// PMHeightField
const int c_defaultHeightFieldVariance = 16;

// PMDialogEditBase, texture preview
const int c_defaultTPSize = 200;
const bool c_defaultTPShowSphere = true;
const bool c_defaultTPShowCylinder = false;
const bool c_defaultTPShowBox = true;
const bool c_defaultTPAA = false;
const int c_defaultTPAADepth = 2;
const double c_defaultTPAAThreshold = 0.3;
const bool c_defaultTPShowFloor = true;
const bool c_defaultTPShowWall = true;
const QColor c_defaultTPWallColor1 = QColor( 255, 255, 255 );
const QColor c_defaultTPWallColor2 = QColor( 0, 0, 0 );
const QColor c_defaultTPFloorColor1 = QColor( 192, 0, 0 );
const QColor c_defaultTPFloorColor2 = QColor( 255, 255, 255 );
const double c_defaultTPGamma = 1;

// PM3DControlPoint
const double c_defaultMoveGrid = 0.1;
const double c_defaultScaleGrid = 0.1;
const double c_defaultRotateGrid = 1.0;

// PMShell
const bool c_defaultTreeViewTabbed = false;
const bool c_defaultTreeViewRight = false;
const bool c_defaultTreeViewBelow = false;

#endif
