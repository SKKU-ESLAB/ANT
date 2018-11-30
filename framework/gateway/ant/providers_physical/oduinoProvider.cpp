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

#include "OCPlatform.h"
#include "OCApi.h"

#include "phy_interface.h"

#define ODU_DEV_PATH1 "/dev/ttyACM0"
#define ODU_DEV_PATH2 "/dev/ttyACM1"
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

class SerialReader
{
	private:
		int fd,c,res;
		int temp, humi;
		char buf[ODU_BUF_SIZ];
	public:
		SerialReader(){
			struct termios toptions;

			//fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
			fd = open(ODU_DEV_PATH1, O_RDWR | O_NONBLOCK );

			if (fd == -1)  {
				printf("serialport_init: Unable to open por\n ");
			}

			//int iflags = TIOCM_DTR;
			//ioctl(fd, TIOCMBIS, &iflags);     // turn on DTR
			//ioctl(fd, TIOCMBIC, &iflags);    // turn off DTR

			if (tcgetattr(fd, &toptions) < 0) {
				perror("serialport_init: Couldn't get term attributes");
			}
			int baud = 115200;
			speed_t brate = baud; // let you override switch below if needed
			switch(baud) {
				case 4800:   brate=B4800;   break;
				case 9600:   brate=B9600;   break;
#ifdef B14400
				case 14400:  brate=B14400;  break;
#endif
				case 19200:  brate=B19200;  break;
#ifdef B28800
				case 28800:  brate=B28800;  break;
#endif
				case 38400:  brate=B38400;  break;
				case 57600:  brate=B57600;  break;
				case 115200: brate=B115200; break;
			}
			cfsetispeed(&toptions, brate);
			cfsetospeed(&toptions, brate);

			// 8N1
			toptions.c_cflag &= ~PARENB;
			toptions.c_cflag &= ~CSTOPB;
			toptions.c_cflag &= ~CSIZE;
			toptions.c_cflag |= CS8;
			// no flow control
			toptions.c_cflag &= ~CRTSCTS;

			//toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset

			toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
			toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

			toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
			toptions.c_oflag &= ~OPOST; // make raw

			// see: http://unixwiz.net/techtips/termios-vmin-vtime.html
			toptions.c_cc[VMIN]  = 0;
			toptions.c_cc[VTIME] = 0;
			//toptions.c_cc[VTIME] = 20;

			tcsetattr(fd, TCSANOW, &toptions);
			if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
				perror("init_serialport: Couldn't set term attributes");
			}
		};
		~SerialReader(){
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
				cout << "SerialReader has not been created" <<endl;
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
#ifdef SERIALPORTDEBUG  
				printf("serialport_read_until: i=%d, n=%d b='%c'\n",i,n,b[0]); // debug
#endif
				buf[i] = b[0]; 
				i++;
			} while( b[0] != '\n' && i < ODU_BUF_SIZ && timeout>0 );

			buf[i] = 0;  // null terminate the string

#ifdef SERIALPORTDEBUG
			cout<<i<<endl;
#endif

			if(i == 17){
			char tmp_str[ODU_BUF_SIZ];
			char *tmp_position = strstr(buf, "TEM:");

			if(tmp_position == 0)
				return 0;

			// TEM:
			tmp_position += 4;
			strcpy(tmp_str, tmp_position);
			tmp_position = strstr(tmp_str, "C");
			*tmp_position = 0;
			temp = atoi(tmp_str);

			tmp_position = strstr(buf, "HUMI:");
			if(tmp_position == 0)
				return 0;

			// HUMI:
			tmp_position += 5;
			strcpy(tmp_str, tmp_position);
			tmp_position = strstr(tmp_str, "%");
			*tmp_position = 0;
			humi = atoi(tmp_str);

#ifdef SERIALPORTDEBUG
			cout <<temp<<' '<<humi<<endl;
#endif
			sleep(2);
			tcflush(fd, TCIFLUSH);

			return 1;
			}

			return 0;
		};
		int get_Temp(){
			return temp;
		}
		int get_Humi(){
			return humi;
		}

};



class OduinoResource
{

public:
    /// Access this property from a TB client
    std::string m_name;
    int m_temp;
    int m_humi;
    int m_availablePolicy;
    std::string m_oduinoUri;
    static OduinoResource *instance;
    SerialReader *sr;
    /// Constructor
    OduinoResource()
    {
        // Initialize representation

        m_name = "ESLab Oduino";
        m_temp = 0;
        m_humi = 0;
        m_oduinoUri = "/a/oduino";
        PHY_Interface::SetPolicy(DT_ACQ_POLLING, 0, DT_ACQ_MODEL|DT_ACQ_POLLING, 0, m_availablePolicy);
        sr = new SerialReader();
    }

public:
    static OduinoResource *getInstance();

    std::string getUri()
    {
        return m_oduinoUri;
    }
    int getTemp()
    {
        return m_temp;
    }
    int getHumi()
    {
        return m_humi;
    } 

    void put(OCRepresentation& rep)
    {
        std::string tempString;
        try {
    	    if (rep.getValue("temp", tempString))
            {
                m_temp = atoi(tempString.c_str());
                cout << "\t\t\t\t" << "temperature: " << m_temp << endl;
            }
            else
            {
                cout << "\t\t\t\t" << "temp not found in the representation" << endl;
            }

            if (rep.getValue("humi", tempString))
            {
                m_humi = atoi(tempString.c_str());
                cout << "\t\t\t\t" << "humidity: " << m_humi << endl;
            }
            else
            {
                cout << "\t\t\t\t" << "humi not found in the representation" << endl;
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
    		m_temp = sr->get_Temp();
    		m_humi = sr->get_Humi();
        }
        
#ifdef SERIALPORTDEBUG
		cout << "TEMP:" <<oduinoPtr->m_temp<<"HUMI:"<<oduinoPtr->m_humi<<endl;
#endif
        std::stringstream ss;
        std::stringstream ss1;
        ss << m_temp;
        rep.setValue("temp", ss.str());
        ss1 << m_humi;
        rep.setValue("humi", ss1.str());
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
            static int prevTemp;
            static int prevHumi;
            if(polType == RAW_POL_TYPE)
            {
                sleep(1);
                return true;
            }
            else if(polType == NET_POL_TYPE)
            {
                int curTemp, curHumi;
                curTemp = OduinoResource::getInstance()->getTemp();
                curHumi = OduinoResource::getInstance()->getHumi();
                if(curTemp != prevTemp || curHumi != prevHumi)
                {
                    prevTemp = curTemp;
                    prevHumi = curHumi;
                    return true;
                }
                else
                    return false;
            }

            return false;
        }
        bool pollingAction(int polType, int level)
        {
            if(polType == NET_POL_TYPE)
                return false;
            return defaultAction(polType);
        }
        bool modelAction(int polType, int level)
        {
            if(polType == RAW_POL_TYPE)
                return false;
            return defaultAction(polType);
        }
        bool batchingAction(int polTyhpe, int level)
        {
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

