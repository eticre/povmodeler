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

#include "pmpovray35format.h"
#include "pmpovray35serialization.h"

#include "pmpovrayparser.h"
#include "pmoutputdevice.h"



PMPovray35Format::PMPovray35Format()
      : PMPovray31Format()
{
   registerMethod( "IsoSurface", PMPov35SerIsoSurface );
   registerMethod( "Light", PMPov35SerLight );
   registerMethod( "ProjectedThrough", PMPov35SerProjectedThrough );
   registerMethod( "GlobalSettings", PMPov35SerGlobalSettings );
   registerMethod( "Radiosity", PMPov35SerRadiosity );
   registerMethod( "GlobalPhotons", PMPov35SerGlobalPhotons );
   registerMethod( "Photons", PMPov35SerPhotons );
   registerMethod( "Interior", PMPov35SerInterior );
   registerMethod( "LightGroup", PMPov35SerLightGroup );
   registerMethod( "Pattern", PMPov35SerPattern );
   registerMethod( "Normal", PMPov35SerNormal );
   registerMethod( "InteriorTexture", PMPov35SerInteriorTexture );
   registerMethod( "Warp", PMPov35SerWarp );
   registerMethod( "SphereSweep", PMPov35SerSphereSweep );
   registerMethod( "Finish", PMPov35SerFinish );
   registerMethod( "Mesh", PMPov35SerMesh );
   registerMethod( "Media", PMPov35SerMedia );
   registerMethod( "GraphicalObject", PMPov35SerGraphicalObject );
   registerMethod( "Pigment", PMPov35SerPigment );
   registerMethod( "Texture", PMPov35SerTexture );
   registerMethod( "BicubicPatch", PMPov35SerBicubicPatch );
   registerMethod( "Triangle", PMPov35SerTriangle );
}


PMPovray35Format::~PMPovray35Format()
{

}

PMParser* PMPovray35Format::newParser( PMPart* part, QIODevice* dev ) const
{
   return new PMPovrayParser( part, dev );
}

PMParser* PMPovray35Format::newParser( PMPart* part, const QByteArray& data ) const
{
   return new PMPovrayParser( part, data );
}

PMSerializer* PMPovray35Format::newSerializer( QIODevice* dev )
{
   return new PMOutputDevice( dev, this );
}

PMRenderer* PMPovray35Format::newRenderer( PMPart* ) const
{
   // TODO
   return nullptr;
}

QString PMPovray35Format::mimeType() const
{
   return QString( "text/plain" );
}

QStringList PMPovray35Format::importPatterns() const
{
   QStringList result;
   result.push_back( QString( "*.pov *.inc|" )
                     + ( "POV-Ray 3.5 Files (*.pov, *.inc)" ) );
   return result;
}

QStringList PMPovray35Format::exportPatterns() const
{
   QStringList result;
   result.push_back( QString( "*.pov|" ) + ( "POV-Ray 3.5 Files (*.pov)" ) );
   result.push_back( QString( "*.ini|" ) + ( "POV-Ray 3.5 Include Files (*.ini)" ) );
   return result;

}
