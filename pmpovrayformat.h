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

#ifndef PMPOVRAY_FORMAT_H
#define PMPOVRAY_FORMAT_H



#include "pmiomanager.h"
#include "pmoutputdevice.h"

typedef void ( *PMPovraySerializeMethod ) ( const PMObject*, const PMMetaObject*,
                                            PMOutputDevice* );
/**
 * Helper class for @ref PMOutputDevice and @ref PMPovrayFormatBase
 */
class PMPovraySerializeMethodInfo
{
public:
   PMPovraySerializeMethodInfo()
   {
      m_method = 0;
   }
   PMPovraySerializeMethodInfo( PMPovraySerializeMethod m )
   {
      m_method = m;
   }
   PMPovraySerializeMethod method() const { return m_method; }
   void call( const PMObject* o, const PMMetaObject* mo, PMOutputDevice* ser ) const
   {
       if( m_method )
         m_method( o, mo, ser );
   }
private:
   PMPovraySerializeMethod m_method;
};

/**
 * Base class for all POV-Ray formats which use a PMOutputDevice
 * for serialization.
 *
 * Plugins can register new serialization methods with @ref registerMethod
 */
class PMPovrayFormat : public PMIOFormat
{
public:
   /**
    * Default constructor
    */
   PMPovrayFormat();
   /**
    * Destructor
    */
   virtual ~PMPovrayFormat();

   /**
    * Registers the serialization method for the class className
    */
   void registerMethod( const QString& className, PMPovraySerializeMethod method );
   /**
    * Removes a registered serialization method
    */
   void removeMethod( const QString& className );
   /**
    * Returns the serialization methods info for the given object type
    * or 0 if there is none.
    */
   const PMPovraySerializeMethodInfo* serializationMethod(
      const QString& className );

private:
   /**
    * Dict class name -> serialization method
    */
   QHash<QString, PMPovraySerializeMethodInfo*> m_methodDict;
};

#endif
