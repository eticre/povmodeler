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

#ifndef PMPOVRAY35_SERIALIZATION_H
#define PMPOVRAY35_SERIALIZATION_H


#include "pmrendermode.h"

//const PMRenderMode* radiosity() ;

class PMObject;
class PMMetaObject;
class PMOutputDevice;

// serialization methods for POV-Ray 3.5

void PMPov35SerIsoSurface( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerLight( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerProjectedThrough( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerGlobalSettings( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerRadiosity( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerGlobalPhotons( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerPhotons( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerInterior( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerLightGroup( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerPattern( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerNormal( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerInteriorTexture( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerWarp( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerSphereSweep( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerFinish( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerMesh( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerMedia( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerGraphicalObject( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerPigment( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerTexture( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerBicubicPatch( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
void PMPov35SerTriangle( const PMObject* object, const PMMetaObject* metaObject, PMOutputDevice* dev );
#endif
