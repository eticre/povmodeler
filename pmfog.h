//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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


#ifndef PMFOG_H
#define PMFOG_H



#include "pmtexturebase.h"
#include "pmcolor.h"
#include "pmvector.h"

/**
 * Class for povray fogs
 */
class PMFog : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates an PMFog
    */
   PMFog( PMPart* part );
   /**
    * Copy constructor
    */
   PMFog( const PMFog& f );
   /**
    * Deletes the object
    */
   virtual ~PMFog();

   /** */
   virtual PMObject* copy() const { return new PMFog( *this ); }
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
    * Returns a new @ref PMFogEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual QString pixmap() const { return QString( "pmfog" ); }

   /** */
   virtual void restoreMemento( PMMemento* s );

   int fogType() const { return m_fogType; }
   double distance() const { return m_distance; }
   PMColor color() const { return m_color; }
   bool isTurbulenceEnabled() const { return m_enableTurbulence; }
   PMVector valueVector() const { return m_valueVector; }
   int octaves() const { return m_octaves; }
   double omega() const { return m_omega; }
   double lambda() const { return m_lambda; }
   double depth() const { return m_depth; }
   double fogOffset() const { return m_fogOffset; }
   double fogAlt() const { return m_fogAlt; }
   PMVector up() const { return m_up; }

   void setFogType( int c );
   void setDistance( double c );
   void setColor( const PMColor& c );
   void enableTurbulence( bool c );
   void setValueVector( const PMVector& v );
   void setOctaves( int c );
   void setOmega( double c );
   void setLambda( double c );
   void setDepth( double c );
   void setFogOffset( double c );
   void setFogAlt( double c );
   void setUp( const PMVector& v );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMFogMementoID { PMFogTypeID, PMDistanceID, PMColorID,
                         PMEnableTurbulenceID, PMValueVectorID,
                         PMOctavesID, PMOmegaID, PMLambdaID,
                         PMDepthID, PMFogOffsetID, PMFogAltID,
                         PMUpID };

   int m_fogType;
   double m_distance;
   PMColor m_color;
   bool m_enableTurbulence;
   PMVector m_valueVector;
   int m_octaves;
   double m_omega;
   double m_lambda;
   double m_depth;
   double m_fogOffset;
   double m_fogAlt;
   PMVector m_up;

   static PMMetaObject* s_pMetaObject;
};


#endif
