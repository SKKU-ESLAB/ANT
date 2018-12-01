#ifndef __VS_UTILITY_H__
#define __VS_UTILITY_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "vs_basic.h"

inline void printfWithTabs(int tabs, const char* format, ...) {
	for(int i=0; i<tabs; i++) printf("  ");
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

class VS_Utility {
	public:
		static VS_Utility* get();
		void printDeviceList(std::shared_ptr<VS_DeviceList> deviceList);
		void printCollection(std::shared_ptr<VS_Collection> collection);
		void printCollection(std::shared_ptr<VS_Collection> collection, int tabs);
		void printResource(std::shared_ptr<VS_Resource> resource);
		void printResource(std::shared_ptr<VS_Resource> resource, int tabs);
//		std::shared_ptr<VS_Resource> getNthResource(IN std::shared_ptr<VS_Collection> collection, int resourceIdx);
		std::string getNthResourceUri(IN std::shared_ptr<VS_DeviceList> deviceList, unsigned int n);
//		std::shared_ptr<VS_Resource> getResourceByUri(IN std::shared_ptr<VS_DeviceList> deviceList, std::string uri);

	private:
		VS_Utility();
		static VS_Utility* singleton;
		int mNumResources;
};

#endif
