#include "gw_spm.h"
#include "gw_sdm.h"
#include <iostream>

int main()
{
	GW_SensorDeviceManager sdm;
	GW_Resource *resource;
	std::string xmlResult;

	sdm.AddResource("root/wow", &resource);
	sdm.AddResource("root/wor/owfj", &resource);
	sdm.AddResource("root/odroidXU3/thermometer/humidity", &resource);
	sdm.AddResource("root/odroidXU3/thermometer/temp", &resource);

	if(resource == NULL){
		std::cout << "Failed Resource" << std::endl;
		return 0;
	}

	sdm.GetResource("root/wow", &resource);

	if(resource ==NULL){
		std::cout << "Failed Resource" << std::endl;
		return 0;
	}

	sdm.GetResource("root/odroidXU3/thermometer/humidity", &resource);
	resource->setAttribute("temperature", "25");

	sdm.GetDeviceListXML(xmlResult);

	std::cout << xmlResult << std::endl;

	if(resource == NULL){
		std::cout << "Get Failed" << std::endl;
	}
	
	resource->addPolicy(2, 255, 1, 230);
	int a,b,c,d;
	GW_PolicyManager::GetNetPolicy(0x21ffe6, a, b);
	GW_PolicyManager::GetRawPolicy(0x21ffe6, c,d);
	std::cout << a << " "  << b << " " << c << " " << d <<std::endl;
	xmlResult = std::string();
	resource->makeXML(xmlResult);
	std::cout << xmlResult << std::endl;

	return 0;
}
