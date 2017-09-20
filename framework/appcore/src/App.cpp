/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "App.h"
#include "ANTdbugLog.h"

bool App::changeState(AppState::Value newState) {
  // Check new state
  bool isChangeApproved = false;
  switch(this->mState) {
    case AppState::Initializing:
      if(newState == AppState::Initialized)
        isChangeApproved = true;
      break;
    case AppState::Initialized:
      if(newState == AppState::Installing
          || newState == AppState::Removed)
        isChangeApproved = true;
      break;
    case AppState::Installing:
      if(newState == AppState::Ready
          || newState == AppState::Removed)
        isChangeApproved = true;
      break;
    case AppState::Ready:
      if(newState == AppState::Launching
          || newState == AppState::Removing)
        isChangeApproved = true;
      break;
    case AppState::Launching:
      if(newState == AppState::Running
          || newState == AppState::Ready)
        isChangeApproved = true;
      break;
    case AppState::Running:
      if(newState == AppState::Terminating
          || newState == AppState::Ready)
        isChangeApproved = true;
      break;
    case AppState::Terminating:
      if(newState == AppState::Ready)
        isChangeApproved = true;
      break;
    case AppState::Removing:
      if(newState == AppState::Removed)
        isChangeApproved = true;
      break;
    case AppState::Removed:
      break;
  }

  // Apply new state
  if(isChangeApproved) {
    this->mState = newState;
    if(this->mStateListener != NULL) {
      this->mStateListener->onChangedState(this->getId(), this->mState);
    }
  }
  return isChangeApproved;
}

bool App::setFromManifest(std::string manifestFilePath) {
	xmlDocPtr xmlDoc;
	xmlNodePtr xmlNode;

	xmlDoc = xmlParseFile(manifestFilePath.c_str());
	if (xmlDoc == NULL) {
		ANT_DBG_ERR("Document not parsed successfully. \n");
		return false;
	}

	xmlNode = xmlDocGetRootElement(xmlDoc);
	if (xmlNode == NULL) {
		ANT_DBG_ERR("empty document\n");
		xmlFreeDoc(xmlDoc);
		return false;
	}

	if (xmlStrcmp(xmlNode->name, (const xmlChar *) "application")) {
		ANT_DBG_ERR("document of the wrong type, root node != story");
		xmlFreeDoc(xmlDoc);
		return false;
	}

	xmlNode = xmlNode->xmlChildrenNode;

	char iconFileName[PATH_BUFFER_SIZE] = {0, };
	char appName[PATH_BUFFER_SIZE] = {0, };
	char mainJSFileName[PATH_BUFFER_SIZE] = {0, };
	
	while (xmlNode != NULL) {
		if (!xmlStrcmp(xmlNode->name, (const xmlChar *)"icon")){
			xmlChar *key = xmlNodeListGetString(xmlDoc, xmlNode->xmlChildrenNode, 1);
			sprintf(iconFileName, "%s", key);
			printf("[AppPackageManager] Parse XML >> keyword: %s %s\n", xmlNode->name, iconFileName);
			xmlFree(key);
		} else if (!xmlStrcmp(xmlNode->name, (const xmlChar *)"label")){
			xmlChar *key = xmlNodeListGetString(xmlDoc, xmlNode->xmlChildrenNode, 1);
			sprintf(appName, "%s", key);
			printf("[AppPackageManager] Parse XML >> keyword: %s %s\n", xmlNode->name, appName);
			xmlFree(key);
    } else if (!xmlStrcmp(xmlNode->name, (const xmlChar *)"mainFile")){
			xmlChar *key = xmlNodeListGetString(xmlDoc, xmlNode->xmlChildrenNode, 1);
			sprintf(mainJSFileName, "%s", key);
			printf("[AppPackageManager] Parse XML >> keyword: %s %s\n", xmlNode->name, mainJSFileName);			
			xmlFree(key);
		}
    xmlNode = xmlNode->next;
	}
	xmlFreeDoc(xmlDoc);

  if(*iconFileName == 0 || *appName == 0 || mainJSFileName == 0) {
    ANT_DBG_ERR("at least a field is missing in app manifest!");
    return false;
  }

  // Set fields
  this->mName.assign(appName);
  this->mIsDefaultApp = false;
  this->mMainJSFileName.assign(mainJSFileName);
  this->mIconFileName.assign(iconFileName);

  return true;
}

void App::finishInitializing(int appId) { // Initializing -> Initialized
  this->mId = appId;
  this->changeState(AppState::Initialized);
}

void App::startInstalling() { // Initialized -> Installing
  this->changeState(AppState::Installing);
}

void App::successInstalling(std::string packagePath) { // Installing -> Ready
  this->mPackagePath = packagePath;
  this->changeState(AppState::Ready);
}

void App::failInstalling() { // Installing -> Removed
  this->changeState(AppState::Removed);
}

void App::startLaunching(int pid) { // Ready -> Launching
  this->mPid = pid;
  this->changeState(AppState::Launching);
}

void App::successLaunching() { // Launching -> Running
  this->changeState(AppState::Running);
}

void App::failLaunching() { // Launching -> Ready
  this->changeState(AppState::Ready);
}

void App::startTerminating() { // Running -> Terminating
  this->changeState(AppState::Terminating);
}

void App::finishTerminating() { // Terminating -> Ready
  this->changeState(AppState::Ready);
}

void App::startRemoving() { // Ready -> Removing
  this->changeState(AppState::Removing);
}

void App::finishRemoving() { // Removing -> Removed
  this->changeState(AppState::Removed);
}
