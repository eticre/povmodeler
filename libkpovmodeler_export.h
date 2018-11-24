/*
    This file is part of kpovemodeler
    Copyright (c) 2006 kpovemodeler team

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/


#ifndef _LIBPOVMODELER_EXPORT_H
#define _LIBPOVMODELER_EXPORT_H

//#include <kdemacros.h>
#include <QtCore/QtGlobal>
#ifdef Q_WS_WIN

#ifndef LKPOVMODELER_EXPORT
# ifdef MAKE_LKPOVMODELER_LIB
#  define LKPOVMODELER_EXPORT Q_DECL_EXPORT
# elif KDE_MAKE_LIB
#  define LKPOVMODELER_EXPORT Q_DECL_IMPORT
# else
#  define LKPOVMODELER_EXPORT
# endif
#endif

#else // not windows

#define LKPOVMODELER_EXPORT Q_DECL_EXPORT
#endif /* not windows */

#endif /* _KPOVMODELER_EXPORT_H */
