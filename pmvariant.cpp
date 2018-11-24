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

#include "pmvariant.h"
#include "pmdebug.h"

inline const char* dcopTypeName( const PMVariant& ) { return "PMVariant"; }

PMVariant::PMVariant()
{
   m_dataType = PMVariant::None;
   m_pData = 0;
}

PMVariant::PMVariant( int data )
{
   m_dataType = PMVariant::Integer;
   m_pData = new int( data );
}

PMVariant::PMVariant( unsigned int data )
{
   m_dataType = PMVariant::Unsigned;
   m_pData = new unsigned( data );
}

PMVariant::PMVariant( double data )
{
   m_dataType = PMVariant::Double;
   m_pData = new double( data );
}


PMVariant::PMVariant( bool data )
{
   m_dataType = PMVariant::Bool;
   m_pData = new bool( data );
}

PMVariant::PMVariant( PMThreeState data )
{
   m_dataType = PMVariant::ThreeState;
   m_pData = new PMThreeState( data );
}


PMVariant::PMVariant( const QString& data )
{
   m_dataType = PMVariant::String;
   m_pData = new QString( data );
}

PMVariant::PMVariant( const PMVector& data )
{
   m_dataType = PMVariant::Vector;
   m_pData = new PMVector( data );
}

PMVariant::PMVariant( const PMColor& data )
{
   m_dataType = PMVariant::Color;
   m_pData = new PMColor( data );
}

PMVariant::PMVariant( PMObject* obj )
{
   m_dataType = PMVariant::ObjectPointer;
   m_pData = ( void* ) obj;
}

PMVariant::PMVariant( const PMVariant& v )
{
   m_pData = 0;
   m_dataType = PMVariant::None;

   switch( v.m_dataType )
   {
      case PMVariant::Integer:
         setInt( *( ( int* ) v.m_pData ) );
         break;
      case PMVariant::Unsigned:
         setUnsigned( *( ( unsigned* ) v.m_pData ) );
         break;
      case PMVariant::Double:
         setDouble( *( ( double* ) v.m_pData ) );
         break;
      case PMVariant::Bool:
         setBool( *( ( bool* ) v.m_pData ) );
         break;
      case PMVariant::ThreeState:
         setThreeState( *( ( PMThreeState* ) v.m_pData ) );
         break;
      case PMVariant::String:
         setString( *( ( QString* ) v.m_pData ) );
         break;
      case PMVariant::Vector:
         setVector( *( ( PMVector* ) v.m_pData ) );
         break;
      case PMVariant::Color:
         setColor( *( ( PMColor* ) v.m_pData ) );
         break;
      case PMVariant::ObjectPointer:
         setObject( ( PMObject* ) v.m_pData );
         break;
      case PMVariant::None:
         break;
   }
}

PMVariant& PMVariant::operator= ( const PMVariant& v )
{
   switch( v.m_dataType )
   {
      case PMVariant::Integer:
         setInt( *( ( int* ) v.m_pData ) );
         break;
      case PMVariant::Unsigned:
         setUnsigned( *( ( unsigned* ) v.m_pData ) );
         break;
      case PMVariant::Double:
         setDouble( *( ( double* ) v.m_pData ) );
         break;
      case PMVariant::Bool:
         setBool( *( ( bool* ) v.m_pData ) );
         break;
      case PMVariant::ThreeState:
         setThreeState( *( ( PMThreeState* ) v.m_pData ) );
         break;
      case PMVariant::String:
         setString( *( ( QString* ) v.m_pData ) );
         break;
      case PMVariant::Vector:
         setVector( *( ( PMVector* ) v.m_pData ) );
         break;
      case PMVariant::Color:
         setColor( *( ( PMColor* ) v.m_pData ) );
         break;
      case PMVariant::ObjectPointer:
         setObject( ( PMObject* ) v.m_pData );
         break;
      case PMVariant::None:
         break;
   }

   return *this;
}

PMVariant::~PMVariant()
{
   clear();
}

void PMVariant::clear()
{
   switch( m_dataType )
   {
      case PMVariant::Integer:
         delete( ( int* ) m_pData );
         break;
      case PMVariant::Unsigned:
         delete( ( unsigned* ) m_pData );
         break;
      case PMVariant::Double:
         delete( ( double* ) m_pData );
         break;
      case PMVariant::Bool:
         delete( ( bool* ) m_pData );
         break;
      case PMVariant::ThreeState:
         delete( ( PMThreeState* ) m_pData );
         break;
      case PMVariant::String:
         delete( ( QString* ) m_pData );
         break;
      case PMVariant::Vector:
         delete( ( PMVector* ) m_pData );
         break;
      case PMVariant::Color:
         delete( ( PMColor* ) m_pData );
         break;
      case PMVariant::ObjectPointer:
         // delete nothing
         break;
      case PMVariant::None:
         break;
   }

   m_dataType = PMVariant::None;
   m_pData = 0;
}

void PMVariant::setInt( const int data )
{
   if( m_dataType != PMVariant::Integer )
   {
      clear();
      m_pData = new int( data );
      m_dataType = PMVariant::Integer;
   }
   else
      *( ( int* ) m_pData ) = data;
}

void PMVariant::setUnsigned( const unsigned int data )
{
   if( m_dataType != PMVariant::Unsigned )
   {
      clear();
      m_pData = new unsigned( data );
      m_dataType = PMVariant::Unsigned;
   }
   else
      *( ( unsigned* ) m_pData ) = data;
}

void PMVariant::setDouble( const double data )
{
   if( m_dataType != PMVariant::Double )
   {
      clear();
      m_pData = new double( data );
      m_dataType = PMVariant::Double;
   }
   else
      *( ( double* ) m_pData ) = data;
}

void PMVariant::setBool( const bool data )
{
   if( m_dataType != PMVariant::Bool )
   {
      clear();
      m_pData = new bool( data );
      m_dataType = PMVariant::Bool;
   }
   else
      *( ( bool* ) m_pData ) = data;
}

void PMVariant::setThreeState( const PMThreeState data )
{
   if( m_dataType != PMVariant::ThreeState )
   {
      clear();
      m_pData = new PMThreeState( data );
      m_dataType = PMVariant::ThreeState;
   }
   else
      *( ( PMThreeState* ) m_pData ) = data;
}

void PMVariant::setString( const QString& data )
{
   if( m_dataType != PMVariant::String )
   {
      clear();
      m_pData = new QString( data );
      m_dataType = PMVariant::String;
   }
   else
      *( ( QString* ) m_pData ) = data;
}

void PMVariant::setVector( const PMVector& data )
{
   if( m_dataType != PMVariant::Vector )
   {
      clear();
      m_pData = new PMVector( data );
      m_dataType = PMVariant::Vector;
   }
   else
      *( ( PMVector* ) m_pData ) = data;
}

void PMVariant::setColor( const PMColor& data )
{
   if( m_dataType != PMVariant::Color )
   {
      clear();
      m_pData = new PMColor( data );
      m_dataType = PMVariant::Color;
   }
   else
      *( ( PMColor* ) m_pData ) = data;
}

void PMVariant::setObject( PMObject* obj )
{
   if( m_dataType != PMVariant::ObjectPointer )
   {
      clear();
      m_pData = obj;
      m_dataType = PMVariant::ObjectPointer;
   }
   else
      m_pData = obj;
}

int PMVariant::intData() const
{
   if( m_dataType == PMVariant::Integer )
      return *( ( int* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return 0;
}

int PMVariant::unsignedData() const
{
   if( m_dataType == PMVariant::Unsigned )
      return *( ( unsigned* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return 0;
}

double PMVariant::doubleData() const
{
   if( m_dataType == PMVariant::Double )
      return *( ( double* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return 0;
}

bool PMVariant::boolData() const
{
   if( m_dataType == PMVariant::Bool )
      return *( ( bool* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return false;
}

PMThreeState PMVariant::threeStateData() const
{
   if( m_dataType == PMVariant::ThreeState )
      return *( ( PMThreeState* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return PMUnspecified;
}

QString PMVariant::stringData() const
{
   if( m_dataType == PMVariant::String )
      return *( ( QString* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return QString();
}

PMVector PMVariant::vectorData() const
{
   if( m_dataType == PMVariant::Vector )
      return *( ( PMVector* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return PMVector();
}

PMColor PMVariant::colorData() const
{
   if( m_dataType == PMVariant::Color )
      return *( ( PMColor* ) m_pData );
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return PMColor();
}

PMObject* PMVariant::objectData() const
{
   if( m_dataType == PMVariant::ObjectPointer )
      return ( PMObject* ) m_pData;
   qCritical(  ) << "Wrong type in PMVariant get function\n";
   return 0;
}

bool PMVariant::convertTo( PMVariant::PMVariantDataType t )
{
   bool success = true;

   switch( m_dataType )
   {
      case PMVariant::Integer:
      {
         int data = *( ( int* ) m_pData );

         switch( t )
         {
            case PMVariant::Integer:
               break;
            case PMVariant::Unsigned:
               setUnsigned( ( unsigned ) data );
               break;
            case PMVariant::Double:
               setDouble( ( double ) data );
               break;
            case PMVariant::Bool:
               setBool( ( bool ) data );
               break;
            case PMVariant::ThreeState:
               success = false;
               break;
            case PMVariant::String:
            {
               QString tmp;
               tmp.setNum( data );
               setString( tmp );
               break;
            }
            case PMVariant::Vector:
               success = false;
               break;
            case PMVariant::Color:
               success = false;
               break;
            case PMVariant::ObjectPointer:
               success = false;
               break;
            case PMVariant::None:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::Unsigned:
      {
         unsigned data = *( ( unsigned* ) m_pData );

         switch( t )
         {
            case PMVariant::Integer:
               setUnsigned( ( int ) data );
               break;
            case PMVariant::Unsigned:
               break;
            case PMVariant::Double:
               setDouble( ( double ) data );
               break;
            case PMVariant::Bool:
               setBool( ( bool ) data );
               break;
            case PMVariant::ThreeState:
               success = false;
               break;
            case PMVariant::String:
            {
               QString tmp;
               tmp.setNum( data );
               setString( tmp );
               break;
            }
            case PMVariant::Vector:
               success = false;
               break;
            case PMVariant::Color:
               success = false;
               break;
            case PMVariant::ObjectPointer:
               success = false;
               break;
            case PMVariant::None:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::Double:
      {
         double data = *( ( double* ) m_pData );

         switch( t )
         {
            case PMVariant::Integer:
               setInt( ( int ) data );
               break;
            case PMVariant::Unsigned:
               setUnsigned( ( unsigned ) data );
               break;
            case PMVariant::Double:
               break;
            case PMVariant::Bool:
               setBool( ( bool ) data );
               break;
            case PMVariant::ThreeState:
               success = false;
               break;
            case PMVariant::String:
            {
               QString tmp;
               tmp.setNum( data );
               setString( tmp );
               break;
            }
            case PMVariant::Vector:
               success = false;
               break;
            case PMVariant::Color:
               success = false;
               break;
            case PMVariant::ObjectPointer:
               success = false;
               break;
            case PMVariant::None:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::Bool:
      {
         bool data = *( ( bool* ) m_pData );

         switch( t )
         {
            case PMVariant::Integer:
               setInt( ( int ) data );
               break;
            case PMVariant::Unsigned:
               setUnsigned( ( unsigned ) data );
               break;
            case PMVariant::Double:
               setDouble( ( double ) data );
               break;
            case PMVariant::Bool:
               break;
            case PMVariant::ThreeState:
               if( data )
                  setThreeState( PMTrue );
               else
                  setThreeState( PMFalse );
               break;
            case PMVariant::String:
               if( data )
                  setString( QString( "true" ) );
               else
                  setString( QString( "false" ) );
               break;
            case PMVariant::Vector:
               success = false;
               break;
            case PMVariant::Color:
               success = false;
               break;
            case PMVariant::ObjectPointer:
               success = false;
               break;
            case PMVariant::None:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::ThreeState:
      {
         PMThreeState data = *( ( PMThreeState* ) m_pData );

         switch( t )
         {
            case PMVariant::Integer:
               success = false;
               break;
            case PMVariant::Unsigned:
               success = false;
               break;
            case PMVariant::Double:
               success = false;
               break;
            case PMVariant::Bool:
               if( data == PMTrue )
                  setBool( true );
               else if( data == PMFalse )
                  setBool( false );
               else
                  success = false;
               break;
            case PMVariant::ThreeState:
               break;
            case PMVariant::String:
               if( data == PMTrue )
                  setString( QString( "true" ) );
               else if( data == PMFalse )
                  setString( QString( "false" ) );
               else
                  setString( QString( "unspecified" ) );
               break;
            case PMVariant::Vector:
               success = false;
               break;
            case PMVariant::Color:
               success = false;
               break;
            case PMVariant::ObjectPointer:
               success = false;
               break;
            case PMVariant::None:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::String:
      {
         QString data = *( ( QString* ) m_pData );

         switch( t )
         {
            case PMVariant::Integer:
            {
               int i = data.toInt( &success );
               if( success )
                  setInt( i );
               break;
            }
            case PMVariant::Unsigned:
            {
               unsigned u = data.toUInt( &success );
               if( success )
                  setUnsigned( u );
               break;
            }
            case PMVariant::Double:
            {
               double d = data.toDouble( &success );
               if( success )
                  setDouble( d );
               break;
            }
            case PMVariant::Bool:
               if( data == "true" || data == "on" || data == "yes" )
                  setBool( true );
               else if( data == "false" || data == "off" || data == "no" )
                  setBool( false );
               else
                  success = false;
               break;
            case PMVariant::ThreeState:
               if( data == "true" || data == "on" || data == "yes" )
                  setThreeState( PMTrue );
               else if( data == "false" || data == "off" || data == "no" )
                  setThreeState( PMFalse );
               else if( data == "unspecified" )
                  setThreeState( PMUnspecified );
               else
                  success = false;
               break;
            case PMVariant::String:
               break;
            case PMVariant::Vector:
               success = false;
               break;
            case PMVariant::Color:
               success = false;
               break;
            case PMVariant::ObjectPointer:
               success = false;
               break;
            case PMVariant::None:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::Vector:
      {
         switch( t )
         {
            case PMVariant::Vector:
               break;
            case PMVariant::Color:
            {
               PMVector v = *( ( PMVector* ) m_pData );
               if( v.size() == 5 )
                  setColor( v );
               else
                  success = false;
               break;
            }
            default:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::Color:
      {
         switch( t )
         {
            case PMVariant::Vector:
            {
               PMColor c = *( ( PMColor* ) m_pData );
               PMVector v( 5 );
               v[0] = c.red();
               v[1] = c.green();
               v[2] = c.blue();
               v[3] = c.filter();
               v[4] = c.transmit();
               setVector( v );
               break;
            }
            case PMVariant::Color:
               break;
            default:
               success = false;
               break;
         }
         break;
      }

      case PMVariant::ObjectPointer:
         success = ( t == PMVariant::ObjectPointer );
         break;

      case PMVariant::None:
         success = ( t == PMVariant::None );
         break;
   }

   return success;
}

QString PMVariant::asString() const
{
   QString tmp;

   switch( m_dataType )
   {
      case PMVariant::Integer:
      {
         int data = *( ( int* ) m_pData );

         tmp.setNum( data );
         break;
      }
      case PMVariant::Unsigned:
      {
         unsigned data = *( ( unsigned* ) m_pData );

         tmp.setNum( data );
         break;
      }
      case PMVariant::Double:
      {
         double data = *( ( double* ) m_pData );

         tmp.setNum( data );
         break;
      }
      case PMVariant::Bool:
      {
         bool data = *( ( bool* ) m_pData );

         if( data )
            tmp = "true";
         else
            tmp = "false";
         break;
      }
      case PMVariant::ThreeState:
      {
         PMThreeState data = *( ( PMThreeState* ) m_pData );

         if( data == PMTrue )
            tmp = "true";
         else if( data == PMFalse )
            tmp = "false";
         else
            tmp = "unspecified";
         break;
      }
      case PMVariant::String:
      {
         tmp = *( ( QString* ) m_pData );
         break;
      }
      case PMVariant::Vector:
      {
         PMVector v = *( ( PMVector* ) m_pData );
         tmp = v.serializeXML();
         break;
      }

      case PMVariant::Color:
      {
         PMColor c = *( ( PMColor* ) m_pData );
         tmp = c.serializeXML();
         break;
      }

      case PMVariant::ObjectPointer:
         tmp = "<object_pointer>";
         break;

      case PMVariant::None:
         tmp = "<none>"; 
         break;

      default:
         tmp = "<unknown>";
         break;
   }

   return tmp;
}

bool PMVariant::fromString( const PMVariant::PMVariantDataType t, const QString& value )
{
   bool success;

   switch( t )
   {
      case PMVariant::Integer:
      { 
         int i = value.toInt( &success );
         if( success )
            setInt( i );
         break;
      }
      case PMVariant::Unsigned:
      {
         unsigned u = value.toUInt( &success );
         if( success )
            setUnsigned( u );
         break;
      }
      case PMVariant::Double:
      {
         double d = value.toDouble( &success );
         if( success )
            setDouble( d );
         break;
      }
      case PMVariant::Bool:
         success = true;
         if( value == "true" || value == "on" || value == "yes" )
            setBool( true );
         else if( value == "false" || value == "off" || value == "no" )
            setBool( false );
         else
            success = false;
         break;
      case PMVariant::ThreeState:
         success = true; // Assume success, set to false if we fail.
         if( value == "true" || value == "on" || value == "yes" )
            setThreeState( PMTrue );
         else if( value == "false" || value == "off" || value == "no" )
            setThreeState( PMFalse );
         else if( value == "unspecified" )
            setThreeState( PMUnspecified );
         else
            success = false;
         break;
      case PMVariant::String:
         setString( value );
         success = true;
         break;
      case PMVariant::Vector:
         {
            PMVector v;
            v.loadXML( value );
            setVector( v );
            success = true;
            break;
         }
      case PMVariant::Color:
         {
            PMColor c;
            c.loadXML( value );
            setColor( c );
            success = true;
            break;
         }
      default:
         success = false;
   }
   return success;
}

QDataStream& operator<<( QDataStream& stream, const PMVariant& value )
{
   stream << ( qint16 )value.dataType();
   stream << value.asString();

   return stream;
}

QDataStream& operator>>( QDataStream& stream, PMVariant& value ) 
{
   qint8 type;
   PMVariant::PMVariantDataType dataType;
   QString str;

   stream >> type;
   dataType = ( PMVariant::PMVariantDataType )type;

   stream >> str;

   value.fromString( dataType, str );

   return stream;
}
