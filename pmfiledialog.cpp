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


#include "pmfiledialog.h"
#include "pmpart.h"

//#include <kfilefiltercombo.h>

//Added by qt3to4:


PMFileDialog::PMFileDialog( QWidget* parent, const QString& caption, const QString& startDir, const QString& filter)
      : QFileDialog( parent, "File", startDir, filter  )
{
}

QString PMFileDialog::getImportFileName( QWidget* parent, PMPart* part, PMIOFormat*& format )
{
   QString filter;
   QList< PMIOFormat* > formats;

   foreach( PMIOFormat* it, part->ioManager()->formats() )
   {
      if( it->services() & PMIOFormat::Import )
      {
     foreach( const QString& pit, it->importPatterns() )
         {
            if( !filter.isEmpty() )
               filter += '\n';
            filter += pit;
            formats.append( it );
         }
      }
   }

   Q_ASSERT( formats.count() );

   //PMFileDialog dlg( parent, "File Import", QString(), filter);

   return PMFileDialog::getOpenFileName( parent, tr( "File Import" ), "/home/", filter );

   //dlg.setObjectName("import file dialog");
   //dlg.setOperationMode( Opening );
   //dlg.setMode( KFile::File | KFile::LocalOnly );
   //dlg.setWindowTitle( tr( "Import" ) );
   //dlg.filterWidget()->setEditable( false );
   //dlg.exec();

   //format = formats.at( qMax ( 0, dlg.filterWidget()->currentIndex() ) );

   //return dlg.selectedFiles();
}

QString PMFileDialog::getExportFileName( QWidget* parent, PMPart* part,
                                         PMIOFormat*& format, QString& selectedFilter )
{
   QString filter;
   QList< PMIOFormat* > formats;

   foreach( PMIOFormat* it, part->ioManager()->formats() )
   {
      if( it->services() & PMIOFormat::Export )
      {
     foreach(const QString& pit, it->exportPatterns() )
         {
            if( !filter.isEmpty() )
               filter += '\n';
            filter += pit;
            formats.append( it );
         }
      }
   }

   Q_ASSERT( formats.count() );
   /*
   PMFileDialog dlg( QString(), filter, parent, true );
    dlg.setObjectName("export file dialog");
   dlg.setOperationMode( Saving );
   dlg.setMode( KFile::File | KFile::LocalOnly );
   dlg.setWindowTitle( tr( "Export" ) );
   dlg.filterWidget()->setEditable( false );
   dlg.exec();
   */
   //format = formats.at( qMax ( 0, dlg.filterWidget()->currentIndex() ) );
   //selectedFilter = dlg.currentFilter();
   return PMFileDialog::getSaveFileName( parent, tr( "File Export" ), "/home/", filter );
   //return dlg.selectedFile();
}

#include "pmfiledialog.moc"
