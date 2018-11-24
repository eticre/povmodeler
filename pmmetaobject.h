//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002-2006 by Andreas Zehender
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

#ifndef PMMETAOBJECT_H
#define PMMETAOBJECT_H

#include <QList>
#include <QStringList>
#include <QHash>
#include "pmvariant.h"

class PMPart;

/**
 * Base class for all properties
 */
class PMPropertyBase
{
public:
   /**
    * Default constructor
    */
   PMPropertyBase( const QString& name, PMVariant::PMVariantDataType t,
                   bool readOnly = false, bool writeOnly = false );
   /**
    * Copy constructor
    */
   PMPropertyBase( const PMPropertyBase& p );
   /**
    * Destructor
    */
   virtual ~PMPropertyBase();

   /**
    * Returns the properties name
    */
   QString name() const { return m_name; }
   /**
    * Returns the data type
    */
   PMVariant::PMVariantDataType type() const { return m_type; }

   /**
    * Sets the property.
    *
    * Returns true if successful.
    *
    * Makes a type check and calls @ref setProtected on success.
    */
   bool setProperty( PMObject* obj, const PMVariant& );

   /**
    * Returns the property value
    */
   PMVariant getProperty( const PMObject* obj );

   /**
    * Returns the number of dimensions for array properties
    * or 0 otherwise
    */
   virtual int dimensions() const { return 0; }
   /**
    * Has to be reimplemented for array properties.
    *
    * The first parameter is the dimension, the second the index.
    */
   virtual void setIndex( int /*dimension*/, int /*index*/ ) { }
   /**
    * Has to be reimplemented for array properties.
    *
    * Returns the current size for the object and dimension.
    */
   virtual int size( PMObject*, int /*dimension*/ ) const { return 0; }

   /**
    * Returns true if the property is an enum
    */
   virtual bool isEnum() const { return false; }
   /**
    * Returns the list of enum values
    */
   virtual QStringList enumValues() const { return QStringList(); }

   /**
    * Returns true if the property is read-only. False by default
    */
   bool isReadOnly() const { return m_readOnly; }
   /**
    * Returns true if the property is write-only. False by default
    */
   bool isWriteOnly() const { return m_writeOnly; }

protected:
   /**
    * Reimplement this function to call the correct object method.
    *
    * The variant is already converted to the correct type.
    */
   virtual bool setProtected( PMObject* obj, const PMVariant& ) = 0;

   /**
    * Reimplement this function to call the correct object method.
    */
   virtual PMVariant getProtected( const PMObject* obj ) = 0;

private:
   PMVariant::PMVariantDataType m_type;
   QString m_name;
   QStringList* m_pEnumList;
   bool m_readOnly;
   bool m_writeOnly;
};

typedef QList<PMPropertyBase*> PMPropertyList;
typedef QHash<QString, PMPropertyBase*> PMPropertyDict;

/**
 * Macro that defines a property class for a PMObject class
 *
 * Example: PMDefinePropertyClass( PMBox, PMProperty ); defines
 * a class PMProperty that can store pointer to member functions
 * for PMBox objects.
 *
 * Use only in .cpp files.
 */

#define PMDefinePropertyClass( ObjectClass, PropertyClass ) \
class PropertyClass : public PMPropertyBase \
{ \
public: \
   typedef void ( ObjectClass::*SetIntPtr ) ( int ); \
   typedef void ( ObjectClass::*SetUnsignedPtr ) ( unsigned ); \
   typedef void ( ObjectClass::*SetDoublePtr ) ( double ); \
   typedef void ( ObjectClass::*SetBoolPtr ) ( bool ); \
   typedef void ( ObjectClass::*SetThreeStatePtr )( PMThreeState ); \
   typedef void ( ObjectClass::*SetStringPtr ) ( const QString& ); \
   typedef void ( ObjectClass::*SetVectorPtr ) ( const PMVector& ); \
   typedef void ( ObjectClass::*SetColorPtr ) ( const PMColor& ); \
   typedef void ( ObjectClass::*SetObjectPtr ) ( PMObject* ); \
   \
   typedef int ( ObjectClass::*GetIntPtr ) ( void ) const;  \
   typedef unsigned ( ObjectClass::*GetUnsignedPtr ) ( void ) const;  \
   typedef double ( ObjectClass::*GetDoublePtr ) ( void ) const; \
   typedef bool ( ObjectClass::*GetBoolPtr ) ( void ) const; \
   typedef PMThreeState ( ObjectClass::*GetThreeStatePtr ) ( void ) const; \
   typedef QString ( ObjectClass::*GetStringPtr ) ( void ) const; \
   typedef PMVector ( ObjectClass::*GetVectorPtr ) ( void ) const; \
   typedef PMColor ( ObjectClass::*GetColorPtr ) ( void ) const; \
   typedef PMObject* ( ObjectClass::*GetObjectPtr ) ( void ) const; \
   \
   PropertyClass( const QString& name, SetIntPtr setFktn, GetIntPtr getFktn ) \
         : PMPropertyBase( name, PMVariant::Integer, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setInt = setFktn; \
      m_getFunction.getInt = getFktn; \
   } \
   PropertyClass( const QString& name, SetUnsignedPtr setFktn, GetUnsignedPtr getFktn ) \
         : PMPropertyBase( name, PMVariant::Unsigned, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setUnsigned = setFktn; \
      m_getFunction.getUnsigned = getFktn; \
   } \
   PropertyClass( const QString& name, SetDoublePtr setFktn, GetDoublePtr getFktn ) \
         : PMPropertyBase( name, PMVariant::Double, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setDouble = setFktn; \
      m_getFunction.getDouble = getFktn; \
   } \
   PropertyClass( const QString& name, SetBoolPtr setFktn, GetBoolPtr getFktn ) \
         : PMPropertyBase( name, PMVariant::Bool, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setBool = setFktn; \
      m_getFunction.getBool = getFktn; \
   } \
   PropertyClass( const QString& name, SetThreeStatePtr setFktn, GetThreeStatePtr getFktn ) \
         : PMPropertyBase( name, PMVariant::ThreeState, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setThreeState = setFktn; \
      m_getFunction.getThreeState = getFktn; \
   } \
   PropertyClass( const QString& name, SetStringPtr setFktn, GetStringPtr getFktn ) \
         : PMPropertyBase( name, PMVariant::String, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setString = setFktn; \
      m_getFunction.getString = getFktn; \
   } \
   PropertyClass( const QString& name, SetVectorPtr setFktn, GetVectorPtr getFktn ) \
         : PMPropertyBase( name, PMVariant::Vector, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setVector = setFktn; \
      m_getFunction.getVector = getFktn; \
   } \
   PropertyClass( const QString& name, SetColorPtr setFktn, GetColorPtr getFktn ) \
         : PMPropertyBase( name, PMVariant::Color, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setColor = setFktn; \
      m_getFunction.getColor = getFktn; \
   } \
   PropertyClass( const QString& name, SetObjectPtr setFktn, GetObjectPtr getFktn ) \
         : PMPropertyBase( name, PMVariant::ObjectPointer, \
                           setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunction.setObject = setFktn; \
      m_getFunction.getObject = getFktn; \
   } \
   \
protected: \
   bool setProtected( PMObject* obj, const PMVariant& v ) \
   { \
      ObjectClass* o = static_cast<ObjectClass*>(obj); \
      switch( type() ) \
      { \
         case PMVariant::Integer: \
            ( o->*( m_setFunction.setInt ) )( v.intData() ); \
            break; \
         case PMVariant::Unsigned: \
            ( o->*( m_setFunction.setUnsigned ) )( v.unsignedData() ); \
            break; \
         case PMVariant::Double: \
            ( o->*( m_setFunction.setDouble ) )( v.doubleData() ); \
            break; \
         case PMVariant::Bool: \
            ( o->*( m_setFunction.setBool ) )( v.boolData() ); \
            break; \
         case PMVariant::ThreeState: \
            ( o->*( m_setFunction.setThreeState ) )( v.threeStateData() ); \
            break; \
         case PMVariant::String: \
            ( o->*( m_setFunction.setString ) )( v.stringData() ); \
            break; \
         case PMVariant::Vector: \
            ( o->*( m_setFunction.setVector ) )( v.vectorData() ); \
            break; \
         case PMVariant::Color: \
            ( o->*( m_setFunction.setColor ) )( v.colorData() ); \
            break; \
         case PMVariant::ObjectPointer: \
            ( o->*( m_setFunction.setObject ) )( v.objectData() ); \
            break; \
         case PMVariant::None: \
            break; \
      } \
      return true; \
   } \
   \
   PMVariant getProtected( const PMObject* obj ) \
   { \
      const ObjectClass* o = ( const ObjectClass* ) obj; \
      PMVariant result; \
      \
      switch( type() ) \
      { \
         case PMVariant::Integer: \
            result.setInt( ( o->*( m_getFunction.getInt ) )() ); \
            break; \
         case PMVariant::Unsigned: \
            result.setUnsigned( ( o->*( m_getFunction.getUnsigned ) )() ); \
            break; \
         case PMVariant::Double: \
            result.setDouble( ( o->*( m_getFunction.getDouble ) )() ); \
            break; \
         case PMVariant::Bool: \
            result.setBool( ( o->*( m_getFunction.getBool ) )() ); \
            break; \
         case PMVariant::ThreeState: \
            result.setThreeState( ( o->*( m_getFunction.getThreeState ) )() ); \
            break; \
         case PMVariant::String: \
            result.setString( ( o->*( m_getFunction.getString ) )() ); \
            break; \
         case PMVariant::Vector: \
            result.setVector( ( o->*( m_getFunction.getVector ) )() ); \
            break; \
         case PMVariant::Color: \
            result.setColor( ( o->*( m_getFunction.getColor ) )() ); \
            break; \
         case PMVariant::ObjectPointer: \
            result.setObject( ( o->*( m_getFunction.getObject ) )() ); \
            break; \
         case PMVariant::None: \
            break; \
      } \
      return result; \
   } \
   \
private: \
   union \
   { \
      SetIntPtr setInt; \
      SetUnsignedPtr setUnsigned; \
      SetDoublePtr setDouble; \
      SetBoolPtr setBool; \
      SetThreeStatePtr setThreeState; \
      SetStringPtr setString; \
      SetVectorPtr setVector; \
      SetColorPtr setColor; \
      SetObjectPtr setObject; \
   } m_setFunction; \
   \
   union \
   { \
      GetIntPtr getInt; \
      GetUnsignedPtr getUnsigned; \
      GetDoublePtr getDouble; \
      GetBoolPtr getBool; \
      GetThreeStatePtr getThreeState; \
      GetStringPtr getString; \
      GetVectorPtr getVector; \
      GetColorPtr getColor; \
      GetObjectPtr getObject; \
   } m_getFunction; \
}
// no semicolon, put a semicolon after the macro!


typedef PMObject* ( *PMObjectFactoryMethod ) ( PMPart* );

/**
 * Meta information object for the @ref PMObject class.
 *
 * Stores information (class name, inheritance hierarchy,
 * object properties) for each class.
 */
class PMMetaObject
{
public:
   /**
    * Creates a PMMetaObject. The class name has to be the class name
    * without the PM prefix.
    *
    * factoryMethod is a function pointer to a factory method
    * with signature PMObject* theMethod( PMPart* ) that returns
    * a new object of that type. factoryMethod may be 0 for
    * abstract classes.
    */
   PMMetaObject( const QString& className, PMMetaObject* superClass = nullptr,
                 PMObjectFactoryMethod factoryMethod = nullptr );
   /**
    * Destructor
    */
   ~PMMetaObject();

   /**
    * Returns the class name
    */
   QString className() const { return m_className; }
   /**
    * Returns the meta object of the super class
    */
   PMMetaObject* superClass() const { return m_pSuperClass; }
   /**
    * Returns a new object instance
    */
   PMObject* newObject( PMPart* part ) const;
   /**
    * Returns true if the class is an abstract class
    * (if no factory method was set in the constructor)
    */
   bool isAbstract() const { return m_factory == nullptr; }

   /**
    * Adds a property.
    *
    * The meta object becomes the owner of the property object
    */
   void addProperty( PMPropertyBase* p );
   /**
    * Returns a list of all properties
    */
   QStringList properties() const;
   /**
    * Returns a property by name or 0 if a property with the name
    * doesn't exist.
    */
   PMPropertyBase* property( const QString& name ) const
   {
		PMPropertyDict::const_iterator it = m_propertiesDict.find( name );
        if( it == m_propertiesDict.end() ) return nullptr;
		return *it;
   }

private:
   QString m_className;
   PMMetaObject* m_pSuperClass;
   PMPropertyList m_properties;
   PMPropertyDict m_propertiesDict;
   PMObjectFactoryMethod m_factory;
};

#endif
