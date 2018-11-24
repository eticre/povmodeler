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


#include "pmblob.h"

#include "pmxmlhelper.h"
#include "pmblobedit.h"
#include "pmmemento.h"



const double c_defaultThreshold = 0.5;
bool c_defaultSturm = false;
bool c_defaultHierarchy = false;

PMDefinePropertyClass( PMBlob, PMBlobProperty );

PMMetaObject* PMBlob::s_pMetaObject = 0;
PMObject* createNewBlob( PMPart* part )
{
   return new PMBlob( part );
}

PMBlob::PMBlob( PMPart* part )
      : Base( part )
{
   m_threshold = c_defaultThreshold;
   m_sturm = c_defaultSturm;
   m_hierarchy = c_defaultHierarchy;
}

PMBlob::PMBlob( const PMBlob& b )
      : Base( b )
{
   m_threshold = b.m_threshold;
   m_sturm = b.m_sturm;
   m_hierarchy = b.m_hierarchy;
}

PMBlob::~PMBlob()
{
}

QString PMBlob::description() const
{
   return ( "blob" );
}

void PMBlob::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "threshold", m_threshold );
   e.setAttribute( "sturm", m_sturm );
   e.setAttribute( "hierarchy", m_hierarchy );
   Base::serialize( e, doc );
}

void PMBlob::readAttributes( const PMXMLHelper& h )
{
   m_threshold = h.doubleAttribute( "threshold", c_defaultThreshold );
   m_sturm = h.boolAttribute( "sturm", c_defaultSturm );
   m_hierarchy = h.boolAttribute( "hierarchy", c_defaultHierarchy );
   Base::readAttributes( h );
}

PMMetaObject* PMBlob::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Blob", Base::metaObject(),
                                        createNewBlob );
      s_pMetaObject->addProperty(
         new PMBlobProperty( "threshold", &PMBlob::setThreshold, &PMBlob::threshold ) );
      s_pMetaObject->addProperty(
         new PMBlobProperty( "hierarchy", &PMBlob::setHierarchy, &PMBlob::hierarchy ) );
      s_pMetaObject->addProperty(
         new PMBlobProperty( "sturm", &PMBlob::setSturm, &PMBlob::sturm ) );
   }
   return s_pMetaObject;
}

void PMBlob::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMBlob::setThreshold( double t )
{
   if( t <= 0.0 )
   {
      qCritical(  ) << "Threshold is not positive in PMBlob::setThreshold\n";
      t = 1.0;
   }

   if( t != m_threshold )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMThresholdID, m_threshold );
      m_threshold = t;
   }
}

void PMBlob::setSturm( bool s )
{
   if( s != m_sturm )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSturmID, m_sturm );
      m_sturm = s;
   }
}

void PMBlob::setHierarchy( bool h )
{
   if( h != m_hierarchy )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHierarchyID, m_hierarchy );
      m_hierarchy = h;
   }
}

PMDialogEditBase* PMBlob::editWidget( QWidget* parent ) const
{
   return new PMBlobEdit( parent );
}

void PMBlob::restoreMemento( PMMemento* s )
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
            case PMThresholdID:
               setThreshold( data->doubleData() );
               break;
            case PMSturmID:
               setSturm( data->boolData() );
               break;
            case PMHierarchyID:
               setHierarchy( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMBlob::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
