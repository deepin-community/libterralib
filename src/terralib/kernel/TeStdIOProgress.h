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
/*! \file TeStdIOProgress.h
    \brief This file an implementation of progress information using stl io stream.
*/
#ifndef __TERRALIB_INTERNAL_STDIOPROGRESS_H
#define __TERRALIB_INTERNAL_STDIOPROGRESS_H

#include <TeProgressBase.h>
//#include <iostream>

//using std::string;
//using std::cout;

class TL_DLL TeStdIOProgress : public TeProgressBase
{
	int totalSteps_;
	int curProg_;
	string messageStr_;
	string captionStr_;

public:
	TeStdIOProgress();

	~TeStdIOProgress();

	//! Sets the total number of steps to n 
	void setTotalSteps(int n);

	//! Resets the progress interface
	void reset();

	//! Cancel activity being excuted
	void cancel();

	//! Sets the label's text
	void setMessage(const string& text);

	//! Returns the label's text
	string getMessage() ;

	//! Returns true whether the process was cancelled
	bool wasCancelled();
	
	//! Sets the caption associated to the progress interface
	void setCaption(const string& cap);

	//! Display progress message
	void setProgress(int steps);
};
#endif
