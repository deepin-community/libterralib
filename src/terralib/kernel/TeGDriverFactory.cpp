#include "TeGDriverFactory.h"

TeGDriverParams::TeGDriverParams(const string & fileType, const string & fileName) 
{
  // Bouml preserved body begin 0005BE02
	fileType_ = fileType;
	fileName_ = fileName;
  // Bouml preserved body end 0005BE02
}

string TeGDriverParams::getProductId() const 
{
  // Bouml preserved body begin 0005C602
	return fileType_;
  // Bouml preserved body end 0005C602
}

TeGDriverFactory::TeGDriverFactory(const string & facName) :
TeAbstractFactory<TeGeoDataDriver, TeGDriverParams, string>(facName)
{
  // Bouml preserved body begin 0005C002
  // Bouml preserved body end 0005C002
}

TeGDriverFactory::~TeGDriverFactory() 
{
  // Bouml preserved body begin 0005C082
  // Bouml preserved body end 0005C082
}

