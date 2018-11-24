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


#include "pmjuliafractal.h"

#include "pmxmlhelper.h"
#include "pmjuliafractaledit.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pm3dcontrolpoint.h"
#include "pmenumproperty.h"



const PMVector c_defaultJuliaParameter = PMVector( -0.083, 0.0, -0.83, -0.025 );
const PMVector c_defaultSliceNormal = PMVector( 0.0, 0.0, 0.0, 1.0 );
const double c_defaultSliceDistance = 0.0;
const int c_defaultMaxIterations = 20;
const PMJuliaFractal::AlgebraType c_defaultAlgebraType = PMJuliaFractal::Quaternion;
const QString c_defaultAlgebraString = "quaternion";
const PMJuliaFractal::FunctionType c_defaultFunctionType = PMJuliaFractal::FTsqr;
const QString c_defaultFunctionString = "sqr";
const PMVector c_defaultExponent = PMVector( 0.0, 0.0 );
const double c_defaultPrecision = 20.0;


PMDefinePropertyClass( PMJuliaFractal, PMJuliaFractalProperty );
PMDefineEnumPropertyClass( PMJuliaFractal, PMJuliaFractal::AlgebraType,
                           PMAlgebraTypeProperty );
PMDefineEnumPropertyClass( PMJuliaFractal, PMJuliaFractal::FunctionType,
                           PMFunctionTypeProperty );

PMMetaObject* PMJuliaFractal::s_pMetaObject = 0;
PMObject* createNewJuliaFractal( PMPart* part )
{
   return new PMJuliaFractal( part );
}

PMJuliaFractal::PMJuliaFractal( PMPart* part )
      : Base( part )
{
   m_juliaParameter = c_defaultJuliaParameter;
   m_algebraType = c_defaultAlgebraType;
   m_functionType = c_defaultFunctionType;
   m_maxIterations = c_defaultMaxIterations;
   m_precision = c_defaultPrecision;
   m_sliceNormal = c_defaultSliceNormal;
   m_sliceDistance = c_defaultSliceDistance;
   m_exponent = c_defaultExponent;
}

PMJuliaFractal::PMJuliaFractal( const PMJuliaFractal& f )
      : Base( f )
{
   m_juliaParameter = f.m_juliaParameter;
   m_algebraType = f.m_algebraType;
   m_functionType = f.m_functionType;
   m_maxIterations = f.m_maxIterations;
   m_precision = f.m_precision;
   m_sliceNormal = f.m_sliceNormal;
   m_sliceDistance = f.m_sliceDistance;
   m_exponent = f.m_exponent;
}

PMJuliaFractal::~PMJuliaFractal()
{
}

QString PMJuliaFractal::description() const
{
   return ( "julia fractal" );
}

void PMJuliaFractal::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "julia_parameter", m_juliaParameter.serializeXML() );
   e.setAttribute( "algebra_type", algebraTypeToString( m_algebraType ) );
   e.setAttribute( "function_type", functionTypeToString( m_functionType ) );
   e.setAttribute( "max_iterations", m_maxIterations );
   e.setAttribute( "precision", m_precision );
   e.setAttribute( "slice_normal", m_sliceNormal.serializeXML() );
   e.setAttribute( "slice_distance", m_sliceDistance );
   e.setAttribute( "exponent", m_exponent.serializeXML() );
   Base::serialize( e, doc );
}

void PMJuliaFractal::readAttributes( const PMXMLHelper& h )
{
   m_juliaParameter = h.vectorAttribute( "julia_parameter", c_defaultJuliaParameter );
   m_algebraType = stringToAlgebraType( h.stringAttribute( "algebra_type", c_defaultAlgebraString ) );
   m_functionType = stringToFunctionType( h.stringAttribute( "function_type", c_defaultFunctionString ) );
   m_maxIterations = h.intAttribute( "max_iterations", c_defaultMaxIterations );
   m_precision = h.doubleAttribute( "precision", c_defaultPrecision );
   m_sliceNormal = h.vectorAttribute( "slice_normal", c_defaultSliceNormal );
   m_sliceDistance = h.doubleAttribute( "slice_distance", c_defaultSliceDistance );
   m_exponent = h.vectorAttribute( "exponent", c_defaultExponent );
   Base::readAttributes( h );
}

PMMetaObject* PMJuliaFractal::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "JuliaFractal", Base::metaObject(),
                                        createNewJuliaFractal );
      s_pMetaObject->addProperty(
         new PMJuliaFractalProperty( "juliaParameter", &PMJuliaFractal::setJuliaParameter,
                         &PMJuliaFractal::juliaParameter ) );
      s_pMetaObject->addProperty(
         new PMJuliaFractalProperty( "maximumIterations", &PMJuliaFractal::setMaximumIterations,
                         &PMJuliaFractal::maximumIterations ) );
      s_pMetaObject->addProperty(
         new PMJuliaFractalProperty( "precision", &PMJuliaFractal::setPrecision,
                         &PMJuliaFractal::precision ) );
      s_pMetaObject->addProperty(
         new PMJuliaFractalProperty( "sliceNormal", &PMJuliaFractal::setSliceNormal,
                         &PMJuliaFractal::sliceNormal ) );
      s_pMetaObject->addProperty(
         new PMJuliaFractalProperty( "sliceDistance", &PMJuliaFractal::setSliceDistance,
                         &PMJuliaFractal::sliceDistance ) );
      s_pMetaObject->addProperty(
         new PMJuliaFractalProperty( "exponent", &PMJuliaFractal::setExponent,
                         &PMJuliaFractal::exponent ) );

      PMAlgebraTypeProperty* ap = new PMAlgebraTypeProperty(
         "algebraType", &PMJuliaFractal::setAlgebraType, &PMJuliaFractal::algebraType );
      ap->addEnumValue( "Quaternion", Quaternion );
      ap->addEnumValue( "Hypercomplex", Hypercomplex );
      s_pMetaObject->addProperty( ap );

      PMFunctionTypeProperty* fp = new PMFunctionTypeProperty(
         "functionType", &PMJuliaFractal::setFunctionType, &PMJuliaFractal::functionType );
      fp->addEnumValue( "sqr", FTsqr );
      fp->addEnumValue( "cube", FTcube );
      fp->addEnumValue( "exp", FTexp );
      fp->addEnumValue( "reciprocal", FTreciprocal );
      fp->addEnumValue( "sin", FTsin );
      fp->addEnumValue( "asin", FTasin );
      fp->addEnumValue( "sinh", FTsinh );
      fp->addEnumValue( "asinh", FTasinh );
      fp->addEnumValue( "cos", FTcos );
      fp->addEnumValue( "acos", FTacos );
      fp->addEnumValue( "cosh", FTcosh );
      fp->addEnumValue( "acosh", FTacosh );
      fp->addEnumValue( "tan", FTtan );
      fp->addEnumValue( "atan", FTatan );
      fp->addEnumValue( "tanh", FTtanh );
      fp->addEnumValue( "atanh", FTatanh );
      fp->addEnumValue( "log", FTlog );
      fp->addEnumValue( "pwr", FTpwr );
      s_pMetaObject->addProperty( fp );
   }
   return s_pMetaObject;
}

void PMJuliaFractal::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMJuliaFractal::setJuliaParameter( const PMVector& p )
{
   if( p != m_juliaParameter )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMJuliaParameterID, m_juliaParameter );
      m_juliaParameter = p;
      m_juliaParameter.resize( 4 );
   }
}

void PMJuliaFractal::setAlgebraType( PMJuliaFractal::AlgebraType t )
{
   if( m_algebraType != t )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAlgebraTypeID, m_algebraType );
      m_algebraType = t;
   }
}

void PMJuliaFractal::setFunctionType( PMJuliaFractal::FunctionType t )
{
   if( m_functionType != t )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFunctionTypeID, m_functionType );
      m_functionType = t;
   }
}

void PMJuliaFractal::setMaximumIterations( int max )
{
   if( max <= 0 )
   {
      qCritical(  ) << "max <= 0 in PMJuliaFractal::setMaximumIterations\n";
      max = 20;
   }
   if( m_maxIterations != max )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMaxIterationsID, m_maxIterations );
      m_maxIterations = max;
   }
}

void PMJuliaFractal::setPrecision( double p )
{
   if( p < 1.0 )
   {
      qCritical(  ) << "p < 1.0 in PMJuliaFractal::setPrecision\n";
      p = 1.0;
   }

   if( m_precision != p )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMPrecisionID, m_precision );
      m_precision = p;
   }
}

void PMJuliaFractal::setSliceNormal( const PMVector& n )
{
   if( m_sliceNormal != n )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSliceNormalID, m_sliceNormal );
      m_sliceNormal = n;
      m_sliceNormal.resize( 4 );
   }
}

void PMJuliaFractal::setSliceDistance( double d )
{
   if( m_sliceDistance != d )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSliceDistanceID, m_sliceDistance );
      m_sliceDistance = d;
   }
}

void PMJuliaFractal::setExponent( const PMVector& e )
{
   if( m_exponent != e )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMExponentID, m_exponent );
      m_exponent = e;
      m_exponent.resize( 2 );
   }
}

PMDialogEditBase* PMJuliaFractal::editWidget( QWidget* parent ) const
{
   return new PMJuliaFractalEdit( parent );
}

void PMJuliaFractal::restoreMemento( PMMemento* s )
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
            case PMJuliaParameterID:
               setJuliaParameter( data->vectorData() );
               break;
            case PMAlgebraTypeID:
               setAlgebraType( ( AlgebraType ) data->intData() );
               break;
            case PMFunctionTypeID:
               setFunctionType( ( FunctionType ) data->intData() );
               break;
            case PMMaxIterationsID:
               setMaximumIterations( data->intData() );
               break;
            case PMPrecisionID:
               setPrecision( data->doubleData() );
               break;
            case PMSliceNormalID:
               setSliceNormal( data->vectorData() );
               break;
            case PMSliceDistanceID:
               setSliceDistance( data->doubleData() );
               break;
            case PMExponentID:
               setExponent( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMJuliaFractal::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

QString PMJuliaFractal::functionTypeToString( PMJuliaFractal::FunctionType t )
{
   QString result = "sqr";
   switch( t )
   {
      case FTsqr:
         result = "sqr";
         break;
      case FTcube:
         result = "cube";
         break;
      case FTexp:
         result = "exp";
         break;
      case FTreciprocal:
         result = "reciprocal";
         break;
      case FTsin:
         result = "sin";
         break;
      case FTasin:
         result = "asin";
         break;
      case FTsinh:
         result = "sinh";
         break;
      case FTasinh:
         result = "asinh";
         break;
      case FTcos:
         result = "cos";
         break;
      case FTacos:
         result = "acos";
         break;
      case FTcosh:
         result = "cosh";
         break;
      case FTacosh:
         result = "acosh";
         break;
      case FTtan:
         result = "tan";
         break;
      case FTatan:
         result = "atan";
         break;
      case FTtanh:
         result = "tanh";
         break;
      case FTatanh:
         result = "atanh";
         break;
      case FTlog:
         result = "log";
         break;
      case FTpwr:
         result = "pwr";
         break;
   }
   return result;
}

PMJuliaFractal::FunctionType PMJuliaFractal::stringToFunctionType( const QString& str )
{
   FunctionType t = c_defaultFunctionType;

   if( str == "sqr" )
      t = FTsqr;
   else if( str == "cube" )
      t = FTcube;
   else if( str == "exp" )
      t = FTexp;
   else if( str == "reciprocal" )
      t = FTreciprocal;
   else if( str == "sin" )
      t = FTsin;
   else if( str == "asin" )
      t = FTasin;
   else if( str == "sinh" )
      t = FTsinh;
   else if( str == "asinh" )
      t = FTasinh;
   else if( str == "cos" )
      t = FTcos;
   else if( str == "acos" )
      t = FTacos;
   else if( str == "cosh" )
      t = FTcosh;
   else if( str == "acosh" )
      t = FTacosh;
   else if( str == "tan" )
      t = FTtan;
   else if( str == "atan" )
      t = FTatan;
   else if( str == "tanh" )
      t = FTtanh;
   else if( str == "atanh" )
      t = FTatanh;
   else if( str == "log" )
      t = FTlog;
   else if( str == "pwr" )
      t = FTpwr;
   return t;
}

QString PMJuliaFractal::algebraTypeToString( PMJuliaFractal::AlgebraType t )
{
   QString result;
   if( t == Quaternion )
      result = "quaternion";
   else
      result = "hypercomplex";
   return result;
}

PMJuliaFractal::AlgebraType PMJuliaFractal::stringToAlgebraType( const QString& str )
{
   AlgebraType t = c_defaultAlgebraType;
   if( str == "quaternion" )
      t = Quaternion;
   else if( str == "hypercomplex" )
      t = Hypercomplex;
   return t;
}
