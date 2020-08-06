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

#ifndef PMVIEWBASE_H
#define PMVIEWBASE_H



#include <QWidget>
#include <QString>

class QDomElement;

/**
 * This class stores the view configuration for one view.
 *
 * Reimplement this class for each view type that has to store
 * additional values.
 *
 * Created, loaded from and saved to the xml config by the corresponding
 * @ref PMViewTypeFactory factory class.
 */
class PMViewOptions
{
public:
	virtual ~PMViewOptions(){}
   /**
    * Returns a deep copy
    */
   virtual PMViewOptions* copy() const = 0;
   /**
    * Returns the identifier for the view type. Has to be equal
    * to @ref PMViewBase::viewType for the corresponding view type.
    */
   virtual QString viewType() const = 0;
   /**
    * Loads the data from the xml element
    */
   virtual void loadData( QDomElement& e ) = 0;
   /**
    * Saves the data from to xml element
    */
   virtual void saveData( QDomElement& e ) = 0;
};

/**
 * Base class for configuration widgets for view types
 * for the layout settings dialog page
 */
class PMViewOptionsWidget : public QWidget
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMViewOptionsWidget( QWidget* parent )
         : QWidget( parent )
   {
   }

signals:
   /**
    * Emitted when the view type description has changed
    */
   void viewTypeDescriptionChanged();
};

/**
 * Interface for views.
 *
 * Each view type has to implement this interface. Handles the
 * config loading and saving
 */
class PMViewBase : public QWidget
{
	Q_OBJECT
public:
   /**
    * Default constructor
    */
    PMViewBase( QWidget* parent, QFlag f=0 )
         : QWidget( parent, f )
   {
   }
   /**
    * Returns the identifier for the view type. Has to be unique
    * for all view types.
    */
   virtual QString viewType() const = 0;
   /**
    * Returns a i18n'ed description
    */
   virtual QString description() const = 0;
   /**
    * Restores the view configuration
    *
    * Reimplement this function if the view type has to
    * restore additional values.
    * @see PMViewOptions
    */
   virtual void restoreViewConfig( PMViewOptions* ) { }
   /**
    * Saves the view configuration
    *
    * Reimplement this function if the view type has to
    * restore additional values.
    * @see PMViewOptions
    */
   virtual void saveViewConfig( PMViewOptions* ) const { }
};

#endif
