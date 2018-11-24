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


#include "pmpovrayparser.h"



#include "pmpart.h"
#include "pmscanner.h"
#include "pmtokens.h"

#include "pmcolor.h"
#include "pmallobjects.h"
#include "pmprototypemanager.h"
#include "pmxmlhelper.h"


PMPovrayParser::PMPovrayParser( PMPart* part, QIODevice* dev )
      : PMParser( part, dev )
{
   init();
}

PMPovrayParser::PMPovrayParser( PMPart* part, const QByteArray& array )
      : PMParser( part, array )
{
   init();
}

PMPovrayParser::~PMPovrayParser()
{
   if( m_pScanner )
      delete m_pScanner;
	foreach(PMComment* c, m_skippedComments) delete c;
}

void PMPovrayParser::init()
{
   m_pScanner = new PMScanner( m_pDevice );
   m_consumedTokens = 0;
   m_bLastPMCommentEmpty = true;
}


void PMPovrayParser::nextToken()
{
   m_token = m_pScanner->nextToken();
   m_consumedTokens++;
   setCurrentLine( m_pScanner->currentLine() );

   if( ( m_token == SCANNER_ERROR_TOK ) || ( m_token == COMMENT_TOK )
          || ( m_token == LINE_COMMENT_TOK ) || ( m_token == PMNAME_TOK ) )
   {
      // create the objects (string) only if necessary
      PMComment* c;
      int lastCommentLine = -2;
      QString commentText;

      while( ( m_token == SCANNER_ERROR_TOK ) || ( m_token == COMMENT_TOK )
             || ( m_token == LINE_COMMENT_TOK ) || ( m_token == PMNAME_TOK ) )
      {
         switch( m_token )
         {
            case SCANNER_ERROR_TOK:
               printError( m_pScanner->error() );
               lastCommentLine = -2;
               break;
            case LINE_COMMENT_TOK:
               commentText = m_pScanner->sValue();
               if( lastCommentLine == ( m_pScanner->currentLine() - 1 ) )
               {
                  c = m_skippedComments.isEmpty() ? nullptr : m_skippedComments.last();
                  if( c )
                     c->setText( c->text() + '\n' + commentText );
                  else
                  {
                     c = new PMComment( m_pPart, commentText );
                     m_skippedComments.append( c );
                  }
               }
               else
               {
                  c = new PMComment( m_pPart, m_pScanner->sValue() );
                  m_skippedComments.append( c );
               }
               lastCommentLine = m_pScanner->currentLine();
               break;
            case COMMENT_TOK:
               c = new PMComment( m_pPart, m_pScanner->sValue() );
               m_skippedComments.append( c );
               lastCommentLine = -2;
               break;
            case PMNAME_TOK:
               // Special comment
               m_lastPMComment = m_pScanner->sValue();
               m_bLastPMCommentEmpty = false;
               lastCommentLine = -2;
               break;
            default:
               lastCommentLine = -2;
               break;
         }

         m_token = m_pScanner->nextToken();
         m_consumedTokens++;
      }
   }
}

bool PMPovrayParser::isTrue() const
{
   if( ( m_token == ON_TOK ) || ( m_token == TRUE_TOK ) || ( m_token == YES_TOK ) )
      return true;
   return false;
}

bool PMPovrayParser::isFalse() const
{
   if( ( m_token == OFF_TOK ) || ( m_token == FALSE_TOK ) || ( m_token == NO_TOK ) )
      return true;
   return false;
}

void PMPovrayParser::topParse()
{
   nextToken();

   do
   {
      if( !parseChildObjects( nullptr ) )
         m_token = EOF_TOK;
      if( m_token != EOF_TOK )
      {
         printUnexpected( m_pScanner->sValue() );
         nextToken();
      }
   }
   while( m_token != EOF_TOK );

   if( errors() || warnings() )
      printMessage( PMMSpecialRawComment );
}

bool PMPovrayParser::parseBool()
{
   if( isFalse() )
   {
      nextToken();
      return false;
   }
   if( isTrue() )
   {
      nextToken();
      return true;
   }

   PMValue v;

   if( parseNumericExpression( v, true ) )
   {
      switch( v.type() )
      {
         case PMVFloat:
            return v.floatValue() > 0.0;
         case PMVVector:
            return ( v.vector() )[0] > 0.0;
         default:
            printError( ( "Boolean expression expected" ) );
            break;
      }
   }

   return true;
}

bool PMPovrayParser::parseChildObjects( PMCompositeObject* parent,
                                        int max /* = -1 */ )
{
   PMObject* child = nullptr;
   bool finished = false;
   bool error = false;
   bool noChild = false;
   int numParsed = 0;
   srgb = false;

   do
   {
      if( !m_bLastPMCommentEmpty && parent )
      {
         if( parent->isA( "NamedObject" ) )
            ( ( PMNamedObject* ) parent )->setName( m_lastPMComment );
         m_bLastPMCommentEmpty = true;
      }
      if( m_skippedComments.count() > 0 )
		{
			child = m_skippedComments.first();
			m_skippedComments.pop_front();
		}
      else
      {
         child = nullptr;
         noChild = false;

         // some objects
         switch( m_token )
         {
            case UNION_TOK:
            case DIFFERENCE_TOK:
            case INTERSECTION_TOK:
            case MERGE_TOK:
               child = new PMCSG( m_pPart );
               error = !parseCSG( ( PMCSG* ) child );
               break;
            case BOX_TOK:
               child = new PMBox( m_pPart );
               error = !parseBox( ( PMBox* ) child );
               break;
            case SPHERE_TOK:
               if( ( parent && ( parent->type() == "Blob" ) )
                   || ( !parent && m_pTopParent
                        && ( m_pTopParent->type() == "Blob" ) ) )
               {
                  child = new PMBlobSphere( m_pPart );
                  error = !parseBlobSphere( ( PMBlobSphere* ) child );
               }
               else
               {
                  child = new PMSphere( m_pPart );
                  error = !parseSphere( ( PMSphere* ) child );
               }
               break;
            case CYLINDER_TOK:
               if( ( parent && ( parent->type() == "Blob" ) )
                   || ( !parent && m_pTopParent
                        && ( m_pTopParent->type() == "Blob" ) ) )
               {
                  child = new PMBlobCylinder( m_pPart );
                  error = !parseBlobCylinder( ( PMBlobCylinder* ) child );
               }
               else
               {
                  child = new PMCylinder( m_pPart );
                  error = !parseCylinder( ( PMCylinder* ) child );
               }
               break;
            case CONE_TOK:
               child = new PMCone( m_pPart );
               error = !parseCone( ( PMCone* ) child );
               break;
            case TORUS_TOK:
               child = new PMTorus( m_pPart );
               error = !parseTorus( ( PMTorus* ) child );
               break;
            case BLOB_TOK:
               child = new PMBlob( m_pPart );
               error = !parseBlob( ( PMBlob* ) child );
               break;
            case COMPONENT_TOK:
               child = new PMBlobSphere( m_pPart );
               error = !parseBlobComponent( ( PMBlobSphere* ) child );
               break;
            case HEIGHT_FIELD_TOK:
               child = new PMHeightField( m_pPart );
               error = !parseHeightField( ( PMHeightField* ) child );
               break;
            case TEXT_TOK:
               child = new PMText( m_pPart );
               error = !parseText( ( PMText* ) child );
               break;
            case JULIA_FRACTAL_TOK:
               child = new PMJuliaFractal( m_pPart );
               error = !parseJuliaFractal( ( PMJuliaFractal* ) child );
               break;
            case PLANE_TOK:
               child = new PMPlane( m_pPart );
               error = !parsePlane( ( PMPlane* ) child );
               break;
            case QUADRIC_TOK:
            case CUBIC_TOK:
            case QUARTIC_TOK:
            case POLY_TOK:
               child = new PMPolynom( m_pPart );
               error = !parsePolynom( ( PMPolynom* ) child );
               break;
            case BICUBIC_PATCH_TOK:
               child = new PMBicubicPatch( m_pPart );
               error = !parseBicubicPatch( ( PMBicubicPatch* ) child );
               break;
            case DISC_TOK:
               child = new PMDisc( m_pPart );
               error = !parseDisc( ( PMDisc* ) child );
               break;
            case TRIANGLE_TOK:
            case SMOOTH_TRIANGLE_TOK:
               child = new PMTriangle( m_pPart );
               error = !parseTriangle( ( PMTriangle* ) child );
               break;
            case LATHE_TOK:
               child = new PMLathe( m_pPart );
               error = !parseLathe( ( PMLathe* ) child );
               break;
            case PRISM_TOK:
               child = new PMPrism( m_pPart );
               error = !parsePrism( ( PMPrism* ) child );
               break;
            case SOR_TOK:
               child = new PMSurfaceOfRevolution( m_pPart );
               error = !parseSor( ( PMSurfaceOfRevolution* ) child );
               break;
            case SUPERELLIPSOID_TOK:
               child = new PMSuperquadricEllipsoid( m_pPart );
               error = !parseSqe( ( PMSuperquadricEllipsoid* ) child );
               break;
            case CAMERA_TOK:
               child = new PMCamera( m_pPart );
               error = !parseCamera( ( PMCamera* ) child );
               break;
            case LIGHT_SOURCE_TOK:
               child = new PMLight( m_pPart );
               error = !parseLight( ( PMLight* ) child );
               break;
            case LOOKS_LIKE_TOK:
               child = new PMLooksLike( m_pPart );
               error = !parseLooksLike( ( PMLooksLike* ) child );
               break;
            case PROJECTED_THROUGH_TOK:
               child = new PMProjectedThrough( m_pPart );
               error = !parseProjectedThrough( ( PMProjectedThrough* ) child );
               break;
            case TEXTURE_TOK:
               child = new PMTexture( m_pPart );
               error = !parseTexture( ( PMTexture* ) child );
               break;
            case AGATE_TOK:
            case AVERAGE_TOK:
            case BOXED_TOK:
            case BOZO_TOK:
            case BUMPS_TOK:
            case CELLS_TOK:
            case CRACKLE_TOK:
            case CYLINDRICAL_TOK:
            case DENTS_TOK:
            case DENSITY_FILE_TOK:
            case GRADIENT_TOK:
            case GRANITE_TOK:
            case JULIA_TOK:
            case LEOPARD_TOK:
            case MAGNET_TOK:
            case MANDEL_TOK:
            case MARBLE_TOK:
            case ONION_TOK:
            case PLANAR_TOK:
            case QUILTED_TOK:
            case RADIAL_TOK:
            case RIPPLES_TOK:
            case SLOPE_TOK:
            case SPHERICAL_TOK:
            case SPIRAL1_TOK:
            case SPIRAL2_TOK:
            case TILING_TOK:
            case SPOTTED_TOK:
            case WOOD_TOK:
            case WAVES_TOK:
            case WRINKLES_TOK:
               child = new PMPattern( m_pPart );
               {
                  bool normal = true;
                  if( parent && ( parent->type() != "Normal" ) )
                     normal = false;
                  error = !parsePattern( ( PMPattern* ) child, normal );
               }
               break;
            case TURBULENCE_TOK:
               //  Search for a PMPattern in the object's children
               child = parent->firstChild();
               while( child && !child->isA( "Pattern" ) )
                  child = child->nextSibling();
               if( child )
               {
                  error = !parsePattern( ( PMPattern* ) child );
                  child = 0;
                  noChild = true;
               }
               else
               {
                  printError( ( "Found turbulence without a pattern." ) );
                  error = true;
               }
               break;
            case FREQUENCY_TOK:
            case PHASE_TOK:
            case RAMP_WAVE_TOK:
            case TRIANGLE_WAVE_TOK:
            case SINE_WAVE_TOK:
            case SCALLOP_WAVE_TOK:
            case CUBIC_WAVE_TOK:
            case POLY_WAVE_TOK:
               //  Search for a PMBlendMapModifiers in the object's children
               child = parent->firstChild();
               while( child && !child->isA( "BlendMapModifiers" ) )
                  child = child->nextSibling();
               if( child )
               {
                  error = !parseBlendMapModifiers( ( PMBlendMapModifiers* ) child );
                  child = 0;
                  noChild = 0;
               }
               else
               {
                  child = new PMBlendMapModifiers( m_pPart );
                  error = !parseBlendMapModifiers( ( PMBlendMapModifiers* ) child );
               }
               break;
            case WARP_TOK:
               child = new PMWarp( m_pPart );
               error = !parseWarp( ( PMWarp* ) child );
               break;
            case PIGMENT_TOK:
               child = new PMPigment( m_pPart );
               error = !parsePigment( ( PMPigment* ) child );
               break;
            case NORMAL_TOK:
               child = new PMNormal( m_pPart );
               error = !parseNormal( ( PMNormal* ) child );
               break;
            case NORMAL_MAP_TOK:
               child = new PMNormalMap( m_pPart );
               error = !parseNormalMap( ( PMNormalMap* ) child );
               break;
            case BUMP_MAP_TOK:
               child = new PMBumpMap( m_pPart );
               error = !parseBumpMap( ( PMBumpMap* ) child );
               break;
            case SLOPE_MAP_TOK:
               child = new PMSlopeMap( m_pPart );
               error = !parseSlopeMap( ( PMSlopeMap* ) child );
               break;
            case DENSITY_MAP_TOK:
               child = new PMDensityMap( m_pPart );
               error = !parseDensityMap( ( PMDensityMap* ) child );
               break;
            case TEXTURE_MAP_TOK:
               child = new PMTextureMap( m_pPart );
               error = !parseTextureMap( ( PMTextureMap* ) child );
               break;
            case MATERIAL_MAP_TOK:
               child = new PMMaterialMap( m_pPart );
               error = !parseMaterialMap( ( PMMaterialMap* ) child );
               break;
            case PIGMENT_MAP_TOK:
               child = new PMPigmentMap( m_pPart );
               error = !parsePigmentMap( ( PMPigmentMap* ) child );
               break;
            case COLOR_MAP_TOK:
            case COLOUR_MAP_TOK:
               child = new PMColorMap( m_pPart );
               error = !parseColorMap( ( PMColorMap* ) child );
               break;
            case CHECKER_TOK:
            case HEXAGON_TOK:
            case BRICK_TOK:
            {
               bool normal = false;
               double depth = 0.0;
               int expect = 0;
               PMListPattern::PMListType type = PMListPattern::ListPatternChecker;

               if( parent && parent->type() == "Normal" )
                  normal = true;
               else if( m_pTopParent && m_pTopParent->type() == "Normal" )
                  normal = true;

               switch( m_token )
               {
                  case CHECKER_TOK:
                     type = PMListPattern::ListPatternChecker;
                     expect = 2;
                     break;
                  case HEXAGON_TOK:
                     type = PMListPattern::ListPatternHexagon;
                     expect = 3;
                     break;
                  case BRICK_TOK:
                     type = PMListPattern::ListPatternBrick;
                     expect = 2;
                     break;
               }
               nextToken();

               if( normal )
               {
                  child = new PMNormalList( m_pPart );
                  if( parseFloat( depth, true ) )
                     ( ( PMNormalList* ) child )->setDepth( depth );

                  if( m_token == NORMAL_TOK )
                     error = !parseNormalList( ( PMNormalList* ) child, expect );
               }
               else
               {
                  switch( m_token )
                  {
                     case COLOR_TOK:
                     case COLOUR_TOK:
                     case RGB_TOK:
                     case SRGB_TOK:
                     case RGBT_TOK:
                     case RGBF_TOK:
                     case RGBFT_TOK:
                     case RED_TOK:
                     case GREEN_TOK:
                     case BLUE_TOK:
                     case TRANSMIT_TOK:
                     case FILTER_TOK:
                     case ID_TOK:
                        child = new PMColorList( m_pPart );
                        error = !parseColorList( ( PMColorList* ) child, expect );
                        break;
                     case PIGMENT_TOK:
                        child = new PMPigmentList( m_pPart );
                        error = !parsePigmentList( ( PMPigmentList* ) child, expect );
                        break;
                     case TEXTURE_TOK:
                        child = new PMTextureList( m_pPart );
                        error = !parseTextureList( ( PMTextureList* ) child, expect );
                        break;
                     case NORMAL_TOK:
                        child = new PMNormalList( m_pPart );
                        error = !parseNormalList( ( PMNormalList* ) child, expect );
                        break;
                     case DENSITY_TOK:
                        child = new PMDensityList( m_pPart );
                        error = !parseDensityList( ( PMDensityList* ) child, expect );
                        break;
                     default:
                        printError( ( "Invalid list member." ) );
                        error = true;
                  }
               }

               if( child )
               {
                  ( ( PMListPattern* ) child )->setListType( type );

                  int oldConsumed;
                  double num = 0;
                  PMVector vector;

                  do
                  {
                     oldConsumed = m_consumedTokens;
                     switch( m_token )
                     {
                        case MORTAR_TOK:
                           nextToken();
                           if( !parseFloat( num ) )
                              return false;
                           ( ( PMListPattern* ) child )->setMortar( num );
                           break;
                        case BRICK_SIZE_TOK:
                           nextToken();
                           if( !parseVector( vector ) )
                              return false;
                           ( ( PMListPattern* ) child )->setBrickSize( vector );
                           break;
                        default:
                           break;
                     }
                  }
                  while( oldConsumed != m_consumedTokens );
               }
               break;
            }
            case IMAGE_MAP_TOK:
               child = new PMImageMap( m_pPart );
               error = !parseImageMap( ( PMImageMap* ) child );
               break;
            case FINISH_TOK:
               child = new PMFinish( m_pPart );
               error = !parseFinish( ( PMFinish* ) child );
               break;
            case INTERIOR_TOK:
               child = new PMInterior( m_pPart );
               error = !parseInterior( ( PMInterior* ) child );
               break;
            case MEDIA_TOK:
               child = new PMMedia( m_pPart );
               error = !parseMedia( ( PMMedia* ) child );
               break;
            case DENSITY_TOK:
               child = new PMDensity( m_pPart );
               error = !parseDensity( ( PMDensity* ) child );
               break;
            case MATERIAL_TOK:
               child = new PMMaterial( m_pPart );
               error = !parseMaterial( ( PMMaterial* ) child );
               break;
            case SKY_SPHERE_TOK:
               child = new PMSkySphere( m_pPart );
               error = !parseSkySphere( ( PMSkySphere* ) child );
               break;
            case RAINBOW_TOK:
               child = new PMRainbow( m_pPart );
               error = !parseRainbow( ( PMRainbow* ) child );
               break;
            case FOG_TOK:
               child = new PMFog( m_pPart );
               error = !parseFog( ( PMFog* ) child );
               break;
            case GLOBAL_SETTINGS_TOK:
               child = new PMGlobalSettings( m_pPart );
               error = !parseGlobalSettings( ( PMGlobalSettings* ) child );
               break;
            case SCALE_TOK:
               child = new PMScale( m_pPart );
               error = !parseScale( ( PMScale* ) child );
               break;
            case ROTATE_TOK:
               child = new PMRotate( m_pPart );
               error = !parseRotate( ( PMRotate* ) child );
               break;
            case TRANSLATE_TOK:
               child = new PMTranslate( m_pPart );
               error = !parseTranslate( ( PMTranslate* ) child );
               break;
            case MATRIX_TOK:
               child = new PMPovrayMatrix( m_pPart );
               error = !parseMatrix( ( PMPovrayMatrix* ) child );
               break;
            case BOUNDED_BY_TOK:
               if( parent && ( parent->type() == "ClippedBy" ) )
                  finished = true;
               else
               {
                  child = new PMBoundedBy( m_pPart );
                  error = !parseBoundedBy( ( PMBoundedBy* ) child );
               }
               break;
            case CLIPPED_BY_TOK:
               if( parent && ( parent->type() == "BoundedBy" ) )
                  finished = true;
               else
               {
                  child = new PMClippedBy( m_pPart );
                  error = !parseClippedBy( ( PMClippedBy* ) child );
               }
               break;
            case ISOSURFACE_TOK:
               child = new PMIsoSurface( m_pPart );
               error = !parseIsoSurface( ( PMIsoSurface* ) child );
               break;
            case RADIOSITY_TOK:
               child = new PMRadiosity( m_pPart );
               error = !parseRadiosity( ( PMRadiosity* ) child );
               break;
            case PHOTONS_TOK:
               if ( parent && ( parent->type() == "GlobalSettings" ) )
               {
                  child = new PMGlobalPhotons( m_pPart );
                  error = !parseGlobalPhotons( ( PMGlobalPhotons* ) child );
               }
               else
               {
                  child = new PMPhotons( m_pPart );
                  error =!parsePhotons( ( PMPhotons* ) child );
               }
               break;
            case LIGHT_GROUP_TOK:
               child = new PMLightGroup( m_pPart );
               error = !parseLightGroup( ( PMLightGroup* ) child );
               break;
            case INTERIOR_TEXTURE_TOK:
               child = new PMInteriorTexture( m_pPart );
               error = !parseInteriorTexture( ( PMInteriorTexture* ) child );
               break;
            case SPHERE_SWEEP_TOK:
               child = new PMSphereSweep( m_pPart );
               error = !parseSphereSweep( ( PMSphereSweep* ) child );
               break;
            case MESH_TOK:
               child = new PMMesh( m_pPart );
               error = !parseMesh( ( PMMesh* ) child );
               break;
            case DECLARE_TOK:
               nextToken();
               if( m_token == ID_TOK )
               {
                  QString id( m_pScanner->sValue() );
                  nextToken();

                  if( !parseToken( '=' ) )
                     error = true;
                  else
                  {
                     PMValue v;
                     switch( m_token )
                     {
                        case OBJECT_TOK:
                           // finite solid
                        case BLOB_TOK:
                        case BOX_TOK:
                        case CONE_TOK:
                        case CYLINDER_TOK:
                        case HEIGHT_FIELD_TOK:
                        case JULIA_FRACTAL_TOK:
                        case LATHE_TOK:
                        case PRISM_TOK:
                        case SPHERE_TOK:
                        case SUPERELLIPSOID_TOK:
                        case SOR_TOK:
                        case TEXT_TOK:
                        case TORUS_TOK:
                        case ISOSURFACE_TOK:
                        case SPHERE_SWEEP_TOK:
                           // finite patch
                        case BICUBIC_PATCH_TOK:
                        case DISC_TOK:
                        case MESH_TOK:
                        case POLYGON_TOK:
                        case TRIANGLE_TOK:
                        case SMOOTH_TRIANGLE_TOK:
                           // infinite solid
                        case PLANE_TOK:
                        case QUADRIC_TOK:
                        case CUBIC_TOK:
                        case QUARTIC_TOK:
                        case POLY_TOK:
                           // csg
                        case UNION_TOK:
                        case INTERSECTION_TOK:
                        case DIFFERENCE_TOK:
                        case MERGE_TOK:
                           // textures
                        case TEXTURE_TOK:
                        case INTERIOR_TEXTURE_TOK:
                        case PIGMENT_TOK:
                        case NORMAL_TOK:
                        case FINISH_TOK:
                        case TEXTURE_MAP_TOK:
                        case PIGMENT_MAP_TOK:
                        case COLOR_MAP_TOK:
                        case COLOUR_MAP_TOK:
                        case NORMAL_MAP_TOK:
                        case SLOPE_MAP_TOK:
                        case DENSITY_MAP_TOK:
                        case INTERIOR_TOK:
                        case MEDIA_TOK:
                        case DENSITY_TOK:
                        case MATERIAL_TOK:
                        case SKY_SPHERE_TOK:
                        case RAINBOW_TOK:
                        case FOG_TOK:
                           // misc
                        case LIGHT_SOURCE_TOK:
                        case LIGHT_GROUP_TOK:
                           child = new PMDeclare( m_pPart );
                           error = !parseDeclare( ( PMDeclare* ) child );
                           break;
                        default:
                           // constant, vector or color declare?
                           if( parseNumericExpression( v ) )
                           {
                              checkID( id, v );
                              noChild = true;
                           }
                           else
                              error = true;
                           break;
                     }
                  }

                  if( child )
                  {
                     if( child->isA( "Declare" ) )
                        ( ( PMDeclare* ) child )->setID( id );
                     if( id.startsWith('_'))//eticre view<-->hide not_in_tree
                        ( ( PMDeclare* ) child )->setTreeVisibility(true);
                   }
                  if( m_token == ';' )
                     nextToken();
               }
               else
                  printExpected( ( "identifier" ), m_pScanner->sValue() );
               break;
            case OBJECT_TOK:
               error = !parseObject( parent );
               noChild = true;
               break;
            case RAW_POVRAY_TOK:
               child = new PMRaw( m_pPart, m_pScanner->sValue() );
               error = false;
               nextToken();
               break;
            default:
               finished = true;
               break;
         }
      }
      if( !finished && !child && !noChild )
         error = true;
      if( child )
      {
         if( !insertChild( child, parent ) )
         {
            delete child;
            child = 0;
         }
         else if( child->isA( "Declare" ) )
            checkID( ( PMDeclare* ) child );
         numParsed ++;
         if( ( max > 0 ) && ( numParsed >= max ) )
            finished = true;
      }
   }
   while( !finished && !error );

   return finished;
}



bool PMPovrayParser::parseToken( int t, const QString& tokenName )
{
   if( t == ',' )
   {
      // do not require commas any more.
       // eticre mod ... not return always true but only if token=comma
      if( m_token == ',' )
      {
         nextToken();
         return true;
      }
      else
         return false;
   }
   else if( m_token == t )
   {
      nextToken();
      return true;
   }

   if( tokenName.isNull() )
      printExpected( ( char ) t, m_pScanner->sValue() );
   else
      printExpected( tokenName, m_pScanner->sValue() );

   return false;
}

/*
bool PMPovrayParser::parseToken( int t, const QString& tokenName )
{
   if( t == ',' )
   {
      // do not require commas any more.
      if( m_token == ',' )
         nextToken();
      return true;
   }
   else if( m_token == t )
   {
      nextToken();
      return true;
   }

   if( tokenName.isNull() )
      printExpected( ( char ) t, m_pScanner->sValue() );
   else
      printExpected( tokenName, m_pScanner->sValue() );
   return false;
}
*/

bool PMPovrayParser::parseNumericItem( PMValue& v, bool checkForBool /*=false*/ )
{
   bool finishColor = false;
   PMVector cv( 0 );
   PMVector vec( 0 );
   PMValue hv;
   PMSymbol* s;
   int i;

   switch( m_token )
   {
      case X_TOK:
         v.setVector( PMVector( 1.0, 0.0, 0.0 ) );
         nextToken();
         break;
      case Y_TOK:
         v.setVector( PMVector( 0.0, 1.0, 0.0 ) );
         nextToken();
         break;
      case Z_TOK:
         v.setVector( PMVector( 0.0, 0.0, 1.0 ) );
         nextToken();
         break;
      case T_TOK:
         v.setVector( PMVector( 0.0, 0.0, 0.0, 1.0 ) );
         nextToken();
         break;
      case U_TOK:
         v.setVector( PMVector( 1.0, 0.0 ) );
         nextToken();
         break;
      case V_TOK:
         v.setVector( PMVector( 0.0, 1.0 ) );
         nextToken();
         break;
      case PI_TOK:
         v.setFloat( 3.1415926535897932384626 );
         nextToken();
         break;
      case CLOCK_TOK:
         printMessage( PMMClockDefault );
         v.setFloat( 0.0 );
         break;
      case CLOCK_DELTA_TOK:
         printMessage( PMMClockDeltaDefault );
         v.setFloat( 1.0 );
         break;
      case FLOAT_TOK:
         v.setFloat( m_pScanner->fValue() );
         nextToken();
         break;
      case INTEGER_TOK:
         v.setFloat( ( double ) m_pScanner->iValue() );
         nextToken();
         break;
      case ON_TOK:
      case TRUE_TOK:
      case YES_TOK:
         v.setFloat( 1.0 );
         nextToken();
         break;
      case OFF_TOK:
      case FALSE_TOK:
      case NO_TOK:
         v.setFloat( 0.0 );
         nextToken();
         break;
      case '(':
         nextToken();
         if( !parseNumericExpression( v ) )
            return false;
         if( !parseToken( ')' ) )
            return false;
         break;
      case '-':
         nextToken();
         if( !parseNumericItem( v ) )
            return false;
         if( v.type() == PMVFloat )
            v.setFloat( -v.floatValue() );
         else
            v.setVector( -v.vector() );
         break;
      case '+':
         nextToken();
         if( !parseNumericItem( v ) )
            return false;
         break;
      case '<':
         if( !parseVectorLiteral( vec ) )
            return false;
         v.setVector( vec );
         break;
      case COLOR_TOK:
      case COLOUR_TOK:
         nextToken();
         // fall through
      case RGB_TOK:
      case SRGB_TOK:
      case RGBT_TOK:
      case RGBF_TOK:
      case RGBFT_TOK:
      case RED_TOK:
      case GREEN_TOK:
      case BLUE_TOK:
      case TRANSMIT_TOK:
      case FILTER_TOK:
         if( m_token == SRGB_TOK )
            srgb = true;
         cv.resize( 5 );
         cv = 0.0;
         finishColor = true;
         break;
      case ID_TOK:
         s = getSymbol( m_pScanner->sValue() );
         if( s )
         {
            nextToken();
            if( s->type() == PMSymbol::Value )
               v = s->value();
            else
            {
               printError( ( "Float, color or vector identifier expected." ) );
               return false;
            }
         }
         else
         {
            printError( ( "Undefined identifier ." + QString( m_pScanner->sValue() ) ) );
            nextToken();
         }
         break;
      default:
         if( !checkForBool )
            printUnexpected( m_pScanner->sValue() );
         return false;
         break;
   }

   if( !finishColor )
   {
      if( m_token == '.' )
      {
         int index = -1;
         nextToken();

         switch( m_token )
         {
            case X_TOK:
            case RED_TOK:
            case U_TOK:
               index = 0;
               break;
            case Y_TOK:
            case GREEN_TOK:
            case V_TOK:
               index = 1;
               break;
            case Z_TOK:
            case BLUE_TOK:
               index = 2;
               break;
            case T_TOK:
            case FILTER_TOK:
               index = 3;
               break;
            case TRANSMIT_TOK:
               index = 4;
               break;
            default:
               break;
         }
         if( index >= 0 )
         {
            nextToken();
            if( v.type() == PMVFloat )
            {
               if( index != 0 )
                  index = -1;
            }
            else
            {
               PMVector vec;
               if( v.type() == PMVVector )
                  vec = v.vector();
               else
                  vec = v.color();

               if( ( index ) < vec.size() )
                  v.setFloat( vec[index] );
               else
                  index = -1;
            }
         }
         if( index == -1 )
         {
            printError( ( "Bad operands for period operator." ) );
            return false;
         }
      }
   }

   while( finishColor )
   {
      switch( m_token )
      {
         case RGB_TOK:
         case SRGB_TOK:
            nextToken();
            if( !parseNumericExpression( hv ) )
               return false;
            switch( hv.type() )
            {
               case PMVFloat:
                  cv[0] = hv.floatValue();
                  cv[1] = hv.floatValue();
                  cv[2] = hv.floatValue();
                  break;
               case PMVVector:
                  vec = hv.vector();
                  vec.resize( 3 );
                  cv[0] = vec[0];
                  cv[1] = vec[1];
                  cv[2] = vec[2];
                  break;
               default:
                  printError( ( "Float or vector expression expected" ) );
                  break;
            }
            break;
         case RGBT_TOK:
            nextToken();
            if( !parseNumericExpression( hv ) )
               return false;
            switch( hv.type() )
            {
               case PMVFloat:
                  cv[0] = hv.floatValue();
                  cv[1] = hv.floatValue();
                  cv[2] = hv.floatValue();
                  cv[4] = hv.floatValue();
                  break;
               case PMVVector:
                  vec = hv.vector();
                  vec.resize( 4 );
                  cv[0] = vec[0];
                  cv[1] = vec[1];
                  cv[2] = vec[2];
                  cv[4] = vec[3];
                  break;
               default:
                  printError( ( "Float or vector expression expected" ) );
                  break;
            }
            break;
         case RGBF_TOK:
            nextToken();
            if( !parseNumericExpression( hv ) )
               return false;
            switch( hv.type() )
            {
               case PMVFloat:
                  cv[0] = hv.floatValue();
                  cv[1] = hv.floatValue();
                  cv[2] = hv.floatValue();
                  cv[3] = hv.floatValue();
                  break;
               case PMVVector:
                  vec = hv.vector();
                  vec.resize( 4 );
                  cv[0] = vec[0];
                  cv[1] = vec[1];
                  cv[2] = vec[2];
                  cv[3] = vec[3];
                  break;
               default:
                  printError( ( "Float or vector expression expected" ) );
                  break;
            }
            break;
         case RGBFT_TOK:
            nextToken();
            if( !parseNumericExpression( hv ) )
               return false;
            switch( hv.type() )
            {
               case PMVFloat:
                  cv = hv.floatValue();
                  break;
               case PMVVector:
                  vec = hv.vector();
                  vec.resize( 5 );
                  cv = vec;
                  break;
               default:
                  printError( ( "Float or vector expression expected" ) );
                  break;
            }
            break;
         case RED_TOK:
            nextToken();
            parseNumericExpression( hv );
            if( hv.type() != PMVFloat )
            {
               printError( ( "Float expression expected" ) );
               break;
            }
            cv[0] = hv.floatValue();
            break;
         case GREEN_TOK:
            nextToken();
            parseNumericExpression( hv );
            if( hv.type() != PMVFloat )
            {
               printError( ( "Float expression expected" ) );
               break;
            }
            cv[1] = hv.floatValue();
            break;
         case BLUE_TOK:
            nextToken();
            parseNumericExpression( hv );
            if( hv.type() != PMVFloat )
            {
               printError( ( "Float expression expected" ) );
               break;
            }
            cv[2] = hv.floatValue();
            break;
         case FILTER_TOK:
         case ALPHA_TOK:
            nextToken();
            parseNumericExpression( hv );
            if( hv.type() != PMVFloat )
            {
               printError( ( "Float expression expected" ) );
               break;
            }
            cv[3] = hv.floatValue();
            break;
         case TRANSMIT_TOK:
            nextToken();
            parseNumericExpression( hv );
            if( hv.type() != PMVFloat )
            {
               printError( ( "Float expression expected" ) );
               break;
            }
            cv[4] = hv.floatValue();
            break;
         case ID_TOK:
            if( parseNumericItem( hv ) )
            {
               if( hv.type() == PMVFloat )
               {
                  for( i = 0; i < 5; i++ )
                     cv[i] = hv.floatValue();
               }
               else if( hv.type() == PMVVector )
               {
                  cv = hv.vector();
                  cv.resize( 5 );
               }
               else
                  cv = hv.color();
            }
            break;
         default:
            finishColor = false;
            v.setColor( cv );
            break;
      }
   }

   return true;
}

bool PMPovrayParser::parseVectorLiteral( PMVector& p )
{
   PMValue v;

   if( !parseToken( '<' ) )
      return false;
   if( !parseNumericExpression( v ) )
      return false;

   if( v.type() != PMVFloat )
   {
      printError( ( "Float expression expected" ) );
      return false;
   }

   p.resize( 1 );
   p[0] = v.floatValue();

   while( m_token != '>' )
   {
      // many old scenes do not use a comma between values
      if( m_token == ',' )
         nextToken();
      // parseToken( ',' );
      if( !parseNumericExpression( v ) )
         return false;

      if( v.type() != PMVFloat )
      {
         printError( ( "Float expression expected" ) );
         return false;
      }

      p.resize( p.size() + 1 );
      p[p.size() - 1] = v.floatValue();
   }

   /** old code
   while( m_token == ',' )
   {
      nextToken();
      if( !parseNumericExpression( v ) )
         return false;

      if( v.type() != PMVFloat )
      {
         printError( ( "Float expression expected" ) );
         return false;
      }

      p.resize( p.size() + 1 );
      p[p.size() - 1] = v.floatValue();
   }
   */

   if( !parseToken( '>' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseNumericExpression( PMValue& v, bool checkForBool /*=false*/ )
{
   bool end = false;
   PMValue v2;
   PMVector hv( 0 );

   if( !parseNumericItem( v, checkForBool ) )
      return false;

   do
   {
      switch( m_token )
      {
         case '*':
            nextToken();
            if( !parseNumericItem( v2 ) )
               break;
            switch( v.type() )
            {
               case PMVFloat:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setFloat( v.floatValue() * v2.floatValue() );
                        break;
                     case PMVVector:
                        v.setVector( v2.vector() * v.floatValue() );
                        break;
                     case PMVColor:
                        v.setColor( v2.color() * v.floatValue() );
                        break;
                  }
                  break;
               case PMVVector:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setVector( v.vector() * v2.floatValue() );
                        break;
                     case PMVVector:
                        v.setVector( v.vector() * v2.vector() );
                        break;
                     case PMVColor:
                        if( v.vector().size() == 5 )
                           v.setColor( v.vector() * v2.color() );
                        else
                           printError( ( "Cannot multiply a vector by a color." ) );
                        break;
                  }
                  break;
               case PMVColor:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setColor( v.color() * v2.floatValue() );
                        break;
                     case PMVVector:
                        if( v2.vector().size() == 5 )
                           v.setColor( v2.vector() * v.color() );
                        else
                           printError( ( "Cannot multiply a vector by a color." ) );
                        break;
                     case PMVColor:
                        v.setColor( v.color() * v2.color() );
                        break;
                  }
                  break;
            }
            break;
         case '/':
            nextToken();
            if( !parseNumericItem( v2 ) )
               break;
            switch( v.type() )
            {
               case PMVFloat:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setFloat( v.floatValue() / v2.floatValue() );
                        break;
                     case PMVVector:
                        hv.resize( v2.vector().size() );
                        hv = v.floatValue();
                        v.setVector( hv / v2.vector() );
                        break;
                     case PMVColor:
                        hv.resize( 5 );
                        hv = v.floatValue();
                        v.setColor( hv / v.floatValue() );
                        break;
                  }
                  break;
               case PMVVector:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setVector( v.vector() / v2.floatValue() );
                        break;
                     case PMVVector:
                        v.setVector( v.vector() / v2.vector() );
                        break;
                     case PMVColor:
                        if( v.vector().size() == 5 )
                           v.setColor( v.vector() / v2.color() );
                        else
                           printError( ( "Cannot divide a vector by a color." ) );
                        break;
                  }
                  break;
               case PMVColor:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setColor( v.color() / v2.floatValue() );
                        break;
                     case PMVVector:
                        if( v2.vector().size() == 5 )
                           v.setColor( v2.vector() / v.color() );
                        else
                           printError( ( "Cannot divide a color by a vector." ) );
                        break;
                     case PMVColor:
                        v.setColor( v.color() / v2.color() );
                        break;
                  }
                  break;
            }
            break;
         case '+':
            nextToken();
            if( !parseNumericExpression( v2 ) )
               break;
            switch( v.type() )
            {
               case PMVFloat:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setFloat( v.floatValue() + v2.floatValue() );
                        break;
                     case PMVVector:
                        v.setVector( v2.vector() + v.floatValue() );
                        break;
                     case PMVColor:
                        v.setColor( v2.color() + v.floatValue() );
                        break;
                  }
                  break;
               case PMVVector:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setVector( v.vector() + v2.floatValue() );
                        break;
                     case PMVVector:
                        v.setVector( v.vector() + v2.vector() );
                        break;
                     case PMVColor:
                        if( v.vector().size() == 5 )
                           v.setColor( v.vector() + v2.color() );
                        else
                           printError( ( "Cannot add a vector and a color." ) );
                        break;
                  }
                  break;
               case PMVColor:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setColor( v.color() + v2.floatValue() );
                        break;
                     case PMVVector:
                        if( v2.vector().size() == 5 )
                           v.setColor( v2.vector() + v.color() );
                        else
                           printError( ( "Cannot add a vector and a color." ) );
                        break;
                     case PMVColor:
                        v.setColor( v.color() + v2.color() );
                        break;
                  }
                  break;
            }
            break;
         case '-':
            nextToken();
            if( !parseNumericExpression( v2 ) )
               break;
            switch( v.type() )
            {
               case PMVFloat:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setFloat( v.floatValue() - v2.floatValue() );
                        break;
                     case PMVVector:
                        v.setVector( v2.vector() - v.floatValue() );
                        break;
                     case PMVColor:
                        v.setColor( v2.color() - v.floatValue() );
                        break;
                  }
                  break;
               case PMVVector:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setVector( v.vector() - v2.floatValue() );
                        break;
                     case PMVVector:
                        v.setVector( v.vector() - v2.vector() );
                        break;
                     case PMVColor:
                        if( v.vector().size() == 5 )
                           v.setColor( v.vector() - v2.color() );
                        else
                           printError( ( "Cannot subtract a color from a vector." ) );
                        break;
                  }
                  break;
               case PMVColor:
                  switch( v2.type() )
                  {
                     case PMVFloat:
                        v.setColor( v.color() - v2.floatValue() );
                        break;
                     case PMVVector:
                        if( v2.vector().size() == 5 )
                           v.setColor( v2.vector() - v.color() );
                        else
                           printError( ( "Cannot subtract a vector from a color." ) );
                        break;
                     case PMVColor:
                        v.setColor( v.color() - v2.color() );
                        break;
                  }
                  break;
            }
            break;
         default:
            end = true;
            break;
      }
   }
   while( !end );

   return true;
}

bool PMPovrayParser::parseVector( PMVector& vector, unsigned int size )
{
   PMValue v;
   unsigned int i;

   if( !parseNumericExpression( v ) )
      return false;

   switch( v.type() )
   {
      case PMVFloat:
         vector.resize( size );
         for( i = 0; i < size; i++ )
            vector[i] = v.floatValue();
         break;
      case PMVVector:
         vector = v.vector();
         vector.resize( size );
         break;
      default:
         printError( ( "Float or vector expression expected" ) );
         return false;
   }
   return true;
}

bool PMPovrayParser::parseFloat( double& d, bool suppressError )
{
   PMValue v;

   if( !parseNumericExpression( v, suppressError ) )
      return false;

   switch( v.type() )
   {
      case PMVFloat:
         d = v.floatValue();
         break;
      case PMVVector:
         d = ( v.vector() )[0];
         break;
      default:
         printError( ( "Float expression expected" ) );
         return false;
   }
   return true;
}

bool PMPovrayParser::parseInt( int& i )
{
   double d;

   if( !parseFloat( d ) )
      return false;

   i = ( int ) ( d + 0.5 );
   return true;
}

bool PMPovrayParser::parseColor( PMColor& c )
{
   PMValue v;

   if( !parseNumericExpression( v ) )
      return false;

   if( v.type() == PMVColor )
      c = PMColor( v.color() );
   else if( v.type() == PMVVector )
   {
      if( v.vector().size() == 5 )
         c = PMColor( v.vector() );
      else
      {
         printError( ( "Color expression expected" ) );
         return false;
      }
   }
   else if( v.type() == PMVFloat )
   {
      double d = v.floatValue();
      c = PMColor( d, d, d, d, d );
   }
   else
   {
      printError( ( "Color expression expected" ) );
      return false;
   }

   return true;
}

bool PMPovrayParser::parseObjectModifiers( PMGraphicalObject* o )
{
   bool finished = false;

   PMSolidObject* so = 0;
   if( o->isA( "SolidObject" ) )
      so = ( PMSolidObject* ) o;

   do
   {
      finished = true;
      switch( m_token )
      {
         case NO_SHADOW_TOK:
            o->setNoShadow( true );
            nextToken();
            finished = false;
            break;
         case NO_IMAGE_TOK:
            o->setNoImage( true );
            nextToken();
            finished = false;
            break;
         case NO_REFLECTION_TOK:
            o->setNoReflection( true );
            nextToken();
            finished = false;
            break;
         case DOUBLE_ILLUMINATE_TOK:
            o->setDoubleIlluminate( true );
            nextToken();
            finished = false;
            break;
         default:
            break;
      }
      if( so )
      {
         switch( m_token )
         {
            case HOLLOW_TOK:
               so->setHollow( PMTrue );
               nextToken();
               if( isTrue() )
                  nextToken();
               else if( isFalse() )
               {
                  nextToken();
                  so->setHollow( PMFalse );
               }
               finished = false;
               break;
            case INVERSE_TOK:
               so->setInverse( true );
               nextToken();
               finished = false;
               break;
            default:
               break;
         }
      }
   }
   while( !finished );
   return true;
}

bool PMPovrayParser::parseCSG( PMCSG* pNewCSG )
{
   int oldConsumed;

   switch( m_token )
   {
      case UNION_TOK:
         pNewCSG->setCSGType( PMCSG::CSGUnion );
         break;
      case INTERSECTION_TOK:
         pNewCSG->setCSGType( PMCSG::CSGIntersection );
         break;
      case DIFFERENCE_TOK:
         pNewCSG->setCSGType( PMCSG::CSGDifference );
         break;
      case MERGE_TOK:
         pNewCSG->setCSGType( PMCSG::CSGMerge );
         break;
      default:
         printUnexpected( m_pScanner->sValue() );
         return false;
         break;
   }
   nextToken();

   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewCSG );
      parseObjectModifiers( pNewCSG );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseBox( PMBox* pNewBox )
{
   PMVector vector;

   int oldConsumed;

   if( !parseToken( BOX_TOK, "box" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewBox->setCorner1( vector );

   if( !parseToken( ',' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewBox->setCorner2( vector );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewBox );
      parseObjectModifiers( pNewBox );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseSphere( PMSphere* pNewSphere )
{
   PMVector vector;
   double radius;

   int oldConsumed;

   if( !parseToken( SPHERE_TOK, "sphere" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewSphere->setCentre( vector );

   if( !parseToken( ',' ) )
      return false;

   if( !parseFloat( radius ) )
      return false;
   pNewSphere->setRadius( radius );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewSphere );
      parseObjectModifiers( pNewSphere );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseCylinder( PMCylinder* pNewCyl )
{
   PMVector vector;
   double radius;
   int oldConsumed;

   if( !parseToken( CYLINDER_TOK, "cylinder" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewCyl->setEnd1( vector );

   if( !parseToken( ',' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewCyl->setEnd2( vector );

   if( !parseToken( ',' ) )
      return false;

   if( !parseFloat(radius) )
      return false;
   pNewCyl->setRadius( radius );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewCyl );
      parseObjectModifiers( pNewCyl );
      switch( m_token )
      {
         case OPEN_TOK:
            nextToken();
            pNewCyl->setOpen( true );
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseCone( PMCone* pNewCone )
{
   PMVector vector;
   double radius;
   int oldConsumed;

   if( !parseToken( CONE_TOK, "cone" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewCone->setEnd1( vector );

   if( !parseToken( ',' ) )
      return false;
   if( !parseFloat( radius ) )
      return false;
   pNewCone->setRadius1( radius );

   if( !parseToken( ',' ) )
      return false;
   if( !parseVector( vector ) )
      return false;
   pNewCone->setEnd2( vector );

   if( !parseToken( ',' ) )
      return false;
   if( !parseFloat( radius ) )
      return false;
   pNewCone->setRadius2( radius );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewCone );
      parseObjectModifiers( pNewCone );
      switch( m_token )
      {
         case OPEN_TOK:
            nextToken();
            pNewCone->setOpen( true );
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;

}

bool PMPovrayParser::parseTorus( PMTorus* pNewTorus )
{
   double radius;
   int oldConsumed;

   if( !parseToken( TORUS_TOK, "torus" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseFloat( radius ) )
      return false;
   pNewTorus->setMajorRadius( radius );
   if( !parseToken( ',' ) )
      return false;
   if( !parseFloat( radius ) )
      return false;
   pNewTorus->setMinorRadius( radius );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewTorus );
      parseObjectModifiers( pNewTorus );
      switch( m_token )
      {
         case STURM_TOK:
            nextToken();
            pNewTorus->setSturm( true );
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseBlob( PMBlob* pNewBlob )
{
   PMVector vector;
   double threshold;
   int oldConsumed;

   if( !parseToken( BLOB_TOK, "blob" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   pNewBlob->setThreshold( 1.0 );

   do
   {
      oldConsumed = m_consumedTokens;

      switch( m_token )
      {
         case STURM_TOK:
            nextToken();
            pNewBlob->setSturm( true );
            break;
         case HIERARCHY_TOK:
            pNewBlob->setHierarchy( true );
            nextToken();
            if( isTrue() )
               nextToken();
            else if( isFalse() )
            {
               nextToken();
               pNewBlob->setHierarchy( false );
            }
            break;
         case THRESHOLD_TOK:
            nextToken();
            if( parseFloat( threshold ) )
            {
               if( threshold <= 0 )
                  printError( ( "The threshold value has to be positive" ) );
               else
                  pNewBlob->setThreshold( threshold );
            }
            break;
      }

      parseChildObjects( pNewBlob );
      parseObjectModifiers( pNewBlob );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseBlobSphere( PMBlobSphere* pNewBlobSphere )
{
   PMVector vector;
   double radius;
   double strength;

   int oldConsumed;

   if( !parseToken( SPHERE_TOK, "sphere" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewBlobSphere->setCentre( vector );

   if( !parseToken( ',' ) )
      return false;

   if( !parseFloat( radius ) )
      return false;
   pNewBlobSphere->setRadius( radius );

   if( !parseToken( ',' ) )
      return false;

   if( m_token == STRENGTH_TOK )
      nextToken();

   if( !parseFloat( strength ) )
      return false;
   pNewBlobSphere->setStrength( strength );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewBlobSphere );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseBlobComponent( PMBlobSphere* pNewBlobSphere )
{
   PMVector vector;
   double radius;
   double strength;

   if( !parseToken( COMPONENT_TOK, "component" ) )
      return false;

   if( !parseFloat( strength ) )
      return false;
   pNewBlobSphere->setStrength( strength );

   if( !parseToken( ',' ) )
      return false;

   if( !parseFloat( radius ) )
      return false;
   pNewBlobSphere->setRadius( radius );

   if( !parseToken( ',' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewBlobSphere->setCentre( vector );

   return true;
}

bool PMPovrayParser::parseBlobCylinder( PMBlobCylinder* pNewBlobCylinder )
{
   PMVector vector;
   double radius;
   double strength;
   int oldConsumed;

   if( !parseToken( CYLINDER_TOK, "cylinder" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewBlobCylinder->setEnd1( vector );

   if( !parseToken( ',' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewBlobCylinder->setEnd2( vector );

   if( !parseToken( ',' ) )
      return false;

   if( !parseFloat( radius ) )
      return false;
   pNewBlobCylinder->setRadius( radius );

   if( !parseToken( ',' ) )
      return false;

   if( m_token == STRENGTH_TOK )
      nextToken();

   if( !parseFloat( strength ) )
      return false;
   pNewBlobCylinder->setStrength( strength );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewBlobCylinder );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseHeightField( PMHeightField* pNewHeightField )
{
   int oldConsumed;
   double wl;

   if( !parseToken( HEIGHT_FIELD_TOK, "height_field" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   switch( m_token )
   {
      case GIF_TOK:
         pNewHeightField->setHeightFieldType( PMHeightField::HFgif );
         nextToken();
         break;
      case TGA_TOK:
         pNewHeightField->setHeightFieldType( PMHeightField::HFtga );
         nextToken();
         break;
      case POT_TOK:
         pNewHeightField->setHeightFieldType( PMHeightField::HFpot );
         nextToken();
         break;
      case PNG_TOK:
         pNewHeightField->setHeightFieldType( PMHeightField::HFpng );
         nextToken();
         break;
      case PGM_TOK:
         pNewHeightField->setHeightFieldType( PMHeightField::HFpgm );
         nextToken();
         break;
      case PPM_TOK:
         pNewHeightField->setHeightFieldType( PMHeightField::HFppm );
         nextToken();
         break;
      case SYS_TOK:
         pNewHeightField->setHeightFieldType( PMHeightField::HFsys );
         nextToken();
         break;
      default:
         printExpected( ( "height field type" ), m_pScanner->sValue() );
         return false;
   }
   if( m_token != STRING_TOK )
   {
      printExpected( ( "height field file" ), m_pScanner->sValue() );
      return false;
   }
   else
   {
      pNewHeightField->setFileName( m_pScanner->sValue() );
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;

      switch( m_token )
      {
         case SMOOTH_TOK:
            nextToken();
            pNewHeightField->setSmooth( true );
            if( isTrue() )
               nextToken();
            else if( isFalse() )
            {
               nextToken();
               pNewHeightField->setSmooth( false );
            }
            break;
         case HIERARCHY_TOK:
            pNewHeightField->setHierarchy( true );
            nextToken();
            if( isTrue() )
               nextToken();
            else if( isFalse() )
            {
               nextToken();
               pNewHeightField->setHierarchy( false );
            }
            break;
         case WATER_LEVEL_TOK:
            nextToken();
            if( parseFloat( wl ) )
            {
               if( ( wl < 0.0 ) || ( wl > 1.0 ) )
                  printError( ( "The water level has to be between 0 and 1" ) );
               else
                  pNewHeightField->setWaterLevel( wl );
            }
            break;
      }

      parseChildObjects( pNewHeightField );
      parseObjectModifiers( pNewHeightField );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseText( PMText* pNewText )
{
   int oldConsumed;
   double thickness;
   PMVector offset;

   if( !parseToken( TEXT_TOK, "text" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseToken( TTF_TOK, "ttf" ) )
      return false;

   if( m_token != STRING_TOK )
   {
      printExpected( ( "font file name" ), m_pScanner->sValue() );
      return false;
   }
   else
   {
      pNewText->setFont( m_pScanner->sValue() );
      nextToken();
   }
   if( m_token != STRING_TOK )
   {
      printExpected( ( "string of text" ), m_pScanner->sValue() );
      return false;
   }
   else
   {
      pNewText->setText( m_pScanner->sValue() );
      nextToken();
   }

   if( !parseFloat( thickness ) )
      return false;
   pNewText->setThickness( thickness );

   parseToken( ',' );

   if( parseVector( offset, 2 ) )
      pNewText->setOffset( offset );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewText );
      parseObjectModifiers( pNewText );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseJuliaFractal( PMJuliaFractal* pNewFractal )
{
   int oldConsumed;
   double d;
   int i;
   PMVector v( 4 ), v2( 2 );

   if( !parseToken( JULIA_FRACTAL_TOK, "julia_fractal" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( v, 4 ) )
      return false;
   pNewFractal->setJuliaParameter( v );

   do
   {
      oldConsumed = m_consumedTokens;

      switch( m_token )
      {
         case QUATERNION_TOK:
            pNewFractal->setAlgebraType( PMJuliaFractal::Quaternion );
            nextToken();
            break;
         case HYPERCOMPLEX_TOK:
            pNewFractal->setAlgebraType( PMJuliaFractal::Hypercomplex );
            nextToken();
            break;
         case SQR_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTsqr );
            nextToken();
            break;
         case CUBE_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTcube );
            nextToken();
            break;
         case EXP_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTexp );
            nextToken();
            break;
         case RECIPROCAL_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTreciprocal );
            nextToken();
            break;
         case SIN_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTsin );
            nextToken();
            break;
         case ASIN_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTasin );
            nextToken();
            break;
         case SINH_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTsinh );
            nextToken();
            break;
         case ASINH_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTasinh );
            nextToken();
            break;
         case COS_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTcos );
            nextToken();
            break;
         case ACOS_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTacos );
            nextToken();
            break;
         case COSH_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTcosh );
            nextToken();
            break;
         case ACOSH_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTacosh );
            nextToken();
            break;
         case TAN_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTtan );
            nextToken();
            break;
         case ATAN_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTatan );
            nextToken();
            break;
         case TANH_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTtanh );
            nextToken();
            break;
         case ATANH_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTatanh );
            nextToken();
            break;
         case LOG_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTlog );
            nextToken();
            break;
         case PWR_TOK:
            pNewFractal->setFunctionType( PMJuliaFractal::FTpwr );
            nextToken();
            if( !parseToken( '(' ) )
               return false;
            if( !parseFloat( v2[0] ) )
               return false;
            parseToken( ',' );
            if( !parseFloat( v2[1] ) )
               return false;
            if( !parseToken( ')' ) )
               return false;
            pNewFractal->setExponent( v2 );
            break;
         case MAX_ITERATION_TOK:
            nextToken();
            if( !parseInt( i ) )
               return false;
            if( i <= 0 )
            {
               printWarning( ( "Maximum iterations are less than 1, fixed" ) );
               i = 1;
            }
            pNewFractal->setMaximumIterations( i );
            break;
         case PRECISION_TOK:
            nextToken();
            if( !parseFloat( d ) )
               return false;
            if( d < 1.0 )
            {
               printWarning( ( "Precision is less than 1.0, fixed" ) );
               d = 1.0;
            }
            pNewFractal->setPrecision( d );
            break;
         case SLICE_TOK:
            nextToken();
            if( !parseVector( v, 4 ) )
               return false;
            pNewFractal->setSliceNormal( v );
            parseToken( ',' );
            if( !parseFloat( d ) )
               return false;
            pNewFractal->setSliceDistance( d );
            break;
      }

      parseChildObjects( pNewFractal );
      parseObjectModifiers( pNewFractal );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parsePlane( PMPlane* pNewPlane )
{
   double dist;
   PMVector vector;
   int oldConsumed;

   if( !parseToken( PLANE_TOK, "plane" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewPlane->setNormal( vector );

   if( !parseToken( ',' ) )
      return false;
   if( !parseFloat( dist ) )
      return false;
   pNewPlane->setDistance( dist );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewPlane );
      parseObjectModifiers( pNewPlane );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

const int c_polynomSize[8] = { 0, 0, 10, 20, 35, 56, 84, 120 };

bool PMPovrayParser::parsePolynom( PMPolynom* pNewPoly )
{
   PMVector vector;
   double d;
   PMVector c;
   int oldConsumed;
   int order = 2;
   int type = m_token;

   pNewPoly->setSturm( false );

   if( ( m_token == QUADRIC_TOK ) || ( m_token == CUBIC_TOK ) ||
       ( m_token == QUARTIC_TOK ) || ( m_token == POLY_TOK ) )
   {
      nextToken();
      if( !parseToken( '{' ) )
         return false;
   }
   else
      printExpected( "poly", m_pScanner->sValue() );

   if( type == QUADRIC_TOK )
   {
      c = PMVector( 10 );
      pNewPoly->setPolynomOrder( 2 );

      // parse the quadric coefficients
      if( !parseVectorLiteral( vector ) )
         return false;
      vector.resize( 3 );
      c[0] = vector[0];
      c[4] = vector[1];
      c[7] = vector[2];
      parseToken( ',' );

      if( !parseVectorLiteral( vector ) )
         return false;
      vector.resize( 3 );
      c[1] = vector[0];
      c[2] = vector[1];
      c[5] = vector[2];
      parseToken( ',' );

      if( !parseVectorLiteral( vector ) )
         return false;
      vector.resize( 3 );
      c[3] = vector[0];
      c[6] = vector[1];
      c[8] = vector[2];
      parseToken( ',' );

      if( !parseFloat( d ) )
         return false;
      c[9] = d;

      pNewPoly->setCoefficients( c );
   }
   else
   {
      if( type == CUBIC_TOK )
         order = 3;
      else if( type == QUARTIC_TOK )
         order = 4;
      else
      {
         if( !parseInt( order ) )
            return false;
         if( ( order < 2 ) || ( order > 7 ) )
         {
            printError( ( "The polynom order has to be between 2 and 7 inclusive" ) );
            return false;
         }
         parseToken( ',' );
      }

      pNewPoly->setPolynomOrder( order );

      if( !parseVectorLiteral( vector ) )
         return false;

      if( vector.size() != c_polynomSize[order] )
      {
         printError( ( QString::number( c_polynomSize[order] ) + " coefficients are needed for a polynom with order " +
                       QString::number( order ) ) );
         vector.resize( c_polynomSize[order] );
      }
      pNewPoly->setCoefficients( vector );
   }

   do
   {
      oldConsumed = m_consumedTokens;

      if( m_token == STURM_TOK )
      {
         pNewPoly->setSturm( true );
         nextToken();
      }

      parseChildObjects( pNewPoly );
      parseObjectModifiers( pNewPoly );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseBicubicPatch( PMBicubicPatch* pNewPatch )
{
   PMVector vector;
   bool stop = false;
   int oldConsumed;
   int type;
   int steps;
   double flatness;
   int i;

   if( !parseToken( BICUBIC_PATCH_TOK, "bicubic_patch" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   // parse patch items
   do
   {
      switch( m_token )
      {
         case TYPE_TOK:
            nextToken();
            if( parseInt( type ) )
            {
               if( ( type == 0 ) || ( type == 1 ) )
                  pNewPatch->setPatchType( type );
               else
                  printError( ( "Patch type has to be 0 or 1" ) );
            }
            break;
         case U_STEPS_TOK:
            nextToken();
            if( parseInt( steps ) )
               pNewPatch->setUSteps( steps );
            break;
         case V_STEPS_TOK:
            nextToken();
            if( parseInt( steps ) )
               pNewPatch->setVSteps( steps );
            break;
         case FLATNESS_TOK:
            nextToken();
            if( parseFloat( flatness ) )
               pNewPatch->setFlatness( flatness );
            break;
         case UV_VECTORS_TOK:
            pNewPatch->enableUV( true );
            nextToken();
            for ( i = 0; i < 4; ++i )
            {
               if( parseVector( vector ) )
                  pNewPatch->setUVVector( i, vector );
               else
                  return false;
            }
            break;
         case ',':
            nextToken();
            stop = true;
            break;
         default:
            stop = true;
            break;
      }
   }
   while( !stop );

   // parse control points
   stop = false;
   for( i = 0; ( i < 16 ) && !stop; i++ )
   {
      if( parseVector( vector ) )
      {
         pNewPatch->setControlPoint( i, vector );
         if( i < 15 )
            if( !parseToken( ',' ) )
               stop = true;
      }
      else
         stop = true;
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewPatch );
      parseObjectModifiers( pNewPatch );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseDisc( PMDisc* pNewDisc )
{
   double d;
   PMVector vector;
   int oldConsumed;

   if( !parseToken( DISC_TOK, "disc" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   pNewDisc->setCenter( vector );

   if( !parseToken( ',' ) )
      return false;
   if( !parseVector( vector ) )
      return false;
   pNewDisc->setNormal( vector );

   if( !parseToken( ',' ) )
      return false;
   if( !parseFloat( d ) )
      return false;
   pNewDisc->setRadius( d );

   if( m_token == ',' )
   {
      nextToken();
      if( !parseFloat( d ) )
         return false;
      pNewDisc->setHoleRadius( d );
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewDisc );
      parseObjectModifiers( pNewDisc );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseTriangle( PMTriangle* pNewTriangle )
{
   PMVector vector;
   int oldConsumed;
   int i;

   if( m_token == SMOOTH_TRIANGLE_TOK )
      pNewTriangle->setSmoothTriangle( true );
   else if( m_token == TRIANGLE_TOK )
      pNewTriangle->setSmoothTriangle( false );
   else
   {
      printExpected( "triangle", m_pScanner->sValue() );
      return false;
   }
   nextToken();

   if( !parseToken( '{' ) )
      return false;

   for( i = 0; i < 3; i++ )
   {
      if( i != 0 )
         parseToken( ',' );
      if( !parseVector( vector ) )
         return false;
      pNewTriangle->setPoint( i, vector );

      if( pNewTriangle->isSmoothTriangle() )
      {
         parseToken( ',' );
         if( !parseVector( vector ) )
            return false;
         pNewTriangle->setNormal( i, vector );
      }
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewTriangle );
      parseObjectModifiers( pNewTriangle );
      if( m_token == UV_VECTORS_TOK )
      {
         nextToken();
         pNewTriangle->enableUV( true );
         for ( i = 0; i < 3; ++i )
         {
            if( parseVector( vector ) )
               pNewTriangle->setUVVector( i, vector );
            else
               return false;
         }
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseLathe( PMLathe* pNewLathe )
{
   PMVector vector;
   int oldConsumed;
   int i;
   bool stop = false;

   if( !parseToken( LATHE_TOK, "lathe" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   int minp = 2;
   while( !stop )
   {
      switch( m_token )
      {
         case LINEAR_SPLINE_TOK:
            pNewLathe->setSplineType( PMLathe::LinearSpline );
            nextToken();
            minp = 2;
            break;
         case QUADRATIC_SPLINE_TOK:
            pNewLathe->setSplineType( PMLathe::QuadraticSpline );
            nextToken();
            minp = 3;
            break;
         case CUBIC_SPLINE_TOK:
            pNewLathe->setSplineType( PMLathe::CubicSpline );
            nextToken();
            minp = 4;
            break;
         case BEZIER_SPLINE_TOK:
            pNewLathe->setSplineType( PMLathe::BezierSpline );
            nextToken();
            minp = 4;
            break;
         default:
            stop = true;
            break;
      }
   }

   int nump;
   if( !parseInt( nump ) )
      return false;

   QList<PMVector> points;
   for( i = 0; i < nump; i++ )
   {
      parseToken( ',' );
      if( !parseVector( vector ) )
         return false;
      vector.resize( 2 );
      points.append( vector );
   }

   if( nump < minp )
      printError( ( "At least " + QString::number( minp ) + " points are needed for that spline type" ) );
   else if( ( pNewLathe->splineType() == PMLathe::BezierSpline ) &&
            ( ( nump % 4 ) != 0 ) )
      printError( ( "Bezier splines need 4 points for each segment" ) );
   else
      pNewLathe->setPoints( points );

   do
   {
      oldConsumed = m_consumedTokens;

      if( m_token == STURM_TOK )
      {
         pNewLathe->setSturm( true );
         nextToken();
      }

      parseChildObjects( pNewLathe );
      parseObjectModifiers( pNewLathe );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parsePrism( PMPrism* pNewPrism )
{
   PMVector vector;
   double height;
   int oldConsumed;
   int i;
   bool stop = false;

   if( !parseToken( PRISM_TOK, "prism" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   int minp = 3;
   while( !stop )
   {
      switch( m_token )
      {
         case LINEAR_SPLINE_TOK:
            pNewPrism->setSplineType( PMPrism::LinearSpline );
            nextToken();
            minp = 3;
            break;
         case QUADRATIC_SPLINE_TOK:
            pNewPrism->setSplineType( PMPrism::QuadraticSpline );
            nextToken();
            minp = 4;
            break;
         case CUBIC_SPLINE_TOK:
            pNewPrism->setSplineType( PMPrism::CubicSpline );
            nextToken();
            minp = 5;
            break;
         case BEZIER_SPLINE_TOK:
            pNewPrism->setSplineType( PMPrism::BezierSpline );
            nextToken();
            minp = 4;
            break;
         case LINEAR_SWEEP_TOK:
            pNewPrism->setSweepType( PMPrism::LinearSweep );
            nextToken();
            break;
         case CONIC_SWEEP_TOK:
            pNewPrism->setSweepType( PMPrism::ConicSweep );
            nextToken();
            break;
         default:
            stop = true;
            break;
      }
   }

   if( !parseFloat( height ) )
      return false;
   pNewPrism->setHeight1( height );
   parseToken( ',' );
   if( !parseFloat( height ) )
      return false;
   pNewPrism->setHeight2( height );
   parseToken( ',' );

   int nump;
   if( !parseInt( nump ) )
      return false;

   QList<PMVector> allPoints;
   for( i = 0; i < nump; i++ )
   {
      parseToken( ',' );
      if( !parseVector( vector ) )
         return false;
      vector.resize( 2 );
      allPoints.append( vector );
   }

   QList< QList<PMVector> > points;
   QList<PMVector> subPoints;
   QList<PMVector>::Iterator it = allPoints.begin();
   int pnr = 0, pmod4;
   PMVector ref( 2 ), ref2( 2 );
   bool error = false;
   bool last = false;

   switch( pNewPrism->splineType() )
   {
      case PMPrism::LinearSpline:
         for( ; ( it != allPoints.end() ) && !error; ++it, pnr++ )
         {
            if( pnr == 0 )
            {
               ref = *it;
               subPoints.append( *it );
            }
            else
            {
               if( ref.approxEqual( *it ) )
               {
                  if( pnr < 3 )
                  {
                     printError( ( "Linear splines need at least 4 points." ) );
                     error = true;
                  }
                  else
                  {
                     points.append( subPoints );
                     subPoints.clear();
                     pnr = -1;
                  }
               }
               else
                  subPoints.append( *it );
            }
         }
         if( ( pnr != 0 ) && ( !error ) )
         {
            printWarning( ( "Linear spline not closed" ) );
            if( pnr < 3 )
            {
               printError( ( "Linear splines need at least 4 points." ) );
               error = true;
            }
            else
            {
               points.append( subPoints );
               subPoints.clear();
            }
         }
         break;
      case PMPrism::QuadraticSpline:
         for( ; ( it != allPoints.end() ) && !error; ++it, pnr++ )
         {
            if( pnr == 0 )
               subPoints.append( *it );
            else if( pnr == 1 )
            {
               ref = *it;
               subPoints.append( *it );
            }
            else
            {
               if( ref.approxEqual( *it ) )
               {
                  if( pnr < 4 )
                  {
                     printError( ( "Quadratic splines need at least 5 points." ) );
                     error = true;
                  }
                  else
                  {
                     points.append( subPoints );
                     subPoints.clear();
                     pnr = -1;
                  }
               }
               else
                  subPoints.append( *it );
            }
         }
         if( ( pnr != 0 ) && ( !error ) )
         {
            printError( ( "Quadratic spline not closed" ) );
            error = true;
         }
         break;
      case PMPrism::CubicSpline:
         for( ; ( it != allPoints.end() ) && !error; ++it, pnr++ )
         {
            if( pnr == 0 )
               subPoints.append( *it );
            else if( pnr == 1 )
            {
               ref = *it;
               subPoints.append( *it );
            }
            else if( last )
            {
               if( pnr < 5 )
               {
                  printError( ( "Cubic splines need at least 6 points." ) );
                  error = true;
               }
               else
               {
                  subPoints.append( *it );
                  points.append( subPoints );
                  subPoints.clear();
                  pnr = -1;
                  last = false;
               }
            }
            else
            {
               if( ref.approxEqual( *it ) )
                  last = true;
               else
                  subPoints.append( *it );
            }
         }
         if( ( pnr != 0 ) && ( !error ) )
         {
            printError( ( "Cubic spline not closed" ) );
            error = true;
         }
         break;
      case PMPrism::BezierSpline:
         for( ; ( it != allPoints.end() ) && !error; ++it, pnr++ )
         {
            pmod4 = pnr % 4;

            if( pnr == 0 )
            {
               ref = *it;
               subPoints.append( *it );
            }
            else if( pmod4 == 0 )
            {
               if( !ref2.approxEqual( *it ) )
               {
                  printError( ( "Bezier spline not closed" ) );
                  error = true;
               }
            }
            else if( pmod4 == 3 )
            {
               if( ref.approxEqual( *it ) )
               {
                  points.append( subPoints );
                  subPoints.clear();
                  pnr = -1;
               }
               else
               {
                  subPoints.append( *it );
                  ref2 = *it;
               }
            }
            else
               subPoints.append( *it );
         }
         if( ( pnr != 0 ) && ( !error ) )
         {
            printError( ( "Bezier spline not closed" ) );
            error = true;
         }
         break;
   }

   if( !error )
      pNewPrism->setPoints( points );

   do
   {
      oldConsumed = m_consumedTokens;

      switch( m_token )
      {
         case STURM_TOK:
            pNewPrism->setSturm( true );
            nextToken();
            break;
         case OPEN_TOK:
            pNewPrism->setOpen( true );
            nextToken();
            break;
         default:
            break;
      }

      parseChildObjects( pNewPrism );
      parseObjectModifiers( pNewPrism );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseSor( PMSurfaceOfRevolution* pNewSor )
{
   PMVector vector;
   int oldConsumed;
   int i;

   if( !parseToken( SOR_TOK, "sor" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   int nump;
   if( !parseInt( nump ) )
      return false;

   QList<PMVector> points;
   for( i = 0; i < nump; i++ )
   {
      parseToken( ',' );
      if( !parseVector( vector ) )
         return false;
      vector.resize( 2 );
      points.append( vector );
   }

   if( nump < 4 )
      printError( ( "At least 4 points are needed for the surface of revolution" ) );
   else
   {
      QList<PMVector>::Iterator it1 = points.begin();
      QList<PMVector>::Iterator it2 = it1; ++it2;
      QList<PMVector>::Iterator it3 = it2; ++it3;
      int pnr = 0;

      for( ; it3 != points.end(); ++it1, ++it2, ++it3, pnr++ )
      {
         if( ( pnr == 0 ) || ( pnr == ( nump - 3 ) ) )
         {
            if( approxZero( ( *it1 )[1] - ( *it3 )[1], c_sorTolerance ) )
            {
               printError( ( "The v coordinate of point " +
                             QString( pnr+1 ) + " and " +
                             QString( pnr+3 ) + " must be different; fixed" ) );
               if( pnr == 0 )
                  ( *it1 )[1] = ( *it3 )[1] - c_sorTolerance;
               else
                  ( *it3 )[1] = ( *it1 )[1] + c_sorTolerance;
            }
         }

         if( pnr != 0 )
         {
            if( ( ( *it2 )[1] - ( *it1 )[1] ) < c_sorTolerance )
            {
               printError( ( "The v coordinates must be strictly increasing; fixed" ) );
               ( *it2 )[1] = ( *it1 )[1] + c_sorTolerance;
            }
         }
      }
      pNewSor->setPoints( points );
   }

   do
   {
      oldConsumed = m_consumedTokens;

      switch( m_token )
      {
         case STURM_TOK:
            pNewSor->setSturm( true );
            nextToken();
            break;
         case OPEN_TOK:
            pNewSor->setOpen( true );
            nextToken();
            break;
         default:
            break;
      }

      parseChildObjects( pNewSor );
      parseObjectModifiers( pNewSor );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseSqe( PMSuperquadricEllipsoid* pNewSqe )
{
   PMVector vector;
   int oldConsumed;

   if( !parseToken( SUPERELLIPSOID_TOK ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( !parseVector( vector ) )
      return false;
   vector.resize( 2 );

   if( vector[0] < 0.001 )
   {
      printError( ( "The east-west exponent must be greater than 0.001" ) );
      vector[0] = 0.001;
   }
   if( vector[1] < 0.001 )
   {
      printError( ( "The north-south exponent must be greater than 0.001" ) );
      vector[1] = 0.001;
   }

   pNewSqe->setEastWestExponent( vector[0] );
   pNewSqe->setNorthSouthExponent( vector[1] );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pNewSqe );
      parseObjectModifiers( pNewSqe );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseRotate( PMRotate* rotate )
{
   PMVector v;

   if( !parseToken( ROTATE_TOK, "rotate" ) )
      return false;
   if( !parseVector( v ) )
      return false;

   rotate->setRotation( v );
   return true;
}

bool PMPovrayParser::parseScale( PMScale* scale )
{
   PMVector v;

   if( !parseToken( SCALE_TOK, "scale" ) )
      return false;
   if( !parseVector( v ) )
      return false;

   scale->setScale( v );
   return true;
}

bool PMPovrayParser::parseTranslate( PMTranslate* translate )
{
   PMVector v;

   if( !parseToken( TRANSLATE_TOK, "translate" ) )
      return false;
   if( !parseVector( v ) )
      return false;

   translate->setTranslation( v );
   return true;
}

bool PMPovrayParser::parseMatrix( PMPovrayMatrix* matrix )
{
   PMVector v;

   if( !parseToken( MATRIX_TOK ), "matrix" )
      return false;
   if( !parseVectorLiteral( v ) )
      return false;

   if( v.size() != 12 )
   {
      printError( ( "Wrong number of matrix values." ) );
      v.resize( 12 );
   }
   matrix->setValues( v );
   return true;
}

bool PMPovrayParser::parseBoundedBy( PMBoundedBy* bound )
{
   int oldConsumed;

   if( !parseToken( BOUNDED_BY_TOK, "bounded_by" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;

      if( m_token == CLIPPED_BY_TOK )
         nextToken();

      parseChildObjects( bound );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseClippedBy( PMClippedBy* clipped )
{
   int oldConsumed;

   if( !parseToken( CLIPPED_BY_TOK, "clipped_by" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;

      if( m_token == BOUNDED_BY_TOK )
         nextToken();

      parseChildObjects( clipped );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseCamera( PMCamera* camera )
{
   PMVector v;
   double d;
   int i;

   int oldConsumed;

   if( !parseToken( CAMERA_TOK, "camera" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;

      do
      {
         oldConsumed = m_consumedTokens;

         switch( m_token )
         {
            case PERSPECTIVE_TOK:
               nextToken();
               camera->setCameraType( PMCamera::Perspective );
               break;
            case ORTHOGRAPHIC_TOK:
               nextToken();
               camera->setCameraType( PMCamera::Orthographic );
               break;
            case FISHEYE_TOK:
               nextToken();
               camera->setCameraType( PMCamera::FishEye );
               break;
            case ULTRA_WIDE_ANGLE_TOK:
               nextToken();
               camera->setCameraType( PMCamera::UltraWideAngle );
               break;
            case OMNIMAX_TOK:
               nextToken();
               camera->setCameraType( PMCamera::Omnimax );
               break;
            case PANORAMIC_TOK:
               nextToken();
               camera->setCameraType( PMCamera::Panoramic );
               break;
            case CYLINDER_TOK:
               nextToken();
               camera->setCameraType( PMCamera::Cylinder );
               if( parseInt( i ) )
                  camera->setCylinderType( i );
               break;
            case LOCATION_TOK:
               nextToken();
               if( parseVector( v ) )
                  camera->setLocation( v );
               break;
            case SKY_TOK:
               nextToken();
               if( parseVector( v ) )
                  camera->setSky( v );
               break;
            case UP_TOK:
               nextToken();
               if( parseVector( v ) )
                  camera->setUp( v );
               break;
            case RIGHT_TOK:
               nextToken();
               if( parseVector( v ) )
                  camera->setRight( v );
               break;
            case DIRECTION_TOK:
               nextToken();
               if( parseVector( v ) )
                  camera->setDirection( v );
               break;
            case LOOK_AT_TOK:
               nextToken();
               if( parseVector( v ) )
                  camera->setLookAt( v );
               break;
            case ANGLE_TOK:
               nextToken();
               if( parseFloat( d ) )
               {
                  camera->enableAngle( true );
                  camera->setAngle( d );
               }
               break;
            case BLUR_SAMPLES_TOK:
               nextToken();
               camera->enableFocalBlur( true );
               if( parseInt( i ) )
                  camera->setBlurSamples( i );
               break;
            case APERTURE_TOK:
               nextToken();
               camera->enableFocalBlur( true );
               if( parseFloat( d ) )
                  camera->setAperture( d );
               break;
            case FOCAL_POINT_TOK:
               nextToken();
               if( parseVector( v ) )
                  camera->setFocalPoint( v );
               break;
            case CONFIDENCE_TOK:
               nextToken();
               if( parseFloat( d ) )
                  camera->setConfidence( d );
               break;
            case VARIANCE_TOK:
               nextToken();
               if( parseFloat( d ) )
                  camera->setVariance( d );
               break;
            default:
               break;
         }
      }
      while( oldConsumed != m_consumedTokens );
      parseChildObjects( camera );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseLight( PMLight* light )
{
   PMVector v;
   PMColor c;
   double d;
   int i;

   int oldConsumed;

   if( !parseToken( LIGHT_SOURCE_TOK, "light_source" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;
   if( !parseVector( v ) )
      return false;
   light->setLocation( v );
   if( m_token == ',' )
      nextToken();
   if( !parseColor( c ) )
      return false;
   light->setColor( c );

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( light );

      switch( m_token )
      {
         case SPOTLIGHT_TOK:
            nextToken();
            light->setLightType( PMLight::SpotLight );
            break;
         case CYLINDER_TOK:
            nextToken();
            light->setLightType( PMLight::CylinderLight );
            break;
         case SHADOWLESS_TOK:
            nextToken();
            light->setLightType( PMLight::ShadowlessLight );
            break;
         case RADIUS_TOK:
            nextToken();
            if( parseFloat( d ) )
               light->setRadius( d );
            break;
         case FALLOFF_TOK:
            nextToken();
            if( parseFloat( d ) )
               light->setFalloff( d );
            break;
         case TIGHTNESS_TOK:
            nextToken();
            if( parseFloat( d ) )
               light->setTightness( d );
            break;
         case POINT_AT_TOK:
            nextToken();
            if( parseVector( v ) )
               light->setPointAt( v );
            break;
         case PARALLEL_TOK:
            nextToken();
            light->setParallel( parseBool() );
            break;
         case AREA_LIGHT_TOK:
            nextToken();
            light->setAreaLight( true );
            if( parseVector( v ) )
               light->setAxis1( v );
            parseToken( ',' );
            if( parseVector( v ) )
               light->setAxis2( v );
            parseToken( ',' );
            if( parseInt( i ) )
               light->setSize1( i );
            parseToken( ',' );
            if( parseInt( i ) )
               light->setSize2( i );
            break;
         case AREA_CIRCULAR_TOK:
            nextToken();
            light->setAreaType( PMLight::Circular );
            break;
         case ADAPTIVE_TOK:
            nextToken();
            if( parseInt( i ) )
               light->setAdaptive( i );
            break;
         case ORIENT_TOK:
            nextToken();
            light->setOrient( parseBool() );
            break;
         case JITTER_TOK:
            nextToken();
            light->setJitter( parseBool() );
            break;
         case FADE_POWER_TOK:
            nextToken();
            light->setFading( true );
            if( parseInt( i ) )
               light->setFadePower( i );
            break;
         case FADE_DISTANCE_TOK:
            nextToken();
            light->setFading( true );
            if( parseFloat( d ) )
               light->setFadeDistance( d );
            break;
         case MEDIA_INTERACTION_TOK:
            nextToken();
            light->setMediaInteraction( parseBool() );
            break;
         case MEDIA_ATTENUATION_TOK:
            nextToken();
            light->setMediaAttenuation( parseBool() );
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseLooksLike( PMLooksLike* ll )
{
   if( !parseToken( LOOKS_LIKE_TOK, "looks_like" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   parseChildObjects( ll );

   if( !parseToken( '}' ) )
      return false;
   return true;
}

bool PMPovrayParser::parseProjectedThrough( PMProjectedThrough* ll )
{
   if( !parseToken( PROJECTED_THROUGH_TOK, "projected_through" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   parseChildObjects( ll );

   if( !parseToken( '}' ) )
      return false;
   return true;
}

bool PMPovrayParser::parseTexture( PMTexture* texture, bool parseOuter )
{
   int oldConsumed;

   if( parseOuter )
   {
      if( !parseToken( TEXTURE_TOK, "texture" ) )
         return false;

      if( !parseToken( '{' ) )
         return false;
   }

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !texture->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( texture );
      if( m_token == UV_MAPPING_TOK )
      {
         nextToken();
         texture->setUVMapping( parseBool() );
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( parseOuter )
      if( !parseToken( '}' ) )
         return false;

   return true;
}

bool PMPovrayParser::parsePattern( PMPattern* pattern, bool normal )
{
   PMVector vector;
   double f_number;
   int i_number;
   int oldConsumed;
   bool type;

   do
   {
      oldConsumed = m_consumedTokens;
      type = false;

      switch( m_token )
      {
         case AGATE_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternAgate );
            type = true;
            break;
         case AGATE_TURB_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            pattern->setAgateTurbulence( f_number );
            break;
         case AVERAGE_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternAverage );
            type = true;
            break;
         case BOXED_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternBoxed );
            type = true;
            break;
         case BOZO_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternBozo );
            type = true;
            break;
         case BUMPS_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternBumps );
            type = true;
            break;
         case CELLS_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternCells );
            type = true;
            break;
         case CRACKLE_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternCrackle );
            type = true;
            break;
         case CYLINDRICAL_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternCylindrical );
            type = true;
            break;
         case DENTS_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternDents );
            type = true;
            break;
         case DENSITY_FILE_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternDensity );
            type = true;
            if( !parseToken( DF3_TOK, "df3" ) )
               return false;
            if( m_token != STRING_TOK )
            {
               printError( ( "Expecting a file name." ) );
               return false;
            }
            else
            {
               pattern->setDensityFile( m_pScanner->sValue() );
               nextToken();
            }
            if( parseToken( INTERPOLATE_TOK, "interpolate" ) )
            {
               if( !parseInt( i_number ) )
                  return false;
               else
                  pattern->setDensityInterpolate( i_number );
            }
            break;
         case GRADIENT_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternGradient );
            type = true;
            if( !parseVector( vector ) )
               return false;
            pattern->setGradient( vector );
            break;
         case GRANITE_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternGranite );
            type = true;
            break;
         case JULIA_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternJulia );
            type = true;
            if( !parseVector( vector ) )
               return false;
            pattern->setJuliaComplex( vector );
            if( !parseInt( i_number ) )
               return false;
            pattern->setMaxIterations( i_number );
            break;
         case LEOPARD_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternLeopard );
            type = true;
            break;
         case MANDEL_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternMandel );
            type = true;
            if( !parseInt( i_number ) )
               return false;
            pattern->setMaxIterations( i_number );
            break;
         case MARBLE_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternMarble );
            type = true;
            break;
         case ONION_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternOnion );
            type = true;
            break;
         case PLANAR_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternPlanar );
            type = true;
            break;
         case QUILTED_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternQuilted );
            type = true;
            break;
         case CONTROL0_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            pattern->setQuiltControl0( f_number );
            break;
         case CONTROL1_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            pattern->setQuiltControl1( f_number );
            break;
         case RADIAL_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternRadial );
            type = true;
            break;
         case RIPPLES_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternRipples );
            type = true;
            break;
         case SLOPE_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternSlope );
            type = true;
            if( !parseToken( '{' ) )
               return false;
            if( m_token == POINT_AT_TOK )
            {
               pattern->setSlopePointAtFlag( true );
               nextToken();
               if( !parseVector( vector ) )
                  return false;
               else
                  pattern->setSlopePointAt( vector );
            }
            else
            {
               if( !parseVector( vector ) )
                  return false;
               pattern->setSlopeDirection( vector );
            }
            if ( parseToken( ',' ) )
            {
               if( !parseFloat( f_number ) )
                  return false;
               pattern->setSlopeLoSlope( f_number );
               if ( parseToken( ',' ) )
               {
                  if ( !parseFloat( f_number ) )
                     return false;
                  pattern->setSlopeHiSlope( f_number );
               }
            }
            if( m_token == ALTITUDE_TOK )
            {
               pattern->setSlopeAltFlag( true );
               nextToken();
               if ( !parseVector( vector ) )
                  return false;
               pattern->setSlopeAltitude( vector );
               if( parseToken( ',' ) )
               {
                  if ( !parseFloat( f_number ) )
                     return false;
                  pattern->setSlopeLoAlt( f_number );
                  if ( parseToken( ',' ) )
                  {
                     if( !parseFloat( f_number ) )
                        return false;
                     pattern->setSlopeHiAlt( f_number );
                  }
               }
            }
            if( !parseToken( '}' ) )
               return false;
            break;
         case SPHERICAL_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternSpherical );
            type = true;
            break;
         case SPIRAL1_TOK:
         case SPIRAL2_TOK:
            if( m_token == SPIRAL1_TOK )
               pattern->setPatternType( PMPattern::PatternSpiral1 );
            else
               pattern->setPatternType( PMPattern::PatternSpiral2 );
            type = true;
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            pattern->setSpiralNumberArms( i_number );
            break;
         case TILING_TOK:
            pattern->setPatternType( PMPattern::PatternTiling );
            type = true;
            nextToken();
            if( !parseInt( i_number ) )
            return false;
            pattern->setTilingNumbers( i_number );
            break;
         case SPOTTED_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternSpotted );
            type = true;
            break;
         case WAVES_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternWaves );
            type = true;
            break;
         case WOOD_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternWood );
            type = true;
            break;
         case WRINKLES_TOK:
            nextToken();
            pattern->setPatternType( PMPattern::PatternWrinkles );
            type = true;
            break;

         //crackle parameters
         case FORM_TOK:
            nextToken();
            if( !parseVector( vector ) )
               return false;
            pattern->setCrackleForm( vector );
            break;
         case METRIC_TOK:
            nextToken();
            if ( !parseInt( i_number ) )
               return false;
            pattern->setCrackleMetric( i_number );
            break;
         case OFFSET_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            pattern->setCrackleOffset( f_number );
            break;
         case SOLID_TOK:
            nextToken();
            pattern->setCrackleSolid( true );
            break;

         //fractal parameters
         case MAGNET_TOK:
            nextToken();
            pattern->setFractalMagnet( true );
            if ( !parseInt( i_number ) )
               return false;
            pattern->setFractalMagnetType( i_number );
            break;
         case EXPONENT_TOK:
            nextToken();
            if ( !parseInt( i_number ) )
               return false;
            pattern->setFractalExponent( i_number );
            break;
         case EXTERIOR_TOK:
            nextToken();
            if ( !parseInt( i_number ) )
               return false;
            pattern->setFractalExtType( i_number );
            if ( !parseFloat( f_number ) )
               return false;
            pattern->setFractalExtFactor( f_number );
            break;
         case INTERIOR_TOK:
            nextToken();
            if ( !parseInt( i_number ) )
               return false;
            pattern->setFractalIntType( i_number );
            if ( !parseFloat( f_number ) )
               return false;
            pattern->setFractalIntFactor( f_number );
            break;

         //turbulence
         case TURBULENCE_TOK:
            nextToken();
            pattern->enableTurbulence( true );
            if( !parseVector( vector ) )
               return false;
            pattern->setValueVector( vector );
            break;
         case OCTAVES_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            pattern->setOctaves( i_number );
            break;
         case OMEGA_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            pattern->setOmega( f_number );
            break;
         case LAMBDA_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            pattern->setLambda( f_number );
            break;

         case NOISE_GENERATOR_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            pattern->setNoiseGenerator( ( PMPattern::PMNoiseType ) ( i_number ) );
            break;
         default:
            break;
      }

      if( type && normal )
      {
         // try to parse the normal pattern depth
         double depth;
         if( parseFloat( depth, true ) )
            pattern->setDepth( depth );
      }
   }
   while( oldConsumed != m_consumedTokens );

   return true;
}

bool PMPovrayParser::parseBlendMapModifiers( PMBlendMapModifiers* blend )
{
   int oldConsumed;
   double f_number;

   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
         case FREQUENCY_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            blend->enableFrequency( true );
            blend->setFrequency( f_number );
            break;
         case PHASE_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            blend->enablePhase( true );
            blend->setPhase( f_number );
            break;
         case RAMP_WAVE_TOK:
            nextToken();
            blend->enableWaveForm( true );
            blend->setWaveFormType( PMBlendMapModifiers::RampWave );
            break;
         case TRIANGLE_WAVE_TOK:
            nextToken();
            blend->enableWaveForm( true );
            blend->setWaveFormType( PMBlendMapModifiers::TriangleWave );
            break;
         case SINE_WAVE_TOK:
            nextToken();
            blend->enableWaveForm( true );
            blend->setWaveFormType( PMBlendMapModifiers::SineWave );
            break;
         case SCALLOP_WAVE_TOK:
            nextToken();
            blend->enableWaveForm( true );
            blend->setWaveFormType( PMBlendMapModifiers::ScallopWave );
            break;
         case CUBIC_WAVE_TOK:
            nextToken();
            blend->enableWaveForm( true );
            blend->setWaveFormType( PMBlendMapModifiers::CubicWave );
            break;
         case POLY_WAVE_TOK:
            nextToken();
            blend->enableWaveForm( true );
            blend->setWaveFormType( PMBlendMapModifiers::PolyWave );
            if( parseFloat( f_number, true ) )
               blend->setWaveFormExponent( f_number );
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   return true;
}

bool PMPovrayParser::parseWarp( PMWarp* warp )
{
   int oldConsumed;
   PMVector vector;
   double f_number;
   int i_number;
   bool parsedFirst;
   bool mapping;

   if( !parseToken( WARP_TOK, "warp" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   mapping = false;
   parsedFirst = false;
   do
   {
      oldConsumed = m_consumedTokens;
      if( !parsedFirst &&
          ( m_token != REPEAT_TOK ||
            m_token != BLACK_HOLE_TOK ||
            m_token != TURBULENCE_TOK ||
            m_token != CYLINDRICAL_TOK ||
            m_token != SPHERICAL_TOK ||
            m_token != TOROIDAL_TOK ||
            m_token != PLANAR_TOK ) )
      {
         printError( ( "Expecting a warp type" ) );
         return false;
      }
      switch( m_token )
      {
         case REPEAT_TOK:
            nextToken();
            if( !parsedFirst )
            {
               warp->setWarpType( PMWarp::Repeat );
               if( !parseVector( vector ) )
                  return false;
               warp->setDirection( vector );
               parsedFirst = true;
            }
            else
            {
               if( !parseVector( vector ) )
                  return false;
               warp->setRepeat( vector );
            }
            break;
         case OFFSET_TOK:
            nextToken();
            if( !parseVector( vector ) )
               return false;
            warp->setOffset( vector );
            break;
         case FLIP_TOK:
            nextToken();
            if( !parseVector( vector ) )
               return false;
            warp->setFlip( vector );
            break;
         case BLACK_HOLE_TOK:
            nextToken();
            warp->setWarpType( PMWarp::BlackHole );
            if( !parseVector( vector ) )
               return false;
            warp->setLocation( vector );
            if( !parseToken( ',' ) )
               return false;
            if( !parseFloat( f_number ) )
               return false;
            warp->setRadius( f_number );
            parsedFirst = true;
            break;
         case STRENGTH_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            warp->setStrength( f_number );
            break;
         case FALLOFF_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            warp->setFalloff( f_number );
            break;
         case INVERSE_TOK:
            nextToken();
            warp->setInverse( true );
            break;
         case TURBULENCE_TOK:
            if( !parsedFirst )
            {
               nextToken();
               warp->setWarpType( PMWarp::Turbulence );
               if( !parseVector( vector ) )
                  return false;
               warp->setValueVector( vector );
               parsedFirst = true;
            }
            else
            {
               if( !parseVector( vector ) )
                  return false;
               warp->setTurbulence( vector );
            }
            break;
         case OCTAVES_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            warp->setOctaves( i_number );
            break;
         case OMEGA_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            warp->setOmega( f_number );
            break;
         case LAMBDA_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            warp->setLambda( f_number );
            break;
         case CYLINDRICAL_TOK:
            warp->setWarpType( PMWarp::Cylindrical );
            mapping = true;
            break;
         case SPHERICAL_TOK:
            warp->setWarpType( PMWarp::Spherical );
            mapping = true;
            break;
         case TOROIDAL_TOK:
            warp->setWarpType( PMWarp::Toroidal );
            mapping = true;
            break;
         case PLANAR_TOK:
            nextToken();
            warp->setWarpType( PMWarp::Planar );
            if( parseVector( vector ) )
            {
               warp->setOrientation( vector );
               if( parseToken( ',' ) )
               {
                  if( !parseFloat( f_number ) )
                     return false;
                  warp->setDistExp( f_number );
               }
            }
            parsedFirst = true;
            break;
         case DIST_EXP_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            warp->setDistExp( f_number );
            break;
         case MAJOR_RADIUS_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            warp->setMajorRadius( f_number );
            break;
         default:
            break;
      }

      if( mapping)
      {
         nextToken();
         if( !parseVector( vector ) )
            return false;
         warp->setOrientation( vector );
         parsedFirst = true;
         mapping = false;
      }

   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parsePigment( PMPigment* pigment, bool parseOuter )
{
   PMColor c;
   PMSolidColor* sc;
   int oldConsumed;

   if( parseOuter )
   {
      if( !parseToken( PIGMENT_TOK, "pigment" ) )
         return false;

      if( !parseToken( '{' ) )
         return false;
   }

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMSymbol* s = getSymbol( id );
      bool skipID = false;

      if( s )
         if( s->type() == PMSymbol::Value )
            skipID = true;

      if( !skipID )
      {
         PMDeclare* decl = checkLink( id );
         if( decl )
         {
            if( !pigment->setLinkedObject( decl ) )
               printError( ( "Wrong declare type" ) );
         }
         nextToken();
      }
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( pigment );//prima volta return-327-color

      switch( m_token )//327color 597srgb
      {
         case '<':
         case SRGB_TOK:
         case COLOR_TOK:
         case COLOUR_TOK:
         case RGB_TOK:
         case RGBT_TOK:
         case RGBF_TOK:
         case RGBFT_TOK:
         case RED_TOK:
         case GREEN_TOK:
         case BLUE_TOK:
         case TRANSMIT_TOK:
         case FILTER_TOK:
         case ID_TOK:
            if( parseColor( c ) )
            {
               sc = new PMSolidColor( m_pPart );
               c.setSrgb( srgb );
               srgb = false;
               sc->setColor( c );
               if( !insertChild( sc, pigment ) )
               {
                  delete sc;
                  sc = 0;
               }
            }
            break;
         case UV_MAPPING_TOK:
            nextToken();
            pigment->setUVMapping( parseBool() );
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( parseOuter )
      if( !parseToken( '}' ) )
         return false;

   return true;
}

bool PMPovrayParser::parseNormal( PMNormal* normal )
{
   double f_number;
   int oldConsumed;

   if( !parseToken( NORMAL_TOK, "normal" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !normal->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( normal );
      switch( m_token )
      {
         case BUMP_SIZE_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            normal->enableBumpSize( true );
            normal->setBumpSize( f_number );
            break;
         case ACCURACY_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            normal->setAccuracy( f_number );
            break;
         case UV_MAPPING_TOK:
            nextToken();
            normal->setUVMapping( parseBool() );
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseTextureMap( PMTextureMap* textureMap )
{
   int oldConsumed;
   double f_number1;
   PMTexture* texture;
   QList<double> mapValues;

   if( !parseToken( TEXTURE_MAP_TOK, "texture_map" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !textureMap->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;

      if( m_token == '[' )
      {
         nextToken();

         if( !parseFloat( f_number1 ) )
            return false;
         mapValues.append( f_number1 );
         texture = new PMTexture( m_pPart );

         parseTexture( texture, false );

         if( !insertChild( texture, textureMap ) )
            delete texture;

         if( !parseToken( ']' ) )
            return false;
      }
   }
   while( oldConsumed != m_consumedTokens );

   textureMap->setMapValues( mapValues );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parsePigmentMap( PMPigmentMap* pigmentMap )
{
   int oldConsumed;
   double f_number1;
   PMPigment* pigment;
   QList<double> mapValues;

   if( !parseToken( PIGMENT_MAP_TOK, "pigment_map" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !pigmentMap->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;

      if( m_token == '[' )
      {
         nextToken();
         if( !parseFloat( f_number1 ) )
            return false;
         mapValues.append( f_number1 );
         pigment = new PMPigment( m_pPart );

         parsePigment( pigment, false );
         if( !insertChild( pigment, pigmentMap ) )
            delete pigment;
         if( !parseToken( ']' ) )
            return false;
      }
   }
   while( oldConsumed != m_consumedTokens );

   pigmentMap->setMapValues( mapValues );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseNormalMap( PMNormalMap* normalMap )
{
   int oldConsumed;
   double f_number1;
   PMNormal* normal;
   QList<double> mapValues;

   if( !parseToken( NORMAL_MAP_TOK, "normal_map" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !normalMap->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;

      //  If we find '}' no need to search for an entry
      if( m_token != '}' && parseToken( '[' ) )
      {
         if( !parseFloat( f_number1 ) )
            return false;
         mapValues.append( f_number1 );
         normal = new PMNormal( m_pPart );
         if( !parseNormal( normal ) )
         {
            delete normal;
            return false;
         }
         if( !insertChild( normal, normalMap ) )
            delete normal;
         if( !parseToken( ']' ) )
            return false;
      }
   }
   while( oldConsumed != m_consumedTokens );

   normalMap->setMapValues( mapValues );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseBumpMap( PMBumpMap* bumpMap )
{
   int oldConsumed;
   int i_number;
   double f_number;

   if( !parseToken( BUMP_MAP_TOK, "bump_map" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   // Parse the bitmap type
   if( m_token != STRING_TOK )
   {
      switch( m_token )
      {
         case GIF_TOK:
            bumpMap->setBitmapType( PMBumpMap::BitmapGif );
            nextToken();
            break;
         case TGA_TOK:
            bumpMap->setBitmapType( PMBumpMap::BitmapTga );
            nextToken();
            break;
         case IFF_TOK:
            bumpMap->setBitmapType( PMBumpMap::BitmapIff );
            nextToken();
            break;
         case PPM_TOK:
            bumpMap->setBitmapType( PMBumpMap::BitmapPpm );
            nextToken();
            break;
         case PGM_TOK:
            bumpMap->setBitmapType( PMBumpMap::BitmapPgm );
            nextToken();
            break;
         case PNG_TOK:
            bumpMap->setBitmapType( PMBumpMap::BitmapPng );
            nextToken();
            break;
         case SYS_TOK:
            bumpMap->setBitmapType( PMBumpMap::BitmapSys );
            nextToken();
            break;
         default:
            printError( ( "Unknown bitmap type" ) );
            return false;
      }
   }

   // Parse the bitmap file name
   if( m_token != STRING_TOK )
   {
      printError( ( "Expecting a file name." ) );
      return false;
   }
   else
   {
      bumpMap->setBitmapFileName( m_pScanner->sValue() );
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
         case ONCE_TOK:
            nextToken();
            bumpMap->enableOnce( true );
            break;
         case MAP_TYPE_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            switch( i_number )
            {
               case 0:
                  bumpMap->setMapType( PMBumpMap::MapPlanar );
                  break;
               case 1:
                  bumpMap->setMapType( PMBumpMap::MapSpherical );
                  break;
               case 2:
                  bumpMap->setMapType( PMBumpMap::MapCylindrical );
                  break;
               case 5:
                  bumpMap->setMapType( PMBumpMap::MapToroidal );
                  break;
            }
            break;
         case INTERPOLATE_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            switch( i_number )
            {
               case 2:
                  bumpMap->setInterpolateType( PMBumpMap::InterpolateBilinear );
                  break;
               case 4:
                  bumpMap->setInterpolateType( PMBumpMap::InterpolateNormalized );
                  break;
            }
            break;
         case USE_INDEX_TOK:
            nextToken();
            bumpMap->enableUseIndex( true );
            break;
         case BUMP_SIZE_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            bumpMap->setBumpSize( f_number );
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseMaterialMap( PMMaterialMap* materialMap )
{
   int oldConsumed;
   int i_number;

   if( !parseToken( MATERIAL_MAP_TOK, "material_map" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   // Parse the bitmap type
   if( m_token != STRING_TOK )
   {
      switch( m_token )
      {
         case GIF_TOK:
            materialMap->setBitmapType( PMMaterialMap::BitmapGif );
            nextToken();
            break;
         case TGA_TOK:
            materialMap->setBitmapType( PMMaterialMap::BitmapTga );
            nextToken();
            break;
         case IFF_TOK:
            materialMap->setBitmapType( PMMaterialMap::BitmapIff );
            nextToken();
            break;
         case PPM_TOK:
            materialMap->setBitmapType( PMMaterialMap::BitmapPpm );
            nextToken();
            break;
         case PGM_TOK:
            materialMap->setBitmapType( PMMaterialMap::BitmapPgm );
            nextToken();
            break;
         case PNG_TOK:
            materialMap->setBitmapType( PMMaterialMap::BitmapPng );
            nextToken();
            break;
         case SYS_TOK:
            materialMap->setBitmapType( PMMaterialMap::BitmapSys );
            nextToken();
            break;
         default:
            printError( ( "Unknown bitmap type" ) );
            return false;
      }
   }

   // Parse the bitmap file name
   if( m_token != STRING_TOK )
   {
      printError( ( "Expecting a file name." ) );
      return false;
   }
   else
   {
      materialMap->setBitmapFileName( m_pScanner->sValue() );
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( materialMap );
      switch( m_token )
      {
         case ONCE_TOK:
            nextToken();
            materialMap->enableOnce( true );
            break;
         case MAP_TYPE_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            switch( i_number )
            {
               case 0:
                  materialMap->setMapType( PMMaterialMap::MapPlanar );
                  break;
               case 1:
                  materialMap->setMapType( PMMaterialMap::MapSpherical );
                  break;
               case 2:
                  materialMap->setMapType( PMMaterialMap::MapCylindrical );
                  break;
               case 5:
                  materialMap->setMapType( PMMaterialMap::MapToroidal );
                  break;
            }
            break;
         case INTERPOLATE_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            switch( i_number )
            {
               case 2:
                  materialMap->setInterpolateType( PMMaterialMap::InterpolateBilinear );
                  break;
               case 4:
                  materialMap->setInterpolateType( PMMaterialMap::InterpolateNormalized );
                  break;
            }
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseSlopeMap( PMSlopeMap* slopeMap )
{
   int oldConsumed;
   double f_number1;
   PMSlope* slope;
   QList<double> mapValues;

   if( !parseToken( SLOPE_MAP_TOK, "slope_map" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !slopeMap->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;

      //  If we find '}' no need to search for an entry
      if( m_token != '}' && parseToken( '[' ) )
      {
         if( !parseFloat( f_number1 ) )
            return false;
         mapValues.append( f_number1 );
         slope = new PMSlope( m_pPart );
         if( !parseSlope( slope ) )
         {
            delete slope;
            return false;
         }
         if( !insertChild( slope, slopeMap ) )
            delete slope;
         if( !parseToken( ']' ) )
            return false;
      }
   }
   while( oldConsumed != m_consumedTokens );

   slopeMap->setMapValues( mapValues );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseDensityMap( PMDensityMap* densityMap )
{
   int oldConsumed;
   double f_number1;
   PMDensity* density;
   QList<double> mapValues;

   if( !parseToken( DENSITY_MAP_TOK, "density_map" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !densityMap->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;

      //  If we find '}' no need to search for an entry
      if( m_token != '}' && parseToken( '[' ) )
      {
         if( !parseFloat( f_number1 ) )
            return false;
         mapValues.append( f_number1 );
         density = new PMDensity( m_pPart );
         if( !parseDensity( density ) )
         {
            delete density;
            return false;
         }
         if( !insertChild( density, densityMap ) )
            delete density;
         if( !parseToken( ']' ) )
            return false;
      }
   }
   while( oldConsumed != m_consumedTokens );

   densityMap->setMapValues( mapValues );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseImageMap( PMImageMap* imageMap )
{
   int oldConsumed;
   int i_number;
   double f_number;
   PMPaletteValue newPaletteValue;
   QList<PMPaletteValue> l_valuesFilter;
   QList<PMPaletteValue> l_valuesTransmit;

   if( !parseToken( IMAGE_MAP_TOK, "image_map" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   // Parse the bitmap type
   if( m_token != STRING_TOK )
   {
      switch( m_token )
      {
         case GIF_TOK:
            imageMap->setBitmapType( PMImageMap::BitmapGif );
            nextToken();
            break;
         case TGA_TOK:
            imageMap->setBitmapType( PMImageMap::BitmapTga );
            nextToken();
            break;
         case IFF_TOK:
            imageMap->setBitmapType( PMImageMap::BitmapIff );
            nextToken();
            break;
         case PPM_TOK:
            imageMap->setBitmapType( PMImageMap::BitmapPpm );
            nextToken();
            break;
         case PGM_TOK:
            imageMap->setBitmapType( PMImageMap::BitmapPgm );
            nextToken();
            break;
         case PNG_TOK:
            imageMap->setBitmapType( PMImageMap::BitmapPng );
            nextToken();
            break;
         case SYS_TOK:
            imageMap->setBitmapType( PMImageMap::BitmapSys );
            nextToken();
            break;
         default:
            printError( ( "Unknown bitmap type" ) );
            return false;
      }
   }

   // Parse the bitmap file name
   if( m_token != STRING_TOK )
   {
      printError( ( "Expecting a file name." ) );
      return false;
   }
   else
   {
      imageMap->setBitmapFileName( m_pScanner->sValue() );
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
         case TRANSMIT_TOK:
            nextToken();
            if( m_token == ALL_TOK )
            {
               nextToken();
               if( !parseFloat( f_number ) )
                  return false;
               imageMap->enableTransmitAll( true );
               imageMap->setTransmitAll( f_number );
            }
            else
            {
               if( !parseInt( i_number ) )
                  return false;
               parseToken( ',' );
               if( !parseFloat( f_number ) )
                  return false;
               newPaletteValue.setIndex( i_number );
               newPaletteValue.setValue( f_number );
               l_valuesTransmit.append( newPaletteValue );
            }
            break;
         case FILTER_TOK:
            nextToken();
            if( m_token == ALL_TOK )
            {
               nextToken();
               if( !parseFloat( f_number ) )
                  return false;
               imageMap->enableFilterAll( true );
               imageMap->setFilterAll( f_number );
            }
            else
            {
               if( !parseInt( i_number ) )
                  return false;
               parseToken( ',' );
               if( !parseFloat( f_number ) )
                  return false;
               newPaletteValue.setIndex( i_number );
               newPaletteValue.setValue( f_number );
               l_valuesFilter.append( newPaletteValue );
            }
            break;
         case ONCE_TOK:
            nextToken();
            imageMap->enableOnce( true );
            break;
         case MAP_TYPE_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            switch( i_number )
            {
               case 0:
                  imageMap->setMapType( PMImageMap::MapPlanar );
                  break;
               case 1:
                  imageMap->setMapType( PMImageMap::MapSpherical );
                  break;
               case 2:
                  imageMap->setMapType( PMImageMap::MapCylindrical );
                  break;
               case 5:
                  imageMap->setMapType( PMImageMap::MapToroidal );
                  break;
            }
            break;
         case INTERPOLATE_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            switch( i_number )
            {
               case 2:
                  imageMap->setInterpolateType( PMImageMap::InterpolateBilinear );
                  break;
               case 4:
                  imageMap->setInterpolateType( PMImageMap::InterpolateNormalized );
                  break;
            }
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );
   imageMap->setFilters( l_valuesFilter );
   imageMap->setTransmits( l_valuesTransmit );


   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parsePigmentList( PMPigmentList* pigmentList, int expectedItems )
{
   int oldConsumed;
   PMPigment* pigment;

   do
   {
      oldConsumed = m_consumedTokens;
      pigment = new PMPigment( m_pPart );
      if( !parsePigment( pigment ) )
      {
         delete pigment;
         return false;
      }
      if( !insertChild( pigment, pigmentList ) )
         delete pigment;

      //  In the last entry don't expect a comma
      if( expectedItems-- )
         if( m_token == ',' )
            nextToken();
   }
   while( oldConsumed != m_consumedTokens && expectedItems );

   return true;
}

bool PMPovrayParser::parseColorList( PMColorList* colorList, int expectedItems )
{
   int oldConsumed;
   PMColor color;
   PMSolidColor* sc;

   do
   {
      oldConsumed = m_consumedTokens;
      if( !parseColor( color ) )
      {
         return false;
      }
      sc = new PMSolidColor( m_pPart );
      sc->setColor( color );
      if( !insertChild( sc, colorList ) )
         delete sc;

      //  In the last entry don't expect a comma
      if( expectedItems-- )
         if( m_token == ',' )
            nextToken();
   }
   while( oldConsumed != m_consumedTokens && expectedItems );

   return true;
}

bool PMPovrayParser::parseNormalList( PMNormalList* normalList, int expectedItems )
{
   int oldConsumed;
   PMNormal* normal;

   do
   {
      oldConsumed = m_consumedTokens;
      normal = new PMNormal( m_pPart );
      if( !parseNormal( normal ) )
      {
         delete normal;
         return false;
      }
      if( !insertChild( normal, normalList ) )
         delete normal;

      //  In the last entry don't expect a comma
      if( expectedItems-- )
         if( m_token == ',' )
            nextToken();
   }
   while( oldConsumed != m_consumedTokens && expectedItems );

   return true;
}

bool PMPovrayParser::parseTextureList( PMTextureList* textureList, int expectedItems )
{
   int oldConsumed;
   PMTexture* texture;

   do
   {
      oldConsumed = m_consumedTokens;
      texture = new PMTexture( m_pPart );
      if( !parseTexture( texture ) )
      {
         delete texture;
         return false;
      }
      if( !insertChild( texture, textureList ) )
         delete texture;

      //  In the last entry don't expect a comma
      if( expectedItems-- )
         if( m_token == ',' )
            nextToken();
   }
   while( oldConsumed != m_consumedTokens && expectedItems );

   return true;
}

bool PMPovrayParser::parseDensityList( PMDensityList* densityList, int expectedItems )
{
   int oldConsumed;
   PMDensity* density;

   do
   {
      oldConsumed = m_consumedTokens;
      density = new PMDensity( m_pPart );
      if( !parseDensity( density ) )
      {
         delete density;
         return false;
      }
      if( !insertChild( density, densityList ) )
         delete density;

      //  In the last entry don't expect a comma
      if( expectedItems-- )
         if( m_token == ',' )
            nextToken();
   }
   while( oldConsumed != m_consumedTokens && expectedItems );

   return true;
}

bool PMPovrayParser::parseColorMap( PMColorMap* colorMap )
{
   int oldConsumed;
   double f_number1, f_number2;
   PMColor color1, color2;
   PMSolidColor* solidColor;
   PMSolidColor* lastColor = 0;
   QList<double> mapValues;
   bool newEntry;
   bool twoColors;

   if( m_token != COLOR_MAP_TOK && m_token != COLOUR_MAP_TOK )
      return false;
   nextToken();
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !colorMap->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;

      if( m_token == '[' )
      {
         nextToken();
         if( !parseFloat( f_number1 ) )
            return false;

         twoColors = false;
         if( m_token == ',' )
         {
            twoColors = true;
            nextToken();
         }
         else if( ( m_token == INTEGER_TOK ) || ( m_token == FLOAT_TOK ) )
            twoColors = true;

         if( twoColors )
         {
            // Two colors in the same entry

            if( parseFloat( f_number2 ) )
            {
               if( !parseColor( color1 ) )
                  return false;
               if( !parseColor( color2 ) )
                  return false;
               //  If the first value doesn't pick up from the previous,
               //  or the color is different...
               newEntry = true;
               if( lastColor && !mapValues.isEmpty() )
                  if( ( mapValues.last() == f_number1 ) &&
                      ( lastColor->color() == color1 ) )
                     newEntry = false;

               if( newEntry )
               {
                  // ... add the two colors in two different entries ...
                  mapValues.append( f_number1 );
                  solidColor = new PMSolidColor( m_pPart );
                  solidColor->setColor( color1 );
                  if( !insertChild( solidColor, colorMap ) )
                     delete solidColor;
                  else
                     lastColor = solidColor;

                  mapValues.append( f_number2 );
                  solidColor = new PMSolidColor( m_pPart );
                  solidColor->setColor( color2 );
                  if( !insertChild( solidColor, colorMap ) )
                     delete solidColor;
                  else
                     lastColor = solidColor;
               }
               else
               {
                  // ... else just add the last value and color
                  mapValues.append( f_number2 );
                  solidColor = new PMSolidColor( m_pPart );
                  solidColor->setColor( color2 );
                  if( !insertChild( solidColor, colorMap ) )
                     delete solidColor;
                  else
                     lastColor = solidColor;
               }
            }
         }
         else
         {
            // Only one color in the entry
            if( !parseColor( color1 ) )
               return false;
            mapValues.append( f_number1 );
            solidColor = new PMSolidColor( m_pPart );
            solidColor->setColor( color1 );
            if( !insertChild( solidColor, colorMap ) )
               delete solidColor;
            else
               lastColor = solidColor;
         }
         if( !parseToken( ']' ) )
            return false;
      }
   }
   while( oldConsumed != m_consumedTokens );

   colorMap->setMapValues( mapValues );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseSkySphere( PMSkySphere* sky )
{
   int oldConsumed;

   if( !parseToken( SKY_SPHERE_TOK, "sky_sphere" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !sky->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( sky );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseRainbow( PMRainbow* rainbow )
{
   PMVector vector;
   double f_number;
   int oldConsumed;

   if( !parseToken( RAINBOW_TOK, "rainbow" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !rainbow->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( rainbow );
      switch( m_token )
      {
         case DIRECTION_TOK:
            nextToken();
            if( parseVector( vector ) )
            {
               rainbow->enableDirection( true );
               rainbow->setDirection( vector );
            }
            break;
         case ANGLE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               rainbow->enableAngle( true );
               rainbow->setAngle( f_number );
            }
            break;
         case WIDTH_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               rainbow->enableWidth( true );
               rainbow->setWidth( f_number );
            }
            break;
         case DISTANCE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               rainbow->enableDistance( true );
               rainbow->setDistance( f_number );
            }
            break;
         case JITTER_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               rainbow->enableJitter( true );
               rainbow->setJitter( f_number );
            }
            break;
         case UP_TOK:
            nextToken();
            if( parseVector( vector ) )
            {
               rainbow->enableUp( true );
               rainbow->setUp( vector );
            }
            break;
         case ARC_ANGLE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               rainbow->enableArcAngle( true );
               rainbow->setArcAngle( f_number );
            }
            break;
         case FALLOFF_ANGLE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               rainbow->enableFalloffAngle( true );
               rainbow->setFalloffAngle( f_number );
            }
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseFog( PMFog* fog )
{
   PMColor color;
   PMVector vector;
   double f_number;
   int i_number;
   int fog_type;
   int oldConsumed;

   if( !parseToken( FOG_TOK, "fog" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !fog->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   fog_type = 1;
   if( parseToken( FOG_TYPE_TOK, "fog_type" ) )
   {
      if( !parseInt( i_number ) )
         return false;
      fog_type = i_number;
   }

   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
         case DISTANCE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
               fog->setDistance( f_number );
            break;
         case '<':
         case COLOR_TOK:
         case COLOUR_TOK:
         case RGB_TOK:
         case SRGB_TOK:
         case RGBT_TOK:
         case RGBF_TOK:
         case RGBFT_TOK:
         case RED_TOK:
         case GREEN_TOK:
         case BLUE_TOK:
         case TRANSMIT_TOK:
         case FILTER_TOK:
         case ID_TOK:
            if( parseColor( color ) )
               fog->setColor( color );
            break;
         case TURBULENCE_TOK:
            nextToken();
            fog->enableTurbulence( true );
            if( !parseVector( vector ) )
               return false;
            fog->setValueVector( vector );
            break;
         case OCTAVES_TOK:
            nextToken();
            if( !parseInt( i_number ) )
               return false;
            fog->setOctaves( i_number );
            break;
         case OMEGA_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            fog->setOmega( f_number );
            break;
         case LAMBDA_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            fog->setLambda( f_number );
            break;
         case TURB_DEPTH_TOK:
            nextToken();
            if( !parseFloat( f_number ) )
               return false;
            fog->setDepth( f_number );
            break;
         case FOG_OFFSET_TOK:
            nextToken();
            fog_type = 2;
            if( parseFloat( f_number ) )
               fog->setFogOffset( f_number );
            break;
         case FOG_ALT_TOK:
            nextToken();
            fog_type = 2;
            if( parseFloat( f_number ) )
               fog->setFogAlt( f_number );
            break;
         case UP_TOK:
            nextToken();
            fog_type = 2;
            if( !parseVector( vector ) )
               return false;
            fog->setUp( vector );
            break;
         default:
            break;
      }
      // Only parseChildObjects() if the token is not turbulence, because this
      // function parses that token.
      if( m_token != TURBULENCE_TOK )
         parseChildObjects( fog );
   }
   while( oldConsumed != m_consumedTokens );

   fog->setFogType( fog_type );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseMedia( PMMedia* media )
{
   PMColor color;
   double f_number;
   int i_number;
   int oldConsumed, oldConsumed1;

   if( !parseToken( MEDIA_TOK, "media" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !media->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( media );
      switch( m_token )
      {
         case METHOD_TOK:
            nextToken();
            if( parseInt( i_number ) )
               media->setMethod( i_number );
            break;
         case INTERVALS_TOK:
            nextToken();
            if( parseInt( i_number ) )
               media->setIntervals( i_number );
            break;
         case SAMPLES_TOK:
            nextToken();
            if( parseInt( i_number ) )
               media->setSamplesMin( i_number );
            parseToken( ',' );
            if( parseInt( i_number ) )
               media->setSamplesMax( i_number );
            break;
         case CONFIDENCE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
               media->setConfidence( f_number );
            break;
         case VARIANCE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
               media->setVariance( f_number );
            break;
         case RATIO_TOK:
            nextToken();
            if( parseFloat( f_number ) )
               media->setRatio( f_number );
            break;
         case AA_LEVEL_TOK:
            nextToken();
            if ( parseInt( i_number ) )
               media->setAALevel( i_number );
            break;
         case AA_THRESHOLD_TOK:
            nextToken();
            if ( parseFloat( f_number ) )
               media->setAAThreshold( f_number );
            break;
         case ABSORPTION_TOK:
            nextToken();
            if( parseColor( color ) )
            {
               media->enableAbsorption( true );
               media->setAbsorption( color );
            }
            break;
         case EMISSION_TOK:
            nextToken();
            media->enableEmission( true );
            if( parseColor( color ) )
               media->setEmission( color );
            break;
         case SCATTERING_TOK:
            nextToken();
            parseToken( '{' );
            media->enableScattering( true );
            if( parseInt( i_number ) )
               media->setScatteringType( i_number );
            parseToken( ',' );
            if( parseColor( color ) )
               media->setScatteringColor( color );
            do
            {
               oldConsumed1 = m_consumedTokens;
               switch( m_token )
               {
                  case ECCENTRICITY_TOK:
                     nextToken();
                     if( parseFloat( f_number ) )
                        media->setScatteringEccentricity( f_number );
                     break;
                  case EXTINCTION_TOK:
                     nextToken();
                     if( parseFloat( f_number ) )
                        media->setScatteringExtinction( f_number );
                     break;
                  default:
                     break;
               }
            }
            while( oldConsumed1 != m_consumedTokens );
            parseToken( '}' );
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseDensity( PMDensity* density )
{
   int oldConsumed;

   if( !parseToken( DENSITY_TOK, "density" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !density->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( density );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseInterior( PMInterior* interior )
{
   double f_number;
   int i_number;
   int oldConsumed;

   if( !parseToken( INTERIOR_TOK, "interior" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !interior->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( interior );
      switch( m_token )
      {
         case IOR_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               interior->enableIor( true );
               interior->setIor( f_number );
            }
            break;
         case CAUSTICS_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               interior->enableCaustics( true );
               interior->setCaustics( f_number );
            }
            break;
         case DISPERSION_TOK:
            nextToken();
            if ( parseFloat( f_number ) )
            {
               interior->enableDispersion( true );
               interior->setDispersion( f_number );
            }
            break;
         case DISPERSION_SAMPLES_TOK:
            nextToken();
            if ( parseInt( i_number ) )
            {
               interior->enableDispSamples( true );
               interior->setDispSamples( i_number );
            }
            break;
         case FADE_DISTANCE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               interior->enableFadeDistance( true );
               interior->setFadeDistance( f_number );
            }
            break;
         case FADE_POWER_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               interior->enableFadePower( true );
               interior->setFadePower( f_number );
            }
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseMaterial( PMMaterial* material )
{
   int oldConsumed;

   if( !parseToken( MATERIAL_TOK, "material" ) )
      return false;

   if( !parseToken( '{' ) )
      return false;

   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !material->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( material );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseSlope( PMSlope* slope )
{
   double f_number;

   if( !parseToken( '<' ) )
      return false;
   if( !parseFloat( f_number ) )
      return false;
   slope->setHeight( f_number );
   if( !parseToken( ',' ) )
      return false;
   if( !parseFloat( f_number ) )
      return false;
   slope->setSlope( f_number );
   if( !parseToken( '>' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseGlobalSettings( PMGlobalSettings* globalsettings )
{
   PMColor color;
   double f_number;
   int i_number;
   int oldConsumed;

   // Initial global settings tokens
   if( !parseToken( GLOBAL_SETTINGS_TOK, "global_settings" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   // Parse global settings tokens
   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( globalsettings );

      switch( m_token )
      {
         case ADC_BAILOUT_TOK:
            nextToken();
            if( parseFloat( f_number ) )
               globalsettings->setAdcBailout( f_number );
            break;
         case AMBIENT_LIGHT_TOK:
            nextToken();
            if( parseColor( color ) )
               globalsettings->setAmbientLight( color );
            break;
         case ASSUMED_GAMMA_TOK:
            nextToken();
            if( parseFloat( f_number ) )
               globalsettings->setAssumedGamma( f_number );
            break;
         case HF_GRAY_16_TOK:
            nextToken();
            switch( m_token )
            {
               case ON_TOK:
                  globalsettings->setHfGray16( true );
                  nextToken();
                  break;
               case OFF_TOK:
                  globalsettings->setHfGray16( false );
                  nextToken();
                  break;
               default:
                  break;
            }
            break;
         case IRID_WAVELENGTH_TOK:
            nextToken();
            if( parseColor( color ) )
               globalsettings->setIridWaveLength( color );
            break;
         case MAX_INTERSECTIONS_TOK:
            nextToken();
            if( parseInt( i_number ) )
               globalsettings->setMaxIntersections( i_number );
            break;
         case MAX_TRACE_LEVEL_TOK:
            nextToken();
            if( parseInt( i_number ) )
               globalsettings->setMaxTraceLevel( i_number );
            break;
         case NUMBER_OF_WAVES_TOK:
            nextToken();
            if( parseInt( i_number ) )
               globalsettings->setNumberWaves( i_number );
            break;
         case NOISE_GENERATOR_TOK:
            nextToken();
            if ( parseInt( i_number ) )
               globalsettings->setNoiseGenerator(
                  ( PMGlobalSettings::PMNoiseType ) ( i_number - 1 ) );
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseFinish( PMFinish* finish )
{
   PMColor color;
   double f_number;
   int oldConsumed, oldConsumed1;

   //  Initial finish tokens "finish {"
   if( !parseToken( FINISH_TOK, "finish" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   //  Parse a possible declare link identifier
   if( m_token == ID_TOK )
   {
      QString id( m_pScanner->sValue() );
      PMDeclare* decl = checkLink( id );
      if( decl )
      {
         if( !finish->setLinkedObject( decl ) )
            printError( ( "Wrong declare type" ) );
      }
      nextToken();
   }

   // Parse finish tokens
   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
        case EMISSION_TOK:
            nextToken();
            finish->enableEmission( true );
            if( parseColor( color ) )
               finish->setEmissionColor( color );
            break;
         case AMBIENT_TOK:
            nextToken();
            finish->enableAmbient( true );
            if( parseColor( color ) )
               finish->setAmbientColor( color );
            break;
         case DIFFUSE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               finish->enableDiffuse( true );
               finish->setDiffuse( f_number );
            }
            break;
         case BRILLIANCE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               finish->enableBrilliance( true );
               finish->setBrilliance( f_number );
            }
            break;
         case PHONG_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               finish->enablePhong( true );
               finish->setPhong( f_number );
            }
            break;
         case PHONG_SIZE_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               finish->enablePhongSize( true );
               finish->setPhongSize( f_number );
            }
            break;
         case METALLIC_TOK:
            nextToken();
            finish->enableMetallic( true );
            finish->setMetallic( 1.0 );
            if( parseFloat( f_number, true ) )
               finish->setMetallic( f_number );
            break;
         case SPECULAR_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               finish->enableSpecular( true );
               finish->setSpecular( f_number );
            }
            break;
         case ROUGHNESS_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               finish->enableRoughness( true );
               finish->setRoughness( f_number );
            }
            break;
         case CRAND_TOK:
            nextToken();
            if( parseFloat( f_number ) )
            {
               finish->enableCrand( true );
               finish->setCrand( f_number );
            }
            break;
         case CONSERVE_ENERGY_TOK:
            nextToken();
            finish->setConserveEnergy( parseBool() );
            break;
         case REFLECTION_TOK:
            nextToken();
            finish->enableReflection( true );
            if( !parseToken( '{' ) )
            {
               printError( ( "Using Old Reflection Syntax" ) );
               if( parseColor( color ) )
                  finish->setReflectionColor( color );
            }
            else if( parseColor( color ) )
            {
               if( parseToken( ',' ) )
               {
                  finish->enableReflectionMin( true );
                  finish->setReflectionMinColor( color );
                  if( parseColor( color ) )
                     finish->setReflectionColor( color );
                  else
                     return false;
               }
               else
                  finish->setReflectionColor( color );

               do
               {
                  oldConsumed1 = m_consumedTokens;
                  switch( m_token )
                  {
                     case FRESNEL_TOK:
                        nextToken();
                        finish->setReflectionFresnel( parseBool() );
                        break;
                     case FALLOFF_TOK:
                        nextToken();
                        if( parseFloat( f_number ) )
                        {
                           finish->enableRefFalloff( true );
                           finish->setReflectionFalloff( f_number );
                        }
                        break;
                     case EXPONENT_TOK:
                        nextToken();
                        if( parseFloat( f_number ) )
                        {
                           finish->enableRefExponent( true );
                           finish->setReflectionExponent( f_number );
                        }
                        break;
                     case METALLIC_TOK:
                        nextToken();
                        if ( parseFloat( f_number ) )
                        {
                           finish->enableRefMetallic( true );
                           finish->setReflectionMetallic( f_number );
                        }
                        break;
                     default:
                        break;
                  }
               }
               while( oldConsumed1 != m_consumedTokens );
               parseToken( '}' );
            }
            else
               return false;
            break;
         case REFLECTION_EXPONENT_TOK:
            nextToken();
            if ( parseFloat( f_number ) )
            {
               finish->enableRefExponent( true );
               finish->setReflectionExponent( f_number );
            }
            break;
         case IRID_TOK:
            nextToken();
            parseToken( '{' );
            finish->setIrid( true );
            if( parseFloat( f_number ) )
               finish->setIridAmount( f_number );
            do
            {
               oldConsumed1 = m_consumedTokens;
               switch( m_token )
               {
                  case THICKNESS_TOK:
                     nextToken();
                     if( parseFloat( f_number ) )
                        finish->setIridThickness( f_number );
                     break;
                  case TURBULENCE_TOK:
                     nextToken();
                     if( parseFloat( f_number ) )
                        finish->setIridTurbulence( f_number );
                     break;
                  default:
                     break;
               }
            }
            while( oldConsumed1 != m_consumedTokens );
            parseToken( '}' );
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseDeclare( PMDeclare* decl )
{
   PMObject* child = 0;
   PMTexture* texture = 0;
   bool error = false;

   switch( m_token )
   {
      case OBJECT_TOK:
         error = !parseObject( decl );
         break;
         // finite solid
      case BLOB_TOK:
         child = new PMBlob( m_pPart );
         error = !parseBlob( ( PMBlob* ) child );
         break;
      case BOX_TOK:
         child = new PMBox( m_pPart );
         error = !parseBox( ( PMBox* ) child );
         break;
      case CONE_TOK:
         child = new PMCone( m_pPart );
         error = !parseCone( ( PMCone* ) child );
         break;
      case CYLINDER_TOK:
         child = new PMCylinder( m_pPart );
         error = !parseCylinder( ( PMCylinder* ) child );
         break;
      case HEIGHT_FIELD_TOK:
         child = new PMHeightField( m_pPart );
         error = !parseHeightField( ( PMHeightField* ) child );
         break;
      case JULIA_FRACTAL_TOK:
         child = new PMJuliaFractal( m_pPart );
         error = !parseJuliaFractal( ( PMJuliaFractal* ) child );
         break;
      case LATHE_TOK:
         child = new PMLathe( m_pPart );
         error = !parseLathe( ( PMLathe* ) child );
         break;
      case PRISM_TOK:
         child = new PMPrism( m_pPart );
         error = !parsePrism( ( PMPrism* ) child );
         break;
      case SPHERE_TOK:
         child = new PMSphere( m_pPart );
         error = !parseSphere( ( PMSphere* ) child );
         break;
      case SUPERELLIPSOID_TOK:
         child = new PMSuperquadricEllipsoid( m_pPart );
         error = !parseSqe( ( PMSuperquadricEllipsoid* ) child );
         break;
      case SOR_TOK:
         child = new PMSurfaceOfRevolution( m_pPart );
         error = !parseSor( ( PMSurfaceOfRevolution* ) child );
         break;
      case TEXT_TOK:
         child = new PMText( m_pPart );
         error = !parseText( ( PMText* ) child );
         break;
      case TORUS_TOK:
         child = new PMTorus( m_pPart );
         error = !parseTorus( ( PMTorus* ) child );
         break;
         // finite patch
      case BICUBIC_PATCH_TOK:
         child = new PMBicubicPatch( m_pPart );
         error = !parseBicubicPatch( ( PMBicubicPatch* ) child );
         break;
      case DISC_TOK:
         child = new PMDisc( m_pPart );
         error = !parseDisc( ( PMDisc* ) child );
         break;
      case TRIANGLE_TOK:
      case SMOOTH_TRIANGLE_TOK:
         child = new PMTriangle( m_pPart );
         error = !parseTriangle( ( PMTriangle* ) child );
         break;
         // infinite solid
      case PLANE_TOK:
         child = new PMPlane( m_pPart );
         error = !parsePlane( ( PMPlane* ) child );
         break;
      case QUADRIC_TOK:
      case CUBIC_TOK:
      case QUARTIC_TOK:
      case POLY_TOK:
         child = new PMPolynom( m_pPart );
         error = !parsePolynom( ( PMPolynom* ) child );
         break;
         // csg
      case UNION_TOK:
      case DIFFERENCE_TOK:
      case INTERSECTION_TOK:
      case MERGE_TOK:
         child = new PMCSG( m_pPart );
         error = !parseCSG( ( PMCSG* ) child );
         break;
         // textures
      case TEXTURE_TOK:
         while( m_token == TEXTURE_TOK )
         {
            texture = new PMTexture( m_pPart );
            if( !parseTexture( texture ) )
               error = true;
            if( !insertChild( texture, decl ) )
            {
               delete texture;
               texture = 0;
            }
         }
         break;
      case PIGMENT_TOK:
         child = new PMPigment( m_pPart );
         error = !parsePigment( ( PMPigment* ) child );
         break;
      case NORMAL_TOK:
         child = new PMNormal( m_pPart );
         error = !parseNormal( ( PMNormal* ) child );
         break;
      case FINISH_TOK:
         child = new PMFinish( m_pPart );
         error = !parseFinish( ( PMFinish* ) child );
         break;
      case TEXTURE_MAP_TOK:
         child = new PMTextureMap( m_pPart );
         error = !parseTextureMap( ( PMTextureMap* ) child );
         break;
      case PIGMENT_MAP_TOK:
         child = new PMPigmentMap( m_pPart );
         error = !parsePigmentMap( ( PMPigmentMap* ) child );
         break;
      case COLOR_MAP_TOK:
      case COLOUR_MAP_TOK:
         child = new PMColorMap( m_pPart );
         error = !parseColorMap( ( PMColorMap* ) child );
         break;
      case NORMAL_MAP_TOK:
         child = new PMNormalMap( m_pPart );
         error = !parseNormalMap( ( PMNormalMap* ) child );
         break;
      case SLOPE_MAP_TOK:
         child = new PMSlopeMap( m_pPart );
         error = !parseSlopeMap( ( PMSlopeMap* ) child );
         break;
      case DENSITY_MAP_TOK:
         child = new PMDensityMap( m_pPart );
         error = !parseDensityMap( ( PMDensityMap* ) child );
         break;
      case INTERIOR_TOK:
         child = new PMInterior( m_pPart );
         error = !parseInterior( ( PMInterior* ) child );
         break;
      case MEDIA_TOK:
         child = new PMMedia( m_pPart );
         error = !parseMedia( ( PMMedia* ) child );
         break;
      case DENSITY_TOK:
         child = new PMDensity( m_pPart );
         error = !parseDensity( ( PMDensity* ) child );
         break;
      case MATERIAL_TOK:
         child = new PMMaterial( m_pPart );
         error = !parseMaterial( ( PMMaterial* ) child );
         break;
      case SKY_SPHERE_TOK:
         child = new PMSkySphere( m_pPart );
         error = !parseSkySphere( ( PMSkySphere* ) child );
         break;
      case RAINBOW_TOK:
         child = new PMRainbow( m_pPart );
         error = !parseRainbow( ( PMRainbow* ) child );
         break;
      case FOG_TOK:
         child = new PMFog( m_pPart );
         error = !parseFog( ( PMFog* ) child );
         break;
         // misc
      case LIGHT_SOURCE_TOK:
         child = new PMLight( m_pPart );
         error = !parseLight( ( PMLight* ) child );
         break;
      case ISOSURFACE_TOK:
         child = new PMIsoSurface( m_pPart );
         error = !parseIsoSurface( ( PMIsoSurface* ) child );
         break;
      case PHOTONS_TOK:
         child = new PMPhotons( m_pPart );
         error = !parsePhotons( ( PMPhotons* ) child );
         break;
      case LIGHT_GROUP_TOK:
         child = new PMLightGroup( m_pPart );
         error = !parseLightGroup( ( PMLightGroup* ) child );
         break;
      case INTERIOR_TEXTURE_TOK:
         child = new PMInteriorTexture( m_pPart );
         error = !parseInteriorTexture( ( PMInteriorTexture* ) child );
         break;
      case SPHERE_SWEEP_TOK:
         child = new PMSphereSweep( m_pPart );
         error = !parseSphereSweep( ( PMSphereSweep* ) child );
         break;
      case MESH_TOK:
         child = new PMMesh( m_pPart );
         error = !parseMesh( ( PMMesh* ) child );
         break;
   }

   if( child )
   {
      if( !insertChild( child, decl ) )
      {
         delete child;
         child = 0;
      }
   }
   return !error;
}

bool PMPovrayParser::parseObject( PMCompositeObject* parent )
{
   PMObject* child;
   bool error = false;
   if( !parseToken( OBJECT_TOK, "object" ) )
      return false;

   if( parseToken( '{' ) )
   {
      switch( m_token )
      {
         case ID_TOK:
            child = new PMObjectLink( m_pPart );
            error = !parseObjectLink( ( PMObjectLink* ) child );
            if( !insertChild( child, parent ) )
               delete child;
            break;
         default:
         {
            PMObject* lastChild = 0;
            if( parent )
               lastChild = parent->lastChild();
            else
               lastChild = m_pResultList->last();

            error = !parseChildObjects( parent, 1 );
            if( !error )
            {
               PMObject* newLast = 0;
               if( parent )
                  newLast = parent->lastChild();
               else
                  newLast = m_pResultList->last();

               if( newLast && ( newLast != lastChild ) &&
                   newLast->isA( "CompositeObject" ) )
               {
                  // one child was parsed
                  // append all following objects
                  error = !parseChildObjects( ( PMCompositeObject* ) newLast );
               }
               else
               {
                  printError( ( "One graphical object expected" ) );
                  error = true;
               }
            }
            break;
         }
      }
      if( !parseToken( '}' ))
         error = true;
   }
   else
      error = true;
   return !error;
}

bool PMPovrayParser::parseObjectLink( PMObjectLink* link )
{
   int oldConsumed;

   if( m_token != ID_TOK )
   {
      printExpected( "identifier", m_pScanner->sValue() );
      return false;
   }

   QString id( m_pScanner->sValue() );
   PMDeclare* decl = checkLink( id );
   if( decl )
   {
      if( !link->setLinkedObject( decl ) )
         printError( ( "Wrong declare type" ) );
   }
   nextToken();

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( link );
      parseObjectModifiers( link );
   }
   while( oldConsumed != m_consumedTokens );

   return true;
}

bool PMPovrayParser::parseIsoSurface( PMIsoSurface* iso )
{
   PMVector vector;
   double f;
   int i;
   int oldConsumed;

   if( !parseToken( ISOSURFACE_TOK, "isosurface" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( iso );
      parseObjectModifiers( iso );

      switch( m_token )
      {
         case FUNCTION_TOK:
            nextToken();
            if( m_token != '{' )
            {
               printExpected( '{', m_pScanner->sValue() );
               return false;
            }

            m_pScanner->scanFunction();
            nextToken();
            if( m_token != FUNCTION_TOK )
               return false;
            iso->setFunction( QString( m_pScanner->sValue() ).simplified() );

            nextToken();
            parseToken( '}' );

            break;
         case CONTAINED_BY_TOK:
            nextToken();

            if( !parseToken( '{' ) )
               return false;

            if( m_token == BOX_TOK )
            {
               iso->setContainedBy( PMIsoSurface::Box );
               nextToken();
               parseToken( '{' );
               if( parseVector( vector ) )
                  iso->setCorner1( vector );
               parseToken( ',' );
               if( parseVector( vector ) )
                  iso->setCorner2( vector );
               if( !parseToken( '}' ) )
                  return false;
            }
            else if( m_token == SPHERE_TOK )
            {
               iso->setContainedBy( PMIsoSurface::Sphere );
               nextToken();
               parseToken( '{' );
               if( parseVector( vector ) )
                  iso->setCenter( vector );
               parseToken( ',' );
               if( parseFloat( f ) )
                  iso->setRadius( f );
               if( !parseToken( '}' ) )
                  return false;
            }
            else
            {
               printUnexpected( m_pScanner->sValue() );
               return false;
            }

            if( !parseToken( '}' ) )
               return false;
            break;
         case THRESHOLD_TOK:
            nextToken();
            if( parseFloat( f ) )
               iso->setThreshold( f );
            break;
         case ACCURACY_TOK:
            nextToken();
            if( parseFloat( f ) )
               iso->setAccuracy( f );
            break;
         case MAX_GRADIENT_TOK:
            nextToken();
            if( parseFloat( f ) )
               iso->setMaxGradient( f );
            break;
         case EVALUATE_TOK:
            nextToken();
            iso->setEvaluate( true );
            if( parseFloat( f ) )
            {
               iso->setEvaluateValue( 0, f );
               if( parseToken( ',' ) && parseFloat( f ) )
               {
                  iso->setEvaluateValue( 1, f );
                  if( parseToken( ',' ) && parseFloat( f ) )
                     iso->setEvaluateValue( 2, f );
               }
            }
            break;
         case OPEN_TOK:
            nextToken();
            iso->setOpen( true );
            break;
         case MAX_TRACE_TOK:
            nextToken();
            if( parseInt( i ) )
               iso->setMaxTrace( i );
            break;
         case ALL_INTERSECTIONS_TOK:
            nextToken();
            iso->setAllIntersections( true );
            break;
         default:
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseRadiosity( PMRadiosity* rad )
{
   double f;
   int i;
   int oldConsumed;


   if( !parseToken( RADIOSITY_TOK, "radiosity" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
         case ADC_BAILOUT_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setAdcBailout( f );
            break;
         case ALWAYS_SAMPLE_TOK:
            nextToken();
            rad->setAlwaysSample( parseBool() );
            break;
         case BRIGHTNESS_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setBrightness( f );
            break;
         case COUNT_TOK:
            nextToken();
            if( parseInt( i ) )
               rad->setCount( i );
            break;
         case ERROR_BOUND_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setErrorBound( f );
            break;
         case GRAY_THRESHOLD_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setGrayThreshold( f );
            break;
         case LOW_ERROR_FACTOR_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setLowErrorFactor( f );
            break;
         case MAX_SAMPLE_TOK:
            nextToken();
            if ( parseFloat( f ) )
               rad->setMaxSample( f );
            break;
         case MEDIA_TOK:
            nextToken();
            rad->setMedia( parseBool() );
            break;
         case MINIMUM_REUSE_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setMinimumReuse( f );
            break;
         case NEAREST_COUNT_TOK:
            nextToken();
            if( parseInt( i ) )
               rad->setNearestCount( i );
            break;
         case NORMAL_TOK:
            nextToken();
            rad->setNormal( parseBool() );
            break;
         case PRETRACE_START_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setPretraceStart( f );
            break;
         case PRETRACE_END_TOK:
            nextToken();
            if( parseFloat( f ) )
               rad->setPretraceEnd( f );
            break;
         case RECURSION_LIMIT_TOK:
            nextToken();
            if( parseInt( i) )
               rad->setRecursionLimit( i );
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseGlobalPhotons( PMGlobalPhotons* gp )
{
   double f;
   int i;
   int oldConsumed;


   if( !parseToken( PHOTONS_TOK, "photons" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
         case SPACING_TOK:
            gp->setNumberType( PMGlobalPhotons::Spacing );
            nextToken();
            if ( parseFloat( f ) )
               gp->setSpacing( f );
            break;
         case COUNT_TOK:
            gp->setNumberType( PMGlobalPhotons::Count );
            nextToken();
            if ( parseInt( i ) )
               gp->setCount( i );
            break;
         case GATHER_TOK:
            nextToken();
            if ( parseInt( i ) )
            {
               gp->setGatherMin( i );
               if ( parseToken( ',' ) && parseInt( i ) )
                  gp->setGatherMax( i );
            }
            break;
         case MEDIA_TOK:
            nextToken();
            if ( parseInt( i ) )
            {
               gp->setMediaMaxSteps( i );
               if ( parseToken( ',' ) && parseFloat( f ) )
                  gp->setMediaFactor( f );
            }
         case JITTER_TOK:
            nextToken();
            if ( parseFloat( f ) )
               gp->setJitter( f );
            break;
         case MAX_TRACE_LEVEL_TOK:
            nextToken();
            gp->setMaxTraceLevelGlobal( false );
            if ( parseInt( i ) )
               gp->setMaxTraceLevel( i );
            break;
         case ADC_BAILOUT_TOK:
            nextToken();
            gp->setAdcBailoutGlobal( false );
            if ( parseFloat( f ) )
               gp->setAdcBailout( f );
            break;
         case AUTOSTOP_TOK:
            nextToken();
            if ( parseFloat( f ) )
               gp->setAutostop( f );
            break;
         case EXPAND_THRESHOLDS_TOK:
            nextToken();
            if ( parseFloat( f ) )
            {
               gp->setExpandIncrease( f );
               if ( parseToken( ',' ) && parseInt( i ) )
                  gp->setExpandMin( i );
            }
            break;
         case RADIUS_TOK:
            nextToken();
            if ( parseFloat( f ) )
            {
               gp->setRadiusGather( f );
               if ( parseToken( ',' ) && parseFloat( f ) )
               {
                  gp->setRadiusGatherMulti( f );
                  if ( parseToken( ',' ) && parseFloat( f ) )
                  {
                     gp->setRadiusMedia( f );
                     if ( parseToken( ',' ) && parseFloat( f ) )
                        gp->setRadiusMediaMulti( f );
                  }
               }
            }
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parsePhotons( PMPhotons* p )
{
   double f;
   int oldConsumed;

   if( !parseToken( PHOTONS_TOK, "photons" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   p->setTarget( false );

   do
   {
      oldConsumed = m_consumedTokens;
      switch( m_token )
      {
         case TARGET_TOK:
            nextToken();
            p->setTarget( true );
            if ( parseFloat( f ) )
               p->setSpacingMulti( f );
            break;
         case REFRACTION_TOK:
            nextToken();
            p->setRefraction( parseBool() );
            break;
         case REFLECTION_TOK:
            nextToken();
            p->setReflection( parseBool() );
            break;
         case COLLECT_TOK:
            nextToken();
            p->setCollect( parseBool() );
            break;
         case PASS_THROUGH_TOK:
            nextToken();
            p->setPassThrough( parseBool() );
            break;
         case AREA_LIGHT_TOK:
            nextToken();
            p->setAreaLight( parseBool() );
            break;
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseLightGroup( PMLightGroup* lg )
{
   int oldConsumed;

   if ( !parseToken( LIGHT_GROUP_TOK, "light_group" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;
      if ( m_token == GLOBAL_LIGHTS_TOK )
      {
         nextToken();
         lg->setGlobalLights( parseBool() );
      }
      else
      {
         parseChildObjects( lg );
         parseObjectModifiers( lg );
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseInteriorTexture( PMInteriorTexture* it )
{
   int oldConsumed;

   if( !parseToken( INTERIOR_TEXTURE_TOK, "interior_texture" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;
      parseChildObjects( it );
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseSphereSweep( PMSphereSweep* ss )
{
   int oldConsumed, numspheres;
   QList<PMVector> points;
   QList<double> radii;
   PMVector point;
   double f;

   if( !parseToken( SPHERE_SWEEP_TOK, "sphere_sweep" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   nextToken();
   switch ( m_token )
   {
      case LINEAR_SPLINE_TOK:
         ss->setSplineType( PMSphereSweep::LinearSpline );
         break;
      case B_SPLINE_TOK:
         ss->setSplineType( PMSphereSweep::BSpline );
         break;
      case CUBIC_SPLINE_TOK:
         ss->setSplineType( PMSphereSweep::CubicSpline );
         break;
      default:
         return false;
   }

   if ( !parseInt( numspheres ) )
      return false;

   for ( int i = 0; i < numspheres; ++i )
   {
      if ( !parseVector( point ) )
         return false;
      points.append( point );
      if ( !parseToken( ',' ) )
         return false;
      if ( !parseFloat( f ) )
         return false;
      radii.append( f );
   }

   ss->setPoints( points );
   ss->setRadii( radii );

   do
   {
      oldConsumed = m_consumedTokens;
      if ( m_token == TOLERANCE_TOK )
      {
         nextToken();
         if ( !parseFloat( f ) )
            return false;
         ss->setTolerance( f );
      }
      else
      {
         parseObjectModifiers( ss );
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}

bool PMPovrayParser::parseMesh( PMMesh* m )
{
   int oldConsumed;
   PMVector vector;

   if( !parseToken( MESH_TOK, "mesh" ) )
      return false;
   if( !parseToken( '{' ) )
      return false;

   do
   {
      oldConsumed = m_consumedTokens;
      if ( m_token == HIERARCHY_TOK )
      {
         nextToken();
         m->setHierarchy( parseBool() );
      }
      else if ( m_token == INSIDE_VECTOR_TOK )
      {
         nextToken();
         if ( !parseVector( vector ) )
            return false;
         m->enableInsideVector( true );
         m->setInsideVector( vector );
      }
      else
      {
         parseChildObjects( m );
         parseObjectModifiers( m );
      }
   }
   while( oldConsumed != m_consumedTokens );

   if( !parseToken( '}' ) )
      return false;

   return true;
}
