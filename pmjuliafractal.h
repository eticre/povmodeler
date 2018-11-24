//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMJULIAFRACTAL_H
#define PMJULIAFRACTAL_H



#include "pmsolidobject.h"
#include "pmvector.h"


/**
 * Class for povray julia fractals.
 */

class PMJuliaFractal : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   enum AlgebraType { Quaternion, Hypercomplex };
   enum FunctionType { FTsqr, FTcube, FTexp, FTreciprocal, FTsin, FTasin,
                       FTsinh, FTasinh, FTcos, FTacos, FTcosh, FTacosh,
                       FTtan, FTatan, FTtanh, FTatanh, FTlog, FTpwr };

   /**
    * Creates an empty PMJuliaFractal
    */
   PMJuliaFractal( PMPart* part );
   /**
    * Copy constructor
    */
   PMJuliaFractal( const PMJuliaFractal& f );
   /**
    * deletes the PMJuliaFractal
    */
   virtual ~PMJuliaFractal();

   /** */
   virtual PMObject* copy() const { return new PMJuliaFractal( *this ); }
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
    * Returns a new @ref PMJuliaFractalEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmjuliafractal" ); }
   /** */
   virtual void restoreMemento( PMMemento* s );

   /**
    * Returns the 4d julia parameter vector
    */
   PMVector juliaParameter() const { return m_juliaParameter; }
   /**
    * Sets the julia parameter vector
    */
   void setJuliaParameter( const PMVector& p );
   /**
    * Returns the algebra type
    */
   AlgebraType algebraType() const { return m_algebraType; }
   /**
    * Sets the algebra type
    */
   void setAlgebraType( AlgebraType t );
   /**
    * Returns the function type
    */
   FunctionType functionType() const { return m_functionType; }
   /**
    * Sets the function type
    */
   void setFunctionType( FunctionType t );
   /**
    * Returns the maximum number of iterations
    */
   int maximumIterations() const { return m_maxIterations; }
   /**
    * Sets the maximum number of iterations
    */
   void setMaximumIterations( int m );
   /**
    * Returns the precision
    */
   double precision() const { return m_precision; }
   /**
    * Sets the precision
    */
   void setPrecision( double p );
   /**
    * Returns the slice normal (4D vector)
    */
   PMVector sliceNormal() const { return m_sliceNormal; }
   /**
    * Sets the slice normal (4D vector)
    */
   void setSliceNormal( const PMVector& v );
   /**
    * Returns the slice distance
    */
   double sliceDistance() const { return m_sliceDistance; }
   /**
    * Sets the slice distance
    */
   void setSliceDistance( double d );
   /**
    * Returns the exponent for the pow function type (2D vector)
    */
   PMVector exponent() const { return m_exponent; }
   /**
    * Sets the exponent for the pow function type (2D Vector)
    */
   void setExponent( const PMVector& p );

   static QString functionTypeToString( FunctionType t );
   static FunctionType stringToFunctionType( const QString& str );
   static QString algebraTypeToString( AlgebraType t );
   static AlgebraType stringToAlgebraType( const QString& str );
private:

   /**
    * IDs for @ref PMMementoData
    */
   enum PMJuliaFractalMementoID { PMJuliaParameterID, PMAlgebraTypeID,
                                  PMFunctionTypeID, PMMaxIterationsID,
                                  PMPrecisionID, PMSliceNormalID,
                                  PMSliceDistanceID, PMExponentID };
   PMVector m_juliaParameter;
   AlgebraType m_algebraType;
   FunctionType m_functionType;
   int m_maxIterations;
   double m_precision;
   PMVector m_sliceNormal;
   double m_sliceDistance;
   PMVector m_exponent;

   static PMMetaObject* s_pMetaObject;
};

#endif
