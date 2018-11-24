/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#include <QLocale>
//#include <kcomponentdata.h>
//#include <KAboutData>
#include <KLocalizedString>
#include <libkpovmodeler_export.h>
#include "pmfactory.h"
#include "pmpart.h"
#include "pmdebug.h"
#include "version.h"

extern "C"
{
   KDE_EXPORT void* init_libkpovmodelerpart( )
   {
      return new PMFactory( );
   }
}

static const char description[] = I18N_NOOP( "Modeler for POV-Ray Scenes" );

//KComponentData *PMFactory::s_instance = 0L;
KAboutData* PMFactory::s_aboutData = 0L;

PMFactory::PMFactory( )
{
   qDebug( ) << "PMFactory::PMFactory( )\n";
}

PMFactory::~PMFactory( )
{
   if( s_instance )
      delete s_instance;
   if( s_aboutData )
      delete s_aboutData;

   s_instance = 0L;
   s_aboutData = 0L;
}

KParts::Part* PMFactory::createPartObject( QWidget* parentWidget,
                                     QObject* parent, 
                                     const char* classname,
                                     const QStringList& /*args*/ )
{
   qDebug( ) << "PMFactory: Created new part\n";

   bool readwrite = !( ( strcmp( classname, "Browser/View" ) == 0 )
          || ( strcmp( classname, "KParts::ReadOnlyPart" ) == 0 ) );

   KParts::ReadWritePart *part = new PMPart( parentWidget, parent, readwrite );

   return part;
}
/*
const KComponentData &PMFactory::componentData( )
{
   if( !s_instance )
      s_instance = new KComponentData( aboutData( ) );
   return *s_instance;
}
*/
const KAboutData *PMFactory::aboutData( )
{
   if( !s_aboutData )
   {
      s_aboutData =
         new KAboutData( "kpovmodeler", 0, ki18n( "KPovModeler" ),
                         KPOVMODELER_VERSION, ki18n(description),
                         KAboutData::License_GPL, ki18n("(c) 2001-2002, Andreas Zehender") );
      s_aboutData->addAuthor( ki18n("Andreas Zehender"), KLocalizedString(),
                              "zehender@kde.org", "http://www.azweb.de" );
      s_aboutData->addAuthor( ki18n("Luis Passos Carvalho"), ki18n( "Textures" ),
                              "lpassos@mail.telepac.pt" );
      s_aboutData->addAuthor( ki18n("Leon Pennington"), ki18n( "POV-Ray 3.5 objects" ),
                              "leon@leonscape.co.uk" );
      s_aboutData->addAuthor( ki18n("Philippe Van Hecke"), ki18n( "Some graphical objects" ),
                              "lephiloux@tiscalinet.be" );
      s_aboutData->addAuthor( ki18n("Leonardo Skorianez"), ki18n( "Some graphical objects" ),
                              "skorianez@bol.com.br" );
   }
   return s_aboutData;
}
#include "pmfactory.moc"
