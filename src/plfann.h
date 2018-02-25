
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003 Steffen Nissen (lukesky@diku.dk)

  Binding for SWI Prolog 5 (http://www.swi-prolog.org)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __plfann_h__
#define __plfann_h__

/* Uncomment when using Version 2.2.0 or up */
// #define VERSION220

/* No changes required after this */

#ifndef FANN_DLL_EXPORTS
#define FANN_DLL_EXPORTS
#endif

#if (_MSC_VER < 1500) // VS2008 doesn't need this
#undef vsnprintf
#define vsnprintf _vsnprintf
#endif

/* To use a dll in an application include one of the doublefann.h, fixedfann.h
or floatfann.h header files in the source code and depending on the
number format selected and the settings (debug, release, single or
multithreaded) in the application link with the matching import library.
When executing the application make sure the required dll is in the search
path. For example copy the dll to the same directory as the application or
add win32/bin to the search path (for example on WinXP use the System
applet Configuration Manager and select Advanced and Environment Variables). */

#ifdef DOUBLEFANN
#include <doublefann.h>
#define PL_FANN_FANNTYPE "float" // The SWI-Prolog definition
#define PL_FANN_C_FANNTYPE double
#define PL_FANN_GET_FANNTYPE(X,Y) PL_get_float(X,Y)
#define PL_FANN_UNIFY_FANNTYPE(X,Y) PL_unify_float(X,Y)
#elif defined FIXEDFANN
#include <fixedfann.h>
#define PL_FANN_FANNTYPE "integer"
#define PL_FANN_C_FANNTYPE int
#define PL_FANN_GET_FANNTYPE(X,Y) PL_get_integer(X,Y)
#define PL_FANN_UNIFY_FANNTYPE(X,Y) PL_unify_integer(X,Y)
#else
#include <floatfann.h>
__inline int PL_get_float32 ( term_t in, float* out );
#define PL_FANN_FANNTYPE "float"
#define PL_FANN_C_FANNTYPE double
#define PL_FANN_GET_FANNTYPE(X,Y) PL_get_float32(X,Y)
#define PL_FANN_UNIFY_FANNTYPE(X,Y) PL_unify_float(X,Y)
#endif

#define FANN_UNDEFINED -1

#ifndef __fann_swi_h__
enum enum_fann_mode {

	FANN_NATIVE = 0,
	FANN_SWI
};

enum enum_fann_mode_switch {

	FANN_SET_NATIVE = 0,
	FANN_SET_SWI,
	FANN_GET_MODE
};
#endif

#endif
