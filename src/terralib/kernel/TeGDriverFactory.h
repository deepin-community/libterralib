
#ifndef _TEGDRIVERFACTORY_H
#define _TEGDRIVERFACTORY_H

#include <string>
using namespace std;
#include "TeAbstractFactory.h"
#include "TeGeoDataDriver.h"

struct TL_DLL TeGDriverParams 
{
    TeGDriverParams(const string & fileType, const string & fileName);

    //Used by the factory.
    string getProductId() const;

    string fileType_; //!< Defines the type of file to be used. It identifies the factory of TeGeodataDriver objects.    

    string fileName_; //!< Complete name of the file.    


  private:
    //No copy allowed.
    TeGDriverParams(const TeGDriverParams & source);

    //No copy allowed.
    TeGDriverParams & operator=(const TeGDriverParams & source);

};
class TL_DLL TeGDriverFactory : public TeAbstractFactory<TeGeoDataDriver, TeGDriverParams, string> 
{
  public:
    TeGDriverFactory(const string & facName);

    virtual ~TeGDriverFactory();

};
#endif
