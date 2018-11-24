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

#ifndef PMRESOURCELOCATOR_H
#define PMRESOURCELOCATOR_H



#include <qstring.h>
#include <QCache>

/**
 * Class to find files in the povray library paths
 */
class PMResourceLocator
{
public:
   /**
    * Destructor
    */
   ~PMResourceLocator();
   /**
    * Returns the full path for the file or a null string if the file
    * was not found. The file can be a relative or absolute path.
    */
   static QString findFile( const QString& file );
   /**
    * Clears the resource cache. Call this if the library paths are changed
    */
   static void clearCache();
   
private:
   /**
    * Constructor
    */
   PMResourceLocator();
   /**
    * File lookup function
    */
   QString lookUp( const QString& file );
   static PMResourceLocator* s_pInstance;
   //static K3StaticDeleter<PMResourceLocator> s_staticDeleter;

   QCache< QString, QString > m_cache;
};

#endif
