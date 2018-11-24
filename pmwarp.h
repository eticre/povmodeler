//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Luis Carvalho
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


#ifndef PMWARP_H
#define PMWARP_H



#include "pmobject.h"

/**
 * Class for Repeat Warps
 */

class PMWarp : public PMObject
{
   typedef PMObject Base;
public:
   enum PMWarpType { Repeat, BlackHole, Turbulence,
                     Cylindrical, Spherical, Toroidal, Planar };

   /**
    * Creates a PMWarp
    */
   PMWarp( PMPart* part );
   /**
    * Copy constructor
    */
   PMWarp( const PMWarp& w );
   /**
    * deletes the PMWarp
    */
   virtual ~PMWarp();

   /** */
   virtual PMObject* copy() const { return new PMWarp( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMWarpEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmwarp" ); }

   PMWarpType warpType() const { return m_warpType; }
   void setWarpType( PMWarpType c );

   PMVector direction() const { return m_direction; }
   void setDirection( const PMVector& c );
   PMVector offset() const { return m_offset; }
   void setOffset( const PMVector& c );
   PMVector flip() const { return m_flip; }
   void setFlip( const PMVector& c );

   PMVector location() const { return m_location; }
   void setLocation( const PMVector& v );
   double radius() const { return m_radius; }
   void setRadius( double c );
   double strength() const { return m_strength; }
   void setStrength( double c );
   double falloff() const { return m_falloff; }
   void setFalloff( double c );
   bool inverse() const { return m_inverse; }
   void setInverse( bool c );
   PMVector repeat() const { return m_repeat; }
   void setRepeat( const PMVector& v );
   PMVector turbulence() const { return m_turbulence; }
   void setTurbulence( const PMVector& v );

   PMVector valueVector() const { return m_valueVector; }
   void setValueVector( const PMVector& v );
   int octaves() const { return m_octaves; }
   void setOctaves( int c );
   double omega() const { return m_omega; }
   void setOmega( double c );
   double lambda() const { return m_lambda; }
   void setLambda( double c );

   PMVector orientation() const { return m_orientation;}
   void setOrientation( const PMVector& v );
   double distExp() const { return m_distExp; }
   void setDistExp( double c );
   double majorRadius() const { return m_majorRadius; }
   void setMajorRadius( double c );


   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMWarpMementoID { PMWarpTypeID, PMDirectionID, PMOffsetID, PMFlipID,
                          PMLocationID, PMRadiusID, PMStrengthID, PMFalloffID,
                          PMInverseID, PMRepeatID, PMTurbulenceID,
                          PMValueVectorID, PMOctavesID, PMOmegaID, PMLambdaID,
                          PMOrientationID, PMDistExpID, PMMajorRadiusID };

   PMWarpType m_warpType;

   // Repeat variables
   PMVector m_direction;
   PMVector m_offset;
   PMVector m_flip;

   // Black Hole variables
   PMVector m_location;
   double m_radius;
   double m_strength;
   double m_falloff;
   bool m_inverse;
   PMVector m_repeat;
   PMVector m_turbulence;

   // Turbulence variables
   PMVector m_valueVector;
   int m_octaves;
   double m_omega;
   double m_lambda;

   // Mapping variables
   PMVector m_orientation;
   double m_distExp;
   double m_majorRadius;

   static PMMetaObject* s_pMetaObject;
};

#endif
