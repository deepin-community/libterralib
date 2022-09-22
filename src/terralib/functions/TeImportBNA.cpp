/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright © 2001-2007 INPE and Tecgraf/PUC-Rio.

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

#include <string>
#include <iostream>
#include <list>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

using namespace std;

#include "TeDriverBNA.h"
#include "TeTable.h"
#include "TeGeometry.h"
#include "TeAsciiFile.h"
#include "TeException.h"
#include "TeProjection.h"
#include "TeAttribute.h"
#include "TeTable.h"
#include "TeAssertions.h"
#include "TeUtils.h"
#include "TeLayer.h"
#include "TeGeometryAlgorithms.h"
#include "TeDatabase.h"

bool TeReadBNAObjects(TeAsciiFile& bnaFile, TeTable& attTable, TePointSet& ptSet,
					  TeLineSet& lnSet, TePolygonSet& pSet, 
					  int ncolumns, int nrecords, int delta, int attLinkColumnIndex);

bool TeBNARegionDecode(TeAsciiFile& bnaFile, TePolygonSet& temp, string& index, const int& npoints);

bool TeBNALineDecode(TeAsciiFile& bnaFile, TeLineSet& temp, string& index, const int& npoints);

bool TeBNAPointDecode(TeAsciiFile& bnaFile, TePointSet& temp, string& index);



TeLayer* TeImportBNA(const string& bnaFileName, TeDatabase* db, const string& layerName)
{
	if(!db || bnaFileName.empty()) 
		return 0;

	// check if format is complete (BNA file exist)
	string filePrefix = TeGetName(bnaFileName.c_str());
	string ftest = filePrefix + ".bna";
	if(access(ftest.c_str(),04) == -1)
	{
		ftest = filePrefix + ".BNA";
		if (access(ftest.c_str(),04) == -1)
			return 0;
	}

	// found a valid layer name
	string lName = layerName;

	if(layerName.empty())
		lName = TeGetBaseName(bnaFileName.c_str());	

	string newLayerName = lName;

	TeLayerMap& layerMap = db->layerMap();
	TeLayerMap::iterator it;

	bool flag = true;

	int n = 1;

	while(flag)
	{
		for(it = layerMap.begin(); it != layerMap.end(); ++it)
		{
			if(TeStringCompare(it->second->name(), newLayerName))
				break;
		}

		if(it == layerMap.end())
			flag = 0;
		else
			newLayerName = lName + "_" + Te2String(n);

		n++;	
	}

	// Use no projection
	TeLayer* newLayer = new TeLayer(newLayerName, db);

	if(newLayer->id() <= 0)
		return 0;				// couldn´t create new layer

	TeAttributeList attList;

	bool res = TeImportBNA(newLayer, bnaFileName, "", attList, 60);

	if (res)
		return newLayer;
	else
	{
		db->deleteLayer(newLayer->id());

		delete newLayer;

		return 0;
	}
}

bool TeImportBNA(TeLayer* layer, const string& bnaFileName, string attrTableName,
				 TeAttributeList& attList, int unsigned chunkSize, const string& linkCol)
{
	if(chunkSize <= 0)
		chunkSize = 1;

	//-- Step 1: Read any adicional information (ex. projection) 
	char separator = ',';


	if(!TeReadBNAAttributeList(bnaFileName, attList))
		return false;	// Attributelist read error


	string linkColName = linkCol;

	if(linkColName.empty())
		linkColName = attList[0].rep_.name_;

	int attLinkColumnIndex = 0;

	for(unsigned int i = 0; i < attList.size(); ++i)
	{
		if(attList[i].rep_.name_ == linkColName)
		{
			attLinkColumnIndex = (int)i;
			break;
		}
	}
		

	// define a TeAttributeTable
	if (attrTableName.empty())
	{
		if (layer->name().empty())
			return false;
		else
			attrTableName = layer->name();
	}
	
	TeTable attTable(attrTableName, attList, linkColName, linkColName, TeAttrStatic);

	attTable.setSeparator(separator);

	// insert the table into the database
	if(!layer->createAttributeTable(attTable))
		return false;

	int ncol = attList.size();

	TeAsciiFile bnaFile(bnaFileName);

	int delta = 0;

	TePointSet ptSet;
	TeLineSet lnSet;
	TePolygonSet pSet;

	while(TeReadBNAObjects(bnaFile, attTable, ptSet, lnSet, pSet, ncol, chunkSize, delta, attLinkColumnIndex))
	{
		// save table
		if (!layer->saveAttributeTable( attTable ))
		{
			attTable.clear();
			break;
		}

		// save the remaining geometries
		if(ptSet.size() > 0)
		{
			layer->addPoints(ptSet); 
			ptSet.clear();
		}

		if(lnSet.size() > 0)
		{
			layer->addLines(lnSet);
			lnSet.clear();
		}

		if(pSet.size() > 0)
		{
			layer->addPolygons(pSet); 
			pSet.clear();
		}

		delta += attTable.size();
		
		attTable.clear();
	}	

	// Create the spatial indexes
	int rep = layer->geomRep();
	if (rep & TePOINTS)
	{
		layer->database()->insertMetadata(layer->tableName(TePOINTS),layer->database()->getSpatialIdxColumn(TePOINTS), 0.0005,0.0005,layer->box());
		layer->database()->createSpatialIndex(layer->tableName(TePOINTS),layer->database()->getSpatialIdxColumn(TePOINTS), (TeSpatialIndexType)TeRTREE);
	}
	if (rep & TeLINES)
	{
		layer->database()->insertMetadata(layer->tableName(TeLINES),layer->database()->getSpatialIdxColumn(TeLINES), 0.0005,0.0005,layer->box());
		layer->database()->createSpatialIndex(layer->tableName(TeLINES),layer->database()->getSpatialIdxColumn(TeLINES), (TeSpatialIndexType)TeRTREE);
	}
	if (rep & TePOLYGONS)
	{
		layer->database()->insertMetadata(layer->tableName(TePOLYGONS),layer->database()->getSpatialIdxColumn(TePOLYGONS), 0.0005,0.0005,layer->box());
		layer->database()->createSpatialIndex(layer->tableName(TePOLYGONS),layer->database()->getSpatialIdxColumn(TePOLYGONS), (TeSpatialIndexType)TeRTREE);
	}
	return true;
}


bool TeReadBNAAttributeList(const string& bnaFileName, TeAttributeList& attList)
{
	TeAsciiFile bnaFile(bnaFileName);

	if(!bnaFile.isNotAtEOF())
		 return false;

	vector<string> strList;

	bnaFile.readStringListCSV(strList, ',');

	unsigned int nFields = (strList.size() - 1u);

	if(nFields <= 0)
		return false;

	if(attList.size() > 0u)	// Has already defined attributes, only check the number
	{
		if(attList.size() != nFields)
			return false;
		else
			return true;
	}

	// In future, look for a better way to find the field name
	// see in the specification of file format there is something about the field names!

	TeAttribute at1;
	at1.rep_.type_ = TeSTRING;
	at1.rep_.numChar_ = 255;		
	at1.rep_.name_ = "IBGE_CODE";
	at1.rep_.isPrimaryKey_ = false;
	attList.push_back(at1);

	if(nFields > 1)
	{
		TeAttribute at2;
		at2.rep_.type_ = TeSTRING;
		at2.rep_.numChar_ = 255;		
		at2.rep_.name_ = "IBGE_NAME";
		at2.rep_.isPrimaryKey_ = false;
		attList.push_back(at2);
	}

	for(unsigned int i = 2; i < nFields; ++i)
	{
		TeAttribute at;
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 255;		
		at.rep_.name_ = string("FIELD_") + Te2String(i);
		at.rep_.isPrimaryKey_ = false;
		attList.push_back(at);
	}

	return true;
}


bool TeReadBNAObjects(TeAsciiFile& bnaFile, TeTable& attTable, TePointSet& ptSet,
					  TeLineSet& lnSet, TePolygonSet& pSet, 
					  int ncolumns, int nrecords, int /* delta */, int attLinkColumnIndex)
{
	if (!bnaFile.isNotAtEOF() || nrecords <=0 || ncolumns <= 0)
		return false;

	char separator = attTable.separator();

	int count;

	for(count=0; count < nrecords; ++count)
	{
		if(!bnaFile.isNotAtEOF())
			 break;

		// Ler os atributos do objeto e o tipo/tamanho da geometria

		TeTableRow row;


		for(int n = 0; n < ncolumns; ++n)
		{
			string value;

			try 
			{
				value = bnaFile.readStringCSVNoQuote(separator);
			}
			catch(...)
			{
				if(count > 0)
					return true;
				else
					return false;
			}

			row.push_back (value);
		}

		attTable.add(row);

		string bnaGeometryTypeLen = bnaFile.readStringCSVNoQuote(separator);

		int bnaGTypeLen = atoi(bnaGeometryTypeLen.c_str());

		string index = row[attLinkColumnIndex];

		// Go to the geometry line begin
		bnaFile.findNewLine();

		if(bnaGTypeLen > 2)
		{
			if(!TeBNARegionDecode(bnaFile, pSet, index, bnaGTypeLen))
				return false;
		}
		else if(bnaGTypeLen < -1)
		{
			if(!TeBNALineDecode(bnaFile, lnSet, index, -bnaGTypeLen))
				return false;
		}
		else if(bnaGTypeLen == 1)
		{
			if(!TeBNAPointDecode(bnaFile, ptSet, index))
				return false;
		}
		else
			return false;

		bnaFile.findNewLine();
	}

	return true;
}

bool TeBNARegionDecode(TeAsciiFile& bnaFile, TePolygonSet& temp, string& index, const int& npoints)
{
	int i;
	unsigned int j;

	vector<TePolygon> pVec;
	TeLine2D l;

	TeCoord2D coordMainArea;
	TeCoord2D firstCoord;
	bool first = false;

	for(i = 0; i < npoints; ++i)
	{
		if(!bnaFile.isNotAtEOF())
			return false;

		double x = bnaFile.readFloat();
		bnaFile.readChar();
		double y = bnaFile.readFloat();

		//bnaFile.findNewLine();

		TeCoord2D c(x, y);

		if(i == 0)
		{
			coordMainArea.setXY(x, y);
			firstCoord = coordMainArea;
			first = false;
			l.add(c);
		}
		else
		{
			if(c == firstCoord)
			{
				l.add(c);
				l.objectId(index);
				TeLinearRing r(l);
				r.objectId(index);
				TePolygon p;
				p.add(r);
				p.objectId(index);

				if(pVec.size() == 0)
					pVec.push_back(p);
				else
				{
					for(j = 0; j < pVec.size(); ++j)
					{	
						TePolygon paux = 	pVec[j];				
						if(TeWithin(p, paux))
						{
							pVec[j].add(p[0]);
							break;
						}		
						
					}

					if(j == pVec.size())
						pVec.push_back(p);
				}

				// Make new line - Don't clear!
				l = TeLine2D();

				first = true;

			}
			else if(c == coordMainArea)
			{
				continue;
			}
			else
			{
				if(first)
				{
					firstCoord = c;
					first = false;
				}

				l.add(c);
			}
		}
	}

	for(unsigned int k = 0; k < pVec.size(); ++k)
		temp.add(pVec[k]);


	return true;
}

bool TeBNALineDecode(TeAsciiFile& bnaFile, TeLineSet& temp, string& index, const int& npoints)
{
	TeLine2D l;

	l.objectId(index);

	for(int i = 0; i < npoints; ++i)
	{
		if(!bnaFile.isNotAtEOF())
			return false;

		double x = bnaFile.readFloat();
		bnaFile.readChar();
		double y = bnaFile.readFloat();

		//bnaFile.findNewLine();

		TeCoord2D c(x, y);

		l.add(c);
	}

	temp.add(l);

	return true;
}

bool TeBNAPointDecode(TeAsciiFile& bnaFile, TePointSet& temp, string& index)
{
	if(!bnaFile.isNotAtEOF())
		return false;

	double x = bnaFile.readFloat();
	bnaFile.readChar();
	double y = bnaFile.readFloat();

	//bnaFile.findNewLine();

	TeCoord2D c(x, y);

	TePoint pt(c);

	pt.objectId(index);

	temp.add(pt);

	return true;
}
