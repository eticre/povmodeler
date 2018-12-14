/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QLocale>
#include "pmshell.h"
#include "pmrendermanager.h"
#include "version.h"

int main( int argc, char* argv[] )
{
   PMShell* shell = nullptr;

    QApplication app( argc, argv ); // PORTING SCRIPT: move this to before the KAboutData initialization
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process( app ); // PORTING SCRIPT: move this to after any parser.addOption
    parser.addPositionalArgument( QLatin1String( "[file]" ), "File to open" );
    parser.addOption( QCommandLineOption( QStringList() << QLatin1String( "no-dri" ),  "Disables direct rendering" ) );

    QCoreApplication::setOrganizationName( "povmodeler" );
    QCoreApplication::setApplicationName( "povmodeler" );

   if( parser.positionalArguments().count() > 0 )
   {
      for( int i = 0 ; i < parser.positionalArguments().count() ; i++ )
      {
         shell = new PMShell( parser.positionalArguments().at( i ) );
         shell->show();
      }
   }
   else
   {
      shell = new PMShell;
      shell->show();
   }

   parser.clearPositionalArguments();
   return app.exec();
}
