/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002-2006 by Andreas Zehender
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

#include "pminsertrulesystem.h"
#include "pmprototypemanager.h"
#include "pmpart.h"
#include "pmvariant.h"
#include "pmdebug.h"

#include <QFile>
#include <QStandardPaths>
//Added by qt3to4:


bool isCategory( QDomElement& e )
{
   return( e.tagName() == "class" || e.tagName() == "group" );
}

PMRuleCategory* newCategory( QDomElement& e,
                             QList< PMRuleDefineGroup* >& globalGroups,
                             QList< PMRuleDefineGroup* >& localGroups )
{
   if( e.tagName() == "class" )
      return new PMRuleClass( e );
   if( e.tagName() == "group" )
      return new PMRuleGroup( e, globalGroups, localGroups );
   return 0;
}

PMPrototypeManager* PMRuleClass::s_pPrototypeManager = 0;

PMRuleClass::PMRuleClass( QDomElement& e )
      : PMRuleCategory()
{
   m_pPrototypeManager = s_pPrototypeManager;
   m_className = e.attribute( "name" );
   if( m_className.isEmpty() )
      qCritical(  ) << "RuleSystem: Invalid class name" << endl;
   if( !m_pPrototypeManager->existsClass( m_className ) && m_className != "Declib" && m_className != "Not_in_tree" )
      qCritical(  ) << "RuleSystem: Unknown class: "
                        << m_className << endl;
}

bool PMRuleClass::matches( const QString& className )
{
   return m_pPrototypeManager->isA( className, m_className );
}

PMRuleGroup::PMRuleGroup( QDomElement& e,
                          QList< PMRuleDefineGroup* >& globalGroups,
                          QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCategory()
{
   m_pGroup = 0;
   QString groupName = e.attribute( "name" );
   if( groupName.isEmpty() )
      qCritical(  ) << "RuleSystem: Invalid group name" << endl;
   // find group
   PMRuleDefineGroup* it;
   foreach( it, localGroups )
      if( it->name() == groupName && !m_pGroup )
         m_pGroup = it;
   foreach( it, globalGroups )
      if( it->name() == groupName && !m_pGroup )
         m_pGroup = it;
   if( !m_pGroup )
      qCritical(  ) << "RuleSystem: Group not defined: "
                        << groupName << endl;
}

bool PMRuleGroup::matches( const QString& className )
{
   if( m_pGroup )
      return m_pGroup->matches( className );
   return false;
}

PMRuleDefineGroup::PMRuleDefineGroup( QDomElement& e,
                         QList< PMRuleDefineGroup* >& globalGroups,
                         QList< PMRuleDefineGroup* >& localGroups )
{
   m_name = e.attribute( "name" );
   if( m_name.isEmpty() )
      qCritical(  ) << "RuleSystem: Invalid group name" << endl;

   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCategory( me ) )
            m_categories.append( newCategory( me, globalGroups, localGroups ) );
      }
      m = m.nextSibling();
   }
}

PMRuleDefineGroup::~PMRuleDefineGroup()
{
   while( !m_categories.isEmpty() )
      delete m_categories.takeFirst();
}

bool PMRuleDefineGroup::matches( const QString& className )
{
   foreach( PMRuleCategory* it, m_categories )
      if( it->matches( className ) )
         return true;
   return false;
}


bool isValue( QDomElement& e )
{
   return( e.tagName() == "property" || e.tagName() == "const" ||
           e.tagName() == "count" );
}

bool isCondition( QDomElement& e )
{
   return( e.tagName() == "not" || e.tagName() == "and" ||
           e.tagName() == "or" || e.tagName() == "before" ||
           e.tagName() == "after" || e.tagName() == "contains" ||
           e.tagName() == "greater" || e.tagName() == "less" ||
           e.tagName() == "equal" );
}

PMRuleValue* newValue( QDomElement& e,
                       QList< PMRuleDefineGroup* >& globalGroups,
                       QList< PMRuleDefineGroup* >& localGroups )
{
   if( e.tagName() == "property" )
      return new PMRuleProperty( e );
   if( e.tagName() == "const" )
      return new PMRuleConstant( e );
   if( e.tagName() == "count" )
      return new PMRuleCount( e, globalGroups, localGroups );
   return 0;
}

PMRuleCondition* newCondition( QDomElement& e,
                               QList< PMRuleDefineGroup* >& globalGroups,
                               QList< PMRuleDefineGroup* >& localGroups )
{
   if( e.tagName() == "not" )
      return new PMRuleNot( e, globalGroups, localGroups );
   if( e.tagName() == "and" )
      return new PMRuleAnd( e, globalGroups, localGroups );
   if( e.tagName() == "or" )
      return new PMRuleOr( e, globalGroups, localGroups );
   if( e.tagName() == "before" )
      return new PMRuleBefore( e, globalGroups, localGroups );
   if( e.tagName() == "after" )
      return new PMRuleAfter( e, globalGroups, localGroups );
   if( e.tagName() == "contains" )
      return new PMRuleContains( e, globalGroups, localGroups );
   if( e.tagName() == "greater" )
      return new PMRuleGreater( e, globalGroups, localGroups );
   if( e.tagName() == "less" )
      return new PMRuleLess( e, globalGroups, localGroups );
   if( e.tagName() == "equal" )
      return new PMRuleEqual( e, globalGroups, localGroups );
   return 0;
}

PMRuleBase::~PMRuleBase()
{
   while( !m_children.isEmpty() )
      delete m_children.takeFirst();
}

void PMRuleBase::countChild( const QString& className, bool afterInsertPoint )
{
   countChildProtected( className, afterInsertPoint );
   foreach( PMRuleBase* it, m_children )
      it->countChild( className, afterInsertPoint );
}

void PMRuleBase::reset()
{
   resetProtected();

   foreach( PMRuleBase* it, m_children )
      it->reset();
}

PMRuleProperty::PMRuleProperty( QDomElement& e )
      : PMRuleValue()
{
   m_property = e.attribute( "name" );
   if( m_property.isNull() )
      qCritical(  ) << "RuleSystem: Invalid property name" << endl;
}

PMVariant PMRuleProperty::evaluate( const PMObject* o )
{
   PMVariant v = o->property( m_property );
   if( v.isNull() )
      qCritical(  ) << "RuleSystem: Invalid property name: "
                        << m_property << endl;
   return v;
}


PMRuleConstant::PMRuleConstant( QDomElement& e )
      : PMRuleValue()
{
   QString v = e.attribute( "value" );
   if( v.isNull() )
      qCritical(  ) << "RuleSystem: Invalid value" << endl;

   m_value = PMVariant( v );
}

PMVariant PMRuleConstant::evaluate( const PMObject* )
{
   return m_value;
}

bool PMRuleConstant::convertTo( PMVariant::PMVariantDataType type )
{
   return m_value.convertTo( type );
}


PMRuleCount::PMRuleCount( QDomElement& e,
                          QList< PMRuleDefineGroup* >& globalGroups,
                          QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleValue()
{
   m_number = 0;
   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCategory( me ) )
            m_categories.append( newCategory( me, globalGroups, localGroups ) );
      }
      m = m.nextSibling();
   }
}

PMRuleCount::~PMRuleCount()
{
   while( !m_categories.isEmpty() )
      delete m_categories.takeFirst();
}

PMVariant PMRuleCount::evaluate( const PMObject* )
{
   return PMVariant( m_number );
}

void PMRuleCount::countChildProtected( const QString& className, bool )
{
   foreach( PMRuleCategory* it, m_categories )
      if( it->matches( className ) )
      {
      m_number++;
	 return;
      }
}

void PMRuleCount::resetProtected()
{
   m_number = 0;
}

PMRuleNot::PMRuleNot( QDomElement& e,
                      QList< PMRuleDefineGroup* >& globalGroups,
                      QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCondition()
{
   m_pChild = 0;
   QDomNode m = e.firstChild();
   while( !m.isNull() && !m_pChild )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCondition( me ) )
         {
            m_pChild = newCondition( me, globalGroups, localGroups );
            m_children.append( m_pChild );
         }
      }
      m = m.nextSibling();
   }
}

bool PMRuleNot::evaluate( const PMObject* object )
{
   if( m_pChild )
      return !m_pChild->evaluate( object );
   return true;
}

PMRuleAnd::PMRuleAnd( QDomElement& e,
                      QList< PMRuleDefineGroup* >& globalGroups,
                      QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCondition()
{
   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCondition( me ) )
         {
            PMRuleCondition* c = newCondition( me, globalGroups, localGroups );
            m_children.append( c );
            m_conditions.append( c );
         }
      }
      m = m.nextSibling();
   }
}

bool PMRuleAnd::evaluate( const PMObject* object )
{
   foreach( PMRuleCondition* it, m_conditions )
      if( !it->evaluate( object ) )
         return false;
   return true;
}

PMRuleOr::PMRuleOr( QDomElement& e,
                    QList< PMRuleDefineGroup* >& globalGroups,
                    QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCondition()
{
   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCondition( me ) )
         {
            PMRuleCondition* c = newCondition( me, globalGroups, localGroups );
            m_children.append( c );
            m_conditions.append( c );
         }
      }
      m = m.nextSibling();
   }
}

bool PMRuleOr::evaluate( const PMObject* object )
{
   foreach( PMRuleCondition* it, m_conditions )
      if( it->evaluate( object ) )
         return true;
   return false;
}

PMRuleBefore::PMRuleBefore( QDomElement& e,
                            QList< PMRuleDefineGroup* >& globalGroups,
                            QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCondition()
{
   m_contains = false;
   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCategory( me ) )
            m_categories.append( newCategory( me, globalGroups, localGroups ) );
      }
      m = m.nextSibling();
   }
}

PMRuleBefore::~PMRuleBefore()
{
   while( !m_categories.isEmpty() )
      delete m_categories.takeFirst();
}

bool PMRuleBefore::evaluate( const PMObject* )
{
   return m_contains;
}

void PMRuleBefore::countChildProtected( const QString& className,
                                        bool afterInsertPoint )
{
   if( afterInsertPoint && !m_contains )
   {
      foreach( PMRuleCategory* it, m_categories )
         if( !m_contains && it->matches( className ) )
	    m_contains = true;
   }
}

void PMRuleBefore::resetProtected()
{
   m_contains = false;
}

PMRuleAfter::PMRuleAfter( QDomElement& e,
                          QList< PMRuleDefineGroup* >& globalGroups,
                          QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCondition()
{
   m_contains = false;
   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCategory( me ) )
            m_categories.append( newCategory( me, globalGroups, localGroups ) );
      }
      m = m.nextSibling();
   }
}

PMRuleAfter::~PMRuleAfter()
{
   while( !m_categories.isEmpty() )
      delete m_categories.takeFirst();
}

bool PMRuleAfter::evaluate( const PMObject* )
{
   return m_contains;
}

void PMRuleAfter::countChildProtected( const QString& className,
                                       bool afterInsertPoint )
{
   if( !afterInsertPoint && !m_contains )
   {
      foreach( PMRuleCategory* it, m_categories )
         if( !m_contains && it->matches( className ) )
	    m_contains = true;
   }
}

void PMRuleAfter::resetProtected()
{
   m_contains = false;
}

PMRuleContains::PMRuleContains( QDomElement& e,
                                QList< PMRuleDefineGroup* >& globalGroups,
                                QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCondition()
{
   m_contains = false;
   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCategory( me ) )
            m_categories.append( newCategory( me, globalGroups, localGroups ) );
      }
      m = m.nextSibling();
   }
}

PMRuleContains::~PMRuleContains()
{
   while( !m_categories.isEmpty() )
      delete m_categories.takeFirst();
}

bool PMRuleContains::evaluate( const PMObject* )
{
   return m_contains;
}

void PMRuleContains::countChildProtected( const QString& className, bool )
{
   if( !m_contains )
   {
      foreach( PMRuleCategory* it, m_categories )
         if( !m_contains && it->matches( className ) )
	    m_contains = true;
   }
}

void PMRuleContains::resetProtected()
{
   m_contains = false;
}

PMRuleCompare::PMRuleCompare( QDomElement& e,
                              QList< PMRuleDefineGroup* >& globalGroups,
                              QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCondition()
{
   m_pValue[0] = 0;
   m_pValue[1] = 0;

   int i = 0;
   QDomNode m = e.firstChild();
   while( !m.isNull() && !m_pValue[1] )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isValue( me ) )
         {
            m_pValue[i] = newValue( me, globalGroups, localGroups );
            m_children.append( m_pValue[i] );
            i++;
         }
      }
      m = m.nextSibling();
   }
   if( !m_pValue[1] )
      qCritical(  ) << "RuleSystem: Comparison needs two values" << endl;
}

bool PMRuleCompare::evaluate( const PMObject* object )
{
   if( !m_pValue[1] )
      return false;

   PMVariant v[2];
   v[0] = m_pValue[0]->evaluate( object );
   v[1] = m_pValue[1]->evaluate( object );

   if( v[0].isNull() || v[1].isNull() )
      return false;

   bool convertError = false;

   if( v[0].dataType() != v[1].dataType() )
   {
      if( m_pValue[1]->type() == "Constant" )
      {
         if( v[1].convertTo( v[0].dataType() ) )
            ( ( PMRuleConstant* ) m_pValue[1] )->convertTo( v[0].dataType() );
         else
            convertError = true;
      }
      else if( m_pValue[0]->type() == "Constant" )
      {
         if( v[0].convertTo( v[1].dataType() ) )
            ( ( PMRuleConstant* ) m_pValue[0] )->convertTo( v[1].dataType() );
         else
            convertError = true;
      }
      else
         convertError = true;
   }
   if( convertError )
   {
      qCritical(  ) << "RuleSystem: Types in comparison must match" << endl;
      return false;
   }

   return compare( v[0], v[1] );
}

PMRuleLess::PMRuleLess( QDomElement& e,
                        QList< PMRuleDefineGroup* >& globalGroups,
                        QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCompare( e, globalGroups, localGroups )
{
}

bool PMRuleLess::compare( const PMVariant& v1, const PMVariant& v2 )
{
   bool c = false;

   switch( v1.dataType() )
   {
      case PMVariant::Integer:
         c = v1.intData() < v2.intData();
         break;
      case PMVariant::Unsigned:
         c = v1.unsignedData() < v2.unsignedData();
         break;
      case PMVariant::Double:
         c = v1.doubleData() < v2.doubleData();
         break;
      case PMVariant::String:
         c = v1.stringData() < v2.stringData();
         break;
      case PMVariant::Bool:
         qCritical(  ) << "RuleSystem: Less: Can't compare booleans" << endl;
         break;
      case PMVariant::ThreeState:
         qCritical(  ) << "RuleSystem: Less: Can't compare ThreeStates" << endl;
         break;
      case PMVariant::Vector:
         qCritical(  ) << "RuleSystem: Less: Can't compare vectors" << endl;
         break;
      case PMVariant::Color:
         qCritical(  ) << "RuleSystem: Less: Can't compare colors" << endl;
         break;
      case PMVariant::ObjectPointer:
         qCritical(  ) << "RuleSystem: Less: Can't compare object pointers" << endl;
         break;
      case PMVariant::None:
         qCritical(  ) << "RuleSystem: Less: Value has type none" << endl;
         break;
   }
   return c;
}

PMRuleGreater::PMRuleGreater( QDomElement& e,
                              QList< PMRuleDefineGroup* >& globalGroups,
                              QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCompare( e, globalGroups, localGroups )
{
}

bool PMRuleGreater::compare( const PMVariant& v1, const PMVariant& v2 )
{
   bool c = false;

   switch( v1.dataType() )
   {
      case PMVariant::Integer:
         c = v1.intData() > v2.intData();
         break;
      case PMVariant::Unsigned:
         c = v1.unsignedData() > v2.unsignedData();
         break;
      case PMVariant::Double:
         c = v1.doubleData() > v2.doubleData();
         break;
      case PMVariant::String:
         c = v1.stringData() > v2.stringData();
         break;
      case PMVariant::Bool:
         qCritical(  ) << "RuleSystem: Greater: Can't compare booleans" << endl;
         break;
      case PMVariant::ThreeState:
         qCritical(  ) << "RuleSystem: Greater: Can't compare ThreeStates" << endl;
         break;
      case PMVariant::Vector:
         qCritical(  ) << "RuleSystem: Greater: Can't compare vectors" << endl;
         break;
      case PMVariant::Color:
         qCritical(  ) << "RuleSystem: Greater: Can't compare colors" << endl;
         break;
      case PMVariant::ObjectPointer:
         qCritical(  ) << "RuleSystem: Greater: Can't compare object pointers" << endl;
         break;
      case PMVariant::None:
         qCritical(  ) << "RuleSystem: Greater: Value has type none" << endl;
         break;
   }
   return c;
}

PMRuleEqual::PMRuleEqual( QDomElement& e,
                          QList< PMRuleDefineGroup* >& globalGroups,
                          QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleCompare( e, globalGroups, localGroups )
{
}

bool PMRuleEqual::compare( const PMVariant& v1, const PMVariant& v2 )
{
   bool c = false;

   switch( v1.dataType() )
   {
      case PMVariant::Integer:
         c = v1.intData() == v2.intData();
         break;
      case PMVariant::Unsigned:
         c = v1.unsignedData() == v2.unsignedData();
         break;
      case PMVariant::Double:
         c = v1.doubleData() == v2.doubleData();
         break;
      case PMVariant::String:
         c = v1.stringData() == v2.stringData();
         break;
      case PMVariant::Bool:
         c = v1.boolData() == v2.boolData();
         break;
      case PMVariant::ThreeState:
         c = v1.threeStateData() == v2.threeStateData();
         break;
      case PMVariant::Vector:
         qCritical(  ) << "RuleSystem: Equal: Can't compare vectors" << endl;
         break;
      case PMVariant::Color:
         qCritical(  ) << "RuleSystem: Equal: Can't compare colors" << endl;
         break;
      case PMVariant::ObjectPointer:
         qCritical(  ) << "RuleSystem: Equal: Can't compare object pointers" << endl;
         break;
      case PMVariant::None:
         qCritical(  ) << "RuleSystem: Equal: Value has type none" << endl;
         break;
   }
   return c;
}


PMRule::PMRule( QDomElement& e,
                QList< PMRuleDefineGroup* >& globalGroups,
                QList< PMRuleDefineGroup* >& localGroups )
      : PMRuleBase()
{
   m_pCondition = 0;

   QDomNode m = e.firstChild();
   while( !m.isNull() && !m_pCondition )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( isCategory( me ) )
            m_categories.append( newCategory( me, globalGroups, localGroups ) );
         else if( isCondition( me ) )
         {
            m_pCondition = newCondition( me, globalGroups, localGroups );
            m_children.append( m_pCondition );
         }
      }
      m = m.nextSibling();
   }
}

PMRule::~PMRule()
{
   while( !m_categories.isEmpty() )
      delete m_categories.takeFirst();
}

bool PMRule::matches( const QString& className )
{
   foreach( PMRuleCategory* it, m_categories )
      if( it->matches( className ) )
         return true;
   return false;
}

bool PMRule::evaluate( const PMObject* parent )
{
   if( !m_pCondition )
      return true;
   else
      return m_pCondition->evaluate( parent );
}

PMRuleTargetClass::PMRuleTargetClass( QDomElement& e,
                                      QList< PMRuleDefineGroup* >& globalGroups )
{
   m_class = e.attribute( "name" );
   if( m_class.isEmpty() )
      qCritical(  ) << "RuleSystem: Invalid class name" << endl;

   appendRules( e, globalGroups );
}

void PMRuleTargetClass::appendRules( QDomElement& e,
                                     QList< PMRuleDefineGroup* >& globalGroups )
{
   QDomNode m = e.firstChild();
   while( !m.isNull() )
   {
      if( m.isElement() )
      {
         QDomElement me = m.toElement();
         if( me.tagName() == "definegroup" )
            m_groups.append( new PMRuleDefineGroup( me, globalGroups, m_groups ) );
         if( me.tagName() == "rule" )
            m_rules.append( new PMRule( me, globalGroups, m_groups ) );
         if( me.tagName() == "exception" )
            m_exceptions.append( me.attribute( "class" ) );
      }
      m = m.nextSibling();
   }
}

PMRuleTargetClass::~PMRuleTargetClass()
{
   while( !m_groups.isEmpty() )
      delete m_groups.takeFirst();
   while( !m_rules.isEmpty() )
      delete m_rules.takeFirst();
}

PMInsertRuleSystem::PMInsertRuleSystem( PMPart* part )
{
   m_pPart = part;
}

PMInsertRuleSystem::~PMInsertRuleSystem()
{
   while( !m_groups.isEmpty() )
      delete m_groups.takeFirst();
   while( !m_classRules.isEmpty() )
      delete m_classRules.takeFirst();
}

void PMInsertRuleSystem::loadRules( const QString& fileName )
{
   PMRuleClass::s_pPrototypeManager = m_pPart->prototypeManager();
   if( m_loadedFiles.indexOf( fileName ) >= 0 )
      return;
   m_loadedFiles.push_back( fileName );


   QString ruleFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString( "povmodeler/" + fileName ) );
   if( ruleFile.isEmpty() )
   {
      ruleFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString( "./" + fileName ) );
      if( ruleFile.isEmpty() )
      {
         qCritical(  ) << "Rule file 'povmodeler/" << fileName
                           << "' not found." << endl;
         return;
      }
   }

   QFile file( ruleFile );
   if( !file.open( QIODevice::ReadOnly ) )
   {
      qCritical(  ) << "Could not open rule file 'povmodeler/" << fileName
                        << "'" << endl;
      return;
   }

   QDomDocument doc( "insertrules" );
   doc.setContent( &file );

   QDomElement e = doc.documentElement();
   if( e.attribute( "format" ) != "1.0" )
      qCritical(  ) << "Rule format " << e.attribute( "format" )
                        << " not supported." << endl;
   else
   {
      QDomNode c = e.firstChild();
      QList< PMRuleDefineGroup* > dummyLocalGroups;

      while( !c.isNull() )
      {
         if( c.isElement() )
         {
            QDomElement ce = c.toElement();
            if( ce.tagName() == "definegroup" )
               m_groups.append( new PMRuleDefineGroup( ce, m_groups,
                                                       dummyLocalGroups ) );
            else if( ce.tagName() == "targetclass" )
            {
               QString className = ce.attribute( "name" );
               // find a target class with the same name
               PMRuleTargetClass* target = 0;

               if( !m_rulesDict.isEmpty() )
                  //target = m_rulesDict.find( className );
                  target = m_rulesDict.value( className );

               if( target )
                  target->appendRules( ce, m_groups );
               else
               {
                  target = new PMRuleTargetClass( ce, m_groups );
                  m_rulesDict.insert( className, target );
                  m_classRules.append( target );
               }
            }
         }
         c = c.nextSibling();
      }
   }
   file.close();

   PMRuleClass::s_pPrototypeManager = 0;
}

bool PMInsertRuleSystem::canInsert( const PMObject* parentObject,
                                    const QString& className,
                                    const PMObject* after,
                                    const PMObjectList* objectsBetween )
{
   bool possible = false;

   // find rules for target class
   PMMetaObject* meta = parentObject->metaObject();
   for( ; meta && !possible; meta = meta->superClass() )
   {
      PMRuleTargetClass* tc = m_rulesDict.value( meta->className() );
      if( tc )
      {
         // check the exception list
         QStringList exceptions = tc->exceptions();
         bool exceptionFound = false;
	 foreach( const QString& it, exceptions )
	    if( !exceptionFound && parentObject->isA( it ) )
               exceptionFound = true;

         if( !exceptionFound )
         {
            // find matching rules for class name
	    foreach( PMRule* rule, tc->rules() )
            {
	       if( possible )
	          break;
               if( rule->matches( className ) )
               {
                  // matching rule found
                  // reset the rule
                  rule->reset();

                  // count already inserted child objects
                  bool afterInsertPoint = false;
                  PMObject* o = parentObject->firstChild();
                  if( !after )
                     afterInsertPoint = true;
                  for( ; o; o = o->nextSibling() )
                  {
                     rule->countChild( o->className(), afterInsertPoint );
                     if( o == after )
                        afterInsertPoint = true;
                  }
                  if( objectsBetween )
                  {
                      foreach( PMObject* bit, *objectsBetween )
                        rule->countChild( bit->type(), false );
                  }

                  // evaluate condition value
                  possible = rule->evaluate( parentObject );
               }
            }
         }
      }
   }

   return possible;
}

bool PMInsertRuleSystem::canInsert( const PMObject* parentObject,
                                    const PMObject* object,
                                    const PMObject* after,
                                    const PMObjectList* objectsBetween )
{
   return canInsert( parentObject, object->type(), after, objectsBetween );
}

int PMInsertRuleSystem::canInsert( const PMObject* parentObject,
                                   const PMObjectList& list,
                                   const PMObject* after )
{
   QStringList classes;
   foreach( PMObject* it, list )
      classes.append( it->type() );
   return canInsert( parentObject, classes, after );
}

int PMInsertRuleSystem::canInsert( const PMObject* parentObject,
                                   const QStringList& list,
                                   const PMObject* after )
{
   if( list.size() == 1 )
   {
      // more efficient
      if( canInsert( parentObject, list.first(), after ) )
         return 1;
      else
         return 0;
   }

   // find rules for target class
   QList< PMRuleTargetClass* > targetClassList;
   PMMetaObject* meta = parentObject->metaObject();
   for( ; meta; meta = meta->superClass() )
   {
      PMRuleTargetClass* tc = m_rulesDict.value( meta->className() );
      if( tc )
         targetClassList.append( tc );
   }
   if( targetClassList.isEmpty() )
      return 0; // not rules found

   // count already inserted children
   PMRuleTargetClass* tit;
   PMRule* rit;
   foreach( tit, targetClassList )
   {
      foreach( rit, tit->rules() )
      {
         rit->reset();
         bool afterInsertPoint = false;
         PMObject* o = parentObject->firstChild();
         if( !after )
            afterInsertPoint = true;
         for( ; o; o = o->nextSibling() )
         {
            rit->countChild( o->className(), afterInsertPoint );
            if( o == after )
               afterInsertPoint = true;
         }
      }
   }

   int number = 0;
   foreach( const QString& oit, list )
   {
      bool possible = false;
      QList< PMRuleTargetClass* >::Iterator cit;
      for( cit = targetClassList.begin(); cit != targetClassList.end() && !possible; ++cit )
      {
         tit = *cit;
         foreach( rit, tit->rules() )
         {
            if( possible )
               break;
            if( rit->matches( oit ) )
               possible = rit->evaluate( parentObject );
         }
      }
      if( possible )
      {
         // object can be inserted, count it
         for( ; cit != targetClassList.end(); ++cit )
         {
	    tit = *cit;
	    foreach( rit, tit->rules() )
	       rit->countChild( oit, false );
         }
			number++;
      }
   }

   return number;
}
