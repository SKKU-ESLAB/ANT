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

#include <sqlite3.h>

#include "AppList.h"
#include "ANTdbugLog.h"

#define PATH_BUFFER_SIZE 1024
#define QUERY_BUFFER_LENGTH 2048

AppList* AppList::initializeFromDB(std::string dbPath,
    std::string systemAppsDir) {
  AppList* appList = new AppList();

  // Open DB
  bool openDBRes = appList->openDB(dbPath);
  if(!openDBRes) {
    delete appList;
    return NULL;
  }

  // Try to fetch app list from DB
  bool fetchAppListRes = appList->fetchAppList();
  if(fetchAppListRes) {
    // If fetching is successful, return it
    appList->initializeDefaultApps(systemAppsDir);
    return appList;
  }

  // If fetching is failed, crate a new table
  appList->createDBTable();
  bool createDBTableRes = appList->createDBTable();
  if(!createDBTableRes) {
    delete appList;
    return NULL;
  }

  // Add default apps and flush their information
  appList->initializeDefaultApps(systemAppsDir);

  return appList;
}

void AppList::initializeDefaultApps(std::string systemAppsDir) {
  int systemAppNamesCount = 4;
  const char* systemAppNames[] = {
    "CameraViewer", "SensorViewer",
    "MotionClassifier", "ImageClassifier"
  };

  for(int i = 0; i < systemAppNamesCount; i++) {
    if(this->getByName(systemAppNames[i]) != NULL) {
      continue;
    }

    // If the system app does not exist, create an entry.
    char packagePath[PATH_BUFFER_SIZE];
    char mainJSFileName[PATH_BUFFER_SIZE];
    char iconFileName[PATH_BUFFER_SIZE];
    snprintf(packagePath, PATH_BUFFER_SIZE, "%s/%s",
        systemAppsDir.c_str(), systemAppNames[i]);
    snprintf(mainJSFileName, PATH_BUFFER_SIZE, "%s",
        "index.js");
    snprintf(iconFileName, PATH_BUFFER_SIZE, ".");
    App* app = new App(this->mNextAppId++, true, systemAppNames[i],
        packagePath, mainJSFileName, iconFileName,
        AppState::Ready);
    ANT_DBG_ERR("Add %s: %s, %s",
        systemAppNames[i], packagePath, mainJSFileName);
    this->add(app);
    this->flush(app);
  }
}

bool AppList::openDB(std::string dbPath) {
  // Open DB
  int openRes = sqlite3_open(dbPath.c_str(), &this->mDB);
  if(openRes != SQLITE_OK) {
    ANT_DBG_ERR("Fail to open DB! (%s)", dbPath.c_str());
    return false;
  }
  return true;
}

bool AppList::fetchAppList() {
  // Fetch all the app's information from DB
  char query[QUERY_BUFFER_LENGTH] = "SELECT * from APPLIST;";
  sqlite3_stmt* stmt;

  int prepareRes = sqlite3_prepare_v2(
      this->mDB, query, -1, &stmt, NULL);
  if(prepareRes != SQLITE_OK) {
    return false;
  }

  int maxAppId = 0;
  int numApps = 0;
  while(sqlite3_step(stmt) == SQLITE_ROW) {
    // Get fields
    int appId = sqlite3_column_int(stmt, 0);
    bool isDefaultApp = (sqlite3_column_int(stmt, 1) != 0);
    std::string name(reinterpret_cast<const char*>(
          sqlite3_column_text(stmt, 2)));
    std::string packagePath(reinterpret_cast<const char*>(
          sqlite3_column_text(stmt, 3)));
    std::string mainJSFileName(reinterpret_cast<const char*>(
          sqlite3_column_text(stmt, 4)));
    std::string iconFileName(reinterpret_cast<const char*>(
          sqlite3_column_text(stmt, 5)));

    // Check this field has already loaded
    App* app = this->getByAppId(appId);
    if(app != NULL)
      continue;
    
    // Allocate a new entry and add to on-memory list
    app = new App(appId, isDefaultApp, name,
        packagePath, mainJSFileName, iconFileName,
        AppState::Ready);
    this->mApps.push_back(app);

    numApps++;
    if(appId > maxAppId) {
      maxAppId = appId;
    }
  }
  sqlite3_finalize(stmt);

  // Update next app id
  this->mNextAppId = maxAppId + 1;

  if(numApps > 0)
    return true;
  else
    return false;
}

bool AppList::createDBTable() {
  // Create table
  char query[QUERY_BUFFER_LENGTH] = "CREATE TABLE IF NOT EXISTS APPLIST(" \
                      "ID INTEGER PRIMARY KEY," \
                      "ISDEFAULTAPP INTEGER NOT NULL," \
                      "NAME TEXT NOT NULL," \
                      "PACKAGEPATH TEXT NOT NULL," \
                      "MAINJSFILENAME TEXT NOT NULL," \
                      "ICONFILENAME TEXT NOT NULL);";

  char* createErr;
  int createRes = sqlite3_exec(this->mDB, query, NULL, NULL, &createErr);
  if(createRes != SQLITE_OK) {
    ANT_DBG_ERR("Fail to create DB table! (%s)", createErr);
    sqlite3_free(createErr);
    return false;
  }

  ANT_DBG_VERB("Succeed to create DB table!");
  return true;
}

App* AppList::getByAppId(int appId) {
  std::vector<App*>::iterator alIter;
  for(alIter = this->mApps.begin();
      alIter != this->mApps.end();
      ++alIter) {
    int thisAppId = (*alIter)->getId();
    if(thisAppId == appId) {
      return (*alIter);
    }
  }
  return NULL;
}

App* AppList::getByPid(int pid) {
  std::vector<App*>::iterator alIter;
  for(alIter = this->mApps.begin();
      alIter != this->mApps.end();
      ++alIter) {
    int thisPid = (*alIter)->getPid();
    if(thisPid == pid) {
      return (*alIter);
    }
  }
  return NULL;
}

App* AppList::getByName(std::string appName) {
  std::vector<App*>::iterator alIter;
  for(alIter = this->mApps.begin();
      alIter != this->mApps.end();
      ++alIter) {
    std::string thisAppName((*alIter)->getName());
    if(thisAppName.compare(appName) == 0) {
      return (*alIter);
    }
  }
  return NULL;
}

bool AppList::add(App* app) {
  // Add to on-memory list
  this->mApps.push_back(app);
  return true;
}

bool AppList::flush(App* app) {
  // Get fields to be stored to DB
  int appId = app->getId();
  bool isDefaultApp = app->isDefaultApp();
  std::string name(app->getName());
  std::string packagePath(app->getPackagePath());
  std::string mainJSFileName(app->getMainJSFileName());
  std::string iconFileName(app->getIconFileName());

  // Store to DB
  char query[QUERY_BUFFER_LENGTH];
  snprintf(query, QUERY_BUFFER_LENGTH,
      "INSERT INTO APPLIST" \
      "(ID, ISDEFAULTAPP, NAME, PACKAGEPATH, MAINJSFILENAME, ICONFILENAME)" \
      "VALUES ('%d', '%d', '%s', '%s', '%s', '%s');",
      appId, ((isDefaultApp) ? 1 : 0), name.c_str(),
      packagePath.c_str(), mainJSFileName.c_str(), iconFileName.c_str());

  char* insertErr;
  int insertRes = sqlite3_exec(this->mDB, query, NULL, NULL, &insertErr);
  if(insertRes != SQLITE_OK) {
    ANT_DBG_ERR("Fail to store a new app information to DB (%s)", insertErr);
    sqlite3_free(insertErr);
    return false;
  }
  return true;
}

bool AppList::remove(App* app) {
  bool isSuccess = false;
  int appId = app->getId();

  // Remove from on-memory list
  std::vector<App*>::iterator alIter;
  for(alIter = this->mApps.begin();
      alIter != this->mApps.end();
      ++alIter) {
    if((*alIter)->getId() == appId) {
      delete (*alIter);
      this->mApps.erase(alIter);
      isSuccess = true;
    }
  }
  if(!isSuccess) {
    ANT_DBG_ERR("Fail to remove app from app list (%d)", appId);
    return false;
  }

  // Remove from DB
  char query[QUERY_BUFFER_LENGTH];
  snprintf(query, QUERY_BUFFER_LENGTH, "DELETE FROM APPLIST WHERE ID=%d;", appId);
  char* insertErr;
  int insertRes = sqlite3_exec(this->mDB, query, NULL, NULL, &insertErr);
  if(insertRes != SQLITE_OK) {
    ANT_DBG_ERR("Fail to remove app from DB (%d, %s)", appId, insertErr);
    sqlite3_free(insertErr);
    return false;
  }
  return true;
}
