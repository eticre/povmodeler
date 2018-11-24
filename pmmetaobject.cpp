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

#include "pmmetaobject.h"

PMPropertyBase::PMPropertyBase( const QString& name,
                                PMVariant::PMVariantDataType t,
                                bool readOnly, bool writeOnly )
{
   m_name = name;
   m_type = t;
   m_pEnumList = nullptr;
   m_readOnly = readOnly;
   m_writeOnly = writeOnly;
}

PMPropertyBase::PMPropertyBase( const PMPropertyBase& p )
{
   m_name = p.m_name;
   m_type = p.m_type;
   m_readOnly = p.m_readOnly;
   m_writeOnly = p.m_writeOnly;

   if( p.m_pEnumList )
      m_pEnumList = new QStringList( *( p.m_pEnumList ) );
   else
      m_pEnumList = nullptr;
}

PMPropertyBase::~PMPropertyBase()
{
    delete m_pEnumList;
}

bool PMPropertyBase::setProperty( PMObject* obj, const PMVariant& v )
{
   if( m_readOnly )
      return false;
   PMVariant cp = v;
   if( cp.convertTo( m_type ) )
      return setProtected( obj, cp );
   return false;
}

PMVariant PMPropertyBase::getProperty( const PMObject* obj )
{
   if( m_writeOnly )
      return PMVariant();
   return getProtected( obj );
}

PMMetaObject::PMMetaObject( const QString& className, PMMetaObject* superClass,
                            PMObjectFactoryMethod factory )
{
   m_className = className;
   m_pSuperClass = superClass;
   m_factory = factory;

   // add the properties of the super class to the dictionary
   if( m_pSuperClass )
      m_propertiesDict = superClass->m_propertiesDict;
}

PMMetaObject::~PMMetaObject()
{
	foreach(PMPropertyBase* p, m_properties) delete p;
   m_properties.clear();
}

void PMMetaObject::addProperty( PMPropertyBase* p )
{
   m_properties.append( p );
   m_propertiesDict.insert( p->name(), p );
}

PMObject* PMMetaObject::newObject( PMPart* part ) const
{
   if( m_factory )
      return m_factory( part );
   return nullptr;
}

QStringList PMMetaObject::properties() const
{
	QStringList result;
	PMPropertyDict::const_iterator it;
    for(it = m_propertiesDict.begin(); it != m_propertiesDict.end(); ++it){
        result.append( (*it)->name() );
	}
	return result;
}
