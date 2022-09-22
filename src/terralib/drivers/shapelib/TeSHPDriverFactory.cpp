#include "TeSHPDriverFactory.h"
#include "TeGeoDataDriver.h"
#include "TeDriverSHPDBF.h"

TeSHPDriverFactory::TeSHPDriverFactory() :
TeGDriverFactory("SHP")
{
  // Bouml preserved body begin 0005C282
  // Bouml preserved body end 0005C282
}

TeSHPDriverFactory::~TeSHPDriverFactory() 
{
  // Bouml preserved body begin 0005C302
  // Bouml preserved body end 0005C302
}

TeGeoDataDriver * TeSHPDriverFactory::build(TeGDriverParams * params) 
{
  // Bouml preserved body begin 0005C202
	TeShapefileDriver* driver = NULL;

	if(params != NULL && !params->fileName_.empty())
		driver = new TeShapefileDriver(params->fileName_);

	return driver;
  // Bouml preserved body end 0005C202
}

TeGeoDataDriver * TeSHPDriverFactory::build() 
{
  // Bouml preserved body begin 0005C382
	return NULL;
  // Bouml preserved body end 0005C382
}

