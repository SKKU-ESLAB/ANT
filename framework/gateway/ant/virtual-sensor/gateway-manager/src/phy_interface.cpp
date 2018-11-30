//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/// @file phy_interface.cpp

/// @brief  This file contains the declaration of classes and its members
///         related to physical sensor interface


#include "OCPlatform.h"
#include "OCApi.h"

#include "phy_interface.h"


PHY_Interface *PHY_Interface::instance = NULL;

PHY_Interface::PHY_Interface(void)
{
	prevPolicy = 0;
	policy = 0;
	availablePolicy = 0;
	myCallback = NULL;
	gObservation = 0;
}
void *PHY_Interface::notificationFuncHelper(void *phyInt)
{
	return ((PHY_Interface *)phyInt)->notificationFunc(NULL);
}

int PHY_Interface::GetNetPolicy(IN int policy, OUT int &dtAcqType, OUT int &level)
{
	dtAcqType = get_dt_acq_net((policy >> DT_ACQ_TYPE_SHIFT));
	level = get_dt_acq_net_lev((policy >> DT_ACQ_LEVEL_SHIFT));

	return VSF_S_OK;
}
int PHY_Interface::GetRawPolicy(IN int policy, OUT int &dtAcqType, OUT int &level)
{
	dtAcqType = get_dt_acq_raw((policy >> DT_ACQ_TYPE_SHIFT));
	level = get_dt_acq_raw_lev((policy >> DT_ACQ_LEVEL_SHIFT));

	return VSF_S_OK;
}
int PHY_Interface::GetNetPolicy(OUT int &dtAcqType, OUT int &level)
{
	dtAcqType = get_dt_acq_net((policy >> DT_ACQ_TYPE_SHIFT));
	level = get_dt_acq_net_lev((policy >> DT_ACQ_LEVEL_SHIFT));

	return VSF_S_OK;
}
int PHY_Interface::GetRawPolicy(OUT int &dtAcqType, OUT int &level)
{
	dtAcqType = get_dt_acq_raw((policy >> DT_ACQ_TYPE_SHIFT));
	level = get_dt_acq_raw_lev((policy >> DT_ACQ_LEVEL_SHIFT));

	return VSF_S_OK;
}

int PHY_Interface::SetNetPolicy(IN int dtAcqType, IN int level)
{
	int result = dtAcqType << DT_ACQ_NET_TYPE_SHIFT;
	result = result << DT_ACQ_TYPE_SHIFT;
	result = result | ((level<<DT_ACQ_NET_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
	policy = result;
	phy_log("policyValue : %x %x %08x", dtAcqType, level, policy);

	return VSF_S_OK;
}
int PHY_Interface::SetRawPolicy(IN int dtAcqType, IN int level)
{
	int result = dtAcqType << DT_ACQ_RAW_TYPE_SHIFT;
	result = result << DT_ACQ_TYPE_SHIFT;
	result = result | ((level<<DT_ACQ_RAW_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
	policy = result;
	
	phy_log("policyValue : %x %x %08x", dtAcqType, level, policy);

	return VSF_S_OK;
}
int PHY_Interface::getNetPolType()
{
	return get_dt_acq_net((policy >> DT_ACQ_TYPE_SHIFT));
}
int PHY_Interface::getNetPolLevel()
{
	return get_dt_acq_net_lev((policy >> DT_ACQ_LEVEL_SHIFT));
}
int PHY_Interface::getRawPolType()
{
	return get_dt_acq_raw((policy >> DT_ACQ_TYPE_SHIFT));
}
int PHY_Interface::getRawPolLevel()
{
	return get_dt_acq_raw_lev((policy >> DT_ACQ_LEVEL_SHIFT));
}
bool PHY_Interface::verifyPolicy(IN int available, IN int policyValue)
{
	int dtAcqType, level, retValue, ret;
	int netAvailable, rawAvailable;
	bool res;

	netAvailable = ((available >> DT_ACQ_NET_TYPE_SHIFT)>>DT_ACQ_TYPE_SHIFT) & DT_ACQ_MASK;
	rawAvailable = ((available >> DT_ACQ_RAW_TYPE_SHIFT)>>DT_ACQ_TYPE_SHIFT) & DT_ACQ_MASK;
	
	ret = GetNetPolicy(policyValue, dtAcqType, level);

	retValue = dtAcqType & netAvailable;

	ret = GetRawPolicy(policyValue, dtAcqType, level);

	if(ret != VSF_S_OK)
		phy_log("NOT happened");

	retValue = retValue && (dtAcqType & rawAvailable);

	if(retValue)
		res = true;
	else
		res = false;

	return res;
}
OCEntityHandlerResult PHY_Interface::entityHandler(std::shared_ptr<OCResourceRequest> request)
{
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	if(request)
	{			
        // Get the request type and request flag
        std::string requestType = request->getRequestType();
        int requestFlag = request->getRequestHandlerFlag();

        if(requestFlag & RequestHandlerFlag::InitFlag)
        {
            cout << "\t\trequestFlag : Init\n";

            // entity handler to perform resource initialization operations
        }
        if(requestFlag & RequestHandlerFlag::RequestFlag)
        {
            cout << "\t\trequestFlag : Request\n";
            auto pResponse = std::make_shared<OC::OCResourceResponse>();
            pResponse->setRequestHandle(request->getRequestHandle());
            pResponse->setResourceHandle(request->getResourceHandle());
            // If the request type is GET
            if(requestType == "GET")
            {
                cout << "\t\t\trequestType : GET\n";
				QueryParamsMap queryParamsMap = request->getQueryParameters();
				OCRepresentation rep;
				rep.setUri(m_resourceUri);
				if(policy != 0)
					rep.setValue("POLICY", policy);
		 		if(queryParamsMap.find("AV_POL") != queryParamsMap.end())
				{
					rep.setValue("AV_POL", availablePolicy);
				}
				if(myCallback != NULL)
					myCallback->get(rep);
				
                pResponse->setErrorCode(200);
                pResponse->setResponseResult(OC_EH_OK);
                pResponse->setResourceRepresentation(rep);
                if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
                {
                    ehResult = OC_EH_OK;
                }		                
            }
            else if(requestType == "PUT")
            {
                cout << "\t\t\trequestType : PUT\n";
				std::map<std::string, std::string>::iterator it;
				int pol;
                OCRepresentation rep = request->getResourceRepresentation();
				OCRepresentation ret_rep;
				ret_rep.setUri(m_resourceUri);
				if(rep.getValue("POLICY", pol))
				{
					if(verifyPolicy(availablePolicy, pol))
					{
						policy = pol;
						ret_rep.setValue("POLICY", pol);
					}
					else
						phy_log("Not allowed policy");
				}
										
				if(myCallback != NULL)
				{
					myCallback->put(rep);
					myCallback->get(ret_rep);
				}
				
                pResponse->setErrorCode(200);
                pResponse->setResponseResult(OC_EH_OK);
                pResponse->setResourceRepresentation(ret_rep);
                if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "POST")
            {
                cout << "\t\t\trequestType : POST\n";

                OCRepresentation rep = request->getResourceRepresentation();

                // Do related operations related to POST request
                pResponse->setErrorCode(200);
               
                if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
                {
                    ehResult = OC_EH_OK;
                }
				else
					cout<<"Cannot send response"<<endl;
            }
            else if(requestType == "DELETE")
            {
                // DELETE request operations
            }
        }

        if(requestFlag & RequestHandlerFlag::ObserverFlag)
        {
            ObservationInfo observationInfo = request->getObservationInfo();
            if(ObserveAction::ObserveRegister == observationInfo.action)
            {
                m_interestedObservers.push_back(observationInfo.obsId);
            }
            else if(ObserveAction::ObserveUnregister == observationInfo.action)
            {
                m_interestedObservers.erase(std::remove(m_interestedObservers.begin(),
                                                        m_interestedObservers.end(),
                                                        observationInfo.obsId),
                                            m_interestedObservers.end());
            }

            pthread_t threadId;

            cout << "\t\trequestFlag : Observer\n";
            gObservation = 1;
            static int startedThread = 0;

            // Observation happens on a different thread in ChangeLightRepresentation function.
            // If we have not created the thread already, we will create one here.
            if(!startedThread)
            {
                pthread_create (&threadId, NULL, &PHY_Interface::notificationFuncHelper, this);
                startedThread = 1;
            }
            ehResult = OC_EH_OK;
        }
    }
    else
    {
        std::cout << "Request invalid" << std::endl;
    }

    return ehResult;
			
}

OCStackResult PHY_Interface::createResource(std::string resourceUri, std::string resourceTypeName, std::string resourceInterface, 
	int availablePolicy, int isSecure, std::shared_ptr<PHY_Basic_Callback> callback)
{
	this->availablePolicy = availablePolicy;
	uint8_t resourceProperty;
	m_resourceUri = resourceUri;

	this->myCallback = callback;
	if(isSecure)
    {
        resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE;
    }
    else
    {
        resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
    }
	PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Server,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0,         // Uses randomly available port
        OC::QualityOfService::LowQos
    };

    OCPlatform::Configure(cfg);
    EntityHandler cb = std::bind(&PHY_Interface::entityHandler, this,PH::_1);

    // This will internally create and register the resource.
    OCStackResult result = OCPlatform::registerResource(
                                m_resourceHandle, resourceUri, resourceTypeName,
                                resourceInterface, cb, resourceProperty);

    if (OC_STACK_OK != result)
    {
        cout << "Resource creation was unsuccessful\n";
    }

	return result;
}		
void PHY_Interface::SetNetPolicy(IN int dtAcqType, IN int level, OUT int& policy)
{
	int result = dtAcqType << DT_ACQ_NET_TYPE_SHIFT;
	result = result << DT_ACQ_TYPE_SHIFT;
	result = result | ((level<<DT_ACQ_NET_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
	policy = result;
}
void PHY_Interface::SetRawPolicy(IN int dtAcqType, IN int level, OUT int& policy)
{
	int result = dtAcqType << DT_ACQ_RAW_TYPE_SHIFT;
	result = result << DT_ACQ_TYPE_SHIFT;
	result = result | ((level<<DT_ACQ_RAW_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
	policy = result;
}

void PHY_Interface::SetPolicy(IN int rawDtAcqType, IN int rawLevel, IN int netDtAcqType, IN int netLevel, OUT int &policy)
{
	int netResult = netDtAcqType << DT_ACQ_NET_TYPE_SHIFT;
	netResult = netResult << DT_ACQ_TYPE_SHIFT;
	netResult = netResult | ((netLevel<<DT_ACQ_NET_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);

	int rawResult = rawDtAcqType << DT_ACQ_RAW_TYPE_SHIFT;
	rawResult = rawResult << DT_ACQ_TYPE_SHIFT;
	rawResult = rawResult | ((rawLevel<<DT_ACQ_RAW_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
	policy = netResult | rawResult;
}
PHY_Interface *PHY_Interface::getInstance()
{
	if(instance == NULL)
	{
		instance = new PHY_Interface();
	}

	return instance;
}

void *PHY_Interface::notificationFunc(void *param)
{
	int result;
	std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
	OCRepresentation rep;
	rep.setUri(m_resourceUri);
	while(1)
	{

		if(0 == policy)
		{
			if(myCallback->defaultAction(RAW_POL_TYPE))
			{
				myCallback->get(rep);
			}
			if(myCallback->defaultAction(NET_POL_TYPE))
			{
				resourceResponse->setErrorCode(200);
				resourceResponse->setResourceRepresentation(rep);
				result = OCPlatform::notifyListOfObservers(m_resourceHandle, m_interestedObservers, resourceResponse);

				if(OC_STACK_NO_OBSERVERS == result)
				{
					phy_log("No more observers");
					gObservation = 0;
				}
			}
		}
		else
		{
			if(policy != 0)
				rep.setValue("POLICY", policy);
			switch(getRawPolType())
			{
				case DT_ACQ_POLLING:
					if(myCallback->pollingAction(RAW_POL_TYPE, getRawPolLevel()))
						myCallback->get(rep);
					break;
				case DT_ACQ_BATCHING:
					if(myCallback->batchingAction(RAW_POL_TYPE, getRawPolLevel()))
						myCallback->get(rep);
					break;
				case DT_ACQ_MODEL:
					if(myCallback->modelAction(RAW_POL_TYPE, getRawPolLevel()))
						myCallback->get(rep);
					break;
				default:
					if(myCallback->defaultAction(RAW_POL_TYPE))
						myCallback->get(rep);
					break;
			}

			switch(getNetPolType())
			{
				case DT_ACQ_POLLING:
					if(myCallback->pollingAction(NET_POL_TYPE, getNetPolLevel()))
					{
						resourceResponse->setErrorCode(200);
						resourceResponse->setResourceRepresentation(rep);
						result = OCPlatform::notifyListOfObservers(m_resourceHandle, m_interestedObservers, resourceResponse);

						if(OC_STACK_NO_OBSERVERS == result)
						{
							phy_log("No more observers");
							gObservation = 0;
						}
					}
					break;
				case DT_ACQ_BATCHING:
					if(myCallback->batchingAction(NET_POL_TYPE, getNetPolLevel()))
					{
						resourceResponse->setErrorCode(200);
						resourceResponse->setResourceRepresentation(rep);
						result = OCPlatform::notifyListOfObservers(m_resourceHandle, m_interestedObservers, resourceResponse);

						if(OC_STACK_NO_OBSERVERS == result)
						{
							phy_log("No more observers");
							gObservation = 0;
						}
					}
					break;
				case DT_ACQ_MODEL:
					if(myCallback->modelAction(NET_POL_TYPE, getNetPolLevel()))
					{
						resourceResponse->setErrorCode(200);
						resourceResponse->setResourceRepresentation(rep);
						result = OCPlatform::notifyListOfObservers(m_resourceHandle, m_interestedObservers, resourceResponse);

						if(OC_STACK_NO_OBSERVERS == result)
						{
							phy_log("No more observers");
							gObservation = 0;
						}
					}
					break;
				default:
					if(myCallback->defaultAction(NET_POL_TYPE))
					{
						resourceResponse->setErrorCode(200);
						resourceResponse->setResourceRepresentation(rep);
						result = OCPlatform::notifyListOfObservers(m_resourceHandle, m_interestedObservers, resourceResponse);

						if(OC_STACK_NO_OBSERVERS == result)
						{
							phy_log("No more observers");
							gObservation = 0;
						}
					}
					break;
			}
		}
	}

	return NULL;
}

