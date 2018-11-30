#include "vs_utility.h"
#include "vs_basic.h"

VS_Utility *VS_Utility::singleton = NULL;

VS_Utility::VS_Utility() : mNumResources(0) {
}

VS_Utility* VS_Utility::get() {
	if(singleton == NULL)
		singleton = new VS_Utility();
	return singleton;
}

void VS_Utility::printDeviceList(std::shared_ptr<VS_DeviceList> deviceList) {
	if(deviceList == NULL) return;
//#ifdef BIG_NETWORK_SIZE_SUPPORTED
//	printCollection(deviceList->getRoot());
//#else
	std::shared_ptr<std::vector<std::string>> resourcesVector = deviceList->getResourcesVector();
	for(std::vector<std::string>::iterator it = resourcesVector->begin(); it != resourcesVector->end(); it++)
	{
		printfWithTabs(0, "[Resource uri = \"%s\"]\n", it->c_str());
	}
//#endif
}

void VS_Utility::printCollection(std::shared_ptr<VS_Collection> collection) {
	printCollection(collection, 0);
}

void VS_Utility::printCollection(std::shared_ptr<VS_Collection> collection, int tabs) {
	printfWithTabs(tabs, "[Collection name=\"%s\"]\n", collection->getName().c_str());
	if(collection->isResource()) {
		std::vector<std::shared_ptr<VS_Resource>> resources;
		resources = collection->getResources();
		for(std::shared_ptr<VS_Resource> resource: resources) {
			printResource(resource, tabs + 1);
		}
	}
	std::vector<std::shared_ptr<VS_Collection>> children = collection->getChildren();
	for(std::shared_ptr<VS_Collection> child : children) {
		printCollection(child, tabs + 1);
	}
}

void VS_Utility::printResource(std::shared_ptr<VS_Resource> resource) {
	if(resource == NULL) return;
	printResource(resource, 0);
}

void VS_Utility::printResource(std::shared_ptr<VS_Resource> resource, int tabs) {
	printfWithTabs(tabs, "[Resource uri=\"%s\", name: \"%s\"]\n", resource->getUri().c_str(), resource->getName().c_str());
	printfWithTabs(tabs + 1, "AvailablePolicy: %08x\n",
			resource->getAvailablePolicy());
	std::vector<int> policies = resource->getPolicies();
	printfWithTabs(tabs + 1, "Policy: ");
	if(policies.size() == 0)
		printf("No policy");
	else {
		for(auto policy: policies) {
			printf("%08x ", policy);
		}
	}
	printf("\n");
	printfWithTabs(tabs + 1, "Attributes: ");
	std::map<std::string, std::string> attributes = resource->getAttributes();
	if(attributes.size() == 0) 
		printf("No attributes\n");
	else {
		for(auto attribute: attributes) {
			printf("%s=\"%s\" ", attribute.first.c_str(), attribute.second.c_str());
		}
	}
	printf("\n");
	return;
}

//// Get N-th resource within collection
//std::shared_ptr<VS_Resource> VS_Utility::getNthResource(IN std::shared_ptr<VS_Collection> collection, int resourceIdx) {
//	if(collection->isResource()) {
//		// Search child resources
//		std::vector<std::shared_ptr<VS_Resource>> resources = collection->getResources();
//		for(std::shared_ptr<VS_Resource> targetResource : resources) {
//			this->mNumResources++;
//			if(this->mNumResources == resourceIdx) return targetResource;
//		}
//		return NULL;
//	} else {
//		// Search child collections
//		std::vector<std::shared_ptr<VS_Collection>> childCollections = collection->getChildren();
//		for(std::shared_ptr<VS_Collection> childCollection: childCollections) {
//			std::shared_ptr<VS_Resource> targetResource = getNthResource(childCollection, resourceIdx);
//			if(targetResource != NULL) {
//				return targetResource;
//			}
//		}
//		return NULL;
//	}
//}

// Get the URI of N-th resource in the device list
std::string VS_Utility::getNthResourceUri(IN std::shared_ptr<VS_DeviceList> deviceList, unsigned int nthResource)
{
//#ifndef BIG_NETWORK_SIZE_SUPPORTED
//	std::shared_ptr<VS_Resource> vs_res = getNthResource(deviceList->getRoot(), nthResource);
//	if(NULL == vs_res)
//		return std::string();
//	else
//		return vs_res->getUri();
//#else
	std::shared_ptr<std::vector<std::string>> resourcesVector = deviceList->getResourcesVector();

	if(nthResource < 1 || nthResource > resourcesVector->size())
		return std::string();

	return resourcesVector->at(nthResource - 1);
//#endif
}

//// Get the resource of given URI in the device list
//std::shared_ptr<VS_Resource> VS_Utility::getResourceByUri(IN std::shared_ptr<VS_DeviceList> deviceList, std::string uri)
//{
////#ifndef BIG_NETWORK_SIZE_SUPPORTED
////	std::vector<std::shared_ptr<VS_Resource>>& resources = deviceList->getRoot()->getResources();
////	for(std::vector<std::shared_ptr<VS_Resource>>::iterator it = resources.begin(); it != resources.end(); it++)
////	{
////		if(uri.compare((*it)->getUri()) != 0)
////				return *it;
////	}
////	return NULL;
////#else
//	std::shared_ptr<std::vector<std::string>> resourcesVector = deviceList->getResourcesVector();
//
//	for(std::vector<std::string>::iterator it = resourcesVector->begin(); it != resourcesVector->end(); it++) {
//		if(uri.compare((*it).getUri()) != 0)
//				return *it;
//	}
//	return NULL;
////#endif
//}
