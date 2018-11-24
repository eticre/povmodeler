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


#include "pmheightfield.h"

#include "pmxmlhelper.h"
#include "pmheightfieldedit.h"
#include "pmheightfieldroam.h"
#include "pmmemento.h"
#include "pmviewstructure.h"
#include "pmenumproperty.h"
#include "pmdefaults.h"



const PMHeightField::HeightFieldType c_defaultType = PMHeightField::HFgif;
const QString c_defaultTypeText = QString( "gif" );
const QString c_defaultFileName = QString( "" );
const bool c_defaultHierarchy = true;
const bool c_defaultSmooth = false;
const double c_defaultWaterLevel = 0.0;

PMDefinePropertyClass( PMHeightField, PMHeightFieldProperty );
PMDefineEnumPropertyClass( PMHeightField, PMHeightField::HeightFieldType,
                           PMHeightFieldTypeProperty );


int PMHeightField::s_variance = c_defaultHeightFieldVariance;
int PMHeightField::s_parameterKey = 0;

PMViewStructure* PMHeightField::s_pDefaultViewStructure = 0;
PMMetaObject* PMHeightField::s_pMetaObject = 0;

PMObject* createNewHeightField( PMPart* part )
{
   return new PMHeightField( part );
}

PMHeightField::PMHeightField( PMPart* part )
      : Base( part )
{
   m_hfType = c_defaultType;
   m_fileName = c_defaultFileName;
   m_hierarchy = c_defaultHierarchy;
   m_smooth = c_defaultSmooth;
   m_waterLevel = c_defaultWaterLevel;

   m_modMap = true;
   m_pROAM = 0;
}

PMHeightField::PMHeightField( const PMHeightField& f )
      : Base( f )
{
   m_hfType = f.m_hfType;
   m_fileName = f.m_fileName;
   m_hierarchy = f.m_hierarchy;
   m_smooth = f.m_smooth;
   m_waterLevel = f.m_waterLevel;

   m_modMap = true;
   m_pROAM = 0;
}

PMHeightField::~PMHeightField()
{
   delete m_pROAM;
}

QString PMHeightField::description() const
{
   return ( "height field" );
}

void PMHeightField::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "hf_type", typeToString( m_hfType ) );
   e.setAttribute( "file_name", m_fileName );
   e.setAttribute( "hierarchy", m_hierarchy );
   e.setAttribute( "smooth", m_smooth );
   e.setAttribute( "water_level", m_waterLevel );
   Base::serialize( e, doc );
}

void PMHeightField::readAttributes( const PMXMLHelper& h )
{
   m_hfType = stringToType( h.stringAttribute( "hf_type", c_defaultTypeText ) );
   m_fileName = h.stringAttribute( "file_name", c_defaultFileName );
   m_hierarchy = h.boolAttribute( "hierarchy", c_defaultHierarchy );
   m_smooth = h.boolAttribute( "smooth", c_defaultSmooth );
   m_waterLevel = h.doubleAttribute( "water_level", c_defaultWaterLevel );
   Base::readAttributes( h );
}

PMMetaObject* PMHeightField::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "HeightField", Base::metaObject(),
                                        createNewHeightField );
      s_pMetaObject->addProperty(
         new PMHeightFieldProperty( "fileName", &PMHeightField::setFileName, &PMHeightField::fileName ) );
      s_pMetaObject->addProperty(
         new PMHeightFieldProperty( "hierarchy", &PMHeightField::setHierarchy, &PMHeightField::hierarchy ) );
      s_pMetaObject->addProperty(
         new PMHeightFieldProperty( "smooth", &PMHeightField::setSmooth, &PMHeightField::smooth ) );
      s_pMetaObject->addProperty(
         new PMHeightFieldProperty( "waterLevel", &PMHeightField::setWaterLevel, &PMHeightField::waterLevel ) );
      PMHeightFieldTypeProperty* p =
         new PMHeightFieldTypeProperty( "heightFieldType", &PMHeightField::setHeightFieldType,
                                        &PMHeightField::heightFieldType );
      p->addEnumValue( "Gif", HFgif );
      p->addEnumValue( "Tga", HFtga );
      p->addEnumValue( "Pot", HFpot );
      p->addEnumValue( "Png", HFpng );
      p->addEnumValue( "Pgm", HFpgm );
      p->addEnumValue( "Ppm", HFppm );
      p->addEnumValue( "Sys", HFsys );
      s_pMetaObject->addProperty( p );
   }
   return s_pMetaObject;
}

void PMHeightField::setHeightFieldType( PMHeightField::HeightFieldType t )
{
   if( t != m_hfType )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHeightFieldTypeID, m_hfType );
      m_hfType = t;
   }
}

void PMHeightField::setFileName( const QString& f )
{
   if( f != m_fileName )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMFileNameID, m_fileName );
      m_fileName = f;
      m_modMap = true;
      setViewStructureChanged();
   }
}

void PMHeightField::setHierarchy( bool h )
{
   if( h != m_hierarchy )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMHierarchyID, m_hierarchy );
      m_hierarchy = h;
   }
}

void PMHeightField::setSmooth( bool s )
{
   if( s != m_smooth )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMSmoothID, m_smooth );
      m_smooth = s;
   }
}

void PMHeightField::setWaterLevel( double wl )
{
   if( wl < 0.0 )
   {
      qCritical(  ) << "Water level < 0.0 in PMHeightField::setWaterLevel\n";
      wl = 0.0;
   }
   if( wl > 1.0 )
   {
      qCritical(  ) << "Water level > 1.0 in PMHeightField::setWaterLevel\n";
      wl = 1.0;
   }

   if( wl != m_waterLevel )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMWaterLevelID, m_waterLevel );
      m_waterLevel = wl;
      setViewStructureChanged();
   }
}

PMDialogEditBase* PMHeightField::editWidget( QWidget* parent ) const
{
   return new PMHeightFieldEdit( parent );
}

void PMHeightField::restoreMemento( PMMemento* s )
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
            case PMHeightFieldTypeID:
               m_hfType = ( HeightFieldType ) data->intData();
               break;
            case PMFileNameID:
               m_fileName = data->stringData();
               break;
            case PMHierarchyID:
               m_hierarchy = data->boolData();
               break;
            case PMSmoothID:
               m_smooth = data->boolData();
               break;
            case PMWaterLevelID:
               m_waterLevel = data->doubleData();
               break;
            default:
               qCritical(  ) << "Wrong ID in PMHeightField::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}


bool PMHeightField::isDefault()
{
   return ( m_waterLevel == c_defaultWaterLevel && m_fileName.isEmpty() );
}

void PMHeightField::createViewStructure()
{
   int detail = 65200 - ( ( s_variance * 163 ) * ( displayDetail() * displayDetail() ) );
   if ( m_modMap )
   {
      m_modMap = false;

      if ( m_pROAM )
      {
         delete m_pROAM;
         m_pROAM = 0;
      }

      if ( !m_fileName.isEmpty() )
      {
         m_pROAM = new PMHeightFieldROAM( m_fileName );

         if ( m_pROAM->isFailed() )
         {
            delete m_pROAM;
            m_pROAM = 0;
         }
      }
   }

   if ( m_pROAM )
   {
      m_pROAM->setDisplayDetail( detail );
      m_pROAM->setWaterLevel( m_waterLevel );
      m_pROAM->updateModel();
      roamViewStructure();
      return;
   }

   if ( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
      m_pViewStructure->lines().detach();
   }
   else
   {
      m_pViewStructure->points().resize(
            defaultViewStructure()->points().size() );
      m_pViewStructure->lines().resize(
            defaultViewStructure()->lines().size() );
   }

   PMPointArray& points = m_pViewStructure->points();

   points[4][1] = m_waterLevel;
   points[5][1] = m_waterLevel;
   points[6][1] = m_waterLevel;
   points[7][1] = m_waterLevel;
}

void PMHeightField::roamViewStructure()
{
   if ( !m_pViewStructure )
   {
      m_pViewStructure = new PMViewStructure( defaultViewStructure() );
      m_pViewStructure->points().detach();
      m_pViewStructure->lines().detach();
   }

   int x, z, i, pts;
   int size = m_pROAM->size();
   int currentLine = defaultViewStructure()->lines().size();
   int defPointsNum = defaultViewStructure()->points().size();
   double dx, dy, dz;
   double sizeM1 = size - 1.0;

   m_pViewStructure->points().resize( m_pROAM->usedPoints() + defPointsNum );
   m_pViewStructure->lines().resize( m_pROAM->numLines() + currentLine );

   PMPointArray& points = m_pViewStructure->points();
   PMLineArray& lines = m_pViewStructure->lines();

   points[4][1] = m_waterLevel;
   points[5][1] = m_waterLevel;
   points[6][1] = m_waterLevel;
   points[7][1] = m_waterLevel;

   for ( x = 0; x < size; ++x )
   {
      dx = x / sizeM1;
      for ( z = 0; z < size; ++z )
      {
         dz = z / sizeM1;
         if ( m_pROAM->usedPoint( x, z ) )
         {
            pts = m_pROAM->posPoint( x, z ) + defPointsNum;
            dy = m_pROAM->height( x, z, true ) / 65535.0;
            points[ pts ] = PMPoint( dx, dy, dz );

            for ( i = 0; m_pROAM->lineExist( x, z, i ) && i < 8; ++i )
            {
               lines[ currentLine++ ] =
                  PMLine( pts, m_pROAM->endPoint( x, z, i ) + defPointsNum );
            }
         }
      }
   }
}

PMViewStructure* PMHeightField::defaultViewStructure() const
{
   if( !s_pDefaultViewStructure )
   {
      s_pDefaultViewStructure = new PMViewStructure( 12, 16 );
      PMPointArray& points = s_pDefaultViewStructure->points();
      PMLineArray& lines = s_pDefaultViewStructure->lines();

      points[ 0] = PMPoint( 0.0, 0.0, 0.0 );
      points[ 1] = PMPoint( 1.0, 0.0, 0.0 );
      points[ 2] = PMPoint( 1.0, 0.0, 1.0 );
      points[ 3] = PMPoint( 0.0, 0.0, 1.0 );
      points[ 4] = PMPoint( 0.0, c_defaultWaterLevel, 0.0 );
      points[ 5] = PMPoint( 1.0, c_defaultWaterLevel, 0.0 );
      points[ 6] = PMPoint( 1.0, c_defaultWaterLevel, 1.0 );
      points[ 7] = PMPoint( 0.0, c_defaultWaterLevel, 1.0 );
      points[ 8] = PMPoint( 0.0, 1.0, 0.0 );
      points[ 9] = PMPoint( 1.0, 1.0, 0.0 );
      points[10] = PMPoint( 1.0, 1.0, 1.0 );
      points[11] = PMPoint( 0.0, 1.0, 1.0 );

      lines[ 0] = PMLine( 0, 1 );
      lines[ 1] = PMLine( 1, 2 );
      lines[ 2] = PMLine( 2, 3 );
      lines[ 3] = PMLine( 0, 3 );

      lines[ 4] = PMLine( 0, 8 );
      lines[ 5] = PMLine( 1, 9 );
      lines[ 6] = PMLine( 2, 10 );
      lines[ 7] = PMLine( 3, 11 );

      lines[ 8] = PMLine( 4, 5 );
      lines[ 9] = PMLine( 5, 6 );
      lines[10] = PMLine( 6, 7 );
      lines[11] = PMLine( 4, 7 );

      lines[12] = PMLine( 8, 9 );
      lines[13] = PMLine( 9, 10 );
      lines[14] = PMLine( 10, 11 );
      lines[15] = PMLine( 8, 11 );
   }
   return s_pDefaultViewStructure;
}

QString PMHeightField::typeToString( PMHeightField::HeightFieldType t )
{
   QString s;
   switch( t )
   {
      case HFgif:
         s = QString( "gif" );
         break;
      case HFtga:
         s = QString( "tga" );
         break;
      case HFpot:
         s = QString( "pot" );
         break;
      case HFpng:
         s = QString( "png" );
         break;
      case HFpgm:
         s = QString( "pgm" );
         break;
      case HFppm:
         s = QString( "ppm" );
         break;
      case HFsys:
         s = QString( "sys" );
         break;
   }
   return s;
}

PMHeightField::HeightFieldType PMHeightField::stringToType( const QString &str )
{
   HeightFieldType t = HFgif;
   if( str == "gif" )
      t = HFgif;
   else if( str == "tga" )
      t = HFtga;
   else if( str == "pot" )
      t = HFpot;
   else if( str == "png" )
      t = HFpng;
   else if( str == "pgm" )
      t = HFpgm;
   else if( str == "ppm" )
      t = HFppm;
   else if( str == "sys" )
      t = HFsys;
   return t;
}

void PMHeightField::setVariance( int v )
{
   if( v < 52 && v > 0 )
      s_variance = v;
   else
      qDebug() << "PMHeightField::setVariance: V must be less than 52 & greater than 0\n";
   s_parameterKey++;
}

void PMHeightField::cleanUp() const
{
   if( s_pDefaultViewStructure )
      delete s_pDefaultViewStructure;
   s_pDefaultViewStructure = 0;
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}
