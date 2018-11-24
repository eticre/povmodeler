/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#ifndef PMVIEWFACTORY_H
#define PMVIEWFACTORY_H




#include <QList>
#include <QHash>
//#include <k3staticdeleter.h>

class PMViewBase;
class QWidget;
class PMPart;
class PMViewOptions;
class PMViewOptionsWidget;

/**
 * Factory and description class for one view type
 * @see PMViewFactory
 */
class PMViewTypeFactory
{
public:
   /**
    * Default constructor
    */
   PMViewTypeFactory() { }
   /**
    * Destructor
    */
   virtual ~PMViewTypeFactory() { }
   /**
    * Returns the id for the view type. Choose an unique name.
    */
   virtual QString viewType() const = 0;
   /**
    * Returns a i18n'ed description for the view type
    */
   virtual QString description() const = 0;
   /**
    * Returns a i18n'ed description for the view type, dependent
    * on the options. Calls the method above by default.
    */
   //using PMViewTypeFactory::description;
   virtual QString description( PMViewOptions* ) const
   {
      return description();
   }
   /**
    * Returns the icon name for the view
    */
   virtual QString iconName() const = 0;
   /**
    * Returns a new view instance
    */
   virtual PMViewBase* newInstance( QWidget* parent, PMPart* part ) const = 0;
   /**
    * Creates a config object for the view type.
    * If the view doesn't have special attributes, the function returns 0;
    */
   virtual PMViewOptions* newOptionsInstance() const { return 0; }
   /**
    * Creates a widget to configure the custom options
    */
   virtual PMViewOptionsWidget* newOptionsWidget( QWidget*, PMViewOptions* )
   {
      return 0;
   }
};


/**
 * Factory class for KPovModeler views.
 *
 * Plugins can add new view types by adding new
 * @ref PMViewTypeFactory objects.
 */
class PMViewFactory
{
public:
   /**
    * Destructor
    */
   ~PMViewFactory();
   /**
    * Returns the factory instance
    */
   static PMViewFactory* theFactory();

   /**
    * Adds a new view type
    *
    * The factory becomes the owner of the object
    */
   void addViewType( PMViewTypeFactory* vt );
   /**
    * Returns a new view of type viewType if available
    */
   PMViewBase* newViewInstance( const QString& viewType,
                                QWidget* parent, PMPart* part ) const;
   /**
    * Returns a new view option instance for the given view type
    */
   PMViewOptions* newOptionsInstance( const QString& viewType ) const;
   /**
    * Returns the factory for the given view type
    */
   PMViewTypeFactory* viewFactory( const QString& viewType ) const;
   /**
    * Returns the list of available view types
    */
   const QList< PMViewTypeFactory* >& viewTypes() const;
private:
   /**
    * Constructor
    */
   PMViewFactory();

   QList< PMViewTypeFactory* > m_viewTypes;
   QHash<QString, PMViewTypeFactory*> m_dict;
   
   static PMViewFactory* s_pInstance;
   //static K3StaticDeleter<PMViewFactory> s_staticDeleter;
};

#endif
