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

#ifndef PMENUMPROPERTY_H
#define PMENUMPROPERTY_H

#include "pmmetaobject.h"
#include <qmap.h>

typedef QMap<int, QString> PMEnumValueStringMap;
typedef QMap<QString, int> PMEnumStringValueMap;

/**
 * Base class for enum properties
 */
class PMEnumProperty : public PMPropertyBase
{
public:
   /**
    * Default constructor
    */
   PMEnumProperty( const char* name, bool readOnly = false,
                   bool writeOnly = false );
   
   /**
    * Adds the string to the list of enum values and sets the enum flag
    * to true
    */
   void addEnumValue( const QString& str, int value );
   
   /**
    * Returns true if the property is an enum
    */
   virtual bool isEnum() const { return true; }
   /**
    * Returns the list of enum values
    */
   virtual QStringList enumValues() const;

protected:
   /**
    * Do not reimplement this method in a sub class
    */
   virtual bool setProtected( PMObject* obj, const PMVariant& v );
   /**
    * Do not reimplement this method in a sub class
    */
   virtual PMVariant getProtected( const PMObject* obj );

   /**
    * Reimplement this method in sub classes. You can safetly
    * cast the integer to the enum type
    */
   virtual void setEnum( PMObject* obj, int value ) = 0;
   /**
    * Reimplement this method in sub classes.
    */
   virtual int getEnum( const PMObject* obj ) = 0;
   
private:
   PMEnumValueStringMap m_valueMap;
   PMEnumStringValueMap m_stringMap;
};

#define PMDefineEnumPropertyClass( ObjectClass, EnumType, PropertyClass ) \
class PropertyClass : public PMEnumProperty \
{ \
public: \
   typedef void ( ObjectClass::*SetPtr ) ( EnumType ); \
   typedef EnumType ( ObjectClass::*GetPtr ) ( void ) const; \
   \
   PropertyClass( const char* name, SetPtr setFktn, GetPtr getFktn ) \
         : PMEnumProperty( name, setFktn == nullptr, getFktn == nullptr ) \
   { \
      m_setFunktion = setFktn; \
      m_getFunktion = getFktn; \
   } \
protected: \
   virtual void setEnum( PMObject* obj, int value ) \
   { \
      ObjectClass* o = static_cast<ObjectClass*>(obj); \
      ( o->*m_setFunktion )( ( EnumType ) value ); \
   } \
   virtual int getEnum( const PMObject* obj ) \
   { \
      const ObjectClass* o = static_cast<const ObjectClass*>(obj); \
      return ( o->*m_getFunktion )(); \
   } \
private: \
   SetPtr m_setFunktion; \
   GetPtr m_getFunktion; \
}
// no semicolon, put a semicolon after the macro!

#endif
