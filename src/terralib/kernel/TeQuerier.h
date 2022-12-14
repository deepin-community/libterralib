/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright ? 2001-2007 INPE and Tecgraf/PUC-Rio.

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
/*! \file TeQuerier.h
	\brief This file contains a mechanism named "Querier" that is responsible for loading spatio-temporal instances from different sources. 
*/

#ifndef  __TERRALIB_INTERNAL_QUERIER_H
#define  __TERRALIB_INTERNAL_QUERIER_H

#include "TeSTInstance.h"
#include "TeTemporalSeries.h"

#include "TeQuerierDBStr1.h"
#include "TeQuerierDBStr2.h"
#include "TeQuerierDBStr3.h"

class TeQuerierParams;
class TeQuerierImpl; 

/*! \class TeQuerier
	\brief A class responsible for loading spatial temporal instances from different sources.

	This class implements a mechanism that is responsible for loading
	spatio-temporal instances (ST instances) from different sources. This mechanism can 
	load ST instances from a layer or theme stored in a 
	TerraLib database or in a shape file. The Querier is initialized for a set of
	parameters that defines its behavior. 

	\sa
	TeSTInstance TeQuerierParams
*/
class TL_DLL TeQuerier
{
protected:
		TeQuerierImpl*		Impl_;		//!<  querier implementation (handle/bory pattern)
		
public:	
		//! Empty constructor
		TeQuerier():
			Impl_(0)
			{} 
			
		//! Constructor from a set of parameters
		TeQuerier(TeQuerierParams& params);

		//! Copy constructor
		TeQuerier(const TeQuerier& other);

		//! Operator =
		TeQuerier& operator= (const TeQuerier& other);
				
		//! Destructor
		~TeQuerier(); 
		
		//! Returns the base theme
		TeTheme* theme();
		
	 	//! Loads the ST instances. If frame > -1, loads only the instances of the frame-th time frame 
		bool loadInstances(int frame=-1);  
		
		//! Gets the current ST instance and moves to the next one. Returns if there is a next instance. 
		bool fetchInstance(TeSTInstance&  sto);

		//! Returns the number of time frames generated by a specific chronon	
		int getNumTimeFrames(); 

		//! Gets a temporal serie entry associated to a time frame
		bool getTSEntry(TeTSEntry& tsEntry, int frame);
		
		//! Gets the temporal series
		bool getTS(TeTemporalSeries& ts); 
		
		//! Returns the temporal series parameters
		TeTSParams& getTSParams();
		
		//! Gets the attribute list of the instances
		TeAttributeList getAttrList();

		//! Gets the minimal bounding box
		TeBox	getBox();

		//! Returns the number of instances loaded by the method "loadInstances"
		int numElemInstances(); 

		//! Returns the querier parameters
		TeQuerierParams& params(); 

		//! Clear querier structures
		void clear(); 

		//! Refreshes the querier based on a new querier parameters
		void refresh(TeQuerierParams& params); 
		
		//! Loads all geometries of the index-th geometry representation  
		bool loadGeometries(TeMultiGeometry& geometries, unsigned int& index); 

		//! Loads all geometries 
		bool loadGeometries(TeMultiGeometry& geometries);
};

 /** \example querierFromLayer.cpp
	Shows how to use a querier from layer
 */

 /** \example querierFromTheme.cpp
	Shows how to use a querier from theme
 */

 /** \example querierGroupChronon.cpp
	Shows how to querier from theme, grouping spatiotemporal instances by a chronon
 */

 /** \example querierGroupElement.cpp
	Shows how to use a querier from theme, grouping spatiotemporal instances by a element
 */

 /** \example querierGroupSpatialRest.cpp
	Shows how to use a querier from theme, grouping all  
   spatiotemporal instances that satisfy a spatial restriction
 */

 /** \example querierWithSpatialRestBox.cpp
	Shows how to use a querier from theme, using a spatial restriction 
	defined by a rectangle (TeBOX)
 */

 /** \example querierWithSpatialRestGeometry.cpp
	Shows how to use a querier from theme, using a spatial restriction 
	defined by a geometry (polygons, lines, cells and points) from another theme
 */
#endif 



