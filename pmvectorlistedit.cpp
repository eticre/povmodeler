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

#include "pmvectorlistedit.h"
#include "pmdebug.h"

#include <QApplication>
#include <QPalette>
#include <QPainter>
#include <QMouseEvent>
#include <QList>
#include <QEvent>
#include <QMessageBox>



const int c_columnWidth = 65;


/*
QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

ui->tableView->setModel(model);
setEditTriggers(QAbstractItemView::NoEditTriggers);//readonly

QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));
model->setItem(0,0,firstRow)

tableView->setIndexWidget(tableView->model()->index(2, 1), new QPushButton);

number = m_model->item( r, i )->text().toDouble();

*/

PMVectorListEdit::PMVectorListEdit( QWidget* parent )
      : QTableView( parent )
{
   m_model = new QStandardItemModel( 1, 3, this );
   m_model->setHorizontalHeaderItem( 0, new QStandardItem( QString( "x" ) ) );
   m_model->setHorizontalHeaderItem( 1, new QStandardItem( QString( "y" ) ) );
   m_model->setHorizontalHeaderItem( 2, new QStandardItem( QString( "z" ) ) );
   init( 3 );
}

PMVectorListEdit::PMVectorListEdit( const QString& c1, const QString& c2,
                                    const QString& c3, const QString& c4,
                                    QWidget* parent )
      : QTableView( parent )
{
   m_model = new QStandardItemModel( 1, 4, this );
   m_model->setHorizontalHeaderItem( 0, new QStandardItem( QString( c1 ) ) );
   m_model->setHorizontalHeaderItem( 1, new QStandardItem( QString( c2 ) ) );
   m_model->setHorizontalHeaderItem( 2, new QStandardItem( QString( c3 ) ) );
   m_model->setHorizontalHeaderItem( 3, new QStandardItem( QString( c4 ) ) );
   init( 4 );
}

PMVectorListEdit::PMVectorListEdit( const QString& c1, const QString& c2,
                                    const QString& c3, QWidget* parent )
    : QTableView( parent )

{
   m_model = new QStandardItemModel( 1, 3, this );
   m_model->setHorizontalHeaderItem( 0, new QStandardItem( QString( c1 ) ) );
   m_model->setHorizontalHeaderItem( 1, new QStandardItem( QString( c2 ) ) );
   m_model->setHorizontalHeaderItem( 2, new QStandardItem( QString( c3 ) ) );
   init( 3 );
}

PMVectorListEdit::PMVectorListEdit( const QString& c1, const QString& c2,
                                    QWidget* parent )
      : QTableView( parent )
{
   m_model = new QStandardItemModel( 1, 2, this );
   m_model->setHorizontalHeaderItem( 0, new QStandardItem( QString( c1 ) ) );
   m_model->setHorizontalHeaderItem( 1, new QStandardItem( QString( c2 ) ) );
   init( 2 );
}

void PMVectorListEdit::init( int dimensions )
{
   setSelectionMode( QAbstractItemView::MultiSelection );
   setModel( m_model );
   m_dimension = dimensions;
   m_size = 0;

   connect( m_model, SIGNAL( itemChanged( QStandardItem* ) ),
                 this, SLOT( slotTextChanged( QStandardItem* ) ) );

   setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
   horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
   setEnabled( true );
}

void PMVectorListEdit::setVectors( const QList<PMVector>& l,
                                   bool resize, int prec )
{
   QList<PMVector>::ConstIterator it;
   int r, nl = 0;

   for( r = 0; r < ( signed ) m_links.size(); ++r )
      if( m_links[r] >= 0 )
         ++nl;

   if( nl == 0 && resize )
      setSize( l.size() );

   for( r = 0, it = l.begin(); it != l.end() && r < m_size; ++r )
   {
      if( m_disabled[r] )
      {

         if( !isEnabled() )
          m_model->item( r )->setEditable( false );
      }
      else
      {
         setVector( r, *it, prec );
         if( !isEnabled() )
          m_model->item( r )->setEditable( false );
         
         if( m_links[r] >= 0 )
            setVector( m_links[r], *it, prec );
         
         ++it;
      }
   }
   if( it != l.end() )
      qCritical(  ) << "Wrong size of vector list in PMVectorListEdit::setVectors" << Qt::endl;
}

QList<PMVector> PMVectorListEdit::vectors() const
{
   QList<PMVector> l;
   int i;

   for( i = 0; i < m_size; ++i )
      if( !m_disabled[i] )
         l.append( vector( i ) );

   return l;
}

void PMVectorListEdit::setVector( int r, const PMVector& v, int precision )
{
   if( r < 0 || r >= m_size )
   {
      qCritical(  ) << "Wrong vector index in PMVectorListEdit::setVector"
                        << Qt::endl;
      return;
   }

   bool sb = signalsBlocked();
   blockSignals( true );
   m_model->blockSignals( true );

   int i;
   QString str;

   for( i = 0; i < m_dimension && i <= ( signed ) v.size(); ++i )
   {
      str.setNum( v[i], 'g', precision );
      m_model->setItem( r, i, new QStandardItem( QString( str ) ) );
   }
   blockSignals( sb );
   m_model->blockSignals( sb );
}

PMVector PMVectorListEdit::vector( int r ) const
{
   PMVector v( m_dimension );

   if( r < 0 || r >= m_size )
   {
      qCritical(  ) << "Wrong vector index in PMVectorListEdit::vector"
                        << Qt::endl;
      return v;
   }

   int i;
   for( i = 0; i < m_dimension; ++i )
      v[i] = m_model->item( r, i )->text().toDouble();

   return v;
}

void PMVectorListEdit::setSize( int s )
{
   if( s < 0 || s == m_size )
      return;

   m_model->setRowCount( s );
   int i;
   QString str;

   for( i = 0; i < s; ++i )
   {
      //setRowStretchable( i, true );
      //setRowReadOnly( i, false );
      str.setNum( i + 1 );
      m_model->setVerticalHeaderItem( i, new QStandardItem( QString( str ) ) );
   }
   m_links.fill( -1, s );
   m_disabled.fill( false, s );
   m_size = s;
   resizeRowsToContents();
   if( s > 16 )
       s = 16;

   this->setFixedHeight( ( s + 1 ) * ( rowHeight( 1 ) + .5 ) );
}

void PMVectorListEdit::setLink( int p1, int p2 )
{
   if( p1 < 0 || p1 >= m_size || p2 >= m_size )
      return;

   QString str;

   // remove old link
   if( m_links[p1] >= 0 )
   {
      str.setNum( m_links[p1] + 1 );
      m_model->setVerticalHeaderItem( m_links[p1], new QStandardItem( QString( str ) ) );
      if( !isEnabled() )
         m_model->item( m_links[p1] )->setEditable( false );

      m_disabled[m_links[p1]] = false;
   }

   if( p2 >= 0 )
   {
      m_disabled[p2] = true;
      str = QString( "%1 (=%2)" ).arg( p2 + 1 ).arg( p1 + 1 );
      m_model->setVerticalHeaderItem( p2, new QStandardItem( QString( str ) ) );
      if( !isEnabled() )
         m_model->item( p2 )->setEditable( true );
   }

   m_links[p1] = p2;
}

void PMVectorListEdit::clearLinks()
{
   int i;
   for( i = 0; i < ( signed ) m_links.size(); ++i )
      if( m_links[i] >= 0 )
         setLink( i, -1 );
}

bool PMVectorListEdit::isSelected( int i ) const
{
    //if( selectionModel()->selectedRows().first().row() == i )
    //if( currentIndex().row() == i )
    if( selectionModel()->isRowSelected( i, QModelIndex() ) )
       return true;
    return false;
}

void PMVectorListEdit::select( int i )
{
   selectRow( i );
}

void PMVectorListEdit::select( int start_row, int end_row )
{
    QModelIndex itemTopL, itemBottomR;
    itemTopL = m_model->index( start_row, 0, rootIndex() );
    itemBottomR = m_model->index( end_row, m_dimension - 1, rootIndex() );
    QItemSelection qisel = QItemSelection( itemTopL, itemBottomR );
    selectionModel()->select( qisel, QItemSelectionModel::Select );
}

bool PMVectorListEdit::isDataValid()
{
   int r, i;
   bool valid = true;

   for( r = 0; r < m_size && valid; ++r )
   {
      if( !m_disabled[r] )
      {
         for( i = 0; i < m_dimension && valid; ++i )
         {
            d = m_model->item( r, i )->text().toDouble( &valid );
            if( !valid )
            {
               m_model->item( r, i )->setText( "" );
               QMessageBox::warning( this, tr( "Error" ), tr( "Please enter a valid float value." ) );
            }
         }
      }
   }
   return valid;
}

QSize PMVectorListEdit::sizeHint() const
{
   return QSize( c_columnWidth * m_dimension + frameWidth() * 2,
                 frameWidth() * 2 + horizontalHeader()->height()
                 + verticalHeader()->sizeHint().height() );
}

void PMVectorListEdit::slotTextChanged( QStandardItem *itm )
{
   emit dataChanged();
}

void PMVectorListEdit::blockSelectionUpdates( bool block )
{
   setUpdatesEnabled( !block );
   verticalHeader()->setUpdatesEnabled( !block );
   horizontalHeader()->setUpdatesEnabled( !block );
   if( !block )
   {
      //updateContents();
      verticalHeader()->update();
      horizontalHeader()->update();
   }
}

bool PMVectorListEdit::eventFilter( QObject* o, QEvent* e )
{
   if( e->type() == QEvent::Wheel && parent() )
      return QApplication::sendEvent( parent(), e );
   if( e->type() == QEvent::MouseButtonPress
       && ( ( QMouseEvent* ) e )->button() == Qt::RightButton )
   {
      bool b = QTableView::eventFilter( o, e );
      emit showContextMenu();
      return b;
   }
   return QTableView::eventFilter( o, e );
}

#include "pmvectorlistedit.moc"
