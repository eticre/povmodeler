/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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

#include "pmsymboltable.h"
#include "pmdeclare.h"
#include "pmdebug.h"

#include <string.h>

PMSymbol::PMSymbol( const QString& id, PMDeclare* o )
{
   setId( id );
   m_type = Object;
   m_pObj = o;
   m_pVal = 0;
   m_pRenamedSymbol = nullptr;
}

PMSymbol::PMSymbol( const QString& id, const PMValue& v )
{
   setId( id );
   m_type = Value;
   m_pObj = 0;
   m_pVal = new PMValue( v );
   m_pRenamedSymbol = nullptr;
}

PMSymbol::~PMSymbol()
{
   if( m_pVal )
      delete m_pVal;
}

void PMSymbol::setId( const QString& id )
{
   m_id = id.left( MaxIDLength );
}

PMDeclare* PMSymbol::object() const
{
   if( m_type == Object )
      return m_pObj;
   qCritical(  ) << "Symbol is not an object\n";
   return nullptr;
}

PMValue PMSymbol::value() const
{
   if( m_type == Value )
      return *m_pVal;
   qCritical(  ) << "Symbol is not a value\n";
   return PMValue();
}


PMSymbolTable::PMSymbolTable()
{
}

PMSymbolTable::~PMSymbolTable()
{
	QHash<QString, PMSymbol*>::iterator it;
	for(it = begin(); it != end(); ++it) delete *it;
}

void PMSymbolTable::clear()
{
    QHash<QString, PMSymbol*>::iterator it;
    for(it = begin(); it != end(); ++it) delete *it;
    QHash<QString, PMSymbol*>::clear();
}

QString PMSymbolTable::findNewID( const QString& prefix )
{
   QString testID;
   unsigned int number = 0;
	iterator it = end();

	QHash<QString, int>::iterator iit = m_lastID.find( prefix );
   if( iit != m_lastID.end() )
      number = (*iit) + 1;

   // find next free id
   do
   {
      testID = prefix + QString( "%1" ).arg( number );
      it = find( testID );
      if( it != end() )
         number++;
   }
   while( it != end() );

   if( iit != m_lastID.end() )
      (*iit) = number;
   else
      m_lastID.insert( prefix, number );

   return testID;
}

PMSymbol* PMSymbolTable::findNewID( const QString& prefix, PMDeclare* obj )
{
   QString newID = findNewID( prefix );
   obj->setID( newID );

   PMSymbol* s = new PMSymbol( newID, obj );
//   insert( newID, s );
   
   return s;
}

PMSymbol* PMSymbolTable::findSymbol(const QString& name) const{
	const_iterator it = find( name );
	if( it == end() ) return 0;
	return *it;
}
