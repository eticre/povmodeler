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


#include "pmparser.h"

#include <qstring.h>
#include <QBuffer>


#include "pmpart.h"
#include "pmdeclare.h"
#include "pmerrorflags.h"
#include "pmrecursiveobjectiterator.h"
#include "pmdebug.h"

unsigned int PMParser::s_maxErrors = 30;
unsigned int PMParser::s_maxWarnings = 50;


PMParser::PMParser( PMPart* part, QIODevice* dev )
{
   m_pPart = part;
   m_pDevice = dev;
   m_bDeviceCreated = false;
   
   init();
}

PMParser::PMParser( PMPart* part, const QByteArray& array )
{
   m_pPart = part;
   QBuffer* buffer = new QBuffer();
   buffer->setData( array.data() );
   buffer->open( QIODevice::ReadOnly );
   m_pDevice = buffer;
   m_bDeviceCreated = true;
   init();
}

void PMParser::init()
{
   m_lineNum = -1;
   m_pResultList = nullptr;
   m_errors = 0;
   m_warnings = 0;
   m_bFatalError = false;
   m_shownMessages = 0;
   m_messages.clear();
   
   m_pTopParent = nullptr;
   m_renamedObjectSymbols.clear();
   m_okDeclares.clear();
   m_pNextCheckDeclare = nullptr;
}

PMParser::~PMParser()
{
   if( m_bDeviceCreated )
      delete m_pDevice;
}

void PMParser::printMessage( const PMPMessage messageNum )
{
   if( !( m_shownMessages & messageNum ) )
   {
      m_shownMessages |= messageNum;
      
      switch( messageNum )
      {
         case PMMClockDefault:
            printWarning( ( "Using the default value of 0.0 for clock" ) );
            break;
         case PMMClockDeltaDefault:
            printWarning( ( "Using the default value of 1.0 for clock_delta" ) );
            break;
         case PMMSpecialRawComment:
            m_messages += PMMessage( ( "Note: The full povray syntax is not supported yet. "
					   "If you want to add unsupported povray code to the "
					   "scene, you can put this code between the two "
					   "special comments \"//*PMRawBegin\" and "
					   "\"//*PMRawEnd\"." ) );
            break;
      }
   }
}

void PMParser::printMessage( const QString& type, const QString& msg )
{
   if( m_lineNum >= 0 )
      m_messages += PMMessage( ( "Line : " + QString::number( m_lineNum ) )  + type + ": " + msg );
   else
      m_messages += PMMessage( type + ": " + msg );
}

void PMParser::printError( const QString& msg )
{
   if( m_errors < s_maxErrors )
   {
      printMessage( ( "Error" ), msg );
      m_errors++;
   }
   else if( m_errors == s_maxErrors )
   {
      m_messages += PMMessage( ( "Maximum of " + QString::number( s_maxErrors ) + " errors reached." ) );
      m_errors++;
   }
}

void PMParser::printWarning( const QString& msg )
{
   if( m_warnings < s_maxWarnings )
   {
      printMessage( ( "Warning" ), msg );
      m_warnings++;
   }
   else if( m_warnings == s_maxWarnings )
   {
      m_messages += PMMessage( ( "Maximum of " + QString::number( s_maxWarnings ) + " warnings reached."  ) );
      m_warnings++;
   }
}

void PMParser::printExpected( const char c, const char* sValue )
{
   printError( ( QString( c ) + " expected, found token " + QString( sValue ) + " instead." ) );
}

void PMParser::printExpected( const QString& str, const char* sValue )
{
   printError( ( str + " expected, found token " + QString( sValue ) + " instead." ) );
}

void PMParser::printUnexpected( const QString& str )
{
   printError( ( "Unexpected token " + str ) );
}

void PMParser::printInfo( const QString& msg )
{
   printMessage( ( "Info" ), msg );
}

int PMParser::errorFlags() const
{
   int result = 0;
   if( errors() )
      result |= PMEError;
   if( warnings() )
      result |= PMEWarning;
   if( fatal() )
      result |= PMEFatal;
   return result;
}


void PMParser::parse( PMObjectList* list, PMObject* parent,
                      PMObject* after )
{
   m_pResultList = list;
   m_pTopParent = parent;
   m_pAfter = after;

   // find first item, that can be a declare and can be used as link
   // for parsed objects.
   if( parent )
   {
      if( parent->type() == "Scene" )
      {
         if( after )
            m_pNextCheckDeclare = after;
         else
            m_pNextCheckDeclare = nullptr;
      }
      else
      {
         PMObject* obj = parent;
         bool stop = false;
         
         // go to parents, until the parent is the scene
         // (declares can only be inserted as top level objects)
         do
         {
            if( obj->parent() )
            {
               if( obj->parent()->type() == "Scene" )
               {
                 stop = true;
               }
               else
               {
                 obj = obj->parent();
               }
            }
            else
            {
               obj = nullptr;
               stop = true;
            }
         }
         while( obj && !stop );

         // now obj is the top level parent of the object, where parsed objects
         // will be inserted
         if( obj )
            m_pNextCheckDeclare = obj->prevSibling();
         else
            m_pNextCheckDeclare = nullptr;
      }
   }
   
   topParse();

   foreach( PMSymbol* s, m_renamedObjectSymbols ) s->setRenamedSymbol( nullptr );
   m_renamedObjectSymbols.clear();
   m_pLocalST.clear();

   if( ( errors() || warnings() ) && m_pResultList->isEmpty() )
      setFatalError();
}

bool PMParser::insertChild( PMObject* child, PMObject* parent )
{
   bool inserted = false;
   
   if( parent )
   {
      if( parent->canInsert( child, parent->lastChild() ) )
      {
         parent->appendChild( child );
         inserted = true;
      }
      else
      {
         printError( ( "Cannot insert " + child->description() + " into " +
                                          parent->description() ) );
      }
   }
   else
   {
      if( m_pTopParent )
      {
         if( m_pTopParent->canInsert( child, m_pAfter, m_pResultList ) )
         { 
               m_pResultList->append( child );
               inserted = true;
         }
         else
         {
            printError( ( "Cannot insert " + child->description() + " into " +
                                               m_pTopParent->description() ) );
         }
      }
      else
      {
           // these lines should not be executed
           // m_pTopParent may not be null
            m_pResultList->append( child );
            inserted = true;
      }
   }

   if( !inserted )
   {
      // insert error
      // remove all links
      PMRecursiveObjectIterator rit( child );
      PMDeclare* decl = nullptr;
      
      for( ; rit.current(); ++rit )
      {
         decl = rit.current()->linkedObject();
         if( decl )
            decl->removeLinkedObject( rit.current() );
      }
   }
   
   return inserted;
}

/**eticre add tree visibility**/
bool PMParser::checkID_lib( PMDeclare* decl )
{
   if(decl->id().startsWith('_'))
   {
    return true;
   }
   else
   {
    return false;
   }
}

void PMParser::checkID( PMDeclare* decl )
{
   if(decl->id().startsWith('_'))
       decl->setTreeVisibility(true);

   PMSymbolTable* st = m_pPart->symbolTable();
   PMSymbol* s = m_pLocalST.findSymbol( decl->id() );
   if( !s )
      s = st->findSymbol( decl->id() );
   
   if( s )
   {
      PMSymbol* newSym = st->findNewID( s->id() + '_', decl );
      s->setRenamedSymbol( newSym );
      // Symbol can be inserted multiple times
      // Faster than searching for s and inserting s
      // if the list does not contain it.
      m_renamedObjectSymbols.append( s );
      
      if( m_pTopParent )
         m_pLocalST.insert( decl->id(), newSym ); // paste/drop
      else
         st->insert( decl->id(), newSym );        // load file
   }
   else
   {
      s = new PMSymbol( decl->id(), decl );
      if( m_pTopParent )
         m_pLocalST.insert( decl->id(), s );      // paste/drop
      else
         st->insert( decl->id(), s );             // load file
      m_okDeclares.insert( decl->id(), true );
   }
}

void PMParser::checkID( const QString& id, const PMValue& v )
{
   PMSymbolTable* st = m_pPart->symbolTable();
   PMSymbol* s = m_pLocalST.findSymbol( id );
   
   if( s )
   {
      PMSymbol* newSym = new PMSymbol( st->findNewID( id + '_' ), v );
      s->setRenamedSymbol( newSym );
      // Symbol can be inserted multiple times
      // Faster than searching for s and inserting s
      // if the list does not contain it.
      m_renamedObjectSymbols.append( s );
      
      if( m_pTopParent )
         m_pLocalST.insert( id, newSym ); // paste/drop

      // values are never inserted into the parts symbol table
      // else
      //   st->insert( decl->id(), newSym );        // load file
   }
   else
   {
      if( m_pTopParent ) {
         s = new PMSymbol( id, v );
         m_pLocalST.insert( id, s );      // paste/drop
      }
      
      // values are never inserted into the parts symbol table
      // else
      //   st->insert( decl->id(), s );             // load file
      
      m_okDeclares.insert( id, true );
   }
}

PMDeclare* PMParser::checkLink( const QString& id )
{
   PMSymbolTable* t = m_pPart->symbolTable();
   bool ok = false;

   // is object declared?
   PMSymbol* s = m_pLocalST.findSymbol( id );
   if( !s )
      s = t->findSymbol( id );
   
   if( !s )
      printError( ( "Undefined object " + id ) );
   else if( s->type() != PMSymbol::Object )
      printError( ( "Undefined object " + id ) );
   else
   {
      // the object is declared
      // is the id already in m_okDeclares
		QHash<QString, bool>::const_iterator dit = m_okDeclares.constFind( id );
      if( dit != m_okDeclares.constEnd() )
         ok = true;
      else
      {
         // the id is not in m_okDeclares
         PMObject* obj = s->object();
         while( m_pNextCheckDeclare && !ok )
         {
            if( m_pNextCheckDeclare->isA( "Declare" ) )
            {
               PMDeclare* decl = static_cast<PMDeclare*>(m_pNextCheckDeclare);
               m_okDeclares.insert( decl->id(), true );
            }
            if( m_pNextCheckDeclare == obj )
               ok = true;

            m_pNextCheckDeclare = m_pNextCheckDeclare->prevSibling();
         }
      }

      if( !ok )
         printError( ( "Object " + id + " is undefined at that point." ) );
   }

   if( ok )
   {
      while( s->renamedSymbol() )
         s = s->renamedSymbol();

      return s->object();
   }
   
   return nullptr;
}

PMSymbol* PMParser::getSymbol( const QString& id ) const
{
   PMSymbol* s = m_pLocalST.findSymbol( id );
   if( !s )
      s = m_pPart->symbolTable()->findSymbol( id );
   return s;
}

