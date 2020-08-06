/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003-2006 by Andreas Zehender
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

#include "pmlayoutsettings.h"

#include "pmlineedits.h"
#include "pmviewfactory.h"
#include "pmdebug.h"

#include <QMessageBox>
#include <QLayout>
#include <QComboBox>
#include <QListWidget>
#include <QHeaderView>
#include <QTreeWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

PMLayoutSettings::PMLayoutSettings( QWidget* parent )
      : PMSettingsDialogPage( parent )
{
   QHBoxLayout* hlayout;
   QVBoxLayout* vlayout;
   QVBoxLayout* gvl;
   QGridLayout* grid;
   QGroupBox* gb;
   QGroupBox* gbe;
   QHBoxLayout* ghe;
   QVBoxLayout* gvle;

   vlayout = new QVBoxLayout( this );
//TODO PORT QT5    vlayout->setSpacing( QDialog::spacingHint() );
   vlayout->setMargin( 0 );

   hlayout = new QHBoxLayout();
   vlayout->addLayout( hlayout );
   hlayout->addWidget( new QLabel( tr( "Default view layout:" ), this ) );
   m_pDefaultLayout = new QComboBox( this );
   hlayout->addWidget( m_pDefaultLayout, 1 );
   hlayout->addStretch( 1 );

   gb = new QGroupBox( tr( "Available View Layouts" ), this );
   vlayout->addWidget( gb );
   gvl = new QVBoxLayout( gb );
//TODO PORT QT5    gvl->setSpacing( QDialog::spacingHint() );
//TODO PORT QT5    gvl->setMargin( QDialog::marginHint() );
   gvl->addSpacing( 10 );
   
   grid = new QGridLayout();
   gvl->addLayout( grid );

   m_pViewLayouts = new QListWidget( gb );
   connect( m_pViewLayouts, SIGNAL( currentRowChanged( int ) ), 
                            SLOT( slotLayoutSelected( int ) ) );
   grid->addWidget( m_pViewLayouts, 0, 0, 3, 1 );

   m_pAddLayout = new QPushButton( tr( "Add" ), gb );
   connect( m_pAddLayout, SIGNAL( clicked() ), SLOT( slotAddLayout() ) );
   grid->addWidget( m_pAddLayout, 0, 1 );

   m_pRemoveLayout = new QPushButton( tr( "Remove" ), gb );
   connect( m_pRemoveLayout, SIGNAL( clicked() ), SLOT( slotRemoveLayout() ) );

   grid->addWidget( m_pRemoveLayout, 1, 1 );
   grid->setRowStretch( 2, 1 );

   gbe = new QGroupBox( tr( "View Layout" ), gb );
   gvl->addWidget( gbe );
   gvle = new QVBoxLayout( gbe );
//TODO PORT QT5    gvle->setSpacing( QDialog::spacingHint() );
//TODO PORT QT5    gvle->setMargin( QDialog::marginHint() );
   gvle->addSpacing( 10 );
   ghe = new QHBoxLayout();
   gvle->addLayout( ghe );
   ghe->addWidget( new QLabel( tr( "Name:" ), gbe ) );
   m_pViewLayoutName = new QLineEdit( gbe );
   connect( m_pViewLayoutName, SIGNAL( textChanged( const QString& ) ), 
                               SLOT( slotLayoutNameChanged( const QString& ) ) );
   ghe->addWidget( m_pViewLayoutName );
   grid = new QGridLayout();
   gvle->addLayout( grid );
   m_pViewEntries = new QTableView( gbe );
   m_model = new QStandardItemModel( 1, 3, this );
   m_model->setHorizontalHeaderItem( 0, new QStandardItem( QString( " " ) ) );
   m_model->setHorizontalHeaderItem( 1, new QStandardItem( QString( "Type" ) ) );
   m_model->setHorizontalHeaderItem( 2, new QStandardItem( QString( "Position" ) ) );
   m_pViewEntries->setModel( m_model );
   m_pViewEntries->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
   m_pViewEntries->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_pViewEntries->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
   m_pViewEntries->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
   m_pViewEntries->setEnabled( true );
   m_pViewEntries->setEditTriggers( QAbstractItemView::NoEditTriggers );
   m_pViewEntries->setMaximumHeight( 150 );
   m_pViewEntries->setShowGrid( false );
   m_pViewEntries->setCornerButtonEnabled( false );
   m_pViewEntries->verticalHeader()->hide();

   connect( m_pViewEntries->selectionModel(),
            SIGNAL( currentChanged( QModelIndex, QModelIndex ) ),
            SLOT( slotViewEntrySelected( QModelIndex ) ) );

   grid->addWidget( m_pViewEntries, 0, 0, 4, 1 );

   m_pAddEntry = new QPushButton( tr( "Add." ), gbe );
   connect( m_pAddEntry, SIGNAL( clicked() ),
                         SLOT( slotAddViewEntryClicked() ) );

   grid->addWidget( m_pAddEntry, 0, 1 );

   m_pRemoveEntry = new QPushButton( tr( "Remove" ), gbe );
   connect( m_pRemoveEntry, SIGNAL( clicked() ),
                            SLOT( slotRemoveViewEntryClicked() ) );

   grid->addWidget( m_pRemoveEntry, 1, 1 );
   /* //TODO
   m_pMoveUpEntry = new QPushButton( tr( "Move Up" ), gbe );
   connect( m_pMoveUpEntry, SIGNAL( clicked() ), SLOT( slotMoveUpViewEntryClicked() ) );
   grid->addWidget( m_pMoveUpEntry, 2, 1 );
   m_pMoveDownEntry = new QPushButton( tr( "Move Down" ), gbe );
   connect( m_pMoveDownEntry, SIGNAL( clicked() ), SLOT( slotMoveDownViewEntryClicked() ) );
   grid->addWidget( m_pMoveDownEntry, 3, 1 );
   */

   QHBoxLayout* ghl = new QHBoxLayout();
   gvle->addLayout( ghl );
   grid = new QGridLayout();
   ghl->addLayout( grid );
   grid->addWidget( new QLabel( tr( "Type:" ), gbe ), 0, 0 );
   grid->addWidget( new QLabel( tr( "Dock position:" ), gbe ), 2, 0 );
   m_pColumnWidthLabel = new QLabel( tr( "Column width:" ), gbe );
   grid->addWidget( m_pColumnWidthLabel, 3, 0 );
   m_pViewHeightLabel = new QLabel( tr( "View height:" ), gbe );
   grid->addWidget( m_pViewHeightLabel, 4, 0 );
   m_pViewTypeEdit = new QComboBox( gbe );

   // insert all view types
   foreach( PMViewTypeFactory* it, PMViewFactory::theFactory()->viewTypes() )
      m_pViewTypeEdit->addItem( it->description() );
   
   connect( m_pViewTypeEdit, SIGNAL( activated( int ) ),
                             SLOT( slotViewTypeChanged( int ) ) );

   grid->addWidget( m_pViewTypeEdit, 0, 1 );
   m_pDockPositionEdit = new QComboBox( gbe );
   m_pDockPositionEdit->addItem( tr( "New Column" ) );
   m_pDockPositionEdit->addItem( tr( "Below" ) );
   m_pDockPositionEdit->addItem( tr( "Tabbed" ) );
   m_pDockPositionEdit->addItem( tr( "Floating" ) );
   connect( m_pDockPositionEdit, SIGNAL( activated( int ) ),
                                 SLOT( slotDockPositionChanged( int ) ) );
   grid->addWidget( m_pDockPositionEdit, 2, 1 );
   
   m_pColumnWidthEdit = new PMIntEdit( gbe );
   connect( m_pColumnWidthEdit, SIGNAL( textChanged( const QString& ) ),
                                SLOT( slotColumnWidthChanged( const QString& ) ) );
   grid->addWidget( m_pColumnWidthEdit, 3, 1 );
   
   m_pViewHeightEdit = new PMIntEdit( gbe );
   connect( m_pViewHeightEdit, SIGNAL( textChanged( const QString& ) ),
                               SLOT( slotViewHeightChanged( const QString& ) ) );
   grid->addWidget( m_pViewHeightEdit, 4, 1 );

   m_pFloatingWidthLabel = new QLabel( tr( "Width:" ), gbe );
   m_pFloatingHeightLabel = new QLabel( tr( "Height:" ), gbe );
   m_pFloatingPosXLabel = new QLabel( tr( "Position x:" ), gbe );
   m_pFloatingPosYLabel = new QLabel( QString( "y:" ), gbe );
   grid->addWidget( m_pFloatingWidthLabel, 5, 0 );
   grid->addWidget( m_pFloatingHeightLabel, 5, 2 );
   grid->addWidget( m_pFloatingPosXLabel, 6, 0 );
   grid->addWidget( m_pFloatingPosYLabel, 6, 2 );

   m_pFloatingWidth = new PMIntEdit( gbe );
   connect( m_pFloatingWidth, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotFloatingWidthChanged( const QString& ) ) );
   m_pFloatingHeight = new PMIntEdit( gbe );
   connect( m_pFloatingHeight, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotFloatingHeightChanged( const QString& ) ) );
   m_pFloatingPosX = new PMIntEdit( gbe );
   connect( m_pFloatingPosX, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotFloatingPosXChanged( const QString& ) ) );
   m_pFloatingPosY = new PMIntEdit( gbe );
   connect( m_pFloatingPosY, SIGNAL( textChanged( const QString& ) ),
            SLOT( slotFloatingPosYChanged( const QString& ) ) );
   grid->addWidget( m_pFloatingWidth, 5, 1 );
   grid->addWidget( m_pFloatingHeight, 5, 3 );
   grid->addWidget( m_pFloatingPosX, 6, 1 );
   grid->addWidget( m_pFloatingPosY, 6, 3 );

   // create a holder widget for custom options widgets
   m_pCustomOptionsWidget = 0;
   m_pCustomOptionsHolder = new QWidget( gbe );

   QVBoxLayout* covl = new QVBoxLayout();
   ghl->addLayout( covl );
   covl->addWidget( m_pCustomOptionsHolder );
   covl->addStretch( 1 );
   
   ghl->addStretch( 1 );

   vlayout->addStretch( 1 );
}

void PMLayoutSettings::displaySettings()
{
   PMViewLayoutManager* m = PMViewLayoutManager::theManager();
   m_viewLayouts = m->layouts();
   m_currentViewLayout = m_viewLayouts.begin();
   m_defaultViewLayout = m_viewLayouts.begin();
   for( ; ( m_defaultViewLayout != m_viewLayouts.end() ) &&
          ( *m_defaultViewLayout ).name() != m->defaultLayout();
        ++m_defaultViewLayout );
   displayLayoutList();
   m_pViewLayouts->setCurrentRow( 0 );
   if( m_pViewLayouts->count() == 1 )
      m_pRemoveLayout->setEnabled( false );
}

void PMLayoutSettings::displayDefaults()
{
}

bool PMLayoutSettings::validateData()
{
   QMessageBox msgbox;
   QList<PMViewLayout>::iterator lit;
   for( lit = m_viewLayouts.begin(); lit != m_viewLayouts.end(); ++lit )
   {
      if( ( *lit ).name().isEmpty() )
      {
         emit showMe();
         msgbox.setText( tr( "View layouts may not have empty names." ) );
         msgbox.exec();
         return false;
      }
      QList<PMViewLayoutEntry>::iterator eit = ( *lit ).begin();
      if( eit != ( *lit ).end() )
      {
         if( ( *eit ).dockPosition() != PMDockWidget::DockRight )
         {
            emit showMe();
            msgbox.setText( tr( "The docking position of the first view layout entry has to be 'New Column'." ) );
            msgbox.exec();
            return false;
         }
      }
   }
   return true;
}

void PMLayoutSettings::applySettings()
{
   QList<PMViewLayout>::iterator lit;
   for( lit = m_viewLayouts.begin(); lit != m_viewLayouts.end(); ++lit )
      ( *lit ).normalize();
   PMViewLayoutManager::theManager()->setDefaultLayout( m_pDefaultLayout->currentText() );
   PMViewLayoutManager::theManager()->setLayouts( m_viewLayouts );
   PMViewLayoutManager::theManager()->saveData();
}

void PMLayoutSettings::displayLayoutList()
{
   QList<PMViewLayout>::iterator it;
   
   m_pViewLayouts->clear();
   m_pDefaultLayout->clear();
   for( it = m_viewLayouts.begin(); it != m_viewLayouts.end(); ++it )
   {
      m_pViewLayouts->addItem( ( *it ).name() );
      m_pDefaultLayout->addItem( ( *it ).name() );
      if( it == m_defaultViewLayout )
         m_pDefaultLayout->setCurrentIndex( m_pDefaultLayout->findText( ( *it ).name() ) );
   }
}

void PMLayoutSettings::slotAddLayout()
{
   QString new_name;
	
   new_name = tr( "Unnamed" );
	QList<QListWidgetItem*> items = m_pViewLayouts->findItems( new_name, Qt::MatchStartsWith );
    if(items.count() > 0) new_name = tr( "Unnamed" ) + QString::number(items.count() + 1);

   PMViewLayout l;
   l.setName( new_name );

   m_currentViewLayout++;
   m_viewLayouts.insert( m_currentViewLayout, l );
   displayLayoutList();
   m_pViewLayouts->setCurrentRow( m_pViewLayouts->count() - 1 );
   m_pRemoveLayout->setEnabled( true );
}

void PMLayoutSettings::slotRemoveLayout()
{
   if( m_currentViewLayout == m_defaultViewLayout )
   {
      m_defaultViewLayout--;
      if( m_defaultViewLayout == m_viewLayouts.end() )
      {
         m_defaultViewLayout++;
         m_defaultViewLayout++;
      }
   }
   m_viewLayouts.erase( m_currentViewLayout );
   displayLayoutList();
   m_pViewLayouts->setCurrentRow( 0 );

   if( m_pViewLayouts->count() == 1 )
      m_pRemoveLayout->setEnabled( false );
}

void PMLayoutSettings::slotLayoutSelected( int layoutIndex )
{
  int i;
  QString str;
  bool sb;

  if( layoutIndex < 0 ) return;

  m_currentLayoutIndex = layoutIndex;

  m_currentViewLayout = m_viewLayouts.begin() + layoutIndex;
  m_currentViewEntry = ( *m_currentViewLayout ).begin();

  sb = m_pViewLayoutName->signalsBlocked();
  m_pViewLayoutName->blockSignals( true );
  m_pViewLayoutName->setText( ( *m_currentViewLayout ).name() );
  m_pViewLayoutName->blockSignals( sb );
  
  PMViewLayout::iterator it;
  QList< QStandardItem* > items;
  m_model->setRowCount( 0 );
  i = 0;
  for( it = ( *m_currentViewLayout ).begin();
       it != ( *m_currentViewLayout ).end(); ++it )
  {
     i++;
     str.setNum( i );//col one
     items.append( new QStandardItem( str ) );
     items.append( new QStandardItem( ( *it ).viewTypeAsString() ) );
     items.append( new QStandardItem( ( *it ).dockPositionAsString() ) );
     m_model->appendRow( items );
     items.clear();
     if( i == 1 )
        m_pViewEntries->selectRow( 0 );// previous, true );
  }
  if( i == 0 )
      slotViewEntrySelected( m_pViewEntries->currentIndex() );

  m_pViewEntries->resizeColumnsToContents();
  m_pViewEntries->resizeRowsToContents();
}


void PMLayoutSettings::slotLayoutNameChanged( const QString& text )
{
	QListWidgetItem* item = m_pViewLayouts->currentItem();
	if(item) item->setText(text);

   ( *m_currentViewLayout ).setName( text );
   
   QList<PMViewLayout>::iterator it;   
   m_pDefaultLayout->clear();
   for( it = m_viewLayouts.begin(); it != m_viewLayouts.end(); ++it )
   {
      m_pDefaultLayout->addItem( ( *it ).name() );
      if( it == m_defaultViewLayout )
         m_pDefaultLayout->setCurrentIndex( m_pDefaultLayout->findText( ( *it ).name() ) );
   }
}

void PMLayoutSettings::slotViewEntrySelected( QModelIndex mindex )
{
   if( mindex.isValid() )
   {
      m_pViewTypeEdit->setEnabled( true );
      m_pDockPositionEdit->setEnabled( true );

      int n_item = m_model->item( mindex.row(), 0 )->text().toInt() - 1;

      m_currentViewEntry = ( *m_currentViewLayout ).begin() + n_item;
      QString vt = ( *m_currentViewEntry ).viewType();

      // find the view type
      int index = 0;
      bool found = false;
      const QList< PMViewTypeFactory* >& types =
         PMViewFactory::theFactory()->viewTypes();
      QList< PMViewTypeFactory* >::ConstIterator it = types.begin();

      for( ; *it && !found; ++it )
      {
         if( ( *it )->viewType() == vt )
            found = true;
         else
            index++;
      }
            
      if( !found )
      {
         qCritical() << "Unknown view type in PMLayoutSettings::slotViewEntrySelected" << Qt::endl;
         m_pViewTypeEdit->setCurrentIndex( 0 );
      }
      else
         m_pViewTypeEdit->setCurrentIndex( index );
      
      /*
      switch( ( *m_currentViewEntry ).glViewType() )
      {
         case PMGLView::PMViewNegY:
            m_pGLViewTypeEdit->setCurrentIndex( 0 );
            break;
         case PMGLView::PMViewPosY:
            m_pGLViewTypeEdit->setCurrentIndex( 1 );
            break;
         case PMGLView::PMViewPosX:
            m_pGLViewTypeEdit->setCurrentIndex( 2 );
            break;
         case PMGLView::PMViewNegX:
            m_pGLViewTypeEdit->setCurrentIndex( 3 );
            break;
         case PMGLView::PMViewPosZ:
            m_pGLViewTypeEdit->setCurrentIndex( 4 );
            break;
         case PMGLView::PMViewNegZ:
            m_pGLViewTypeEdit->setCurrentIndex( 5 );
            break;
         case PMGLView::PMViewCamera:
            m_pGLViewTypeEdit->setCurrentIndex( 6 );
            break;
      }
      */
      switch( ( *m_currentViewEntry ).dockPosition() )
      {
         case PMDockWidget::DockRight:
            m_pDockPositionEdit->setCurrentIndex( 0 );
            m_pColumnWidthLabel->show();
            m_pColumnWidthEdit->show();
            m_pViewHeightEdit->show();
            m_pViewHeightLabel->show();
            m_pFloatingWidth->hide();
            m_pFloatingHeight->hide();
            m_pFloatingPosX->hide();
            m_pFloatingPosY->hide();
            m_pFloatingWidthLabel->hide();
            m_pFloatingHeightLabel->hide();
            m_pFloatingPosXLabel->hide();
            m_pFloatingPosYLabel->hide();
            break;
         case PMDockWidget::DockBottom:
            m_pDockPositionEdit->setCurrentIndex( 1 );
            m_pColumnWidthLabel->hide();
            m_pColumnWidthEdit->hide();
            m_pViewHeightEdit->show();
            m_pViewHeightLabel->show();
            m_pFloatingWidth->hide();
            m_pFloatingHeight->hide();
            m_pFloatingPosX->hide();
            m_pFloatingPosY->hide();
            m_pFloatingWidthLabel->hide();
            m_pFloatingHeightLabel->hide();
            m_pFloatingPosXLabel->hide();
            m_pFloatingPosYLabel->hide();
            break;
         case PMDockWidget::DockCenter:
            m_pDockPositionEdit->setCurrentIndex( 2 );
            m_pColumnWidthLabel->hide();
            m_pColumnWidthEdit->hide();
            m_pViewHeightEdit->hide();
            m_pViewHeightLabel->hide();
            m_pFloatingWidth->hide();
            m_pFloatingHeight->hide();
            m_pFloatingPosX->hide();
            m_pFloatingPosY->hide();
            m_pFloatingWidthLabel->hide();
            m_pFloatingHeightLabel->hide();
            m_pFloatingPosXLabel->hide();
            m_pFloatingPosYLabel->hide();
            break;
         default:
            m_pDockPositionEdit->setCurrentIndex( 3 );
            m_pColumnWidthLabel->hide();
            m_pColumnWidthEdit->hide();
            m_pViewHeightEdit->hide();
            m_pViewHeightLabel->hide();
            m_pFloatingWidth->show();
            m_pFloatingHeight->show();
            m_pFloatingPosX->show();
            m_pFloatingPosY->show();
            m_pFloatingWidthLabel->show();
            m_pFloatingHeightLabel->show();
            m_pFloatingPosXLabel->show();
            m_pFloatingPosYLabel->show();
            break;
      }
      m_pColumnWidthEdit->setValue( ( *m_currentViewEntry ).columnWidth() );
      m_pViewHeightEdit->setValue( ( *m_currentViewEntry ).height() );
      m_pFloatingWidth->setValue( ( *m_currentViewEntry ).floatingWidth() );
      m_pFloatingHeight->setValue( ( *m_currentViewEntry ).floatingHeight() );
      m_pFloatingPosX->setValue( ( *m_currentViewEntry ).floatingPositionX() );
      m_pFloatingPosY->setValue( ( *m_currentViewEntry ).floatingPositionY() );
      m_pViewEntries->updateGeometry();
      displayCustomOptions();
      m_pViewEntries->resizeColumnsToContents();
      m_pViewEntries->resizeRowsToContents();
   }
   else
   {
      m_pViewTypeEdit->setEnabled( false );
      m_pDockPositionEdit->setEnabled( false );
      m_pColumnWidthLabel->hide();
      m_pColumnWidthEdit->hide();
      m_pViewHeightEdit->hide();
      m_pViewHeightLabel->hide();
      m_pFloatingWidth->hide();
      m_pFloatingHeight->hide();
      m_pFloatingPosX->hide();
      m_pFloatingPosY->hide();
      m_pFloatingWidthLabel->hide();
      m_pFloatingHeightLabel->hide();
      m_pFloatingPosXLabel->hide();
      m_pFloatingPosYLabel->hide();
   }
}

void PMLayoutSettings::slotViewTypeChanged( int index )
{
   const QList< PMViewTypeFactory* >& types =
      PMViewFactory::theFactory()->viewTypes();

   QList< PMViewTypeFactory* >::ConstIterator it = types.begin();
   it += index;

   const PMViewTypeFactory* factory = *it;
   PMViewLayoutEntry& ve = ( *m_currentViewEntry );
   
   if( factory && factory->viewType() != ve.viewType() )
   {
      ve.setViewType( factory->viewType() );
      ve.setCustomOptions( factory->newOptionsInstance() );
      QModelIndex mindex = m_pViewEntries->currentIndex();
      if( mindex.isValid() )
      {
         if( ve.customOptions() )
            m_model->item( mindex.row(), 1 )->setText( factory->description( ve.customOptions() ) );
         else
            m_model->item( mindex.row(), 1 )->setText( factory->description() );
         displayCustomOptions();
      }
   }
}

void PMLayoutSettings::slotDockPositionChanged( int index )
{
   switch( index )
   {
      case 0:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockRight );
         m_pColumnWidthLabel->show();
         m_pColumnWidthEdit->show();
         m_pViewHeightEdit->show();
         m_pViewHeightLabel->show();
         m_pFloatingWidth->hide();
         m_pFloatingHeight->hide();
         m_pFloatingPosX->hide();
         m_pFloatingPosY->hide();
         m_pFloatingWidthLabel->hide();
         m_pFloatingHeightLabel->hide();
         m_pFloatingPosXLabel->hide();
         m_pFloatingPosYLabel->hide();
         break;
      case 1:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockBottom );
         m_pColumnWidthLabel->hide();
         m_pColumnWidthEdit->hide();
         m_pViewHeightEdit->show();
         m_pViewHeightLabel->show();
         m_pFloatingWidth->hide();
         m_pFloatingHeight->hide();
         m_pFloatingPosX->hide();
         m_pFloatingPosY->hide();
         m_pFloatingWidthLabel->hide();
         m_pFloatingHeightLabel->hide();
         m_pFloatingPosXLabel->hide();
         m_pFloatingPosYLabel->hide();
         break;
      case 2:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockCenter );
         m_pColumnWidthLabel->hide();
         m_pColumnWidthEdit->hide();
         m_pViewHeightEdit->hide();
         m_pViewHeightLabel->hide();
         m_pFloatingWidth->hide();
         m_pFloatingHeight->hide();
         m_pFloatingPosX->hide();
         m_pFloatingPosY->hide();
         m_pFloatingWidthLabel->hide();
         m_pFloatingHeightLabel->hide();
         m_pFloatingPosXLabel->hide();
         m_pFloatingPosYLabel->hide();
         break;
      case 3:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockNone );
         m_pColumnWidthLabel->hide();
         m_pColumnWidthEdit->hide();
         m_pViewHeightEdit->hide();
         m_pViewHeightLabel->hide();
         m_pFloatingWidth->show();
         m_pFloatingHeight->show();
         m_pFloatingPosX->show();
         m_pFloatingPosY->show();
         m_pFloatingWidthLabel->show();
         m_pFloatingHeightLabel->show();
         m_pFloatingPosXLabel->show();
         m_pFloatingPosYLabel->show();
         break;
   }
   QModelIndex ix = m_pViewEntries->currentIndex();
   if( ix.isValid() )
       m_model->item( ix.row(), 2 )->setText( ( *m_currentViewEntry ).dockPositionAsString() );
}

void PMLayoutSettings::slotViewHeightChanged( const QString& text )
{
   ( *m_currentViewEntry ).setHeight( text.toInt() );
}

void PMLayoutSettings::slotColumnWidthChanged( const QString& text )
{
   ( *m_currentViewEntry ).setColumnWidth( text.toInt() );
}

void PMLayoutSettings::slotFloatingWidthChanged( const QString& text )
{
   ( *m_currentViewEntry ).setFloatingWidth( text.toInt() );
}

void PMLayoutSettings::slotFloatingHeightChanged( const QString& text )
{
   ( *m_currentViewEntry ).setFloatingHeight( text.toInt() );
}

void PMLayoutSettings::slotFloatingPosXChanged( const QString& text )
{
   ( *m_currentViewEntry ).setFloatingPositionX( text.toInt() );
}

void PMLayoutSettings::slotFloatingPosYChanged( const QString& text )
{
   ( *m_currentViewEntry ).setFloatingPositionY( text.toInt() );
}

void PMLayoutSettings::slotAddViewEntryClicked()
{
   PMViewLayoutEntry p;
   QString str;
   QList< QStandardItem* > items;
   QModelIndex index = m_pViewEntries->currentIndex();
   if( index.isValid() )
   {
      int n_item = index.row() + 1;
      ( *m_currentViewLayout ).addEntry( p, n_item );
      slotLayoutSelected( m_currentLayoutIndex );
      m_pViewEntries->selectRow( n_item - 1 );
      return;
      /*
      n_item++;
      str.setNum( n_item );//new row
      items.append( new QStandardItem( str ) );
      items.append( new QStandardItem( p.viewTypeAsString() ) );
      items.append( new QStandardItem( p.dockPositionAsString() ) );
      m_model->insertRow( n_item - 1, items );
      QAbstractItemModel* tableModel =  m_pViewEntries->model();
      int rcount = tableModel->rowCount();

      m_model->blockSignals( true );
      for( int i = 0; i < rcount ; i++)
      {
          str.setNum( i + 1 );
          m_model->item( i, 0 )->setText( str );
      }
      m_model->blockSignals( false );
      m_pViewEntries->selectRow( n_item - 1 );
      m_pViewEntries->resizeColumnsToContents();
      m_pViewEntries->resizeRowsToContents();
      */
   } 
   else
   {
      // If there is no selected the list must be empty
      ( *m_currentViewLayout ).addEntry( p );
       slotLayoutSelected( m_currentLayoutIndex );
       m_pViewEntries->selectRow( 0 );
       return;
      /*
      str.setNum( 1 );
      items.append( new QStandardItem( str ) );
      items.append( new QStandardItem( p.viewTypeAsString() ) );
      items.append( new QStandardItem( p.dockPositionAsString() ) );
      m_model->appendRow( items );
      m_pViewEntries->selectRow( 0 );
      m_pViewEntries->resizeColumnsToContents();
      m_pViewEntries->resizeRowsToContents();
      */
   }
}

void PMLayoutSettings::slotRemoveViewEntryClicked()
{
   QList< QStandardItem* > temp;
   QString str;
   QModelIndex current = m_pViewEntries->currentIndex();
   if( current.isValid() )
   {
      int n_item = m_model->item( current.row(), 0 )->text().toInt() - 1;
      ( *m_currentViewLayout ).removeEntry( n_item );
      slotLayoutSelected( m_currentLayoutIndex );
      return;

      // Save the next selected item in temp, since the current item will
      // be removed.
      /*
      m_model->removeRow( n_item, current);
      m_pViewEntries->updateGeometry();
      int rowCount = m_model->rowCount( current );
      for(int row = 0; row < rowCount; row++)
          m_model->item( row, 0 )->setText( QString( row ) );
       */

     /* temp = current.nextSibling();
      if( !temp )
         temp = current->itemAbove();
      else
         n_item++;

      delete current;

      if( temp )
      {
         str.setNum( n_item );
         temp->setText( 0, str );
         m_pViewEntries->setSelected( temp, true );
         n_item++;
         temp = temp->nextSibling();
      }
      else
         slotViewEntrySelected( 0 );
      while( temp )
      {
         str.setNum( n_item );
         temp->setText( 0, str );
         n_item++;
         temp = temp->nextSibling();
      }*/
   }
}

void PMLayoutSettings::slotMoveUpViewEntryClicked()
{
}

void PMLayoutSettings::slotMoveDownViewEntryClicked()
{
}

void PMLayoutSettings::displayCustomOptions()
{
   // delete an old widget
   if( m_pCustomOptionsHolder->layout() )
      delete m_pCustomOptionsHolder->layout();
   if( m_pCustomOptionsWidget )
   {
      delete m_pCustomOptionsWidget;
      m_pCustomOptionsWidget = 0;
   }
   
   if( m_currentViewLayout != m_viewLayouts.end() &&
       m_currentViewEntry != ( *m_currentViewLayout ).end() &&
       ( *m_currentViewEntry ).customOptions() )
   {
      PMViewTypeFactory* vf = PMViewFactory::theFactory()->viewFactory(
         ( *m_currentViewEntry ).viewType() );
      if( vf )
      {
         m_pCustomOptionsWidget =
            vf->newOptionsWidget( m_pCustomOptionsHolder,
                                  ( *m_currentViewEntry ).customOptions() );
         if( m_pCustomOptionsWidget )
         {
            connect( m_pCustomOptionsWidget, SIGNAL( viewTypeDescriptionChanged() ),
                     SLOT( slotViewTypeDescriptionChanged() ) );
            QHBoxLayout* hl = new QHBoxLayout( m_pCustomOptionsHolder );
//TODO PORT QT5             hl->setSpacing( QDialog::spacingHint() );
            hl->addWidget( m_pCustomOptionsWidget );
            m_pCustomOptionsWidget->show();
         }
      }
   }
}

void PMLayoutSettings::slotViewTypeDescriptionChanged()
{
   PMViewLayoutEntry& ve = *m_currentViewEntry;
   const PMViewTypeFactory* factory =
      PMViewFactory::theFactory()->viewFactory( ve.viewType() );
   
   if( factory )
   {
      QModelIndex mindex = m_pViewEntries->currentIndex();
      if( mindex.isValid() )
      {
         if( ve.customOptions() )
            m_model->item( mindex.row(), 1 )->setText( factory->description( ve.customOptions() ) );
         else
            m_model->item( mindex.row(), 1 )->setText( factory->description() );
      }
   }
}

#include "pmlayoutsettings.moc"
