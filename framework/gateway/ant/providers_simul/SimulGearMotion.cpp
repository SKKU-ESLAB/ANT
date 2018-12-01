//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

///
/// This sample provides steps to define an interface for a resource
/// (properties and methods) and host this resource on the server.
///

//Oduino serial reader
#include <stdio.h>    // Standard input/output definitions 
#include <unistd.h>   // UNIX standard function definitions 
#include <fcntl.h>    // File control definitions 
#include <errno.h>    // Error number definitions 
#include <termios.h>  // POSIX terminal control definitions 
#include <string.h>   // String function definitions 
#include <sys/ioctl.h>
///////////////////////
#include <functional>
#include <sstream>

#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <math.h>//for model driven calculation

#include "OCPlatform.h"
#include "OCApi.h"

#include "phy_interface.h"

#define ODU_DEV_PATH1 "/dev/ttyUSB0"
#define ODU_DEV_PATH2 "/dev/ttyUSB1"
#define ODU_DEV_PATH3 "/dev/ttyACM2"

#define ODU_BD_RT B115200
#define ODU_BUF_SIZ 256
//#define SERIALPORTDEBUG

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

int gObservation = 0;
void * ChangeOduinoRepresentation (void *param);
void * ReadOduinoData (void *param);
void * handleSlowResponse (void *param, std::shared_ptr<OCResourceRequest> pRequest);

// Specifies where to notify all observers or list of observers
// false: notifies all observers
// true: notifies list of observers
bool isListOfObservers = false;

// Specifies secure or non-secure
// false: non-secure resource
// true: secure resource
bool isSecure = false;

/// Specifies whether Entity handler is going to do slow response or not
bool isSlowResponse = false;

// Forward declaring the entityHandler

/// This class represents a single resource named 'lightResource'. This resource has
/// two simple properties named 'state' and 'power'

class SensorReader
{
	private:
		int fd,c,res;
		int power;
		char buf[ODU_BUF_SIZ];
	public:
		SensorReader(){
			struct termios toptions;

			fd = open("sensor/gear/motion", O_RDWR | O_NONBLOCK );

			if (fd == -1)  {
				printf("Unable to open file\n ");
			}
		};
		~SensorReader(){
			close(fd);
		};
		int isCreated(){	//1:Success, 0:Failed
			if(fd<=0)
				return 0;
			else
				return 1;
		}

		int listen(){
			if(isCreated() == 0){
				cout << "SensorReader has not been created" <<endl;
				return 0;
			}

			return parse()==1;
		};
		int parse(){
			char b[1];  // read expects an array, so we give it a 1-byte array
			int i=0;
			int timeout = 2;
			do { 
				int n = read(fd, b, 1);  // read a char at a time
				if( n==-1) return -1;    // couldn't read
				if( n==0 ) {
					usleep( 1 * 1000 );  // wait 1 msec try again
					timeout--;
					continue;
				}
				buf[i] = b[0]; 
				i++;
			} while( b[0] != '\n' && i < ODU_BUF_SIZ && timeout>0 );

			buf[i] = 0;  // null terminate the string

			char *cur, *next;
			int len, index;
			char value_string[ODU_BUF_SIZ];
			float value[3];

			cur = buf;
			for(index = 0;;index++){
				memset(value_string, 0, sizeof(char) * 10);
				next = strstr(cur, "/");
				if (next == NULL)
					break;
				len = next - cur;
				next++;
				strncpy(value_string, cur, len);
				cur = next;
				value[index] = atof(value_string);

				//printf("%.2f\n", value[index]);
			}
			power = (int)value[0];
			//printf("%d\n", power);

			lseek(fd, 0, SEEK_SET);

			return 1;
		};
		int get_Power(){
			return power;		
		}
};



class OduinoResource
{

public:
    /// Access this property from a TB client
    std::string m_name;
    int m_power;
    int m_availablePolicy;
    std::string m_oduinoUri;
    static OduinoResource *instance;
    SensorReader *sr;
    /// Constructor
    OduinoResource()
    {
        // Initialize representation

        m_name = "ESLab Gear Motion Sensor";
        m_power = 0;
        m_oduinoUri = "/gear/motion";
  //      PHY_Interface::SetNetPolicy(DT_ACQ_POLLING | DT_ACQ_BATCHING | DT_ACQ_MODEL, 0, m_availablePolicy);
//        PHY_Interface::SetRawPolicy(DT_ACQ_POLLING, 0, m_availablePolicy);
	//PHY_Interface::SetPolicy(DT_ACQ_POLLING, 0, DT_ACQ_POLLING | DT_ACQ_MODEL, 0, m_availablePolicy);
	 PHY_Interface::SetPolicy(DT_ACQ_POLLING, 0, DT_ACQ_POLLING | DT_ACQ_BATCHING | DT_ACQ_MODEL, 0, m_availablePolicy);
        sr = new SensorReader();
    }

public:
    static OduinoResource *getInstance();

    std::string getUri()
    {
        return m_oduinoUri;
    }
    int getPower()
    {
        return m_power;
    }

    void put(OCRepresentation& rep)
    {
        try {
	    std::string tempString;
	    if (rep.getValue("motion", tempString)){
		m_power = atoi(tempString.c_str());
                cout << "\t\t\t\t" << "motion:  " << m_power << endl;
            }
            else
            {
                cout << "\t\t\t\t" << "motion not found in the representation" << endl;
            }
        }
        catch (exception& e)
        {
            cout << e.what() << endl;
        }

    }
    void get(OCRepresentation &rep)
    {
       if(sr->listen()){
    		m_power = sr->get_Power();
        }
        std::stringstream ss;
        std::stringstream ss1;
        ss << m_power;
        rep.setValue("motion", ss.str());
    }
    int getAvailablePolicy()
    {
        return m_availablePolicy;
    }

};

OduinoResource *OduinoResource::instance = NULL;
OduinoResource *OduinoResource::getInstance()
{
    if(instance == NULL)
        instance = new OduinoResource();

    return instance;
}
// ChangeLightRepresentaion is an observation function,
// which notifies any changes to the resource to stack
// via notifyObservers

class MY_Callback : public PHY_Basic_Callback
{
    public:
	int num_notify;
	int net_level, raw_level;
        
	MY_Callback()
	{
	    net_level = 0;
	    raw_level = 0;
	    num_notify = 0;
	}

        void get(OUT OCRepresentation &rep)
        {
            OduinoResource::getInstance()->get(rep);
        }
        void put(IN OCRepresentation &rep)
        {
            OduinoResource::getInstance()->put(rep);
        }
        bool defaultAction(int polType)
        {
	    //return pollingAction(polType, 10);
	    //return modelAction(polType, 10);
/*	
	    if(polType == RAW_POL_TYPE){
		return modelAction(polType, 2);
	    }
	    else if(polType == NET_POL_TYPE){
		return modelAction(polType, 20);
	    }
*/

	    if(polType == RAW_POL_TYPE){
		    usleep(100*1000);
		    num_notify++;
            return true;
	    }
	    else if(polType == NET_POL_TYPE){
	        num_notify = 0;
            return modelAction(polType, 255);
	    }
        return false;
        }

        bool pollingAction(int polType, int level)
        {
	    static int raw_sleep;
	    static int net_sleep;

            if(polType == RAW_POL_TYPE){
		raw_level = level;
		if(raw_sleep >= level){
		    raw_sleep = 0;
		    num_notify++;
		    printf("polling raw num_notify = %d\n", num_notify);
                    return true;		 
		}
		else{
		    int sleep_time;
		    int raw_expect = raw_level - raw_sleep;
		    int net_expect = net_level - net_sleep;
		    if(net_expect>0){
			sleep_time = raw_expect < net_expect ? raw_expect:net_expect;
		    }
		    else{
			sleep_time = raw_expect;
		    }
		    if(sleep_time > 0){
			usleep(sleep_time*100000);
		        raw_sleep+=sleep_time;
		        net_sleep+=sleep_time;
		    }
		    return false;
		}
	    }
	    else if(polType == NET_POL_TYPE){
		net_level = level;
		if(net_sleep >= level){
		    net_sleep = 0;
		    return true;
		}
		else{
		    int sleep_time;
		    int raw_expect = raw_level - raw_sleep;
		    int net_expect = net_level - net_sleep;
		    if(raw_expect>0){
			sleep_time = raw_expect < net_expect ? raw_expect:net_expect;
		    }
		    else{
			 sleep_time = net_expect;
		    }
		    if(sleep_time > 0){
			usleep(sleep_time*100000);
			raw_sleep+=sleep_time;
			net_sleep+=sleep_time;
		    }
		    return false;
		}
	    }
	    return true;
	}

        bool modelAction(int polType, int level)
        {  
	    static int prevPower;
            if(polType == RAW_POL_TYPE){
		raw_level = level;
                return pollingAction(polType, level);
	    }
	    else if(polType == NET_POL_TYPE){
		int curPower;
		net_level = level;
		curPower = OduinoResource::getInstance()->getPower();
		if(curPower != prevPower){
		    prevPower = curPower;
		    return true;
		}
		else{
		    return false;
		}
	    }
	    return false;
        }

        bool batchingAction(int polType, int level)
        {
	    static int net_flag;
	    if(polType == RAW_POL_TYPE){
		raw_level = level;
		return pollingAction(polType, level);
	    }
	    else if(polType == NET_POL_TYPE){
		net_level = level;
		int ret = false;
		
		if(net_flag != true){
		    ret = pollingAction(polType, level);
		}
		if(ret == true){
		    net_flag = true;
		}

		if(net_flag == true && num_notify > 0){
		    num_notify--;
		    printf("num_notify = %d\n", num_notify);
		    return true;
		}
		else if(net_flag == true && num_notify == 0){
		    net_flag = false;
		    return false;
		}
		return false;
	    }
	    return false;
        }
};

int main(int argc, char* argv[])
{
    try
    {
        PHY_Interface::getInstance()->createResource( OduinoResource::getInstance()->getUri(), "core.provider", DEFAULT_INTERFACE, 
            OduinoResource::getInstance()->getAvailablePolicy(), false, std::make_shared<MY_Callback>());

        // A condition variable will free the mutex it is given, then do a non-
        // intensive block until 'notify' is called on it.  In this case, since we
        // don't ever call cv.notify, this should be a non-processor intensive version
        // of while(true);
        std::mutex blocker;
        std::condition_variable cv;
        std::unique_lock<std::mutex> lock(blocker);
        cv.wait(lock);
    }
    catch(OCException e)
    {
        //log(e.what());
    }

    // No explicit call to stop the platform.
    // When OCPlatform::destructor is invoked, internally we do platform cleanup

    return 0;
}

