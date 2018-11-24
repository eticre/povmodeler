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


#include "pmpolynom.h"
#include "pmpolynomedit.h"

#include "pmxmlhelper.h"
#include "pmmemento.h"



const double c_defaultCoefficients[10] =
{  // Hyperboloid_Y
   1.0,  0.0, 0.0,
   0.0, -1.0, 0.0,
   0.0, 1.0, 0.0,
   -1.0
};
const int c_defaultOrder = 2;
const bool c_defaultSturm = true;
const int c_polynomSize[8] = { 0, 0, 10, 20, 35, 56, 84, 120 };

PMDefinePropertyClass( PMPolynom, PMPolynomProperty );

class PMCoefficientProperty : public PMPropertyBase
{
public:
   PMCoefficientProperty()
         : PMPropertyBase( "coefficients", PMVariant::Double )
   {
      m_index = 0;
   }
   virtual int dimensions() const { return 1; }
   virtual void setIndex( int /*dimension*/, int index )
   {
      m_index = index;
   }
   virtual int size( PMObject* object, int /*dimension*/ ) const
   {
      return c_polynomSize[ ( ( PMPolynom* ) object )->polynomOrder() ];
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& var )
   {
      PMPolynom* p = ( PMPolynom* ) obj;
      PMVector v = p->coefficients();
      v[m_index] = var.doubleData();
      p->setCoefficients( v );

      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      PMPolynom* p = ( PMPolynom* ) obj;
      return PMVariant( p->coefficients()[m_index] );
   }

private:
   int m_index;
};

PMMetaObject* PMPolynom::s_pMetaObject = 0;
PMObject* createNewPolynom( PMPart* part )
{
   return new PMPolynom( part );
}

PMPolynom::PMPolynom( PMPart* part )
      : Base( part )
{
   int i;
   m_order = c_defaultOrder;
   m_coefficients = PMVector( 10 );
   for( i = 0; i < 10; i++ )
      m_coefficients[i] = c_defaultCoefficients[i];
   m_sturm = c_defaultSturm;
}

PMPolynom::PMPolynom( const PMPolynom& p )
      : Base( p )
{
   m_order = p.m_order;
   m_coefficients = p.m_coefficients;
   m_sturm = p.m_sturm;
}

PMPolynom::~PMPolynom()
{
}

QString PMPolynom::description() const
{
   if( m_order == 2 )
      return ( "quadric" );
   else if( m_order == 3 )
      return ( "cubic" );
   else if( m_order == 4 )
      return ( "quartic" );
   return ( "polynom" );
}

void PMPolynom::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "order", m_order );
   e.setAttribute( "coefficients", m_coefficients.serializeXML() );
   e.setAttribute( "sturm", m_sturm );
   Base::serialize( e, doc );
}

void PMPolynom::readAttributes( const PMXMLHelper& h )
{
   m_order = h.intAttribute( "order", c_defaultOrder );
   m_coefficients = h.vectorAttribute( "coefficients", m_coefficients );
   m_sturm = h.boolAttribute( "sturm", c_defaultSturm );
   Base::readAttributes( h );
}

PMMetaObject* PMPolynom::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Polynom", Base::metaObject(),
                                        createNewPolynom );
      s_pMetaObject->addProperty(
         new PMPolynomProperty( "polynomOrder", &PMPolynom::setPolynomOrder,
                         &PMPolynom::polynomOrder ) );
      s_pMetaObject->addProperty(
         new PMPolynomProperty( "sturm", &PMPolynom::setSturm,
                         &PMPolynom::sturm ) );

      s_pMetaObject->addProperty( new PMCoefficientProperty() );
   }
   return s_pMetaObject;
}

void PMPolynom::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

PMDialogEditBase* PMPolynom::editWidget( QWidget* parent ) const
{
   return new PMPolynomEdit( parent );
}

void PMPolynom::setPolynomOrder( int o )
{
   if( ( o < 2 ) || ( o > 7 ) )
   {
      qCritical(  ) << "Invalid order in PMPolynom::setPolynomOrder\n";
      o = 2;
   }
   if( o != m_order )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMOrderID, m_order );
         if( ( o <= 4 ) || ( m_order <= 4 ) )
            m_pMemento->setDescriptionChanged();
      }
      m_order = o;
   }
}

void PMPolynom::setCoefficients( const PMVector& c )
{
   if( c.size() != c_polynomSize[m_order] )
      qCritical(  ) << "Wrong vector size in PMPolynom::setCoefficients\n";

   if( c != m_coefficients )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCoefficientsID, m_coefficients );
      m_coefficients = c;
      m_coefficients.resize( c_polynomSize[m_order] );
      //setViewStructureChanged();
   }
}

void PMPolynom::setSturm( bool s )
{
   if( m_sturm != s )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSturmID, m_sturm );
      m_sturm = s;
   }
}

void PMPolynom::restoreMemento( PMMemento* s )
{
	const QList<PMMementoData*>& changes = s->changes();
	QList<PMMementoData*>::const_iterator it;
   PMMementoData* data;
	for( it = changes.begin(); it != changes.end(); ++it )
	{
		data = *it;
      if( data->objectType() == s_pMetaObject )
      {
         switch( data->valueID() )
         {
            case PMOrderID:
               setPolynomOrder( data->intData() );
               break;
            case PMCoefficientsID:
               setCoefficients( data->vectorData() );
               break;
            case PMSturmID:
               setSturm( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMPolynom::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
