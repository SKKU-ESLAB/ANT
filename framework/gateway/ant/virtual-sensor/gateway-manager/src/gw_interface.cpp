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

/// @file gw_interface.cpp

/// @brief  This file contains the implemenetation of classes and its members
///         related to Gatewayinterface 

#include "gw_interface.h"

GW_Interface *GW_Interface::instance = NULL;

GW_Interface::GW_Interface()
{
	gw_name = "Gateway";
	gw_uri = "/gateway";
	gw_rep.setUri(gw_uri);
	sdm = GW_SensorDeviceManager::getInstance();
	snf = GW_SensorNotifier::getInstance();
	snf->setResourceHandle(&gw_resourceHandle);
}
GW_Interface::~GW_Interface()
{}

void GW_Interface::addType(const std::string& type) const
{
    OCStackResult result = OCPlatform::bindTypeToResource(gw_resourceHandle, type);
    if (OC_STACK_OK != result)
    {
//	            cout << "Binding TypeName to Resource was unsuccessful\n";
    }
}

void GW_Interface::addInterface(const std::string& interface) const
{
    OCStackResult result = OCPlatform::bindInterfaceToResource(gw_resourceHandle, interface);
    if (OC_STACK_OK != result)
    {
//	            cout << "Binding TypeName to Resource was unsuccessful\n";
    }
}
void GW_Interface::createResource()
{
	std::string resourceTypeName = "core.gateway";
	std::string resourceInterface = DEFAULT_INTERFACE;

	uint8_t resourceProperty;
	resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
	EntityHandler cb = std::bind(&GW_Interface::gw_entityHandler, this, std::placeholders::_1);

	OCStackResult result = OCPlatform::registerResource(gw_resourceHandle, gw_uri, resourceTypeName,
													resourceInterface, cb, resourceProperty);			
	gw_log(gw_uri.c_str());
	if(OC_STACK_OK != result)
	{
		gw_log("Resource createion was unsuccessful");
	}
}
OCEntityHandlerResult GW_Interface::gw_entityHandler(std::shared_ptr<OCResourceRequest> request)
{
	OCEntityHandlerResult ehResult = OC_EH_ERROR;
	static std::shared_ptr<OCResourceRequest> req = NULL;

	if(req)
	{
		//	gw_log("CUR Req: %x, Prev Req: %x", request.get(), req.get());

	}
	req = request;
	if(request)
	{
		// Get the request type and request flag
		std::string requestType = request->getRequestType();
		int requestFlag = request->getRequestHandlerFlag();

		if(requestFlag & RequestHandlerFlag::InitFlag)
		{
			// entity handler to perform resource initialization operations
		}
		if(requestFlag & RequestHandlerFlag::RequestFlag)
		{
			auto pResponse = std::make_shared<OC::OCResourceResponse>();
			pResponse->setRequestHandle(request->getRequestHandle());
			pResponse->setResourceHandle(request->getResourceHandle());
			// If the request type is GET
			if(requestType == "GET")
			{
				if(!(requestFlag & RequestHandlerFlag::ObserverFlag))
				{
					gw_log("Request Type: GET - Error");
					if(1) // Slow response case
					{
						ehResult = OC_EH_ERROR;
					}
					else // normal response case.
					{
						ehResult = OC_EH_ERROR;
					}
				}						
			}
			else if(requestType == "PUT")
			{
				OCRepresentation rep = request->getResourceRepresentation();
				OCRepresentation rep_result;
				rep_result.setUri(gw_uri);
				int gw_cmd;
				bool ok;

				ok = rep.getValue("GW_CMD", gw_cmd);

				if(ok == false)
					gw_log("GW_CMD acquisition failed");

				int ret = VSF_S_OK;
				switch(gw_cmd)
				{

					// IN "SURI":std::string, OUT "RESULT":std::string
					case GW_CMD_GET_SENSOR_INFO:
						{
							gw_log("Get Info");
							std::string sUri;
							bool ok;
							std::string resXML;
							GW_Resource *gw_res;

							ok  = rep.getValue("SURI", sUri);

							if(ok == false)
							{
								gw_log("No SURI value");
							}
							gw_log(sUri.c_str());
							if(sUri == "/ss/discomfort")
							{
								/* Virtual Sensor Test Code */
								//Discomfort_Update();
								ret = sdm->GetResourceXML(sUri, resXML);
								rep_result.setValue("RESULT", resXML);
								//									gw_log(rep_result.getJSONRepresentation().c_str());
								break;
							}


							sdm->GetResource(sUri, &gw_res);
							GW_SensorArbiter::getInstance()->get(*gw_res);
							ret = sdm->GetResourceXML(sUri, resXML);
							rep_result.setValue("RESULT", resXML);
							if(VSF_S_OK != ret)
								gw_log("Failed");
							break;
						}
						// IN "SURI":std::string, IN "EPOCH" :std::string, OUT "RESULT":std::string
					case GW_CMD_GET_SENSOR_DATA_EPOCH:
						{
							gw_log("Get Data");
							std::string epoch_str;
							char tmpstr[256];
							int epoch;
							std::string resXML;
							sensorDB db;
							std::map<std::string, std::map<int, std::string>> out;

							bool ok;

							ok = rep.getValue("SURI", db.suri);
							if(ok == false)
							{
								gw_log("No SURI found in rep");
								break;
							}
							ok = rep.getValue("EPOCH", epoch_str);
							if(ok == false)
							{
								gw_log("No EPOCH found in rep");
								break;
							}
							epoch = atoi(epoch_str.c_str());
							db.ttl = 100;

							DBManager::getInstance()->getSensorVector(&db, epoch, &out);
							if(out.size() <= 0)
							{
								int ret;
								GW_Resource *gw_res;
								ret = sdm->GetResource(db.suri, &gw_res);
								if(ret != VSF_S_OK || gw_res == NULL)
									gw_log("Get Resource failed");


								GW_SensorArbiter::getInstance()->get(*gw_res);
								gw_res->lockGetMutex();
								gw_res->unlockGetMutex(); //For synchronous operation
							}
							if(out.size()>0)
							{
								gw_log("%d size", out.size());
								for(std::map<std::string, std::map<int, std::string>>::iterator it = out.begin(); it != out.end(); it++)
								{
									std::map<int, std::string> temp = it->second;
									if(temp.size() > 0)
									{
										resXML+="<attribute name=\"";
										resXML+=it->first;
										resXML+="\">";
										for (std::map<int, std::string>::iterator it_i = temp.begin(); it_i != temp.end(); it_i++)
										{

											printf("%d, %s\n", it_i->first, it_i->second.c_str());
											itoa10(it_i->first, tmpstr);
											resXML+="<value time=\"";
											resXML+=tmpstr;
											resXML+="\">";
											resXML+=it_i->second;
											resXML+="</value>";

										}
										resXML+="</attribute>";
									}
								}
							}

							//TODO
							//Fill out the rep_result
						}
						// OUT "RESULT":std::string
					case GW_CMD_GET_DEVICE_LIST:
						{
							std::string resXML;

							ret = sdm->GetDeviceListXML(resXML);
							//								gw_log(resXML.c_str());
							if(VSF_S_OK != ret)
							{
								gw_log("Failed");
								break;
							}
							rep_result.setValue("RESULT", resXML);
							break;
						}

						// IN "SURI":std::string, IN "NET_TYPE":int, IN "NET_VALUE":int,
						//IN "RAW_TYPE":int, IN "RAW_VALUE":int  OUT "RESULT":bool
					case GW_CMD_VERIFY_POLICY:
						{
							std::string sUri ;
							bool ok;
							int net_type, net_value, raw_type, raw_value;
							GW_Resource *resource;
							bool result;

							ok = rep.getValue("SURI", sUri);
							if(ok == false)
							{
								gw_log("No SURI value");
							}

							ok = rep.getValue("NET_TYPE", net_type);
							if(ok == false)
							{
								gw_log("No NET_TYPE value");
							}
							ok = rep.getValue("NET_VALUE", net_value);
							if(ok == false)
							{
								gw_log("No NET_VALUE value");
							}
							ok = rep.getValue("RAW_TYPE", raw_type);
							if(ok == false)
							{
								gw_log("No RAW_TYPE value");
							}
							ok = rep.getValue("RAW_VALUE", raw_value);
							if(ok == false)
							{
								gw_log("No RAW_VALUE value");
							}

							ret = sdm->GetResource(sUri, &resource);

							if(VSF_S_OK == ret && resource != NULL)
							{
								resource->verifyPolicy(net_type, net_value, raw_type, raw_value, result);
								rep_result.setValue("ACK", result);
							}
							else
							{
								result = false;
								rep_result.setValue("RESULT", result);
							}
						}
						// IN "SURI":std::string, IN "NET_TYPE":int, IN "NET_VALUE":int,
						//IN "RAW_TYPE":int, IN "RAW_VALUE":int  OUT "ACK":bool
					case GW_CMD_ADD_POLICY:
						{
							std::string sUri ;
							bool ok;
							int net_type, net_value, raw_type, raw_value;
							GW_Resource *resource;

							ok = rep.getValue("SURI", sUri);
							if(ok == false)
							{
								gw_log("No SURI value");
							}

							ok = rep.getValue("NET_TYPE", net_type);
							if(ok == false)
							{
								gw_log("No NET_TYPE value");
							}

							ok = rep.getValue("NET_VALUE", net_value);
							if(ok == false)
							{
								gw_log("No NET_VALUE value");
							}

							ok = rep.getValue("RAW_TYPE", raw_type);
							if(ok == false)
							{
								gw_log("No RAW_TYPE value");
							}

							ok = rep.getValue("RAW_VALUE", raw_value);
							if(ok == false)
							{
								gw_log("No RAW_VALUE value");
							}

							ret = sdm->GetResource(sUri, &resource);

							if(VSF_S_OK == ret && resource != NULL)
							{
								ret = resource->addPolicy(net_type, net_value, raw_type, raw_value);
								if(ret == VSF_S_OK)
									rep_result.setValue("ACK", true);
								else
									rep_result.setValue("ACK", false);
							}
							else
							{
								rep_result.setValue("ACK", false);
							}
							break;
						}
					case GW_CMD_APPLY_POLICY:
						gw_log("SET POLICY-->");
					case GW_CMD_PUT_ACTUATOR_DATA:
						{ 
							std::string suri, resXML;
							int ret, policy;
							gw_log("GW_CMD_PUT_ACTUATOR_DATA");
							if(rep.getValue("SURI", suri))
							{
								GW_Resource *gw_res;
								sdm->GetResource(suri, &gw_res);
								if(gw_res == NULL)
									rep_result.setValue("ACK", false);

								if(rep.getValue("POLICY", policy))
								{
									gw_res->setCurPolicy(policy);
								}
								std::map<std::string, std::string> res_attr = gw_res->getAttribute();
								for(std::map<std::string, std::string>::iterator it = res_attr.begin(); it != res_attr.end(); it++)
								{
									std::string attrValue;
									if(rep.getValue(it->first, attrValue))
									{
										gw_res->setAttribute(it->first, attrValue);
									}
								}

								GW_SensorArbiter::getInstance()->put(*gw_res, rep);
								ret = sdm->GetResourceXML(suri, resXML);
								rep_result.setValue("RESULT", resXML);
								//gw_log("PUT_RES:%s", resXML.c_str());
								if(VSF_S_OK != ret)
									gw_log("Failed");
							}
							break;
						}
					case GW_CMD_START_OBSERVING:
						{
							QueryParamsMap queryParamsMap = request->getQueryParameters();
							std::string res_uri = queryParamsMap.find("SURI")->second;
							std::string host_key = queryParamsMap.find("HOST")->second;
							gw_log("Observe res uri:%s", res_uri.c_str());
							GW_Resource *gw_res;
							sdm->GetResource(res_uri, &gw_res);

							if(!gw_res->is_observated())
							{
								QueryParamsMap haha;

								if(res_uri == "/ss/discomfort")
								{
									gw_res->observing();
								}
								else
								{
									gw_res->observing();
									GW_SensorArbiter::getInstance()->observe(*gw_res, haha);
								}
							}


							if(0 == gw_res->numberOfObs())
							{
								gw_res->addObsId(GW_SensorNotifier::getInstance()->getHostKey(host_key));
								snf->addInterestingResources(gw_res);
							}
							else
								gw_res->addObsId(GW_SensorNotifier::getInstance()->getHostKey(host_key));


						}

				}

				pResponse->setErrorCode(200);
				pResponse->setResponseResult(OC_EH_OK);

				pResponse->setResourceRepresentation(rep_result);

				// Bugfix: persistent response buffer is required
				//pResponse->
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
				{
					ehResult = OC_EH_OK;
				}
			}
		}

		/* Observe */
		if(requestFlag & RequestHandlerFlag::ObserverFlag)
		{
			ObservationInfo observationInfo = request->getObservationInfo();
			QueryParamsMap queryParamsMap = request->getQueryParameters();
			std::string res_uri = queryParamsMap.find("SURI")->second;
			std::string host_key = queryParamsMap.find("HOST")->second;
			gw_log("Observe res uri:%s", res_uri.c_str());
			GW_Resource *gw_res;
			sdm->GetResource(res_uri, &gw_res);
			if(res_uri == "/ss/discomfort" || gw_res == NULL)
				gw_log("/ss/discomfort is not initialized yet");
			if(ObserveAction::ObserveRegister == observationInfo.action)
			{
				if(!gw_res->is_observated())
				{
					QueryParamsMap qpm;

					if(res_uri == "/ss/discomfort")
					{
						gw_res->observing();
					}
					else
					{
						gw_res->observing();
						GW_SensorArbiter::getInstance()->observe(*gw_res, qpm);
					}
				}
				if(0 == gw_res->numberOfObs())
				{
					snf->addInterestingResources(gw_res);
				}
				GW_SensorNotifier::getInstance()->setHostKey(host_key, observationInfo.obsId);
				gw_res->addObsId(observationInfo.obsId);
			}
			else if(ObserveAction::ObserveUnregister == observationInfo.action)
			{
				/* Not implemented yet */
				/*  gw_res.erase(std::remove(m_interestedObservers.begin(),
						m_interestedObservers.end(),
						observationInfo.obsId),
						m_interestedObservers.end());
				 */
			}

			pthread_t threadId;
			//		              cout << "\t\trequestFlag : Observer\n";

			static int startedThread = 0;
			// Observation happens on a different thread in ChangeLightRepresentation function.
			// If we have not created the thread already, we will create one here.
			if(!startedThread)
			{
				pthread_create (&threadId, NULL, notifierWrapparFunc, NULL);
				startedThread = 1;
			}
			ehResult = OC_EH_OK;
		}

	}
	else
	{
		//			      std::cout << "Request invalid" << std::endl;
	}
	return ehResult;
}

void* notifierWrapparFunc(void *)
{
	GW_SensorNotifier *notifier = GW_SensorNotifier::getInstance();

	while(1)
	{
		
		notifier->notification();
		
		usleep(100 * 1000);
	}

	return NULL;
}

GW_Interface *GW_Interface::getInstance()
{
	if(NULL == instance)
	{
		instance = new GW_Interface();
	}

	return instance;
}

