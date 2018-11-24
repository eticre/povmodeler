//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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


#ifndef PMTEXTUREMAP_H
#define PMTEXTUREMAP_H



#include "pmtexturebase.h"
#include "pmvector.h"

#include <QList>

/**
 * Base class for color, pigment, texture and normal maps
 */

class PMTextureMapBase : public PMTextureBase
{
   typedef PMTextureBase Base;
public:
   /**
    * Creates a PMTextureMapBase
    */
   PMTextureMapBase( PMPart* part );
   /**
    * Copy constructor
    */
   PMTextureMapBase( const PMTextureMapBase& b );
   /**
    * deletes the PMTextureMapBase
    */
   virtual ~PMTextureMapBase();

   /** */
   virtual bool dataChangeOnInsertRemove() const { return true; }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void childAdded( PMObject* o );
   /** */
   using PMCompositeObject::takeChild;
   virtual bool takeChild( PMObject* o );

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns the map object type
    */
   virtual QString mapType() const = 0;

   /**
    * Returns a new @ref PMTextureMapEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;

   /** */
   virtual void createMemento();
   /** */
   virtual void restoreMemento( PMMemento* s );

   /**
    * Returns the map values
    */
   QList<double> mapValues() const { return m_mapValues; }
   /**
    * Sets the map values
    */
   void setMapValues( const QList<double>& v );
   /**
    * Returns the map value for the object o
    */
   double mapValue( const PMObject* o ) const;
   /**
    * Returns the number of map entries
    */
   int mapEntries() const { return m_mapValues.size(); }

private:
   PMObject* nextMapEntry( PMObject* o );
   void stringToValues( const QString& str );
   QString valuesToString() const;
   QList<double>::Iterator valueForChild( PMObject* o );

   /**
    * IDs for @ref PMMementoData
    */
   // enum PMTextureMapBaseMementoID { };

   /**
    * list of map values
    */
   QList<double> m_mapValues;
   /**
    * removed map values
    */
   QList<double> m_removedValues;

   static PMMetaObject* s_pMetaObject;
};


/**
 * Class for texture maps
 */

class PMTextureMap : public PMTextureMapBase
{
public:
   typedef PMTextureMapBase Base;
   /**
    * Creates a texture map
    */
   PMTextureMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMTextureMap( const PMTextureMap& m );
   /**
    * Deletes the texture map
    */
   virtual ~PMTextureMap();

   /** */
   virtual PMObject* copy() const { return new PMTextureMap( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual QString mapType() const { return QString( "Texture" ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmtexturemap" ); }

private:
   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for pigment maps
 */

class PMPigmentMap : public PMTextureMapBase
{
public:
   typedef PMTextureMapBase Base;
   /**
    * Creates a pigment map
    */
   PMPigmentMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMPigmentMap( const PMPigmentMap& m );
   /**
    * Deletes the pigment map
    */
   virtual ~PMPigmentMap();

   /** */
   virtual PMObject* copy() const { return new PMPigmentMap( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual QString mapType() const { return QString( "Pigment" ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmpigmentmap" ); }

private:
   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for color maps
 */

class PMColorMap : public PMTextureMapBase
{
public:
   typedef PMTextureMapBase Base;
   /**
    * Creates a color map
    */
   PMColorMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMColorMap( const PMColorMap& m );
   /**
    * Deletes the color map
    */
   virtual ~PMColorMap();

   /** */
   virtual PMObject* copy() const { return new PMColorMap( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual QString mapType() const { return QString( "SolidColor" ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmcolormap" ); }

private:
   static PMMetaObject* s_pMetaObject;
};


/**
 * Class for normal maps
 */

class PMNormalMap : public PMTextureMapBase
{
public:
   typedef PMTextureMapBase Base;
   /**
    * Creates a normal map
    */
   PMNormalMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMNormalMap( const PMNormalMap& m );
   /**
    * Deletes the normal map
    */
   virtual ~PMNormalMap();

   /** */
   virtual PMObject* copy() const { return new PMNormalMap( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual QString mapType() const { return QString( "Normal" ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmnormalmap" ); }

private:
   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for slope maps
 */

class PMSlopeMap : public PMTextureMapBase
{
public:
   typedef PMTextureMapBase Base;
   /**
    * Creates a slope map
    */
   PMSlopeMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMSlopeMap( const PMSlopeMap& m );
   /**
    * Deletes the slope map
    */
   virtual ~PMSlopeMap();

   /** */
   virtual PMObject* copy() const { return new PMSlopeMap( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual QString mapType() const { return QString( "Slope" ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmslopemap" ); }

private:
   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for density maps
 */

class PMDensityMap : public PMTextureMapBase
{
public:
   typedef PMTextureMapBase Base;
   /**
    * Creates a density map
    */
   PMDensityMap( PMPart* part );
   /**
    * Copy constructor
    */
   PMDensityMap( const PMDensityMap& m );
   /**
    * Deletes the density map
    */
   virtual ~PMDensityMap();

   /** */
   virtual PMObject* copy() const { return new PMDensityMap( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual QString mapType() const { return QString( "Density" ); }

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual QString pixmap() const { return QString( "pmdensitymap" ); }

private:
   static PMMetaObject* s_pMetaObject;
};

#endif
