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

#ifndef PMVARIANT_H
#define PMVARIANT_H

#include <QString>
#include <QDataStream>
#include "pmcolor.h"
#include "pmvector.h"
#include "pmthreestate.h"

class PMObject;

/**
 * Variant class for PovModeler.
 *
 * Can store and convert: int, unsigned int, double, bool,
 * PMThreeState, QString, PMVector, PMColor, PMObject*.
 *
 * Enums are stored as QString.
 *
 * The variant can store one type at a time. You can try to convert
 * the type with the convertTo* methods. These will return bool on success.
 * If the variant could not be converted, the old type and value
 * did not change.
 */

class PMVariant
{
public:
   /**
    * Type of stored data
    */
   enum PMVariantDataType { Integer, Unsigned, Double, Bool,
                            ThreeState, String, Vector, Color,
                            ObjectPointer, None };
   /**
    * Creates an empty variant object
    */
   PMVariant();
   /**
    * Stores an integer
    */
   PMVariant( int data );
   /**
    * Stores an unsigned integer
    */
   PMVariant( unsigned int data );
   /**
    * Stores a double
    */
   PMVariant( double data );
   /**
    * Stores a boolean
    */
   PMVariant( bool data );
   /**
    * Stores a @ref PMThreeState
    */
   PMVariant( PMThreeState data );
   /**
    * Stores a string
    */
   PMVariant( const QString& data );
   /**
    * Stores a @ref PMVector
    */
   PMVariant( const PMVector& data );
   /**
    * Stores a @ref PMColor
    */
   PMVariant( const PMColor& data );
   /**
    * Stores a pointer to a PMObject
    */
   PMVariant( PMObject* obj );
   /**
    * Copy constructor
    */
   PMVariant( const PMVariant& v );
   /**
    * Deletes the variant
    */
   ~PMVariant();

   /**
    * Assignment operator
    */
   PMVariant& operator= ( const PMVariant& v );

   /**
    * Returns true if no data is stored
    */
   bool isNull() const { return !m_pData; }

   /**
    * Sets the integer value
    */
   void setInt( int data );
   /**
    * Sets the unsigned value
    */
   void setUnsigned( unsigned int data );
   /**
    * Sets the double value
    */
   void setDouble( double data );
   /**
    * Sets the boolean value
    */
   void setBool( bool data );
   /**
    * Sets the PMThreeState value
    */
   void setThreeState( PMThreeState data );
   /**
    * Sets the string data
    */
   void setString( const QString& data );
   /**
    * Sets the vector data
    */
   void setVector( const PMVector& data );
   /**
    * Sets the color data
    */
   void setColor( const PMColor& data );
   /**
    * Sets the object pointer
    */
   void setObject( PMObject* o );

   /**
    * Returns the integer value. Data type has to be Integer!
    */
   int intData() const;
   /**
    * Returns the unsigned value. Data type has to be Unsigned!
    */
   int unsignedData() const;
   /**
    * Returns the double value. Data type has to be Double!
    */
   double doubleData() const;
   /**
    * Returns the boolean value. Data type has to be Bool!
    */
   bool boolData() const;
   /**
    * Returns the PMThreeState value. Data type has to be ThreeState!
    */
   PMThreeState threeStateData() const;
   /**
    * Returns the string data. Data type has to be String!
    */
   QString stringData() const;
   /**
    * Returns the vector data. Data type has to be Vector!
    */
   PMVector vectorData() const;
   /**
    * Returns the color data. Data type has to be Color!
    */
   PMColor colorData() const;
   /**
    * Returns the object pointer. Data type has to be ObjectPointer!
    */
   PMObject* objectData() const;

   /**
    * Converts the variant to an integer. Returns true if possible
    */
   bool convertTo( PMVariantDataType t );

   /**
    * Returns the type of the stored data
    */
   PMVariantDataType dataType() const { return m_dataType; }

   /**
    * Returns the value of the stored data in string format
    */
   QString asString() const;
   /**
    * Sets the value of the variant based on the string
    */
   bool fromString( const PMVariant::PMVariantDataType t, const QString& value );
private:
   void clear();

   /**
    * a pointer to the stored data
    */
   void* m_pData;
   /**
    * Type of the data
    */
   PMVariantDataType m_dataType;
};

// Streaming operators for PMVariant
QDataStream& operator<<( QDataStream& stream, const PMVariant& value );
QDataStream& operator>>( QDataStream& stream, PMVariant& value );


#endif
