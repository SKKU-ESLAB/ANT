#include "vs_basic.h"
#include "vs_utility.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

enum
{
	SC_ACQ_PULL = 1,
	SC_ACQ_PUSH = 2
};

// Callbacks for virtual sensor API
class AppCallbacks : public VS_Basic_Callbacks 
{
	public:
		AppCallbacks() : mPolicySettingCount(0) {
		}

		// onConnected() Callback
		void onConnected() {
			vs_log("[EVENT] onConnected");

			// Get device list
			vs_log("Request: GetDeviceList");
			VS_Basic::get()->GetDeviceList();
		}

		// onGetDeviceList() Callback
		void onGetDeviceList(IN std::shared_ptr<VS_DeviceList> deviceList, const int eCode) {
			vs_log("[EVENT] onGetDeviceList");

			if(deviceList == NULL || deviceList->getResourcesVector()->size() == 0) {
				vs_log("Empty device list! Retry to get the device list.");
				VS_Basic::get()->GetDeviceList();
				return;
			}

			// Show device list
			printf("Device List: %s\n", deviceList->getDeviceListXML().c_str());
			VS_Utility::get()->printDeviceList(deviceList);

			// XXX: (SKIP) Check if target sensors exist
#define _SENSOR_URI "/gear/motion"
#define _SDA_PUSH 1
#define _SDA_BATCHING 2
#define _SDA_MODELDRIVEN 4

			// XXX: First policy
			VS_Basic::get()->SetPolicy(_SENSOR_URI,
					_SDA_PUSH, 100, 
					_SDA_BATCHING, 250);
			mPolicySettingCount++;
		}

		void startSensorDataAcquisition() {
			// Start sensor data acquisition 
			vs_log("Start to observe sensor data:");
			vs_log("* (PUSH: StartObservingSensorData)");
			if(VSF_S_OK != VS_Basic::get()->StartObservingSensorData(_SENSOR_URI))
			{
				vs_log("No connection with GW");
			}
		}

		// onPutActuatorData() Callback: it includes sensor data acquisition policy setting.
		void onPutActuatorData(IN std::shared_ptr<VS_Resource> resource, const int& eCode) {
			vs_log("[EVENT] onPutActuatorData");
			vs_log("ACK message:");
			VS_Utility::get()->printResource(resource);

			// If all the sensors complete policy setting, start sensor data acquisition.
			if(this->mPolicySettingCount == 0) {
				startSensorDataAcquisition();
			} else if(this->mPolicySettingCount == 1) {
				// XXX: Second policy
				usleep(10000);
				VS_Basic::get()->SetPolicy(_SENSOR_URI,
						_SDA_PUSH, 100, 
						_SDA_MODELDRIVEN, 200);
			} else if(this->mPolicySettingCount == 2) {
				// XXX: Third policy
				usleep(10000);
				VS_Basic::get()->SetPolicy(_SENSOR_URI,
						_SDA_PUSH, 50, 
						_SDA_MODELDRIVEN, 150);
			} else if(this->mPolicySettingCount == 3) {
				// XXX: Fourth policy
				usleep(10000);
				VS_Basic::get()->SetPolicy(_SENSOR_URI,
						_SDA_PUSH, 10, 
						_SDA_MODELDRIVEN, 50);
			}
			mPolicySettingCount++;
		}

		// onObservingSensorData() Callback
		void onObservingSensorData(IN std::shared_ptr<VS_Resource> resource, const int& eCode, const int& sequenceNumber) {
			vs_log("[EVENT] onObserveSensorData");
			vs_log("Sensor data:");
			VS_Utility::get()->printResource(resource);
		}

		// onGetSensorInfo() Callback
		void onGetSensorInfo(IN std::shared_ptr<VS_Resource> resource, const int eCode) {
			// Not implemented yet
		}

		// onGetSensorData() Callback
		void onGetSensorData(IN std::shared_ptr<VS_Resource> resource, const int eCode) {            
			// Not implemented yet
		}

	private:
//		std::shared_ptr<Configuration> mConfig;
		int mPolicySettingCount;
};


// Main function
int main(int argc, char* argv[])
{
	// Initialize callbacks
	std::shared_ptr<VS_Basic_Callbacks> callbacks(new AppCallbacks());
	VS_Basic::get()->setCallbacks(callbacks);

	// Connect to the gateway
	VS_Basic::get()->ConnectGW();
}
