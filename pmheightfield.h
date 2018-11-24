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


#ifndef PMHEIGHTFIELD_H
#define PMHEIGHTFIELD_H



#include "pmsolidobject.h"

class PMViewStructure;
class PMHeightFieldROAM;

/**
 * Class for povray height fields.
 */

class PMHeightField : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Height field type
    */
   enum HeightFieldType { HFgif, HFtga, HFpot, HFpng, HFpgm, HFppm, HFsys };
   /**
    * Creates an empty PMHeightField
    */
   PMHeightField( PMPart* part );
   /**
    * Copy constructor
    */
   PMHeightField( const PMHeightField& f );
   /**
    * deletes the PMHeightField
    */
   virtual ~PMHeightField();

   /** */
   virtual PMObject* copy() const { return new PMHeightField( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual PMMetaObject* metaObject() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMHeightFieldEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmheightfield" ); }

   /**
    * Returns the height field type
    */
   HeightFieldType heightFieldType() const { return m_hfType; }
   /**
    * Sets the height field type
    */
   void setHeightFieldType( HeightFieldType t );
   /**
    * Returns the file name
    */
   QString fileName() const { return m_fileName; }
   /**
    * Sets the file name
    */
   void setFileName( const QString& name );
   /**
    * Returns the hierarchy flag
    */
   bool hierarchy() const { return m_hierarchy; }
   /**
    * Sets the hierarchy flag
    */
   void setHierarchy( bool h );
   /**
    * Returns the smooth flag
    */
   bool smooth() const { return m_smooth; }
   /**
    * Sets the smooth flag
    */
   void setSmooth( bool s );
   /**
    * Returns the water level
    */
   double waterLevel() const { return m_waterLevel; }
   /**
    * Sets the water level
    */
   void setWaterLevel( double wl );

   /**
    * Sets the heightfield variance
    */
   static void setVariance( int v );
   /**
    * Returns the heightfield variance
    */
   static int variance() { return s_variance; }

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual bool hasDisplayDetail() const { return true; }
   /** */
   virtual void cleanUp() const;

   static QString typeToString( HeightFieldType t );
   static HeightFieldType stringToType( const QString &str );

protected:
   /** */
   virtual bool isDefault();
   /** */
   virtual void createViewStructure();
   /** */
   virtual PMViewStructure* defaultViewStructure() const;
   /** */
   virtual int viewStructureParameterKey() const { return s_parameterKey + globalDetailKey(); }

private:
   /**
    * Creates the ROAM view structure
    */
   void roamViewStructure();

   /**
    * IDs for @ref PMMementoData
    */
   enum PMHeightFieldMementoID { PMHeightFieldTypeID, PMFileNameID,
                                 PMHierarchyID, PMSmoothID, PMWaterLevelID };

   HeightFieldType m_hfType;
   QString m_fileName;
   bool m_hierarchy;
   bool m_smooth;
   double m_waterLevel;

   bool m_modMap;
   PMHeightFieldROAM* m_pROAM;

   /**
    * The default view structure. It can be shared between height fields
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static int s_variance;
   static int s_parameterKey;

   static PMMetaObject* s_pMetaObject;
};

#endif
