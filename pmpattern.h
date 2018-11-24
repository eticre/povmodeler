//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMPATTERN_H
#define PMPATTERN_H

#include "pmobject.h"
#include "pmvector.h"
#include "pmcolor.h"
#include "pmpigment.h"
#include "pmdeclare.h"

#include <QList>

/**
 * Class for povray patterns.
 */
class PMPattern : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * The type of the pattern
    */
   enum PMPatternType { PatternAgate, PatternAverage, PatternBoxed,
                        PatternBozo, PatternBumps, PatternCells,
                        PatternCrackle, PatternCylindrical, PatternDensity,
                        PatternDents, PatternGradient, PatternGranite,
                        PatternJulia, PatternLeopard, PatternMandel,
                        PatternMarble, PatternOnion, PatternPlanar,
                        PatternQuilted, PatternRadial, PatternRipples,
                        PatternSlope, PatternSpherical, PatternSpiral1,
                        PatternSpiral2, PatternSpotted ,PatternWaves,
                        PatternWood, PatternWrinkles, PatternTiling, PatternPigment };
   /**
    * The noise generators
    */
   enum PMNoiseType { GlobalSetting, Original, RangeCorrected, Perlin };
   /**
    * Creates a PMPattern
    */
   PMPattern( PMPart* part );
   /**
    * Copy constructor
    */
   PMPattern( const PMPattern& p );
   /**
    * deletes the PMPattern
    */
   virtual ~PMPattern();

   /** */
   virtual PMObject* copy() const { return new PMPattern( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual QString pixmap() const { return QString( "pmpattern" ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMPatternEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;

   /**
    * Gets the pattern type
    */
   PMPatternType patternType() const { return m_patternType; }
   /**
    * Gets the agate Turbulence*/
   double agateTurbulence() const { return m_agateTurbulence; }
   /*
    * Gets the crackle form
    */
   PMVector crackleForm() const { return m_crackleForm; }
   /**
    * Gets the crackle metric
    */
   int crackleMetric() const { return m_crackleMetric; }
   /**
    * Gets the crackle offset
    */
   double crackleOffset() const { return m_crackleOffset; }
   /**
    * Gets the crackle solid flag
    */
   bool crackleSolid() const { return m_crackleSolid; }
   /**
    * Gets the desity file name
    */
   QString densityFile() const { return m_densityFile; }
   /**
    * Gets the density interpolation
    */
   int densityInterpolate() const { return m_densityInterpolate; }
   /**
    * Gets the gradint vector
    */
   PMVector gradient() const { return m_gradient; }
   /**
    * Returns the Complex number for Julia Fractals
    */
   PMVector juliaComplex() const { return m_juliaComplex; }
   /**
    * Returns true if where using tha magnet types of fractals
    */
   bool fractalMagnet() const { return m_fractalMagnet; }
   /**
    * Returns the magnet type of a fractal
    */
   int fractalMagnetType() const { return m_fractalMagnetType; }
   /**
    * Gets the number of Iterations for fractals
    */
   int maxIterations() const { return m_maxIterations; }
   /**
    * Returns the exponent of a fractal
    */
   int fractalExponent() const { return m_fractalExponent; }
   /**
    * Returns the exterior type of a fractal
    */
   int fractalExtType() const { return m_fractalExtType; }
   /**
    * Return the exterior factor of a fractal
    */
   double fractalExtFactor() const { return m_fractalExtFactor; }
   /**
    * Returns the interior type of a fractal
    */
   int fractalIntType() const { return m_fractalIntType; }
   /**
    * Returns the interior factor of a fractal
    */
   double fractalIntFactor() const { return m_fractalIntFactor; }
   /**
    * Gets control0 for Quilt
    */
   double quiltControl0() const { return m_quiltControl0; }
   /**
    * Gets control1 for Quilt
    */
   double quiltControl1() const { return m_quiltControl1; }
   /**
    * Returns the slope direction
    */
   PMVector slopeDirection() const { return m_slopeDirection; }
   PMVector slopePointAt() const { return m_slopePointAt; }
   bool slopePointAtFlag() const { return m_slopePointAtFlag; }
   /**
    * Returns the low slope value
    */
   double slopeLoSlope() const { return m_slopeLoSlope; }
   /**
    * Returns the high slope value
    */
   double slopeHiSlope() const { return m_slopeHiSlope; }
   /**
    * Returns the slopes altitude flag
    */
   bool slopeAltFlag() const { return m_slopeAltFlag; }
   /**
    * Returns the altitude for the slope
    */
   PMVector slopeAltitude() const { return m_slopeAltitude; }
   /**
    * Returns the low altitude for slope
    */
   double slopeLoAltitude() const { return m_slopeLoAlt; }
   /**
    * Returns the high altitude for slope
    */
   double slopeHiAltitude() const { return m_slopeHiAlt; }
   /**
    * Gets the number of arms in spiral types 1 and 2
    */
   int spiralNumberArms() const { return m_spiralNumberArms; }
   /**
    * Gets the tiling number -> pmpovray35serialization.cpp
    */
   int tilingNumbers() const { return m_tilingNumbers; }
   /**
    * Returns the noise generator
    */
   PMNoiseType noiseGenerator() const { return m_noiseGenerator; }
   /**
    * Returns whether turbulence is enabled
    */
   bool isTurbulenceEnabled() const { return m_enableTurbulence; }
   /**
    * Gets the Turbulence vector
    */
   PMVector valueVector() const { return m_valueVector; }
   /**
    * Gets the turbulence octaves
    */
   int octaves() const { return m_octaves; }
   /**
    * Gets the turbulence omega
    */
   double omega() const { return m_omega; }
   /**
    * Gets the turbulence lambda
    */
   double lambda() const { return m_lambda; }
   /**
    * Returns depth (only has meaning inside a normal)
    */
   double depth() const { return m_depth; }

   /**
    * Sets the pattern type
    */
   void setPatternType( PMPatternType l );
   /**
    * Sets agate turbulence
    */
   void setAgateTurbulence( double c );
   /**
    * Sets the crackle form
    */
   void setCrackleForm( const PMVector& v );
   /**
    * Sets the crackle metric
    */
   void setCrackleMetric( int c );
   /**
    * Sets the crackle offset
    */
   void setCrackleOffset( double c );
   /**
    * Sets the crackle solid flag
    */
   void setCrackleSolid( bool c );
   /**
    * Sets the Density File Name
    */
   void setDensityFile( const QString& v );
   /**
    * Sets the density interpolation
    */
   void setDensityInterpolate( int c );
   /**
    * sets gradient vector
    */
   void setGradient( const PMVector& v );
   /**
    * Sets the complex number for Julia
    */
   void setJuliaComplex( const PMVector& v );
   /**
    * Sets if where using magnet types of fractals
    */
   void setFractalMagnet( bool c );
   /**
    * Sets the magnet type of a fractal
    */
   void setFractalMagnetType( int c );
   /**
    * Sets the maximum iterations for fractals
    */
   void setMaxIterations( int c );
   /**
    * Sets the exponent for fractals
    */
   void setFractalExponent( int c );
   /**
    * Sets the exterior type of fractals
    */
   void setFractalExtType( int c );
   /**
    * Sets the exterior factor of fractals
    */
   void setFractalExtFactor( double c );
   /**
    * Sets the interior type of fractals
    */
   void setFractalIntType( int c );
   /**
    * Sets the interior factor of fractals
    */
   void setFractalIntFactor( double c );
   /**
    * Sets quilt control 0
    */
   void setQuiltControl0( double c );
   /**
    * Sets quite control 1
    */
   void setQuiltControl1( double c );
   /**
    * Sets the slope direction
    */
   void setSlopeDirection( const PMVector& v );
   /**
    * Sets the point_at
    */
   void setSlopePointAt( const PMVector& v );
   void setSlopePointAtFlag( bool c );
   /**
    * Sets the low slope value
    */
   void setSlopeLoSlope( double c );
   /**
    * Sets the high slope value
    */
   void setSlopeHiSlope( double c );
   /**
    * Sets the slopes altitude flag
    */
   void setSlopeAltFlag( bool c );
   /**
    * Sets the slopes alititude direction
    */
   void setSlopeAltitude( const PMVector& v );
   /**
    * Sets the slopes low altitude
    */
   void setSlopeLoAlt( double c );
   /**
    * Sets the slopes high altitude
    */
   void setSlopeHiAlt( double c );
   /**
    * Sets the number of spiral arms
    */
   void setSpiralNumberArms( int c );
   /**
    * Sets the tiling numbers
    */
   void setTilingNumbers( int c );
   /**
    * Sets the pigment-pattern as linked object
    */
   void setPigmentPattern( PMDeclare* o );
   /**
    * Get the linked object as pigment-pattern
    */
   virtual PMDeclare* linkedObject() const { return m_pLinkedObject; }
   /** */
   /**
    * Sets noise generator
    */
   void setNoiseGenerator( PMNoiseType c );
   /**
    * Enables turbulence
    */
   void enableTurbulence( bool c );
   /**
    * sets the turbulence vector
    */
   void setValueVector( const PMVector& v );
   /**
    * Sets the turbulence octaves
    */
   void setOctaves( int c );
   /**
    * Sets the turblence omega
    */
   void setOmega( double c );
   /**
    * Sets the turbulence Lambda
    */
   void setLambda( double c );
   /**
    * Sets the depth value (only inside normals)
    */
   void setDepth( double c );

   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMPatternMementoID {
       PMPatternTypeID, PMAgateTurbulenceID, PMCrackleFormID,
       PMCrackleMetricID, PMCrackleOffsetID, PMCrackleSolidID,
       PMDensityFileID, PMDensityInterpolateID, PMGradientID,
       PMJuliaComplexID, PMFractalMagnetID, PMFractalMagnetTypeID,
       PMMaxIterationsID, PMFractalExponentID, PMFractalExtTypeID,
       PMFractalExtFactorID, PMFractalIntTypeID, PMFractalIntFactorID,
       PMQuiltControl0ID, PMQuiltControl1ID, PMSlopeDirectionID,
       PMSlopePointAtID,PMSlopePointAtFlagID,
       PMSlopeLoSlopeID, PMSlopeHiSlopeID, PMSlopeAltFlagID,
       PMSlopeAltitudeID, PMSlopeLoAltID, PMSlopeHiAltID,
       PMSpiralNumberArmsID, PMNoiseGeneratorID, PMEnableTurbulenceID,
       PMValueVectorID, PMOctavesID, PMOmegaID, PMLambdaID, PMDepthID,
       PMTilingNumbersID, PMPigmentPatternID };
   /**
    * Pattern type
    */
   PMPatternType m_patternType;

   // Extra values for Individual Patterns
   // Agate
   double m_agateTurbulence;
   // crackle
   PMVector m_crackleForm;
   int m_crackleMetric;
   double m_crackleOffset;
   bool m_crackleSolid;
   // density
   QString m_densityFile;
   int m_densityInterpolate;
   // gradient
   PMVector m_gradient;
   // Julia
   PMVector m_juliaComplex;
   // Fractal patterns Mandel and Julia
   bool m_fractalMagnet;
   int m_fractalMagnetType;
   int m_maxIterations;
   int m_fractalExponent;
   int m_fractalExtType;
   double m_fractalExtFactor;
   int m_fractalIntType;
   double m_fractalIntFactor;
   // quilt
   double m_quiltControl0;
   double m_quiltControl1;
   // slope
   PMVector m_slopeDirection;
   PMVector m_slopePointAt;
   bool m_slopePointAtFlag;
   double m_slopeLoSlope;
   double m_slopeHiSlope;
   bool m_slopeAltFlag;
   PMVector m_slopeAltitude;
   double m_slopeLoAlt;
   double m_slopeHiAlt;
   // Spiral
   int m_spiralNumberArms;
   // Tiling
   int m_tilingNumbers;
   // pigment_pattern
   PMDeclare* m_pLinkedObject;
   // noise generator type for Bozo, Bumps, Granite and Wrinkles.
   PMNoiseType m_noiseGenerator;

   // Turbulence available for all patterns
   bool m_enableTurbulence;
   PMVector m_valueVector;
   int m_octaves;
   double m_omega;
   double m_lambda;
   // depth when used with Normal
   double m_depth;
   static PMMetaObject* s_pMetaObject;
};

#endif
