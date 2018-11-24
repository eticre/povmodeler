/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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


#include "pmtexturemapedit.h"
#include "pmtexturemap.h"

#include "pmxmlhelper.h"
#include "pmmapmemento.h"

#include <qtextstream.h>
#include <QTextStream>
#include <QList>


class PMValueProperty : public PMPropertyBase
{
public:
   PMValueProperty()
         : PMPropertyBase( "mapValues", PMVariant::Double )
   {
      m_index = 0;
   }
   virtual int dimensions() const { return 1; }
   virtual void setIndex( int /*dimension*/, int index )
   {
      m_index = index;
   }
   virtual int size( PMObject* object, int /*dimension*/ ) const
   {
      return ( ( PMTextureMapBase* ) object )->mapEntries();
   }
protected:
   virtual bool setProtected( PMObject* obj, const PMVariant& var )
   {
      PMTextureMapBase* m = ( PMTextureMapBase* ) obj;
      QList<double> list = m->mapValues();

      if( m_index >= list.count() )
      {
         qCritical(  ) << "Range error in PMTextureMapBase::ValueProperty::set" << endl;
         return false;
      }

      list[m_index] = var.doubleData();

      m->setMapValues( list );
      return true;
   }
   virtual PMVariant getProtected( const PMObject* obj )
   {
      PMTextureMapBase* m = ( PMTextureMapBase* ) obj;
      QList<double> list = m->mapValues();

      if( m_index >= list.count() )
      {
         qCritical(  ) << "Range error in PMTextureMapBase::ValueProperty::get" << endl;
         return PMVariant();
      }

      return PMVariant( list[m_index] );
   }

private:
   int m_index;
};


PMMetaObject* PMTextureMapBase::s_pMetaObject = 0;

PMTextureMapBase::PMTextureMapBase( PMPart* part )
      : Base( part )
{
}

PMTextureMapBase::PMTextureMapBase( const PMTextureMapBase& m )
      : Base( m )
{
}

PMTextureMapBase::~PMTextureMapBase()
{
}

void PMTextureMapBase::serialize( QDomElement& e, QDomDocument& doc ) const
{
   e.setAttribute( "map_values", valuesToString() );
   Base::serialize( e, doc );
}

void PMTextureMapBase::readAttributes( const PMXMLHelper& h )
{
   stringToValues( h.stringAttribute( "map_values", "" ) );
   Base::readAttributes( h );
}

QString PMTextureMapBase::valuesToString() const
{
   QString str;
   QList<double>::ConstIterator it;

   it = m_mapValues.begin();
   if( it != m_mapValues.end() )
   {
      str.setNum( *it );
      ++it;
      for( ; it != m_mapValues.end(); ++it )
         str += QString( " %1" ).arg( *it );
   }
   return str;
}

void PMTextureMapBase::stringToValues( const QString& str )
{
   m_mapValues.clear();
   QString tstr = str;
   QTextStream s( &tstr );
   double d;

   while( !s.atEnd() )
   {
      s >> d;
      m_mapValues.append( d );
   }
}

PMMetaObject* PMTextureMapBase::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "TextureMapBase", Base::metaObject() );
      s_pMetaObject->addProperty( new PMValueProperty() );
   }
   return s_pMetaObject;
}

void PMTextureMapBase::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

PMDialogEditBase* PMTextureMapBase::editWidget( QWidget* parent ) const
{
   return new PMTextureMapEdit( parent );
}

void PMTextureMapBase::restoreMemento( PMMemento* s )
{
   PMMapMemento* m = ( PMMapMemento* ) s;
   if( m->mapValuesSaved() )
   {
      if( m_pMemento )
         ( ( PMMapMemento* ) m_pMemento )->setMapValues( m_mapValues );
      m_mapValues = m->mapValues();
   }
   if( m->removedValuesSaved() )
   {
      if( m_pMemento )
         ( ( PMMapMemento* ) m_pMemento )->setRemovedValues( m_removedValues );
      m_removedValues = m->removedValues();
   }

   Base::restoreMemento( s );
}

void PMTextureMapBase::createMemento()
{
   if( m_pMemento )
      delete m_pMemento;
   m_pMemento = new PMMapMemento( this );
}

QList<double>::Iterator PMTextureMapBase::valueForChild( PMObject* obj )
{
   PMObject* o = firstChild();
   QList<double>::Iterator it = m_mapValues.begin();

   while( o && ( o != obj ) && it != m_mapValues.end() )
   {
      if( o->type() == mapType() )
         ++it;
      o = o->nextSibling();
   }
   return it;
}

double PMTextureMapBase::mapValue( const PMObject* obj ) const
{
   PMObject* o = firstChild();
   QList<double>::ConstIterator it = m_mapValues.begin();

   while( o && ( o != obj ) )
   {
      if( o->type() == mapType() )
         ++it;
      o = o->nextSibling();
   }
   return *it;
}

void PMTextureMapBase::childAdded( PMObject* ao )
{
   if( countChildren() <= m_mapValues.count() )
      return;

   if( m_pMemento )
      ( ( PMMapMemento* ) m_pMemento )->setMapValues( m_mapValues );
   if( m_removedValues.isEmpty() )
   {
      QList<double>::Iterator it = valueForChild( ao );
      if( it == m_mapValues.end() )
      {
         if( it == m_mapValues.begin() )
            m_mapValues.append( 0.0 );
         else
         {
				--it;
            double v = *it + 0.1;
            if( v > 1.0 )
               v = 1.0;
            m_mapValues.append( v );
         }
      }
      else if( it == m_mapValues.begin() )
         m_mapValues.prepend( 0.0 );
      else
      {
         double va = *it;
         double vb = *( --it );
         m_mapValues.insert( ++it, ( va + vb ) / 2.0 );
      }
   }
   else
   {
      if( m_pMemento )
         ( ( PMMapMemento* ) m_pMemento )->setRemovedValues( m_removedValues );

      QList<double>::Iterator it = m_mapValues.begin();
      bool stop = false;
      double v = m_removedValues.last();
      m_removedValues.removeLast();

      while( ( it != m_mapValues.end() ) && !stop )
      {
         if( ( *it ) > v )
            stop = true;
         else
            ++it;
      }
      m_mapValues.insert( it, v );
   }
}

bool PMTextureMapBase::takeChild( PMObject* o )
{
   if( m_pMemento )
   {
      ( ( PMMapMemento* ) m_pMemento )->setMapValues( m_mapValues );
      ( ( PMMapMemento* ) m_pMemento )->setRemovedValues( m_removedValues );
   }

   QList<double>::Iterator it = valueForChild( o );
   if( it != m_mapValues.end() )
   {
      m_removedValues.append( *it );
      m_mapValues.erase( it );
   }

   return Base::takeChild( o );
}

void PMTextureMapBase::setMapValues( const QList<double>& v )
{
   if( m_pMemento )
   {
      ( ( PMMapMemento* ) m_pMemento )->setMapValues( m_mapValues );
      ( ( PMMapMemento* ) m_pMemento )->setRemovedValues( m_removedValues );
   }
   m_removedValues.clear();
   m_mapValues = v;
}

PMObject* PMTextureMapBase::nextMapEntry( PMObject* o )
{
   bool stop = false;
   PMObject* result = o;

   do
   {
      if( result == 0 )
         result = firstChild();
      else
         result = result->nextSibling();

      if( !result )
         stop = true;
      else if( result->type() == mapType() )
         stop = true;
   }
   while( !stop );

   return result;
}


PMMetaObject* PMTextureMap::s_pMetaObject = 0;
PMObject* createNewTextureMap( PMPart* part )
{
   return new PMTextureMap( part );
}

PMTextureMap::PMTextureMap( PMPart* part )
      : Base( part )
{
}

PMTextureMap::PMTextureMap( const PMTextureMap& m )
      : Base( m )
{
}

PMTextureMap::~PMTextureMap()
{
}

PMMetaObject* PMTextureMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "TextureMap", Base::metaObject(),
                                        createNewTextureMap );
   }
   return s_pMetaObject;
}

void PMTextureMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMTextureMap::description() const
{
   return ( "texture map" );
}


PMMetaObject* PMPigmentMap::s_pMetaObject = 0;
PMObject* createNewPigmentMap( PMPart* part )
{
   return new PMPigmentMap( part );
}

PMPigmentMap::PMPigmentMap( PMPart* part )
      : Base( part )
{
}

PMPigmentMap::PMPigmentMap( const PMPigmentMap& m )
      : Base( m )
{
}

PMPigmentMap::~PMPigmentMap()
{
}

PMMetaObject* PMPigmentMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "PigmentMap", Base::metaObject(),
                                        createNewPigmentMap );
   }
   return s_pMetaObject;
}

void PMPigmentMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMPigmentMap::description() const
{
   return ( "pigment map" );
}


PMMetaObject* PMColorMap::s_pMetaObject = 0;
PMObject* createNewColorMap( PMPart* part )
{
   return new PMColorMap( part );
}

PMColorMap::PMColorMap( PMPart* part )
      : Base( part )
{
}

PMColorMap::PMColorMap( const PMColorMap& m )
      : Base( m )
{
}

PMColorMap::~PMColorMap()
{
}

PMMetaObject* PMColorMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "ColorMap", Base::metaObject(),
                                        createNewColorMap );
   }
   return s_pMetaObject;
}

void PMColorMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMColorMap::description() const
{
   return ( "color map" );
}


PMMetaObject* PMNormalMap::s_pMetaObject = 0;
PMObject* createNewNormalMap( PMPart* part )
{
   return new PMNormalMap( part );
}

PMNormalMap::PMNormalMap( PMPart* part )
      : Base( part )
{
}

PMNormalMap::PMNormalMap( const PMNormalMap& m )
      : Base( m )
{
}

PMNormalMap::~PMNormalMap()
{
}

PMMetaObject* PMNormalMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "NormalMap", Base::metaObject(),
                                        createNewNormalMap );
   }
   return s_pMetaObject;
}

void PMNormalMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMNormalMap::description() const
{
   return ( "normal map" );
}


PMMetaObject* PMSlopeMap::s_pMetaObject = 0;
PMObject* createNewSlopeMap( PMPart* part )
{
   return new PMSlopeMap( part );
}

PMSlopeMap::PMSlopeMap( PMPart* part )
      : Base( part )
{
}

PMSlopeMap::PMSlopeMap( const PMSlopeMap& m )
      : Base( m )
{
}

PMSlopeMap::~PMSlopeMap()
{
}

PMMetaObject* PMSlopeMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "SlopeMap", Base::metaObject(),
                                        createNewSlopeMap );
   }
   return s_pMetaObject;
}

void PMSlopeMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMSlopeMap::description() const
{
   return ( "slope map" );
}


PMMetaObject* PMDensityMap::s_pMetaObject = 0;
PMObject* createNewDensityMap( PMPart* part )
{
   return new PMDensityMap( part );
}

PMDensityMap::PMDensityMap( PMPart* part )
      : Base( part )
{
}

PMDensityMap::PMDensityMap( const PMDensityMap& m )
      : Base( m )
{
}

PMDensityMap::~PMDensityMap()
{
}

PMMetaObject* PMDensityMap::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "DensityMap", Base::metaObject(),
                                        createNewDensityMap );
   }
   return s_pMetaObject;
}

void PMDensityMap::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp();
}

QString PMDensityMap::description() const
{
   return ( "density map" );
}



