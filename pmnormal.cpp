/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmnormal.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmnormaledit.h"
#include "pmlistpattern.h"
#include "pmpattern.h"



const double bumpSizeDefault = 0.0;
const double accuracyDefault = 0.02;

PMDefinePropertyClass( PMNormal, PMNormProperty );

PMMetaObject* PMNormal::s_pMetaObject = nullptr;
PMObject* createNewNormal( PMPart* part )
{
   return new PMNormal( part );
}

PMNormal::PMNormal( PMPart* part )
      : Base( part )
{
   m_enableBumpSize = false;
   m_bumpSize = bumpSizeDefault;
   m_accuracy = accuracyDefault;
   m_uvMapping = false;
}

PMNormal::PMNormal( const PMNormal& n )
      : Base( n )
{
   m_enableBumpSize = n.m_enableBumpSize;
   m_bumpSize = n.m_bumpSize;
   m_accuracy = n.m_accuracy;
   m_uvMapping = n.m_uvMapping;
}

PMNormal::~PMNormal()
{
}

PMMetaObject* PMNormal::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Normal", Base::metaObject(),
                                        createNewNormal );
      s_pMetaObject->addProperty(
         new PMNormProperty( "bumpSize", &PMNormal::setBumpSize, &PMNormal::bumpSize ) );
      s_pMetaObject->addProperty(
         new PMNormProperty( "bumpSizeEnabled", &PMNormal::enableBumpSize, &PMNormal::isBumpSizeEnabled ) );
      s_pMetaObject->addProperty(
         new PMNormProperty( "accuracy", &PMNormal::setAccuracy, &PMNormal::accuracy ) );
      s_pMetaObject->addProperty(
         new PMNormProperty( "uvMapping", &PMNormal::setUVMapping, &PMNormal::uvMapping ) );
   }
   return s_pMetaObject;
}

void PMNormal::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMNormal::description() const
{
   return ( "normal" );
}

void PMNormal::serialize( QDomElement& e, QDomDocument& doc ) const
{
   Base::serialize( e, doc );
   e.setAttribute( "enable_bump_size", m_enableBumpSize );
   e.setAttribute( "bump_size", m_bumpSize );
   e.setAttribute( "accuracy", m_accuracy );
   e.setAttribute( "uv_mapping", m_uvMapping );
}

void PMNormal::readAttributes( const PMXMLHelper& h )
{
   Base::readAttributes( h );
   m_enableBumpSize = h.boolAttribute( "enable_bump_size", false );
   m_bumpSize = h.doubleAttribute( "bump_size", bumpSizeDefault );
   m_accuracy = h.doubleAttribute( "accuracy", accuracyDefault );
   m_uvMapping = h.boolAttribute( "uv_mapping", false );
}

PMDialogEditBase* PMNormal::editWidget( QWidget* parent ) const
{
   return new PMNormalEdit( parent );
}

void PMNormal::enableBumpSize( bool c )
{
   if( c != m_enableBumpSize )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMEnableBumpSizeID, m_enableBumpSize );
      m_enableBumpSize = c;
   }
}

void PMNormal::setBumpSize( double c )
{
   if( c != m_bumpSize )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBumpSizeID, m_bumpSize );
      m_bumpSize = c;
   }
}

void PMNormal::setAccuracy( double c )
{
   if( c!= m_accuracy )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMAccuracyID, m_accuracy );
      m_accuracy = c;
   }
}

void PMNormal::setUVMapping( bool m )
{
   if( m != m_uvMapping )
   {
      if ( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMUVMappingID, m_uvMapping );
      m_uvMapping = m;
   }
}

void PMNormal::restoreMemento( PMMemento* s )
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
            case PMEnableBumpSizeID:
               enableBumpSize( data->boolData() );
               break;
            case PMBumpSizeID:
               setBumpSize( data->doubleData() );
               break;
            case PMAccuracyID:
               setAccuracy( data->doubleData() );
               break;
            case PMUVMappingID:
               setUVMapping( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMNormal::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

