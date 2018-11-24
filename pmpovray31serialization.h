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

#ifndef PMPOVRAY31_SERIALIZATION_H
#define PMPOVRAY31_SERIALIZATION_H

class PMObject;
class PMMetaObject;
class PMOutputDevice;

// serialization methods for POV-Ray 3.1

void PMPov31SerBicubicPatch( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerBlendMapModifiers( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerBlob( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerBlobCylinder( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerBlobSphere( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerBoundedBy( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerBox( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerBumpMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerCamera( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerClippedBy( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerComment( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerCompositeObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerCone( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerCSG( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerCylinder( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerDeclare( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerDensity( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerDisc( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerFinish( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerFog( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerGlobalSettings( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerGraphicalObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerHeightField( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerImageMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerInterior( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerJuliaFractal( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerLathe( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerLight( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerListPattern( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTextureList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPigmentList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerColorList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerDensityList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerNormalList( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerLooksLike( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerMaterial( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerMaterialMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerMedia( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerNamedObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerNormal( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerObjectLink( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPattern( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPigment( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPlane( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPolynom( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPovrayMatrix( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPrism( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerQuickColor( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerRainbow( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerRaw( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerRotate( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerScale( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerScene( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSkySphere( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSlope( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSolidColor( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSolidObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSurfaceOfRevolution( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSphere( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSuperquadricEllipsoid( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerText( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTexture( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTextureBase( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTextureMapBase( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTextureMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerPigmentMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerColorMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerNormalMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerSlopeMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerDensityMap( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTorus( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTranslate( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerTriangle( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerWarp( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov31SerDetailObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );

#endif
