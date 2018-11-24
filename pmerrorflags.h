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

#ifndef PMERRORFLAGS_H
#define PMERRORFLAGS_H

/**
 * Error flags
 *
 * These flags are returnd by @ref PMParser::errorFlags()
 * and PMCommand::errorFlags()
 */
 
enum PMErrorFlags { PMENone = 0, PMEWarning = 1, PMEError = 2, PMEFatal = 4 };

#endif
