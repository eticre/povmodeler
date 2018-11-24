//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2006 by Andreas Zehender
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


#ifndef PMSCENE_H
#define PMSCENE_H



#include "pmcompositeobject.h"
#include "pmrendermode.h"

/**
 * Class for povray scenes.
 *
 * A document has a PMScene object as top level object. A PMScene can't have
 * a parent
 */
class PMScene : public PMCompositeObject
{
   typedef PMCompositeObject Base;
public:
   /**
    * Creates an empty PMScene
    */
   PMScene( PMPart* part );
   /**
    * Copy constructor
    */
   PMScene( const PMScene& s );
   /**
    * deletes the scene and all objects
    */
   virtual ~PMScene();

   /** */
   virtual PMMetaObject* metaObject() const;
   /** */
   virtual void cleanUp() const;

   /** */
   virtual PMObject* copy() const { return new PMScene( *this ); }
   /** */
   virtual QString description() const;

   /** */
   virtual QString pixmap() const { return QString( "pmscene" ); }

   /** */
   virtual void serialize( QDomElement& e, QDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns the list of render modes
    * @see PMRenderMode
    */
   PMRenderModeList renderModes() const { return m_renderModes; }
	/**
	 * Sets render modes
	 */
	void setRenderModes( const PMRenderModeList& list );
	/**
	 * Sets the active render mode
	 */
	void setActiveRenderMode( int i );
	/**
	 * returns the active render mode or 0 if there is none
	 */
	const PMRenderMode* activeRenderMode() const;
	/**
	 * Returns the index of the active render mode or -1 if there is none
	 */
	int activeRenderModeIndex() const { return m_activeMode; }
   /**
    * Returns the scenes visibility level
    */
   int visibilityLevel() const { return m_visibilityLevel; }
   /**
    * Sets the visibility level
    */
   void setVisibilityLevel( int l ) { m_visibilityLevel = l; }

private:
   PMRenderModeList m_renderModes;
   int m_activeMode;
   int m_visibilityLevel;

   static PMMetaObject* s_pMetaObject;
};

#endif
