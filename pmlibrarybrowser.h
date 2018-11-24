//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMLIBRARYBROWSER_H
#define PMLIBRARYBROWSER_H



#include <QWidget>
//Added by qt3to4:
#include <QResizeEvent>
#include "pmviewbase.h"
#include "pmviewfactory.h"



class QComboBox;
class QListWidgetItem;
class QPushButton;
class PMLibraryIconView;
class PMLibraryEntryPreview;
class PMLibraryHandle;
class PMPart;
//class KJob;
//namespace KIO{ class Job; }

/**
 * Wrapper class for the treeview/dock widget
 */
class PMLibraryBrowserView : public PMViewBase
{
   Q_OBJECT
public:
   /**
    * Default constructor
    */
   PMLibraryBrowserView( PMPart* part, QWidget* parent );

   /** */
   virtual QString viewType() const { return QString( "librarybrowserview" ); }
   /** */
   virtual QString description() const;
};

/**
 * This class provides a non-modal dialog to browse the available libraries.
 */
class PMLibraryBrowserViewWidget: public QWidget
{
   Q_OBJECT
public:
   PMLibraryBrowserViewWidget( QWidget *parent );

private slots:
   void resizeEvent( QResizeEvent* ev );
   void slotPathSelected( const QString& str );
   void slotSelectionChanged( QListWidgetItem *item );
   void slotSelectionExecuted( QListWidgetItem *item );
   void slotUpButtonClicked();
   void slotDeleteClicked();
   void slotNewObjectClicked();
   void slotNewSubLibraryClicked();

   //void slotJobResult( KJob * );

   // These slots provide delayed activity on the IconView
   void slotIconViewRefresh();
   void slotIconViewSetLibrary();

private:


   QPushButton*           m_pUpButton;
   QPushButton*           m_pNewSubLibraryButton;
   QPushButton*           m_pNewObjectButton;
   QPushButton*           m_pDeleteObjectButton;
   QComboBox*             m_pLibraryComboBox;
   PMLibraryIconView*     m_pLibraryIconView;
   PMLibraryEntryPreview* m_pLibraryEntryPreview;
   PMLibraryHandle*       m_pCurrentLibrary;
   PMLibraryHandle*       m_pFutureLibrary;
   bool                   m_topLibraryReadOnly;
   QAction*               m_pCopyAction;
};

/**
 * Factory class for the tree view
 */
class PMLibraryBrowserViewFactory : public PMViewTypeFactory
{
public:
   PMLibraryBrowserViewFactory() { }
   virtual QString viewType() const { return QString( "librarybrowserview" ); }
   using PMViewTypeFactory::description;
   virtual QString description() const;
   virtual QString iconName() const { return QString( "pmlibrarybrowserview" ); }
   virtual PMViewBase* newInstance( QWidget* parent, PMPart* part ) const
   {
      return new PMLibraryBrowserView( part, parent );
   }
};

#endif
