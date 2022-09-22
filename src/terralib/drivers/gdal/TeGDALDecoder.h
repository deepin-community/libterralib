/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright  2001-2007 INPE and Tecgraf/PUC-Rio.

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

/** \file  TeGDALDecoder.h
    \brief This file contains a concrete raster decoder based on GDAL Library.
    \author Douglas Uba <douglas@dpi.inpe.br>
   */

#ifndef _TEGDALDRIVER_H
#define _TEGDALDRIVER_H

// Defines
#include "TeGDALDefines.h"

// TerraLib include files
#include "TeDecoder.h"

// STL include files
#include <vector>
#include <string>

// forward declarations;
class GDALDataset;

class TEGDAL_DLL TeGDALDecoder : public TeDecoder 
{
	
public:
  
	TeGDALDecoder();

	TeGDALDecoder(const TeRasterParams& par);

    ~TeGDALDecoder();

	//! Initializes the internal structures
	void init();

	//! Releases the internal structures
	bool clear();

	//! Writes an element
	bool setElement(int col, int lin, double val, int band = 0);

	//! Reads an element
	bool getElement(int col, int lin, double &val, int band = 0);

protected:
	
	GDALDataset* _gdalDS; //!< A pointer to GDAL Data Source.

private:
  
    TeGDALDecoder(const TeGDALDecoder& source);

    TeGDALDecoder& operator=(const TeGDALDecoder& source);
};

//! Implements a factory to build decoders based on GDAL library
class TEGDAL_DLL TeGDALDecoderFactory : public TeDecoderFactory
{
public:

	//! Constructor for the factory
	TeGDALDecoderFactory(const string& name);

	//! Built the object
	TeDecoder* build (const TeRasterParams& arg)
	{  return new TeGDALDecoder(arg); }
};

#endif // _TEGDALDRIVER_H
