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


#ifndef PMOBJECTACTION_H
#define PMOBJECTACTION_H



#include "pmvector.h"
#include "pmcontrolpoint.h"
#include "pmmetaobject.h"
#include <qstring.h>


/**
 * Helper class for object action functions.
 *
 * Each object can define actions that are inserted into the context
 * menu. This class stores information of one action.
 *
 * The actionID has to be unique for each class.
 *
 * The member m_action is set later by the framework.
 */
class PMObjectAction
{
public:
   /**
    * Constructor
    */
   PMObjectAction( PMMetaObject* objectType, int actionID,
                   const QString& description, bool enabled = true )
   {
      m_class = objectType;
      m_actionID = actionID;
      m_description = description;
      m_enabled = enabled;
      m_menuID = -1;
   }

   /**
    * Returns the class type for the action
    */
   PMMetaObject* objectType() const { return m_class; }
   /**
    * Returns the action ID
    */
   int actionID() const { return m_actionID; }
   /**
    * Returns the description
    */
   QString description() const { return m_description; }

   /**
    * Returns the enabled flag
    */
   bool isEnabled() const { return m_enabled; }
   /**
    * Sets the enabled flag
    */
   void setEnabled( bool enable ) { m_enabled = enable; }

   /**
    * Returns the menu id
    */
   int menuID() const { return m_menuID; }
   /**
    * Sets the menu id
    */
   void setMenuID( int id ) { m_menuID = id; }
   
private:
   PMMetaObject* m_class;
   int m_actionID;
   QString m_description;
   bool m_enabled;
   int m_menuID;
};

#endif
