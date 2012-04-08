/******************************************************************************
    qlangwidget_global.h: description
    Copyright (C) 2011-2012 Wang Bin <wbsecg1@gmail.com>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/

#ifndef QLANGWIDGET_GLOBAL_H
#define QLANGWIDGET_GLOBAL_H

#include <qglobal.h>

#if QT_VERSION >= 0x040000
# ifndef QT_VERSION4
#  define QT_VERSION4
# endif
#endif

#ifndef QT_VERSION4
#define qPrintable(qstr) qstr.local8Bit().data()
#endif

#if defined(Q_DLL_LIBRARY)
#  undef Q_EXPORT
#  define Q_EXPORT Q_DECL_EXPORT
#else
#  undef Q_EXPORT
#  define Q_EXPORT //Q_DECL_IMPORT //only for vc?
#endif

#endif // QLANGWIDGET_GLOBAL_H

