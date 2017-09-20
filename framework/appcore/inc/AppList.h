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

#ifndef __APP_LIST_H__
#define __APP_LIST_H__

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

#include "App.h"

class AppList {
  public:
    ~AppList() {
      std::vector<App*>::iterator alIter;
      for(alIter = this->mApps.begin();
          alIter != this->mApps.end();
          ++alIter) {
        delete (*alIter);
        this->mApps.erase(alIter);
      }
      sqlite3_close(this->mDB);
    }

    static AppList* initializeFromDB(std::string dbPath, std::string systemAppsDir);
    void initializeDefaultApps(std::string systemAppsDir);

    int getAndIncreaseNextAppId() { return this->mNextAppId++; }

    std::vector<App*>& getApps() { return this->mApps; }
    App* getByAppId(int appId);
    App* getByPid(int pid);
    bool add(App* app);
    bool flush(App* app);
    bool remove(App* app);
    
  protected:
    std::vector<App*> mApps;
    sqlite3* mDB;

    // Current appId
    int mNextAppId;

    AppList() : mNextAppId(0) { }

    bool openDB(std::string dbPath);
    bool fetchAppList();
    bool createDBTable();
};

#endif // !defined(__APP_LIST_H__)
