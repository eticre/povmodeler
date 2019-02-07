/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2006 by Andreas Zehender
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


#include "pmrendermodesdialog.h"

#include <QListWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTabWidget>

#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResizeEvent>

#include <QLocale>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include "pmlineedits.h"

QSize PMRenderModesDialog::s_size = QSize( 300, 200 );

PMRenderModesDialog::PMRenderModesDialog( const PMRenderModeList& modes, int index, QWidget* parent )
      : QDialog( parent)
{
    setWindowTitle( "Render Modes" );

    buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok|QDialogButtonBox::Cancel );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout( mainLayout );

    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( slotOk() ) );
    connect( buttonBox, SIGNAL (rejected() ), this, SLOT( reject() ) );

    m_selectionIndex = index;
    m_workingModes = modes;
	
   m_pListBox = new QListWidget( this );
   mainLayout->addWidget( m_pListBox );
   connect( m_pListBox, SIGNAL( currentRowChanged( int ) ), SLOT( slotModeSelected( int ) ) );

   QWidget* buttons = new QWidget( this );
   mainLayout->addWidget( buttons );
   QHBoxLayout *hboxLayout1 = new QHBoxLayout( buttons );

   m_pAddButton = new QPushButton( tr( "Add" ), buttons );
   hboxLayout1->addWidget(m_pAddButton);
   connect( m_pAddButton, SIGNAL( clicked() ), SLOT( slotAdd() ) );

   m_pRemoveButton = new QPushButton( tr( "Remove" ), buttons );
   hboxLayout1->addWidget(m_pRemoveButton);
   connect( m_pRemoveButton, SIGNAL( clicked() ), SLOT( slotRemove() ) );

   m_pEditButton = new QPushButton( tr( "Edit..." ), buttons );
   hboxLayout1->addWidget(m_pEditButton);
   connect( m_pEditButton, SIGNAL( clicked() ), SLOT( slotEdit() ) );

   m_pUpButton = new QPushButton( tr( "Up" ), buttons );
   hboxLayout1->addWidget(m_pUpButton);
   connect( m_pUpButton, SIGNAL( clicked() ), SLOT( slotUp() ) );

   m_pDownButton = new QPushButton( tr( "Down" ), buttons );
   hboxLayout1->addWidget(m_pDownButton);
   connect( m_pDownButton, SIGNAL( clicked() ), SLOT( slotDown() ) );

   m_pRemoveButton->setEnabled( false );
   m_pUpButton->setEnabled( false );
   m_pDownButton->setEnabled( false );

   buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
   mainLayout->addWidget( buttonBox );
   resize( s_size );
   displayList();
   connect( m_pListBox,  SIGNAL( itemDoubleClicked ( QListWidgetItem *) ), this, SLOT( slotEdit() ) );
   //connect( this, SIGNAL( okClicked() ),this, SLOT( slotOk() ) );
}

void PMRenderModesDialog::slotChanged()
{
   buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
}

void PMRenderModesDialog::slotModeSelected( int index )
{
   m_selectionIndex = index;
   checkButtons();
   slotChanged();
}

void PMRenderModesDialog::displayList()
{
   PMRenderModeList::iterator it;
   bool b = m_pListBox->signalsBlocked();

   m_pListBox->blockSignals( true );
   m_pListBox->clear();

   for( it = m_workingModes.begin(); it != m_workingModes.end(); ++it )
      m_pListBox->addItem( (*it ).description() );

   m_pListBox->setCurrentRow( m_selectionIndex );
   m_pListBox->blockSignals( b );
   checkButtons();
}

void PMRenderModesDialog::checkButtons()
{
   if( m_selectionIndex < 0 )
   {
      m_pRemoveButton->setEnabled( false );
      m_pEditButton->setEnabled( false );
      m_pUpButton->setEnabled( false );
      m_pDownButton->setEnabled( false );
   }
   else
   {
      int num = m_workingModes.count();

      m_pRemoveButton->setEnabled( true );
      m_pEditButton->setEnabled( true );
      m_pUpButton->setEnabled( m_selectionIndex != 0 );
      m_pDownButton->setEnabled( m_selectionIndex != ( num - 1 ) );
   }
}

void PMRenderModesDialog::saveConfig()
{
   QSettings qset;
   qset.setValue( "Appearance/RenderModesDialogSize", s_size );
}

void PMRenderModesDialog::restoreConfig()
{
   QSettings qset;
   QSize defaultSize( 300, 200 );
   s_size = qset.value( "Appearance/RenderModesDialogSize", defaultSize ).value<QSize>();
}

void PMRenderModesDialog::resizeEvent( QResizeEvent* ev )
{
   s_size = ev->size();
}

void PMRenderModesDialog::slotAdd()
{
   m_selectionIndex++;

   if( m_selectionIndex < 0 )
      m_selectionIndex = 0;

   m_workingModes.insert( m_selectionIndex, PMRenderMode() );

   displayList();
   slotChanged();
}

void PMRenderModesDialog::slotRemove()
{
   m_workingModes.removeAt( m_selectionIndex );

   int num = m_workingModes.count();
   if( m_selectionIndex >= num )
      m_selectionIndex = num - 1;

   displayList();
   slotChanged();
}

void PMRenderModesDialog::slotUp()
{
   PMRenderMode mode = m_workingModes[ m_selectionIndex ];
	m_workingModes.removeAt( m_selectionIndex );
   m_selectionIndex--;
   if( m_selectionIndex < 0 )
      m_selectionIndex = 0;
   m_workingModes.insert( m_selectionIndex, mode );

   displayList();
   slotChanged();
}

void PMRenderModesDialog::slotDown()
{
   PMRenderMode mode = m_workingModes[ m_selectionIndex ];
	m_workingModes.removeAt( m_selectionIndex );
   m_selectionIndex++;

   int num = m_workingModes.count();
   if( m_selectionIndex > num )
      m_selectionIndex = num;
   m_workingModes.insert( m_selectionIndex, mode );

   displayList();
   slotChanged();
}

void PMRenderModesDialog::slotEdit()
{
    if ( m_selectionIndex == -1 )
        return;

   PMRenderModeDialog dlg( m_workingModes.at( m_selectionIndex ) );
   bool changed = ( dlg.exec() == QDialog::Accepted );
   if( changed )
   {
      m_workingModes[m_selectionIndex] = dlg.mode();
      slotChanged();
      displayList();
   }
}

void PMRenderModesDialog::slotOk()
{
   accept();
}


QSize PMRenderModeDialog::s_size = QSize( 300, 200 );

const int numQuality = 9;
const char* qualityString[numQuality] =
{
   "0, 1: Quick colors, full ambient lighting only" ,
   "2, 3: Show specified diffuse and ambient light" ,
   "4: Render shadows, but no extended lights" ,
   "5: Render shadows, including extended lights" ,
   "6, 7: Compute texture patterns" ,
   "8: Compute reflected, refracted, and transmitted rays" ,
   "9: Compute media" ,
   "10: Compute radiosity but no media" ,
   "11: Compute radiosity and media"
};

const int c_qualityToIndex[12] = { 0, 0, 1, 1, 2, 3, 4, 4, 5, 6, 7, 8 };
const int c_indexToQuality[numQuality] = { 0, 2, 4, 5, 6, 8, 9, 10, 11 };

PMRenderModeDialog::PMRenderModeDialog( const PMRenderMode& mode, QWidget* parent )
      : QDialog( parent)
{
    setWindowTitle( "Render Modes" );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    modeButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok|QDialogButtonBox::Cancel );

    connect( modeButtonBox, SIGNAL( accepted() ), this, SLOT( slotOk() ) );
    connect( modeButtonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

    m_mode = mode;
    int i;

   QHBoxLayout* descrLayout = new QHBoxLayout();
   mainLayout->addLayout( descrLayout );
   QLabel* descrLabel = new QLabel( "Description:", this );
   descrLayout->addWidget( descrLabel );

   m_pDescriptionEdit = new QLineEdit( this );
   descrLayout->addWidget( m_pDescriptionEdit );

   m_pTabWidget = new QTabWidget( this );
   mainLayout->addWidget( m_pTabWidget );

   mainLayout->addWidget( modeButtonBox );

   QWidget* tab;
   QVBoxLayout* tabLayout;

   // size tab
   tab = new QWidget();
   m_pTabWidget->addTab( tab, tr( "Size" ) );
   tabLayout = new QVBoxLayout( tab );
   tabLayout->setSpacing( 1 );
   tabLayout->setMargin( 1 );

   QHBoxLayout* sizeHelpLayout = new QHBoxLayout();
   tabLayout->addLayout( sizeHelpLayout );
   QGridLayout* sizeLayout = new QGridLayout();
   sizeHelpLayout->addLayout( sizeLayout );
   sizeLayout->addWidget( new QLabel( tr( "Width:" ), tab ), 0, 0 );
   m_pWidthEdit = new PMIntEdit( tab );
   m_pWidthEdit->setValidation( true, 1, false, 0 );
   sizeLayout->addWidget( m_pWidthEdit, 0, 1 );
   sizeLayout->addWidget( new QLabel( tr( "Height:" ), tab ), 1, 0 );
   m_pHeightEdit = new PMIntEdit( tab );
   m_pHeightEdit->setValidation( true, 1, false, 0 );
   sizeLayout->addWidget( m_pHeightEdit, 1, 1 );
   sizeHelpLayout->addStretch( 1 );

   m_pSubsectionBox = new QCheckBox( tr( "Subsection" ), tab );
   tabLayout->addWidget( m_pSubsectionBox );

   QHBoxLayout* ssHelpLayout = new QHBoxLayout();
   tabLayout->addLayout( ssHelpLayout );
   QGridLayout* ssLayout = new QGridLayout();
   ssHelpLayout->addLayout( ssLayout );
   ssLayout->addWidget( new QLabel( tr( "Start column:" ), tab ), 0, 0 );
   m_pStartColumnEdit = new PMFloatEdit( tab );
   m_pStartColumnEdit->setValidation( true, 0.0, false, 0.0 );
   ssLayout->addWidget( m_pStartColumnEdit, 0, 1 );
   ssLayout->addWidget( new QLabel( tr( "End column:" ), tab ), 1, 0 );
   m_pEndColumnEdit = new PMFloatEdit( tab );
   m_pEndColumnEdit->setValidation( true, 0.0, false, 0.0 );
   ssLayout->addWidget( m_pEndColumnEdit, 1, 1 );
   ssLayout->addWidget( new QLabel( tr( "Start row:" ), tab ), 2, 0 );
   m_pStartRowEdit = new PMFloatEdit( tab );
   m_pStartRowEdit->setValidation( true, 0.0, false, 0.0 );
   ssLayout->addWidget( m_pStartRowEdit, 2, 1 );
   ssLayout->addWidget( new QLabel( tr( "End row:" ), tab ), 3, 0 );
   m_pEndRowEdit = new PMFloatEdit( tab );
   m_pEndRowEdit->setValidation( true, 0.0, false, 0.0 );
   ssLayout->addWidget( m_pEndRowEdit, 3, 1 );
   ssHelpLayout->addStretch( 1 );

   tabLayout->addStretch( 1 );

   // quality tab
   tab = new QWidget();
   m_pTabWidget->addTab( tab, tr( "Quality" ) );
   tabLayout = new QVBoxLayout( tab );
   tabLayout->setSpacing( 1 );
   tabLayout->setMargin( 1 );

   QHBoxLayout* quHelpLayout = new QHBoxLayout();
   tabLayout->addLayout( quHelpLayout );
   quHelpLayout->addWidget( new QLabel( tr( "Quality:" ), tab ) );
   m_pQualityCombo = new QComboBox( tab );
   quHelpLayout->addWidget( m_pQualityCombo );
   for( i = 0; i < numQuality; i++ )
      m_pQualityCombo->addItem( tr( qualityString[i] ) );

   m_pAntialiasingBox = new QCheckBox( tr( "Antialiasing" ), tab );
   tabLayout->addWidget( m_pAntialiasingBox );

   QHBoxLayout* aaHelpLayout = new QHBoxLayout();
   tabLayout->addLayout( aaHelpLayout );
   QGridLayout* aaGridLayout = new QGridLayout();
   aaHelpLayout->addLayout( aaGridLayout );
   aaGridLayout->addWidget( new QLabel( tr( "Method:" ), tab ), 0, 0 );
   m_pSamplingCombo = new QComboBox( tab );
   aaGridLayout->addWidget( m_pSamplingCombo, 0, 1 );
   m_pSamplingCombo->addItem( tr( "Non Recursive" ) );
   m_pSamplingCombo->addItem( tr( "Recursive" ) );

   aaGridLayout->addWidget( new QLabel( tr( "Threshold:" ), tab ), 1, 0 );
   m_pThresholdEdit = new PMFloatEdit( tab );
   aaGridLayout->addWidget( m_pThresholdEdit, 1, 1 );

   aaGridLayout->addWidget( new QLabel( tr( "Depth:" ), tab ), 2, 0 );
   m_pAntialiasDepthEdit = new PMIntEdit( tab );
   m_pAntialiasDepthEdit->setValidation( true, 1, true, 9 );
   aaGridLayout->addWidget( m_pAntialiasDepthEdit, 2, 1 );

   m_pJitterBox = new QCheckBox( tr( "Jitter" ), tab );
   aaGridLayout->addWidget( m_pJitterBox, 3, 0, 1, 2 );

   aaGridLayout->addWidget( new QLabel( tr( "Amount:" ), tab ), 4, 0 );
   m_pJitterAmountEdit = new PMFloatEdit( tab );
   aaGridLayout->addWidget( m_pJitterAmountEdit, 4, 1 );

   aaHelpLayout->addStretch( 1 );

   m_pRadiosityBox = new QCheckBox( tr( "Radiosity" ), tab );
   tabLayout->addWidget( m_pRadiosityBox );

   tabLayout->addStretch( 1 );

   // output options tab
   tab = new QWidget();
   m_pTabWidget->addTab( tab, tr( "Output" ) );
   tabLayout = new QVBoxLayout( tab );
   tabLayout->setSpacing( 1 );
   tabLayout->setMargin( 1 );

   m_pAlphaBox = new QCheckBox( tr( "Alpha" ), tab );
   tabLayout->addWidget( m_pAlphaBox );

   tabLayout->addStretch( 1 );


   resize( s_size );

   // display the mode BEFORE the signals are connected!!!
   displayMode();

   setButton( modeButtonBox->button( QDialogButtonBox::Ok ), false );

   // connect signals
   connect( m_pDescriptionEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotTextChanged( const QString& ) ) );
   connect( m_pHeightEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pWidthEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pSubsectionBox, SIGNAL( toggled( bool ) ), SLOT( slotSubsectionToggled( bool ) ) );
   connect( m_pStartRowEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pEndRowEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pStartColumnEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pEndColumnEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pQualityCombo, SIGNAL( activated( int ) ), SLOT( slotActivated( int ) ) );
   connect( m_pRadiosityBox, SIGNAL( clicked() ), SLOT( slotChanged() ) );
   connect( m_pAntialiasingBox, SIGNAL( toggled( bool ) ), SLOT( slotAntialiasingToggled( bool ) ) );
   connect( m_pSamplingCombo, SIGNAL( activated( int ) ), SLOT( slotActivated( int ) ) );
   connect( m_pThresholdEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pJitterBox, SIGNAL( toggled( bool ) ), SLOT( slotJitterToggled( bool ) ) );
   connect( m_pJitterAmountEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pAntialiasDepthEdit, SIGNAL( dataChanged() ), SLOT( slotChanged() ) );
   connect( m_pAlphaBox, SIGNAL( toggled( bool ) ), SLOT( slotToggled( bool ) ) );
   //connect( modeButtonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( slotOk( QAbstractButton* ) ) );
}

void PMRenderModeDialog::setButton( QPushButton *btn, bool b )
{
   btn->setEnabled( b );
}

void PMRenderModeDialog::saveConfig()
{
   QSettings qset;
   qset.setValue( "Appearance/RenderModeDialogSize", s_size );
}

void PMRenderModeDialog::restoreConfig()
{
   QSettings qset;
   QSize defaultSize( 400, 400 );
   s_size = qset.value( "Appearance/RenderModeDialogSize", defaultSize ).value<QSize>();
}

void PMRenderModeDialog::resizeEvent( QResizeEvent* ev )
{
   s_size = ev->size();
}

bool PMRenderModeDialog::saveChanges()
{
   if( validate() )
   {
      m_mode.setDescription( m_pDescriptionEdit->text() );
      m_mode.setWidth( m_pWidthEdit->value() );
      m_mode.setHeight( m_pHeightEdit->value() );
      m_mode.setSubSection( m_pSubsectionBox->isChecked() );
      if( m_pSubsectionBox->isChecked() )
      {
         m_mode.setStartRow( m_pStartRowEdit->value() );
         m_mode.setEndRow( m_pEndRowEdit->value() );
         m_mode.setStartColumn( m_pStartColumnEdit->value() );
         m_mode.setEndColumn( m_pEndColumnEdit->value() );
      }
      m_mode.setQuality( indexToQuality( m_pQualityCombo->currentIndex() ) );
      m_mode.setRadiosity( m_pRadiosityBox->isChecked() );
      m_mode.setAntialiasing( m_pAntialiasingBox->isChecked() );
      if( m_pAntialiasingBox->isChecked() )
      {
         m_mode.setSamplingMethod( m_pSamplingCombo->currentIndex() );
         m_mode.setAntialiasingThreshold( m_pThresholdEdit->value() );
         m_mode.setAntialiasingJitter( m_pJitterBox->isChecked() );
         if( m_pJitterBox->isChecked() )
            m_mode.setAntialiasingJitterAmount( m_pJitterAmountEdit->value() );
         m_mode.setAntialiasingDepth( m_pAntialiasDepthEdit->value() );
      }
      m_mode.setAlpha( m_pAlphaBox->isChecked() );
      return true;
   }
   return false;
}

bool PMRenderModeDialog::validate()
{
   if( m_pDescriptionEdit->text().isEmpty() )
   {
      QMessageBox::critical( this, "error",
                   "Please enter a description for the render mode",
                   QMessageBox::Ok, QMessageBox::NoButton);
      m_pDescriptionEdit->selectAll();
      return false;
   }

   // tab 0
   bool error = true;

   if( m_pHeightEdit->isDataValid() )
      if( m_pWidthEdit->isDataValid() )
         error = false;
   if( !error && m_pSubsectionBox->isChecked() )
   {
      error = true;
      if( m_pStartColumnEdit->isDataValid() )
         if( m_pEndColumnEdit->isDataValid() )
            if( m_pStartRowEdit->isDataValid() )
               if( m_pEndRowEdit->isDataValid() )
                  error = false;
   }

   if( error )
   {
      m_pTabWidget->setCurrentIndex( 0 );
      return false;
   }

   // tab 1
   if( m_pAntialiasingBox->isChecked() )
   {
      error = false;
      if( m_pThresholdEdit->isDataValid() )
         if( m_pAntialiasDepthEdit->isDataValid() )
            error = false;

      if( m_pJitterBox->isChecked() && !error )
         error = !m_pJitterAmountEdit->isDataValid();

      if( error )
      {
         m_pTabWidget->setCurrentIndex( 1 );
         return false;
      }
   }

   // tab 2

   return true;
}

void PMRenderModeDialog::displayMode()
{
   m_pDescriptionEdit->setText( m_mode.description() );
   m_pHeightEdit->setValue( m_mode.height() );
   m_pWidthEdit->setValue( m_mode.width() );
   m_pSubsectionBox->setChecked( m_mode.subSection() );
   enableSubsection( m_mode.subSection() );
   m_pStartRowEdit->setValue( m_mode.startRow() );
   m_pEndRowEdit->setValue( m_mode.endRow() );
   m_pStartColumnEdit->setValue( m_mode.startColumn() );
   m_pEndColumnEdit->setValue( m_mode.endColumn() );
   m_pQualityCombo->setCurrentIndex( qualityToIndex( m_mode.quality() ) );
   m_pRadiosityBox->setChecked( m_mode.radiosity() );
   m_pAntialiasingBox->setChecked( m_mode.antialiasing() );
   enableAntialiasing( m_mode.antialiasing() );
   m_pSamplingCombo->setCurrentIndex( m_mode.samplingMethod() );
   m_pThresholdEdit->setValue( m_mode.antialiasingThreshold() );
   m_pJitterBox->setChecked( m_mode.antialiasingJitter() );
   enableJitter( m_mode.antialiasingJitter() && m_mode.antialiasing() );
   m_pJitterAmountEdit->setValue( m_mode.antialiasingJitterAmount() );
   m_pAntialiasDepthEdit->setValue( m_mode.antialiasingDepth() );
   m_pAlphaBox->setChecked( m_mode.alpha() );
}

void PMRenderModeDialog::enableSubsection( bool yes )
{
   m_pStartRowEdit->setEnabled( yes );
   m_pEndRowEdit->setEnabled( yes );
   m_pStartColumnEdit->setEnabled( yes );
   m_pEndColumnEdit->setEnabled( yes );
}

void PMRenderModeDialog::enableAntialiasing( bool yes )
{
   m_pSamplingCombo->setEnabled( yes );
   m_pThresholdEdit->setEnabled( yes );
   m_pAntialiasDepthEdit->setEnabled( yes );
   m_pJitterBox->setEnabled( yes );
   enableJitter( m_pJitterBox->isChecked() );
}

void PMRenderModeDialog::enableJitter( bool yes )
{
   m_pJitterAmountEdit->setEnabled( yes );
}

int PMRenderModeDialog::qualityToIndex( int quality )
{
   if( quality < 0 )
      quality = 0;
   if( quality > 11 )
      quality = 11;

   return c_qualityToIndex[quality];
}

int PMRenderModeDialog::indexToQuality( int index )
{
   if( index < 0 )
      index = 0;
   if( index >= numQuality )
      index = numQuality - 1;

   return c_indexToQuality[index];
}
//button->text()=="OK" &&
//void PMRenderModeDialog::slotOk( QAbstractButton *button  )
void PMRenderModeDialog::slotOk()
{
   if( saveChanges() )
      accept();
}

void PMRenderModeDialog::slotChanged()
{
   setButton( modeButtonBox->button( QDialogButtonBox::Ok ), true );
}

void PMRenderModeDialog::slotTextChanged( const QString& )
{
   slotChanged();
}

void PMRenderModeDialog::slotActivated( int )
{
   slotChanged();
}

void PMRenderModeDialog::slotSubsectionToggled( bool on )
{
   slotChanged();
   enableSubsection( on );
}

void PMRenderModeDialog::slotAntialiasingToggled( bool on )
{
   slotChanged();
   enableAntialiasing( on );
}

void PMRenderModeDialog::slotJitterToggled( bool on )
{
   slotChanged();
   enableJitter( on );
}

void PMRenderModeDialog::slotToggled( bool )
{
   slotChanged();
}

#include "pmrendermodesdialog.moc"

