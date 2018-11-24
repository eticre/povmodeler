/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#include "pmpovraymatrix.h"
#include "pmpovraymatrixedit.h"

#include "pmxmlhelper.h"
#include "pmmemento.h"



PMDefinePropertyClass( PMPovrayMatrix, PMPovrayMatrixProperty );

PMMetaObject* PMPovrayMatrix::s_pMetaObject = 0;
PMObject* createNewPovrayMatrix( PMPart* part )
{
   return new PMPovrayMatrix( part );
}

PMPovrayMatrix::PMPovrayMatrix( PMPart* part )
      : Base( part )
{
   m_values = PMVector( 12 );
   m_values[0] = 1.0;
   m_values[4] = 1.0;
   m_values[8] = 1.0;
}

PMPovrayMatrix::PMPovrayMatrix( const PMPovrayMatrix& m )
      : Base( m )
{
   m_values = m.m_values;
}

PMPovrayMatrix::~PMPovrayMatrix()
{
}

QString PMPovrayMatrix::description() const
{
   return ( "matrix" );
}

void PMPovrayMatrix::serialize( QDomElement& e, QDomDocument& /*doc*/ ) const
{
   e.setAttribute( "value", m_values.serializeXML() );
}

void PMPovrayMatrix::readAttributes( const PMXMLHelper& h )
{
   PMVector d = PMVector( 12 );
   d[0] = 1.0;
   d[4] = 1.0;
   d[8] = 1.0;

   m_values = h.vectorAttribute( "value", d );
   m_values.resize( 12 );
}

PMMetaObject* PMPovrayMatrix::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "PovrayMatrix", Base::metaObject(),
                                        createNewPovrayMatrix );
      s_pMetaObject->addProperty(
         new PMPovrayMatrixProperty( "values", &PMPovrayMatrix::setValues, &PMPovrayMatrix::values ) );
   }
   return s_pMetaObject;
}

void PMPovrayMatrix::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMPovrayMatrix::setValues( const PMVector& v )
{
   if( v != m_values )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMMatrixID, m_values );
         m_pMemento->setViewStructureChanged();
      }
      m_values = v;
      m_values.resize( 12 );
   }
}

PMDialogEditBase* PMPovrayMatrix::editWidget( QWidget* parent ) const
{
   return new PMPovrayMatrixEdit( parent );
}

void PMPovrayMatrix::restoreMemento( PMMemento* s )
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
            case PMMatrixID:
               setValues( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMPovrayMatrix::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

PMMatrix PMPovrayMatrix::transformationMatrix() const
{
   PMMatrix m;
   int l, c;

   for( l = 0; l < 4; l++ )
      for( c = 0; c < 3; c++ )
         m[l][c] = m_values[l*3+c];
   m[3][3] = 1.0;

   return m;
}

