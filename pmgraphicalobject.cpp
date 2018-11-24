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


#include "pmgraphicalobject.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"

const bool c_defaultNoShadow = false;
const bool c_defaultNoImage = false;
const bool c_defaultNoReflection = false;
const bool c_defaultDoubleIlluminate = false;
const int c_defaultVisibilityLevel = 0;
const bool c_defaultRelativeVisibility = true;
const bool c_defaultExport = true;

PMDefinePropertyClass( PMGraphicalObject, PMGraphicalObjectProperty );

PMMetaObject* PMGraphicalObject::s_pMetaObject = nullptr;

PMGraphicalObject::PMGraphicalObject( PMPart* part )
      : Base( part )
{
   m_noShadow = c_defaultNoShadow;
   m_noImage = c_defaultNoImage;
   m_noReflection = c_defaultNoReflection;
   m_doubleIlluminate = c_defaultDoubleIlluminate;
   m_visibilityLevel = c_defaultVisibilityLevel;
   m_relativeVisibility = c_defaultRelativeVisibility;
   m_export = c_defaultExport;
}

PMGraphicalObject::PMGraphicalObject( const PMGraphicalObject& o )
      : Base( o )
{
   m_noShadow = o.m_noShadow;
   m_noImage = o.m_noImage;
   m_noReflection = o.m_noReflection;
   m_doubleIlluminate = o.m_doubleIlluminate;
   m_visibilityLevel = o.m_visibilityLevel;
   m_relativeVisibility = o.m_relativeVisibility;
   m_export = o.m_export;
}

PMGraphicalObject::~PMGraphicalObject()
{
}

PMMetaObject* PMGraphicalObject::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "GraphicalObject", Base::metaObject() );
      s_pMetaObject->addProperty(
         new PMGraphicalObjectProperty( "noShadow", &PMGraphicalObject::setNoShadow,
                         &PMGraphicalObject::noShadow ) );
      s_pMetaObject->addProperty(
         new PMGraphicalObjectProperty( "noImage", &PMGraphicalObject::setNoImage,
                         &PMGraphicalObject::noImage ) );
      s_pMetaObject->addProperty(
         new PMGraphicalObjectProperty( "noReflection", &PMGraphicalObject::setNoReflection,
                         &PMGraphicalObject::noReflection ) );
      s_pMetaObject->addProperty(
         new PMGraphicalObjectProperty( "doubleIlluminate", &PMGraphicalObject::setDoubleIlluminate,
                         &PMGraphicalObject::doubleIlluminate ) );
      s_pMetaObject->addProperty(
         new PMGraphicalObjectProperty( "visibilityLevel", &PMGraphicalObject::setVisibilityLevel,
                         &PMGraphicalObject::visibilityLevel ) );
      s_pMetaObject->addProperty(
         new PMGraphicalObjectProperty( "relativeVisibilityLevel", &PMGraphicalObject::setVisibilityLevelRelative,
                         &PMGraphicalObject::isVisibilityLevelRelative ) );
      s_pMetaObject->addProperty(
         new PMGraphicalObjectProperty( "export", &PMGraphicalObject::setExportPovray,
                         &PMGraphicalObject::exportPovray ) );
   }
   return s_pMetaObject;
}

void PMGraphicalObject::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMGraphicalObject::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "no_shadow", m_noShadow );
   e.setAttribute( "no_image", m_noImage );
   e.setAttribute( "no_reflection", m_noReflection );
   e.setAttribute( "double_illuminate", m_doubleIlluminate );
   e.setAttribute( "visibility_level", m_visibilityLevel );
   e.setAttribute( "relative_visibility", m_relativeVisibility );
   e.setAttribute( "export", m_export );
   Base::serialize( e, doc );
}

void PMGraphicalObject::readAttributes( const PMXMLHelper& h )
{
   m_noShadow = h.boolAttribute( "no_shadow", c_defaultNoShadow );
   m_noImage = h.boolAttribute( "no_image", c_defaultNoImage );
   m_noReflection = h.boolAttribute( "no_reflection", c_defaultNoReflection );
   m_doubleIlluminate = h.boolAttribute( "double_illuminate",
                                         c_defaultDoubleIlluminate );
   m_visibilityLevel = h.intAttribute( "visibility_level",
                                       c_defaultVisibilityLevel );
   m_relativeVisibility = h.boolAttribute( "relative_visibility",
                                           c_defaultRelativeVisibility );
   m_export = h.boolAttribute( "export", c_defaultExport );
   Base::readAttributes( h );
}

void PMGraphicalObject::setNoShadow( bool yes )
{
   if( m_noShadow != yes )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNoShadowID, m_noShadow );
      m_noShadow = yes;
   }
}

void PMGraphicalObject::setNoImage( bool yes )
{
   if( m_noImage != yes )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNoImageID, m_noImage );
      m_noImage = yes;
   }
}

void PMGraphicalObject::setNoReflection( bool yes )
{
   if ( m_noReflection != yes )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMNoReflectionID, m_noReflection );
      m_noReflection = yes;
   }
}

void PMGraphicalObject::setDoubleIlluminate( bool yes )
{
   if( m_doubleIlluminate != yes )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDoubleIlluminateID, m_doubleIlluminate );
      m_doubleIlluminate = yes;
   }
}

void PMGraphicalObject::setVisibilityLevel( int level )
{
   if( m_visibilityLevel != level )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMVisibilityID,
                              m_visibilityLevel );
         // do not call PMCompositeObject::setViewStructureChanged because
         // the view structure has not really changed.
         // Only the scene has to be rendered.
         m_pMemento->setViewStructureChanged();
      }
      m_visibilityLevel = level;
   }
}

void PMGraphicalObject::setVisibilityLevelRelative( bool relative )
{
   if( m_relativeVisibility != relative )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMRelativeVisibilityID,
                              m_relativeVisibility );
      m_relativeVisibility = relative;
   }
}

void PMGraphicalObject::setExportPovray( bool ex )
{
   if( m_export != ex )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMExportID, m_export );
      m_export = ex;
   }
}

void PMGraphicalObject::restoreMemento( PMMemento* s )
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
            case PMNoShadowID:
               setNoShadow( data->boolData() );
               break;
            case PMNoImageID:
               setNoImage( data->boolData() );
               break;
            case PMNoReflectionID:
               setNoReflection( data->boolData() );
               break;
            case PMDoubleIlluminateID:
               setDoubleIlluminate( data->boolData() );
               break;
            case PMVisibilityID:
               setVisibilityLevel( data->intData() );
               break;
            case PMRelativeVisibilityID:
               setVisibilityLevelRelative( data->boolData() );
               break;
            case PMExportID:
               setExportPovray( data->boolData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMGraphicalObject::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}
