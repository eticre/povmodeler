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


#include "pmprismedit.h"
#include "pmprism.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"
#include "pmvectorlistedit.h"
#include "pmpart.h"

#include <qlayout.h>
#include <qlabel.h>

#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <QPixmap>
#include <QVBoxLayout>
#include <QList>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include <QMessageBox>

PMPrismEdit::PMPrismEdit( QWidget* parent )
      : Base( parent )
{
   m_pDisplayedObject = nullptr;
   m_lastSplineType = 0;
}

PMPrismEdit::~PMPrismEdit()
{
}

void PMPrismEdit::createTopWidgets()
{
   Base::createTopWidgets();
   QBoxLayout* tl = topLayout();

   QHBoxLayout* hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Spline type:" ), this ) );
   m_pSplineType = new QComboBox( this );
   m_pSplineType->addItem( tr( "Linear Spline" ) );
   m_pSplineType->addItem( tr( "Quadratic Spline" ) );
   m_pSplineType->addItem( tr( "Cubic Spline" ) );
   m_pSplineType->addItem( tr( "Bezier Spline" ) );
   hl->addWidget( m_pSplineType );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   hl->addWidget( new QLabel( tr( "Sweep type:" ), this ) );
   m_pSweepType = new QComboBox( this );
   m_pSweepType->addItem( tr( "Linear Sweep" ) );
   m_pSweepType->addItem( tr( "Conic Sweep" ) );
   hl->addWidget( m_pSweepType );

   connect( m_pSplineType, SIGNAL( activated( int ) ),
            SLOT( slotTypeChanged( int ) ) );
   connect( m_pSweepType, SIGNAL( activated( int ) ),
            SLOT( slotSweepChanged( int ) ) );

   hl = new QHBoxLayout();
   tl->addLayout( hl );
   QGridLayout* gl = new QGridLayout();
   hl->addLayout( gl );
   gl->addWidget( new QLabel( tr( "Height 1:" ), this ), 0, 0 );
   m_pHeight1 = new PMFloatEdit( this );
   gl->addWidget( m_pHeight1, 0, 1 );
   connect( m_pHeight1, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   
   gl->addWidget( new QLabel( tr( "Height 2:" ), this ), 1, 0 );
   m_pHeight2 = new PMFloatEdit( this );
   gl->addWidget( m_pHeight2, 1, 1 );
   connect( m_pHeight2, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );
   hl->addStretch( 1 );
}

void PMPrismEdit::createBottomWidgets()
{
   m_pEditWidget = new QWidget( this );
   topLayout()->addWidget( m_pEditWidget );
   m_pOpen = new QCheckBox( ( "type of the object Open" ), this );
   topLayout()->addWidget( m_pOpen );
   m_pSturm = new QCheckBox( tr( "Sturm" ), this );
   topLayout()->addWidget( m_pSturm );
   connect( m_pSturm, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );
   connect( m_pOpen, SIGNAL( clicked() ), SIGNAL( dataChanged() ) );

   Base::createBottomWidgets();
}

void PMPrismEdit::displayObject( PMObject* o )
{
   if( o->isA( "Prism" ) )
   {
      bool readOnly = o->isReadOnly();
      m_pDisplayedObject = static_cast<PMPrism*>(o);

      switch( m_pDisplayedObject->splineType() )
      {
         case PMPrism::LinearSpline:
            m_pSplineType->setCurrentIndex( 0 );
            break;
         case PMPrism::QuadraticSpline:
            m_pSplineType->setCurrentIndex( 1 );
            break;
         case PMPrism::CubicSpline:
            m_pSplineType->setCurrentIndex( 2 );
            break;
         case PMPrism::BezierSpline:
            m_pSplineType->setCurrentIndex( 3 );
            break;
      }
      m_pSplineType->setEnabled( !readOnly );
      switch( m_pDisplayedObject->sweepType() )
      {
         case PMPrism::LinearSweep:
            m_pSweepType->setCurrentIndex( 0 );
            break;
         case PMPrism::ConicSweep:
            m_pSweepType->setCurrentIndex( 1 );
            break;
      }
      m_pHeight1->setValue( m_pDisplayedObject->height1() );
      m_pHeight1->setReadOnly( readOnly );
      m_pHeight2->setValue( m_pDisplayedObject->height2() );
      m_pHeight2->setReadOnly( readOnly );
      m_pSweepType->setEnabled( !readOnly );
      m_pSturm->setChecked( m_pDisplayedObject->sturm() );
      m_pSturm->setEnabled( !readOnly );
      m_pOpen->setChecked( m_pDisplayedObject->open() );
      m_pOpen->setEnabled( !readOnly );
      displayPoints( m_pDisplayedObject->points() );

      Base::displayObject( o );
   }
   else
      qCritical(  ) << "PMPrismEdit: Can't display object\n";
}

void PMPrismEdit::displayPoints( const QList< QList<PMVector> >& sp )
{
   bool readOnly = m_pDisplayedObject->isReadOnly();

   // (re)create the edit widget if necessary
   createEdits( sp );
   
   QList< QList<PMVector> >::ConstIterator spit = sp.begin();
   QList< PMVectorListEdit* >::Iterator seit = m_points.begin();
   QList< QPushButton* >::Iterator sbit1 = m_removeButtons.begin();

   // display the points
   for( ; ( spit != sp.end() ) && *seit; ++spit, ++seit, ++sbit1 )
   {
      ( *seit )->setVectors( *spit );
      //( *seit )->setReadOnly( readOnly );
      ( *sbit1 )->setEnabled( !readOnly && ( *spit ).size() > 3 );
   }

   QPushButton* sbit2;
   foreach( sbit2, m_addAboveButtons )
      sbit2->setEnabled( !readOnly );
   foreach( sbit2, m_addBelowButtons )
      sbit2->setEnabled( !readOnly );
   foreach( sbit2, m_subPrismAddButtons )
      sbit2->setEnabled( !readOnly );
   foreach( sbit2, m_subPrismRemoveButtons )
      sbit2->setEnabled( !readOnly && sp.count() > 1 );
   updateControlPointSelection();
}

void PMPrismEdit::createEdits( const QList< QList<PMVector> >& sp )
{
   int st = m_pSplineType->currentIndex();
   
   if( sp.count() != m_points.count() )
   {
      deleteEdits();

      QIcon addIcon = QIcon( ":/smallicon/icons/povicons/small/pmaddpoint" );
      QIcon removeIcon = QIcon( ":/smallicon/icons/povicons/small/pmremovepoint" );
      QIcon addPrismIcon = QIcon( ":/smallicon/icons/povicons/small/pmaddsubprism" );
      QVBoxLayout* tvl = new QVBoxLayout( m_pEditWidget);
//TODO PORT QT5       tvl->setSpacing( QDialog::spacingHint() );
      QHBoxLayout* hl = nullptr;
      QVBoxLayout* vl;
      QLabel* label = nullptr;
      QPushButton* button = nullptr;
      PMVectorListEdit* vle;
      int spnr = 0;
      
      for( spnr = 0; spnr < ( signed ) sp.count(); spnr++ )
      {
         // create all edits for one sub prism
         hl = new QHBoxLayout();
         tvl->addLayout( hl );
         label = new QLabel(("Sub prism :" + QString::number(spnr + 1)), m_pEditWidget );
         hl->addWidget( label );
         hl->addStretch( 1 );
         m_labels.append( label );
         label->show();
         
         button = new QPushButton( m_pEditWidget );
         button->setIcon( addPrismIcon );
         m_subPrismAddButtons.append( button );
         connect( button, SIGNAL( clicked() ), SLOT( slotAddSubPrism() ) );
         hl->addWidget( button );
         button->show();
         button->setToolTip( tr( "Add sub prism" ) );
         
         button = new QPushButton( m_pEditWidget );
         button->setIcon( removeIcon );
         m_subPrismRemoveButtons.append( button );
         connect( button, SIGNAL( clicked() ), SLOT( slotRemoveSubPrism() ) );
         hl->addWidget( button );
         button->show();
         if( sp.size() < 2 )
            button->setEnabled( false );
         button->setToolTip( tr( "Remove sub prism" ) );

         hl = new QHBoxLayout();
         tvl->addLayout( hl );
         
         vle = new PMVectorListEdit( "x", "z", m_pEditWidget );
         m_points.append( vle );
         connect( vle, SIGNAL( dataChanged() ), SIGNAL( dataChanged() ) );

         /*???connect( vle->selectionModel(),
                  SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ),
                  SLOT( slotSelectionChanged() ) );*/
         connect( vle->selectionModel(),
                  SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ),
                  SLOT( slotSelectionChanged() ) );

         hl->addWidget( vle, 2 );
         vle->show();

         vl = new QVBoxLayout();
         hl->addLayout( vl );
         
         button = new QPushButton( m_pEditWidget );
         button->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmaddpointabove" ) );
         connect( button, SIGNAL( clicked() ), SLOT( slotAddPointAbove() ) );
         m_addAboveButtons.append( button );
         button->show();
         vl->addWidget( button );
         button = new QPushButton( m_pEditWidget );
         button->setIcon( addIcon );
         connect( button, SIGNAL( clicked() ), SLOT( slotAddPointBelow() ) );
         m_addBelowButtons.append( button );
         button->show();
         vl->addWidget( button );
         button = new QPushButton( m_pEditWidget );
         button->setIcon( QIcon( ":/smallicon/icons/povicons/small/pmremovepoint" ) );
         connect( button, SIGNAL( clicked() ), SLOT( slotRemovePoint() ) );
         m_removeButtons.append( button );
         button->show();
         vl->addWidget( button );

         vl->addStretch( 1 );
         
//TODO PORT QT5          tvl->addSpacing( QDialog::spacingHint() );
      }
      
      hl = new QHBoxLayout();
      tvl->addLayout( hl );
      label = new QLabel( tr( "New sub prism" ), m_pEditWidget );
      hl->addWidget( label );
      hl->addStretch( 1 );
      m_labels.append( label );
      label->show();
         
      button = new QPushButton( m_pEditWidget );
      button->setIcon( addPrismIcon );
      m_subPrismAddButtons.append( button );
      connect( button, SIGNAL( clicked() ), SLOT( slotAddSubPrism() ) );
      hl->addWidget( button );
      button->show();
      button->setToolTip( tr( "Append sub prism" ) );
   }

   QList< PMVectorListEdit* >::Iterator vlit = m_points.begin();
   QList< QList< PMVector > >::ConstIterator spit;
   PMVectorListEdit* vle = nullptr;
   bool newSize = false;
   
   for( spit = sp.begin(); spit != sp.end(); ++spit, ++vlit )
   {
      int lines = ( *spit ).count();
      
      vle = *vlit;
      if( ( vle->size() != lines ) /*|| ( st != m_lastSplineType )*/ )
      {
         newSize = true;
         vle->setSize( lines );
      }
   }
   if( newSize )
   {
      m_pEditWidget->updateGeometry();
      emit sizeChanged();
   }

   m_lastSplineType = st;
}

void PMPrismEdit::deleteEdits()
{
   while( !m_labels.isEmpty() )
      delete m_labels.takeFirst();
   while( !m_subPrismAddButtons.isEmpty() )
      delete m_subPrismAddButtons.takeFirst();
   while( !m_subPrismRemoveButtons.isEmpty() )
      delete m_subPrismRemoveButtons.takeFirst();
   while( !m_addAboveButtons.isEmpty() )
      delete m_addAboveButtons.takeFirst();
   while( !m_addBelowButtons.isEmpty() )
      delete m_addBelowButtons.takeFirst();
   while( !m_removeButtons.isEmpty() )
      delete m_removeButtons.takeFirst();
   while( !m_points.isEmpty() ) 
      delete m_points.takeFirst();
   if( m_pEditWidget->layout() )
      delete m_pEditWidget->layout();
}

QList< QList<PMVector> > PMPrismEdit::splinePoints()
{
   QList< QList<PMVector> > values;

   foreach( PMVectorListEdit* pmvle, m_points )
      values.append( pmvle->vectors() );
   
   return values;
}

void PMPrismEdit::saveContents()
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setPoints( splinePoints() );

      switch( m_pSplineType->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setSplineType( PMPrism::LinearSpline );
            break;
         case 1:
            m_pDisplayedObject->setSplineType( PMPrism::QuadraticSpline );
            break;
         case 2:
            m_pDisplayedObject->setSplineType( PMPrism::CubicSpline );
            break;
         case 3:
            m_pDisplayedObject->setSplineType( PMPrism::BezierSpline );
            break;
      }
      switch( m_pSweepType->currentIndex() )
      {
         case 0:
            m_pDisplayedObject->setSweepType( PMPrism::LinearSweep );
            break;
         case 1:
            m_pDisplayedObject->setSweepType( PMPrism::ConicSweep );
            break;
      }
      m_pDisplayedObject->setSturm( m_pSturm->isChecked() );
      m_pDisplayedObject->setOpen( m_pOpen->isChecked() );
      m_pDisplayedObject->setHeight1( m_pHeight1->value() );
      m_pDisplayedObject->setHeight2( m_pHeight2->value() );
      Base::saveContents();
   }
}

bool PMPrismEdit::isDataValid()
{
   PMVectorListEdit* current;
   foreach( current, m_points)
      if( !current->isDataValid() )
         return false;
   
   foreach( current, m_points)
   {
      int np = current->size();
      switch( m_pSplineType->currentIndex() )
      {
         case 0:
            if( np < 3 )
            {
               QMessageBox::warning( this, "Linear splines need at least 3 points.", "Error" );
               return false;
            }
            break;
         case 1:
            if( np < 4 )
            {
               QMessageBox::warning( this, "Quadratic splines need at least 4 points.", "Error" );
               return false;
            }
            break;
         case 2:
            if( np < 5 )
            {
               QMessageBox::warning( this, "Cubic splines need at least 5 points.", "Error" );
               return false;
            }
            break;
         case 3:
            if( ( np < 3 ) || ( ( np % 3 ) != 0 ) )
            {
               QMessageBox::warning( this, "Bezier splines need 3 points for each segment.", "Error" );
               return false;
            }
            break;
      }
   }
   
   return Base::isDataValid();
}

void PMPrismEdit::slotTypeChanged( int )
{
   displayPoints( splinePoints() );
   emit dataChanged();
   emit sizeChanged();
}

void PMPrismEdit::slotSweepChanged( int )
{
   emit dataChanged();
}

void PMPrismEdit::slotAddPointAbove()
{
   QPushButton* bt = static_cast<QPushButton*>(sender());
   if( bt )
   {
      int subIndex = m_addAboveButtons.indexOf( bt );
      if( subIndex >= 0 )
      {
         PMVectorListEdit* ed = m_points.at( subIndex );
         int index = ed->currentIndex().row();
         if( index >= 0 && index < ed->size() )
         {
            QList<PMVector> points = ed->vectors();
            QList<PMVector>::iterator it = points.begin() + index;
            
            PMVector newPoint = *it;
            if( index != 0 )
            {
               --it;
               newPoint = ( newPoint + *it ) / 2;
               ++it;
            }
            points.insert( it, newPoint );

            ed->setSize( points.size() );
            ed->setVectors( points );
            if( points.size() > 3 )
               m_removeButtons.at( subIndex )->setEnabled( true );

            emit dataChanged();
            emit sizeChanged();
         }
      }
   }
}

void PMPrismEdit::slotAddPointBelow()
{
   QPushButton* bt = static_cast<QPushButton*>(sender());
   if( bt )
   {
      int subIndex = m_addBelowButtons.indexOf( bt );
      if( subIndex >= 0 )
      {
         PMVectorListEdit* ed = m_points.at( subIndex );
         int index = ed->currentIndex().row();
         if( index >= 0 && index < ed->size() )
         {
            QList<PMVector> points = ed->vectors();
            QList<PMVector>::iterator it = points.begin() + index;
            
            PMVector newPoint = *it;
            ++it;
            
            if( it != points.end() )
               newPoint = ( newPoint + *it ) / 2;
            
            points.insert( it, newPoint );

            ed->setSize( points.size() );
            ed->setVectors( points );
            //ed->setCurrentCell( index + 1, ed->currentIndex().column() );
            //ed->clearSelection();
            //ed->select( index + 1, ed->currentIndex().column() );
            if( points.size() > 3 )
               m_removeButtons.at( subIndex )->setEnabled( true );

            emit dataChanged();
            emit sizeChanged();
         }
      }
   }
}

void PMPrismEdit::slotRemovePoint()
{
   QPushButton* bt = static_cast<QPushButton*>(sender());
   if( bt )
   {
      int subIndex = m_removeButtons.indexOf( bt );
      if( subIndex >= 0 )
      {
         PMVectorListEdit* ed = m_points.at( subIndex );
         int index = ed->currentIndex().row();
         if( index >= 0 && index < ed->size() )
         {
            QList<PMVector> points = ed->vectors();
            QList<PMVector>::iterator it = points.begin() + index;
            
            points.erase( it );

            ed->setSize( points.size() );
            ed->setVectors( points );
            if( points.size() <= 3 )
               m_removeButtons.at( subIndex )->setEnabled( false );

            emit dataChanged();
            emit sizeChanged();
         }
      }
   }
}

void PMPrismEdit::slotAddSubPrism()
{
   if( m_pSplineType->currentIndex() == 3 )
   {
      QMessageBox::warning( this, "Sub prisms do not work with bezier splines in POV-Ray 3.1.",
                                "subPrismWithBezierSplines" );
   }
   
   QPushButton* button = static_cast<QPushButton*>(sender());
   if( button )
   {
      int index = m_subPrismAddButtons.indexOf( button );
		QList< QList<PMVector> > points = splinePoints();
		
      if( index >= 0 && index < points.count() )
      {
         QList< QList<PMVector> >::Iterator it = points.begin() + index;
         QList<PMVector> newSubPrism;

         if( it != points.begin() )
         {
            --it;
            newSubPrism = *it;
            ++it;
            
            // find middle point
            PMVector mid( 2 );
            int num = 0;
            QList<PMVector>::Iterator pit = newSubPrism.begin();
            for( ; pit != newSubPrism.end(); ++pit, ++num )
               mid += *pit;
            if( num > 0 )
               mid /= num;
            for( pit = newSubPrism.begin(); pit != newSubPrism.end(); ++pit )
               *pit = ( *pit - mid ) * 0.8 + mid;
         }
         else
            newSubPrism = *it;

         points.insert( it, newSubPrism );
         displayPoints( points );
         emit dataChanged();
         emit sizeChanged();
      }
   }
}

void PMPrismEdit::slotRemoveSubPrism()
{
   QPushButton* button = static_cast<QPushButton*>(sender());
   if( button )
   {
      int index = m_subPrismRemoveButtons.indexOf( button );
		QList< QList<PMVector> > points = splinePoints();
		
      if( index >= 0 && index < points.count() )
      {
         QList< QList<PMVector> >::Iterator it = points.begin() + index;

         if( points.count() > 1 )
         {
            points.erase( it );
            displayPoints( points );
            emit dataChanged();
            emit sizeChanged();
         }
      }
   }
}

void PMPrismEdit::slotSelectionChanged()
{
   PMVectorListEdit* edit = static_cast<PMVectorListEdit*>(sender());
   if( edit )
   {
      QList< QList< PMVector > > points = m_pDisplayedObject->points();

      if( m_points.count() == points.size() )
      {
         int i;
         bool changed = false;
         QList< QList< PMVector > >::Iterator spit;
         PMControlPointList cp = part()->activeControlPoints();
         PMControlPointList::iterator it = cp.begin();  ++it; ++it;
         QList<PMVectorListEdit*>::Iterator edit = m_points.begin();
         
         for( spit = points.begin(); spit != points.end() && it != cp.end();
              ++spit, ++edit )
         {
            int np = ( *spit ).size();
            
            if( ( *edit )->size() == np )
            {
               for( i = 0; i < np && it != cp.end(); i++, ++it )
                  ( *it )->setSelected( ( *edit )->isSelected( i ) );
               changed = true;
            }
            else
               for( i = 0; i < np; i++ )
                  ++it;
         }
         if( changed )
            emit controlPointSelectionChanged();
      }
   }
}

void PMPrismEdit::updateControlPointSelection()
{
   QList<QList <PMVector> > points = m_pDisplayedObject->points();

   if( m_points.count() == points.size() )
   {
      QList< QList< PMVector > >::Iterator spit;
      PMControlPointList cp = part()->activeControlPoints();
      PMControlPointList::iterator it = cp.begin();  ++it; ++it;
      QList<PMVectorListEdit*>::Iterator edit = m_points.begin();
      
      for( spit = points.begin(); spit != points.end() && it != cp.end();
           ++spit, ++edit )
      {
         PMVectorListEdit* vl = *edit;
         int np = ( *spit ).size();
         int i;
         
         if( vl->size() == np )
         {
            vl->blockSelectionUpdates( true );
            bool sb = vl->signalsBlocked();
            vl->blockSignals( true );
            vl->selectionModel()->blockSignals( true );
            vl->model()->blockSignals( true );

            vl->clearSelection();
            for( i = 0; i < np && it != cp.end(); i++, ++it )
               if( ( *it )->selected() )
                  vl->select( i );
            
            vl->blockSignals( sb );
            vl->selectionModel()->blockSignals( sb );
            vl->model()->blockSignals( sb );
            vl->blockSelectionUpdates( false );
         }
         else
            for( i = 0; i < np; i++ )
               ++it;
      }
   }
}

#include "pmprismedit.moc"
