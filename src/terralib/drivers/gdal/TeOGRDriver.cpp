#include "TeOGRDriver.h"
#include "TeOGRUtils.h"

// TerraLib include files
#include "TeProjection.h"
#include "TeSTElementSet.h"

// OGR
#include <ogrsf_frmts.h>

TeOGRDriver::TeOGRDriver(const std::string& fileName) :
TeGeoDataDriver(),
fileName_(fileName),
numObjs_(0),
ogrLayer_(0)
{
	// Registers all format drivers built into OGR.
	OGRRegisterAll();	
	
	// Open OGR Datasource
	ogrDS_ = OGRSFDriverRegistrar::Open(fileName.c_str());
	
	/* TODO: The data set may has one or more layers. Is necessary a revision on TeGeoDataDriver interface?
			 For while, only first layer is considered by the driver. */

	if(ogrDS_ != 0)
	{
		int nLayers = ogrDS_->GetLayerCount();
		if(nLayers > 0)
			ogrLayer_ = ogrDS_->GetLayer(0); // Gets the first layer
	}
}

TeOGRDriver::~TeOGRDriver() 
{
	OGRDataSource::DestroyDataSource(ogrDS_);
	ogrDS_ = 0;
}

std::string TeOGRDriver::getFileName() 
{
	return fileName_;
}

bool TeOGRDriver::isDataAccessible() 
{
	return TeCheckFileExistence(fileName_) && (ogrDS_ != 0);
}

TeProjection* TeOGRDriver::getDataProjection() 
{
	if(ogrLayer_ == 0)
		return new TeNoProjection();

	OGRSpatialReference* srs = ogrLayer_->GetSpatialRef();
	if(srs == 0)
		return new TeNoProjection();

	if(srs->AutoIdentifyEPSG() == OGRERR_UNSUPPORTED_SRS)
		return new TeNoProjection();

	TeProjection* proj = TeProjectionFactory::make(atoi(srs->GetAuthorityCode(0)));

	return proj;
}

bool TeOGRDriver::getDataInfo(unsigned int& nObjects, TeBox& ext, TeGeomRep& repres) 
{
	if(ogrLayer_ == 0)
		return false;

	// Number of objects
	nObjects = ogrLayer_->GetFeatureCount();

	// Box
	OGREnvelope* env = new OGREnvelope();
	ogrLayer_->GetExtent(env);
	ext = Convert2TerraLib(env);
	delete env;
	
	// Geometric representation
	repres = (TeGeomRep)Convert2TerraLib(ogrLayer_->GetLayerDefn()->GetGeomType());

	return true;
}

bool TeOGRDriver::getDataAttributesList(TeAttributeList& attList) 
{
	if(ogrLayer_ == 0)
		return false;
	
	attList = Convert2TerraLib(ogrLayer_->GetLayerDefn());

	return true;
}

bool TeOGRDriver::loadData(TeSTElementSet* dataSet) 
{
	if(ogrLayer_ == 0)
		return false;

	TeAttributeList attrList = Convert2TerraLib(ogrLayer_->GetLayerDefn());
	dataSet->setAttributeList(attrList);

	ogrLayer_->ResetReading();
	OGRFeature* feat;
	while((feat = ogrLayer_->GetNextFeature()) != 0)
	{
		dataSet->insertSTInstance(Convert2TerraLib(feat));
	}

	return true;
}

void TeOGRDriver::getDriversNames(std::vector<std::string>& dnames)
{
	int ndrivers = OGRSFDriverRegistrar::GetRegistrar()->GetDriverCount();
	for(int i = 0; i < ndrivers; ++i)
	{
		OGRSFDriver* driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriver(i);
		dnames.push_back(driver->GetName());
	}
}
