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

#include "pmmemento.h"
#include "pmdebug.h"


PMMementoData::PMMementoData( PMMetaObject* classType, int vID, int data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}

PMMementoData::PMMementoData( PMMetaObject* classType, int vID, unsigned int data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}

PMMementoData::PMMementoData( PMMetaObject* classType, int vID, double data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}


PMMementoData::PMMementoData( PMMetaObject* classType, int vID, bool data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}

PMMementoData::PMMementoData( PMMetaObject* classType, int vID, PMThreeState data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}


PMMementoData::PMMementoData( PMMetaObject* classType, int vID, const QString& data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}

PMMementoData::PMMementoData( PMMetaObject* classType, int vID, const PMVector& data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}

PMMementoData::PMMementoData( PMMetaObject* classType, int vID, const PMColor& data )
      : PMVariant( data )
{
   m_objectType = classType;
   m_valueID = vID;
}

PMMementoData::PMMementoData( PMMetaObject* classType, int vID, PMObject* obj )
      : PMVariant( obj )
{
   m_objectType = classType;
   m_valueID = vID;
}

PMMemento::PMMemento( PMObject* originator )
{
   m_pIDData = 0;
   m_pOriginatorChange = 0;
   m_pOriginator = originator;
}

PMMemento::~PMMemento()
{
	foreach( PMMementoData* data, m_data ) delete data;
   m_data.clear();
	foreach( PMObjectChange* change, m_changedObjects ) delete change;
   m_changedObjects.clear();
   // m_pOriginatorChange is in m_changedObjects and is already deleted!
}

PMMementoData* PMMemento::findData( PMMetaObject* classType, int vID ) const
{
	QList<PMMementoData*>::const_iterator it;
	for( it = m_data.begin(); it != m_data.end(); ++it )
   {
      if( ( ( *it )->objectType() == classType ) &&
          ( ( *it )->valueID() == vID ) )
         return *it;
   }
   return 0;
}

void PMMemento::addData( PMMementoData* data )
{
   m_data.append( data );
   addChange( PMCData );
}

void PMMemento::addData( PMMetaObject* classType, int vID, const int data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addData( PMMetaObject* classType, int vID, const unsigned int data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addData( PMMetaObject* classType, int vID, const double data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addData( PMMetaObject* classType, int vID, const bool data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addData( PMMetaObject* classType, int vID, const PMThreeState data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addData( PMMetaObject* classType, int vID, const QString& data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addIDChange( PMMetaObject* classType, int vID, const QString& data )
{
   if( !findData( classType, vID ) )
   {
      PMMementoData* d = new PMMementoData( classType, vID, data );
      addData( d );
      m_pIDData = d;
   }
}

void PMMemento::addData( PMMetaObject* classType, int vID, const PMVector& data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addData( PMMetaObject* classType, int vID, const PMColor& data )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, data ) );
}

void PMMemento::addData( PMMetaObject* classType, int vID, PMObject* obj )
{
   if( !findData( classType, vID ) )
      addData( new PMMementoData( classType, vID, obj ) );
}

QString PMMemento::oldID() const
{
   if( m_pIDData )
      return m_pIDData->stringData();
   return QString();
}

void PMMemento::addChangedObject( PMObject* obj, int mode )
{
   QList<PMObjectChange*>::iterator it = m_changedObjects.begin();
   PMObjectChange* change = 0;

   while( it != m_changedObjects.end() && !change )
   {
      if( ( *it )->object() == obj )
         change = *it;
      else
         ++it;
   }
   
   if( change )
      change->addMode( mode );
   else
      m_changedObjects.append( new PMObjectChange( obj, mode ) );
}

void PMMemento::addChange( int mode )
{
   if( !m_pOriginatorChange )
   {
      m_pOriginatorChange = new PMObjectChange( m_pOriginator, PMCData );
      m_changedObjects.append( m_pOriginatorChange );
   }
   m_pOriginatorChange->addMode( mode );
}
