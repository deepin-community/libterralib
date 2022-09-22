// OGR Driver include files
#include "TeGDALUtils.h"

// TerraLib include files
#include <TeRaster.h>

void Convert2TerraLib(GDALDataset* ds, TeRasterParams& params)
{
	// Number of collumns
	params.ncols_  = ds->GetRasterXSize();
	// Number of lines
	params.nlines_ = ds->GetRasterYSize();
	// Number of bands
	params.nBands(ds->GetRasterCount());

	// For each band, adjusts params
	for(int b = 0; b < params.nBands(); b++)
	{
		GDALRasterBand* band = ds->GetRasterBand(b+1);
		Convert2TerraLib(band, params, b);
	}

	double geoTransformParams[6];
	CPLErr error = ds->GetGeoTransform(geoTransformParams);
	if(error == CE_Failure) // format does not support transformation to projection coordinates
	{
		// Builds the Box
		double llx = 0;
		double lly = 0;
		double urx = (double)params.ncols_;
		double ury = (double)params.nlines_;

		params.boundingBoxLinesColumns(llx, lly, urx, ury, params.nlines_, params.ncols_);
		
		return;
	}

	// Calculates box and pixel resolution
	Convert2TerraLib(geoTransformParams, params);
}

void Convert2TerraLib(GDALRasterBand* rasterBand, TeRasterParams& params, const int& i)
{
	// Gets the dummy value
	int noDataValueIsUsed = 0;
	double dummy = rasterBand->GetNoDataValue(&noDataValueIsUsed);
	if(!noDataValueIsUsed)
	{
		params.useDummy_ = false;
		dummy = TeMAXFLOAT;
	}
	params.setDummy(dummy, i);

	// Gets the DataType
	GDALDataType type = rasterBand->GetRasterDataType();
	params.setDataType(Convert2TerraLib(type), i);
}

void Convert2TerraLib(double* geoTransformParams, TeRasterParams& params)
{
	// Gets the resolution
	params.resx_ = geoTransformParams[1];
	params.resy_ = (-1.0) * geoTransformParams[5];

	// Builds the Box
	double llx = geoTransformParams[0];
	double lly = geoTransformParams[3] + ((double)params.nlines_ * geoTransformParams[5]);
	double urx = geoTransformParams[0] + ((double)params.ncols_  * geoTransformParams[1]);
	double ury = geoTransformParams[3];

	params.boundingBoxLinesColumns(llx, lly, urx, ury, params.nlines_, params.ncols_);
}

TeDataType Convert2TerraLib(GDALDataType type)
{
	// Convert GDAL DataType to TerraLib
	switch(type)
	{
		case GDT_Byte: 
			return TeUNSIGNEDCHAR;
		break;
		
		case GDT_UInt16: 
		case GDT_UInt32:
			return TeUNSIGNEDLONG;
		break;

		case GDT_Int16:
		case GDT_Int32:
			return TeINTEGER;
		break;

		case GDT_Float32: 
			return TeFLOAT;
		break;
		
		case GDT_Float64: 
			return TeDOUBLE;
		break;

		default:
			return TeUNSIGNEDCHAR;
	}
}
