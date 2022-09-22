#include "TeOGRDriverFactory.h"
#include "TeOGRDriver.h"

// TerraLib include files
#include "TeGeoDataDriver.h"

TeOGRDriverFactory::TeOGRDriverFactory() :
TeGDriverFactory("OGR")
{}

TeOGRDriverFactory::~TeOGRDriverFactory() 
{}

TeGeoDataDriver * TeOGRDriverFactory::build(TeGDriverParams * params) 
{
	TeGeoDataDriver* driver = 0;

	if(params != 0 && !params->fileName_.empty())
		driver = new TeOGRDriver(params->fileName_);

	return driver;
}

TeGeoDataDriver* TeOGRDriverFactory::build() 
{
	return 0;
}
