//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMPHOTONS_H
#define PMPHOTONS_H



#include "pmobject.h"

/**
 * Class for Photons settings.
 */

class PMPhotons : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates a PMPhotons
    */
   PMPhotons( PMPart* part );
   /**
    * Copy constructor
    */
   PMPhotons( const PMPhotons& p );
   /**
    * deletes the PMPhotons
    */
   virtual ~PMPhotons();

   /** */
   virtual PMObject* copy() const { return new PMPhotons( *this ); }
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
    * Returns a new @ref PMPhotonsEdit
    */
   virtual PMDialogEditBase* editWidget( QWidget* parent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual QString pixmap() const { return QString( "pmphotons" ); }

   /**
    * Returns the target flag
    */
   bool target() const { return m_target; }
   /**
    * Sets the target flag
    */
   void setTarget( bool t );

   /**
    * Returns the spacing multiplier
    */
   double spacingMulti() const { return m_spacingMulti; }
   /**
    * Sets the spacing multipler
    */
   void setSpacingMulti( double sm );

   /**
    * Returns the refraction flag
    */
   bool refraction() const { return m_refraction; }
   /**
    * Sets the refraction flag
    */
   void setRefraction( bool r );

   /**
    * Returns the reflection flag
    */
   bool reflection() const { return m_reflection; }
   /**
    * Sets the reflection flag
    */
   void setReflection( bool r );

   /**
    * Returns the collect flag
    */
   bool collect() const { return m_collect; }
   /**
    * Sets the collect flag
    */
   void setCollect( bool c );

   /**
    * Returns the pass through flag
    */
   bool passThrough() const { return m_passThrough; }
   /**
    * Sets the pass through flag
    */
   void setPassThrough( bool pt );

   /**
    * Returns the area light flag
    */
   bool areaLight() const { return m_areaLight; }
   /**
    * Sets the area light flag
    */
   void setAreaLight( bool al );

   /** */
   virtual void restoreMemento( PMMemento* s );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMPhotonsMementoID{ PMTargetID, PMSpacingMultiID, PMRefractionID,
                            PMReflectionID, PMCollectID, PMPassThroughID,
                            PMAreaLightID };

   bool m_target;
   double m_spacingMulti;
   bool m_refraction;
   bool m_reflection;
   bool m_collect;
   bool m_passThrough;
   bool m_areaLight;
   
   static PMMetaObject* s_pMetaObject;
};

#endif
