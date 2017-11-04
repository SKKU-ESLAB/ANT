
# This directory is to make shared object file of sensor manager

***Notice: Please leave 'libsensors.so' file in the directory
(cmake file refers to that file when compile \
 and also does binary file)

## Sensor List

The list of sensor is SENSOR_LIST file in this directory.
Sensor Manager will use this file to figure out what sensors it has.
The list is not order-sensitive. 

## How to make shared object file with source files
1. make directory 'build'
2. cd build
3. cmake ../
4. make
5. cp libsensors.so ../

Shared object file will be shown.

## How to customize shared object file with new driver codes
1. Write your own driver code referring to previous driver code 
(Driver codes are in the 'drivers/' directory)
you need to include 'devices.h' (in the 'inc/' dir) to bind with the sensor manager.
The couplings between sensor manager and driver code will be removed later.

2. Modify the CMakeLists.txt file
Modify the ${driver_sources} part to refer to the new driver code.

