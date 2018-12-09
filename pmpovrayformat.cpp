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

#include "pmpovrayformat.h"
#include "pmdebug.h"

PMPovrayFormat::PMPovrayFormat()
{
   //m_methodDict.setAutoDelete( true );
}


PMPovrayFormat::~PMPovrayFormat()
{

}

void PMPovrayFormat::registerMethod( const QString& className,
                                     PMPovraySerializeMethod method )
{
   PMPovraySerializeMethodInfo* info = m_methodDict.value( className );
   //if( info )
   //   qDebug() << "Serialization method for " << className
   //                       << " shadows old implementation" << endl;
   info = new PMPovraySerializeMethodInfo( method );
   m_methodDict.insert( className, info );
}

void PMPovrayFormat::removeMethod( const QString& className )
{
   m_methodDict.remove( className );
}

const PMPovraySerializeMethodInfo* PMPovrayFormat::serializationMethod(
   const QString& className )
{
   return m_methodDict.value( className );
}
