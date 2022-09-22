#include "TeDXFDriverFactory.h"
#include "TeDXFDriver.h"
#include "TeGeoDataDriver.h"

TeDXFDriverFactory::TeDXFDriverFactory() :
TeGDriverFactory("DXF")
{
}

TeDXFDriverFactory::~TeDXFDriverFactory() 
{
}

TeGeoDataDriver * TeDXFDriverFactory::build(TeGDriverParams * params) 
{
	TeGeoDataDriver* driver = NULL;

	if(params != NULL && !params->fileName_.empty())
		driver = new TeDXFDriver(params->fileName_);

	return driver;
}

TeGeoDataDriver* TeDXFDriverFactory::build() 
{
	return NULL;
}

