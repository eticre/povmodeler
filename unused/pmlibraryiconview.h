//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMLIBRARYICONVIEW_H
#define PMLIBRARYICONVIEW_H



#include <k3iconview.h>
#include <QList>
#include <QDropEvent>

class PMLibraryHandle;

/**
 * This class is the drag and drop object for the icon view
 */
class PMLibraryIconDrag : public Q3IconDrag
{
   Q_OBJECT
public:
   /** Constructor */
   PMLibraryIconDrag( QWidget * dragSource ) : Q3IconDrag( dragSource, name ) {}

   /** @return The ith format, or NULL. */
   const char* format( int i ) const;
   /** @return The encoded payload of this object, in the specified MIME format. */
   QByteArray encodedData( const char* mime ) const;
   /** @return True if the information in e can be decoded */
   static bool canDecode( QMimeSource* e );
   /**
    * Attempts to decode the data in e
    * @return True if successful otherwise returns false
    */
   static bool decode( QMimeSource* e, QStringList& StrList, QList<bool>& subLibList );
   /** Adds a item to the object */
   void append( const Q3IconDragItem &item, const QRect &pr, const QRect &tr, const QString &path, bool subLib );

private:
   QStringList m_paths;
   QList<bool> m_subLibs;
};

/**
 * This class provides a view to browse objects, showing their previews.
 */
class PMLibraryIconView: public K3IconView
{
   Q_OBJECT
public:
   PMLibraryIconView( QWidget *parent );

   /**
    * Set the library base path
    */
   void setLibrary( PMLibraryHandle* h );
   /**
    * Returns the library in view
    */
   PMLibraryHandle* library() const { return m_pLibrary; }

public slots:
   /**
    * refresh the icon view
    */
   void refresh();

   /** called when an Item is dropped onto the view */
   void slotDropped( QDropEvent *e, const QList<Q3IconDragItem>& );

protected:
   /** @return a QDragObject for drag and drop */
   virtual Q3DragObject* dragObject();

private:
   PMLibraryHandle *m_pLibrary;
   PMLibraryHandle *m_pCurrentLibrary;
};

/**
 * This class holds a library object's icon. It also remembers the path
 * where the file is.
 */
class PMLibraryIconViewItem: public K3IconViewItem
{
public:
   PMLibraryIconViewItem( Q3IconView *parent, const QString& text, const QString& path, bool isSubLibrary );
   PMLibraryIconViewItem( Q3IconView *parent, const QString& text, const QImage& image, const QString& path, bool isSubLibrary );

   /** Get the path of the entry */
   QString path() const { return m_path; }
   /** Is the entry a sublib library? */
   bool isSubLibrary() const { return m_isSubLibrary; }
   /** Reimplement accept drop to take items */
   virtual bool acceptDrop( const QMimeSource *mime ) const;

protected:
   /** Tokes a dropped item */
   void dropped( QDropEvent *evt, const QList<Q3IconDragItem>& );
   /** Checks for the existance of oldpath and generates a new path as required */
   QString newPath( const QString oldPath, bool subLib );

private:
   QString m_path;
   bool    m_isSubLibrary;
};

#endif
