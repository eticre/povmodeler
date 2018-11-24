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


#include "pmtranslate.h"
#include "pmtranslateedit.h"

#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmtranslatecontrolpoint.h"



const PMVector moveDefault = PMVector( 0.0, 0.0, 0.0 );

PMDefinePropertyClass( PMTranslate, PMTranslateProperty );

PMMetaObject* PMTranslate::s_pMetaObject = nullptr;
PMObject* createNewTranslate( PMPart* part )
{
   return new PMTranslate( part );
}

PMTranslate::PMTranslate( PMPart* part )
      : Base( part )
{
}

PMTranslate::PMTranslate( const PMTranslate& t )
      : Base( t )
{
   m_move = t.m_move;
}

PMTranslate::~PMTranslate()
{
}

QString PMTranslate::description() const
{
   return ( "translate" );
}

void PMTranslate::serialize( QDomElement& e, QDomDocument& /*doc*/ ) const
{
   e.setAttribute( "value", m_move.serializeXML() );
}

void PMTranslate::readAttributes( const PMXMLHelper& h )
{
   m_move = h.vectorAttribute( "value", moveDefault );
}

PMMetaObject* PMTranslate::metaObject() const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Translate", Base::metaObject(),
                                        createNewTranslate );
      s_pMetaObject->addProperty(
         new PMTranslateProperty( "translation", &PMTranslate::setTranslation, &PMTranslate::translation ) );
   }
   return s_pMetaObject;
}

void PMTranslate::cleanUp() const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = nullptr;
   }
   Base::cleanUp();
}

void PMTranslate::setTranslation( const PMVector& p )
{
   if( p != m_move )
   {
      if( m_pMemento )
      {
         m_pMemento->addData( s_pMetaObject, PMTranslationID, m_move );
         m_pMemento->setViewStructureChanged();
      }
      m_move = p;
      m_move.resize( 3 );
   }
}

PMDialogEditBase* PMTranslate::editWidget( QWidget* parent ) const
{
   return new PMTranslateEdit( parent );
}

void PMTranslate::restoreMemento( PMMemento* s )
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
            case PMTranslationID:
               setTranslation( data->vectorData() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMTranslate::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

PMMatrix PMTranslate::transformationMatrix() const
{
   return PMMatrix::translation( m_move[0], m_move[1], m_move[2] );
}

void PMTranslate::controlPoints( PMControlPointList& list )
{
   list.append( new PMTranslateControlPoint( m_move, PMTranslationID ) );
}

void PMTranslate::controlPointsChanged( PMControlPointList& list )
{
	PMControlPointList::iterator it;

	for( it = list.begin(); it != list.end(); ++it )
   {
      if( ( *it )->changed() )
      {
         switch( ( *it )->id() )
         {
            case PMTranslationID:
               setTranslation( ( ( PMTranslateControlPoint* ) *it )->translation() );
               break;
            default:
               qCritical(  ) << "Wrong ID in PMTranslate::controlPointsChanged\n";
               break;
         }
      }
   }
}
