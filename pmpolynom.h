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


#ifndef PMPOLYNOM_H
#define PMPOLYNOM_H



#include "pmsolidobject.h"
#include "pmvector.h"

/**
 * Class for povray polynoms.
 */

class PMPolynom : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Creates an empty PMPolynom
    */
   PMPolynom( PMPart* part );
   /**
    * Copy constructor
    */
   PMPolynom( const PMPolynom& p );
   /**
    * deletes the PMPolynom
    */
   virtual ~PMPolynom();

   /** */
   virtual PMObject* copy() const { return new PMPolynom( *this ); }
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
   /** */
   virtual QString pixmap() const { return QString( "pmpolynom" ); }
   /**
    * Returns a new @ref PMPolynomEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /** */
   virtual void restoreMemento( PMMemento* s );

   /** */
   int polynomOrder() const { return m_order; }
   /**
    * Sets the polynom order
    */
   void setPolynomOrder( int order );
   /**
    * Returns the coefficients.
    */
   PMVector coefficients() const { return m_coefficients; }
   /**
    * Sets the coefficients.
    */
   void setCoefficients( const PMVector& p );
   /**
    * Returns the sturm flag
    */
   bool sturm() const { return m_sturm; }
   /**
    * Sets the sturm flag
    */
   void setSturm( bool s );
private:
   enum PMPolynomMementoID { PMOrderID, PMCoefficientsID, PMSturmID };
   int m_order;
   PMVector m_coefficients;
   bool m_sturm;

   static PMMetaObject* s_pMetaObject;
};

#endif
