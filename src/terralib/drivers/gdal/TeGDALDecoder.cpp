#include "TeGDALDecoder.h"
#include "TeGDALUtils.h"

// GDAL
#include <gdal_priv.h>

TeGDALDecoder::TeGDALDecoder() :
_gdalDS(0)
{}

TeGDALDecoder::TeGDALDecoder(const TeRasterParams& par)
{
	params_ = par;
	params_.decoderIdentifier_ = "GDAL";
	params_.errorMessage_.clear();
}

TeGDALDecoder::~TeGDALDecoder() 
{
	if(_gdalDS != 0)
		clear();
}

void TeGDALDecoder::init()
{
	// Registers all format drivers built into GDAL.
	GDALAllRegister();

	params_.status_= TeRasterParams::TeNotReady;

	if(params_.mode_ != 'r')
	{
		params_.errorMessage_ = "There is no support for creating or altering a file from GDAL Decoder.";
		return;
	}

	// Opens the raster file
	_gdalDS = (GDALDataset*)GDALOpen(params_.fileName_.c_str(), GA_ReadOnly);
	if(_gdalDS == 0)
	{
		params_.errorMessage_ = "Fail to open the raster file:" + params_.fileName_ + "\n";
		params_.errorMessage_ += CPLGetLastErrorMsg();
		return;
	}

	params_.status_ = TeRasterParams::TeReadyToRead;

	Convert2TerraLib(_gdalDS, params_);
}

bool TeGDALDecoder::clear()
{
	GDALClose((GDALDataset*)_gdalDS);
	_gdalDS = 0;
	return true;
} 

bool TeGDALDecoder::setElement(int col, int lin, double val, int band)
{
	return false; // TODO!
}

bool TeGDALDecoder::getElement(int col, int lin, double& val, int band)
{
	GDALRasterBand* rasterBand = _gdalDS->GetRasterBand(band+1);

	// Reads the pixel value
	CPLErr error = rasterBand->RasterIO(GF_Read, col, lin, 1, 1, &val, 1, 1, GDT_Float64/*?*/, 0, 0);
	if(error == CE_Failure)
		return false;

	return true;
}

TeGDALDecoderFactory::TeGDALDecoderFactory(const string& name):  TeDecoderFactory(name) 
{
	// Registers some file formats...
	TeDecoderFactory::instanceName2Dec()["JPG"]  = "GDAL";	
	TeDecoderFactory::instanceName2Dec()["JPEG"] = "GDAL";
	TeDecoderFactory::instanceName2Dec()["TIF"]  = "GDAL";
	TeDecoderFactory::instanceName2Dec()["TER"]  = "GDAL";
	TeDecoderFactory::instanceName2Dec()["DEM"]  = "GDAL";
	TeDecoderFactory::instanceName2Dec()["BMP"]  = "GDAL";
	TeDecoderFactory::instanceName2Dec()["PNG"]  = "GDAL";
	TeDecoderFactory::instanceName2Dec()["GIF"]  = "GDAL";
	// ... TODO ...
}
