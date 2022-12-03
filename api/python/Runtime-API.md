
# ANTRuntime



## Class
ANTRuntime(m_current_app)

**Kind**: class

| Param | Type | Description |
| --- | --- | --- |
| m_current_app | method |get application |

## Method

### set_current_app(onInitialize, onStart, onStop)

Application

**Kind**: class method

| Param | Type | Description |
| --- | --- | --- |
| onInitialize | method | callback handler invoked on application's initialization. |
| onStart | method | callback handler invoked when application starts. |
| onStop | method | callback handler invoked when application stops. |


### remove_current_app()


Application

**Kind**: class method

| Param | Type | Description |
| --- | --- | --- |
| onInitialize | method | callback handler invoked on application's initialization. |
| onStart | method | callback handler invoked when application starts. |
| onStop | method | callback handler invoked when application stops. |


### get_current_app()

this method return current app

**Kind**: class method


### download_file_via_http(url, downloadPath)

this method use ant_runtime_downloadFileViaHTTP function in native.

**Kind**: class method

| Param | Type | Description |
| --- | --- | --- |
| url | string | url |
| downloadPath | string | source code path for download|


### unarchive(archiveFilePath, targetDirPath)

this method use native function ant_runtime_unarchive to archiveFile to target path

**Kind**: class method

| Param | Type | Description |
| --- | --- | --- |
| archiveFilePath | string | archive File Path |
| targetDirPath | string | target Dirtory Path|



## Class
APP(onInitialize,onStart,onStop)

class

**Kind**: class

| Param | Type | Description |
| --- | --- | --- |
| onInitialize | method | callback handler invoked on application's initialization. |
| onStart | method | callback handler invoked when application starts. |
| onStop | method | callback handler invoked when application stops. |


### start()

start target app

**Kind**: class method


### stop()

stop target app

**Kind**: class method

### getState()

get state target app. return state string date

**Kind**: class method

### getInfo()

getInfo target app

**Kind**: class method