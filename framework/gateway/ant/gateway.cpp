#include "gw_interface.h"

void PrintUsage()
{
   gw_log("Start Gateway");
}

int main(int argc, char* argv[])
{
    PrintUsage();

    GW_Interface *gwInterface = GW_Interface::getInstance();

    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Both,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0,         // Uses randomly available port
        OC::QualityOfService::LowQos
    };

    OCPlatform::Configure(cfg);
    try
    {
        // Create the instance of the resource class
        // (in this case instance of class 'LightResource').
        
    

        // Invoke createResource function of class light.
        
        gwInterface->createResource();
        gwInterface->addType(std::string("core.specialgateway"));
        gwInterface->addInterface(std::string("oc.mi.gw"));

        while(1)
        {
            OCPlatform::findResource("", "coap://224.0.1.187/oc/core?rt=core.provider", onGWFoundResource);
            sleep(10);
       }

        
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

