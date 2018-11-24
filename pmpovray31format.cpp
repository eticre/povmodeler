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

#include "pmpovray31format.h"
#include "pmpovray31serialization.h"

#include "pmpovrayparser.h"
#include "pmoutputdevice.h"



PMPovray31Format::PMPovray31Format()
      : PMPovrayFormat()
{
   registerMethod( "BicubicPatch", PMPov31SerBicubicPatch );
   registerMethod( "BlendMapModifiers", PMPov31SerBlendMapModifiers );
   registerMethod( "Blob", PMPov31SerBlob );
   registerMethod( "BlobCylinder", PMPov31SerBlobCylinder );
   registerMethod( "BlobSphere", PMPov31SerBlobSphere );
   registerMethod( "BoundedBy", PMPov31SerBoundedBy );
   registerMethod( "Box", PMPov31SerBox );
   registerMethod( "BumpMap", PMPov31SerBumpMap );
   registerMethod( "Camera", PMPov31SerCamera );
   registerMethod( "ClippedBy", PMPov31SerClippedBy );
   registerMethod( "Comment", PMPov31SerComment );
   registerMethod( "CompositeObject", PMPov31SerCompositeObject );
   registerMethod( "Cone", PMPov31SerCone );
   registerMethod( "CSG", PMPov31SerCSG );
   registerMethod( "Cylinder", PMPov31SerCylinder );
   registerMethod( "Declare", PMPov31SerDeclare );
   registerMethod( "Density", PMPov31SerDensity );
   registerMethod( "Disc", PMPov31SerDisc );
   registerMethod( "Finish", PMPov31SerFinish );
   registerMethod( "Fog", PMPov31SerFog );
   registerMethod( "GlobalSettings", PMPov31SerGlobalSettings );
   registerMethod( "GraphicalObject", PMPov31SerGraphicalObject );
   registerMethod( "HeightField", PMPov31SerHeightField );
   registerMethod( "ImageMap", PMPov31SerImageMap );
   registerMethod( "Interior", PMPov31SerInterior );
   registerMethod( "JuliaFractal", PMPov31SerJuliaFractal );
   registerMethod( "Lathe", PMPov31SerLathe );
   registerMethod( "Light", PMPov31SerLight );
   registerMethod( "ListPattern", PMPov31SerListPattern );
   registerMethod( "TextureList", PMPov31SerTextureList );
   registerMethod( "PigmentList", PMPov31SerPigmentList );
   registerMethod( "ColorList", PMPov31SerColorList );
   registerMethod( "DensityList", PMPov31SerDensityList );
   registerMethod( "NormalList", PMPov31SerNormalList );
   registerMethod( "LooksLike", PMPov31SerLooksLike );
   registerMethod( "Material", PMPov31SerMaterial );
   registerMethod( "MaterialMap", PMPov31SerMaterialMap );
   registerMethod( "Media", PMPov31SerMedia );
   registerMethod( "NamedObject", PMPov31SerNamedObject );
   registerMethod( "Normal", PMPov31SerNormal );
   registerMethod( "ObjectLink", PMPov31SerObjectLink );
   registerMethod( "Pattern", PMPov31SerPattern );
   registerMethod( "Pigment", PMPov31SerPigment );
   registerMethod( "Plane", PMPov31SerPlane );
   registerMethod( "Polynom", PMPov31SerPolynom );
   registerMethod( "PovrayMatrix", PMPov31SerPovrayMatrix );
   registerMethod( "Prism", PMPov31SerPrism );
   registerMethod( "QuickColor", PMPov31SerQuickColor );
   registerMethod( "Rainbow", PMPov31SerRainbow );
   registerMethod( "Raw", PMPov31SerRaw );
   registerMethod( "Rotate", PMPov31SerRotate );
   registerMethod( "Scale", PMPov31SerScale );
   registerMethod( "Scene", PMPov31SerScene );
   registerMethod( "SkySphere", PMPov31SerSkySphere );
   registerMethod( "Slope", PMPov31SerSlope );
   registerMethod( "SolidColor", PMPov31SerSolidColor );
   registerMethod( "SolidObject", PMPov31SerSolidObject );
   registerMethod( "SurfaceOfRevolution", PMPov31SerSurfaceOfRevolution );
   registerMethod( "Sphere", PMPov31SerSphere );
   registerMethod( "SuperquadricEllipsoid", PMPov31SerSuperquadricEllipsoid );
   registerMethod( "Text", PMPov31SerText );
   registerMethod( "Texture", PMPov31SerTexture );
   registerMethod( "TextureBase", PMPov31SerTextureBase );
   registerMethod( "TextureMapBase", PMPov31SerTextureMapBase );
   registerMethod( "TextureMap", PMPov31SerTextureMap );
   registerMethod( "PigmentMap", PMPov31SerPigmentMap );
   registerMethod( "ColorMap", PMPov31SerColorMap );
   registerMethod( "NormalMap", PMPov31SerNormalMap );
   registerMethod( "SlopeMap", PMPov31SerSlopeMap );
   registerMethod( "DensityMap", PMPov31SerDensityMap );
   registerMethod( "Torus", PMPov31SerTorus );
   registerMethod( "Translate", PMPov31SerTranslate );
   registerMethod( "Triangle", PMPov31SerTriangle );
   registerMethod( "Warp", PMPov31SerWarp );
   registerMethod( "DetailObject", PMPov31SerDetailObject );
}


PMPovray31Format::~PMPovray31Format()
{

}

PMParser* PMPovray31Format::newParser( PMPart* part, QIODevice* dev ) const
{
   return new PMPovrayParser( part, dev );
}

PMParser* PMPovray31Format::newParser( PMPart* part, const QByteArray& data ) const
{
   return new PMPovrayParser( part, data );
}

PMSerializer* PMPovray31Format::newSerializer( QIODevice* dev )
{
   return new PMOutputDevice( dev, this );
}

PMRenderer* PMPovray31Format::newRenderer( PMPart* ) const
{
   // TODO
   return 0;
}

QString PMPovray31Format::mimeType() const
{
   return QString( "text/plain" );
}

QStringList PMPovray31Format::importPatterns() const
{
   QStringList result;
   result.push_back( QString( "*.pov *.inc|" )
                     + ( "POV-Ray 3.1 Files (*.pov, *.inc)" ) );
   return result;
}

QStringList PMPovray31Format::exportPatterns() const
{
   QStringList result;
   result.push_back( QString( "*.pov|" ) + ( "POV-Ray 3.1 Files (*.pov)" ) );
   result.push_back( QString( "*.ini|" ) + ( "POV-Ray 3.1 Include Files (*.ini)" ) );
   return result;

}
