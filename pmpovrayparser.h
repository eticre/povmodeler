//-*-C++-*-
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


#ifndef PMPOVRAYPARSER_H
#define PMPOVRAYPARSER_H



#include "pmparser.h"
#include "pmcomment.h"
#include "pmvector.h"
#include <QList>

class PMCompositeObject;
class PMGraphicalObject;
class PMBox;
class PMSphere;
class PMCylinder;
class PMCone;
class PMTorus;
class PMPlane;
class PMPolynom;
class PMDisc;
class PMBlob;
class PMBlobCylinder;
class PMBlobSphere;
class PMBicubicPatch;
class PMTriangle;
class PMCSG;
class PMHeightField;
class PMText;
class PMJuliaFractal;
class PMLathe;
class PMPrism;
class PMSurfaceOfRevolution;
class PMSuperquadricEllipsoid;
class PMScale;
class PMRotate;
class PMTranslate;
class PMBoundedBy;
class PMClippedBy;
class PMPovrayMatrix;
class PMCamera;
class PMLight;
class PMLooksLike;
class PMProjectedThrough;
class PMColor;
class PMTexture;
class PMPattern;
class PMBlendMapModifiers;
class PMWarp;
class PMPigment;
class PMNormal;
class PMTextureMap;
class PMPigmentMap;
class PMColorMap;
class PMNormalMap;
class PMBumpMap;
class PMMaterialMap;
class PMSlopeMap;
class PMDensityMap;
class PMTextureList;
class PMPigmentList;
class PMColorList;
class PMNormalList;
class PMDensityList;
class PMImageMap;
class PMSlope;
class PMFinish;
class PMInterior;
class PMMedia;
class PMDensity;
class PMMaterial;
class PMSkySphere;
class PMRainbow;
class PMFog;
class PMDeclare;
class PMObjectLink;
class PMGlobalSettings;
class PMScanner;
class PMIsoSurface;
class PMRadiosity;
class PMGlobalPhotons;
class PMPhotons;
class PMLightGroup;
class PMInteriorTexture;
class PMSphereSweep;
class PMMesh;

/**
 * Parser that parses povray code.
 *
 * All parse functions return false or 0 if an error occurred and the parse
 * function couldn't recover.
 */
class PMPovrayParser : public PMParser
{
public:
   /**
    * Parser that parses the device
    */
   PMPovrayParser( PMPart* part, QIODevice* device );
   /**
    * Parser that parses the byte array
    */
   PMPovrayParser( PMPart* part, const QByteArray& array );
   /**
    * Deletes the parser
    */
   virtual ~PMPovrayParser();

protected:
   /**
    * Top level parse function
    */
   virtual void topParse();

private:
   /**
    * Initializes the parser
    */
   void init();

   /**
    * Sets m_token to the next token
    */
   void nextToken();

   /**
    * Returns true if the current token is ON_TOK, TRUE_TOK or YES_TOK
    */
   bool isTrue() const;
   /**
    * Returns true if the current token is OFF_TOK, FALSE_TOK or NO_TOK
    */
   bool isFalse() const;
   /**
    * Returns true if the next token is no bool value or one of the
    * ON, TRUE or YES tokens
    */
   bool parseBool();


   /**
    * Looks for child objects, parses them and appends them to the parent
    * object. If parent is 0, all objects are appended to the result list.
    *
    * If max is > 0, then the maximum number of parsed objects is max.
    *
    * Returns true if there where no objects or parsing was successful.
    */
   bool parseChildObjects( PMCompositeObject* parent, int max = -1 );


   /**
    * Parses the token t.
    *
    * If the next token is not t, it appends an error to the messages
    * and returns false.
    *
    * If t is not a single character token, set tokenName to the token
    * name (like "box", "sphere" ...)
    */
   bool parseToken( int t, const QString& tokenName = QString() );

   /**
    * Parses an item of a vector, float or color expression
    */
   bool parseNumericItem( PMValue& v, bool checkForBool = false );
   /**
    * Parse function for a vector literal <x, y, z, ...>
    */
   bool parseVectorLiteral( PMVector& v );
   /**
    * Parses a vector, float or color expression
    */
   bool parseNumericExpression( PMValue& v, bool checkForBool = false );

   /**
    * parses a vector and float expression and returns a vector
    * of size s
    */
   bool parseVector( PMVector& v, unsigned int s=3 );
   /**
    * parses a vector and float expression and returns the
    * float value or the first coordinate of the vector
    */
   bool parseFloat( double& d, bool suppressError = false );
   /**
    * parses a float or int value and rounds if necessary
    */
   bool parseInt( int& d );
   /**
    * parses a color expression
    */
   bool parseColor( PMColor& c );

   /**
    * Parse function for object modifiers
    */
   bool parseObjectModifiers( PMGraphicalObject* obj );

   /**
    * Parse function for cvs objects
    */
   bool parseCSG( PMCSG* csg );

   /**
    * Parse function for box objects
    */
   bool parseBox( PMBox* box );
   /**
    * Parse function for box objects
    */
   bool parseSphere( PMSphere* sphere );
   /**
    * Parse function for cylinder objects
    */
   bool parseCylinder( PMCylinder* pNewCyl );
   /**
    * Parse function for cone objects
    */
   bool parseCone( PMCone* pNewCone );
   /**
    * Parse function for torus objects
    */
   bool parseTorus( PMTorus* pNewTorus );

   /**
    * Parse function for blob objects
    */
   bool parseBlob( PMBlob* pNewBlob );
   /**
    * Parse function for blob sphere components
    */
   bool parseBlobSphere( PMBlobSphere* pNewBlobSphere );
   /**
    * Parse function for blob cylinder components
    */
   bool parseBlobCylinder( PMBlobCylinder* pNewBlobCylinder );
   /**
    * Parse function for old blob components
    */
   bool parseBlobComponent( PMBlobSphere* pNewBlobSphere );

   /**
    * Parse function for height fields
    */
   bool parseHeightField( PMHeightField* pNewHeightField );
   /**
    * Parse function for text objects
    */
   bool parseText( PMText* pNewText );
   /**
    * Parse function for julia fractals
    */
   bool parseJuliaFractal( PMJuliaFractal* pNewFractal );

   /**
    * Parse function for plane objects
    */
   bool parsePlane( PMPlane* pNewPlane );
   /**
    * Parse function for quadric, cubic, quartic and poly
    */
   bool parsePolynom( PMPolynom* pNewPoly );
   /**
    * Parse function for bicubic patch objects
    */
   bool parseBicubicPatch( PMBicubicPatch* pNewPatch );
   /**
    * Parse function for disks
    */
   bool parseDisc( PMDisc* pNewDisc );
   /**
    * Parse function for triangles
    */
   bool parseTriangle( PMTriangle* pNewTriangle );

   /**
    * Parse function for lathes
    */
   bool parseLathe( PMLathe* pNewLathe );
   /**
    * Parse function for prisms
    */
   bool parsePrism( PMPrism* pNewPrism );
   /**
    * Parse function for surface of revolutions
    */
   bool parseSor( PMSurfaceOfRevolution* pNewSor );
   /**
    * Parse function for superquadric ellipsoid
    */
   bool parseSqe( PMSuperquadricEllipsoid* pNewSqe );

   /**
    * Parse function for scale commands
    */
   bool parseScale( PMScale* scale );
   /**
    * Parse function for rotate commands
    */
   bool parseRotate( PMRotate* rotate );
   /**
    * Parse function for translate commands
    */
   bool parseTranslate( PMTranslate* translate );
   /**
    * Parse function for matrix commands
    */
   bool parseMatrix( PMPovrayMatrix* matrix );

   /**
    * Parse function for bounded_by statements
    */
   bool parseBoundedBy( PMBoundedBy* bound );
   /**
    * Parse function for clipped_by statements
    */
   bool parseClippedBy( PMClippedBy* clipped );

   /**
    * Parse function for camera objects
    */
   bool parseCamera( PMCamera* camera );

   /**
    * Parse function for light objects
    */
   bool parseLight( PMLight* light );
   /**
    * Parse function for looks_like statement
    */
   bool parseLooksLike( PMLooksLike* ll );
   /**
    * Parse function for projected_through statement
    */
   bool parseProjectedThrough( PMProjectedThrough* ll );

   /**
    * Parse function for texture objects. If parseOuter is false, the parser
    * won't search for the texture{} wrapper. This is useful inside a texture
    * map.
    */
   bool parseTexture( PMTexture* pigment, bool parseOuter = true );
   /**
    * Parse function for pattern objects
    */
   bool parsePattern( PMPattern* pattern, bool normal = false );
   /**
    * Parse function for blend map modifier objects
    */
   bool parseBlendMapModifiers( PMBlendMapModifiers* blend );
   /**
    * Parse function for warp objects
    */
   bool parseWarp( PMWarp* pattern );
   /**
    * Parse function for pigment objects
    */
   bool parsePigment( PMPigment* pigment, bool parseOuter = true );
   /**
    * Parse function for normal objects
    */
   bool parseNormal( PMNormal* normal );
   /**
    * Parse function for texture map objects
    */
   bool parseTextureMap( PMTextureMap* textureMap );
   /**
    * Parse function for pigment map objects
    */
   bool parsePigmentMap( PMPigmentMap* pigmentMap );
   /**
    * Parse function for color map objects
    */
   bool parseColorMap( PMColorMap* colorMap );
   /**
    * Parse function for normal map objects
    */
   bool parseNormalMap( PMNormalMap* normalMap );
   /**
    * Parse function for bump map objects
    */
   bool parseBumpMap( PMBumpMap* bumpMap );
   /**
    * Parse function for material map objects
    */
   bool parseMaterialMap( PMMaterialMap* materialMap );
   /**
    * Parse function for slope map objects
    */
   bool parseSlopeMap( PMSlopeMap* slopeMap );
   /**
    * Parse function for density map objects
    */
   bool parseDensityMap( PMDensityMap* densityMap );
   /**
    * Parse function for image map objects
    */
   bool parseImageMap( PMImageMap* imageMap );
   /**
    * Parse function for slope objects
    */
   bool parseSlope( PMSlope* slope );
   /**
    * Parse function for texture list objects
    */
   bool parseTextureList( PMTextureList* textureList, int expectedItems );
   /**
    * Parse function for pigment list objects
    */
   bool parsePigmentList( PMPigmentList* pigmentList, int expectedItems );
   /**
    * Parse function for color list objects
    */
   bool parseColorList( PMColorList* colorList, int expectedItems );
   /**
    * Parse function for normal list objects
    */
   bool parseNormalList( PMNormalList* normalList, int expectedItems );
   /**
    * Parse function for density list objects
    */
   bool parseDensityList( PMDensityList* densityList, int expectedItems );
   /**
    * Parse function for finish objects
    */
   bool parseFinish( PMFinish* finish );
   /**
    * Parse function for interior objects
    */
   bool parseInterior( PMInterior* interior );
   /**
    * Parse function for media objects
    */
   bool parseMedia( PMMedia* media );
   /**
    * Parse function for density objects
    */
   bool parseDensity( PMDensity* density );
   /**
    * Parse function for material objects
    */
   bool parseMaterial( PMMaterial* material );

   /**
    * Parse function for sky sphere objects
    */
   bool parseSkySphere( PMSkySphere* sky );
   /**
    * Parse function for rainbow objects
    */
   bool parseRainbow( PMRainbow* rainbow );
   /**
    * Parse function for fog objects
    */
   bool parseFog( PMFog* fog );

   /**
    * Parse function for global settings
    */
   bool parseGlobalSettings( PMGlobalSettings* decl );
   /**
    * Parse function for declares
    */
   bool parseDeclare( PMDeclare* decl );
   /**
    * Parse function for object links
    */
   bool parseObjectLink( PMObjectLink* link );
   /**
    * Parse function for object keywords
    */
   bool parseObject( PMCompositeObject* parent );

   // POV-Ray 3.5 objects
   bool parseIsoSurface( PMIsoSurface* iso );
   bool parseRadiosity( PMRadiosity* rad );
   bool parseGlobalPhotons( PMGlobalPhotons* gp );
   bool parsePhotons( PMPhotons* p );
   bool parseLightGroup( PMLightGroup* lg );
   bool parseInteriorTexture( PMInteriorTexture* it );
   bool parseSphereSweep( PMSphereSweep * ss );
   bool parseMesh( PMMesh* m );

   /**
    * The used scanner
    */
   PMScanner* m_pScanner;
   /**
    * The last scanned token
    */
   int m_token;

   /**
    * Number of consumed tokens.
    */
   int m_consumedTokens;

   /**
    * All comments are skipped during parsing and stored here
    */
   QList<PMComment*> m_skippedComments;
   /**
    * The last skipped comment text with special povmodeler tags*/
   QString m_lastPMComment;
   /**
    * true if m_lastPMComment is empty
    */
   bool m_bLastPMCommentEmpty;

   bool srgb;
};


#endif
