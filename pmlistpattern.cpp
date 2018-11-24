/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
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


#include "pmlistpattern.h"

#include "pmxmlhelper.h"
#include "pmlistpatternedit.h"
#include "pmcompositeobject.h"
#include "pmmemento.h"
#include "pmenumproperty.h"



PMDefinePropertyClass( PMListPattern, PMListPatternProperty );
PMDefineEnumPropertyClass( PMListPattern, PMListPattern::PMListType,
                           PMListTypeProperty );

const PMVector brickSizeDefault = PMVector( 8, 6, 4.5 );
const double mortarDefault = 0.5;
const double depthDefault = 0;

PMMetaObject* PMListPattern::s_pMetaObject = nullptr;
PMMetaObject* PMColorList::s_pMetaObject = nullptr;
PMObject* createNewColorList( PMPart* part )
{
   return new PMColorList( part );
}
PMMetaObject* PMDensityList::s_pMetaObject = nullptr;
PMObject* createNewDensityList( PMPart* part )
{
   return new PMDensityList( part );
}
PMMetaObject* PMNormalList::s_pMetaObject = nullptr;
PMObject* createNewNormalList( PMPart* part )
{
   return new PMNormalList( part );
}
PMMetaObject* PMPigmentList::s_pMetaObject = nullptr;
PMObject* createNewPigmentList( PMPart* part )
{
   return new PMPigmentList( part );
}
PMMetaObject* PMTextureList::s_pMetaObject = nullptr;
PMObject* createNewTextureList( PMPart* part )
{
   return new PMTextureList( part );
}

PMListPattern::PMListPattern( PMPart* part )
      : Base( part )
{
   m_listType = ListPatternChecker;
   m_brickSize = brickSizeDefault;
   m_mortar = mortarDefault;
}

PMListPattern::PMListPattern( const PMListPattern& p )
      : Base( p )
{
   m_listType = p.m_listType;
   m_brickSize = p.m_brickSize;
   m_mortar = p.m_mortar;
}

PMListPattern::~PMListPattern()
{
}

void PMListPattern::serialize( QDomElement& e, QDomDocument& doc ) const
{
   switch( m_listType )
   {
      case ListPatternBrick:
         e.setAttribute( "listtype", "brick" );
         break;
      case ListPatternChecker:
         e.setAttribute( "listtype", "checker" );
         break;
      case ListPatternHexagon:
         e.setAttribute( "listtype", "hexagon" );
         break;
   };
   e.setAttribute( "bricksize", m_brickSize.serializeXML() );
   e.setAttribute( "mortar", m_mortar );
   Base::serialize( e, doc );
}

void PMListPattern::readAttributes( const PMXMLHelper& h )
{
   QString str = h.stringAttribute( "listtype", "checker" );
   if( str == "checker" )
      m_listType = ListPatternChecker;
   else if( str == "brick" )
      m_listType = ListPatternBrick;
   else
      m_listType = ListPatternHexagon;
   m_brickSize = h.vectorAttribute( "bricksize", brickSizeDefault );
   m_mortar = h.doubleAttribute( "mortar", mortarDefault );
   Base::readAttributes( h );
}

PMMetaObject* PMListPattern::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "ListPattern", Base::metaObject() );

      PMListTypeProperty* p = new PMListTypeProperty(
         "listType", &PMListPattern::setListType,
         &PMListPattern::listType );
      p->addEnumValue( QString( "Checker" ), ListPatternChecker );
      p->addEnumValue( QString( "Brick" ), ListPatternBrick );
      p->addEnumValue( QString( "Hexagon" ), ListPatternHexagon );
      s_pMetaObject->addProperty( p );

      s_pMetaObject->addProperty(
         new PMListPatternProperty( "brickSize", &PMListPattern::setBrickSize,
                         &PMListPattern::brickSize ) );
      s_pMetaObject->addProperty(
         new PMListPatternProperty( "mortar", &PMListPattern::setMortar,
                         &PMListPattern::mortar ) );
   }
   return s_pMetaObject;
}

void PMListPattern::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

void PMListPattern::setListType( PMListType l )
{
   if( l != m_listType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMListTypeID, m_listType );
      m_listType = l;
   }
}

void PMListPattern::setBrickSize( const PMVector& n )
{
   if( n != m_brickSize )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMBrickSizeID, m_brickSize );
      m_brickSize = n;
   }
}

void PMListPattern::setMortar( double n )
{
   if( n != m_mortar )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMMortarID, m_mortar );
      m_mortar = n;
   }
}

PMDialogEditBase* PMListPattern::editWidget( QWidget* parent ) const
{
   return new PMListPatternEdit( parent );
}

void PMListPattern::restoreMemento( PMMemento* s )
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
            case PMListTypeID:
               setListType( ( PMListType ) data->intData() );
               break;
            case PMBrickSizeID:
               setBrickSize( data->vectorData() );
               break;
            case PMMortarID:
               setMortar( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMListPattern::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}


PMTextureList::PMTextureList( PMPart* part )
      : Base( part )
{
}

PMTextureList::PMTextureList( const PMTextureList& l )
      : Base( l )
{
}

PMTextureList::~PMTextureList()
{
}

PMMetaObject* PMTextureList::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "TextureList", Base::metaObject(),
                                        createNewTextureList );
   }
   return s_pMetaObject;
}

void PMTextureList::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMTextureList::description() const
{
   return ( "texture list" );
}

PMPigmentList::PMPigmentList( PMPart* part )
      : Base( part )
{
}

PMPigmentList::PMPigmentList( const PMPigmentList& l )
      : Base( l )
{
}

PMPigmentList::~PMPigmentList()
{
}

PMMetaObject* PMPigmentList::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "PigmentList", Base::metaObject(),
                                        createNewPigmentList );
   }
   return s_pMetaObject;
}

void PMPigmentList::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMPigmentList::description() const
{
   return ( "pigment list" );
}

PMColorList::PMColorList( PMPart* part )
      : Base( part )
{
}

PMColorList::PMColorList( const PMColorList& l )
      : Base( l )
{
}

PMColorList::~PMColorList()
{
}

PMMetaObject* PMColorList::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "ColorList", Base::metaObject(),
                                        createNewColorList );
   }
   return s_pMetaObject;
}

void PMColorList::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMColorList::description() const
{
   return ( "color list" );
}

PMDensityList::PMDensityList( PMPart* part )
      : Base( part )
{
}

PMDensityList::PMDensityList( const PMDensityList& l )
      : Base( l )
{
}

PMDensityList::~PMDensityList()
{
}

PMMetaObject* PMDensityList::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "DensityList", Base::metaObject(),
                                        createNewDensityList );
   }
   return s_pMetaObject;
}

void PMDensityList::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMDensityList::description() const
{
   return ( "density list" );
}

PMDefinePropertyClass( PMNormalList, PMNormalListProperty );

PMNormalList::PMNormalList( PMPart* part )
      : Base( part )
{
   m_depth = depthDefault;
}

PMNormalList::PMNormalList( const PMNormalList& l )
      : Base( l )
{
   m_depth = depthDefault;
}

PMNormalList::~PMNormalList()
{
}

PMMetaObject* PMNormalList::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "NormalList", Base::metaObject(),
                                        createNewNormalList );
      s_pMetaObject->addProperty(
         new PMNormalListProperty( "depth", &PMNormalList::setDepth,
                                   &PMNormalList::depth ) );
   }
   return s_pMetaObject;
}

void PMNormalList::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMNormalList::description() const
{
   return ( "normal list" );
}

void PMNormalList::setDepth( double d )
{
   if( d != m_depth )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMDepthID, m_depth );
      m_depth = d;
   }
}

void PMNormalList::restoreMemento( PMMemento* s )
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
            case PMDepthID:
               setDepth( data->doubleData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMNormalList::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMNormalList::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "depth", m_depth );
   Base::serialize( e, doc );
}

void PMNormalList::readAttributes( const PMXMLHelper& h )
{
   m_depth = h.doubleAttribute( "depth", depthDefault );
   Base::readAttributes( h );
}

PMDialogEditBase* PMNormalList::editWidget( QWidget* parent ) const
{
   return new PMListPatternEdit( parent );
}

