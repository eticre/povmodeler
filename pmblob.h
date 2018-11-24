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


#ifndef PMBLOB_H
#define PMBLOB_H



#include "pmsolidobject.h"

/**
 * Class for povray blobs.
 */

class PMBlob : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Creates an empty PMBlob
    */
   PMBlob( PMPart* part );
   /**
    * Copy constructor
    */
   PMBlob( const PMBlob& b );
   /**
    * deletes the PMBlob
    */
   virtual ~PMBlob();

   /** */
   virtual PMObject* copy() const { return new PMBlob( *this ); }
   /** */
   virtual QString description() const;
   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMBlobEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmblob" ); }

   /**
    * Returns the threshold
    */
   double threshold() const { return m_threshold; }
   /**
    * Sets the threshold
    */
   void setThreshold( double t );
   /**
    * Returns the hierarchy flag
    */
   bool hierarchy() const { return m_hierarchy; }
   /**
    * Sets the hierarchy flag
    */
   void setHierarchy( bool h );
   /**
    * Returns the sturm flag
    */
   bool sturm() const { return m_sturm; }
   /**
    * Sets the sturm flag
    */
   void setSturm( bool s );

   /** */
   virtual void restoreMemento( PMMemento* s );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMBlobMementoID { PMThresholdID, PMHierarchyID, PMSturmID };
   double m_threshold;
   bool m_hierarchy;
   bool m_sturm;

   static PMMetaObject* s_pMetaObject;
};

#endif
