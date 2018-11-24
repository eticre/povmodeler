//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMCOMMAND_H
#define PMCOMMAND_H



#include <qstring.h>

#include "pmmessage.h"
#include "pmerrorflags.h"

class PMCommandManager;
class PMPart;

/**
 * Constants for object changes
 *
 * PMCAdd: An object was added
 *
 * PMCRemove: An object was removed. A signal with this constant has
 * to be emitted _before_ the object is removed.
 *
 * PMCChildren: Some children are added or removed. Do not use. Use PMCAdd and
 * PMCRemove for all childrens.
 *
 * PMCData: Data (for dialog views) is changed
 *
 * PMCDescription: The name or pixmap of the object has changed. Always
 * together with PMCData.
 *
 * PMCViewStructure: The rendering has changed
 *
 * PMCNewSelection: The object was selected and all other deleselected.
 * If the changed object is 0, all objects are deselected!
 *
 * PMCSelected: The object was selected.
 *
 * PMCDeselected: The object was deselected.
 *
 * PMCNewControlPoints: The list of control points has changed.
 *
 * PMCControlPointSelection: The control point selection has changed.
 *
 * PMCInsertError: The added object couldn't be inserted and was appended
 * to the insert errors. Always together with PMCAdd.
 *
 * PMCNothing: Nothing was changed
 */
enum PMChange
{
   PMCNothing = 0, PMCAdd = 1, PMCRemove = 2, PMCChildren = 4,
   PMCData = 8, PMCDescription = 16, PMCViewStructure = 32,
   PMCGraphicalChange = 64,
   PMCNewSelection = 128, PMCSelected = 256,
   PMCDeselected = 512,
   PMCNewControlPoints = 1024, PMCControlPointSelection = 2048,
   PMCInsertError = 4096
};

/**
 * Base class for all commands, that support undo/redo.
 */
class PMCommand
{
   friend class PMCommandManager; // only PMCommandManager can execute commands
public:
   /**
    * Creates an empty command object.
    */
   PMCommand() { }
   /**
    * Creates a command with command text text
    */
   PMCommand( const QString &text )
   { m_text = text; }
   /**
    * Deletes the command.
    */
   virtual ~PMCommand() { }

   /**
    * Command text shown in the undo/redo menues
    */
   QString text() const { return m_text; }
   /**
    * Sets the command text
    */
   void setText( const QString& s ) { m_text = s; }

   /**
    * Checks if the command is valid and sets the error message
    *
    * Returns a bitwise combination of @ref PMErrorFlags flags
    */
   virtual int errorFlags( PMPart* ) { return PMENone; }
   /**
    * Returns the error message
    */
   PMMessageList messages() { return m_errors; }

protected:
   /**
    * Executes the command and stores undo information.
    */
   virtual void execute( PMCommandManager* theManager ) = 0;
   /**
    * Undoes the command
    */
   virtual void undo( PMCommandManager* theManager ) = 0;
   /**
    * The error messages
    */
   PMMessageList m_errors;

private:
   /**
    * The command text.
    */
   QString m_text;
};

#endif
