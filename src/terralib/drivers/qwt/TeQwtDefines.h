/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/
/*! \file TeQwtDefines.h
    \brief Provides a set of general definitions used by te_qwt project
*/
#ifndef  __TEQWT_INTERNAL_DEFINES_H
#define  __TEQWT_INTERNAL_DEFINES_H

#include <string> 

/** @defgroup Defines Defines and constants
  @{
 */

/** @defgroup TLQWT_AS_DLL macros.
  @{
 */
#if defined( WIN32 ) || defined( __WIN32__ ) || defined ( _WIN32 ) || defined( WIN64 ) || defined( _WIN32_WCE )

#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning(disable: 4251)
#endif

#ifdef TLQWT_AS_DLL
#define TLQWT_DLL __declspec(dllexport)
#else
#define TLQWT_DLL __declspec(dllimport)
#endif

#else
#define TLQWT_DLL
#endif
/** @} */ 

#endif //__TEQWT_INTERNAL_DEFINES_H

