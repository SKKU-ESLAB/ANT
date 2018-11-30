#ifndef _SQLwrapper_H_
#define _SQLwrapper_H_

//using namespace std;

#include "sqlite3.h"
//#include "sqlite3ext.h"
#include <sstream>
#include <iostream>
#include <string>
#include <assert.h>
#include <map>
#include <vector>

#ifndef NULL
#define NULL 0
#endif
#define DB_CLEANUP_COND_ASSERT(Exp, Cond, STRErrorMsg) \
			    { \
    if(Exp != Cond) \
		        { \
        assert(0); \
        goto CLEANUP; \
		        } \
			    }

#define DB_CLEANUP_ASSERT(Exp) \
			    { \
    if((res = (Exp)) != DB_S_OK) \
		        { \
        assert(0); \
        goto CLEANUP; \
		        } \
			    }

#define CHK_DBSQL(val, cond) {DB_CLEANUP_COND_ASSERT(val, cond, sqlite3_errmsg(m_pSQLite3));}

// Attribute type
enum Type { TYPE_NUMERIC, TYPE_INTEGER, TYPE_REAL, TYPE_TEXT };

// DB query execution result
enum DBRESULT
{
	DB_S_OK
	, DB_S_FALSE
	, DB_E_FAIL
	, DB_E_NOTIMPL
};

struct sensorDB{
	public:
	sensorDB()
	{}
	int index;
	std::string suri;
	std::string attribute;
	Type attributeType;
	int ttl;
	std::string value;
	int epoch_offset;
};

class DBManager{
private:
	sqlite3	*m_pSQLite3;
	
	DBManager()
	{
		initializeManager();
	}
	static DBManager *instance;
	
	// wrapper for processing simple query message
	DBRESULT executeSQL_NoReturn(std::string strSQL);
	DBRESULT executeSQL_IntReturn(std::string strSQL, int *pResult);

	
public:
	static DBManager *getInstance();

	// Open db handle and create tables if not exists
	DBRESULT initializeManager(void);
	// Close db handle
	DBRESULT terminateManager();

	// Add a row in the sensor index table
	// Create table for a attribute or add a row in the general attribute table
	DBRESULT addSensorAttribute(sensorDB *sDir);

	// Update a row in the sensor index table
	// Drop and recreate table for a attribute or add a row in the general attribute table
	DBRESULT updateSensorAttribute(sensorDB *sDir);

	// Delete a row in the sensor index table
	// Drop table for deleted attribute or delete a row in the general attribute table
	DBRESULT deleteSensorAttribute(sensorDB *sDir);

	// Read a row in the sensor index table
	DBRESULT getSensorAttribute(sensorDB *sDir);

	// Find attribute of suri
	DBRESULT getSensorAttrValueMap(sensorDB *sDir, std::map<std::string, std::string> *out);

	// Add a row in the attribute value table
	// If the oldest row is expired, delete oldest row
	DBRESULT addSensorData(sensorDB *sDir);

	DBRESULT getSensorVector(sensorDB *sDir, int epoch_offset, std::map<std::string, std::map<int, std::string>> *out);

	DBRESULT getSensorData(sensorDB *sDir, int epoch_offset, std::map<int, std::string> *out);

	// Timestamp to Unix Epoch
	DBRESULT Time2Epoch(const char *time, int* unix_epoch);

	// Unix Epoch to Timestamp
	DBRESULT Epoch2Time(char *time, const int* unix_epoch);

	// Get current Epoch
	DBRESULT getCurEpoch(int* unix_epoch);

	// Read a row in the attribute value table
	DBRESULT getRecentSensorData(sensorDB *sDir);

	// Discarding old sensor data row if timestamp is too old
	DBRESULT discardOldData(sensorDB *sDir);
};
#endif
