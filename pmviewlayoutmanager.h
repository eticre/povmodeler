//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
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

#ifndef PMVIEWLAYOUTMANAGER_H
#define PMVIEWLAYOUTMANAGER_H

#include "pmdockwidget.h"
#include "pmviewbase.h"

#include <qmap.h>
#include <QList>
#include <QDialogButtonBox>

class QDomElement;
class QLineEdit;
class PMShell;
class QObject;

/**
 * Class used internally by @ref PMViewLayout
 *
 * This class maintains all the information needed to create a docked view.
 */
class PMViewLayoutEntry
{
public:
   /**
    * Constructor
    */
   PMViewLayoutEntry();
   /**
    * Copy constructor
    */
   PMViewLayoutEntry( const PMViewLayoutEntry& e );
   /**
    * Destructor
    */
   ~PMViewLayoutEntry();

   QString viewType() const { return m_viewType; }
   void setViewType( const QString& vt );

   PMDockWidget::DockPosition dockPosition() const { return m_dockPosition; }
   void setDockPosition( PMDockWidget::DockPosition i );

   int columnWidth() const { return m_columnWidth; }
   void setColumnWidth( int i );

   int height() const { return m_height; }
   void setHeight( int i );

   int floatingHeight() const { return m_floatingHeight; }
   void setFloatingHeight( int h ) { m_floatingHeight = h; }

   int floatingWidth() const { return m_floatingWidth; }
   void setFloatingWidth( int w ) { m_floatingWidth = w; }

   int floatingPositionX() const { return m_floatingPositionX; }
   void setFloatingPositionX( int p ) { m_floatingPositionX = p; }
   int floatingPositionY() const { return m_floatingPositionY; }
   void setFloatingPositionY( int p ) { m_floatingPositionY = p; }

   void loadData( QDomElement& e );
   void saveData( QDomElement& e ) const;

   PMViewOptions* customOptions() const { return m_pCustomOptions; }
   void setCustomOptions( PMViewOptions* o );

   const QString dockPositionAsString();
   const QString viewTypeAsString();

private:
   QString m_viewType;
   PMDockWidget::DockPosition m_dockPosition;
   int m_columnWidth;
   int m_height;
   int m_floatingWidth;
   int m_floatingHeight;
   int m_floatingPositionX;
   int m_floatingPositionY;
   PMViewOptions* m_pCustomOptions;
};

/**
 * Class used internally by @ref PMViewLayoutManager
 *
 * This class maintains a named layout. It basically stores all views
 * associated with that layout.
 */
class PMViewLayout
{
public:
   typedef QList< PMViewLayoutEntry >::iterator iterator;
   /**
    * Constructor
    */
   PMViewLayout();
   /**
    * Copy constructor
    */
   PMViewLayout( const PMViewLayout& vl );
   /**
    * Destructor
    */
   ~PMViewLayout() { }

   /**
    * Assignment operator
    */
   PMViewLayout& operator = ( const PMViewLayout& vl );

   QString name() const { return m_name; }

   void loadData( QDomElement& e );

   void saveData( QDomElement& e, QDomDocument& doc ) const;
   /**
    * Destroy all dock widgets in PMShell and create the new ones
    */
   void displayLayout( PMShell* shell );
   /**
    * Sets the name of the layout
    */
   void setName( const QString& n );
   /**
    *
    * Add a new entry to the layout. By default it adds the entry at the end
    * of the list. If a position is given it adds the entry at the indicated
    * position
    */
   void addEntry( const PMViewLayoutEntry& e, int index = -1 );
   /**
    * Removes the entry at the given position
    */
   void removeEntry( int index );
   /**
    * Returns an iterator to the first entry
    */
   iterator begin() { return m_entries.begin(); }
   /**
    * Returns an iterator to the last entry
    */
   iterator end() { return m_entries.end(); }
   /**
    * Returns the entry at the given position
    */
   PMViewLayoutEntry& operator[]( int index ) { return m_entries[ index ]; }
   /**
    * Normalizes the column width and view heights
    */
   void normalize();

   /**
    * Extracts the view layout from the current window
    */
   static PMViewLayout extractViewLayout( PMShell* shell );
private:
   static void recursiveExtractColumns(
      QList< QList< PMViewLayoutEntry > >& cols,
      QList< QList< PMViewLayoutEntry > >::iterator cit,
      int width, QWidget* widget );

   static void recursiveExtractOneColumn(
      QList< PMViewLayoutEntry >& entries,
      QList< PMViewLayoutEntry >::iterator eit,
      int height, QWidget* widget );

   QString m_name;
   QList< PMViewLayoutEntry > m_entries;
};

/**
 * Singleton that contains the view layouts available.
 *
 * It interacts with @ref PMShell to create the view layouts.
 * The class maintains a list of layouts as well as the name of the default
 * layout. The layouts are stored in a XML file caled viewlayouts.xml.
 */
class PMViewLayoutManager : private QObject
{
public:
   /**
    * Destructor
    */
   ~PMViewLayoutManager();
   /**
    * Returns the manager instance (singleton)
    */
   static PMViewLayoutManager* theManager();

   /**
    * Returns the list of available view layouts
    */
   QStringList availableLayouts();
   /**
    * Sets the default layout
    */
   void setDefaultLayout( const QString& name );
   /**
    * Returns the default layout
    */
   QString defaultLayout() const { return m_defaultLayout; }
   /**
    * Destroy all dock widgets in PMShell and create the new ones
    */
   void displayLayout( const QString& name, PMShell* shell );
   /**
    * Destroy all dock widgets in PMShell and create the new ones
    */
   void displayLayout( int i, PMShell* shell );
   /**
    * Displays the layout indicated as default
    */
   void displayDefaultLayout( PMShell* shell );
   /**
    * Loads all layouts from the configuration file
    */
   void loadData();
   /**
    * Saves the current layout collection to the configuration file
    */
   void saveData();
   /**
    * Add a new empty layout
    */
   void addLayout( const QString& name );
   /**
    * Add a new layout
    */
   void addLayout( const PMViewLayout& l ) { m_layouts.append( l ); }
   /**
    * Remove a layout
    */
   void removeLayout( const QString& name );
   /**
    * Get a known layout
    */
   PMViewLayout* findLayout( const QString& name );
   /**
    * Returns the list of available layouts
    */
   QList<PMViewLayout> layouts() { return m_layouts; }
   /**
    * Sets the list of available layouts
    */
   void setLayouts( const QList<PMViewLayout>& l ) { m_layouts = l; }
   /**
    * Fill the available layouts menu
    */
   void fillPopupMenu( QMenu* pMenu );
private:
   /**
    * Constructor
    */
   PMViewLayoutManager();
   /**
    * Destroy all dock widgets in PMShell and create the new ones
    */
   void displayLayout( PMViewLayout* layout, PMShell* shell );

   bool m_layoutsLoaded;
   bool m_layoutDisplayed;
   QString m_defaultLayout;
   QList< PMViewLayout > m_layouts;

   static PMViewLayoutManager* s_pInstance;
};


class PMSaveViewLayoutDialog : public QDialogButtonBox
{
   Q_OBJECT
public:
   PMSaveViewLayoutDialog( PMShell* parent );
   ~PMSaveViewLayoutDialog();
protected slots:
   virtual void slotOk();
   void slotNameChanged( const QString& );
   void slotNameSelected( const QString& );
private:
   QLineEdit* m_pLayoutName;
   PMShell* m_pShell;
};

#endif
