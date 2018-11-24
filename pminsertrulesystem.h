//-*-C++-*-
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

#ifndef PMINSERTRULESYSTEM_H
#define PMINSERTRULESYSTEM_H



#include "pmobject.h"

#include <QStringList>
#include <QtXml/QDomDocument>

class PMInsertRuleSystem;
class PMPrototypeManager;

/**
 * Base class for all object categories (class and group)
 * for the insert rule system
 */
class PMRuleCategory
{
public:
   /**
    * Default constructor
    */
   PMRuleCategory() { }
   /**
    * Destructor
    */
   virtual ~PMRuleCategory() { }
   /**
    * Returns true if the given class types matches the category.
    */
   virtual bool matches( const QString& className ) = 0;
};

/**
 * Represents a class for the insert rule system
 */
class PMRuleClass : public PMRuleCategory
{
public:
   /**
    * Workaround to tell a created instance which
    * prototype manager to use.
    */
   static PMPrototypeManager* s_pPrototypeManager;
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleClass( QDomElement& e );
   /** */
   virtual bool matches( const QString& className );
private:
   QString m_className;
   PMPrototypeManager* m_pPrototypeManager;
};

/**
 * Groups together multiple classes or groups to form a new
 * category for the insert rule system.
 */
class PMRuleDefineGroup
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleDefineGroup( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                      QList< PMRuleDefineGroup* >& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleDefineGroup();
   /**
    * Returns true if the given class types matches the category.
    */
   virtual bool matches( const QString& className );
   /**
    * Returns the group's name
    */
   QString name() const { return m_name; }
private:
   QList< PMRuleCategory* > m_categories;
   QString m_name;
};

/**
 * Represents a group for the insert rule system
 */
class PMRuleGroup : public PMRuleCategory
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleGroup( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                QList< PMRuleDefineGroup* >& localGroups );
   /** */
   bool matches( const QString& className );
private:
   PMRuleDefineGroup* m_pGroup;
};


/**
 * Base class for all nodes for the insert rule system
 */
class PMRuleBase
{
public:
   /**
    * Default constructor.
    */
   PMRuleBase() { }
   /**
    * Destructor
    */
   virtual ~PMRuleBase();
   /**
    * Returns the node type.
    */
   virtual QString type() const = 0;
   /**
    * Calls countChildProtected for this node and all child nodes
    */
   void countChild( const QString& className, bool afterInsertPoint );
   /**
    * Calls resetProtected for this node and all child nodes
    */
   void reset();
protected:
   /**
    * Reimplement this method if the nodes value depends
    * on already inserted child objects.
    */
   virtual void countChildProtected( const QString&, bool ) { }
   /**
    * Reset all cached data (like counted child objects) here.
    */
   virtual void resetProtected() { }

   /**
    * Add all child nodes to this list.
    */
   QList< PMRuleBase* > m_children;
};

/**
 * Base class for values used by the rule system
 */
class PMRuleValue : public PMRuleBase
{
public:
   /**
    * Default constructor
    */
   PMRuleValue() : PMRuleBase() { }
   /**
    * Returns the node's value as variant.
    *
    * Reimplement this method in sub classes.
    */
   virtual PMVariant evaluate( const PMObject* o ) = 0;
};


/**
 * Value node for object properties
 */
class PMRuleProperty : public PMRuleValue
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleProperty( QDomElement& e );
   /** */
   virtual QString type() const { return QString( "Property" ); }
   /** */
   virtual PMVariant evaluate( const PMObject* o );
private:
   QString m_property;
};


/**
 * Simple constant
 */
class PMRuleConstant : public PMRuleValue
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleConstant( QDomElement& e );
   /** */
   virtual QString type() const { return QString( "Constant" ); }
   /** */
   virtual PMVariant evaluate( const PMObject* );
   /**
    * Converts the constant to the given type and returns true if
    * successful.
    */
   bool convertTo( PMVariant::PMVariantDataType type );
private:
   PMVariant m_value;
};

/**
 * Rule that counts child objects of certaint types
 */
class PMRuleCount : public PMRuleValue
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleCount( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                QList< PMRuleDefineGroup* >& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleCount();
   /** */
   virtual QString type() const { return QString( "Count" ); }
   /** */
   virtual PMVariant evaluate( const PMObject* );
protected:
   /** */
   virtual void countChildProtected( const QString& className,
                                     bool afterInsertPoint );
   /**
    * Reset all cached data (like counted child objects) here.
    */
   virtual void resetProtected();
private:
   QList< PMRuleCategory* > m_categories;
   int m_number;
};

/**
 * Base class for conditions
 */
class PMRuleCondition : public PMRuleBase
{
public:
   /**
    * Default constructor
    */
   PMRuleCondition() : PMRuleBase() { }
   /**
    * Returns the condition's value.
    *
    * Reimplement this method in sub classes.
    */
   virtual bool evaluate( const PMObject* object ) = 0;
};


/**
 * Logical negation
 */
class PMRuleNot : public PMRuleCondition
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleNot( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
              QList< PMRuleDefineGroup* >& localGroups );
   /** */
   virtual QString type() const { return QString( "Not" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
private:
   PMRuleCondition* m_pChild;
};


/**
 * Logical and
 */
class PMRuleAnd : public PMRuleCondition
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleAnd( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
              QList< PMRuleDefineGroup* >& localGroups );
   /** */
   virtual QString type() const { return QString( "And" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
private:
   QList<PMRuleCondition*> m_conditions;
};

/**
 * Logical or
 */
class PMRuleOr : public PMRuleCondition
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleOr( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
             QList< PMRuleDefineGroup* >& localGroups );
   /** */
   virtual QString type() const { return QString( "Or" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
private:
   QList<PMRuleCondition*> m_conditions;
};

/**
 * Condition. Value is true if the object already contains
 * objects of certaint classes before the insert point.
 */
class PMRuleBefore : public PMRuleCondition
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleBefore( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                 QList<PMRuleDefineGroup* >& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleBefore();
   /** */
   virtual QString type() const { return QString( "Before" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /** */
   virtual void countChildProtected( const QString& className,
                                     bool afterInsertPoint );
   /** */
   virtual void resetProtected();
private:
   QList< PMRuleCategory* > m_categories;
   bool m_contains;
};

/**
 * Condition. Value is true if the object already contains
 * objects of certaint classes after the insert point.
 */
class PMRuleAfter : public PMRuleCondition
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleAfter( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                QList<PMRuleDefineGroup* >& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleAfter();
   /** */
   virtual QString type() const { return QString( "After" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /** */
   virtual void countChildProtected( const QString& className,
                                     bool afterInsertPoint );
   /** */
   virtual void resetProtected();
private:
   QList< PMRuleCategory* > m_categories;
   bool m_contains;
};

/**
 * Condition. Value is true if the object already contains
 * objects of certaint classes.
 */
class PMRuleContains : public PMRuleCondition
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleContains( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                   QList< PMRuleDefineGroup* >& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleContains();
   /** */
   virtual QString type() const { return QString( "Contains" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /** */
   virtual void countChildProtected( const QString& className,
                                     bool afterInsertPoint );
   /** */
   virtual void resetProtected();
private:
   QList< PMRuleCategory* > m_categories;
   bool m_contains;
};

/**
 * Base class for comparisons
 */
class PMRuleCompare : public PMRuleCondition
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleCompare( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                  QList< PMRuleDefineGroup* >& localGroups );
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /**
    * Compares the two variants. The variants have the same type.
    *
    * Reimplement this method in sub classes.
    */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 ) = 0;
private:
   PMRuleValue* m_pValue[2];
};

/**
 * Less than comparison
 */
class PMRuleLess : public PMRuleCompare
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleLess( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
               QList< PMRuleDefineGroup* >& localGroups );
   /** */
   virtual QString type() const { return QString( "Less" ); }
protected:
   /** */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 );
};

/**
 * Greater than comparison
 */
class PMRuleGreater : public PMRuleCompare
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleGreater( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                  QList< PMRuleDefineGroup* >& localGroups );
   /** */
   virtual QString type() const { return QString( "Greater" ); }
protected:
   /** */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 );
};

/**
 * Equal comparison
 */
class PMRuleEqual : public PMRuleCompare
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleEqual( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
                QList< PMRuleDefineGroup* >& localGroups );
   /** */
   virtual QString type() const { return QString( "Equal" ); }
protected:
   /** */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 );
};

class PMRule : public PMRuleBase
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRule( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups,
           QList< PMRuleDefineGroup* >& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRule();
   /** */
   virtual QString type() const { return QString( "Rule" ); }
   /**
    * Returns true if this rule matches for the given class.
    */
   bool matches( const QString& className );
   /**
    * Returns the conditions value.
    */
   bool evaluate( const PMObject* parent );

private:
   QList< PMRuleCategory* > m_categories;
   PMRuleCondition* m_pCondition;
};

/**
 * Class used internally by the insert rule system
 *
 * Stores all rules for one class.
 */
class PMRuleTargetClass
{
public:
   /**
    * Reads the data from the QDomElement.
    */
   PMRuleTargetClass( QDomElement& e,
                      QList< PMRuleDefineGroup* >& globalGroups );
   /**
    * Destructor
    */
   ~PMRuleTargetClass();

   /**
    * Reads rules and groups from the QDomELement and appends
    * them to the local ones.
    */
   void appendRules( QDomElement& e, QList< PMRuleDefineGroup* >& globalGroups );
   /**
    * Returns the rules
    */
   QList< PMRule* > rules() const
   {
      return m_rules;
   }
   /**
    * Returns the class name
    */
   QString name() const { return m_class; }
   /**
    * Returns a list of exceptions for this rule.
    */
   QStringList exceptions() const { return m_exceptions; }
private:
   QList< PMRuleDefineGroup* > m_groups;
   QList< PMRule* > m_rules;
   QString m_class;
   QStringList m_exceptions;
};

/**
 * Rule based system that checks which objects can be inserted as child
 * into another object.
 *
 * The rules are defined in the file "pmbaseinsertrules.xml".
 * The file "pminsertrules.dtd" is a DTD file that can be used
 * to validate rule files.
 *
 * Plugins can add additional rules to the system.
 */
class PMInsertRuleSystem
{
public:
   /**
    * Constructor
    */
   PMInsertRuleSystem( PMPart* part );
   /**
    * Destructor
    */
   ~PMInsertRuleSystem();

   /**
    * Tells the system to load the rules from a file.
    *
    * Rules are never loaded twice for the same file. It is save
    * to call this method twice for the same file.
    */
   void loadRules( const QString& fileName );
   //void loadRulez();

   /**
    * Returns true if an object of the given class can be inserted as child
    * after the object after.
    *
    * The parser uses the third parameter for top level objects. These objects
    * have to be treated as if they are inserted after the object after.
    */
   bool canInsert( const PMObject* parentObject, const QString& className,
                   const PMObject* after, const PMObjectList* objectsBetween = 0 );

   /**
    * Returns true if the object can be inserted as child
    * after the object after.
    *
    * The parser uses the third parameter for top level objects. These objects
    * have to be treated as if they are inserted after the object after.
    *
    * Same as canInsert( parentObject, object->class(), after, objectsBetween )
    */
   bool canInsert( const PMObject* parentObject, const PMObject* object,
                   const PMObject* after, const PMObjectList* objectsBetween = 0 );

   /**
    * Returns the number of objects that can be inserted at that position
    */
   int canInsert( const PMObject* parentObject, const PMObjectList& list,
                  const PMObject* after );
   /**
    * Returns the number of objects that can be inserted at that position
    */
   int canInsert( const PMObject* parentObject, const QStringList& listOfClasses,
                  const PMObject* after );
   /**
    * Returns a pointer to the part
    */
   PMPart* part() const { return m_pPart; }

private:
   /**
    * List of all rules.
    */
   QList< PMRuleTargetClass* > m_classRules;
   /**
    * List of global groups
    */
   QList< PMRuleDefineGroup* > m_groups;
   /**
    * Dictionary that maps from the class name
    * to a list of rules that match.
    */
   QHash<QString, PMRuleTargetClass*> m_rulesDict;
   /**
    * List of already loaded files
    */
   QStringList m_loadedFiles;
   PMPart* m_pPart;
};

#endif
