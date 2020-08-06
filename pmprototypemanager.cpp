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


#include "pmprototypemanager.h"
#include "pmallobjects.h"

PMPrototypeManager::PMPrototypeManager( PMPart* part )
{
   m_pPart = part;
   addPrototype( new PMScene( part ) );
   addPrototype( new PMGlobalSettings( part ) );
   addPrototype( new PMBox( part ) );
   addPrototype( new PMSphere( part ) );
   addPrototype( new PMCylinder( part ) );
   addPrototype( new PMCone( part ) );
   addPrototype( new PMTorus( part ) );
   addPrototype( new PMLathe( part ) );
   addPrototype( new PMPrism( part ) );

   addPrototype( new PMSurfaceOfRevolution( part ) );
   addPrototype( new PMSuperquadricEllipsoid( part ) );
   addPrototype( new PMHeightField( part ) );
   addPrototype( new PMText( part ) );
   addPrototype( new PMJuliaFractal( part ) );

   addPrototype( new PMBlob( part ) );
   addPrototype( new PMBlobSphere( part ) );
   addPrototype( new PMBlobCylinder( part ) );

   addPrototype( new PMPlane( part ) );
   addPrototype( new PMPolynom( part ) );

   addPrototype( new PMCSG( part ) );
   //addPrototype( new PMDeclib( part ) );
   addPrototype( new PMDeclare( part ) );
   addPrototype( new PMObjectLink( part ) );

   addPrototype( new PMDisc( part ) );
   addPrototype( new PMBicubicPatch( part ) );
   addPrototype( new PMTriangle( part ) );

   addPrototype( new PMBoundedBy( part ) );
   addPrototype( new PMClippedBy( part ) );

   addPrototype( new PMTranslate( part ) );
   addPrototype( new PMScale( part ) );
   addPrototype( new PMRotate( part ) );
   addPrototype( new PMPovrayMatrix( part ) );
   addPrototype( new PMComment( part ) );
   addPrototype( new PMRaw( part ) );
   addPrototype( new PMCamera( part ) );
   addPrototype( new PMLight( part ) );
   addPrototype( new PMLooksLike( part ) );
   addPrototype( new PMProjectedThrough( part ) );

   addPrototype( new PMTexture( part ) );
   addPrototype( new PMPigment( part ) );
   addPrototype( new PMSolidColor( part ) );
   addPrototype( new PMNormal( part ) );
   addPrototype( new PMFinish( part ) );
   addPrototype( new PMPattern( part ) );
   addPrototype( new PMBlendMapModifiers( part ) );
   addPrototype( new PMImageMap( part ) );
   addPrototype( new PMTextureMap( part ) );
   addPrototype( new PMColorMap( part ) );
   addPrototype( new PMPigmentMap( part ) );
   addPrototype( new PMNormalMap( part ) );
   addPrototype( new PMSlopeMap( part ) );
   addPrototype( new PMDensityMap( part ) );
   addPrototype( new PMMaterialMap( part ) );
   addPrototype( new PMBumpMap( part ) );
   addPrototype( new PMTextureList( part ) );
   addPrototype( new PMColorList( part ) );
   addPrototype( new PMPigmentList( part ) );
   addPrototype( new PMNormalList( part ) );
   addPrototype( new PMDensityList( part ) );
   addPrototype( new PMWarp( part ) );
   addPrototype( new PMQuickColor( part ) );
   addPrototype( new PMSlope( part ) );
   addPrototype( new PMSkySphere( part ) );
   addPrototype( new PMRainbow( part ) );
   addPrototype( new PMFog( part ) );
   addPrototype( new PMInterior( part ) );
   addPrototype( new PMMedia( part ) );
   addPrototype( new PMMaterial( part ) );
   addPrototype( new PMDensity( part ) );

   // POV-Ray 3.5 objects
   addPrototype( new PMIsoSurface( part ) );
   addPrototype( new PMRadiosity( part ) );
   addPrototype( new PMGlobalPhotons( part ) );
   addPrototype( new PMPhotons( part ) );
   addPrototype( new PMLightGroup( part ) );
   addPrototype( new PMInteriorTexture( part ) );
   addPrototype( new PMSphereSweep( part ) );
   addPrototype( new PMMesh( part ) );

   addDeclarationType( "GraphicalObject", ( "object declaration" ), "pmobjectdeclare" );
   addDeclarationType( "Light", ( "object declaration" ), "pmobjectdeclare" );
   addDeclarationType( "Texture", ( "texture declaration" ), "pmtexturedeclare" );
   addDeclarationType( "Pigment", ( "pigment declaration" ), "pmpigmentdeclare" );
   addDeclarationType( "Normal", ( "normal declaration" ), "pmnormaldeclare" );
   addDeclarationType( "Finish", ( "finish declaration" ), "pmfinishdeclare" );
   addDeclarationType( "TextureMap", ( "texture map declaration" ), "pmtexturemapdeclare" );
   addDeclarationType( "PigmentMap", ( "pigment map declaration" ), "pmpigmentmapdeclare" );
   addDeclarationType( "ColorMap", ( "color map declaration" ), "pmcolormapdeclare" );
   addDeclarationType( "NormalMap", ( "normal map declaration" ), "pmnormaldeclare" );
   addDeclarationType( "SlopeMap", ( "slope map declaration" ), "pmslopemapdeclare" );
   addDeclarationType( "DensityMap", ( "density map declaration" ), "pmdensitydeclare" );
   addDeclarationType( "Interior", ( "interior declaration" ), "pminteriordeclare" );
   addDeclarationType( "Media", ( "media declaration" ), "pmmediadeclare" );
   addDeclarationType( "SkySphere", ( "sky sphere declaration" ), "pmskyspheredeclare" );
   addDeclarationType( "Rainbow", ( "rainbow declaration" ), "pmrainbowdeclare" );
   addDeclarationType( "Fog", ( "fog declaration" ), "pmfogdeclare" );
   addDeclarationType( "Material", ( "material declaration" ), "pmmaterialdeclare" );
   addDeclarationType( "Density", ( "density declaration" ), "pmdensitydeclare" );
   addDeclarationType( "InteriorTexture", ( "texture declaration" ), "pminteriortexturedeclare" );
}

PMPrototypeManager::~PMPrototypeManager()
{
}

void PMPrototypeManager::addPrototype( PMObject* obj )
{
   if( !obj )
      return;

   PMMetaObject* metaObject = obj->metaObject();
	QMap<QString, PMMetaObject*>::iterator it = m_metaDict.find( metaObject->className() );
   if( it != m_metaDict.end() )
   {
      qCritical() << "PMPrototypeManager: Class '"
                        << metaObject->className()
                        << "' already registered." << Qt::endl;
   }
   else
   {
      if( metaObject->isAbstract() )
         qCritical() << "PMPrototypeManager: The meta object for the prototype "
                           << metaObject->className()
                           << " doesn't have a factory method" << Qt::endl;

      m_prototypes.append( metaObject );
      m_lowerCaseDict.insert( metaObject->className().toLower(), metaObject->className() );

      // insert the meta object and all super classes into the hash table
      while( metaObject )
      {
			it = m_metaDict.find( metaObject->className() );
         if( it != m_metaDict.end() )
            metaObject = nullptr;
         else
         {
            m_metaDict.insert( metaObject->className(), metaObject );
            metaObject = metaObject->superClass();
         }
      }
   }
   delete obj;
}

void PMPrototypeManager::addDeclarationType( const QString& className,
                                             const QString& description,
                                             const QString& pixmap )
{
   PMMetaObject* m = metaObject( className );
   if( !m )
      qCritical() << "PMPrototypeManager::addDeclarationType: Unknown class " << className << Qt::endl;
   else
      m_declareDescriptions.push_back( PMDeclareDescription( m, description, pixmap ) );
}

const QList<PMMetaObject*>& PMPrototypeManager::prototypes() const
{
   return m_prototypes;
}

const QList<PMDeclareDescription>& PMPrototypeManager::declarationTypes() const
{
   return m_declareDescriptions;
}

PMObject* PMPrototypeManager::newObject( const QString& name ) const
{
   if( name.isEmpty() )
      return nullptr;

	QMap<QString, PMMetaObject*>::const_iterator it = m_metaDict.find( name );
   if( it == m_metaDict.end() )
      return nullptr;
   return it.value()->newObject( m_pPart );
}

PMMetaObject* PMPrototypeManager::metaObject( const QString& name ) const
{
   if( name.isNull() )
      return nullptr;
	QMap<QString, PMMetaObject*>::const_iterator it = m_metaDict.find( name );
   if( it == m_metaDict.end() )
      return nullptr;
   return it.value();
}

bool PMPrototypeManager::isA( const QString& className,
                              const QString& baseClass ) const
{
   return isA( metaObject( className ), baseClass );
}

bool PMPrototypeManager::isA( PMMetaObject* c,
                              const QString& baseClass ) const
{
   PMMetaObject* bc = metaObject( baseClass );
   while( c && c != bc )
      c = c->superClass();
   return( c && ( c == bc ) );
}

QString PMPrototypeManager::className( const QString& lowercase ) const
{
   QMap<QString, QString>::const_iterator it = m_lowerCaseDict.find( lowercase );
   if( it != m_lowerCaseDict.end() )
      return *it;
   return QString();
}
