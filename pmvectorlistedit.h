//-*-C++-*-
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

#ifndef PMVECTORLISTEDIT_H
#define PMVECTORLISTEDIT_H


#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QList>
#include <QVector>
#include <QEvent>
#include "pmvector.h"
#include <QItemSelectionRange>
#include <QItemSelection>
#include <QItemSelectionModel>


/**
 * Widget that displays a list of vectors, based on @ref QTable.
 */
class PMVectorListEdit : public QTableView
{
   Q_OBJECT
public:
   /**
    * Constructor for 3d vectors (x, y, z)
    */
   PMVectorListEdit( QWidget* parent );
   /**
    * Constructor for 3d vectors
    */
   PMVectorListEdit( const QString& c1, const QString& c2, const QString& c3,
                    QWidget* parent );
   /**
    * Constructor for 2d vectors
    */
   PMVectorListEdit( const QString& c1, const QString& c2,
                    QWidget* parent );

   /**
    * Constructor for 4d vectors
    */
   PMVectorListEdit( const QString& c1, const QString& c2, const QString& c3,
                     const QString& c4, QWidget* parent );
   /**
    * Sets and displays the vectors. The widget will automatically
    * resize if no link is set and resize is true.
    */
   void setVectors( const QList<PMVector>& v, bool resize = false,
                    int precision = 5 );
   /**
    * Returns the vectors
    */
   QList<PMVector> vectors() const;
   /**
    * Sets the i-th vector
    */
   void setVector( int i, const PMVector& v, int precision = 5 );
   /**
    * Returns the i-th vector
    */
   PMVector vector( int i ) const;
   /**
    * Sets the number of vectors
    */
   void setSize( int s );
   /**
    * Returns the number of vectors
    */
   int size() const { return m_size; }

   /**
    * Adds a linked point. The point p2 will be disabled and synchronized
    * with p1.
    *
    * Call this method before displaying vectors and remove the point
    * p2 from the vector list before you call @ref setVectors().
    * The list returned by @ref vectors() will not return this point.
    *
    * Set p2 to -1 to remove a link.
    */
   void setLink( int p1, int p2 );
   /**
    * Removes all links. The widget is not updated. You have to resize
    * the widget and redisplay the points.
    */
   void clearLinks();

   /**
    * Returns the selection status of vector i
    */
   bool isSelected( int i ) const;
   /**
    * Selects vector i
    */
   void select( int i );
   /**
    * Selects vector start_row to end_row
    */
   void select(int start_row, int end_row );
   /**
    * Blocks/unblocks selection updates. If block is false, the
    * selection is repainted.
    */
   void blockSelectionUpdates( bool block );

   /**
    * Returns true if the edited data is valid.
    */
   bool isDataValid();

   /** */
   virtual QSize sizeHint() const;
   /** */
   bool eventFilter( QObject* o, QEvent* e );

protected slots:
   void slotTextChanged( QStandardItem* itm );

signals:
   /**
    * Emitted when the used edits a field
    */
   void dataChanged();
   /**
    * Emitted after a right mouse button click
    */
   void showContextMenu();
private:
   void init( int dimensions );
   int m_dimension, m_size;
   QVector<int> m_links;
   QVector<bool> m_disabled;
   QStandardItemModel* m_model;
   QItemSelectionModel* qism;
   QStandardItem m_sItem;
   double d;
};


#endif
