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
/*! \file TeWaitCursor.h
    \brief This file supports the definition of a waiting cursor using the Qt toolkit
*/
#ifndef  __TERRALIB_INTERNAL_WAITCURSOR_H
#define  __TERRALIB_INTERNAL_WAITCURSOR_H

#include <TeAppUtilsDefines.h>

#include <qcursor.h>

/** @defgroup QtDriver Interfaces to Qt Toolkit
	@ingroup Drivers
	TerraLib routines build on top of Qt Toolkit
	@{
*/
//! A waiting cursor
class TLAPPUTILS_DLL TeWaitCursor : public QCursor  {
public: 
	//! Constructor
	TeWaitCursor();
	
	//! Destructor
	~TeWaitCursor();

	//! Instantiate a waiting cursor
	void setWaitCursor();

	//! Reset the waiting cursor
	void resetWaitCursor();
};
/** @} */
#endif
