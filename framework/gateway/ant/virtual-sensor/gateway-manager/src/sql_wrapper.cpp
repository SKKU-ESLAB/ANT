#include "sql_wrapper.h"
#include <cstdio>
#include <cstring>

DBManager *DBManager::instance = NULL;

DBManager *DBManager::getInstance()
{
	if (NULL == instance)
	{
		instance = new DBManager();
	}

	return instance;
}

//DBManager dbm;

DBRESULT DBManager::initializeManager(){
	DBRESULT res = DB_E_FAIL;

	m_pSQLite3 = NULL;

	const char *create_table = "create table [sensorindex] (idx integer primary key autoincrement, suri text, attribute text, attributetype integer, ttl integer);";
	const char *table_check = "select count(*) from sqlite_master where name = \"sensorindex\"";
	int isCreated = 0;

	CHK_DBSQL(sqlite3_open_v2("olddata", &m_pSQLite3, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL), SQLITE_OK);
	
	// If sensorindex table is not exists, create table
	CHK_DBSQL(executeSQL_IntReturn(std::string(table_check), &isCreated), SQLITE_OK);
	if (!isCreated) CHK_DBSQL(executeSQL_NoReturn(std::string(create_table)), SQLITE_OK);

CLEANUP:
	return res;
}

DBRESULT DBManager::terminateManager(){
	DBRESULT res = DB_S_OK;
	CHK_DBSQL(sqlite3_close_v2(m_pSQLite3), SQLITE_OK);
CLEANUP:
	return res;
}

// Processing simple query message (Lower level wrapper)
DBRESULT DBManager::executeSQL_NoReturn(std::string strSQL){
	DBRESULT		res = DB_E_FAIL;
	sqlite3_stmt	*stmt = NULL;

	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
	CHK_DBSQL(sqlite3_step(stmt), SQLITE_DONE);
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);

	res = DB_S_OK;
CLEANUP:
	return res;

}
DBRESULT DBManager::executeSQL_IntReturn(std::string strSQL, int *pResult){
	DBRESULT		res = DB_E_FAIL;
	sqlite3_stmt	*stmt = NULL;

	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
	CHK_DBSQL(sqlite3_step(stmt), SQLITE_ROW);
	*pResult = sqlite3_column_int(stmt, 0);
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);

	res = DB_S_OK;
CLEANUP:
	return res;

}

// Add a row in the sensor index table
// Create a table for a attribute or add a row in the general attribute table
// OK!
DBRESULT DBManager::addSensorAttribute(sensorDB *sDir){
	DBRESULT res = DB_E_FAIL;
	std::string strSQL;
	std::stringstream sstream;
	sqlite3_stmt *stmt = NULL;
	
	DB_CLEANUP_ASSERT(getSensorAttribute(sDir));
	
	// Make SQL string base
	sstream << "insert into sensorindex (suri,attribute,attributetype,ttl) values (?,?,?,?);";

	// Compile SQL string into sqlite statement
	strSQL = sstream.str();
	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.size(), &stmt, NULL), SQLITE_OK);
	sstream.str("");

	// Bind types of input data
	CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->suri.c_str(), sDir->suri.size(), SQLITE_STATIC), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_text(stmt, 2, sDir->attribute.c_str(), sDir->attribute.size(), SQLITE_STATIC), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_int(stmt, 3, (int)sDir->attributeType), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_int(stmt, 4, sDir->ttl), SQLITE_OK);

	// Execute sqlite statement
	CHK_DBSQL(sqlite3_step(stmt), SQLITE_DONE);
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);

	// Get S-URI and Attribute index number
	DB_CLEANUP_ASSERT(executeSQL_IntReturn("select last_insert_rowid();", &sDir->index));

	// Create table for Attribute data
	if (sDir->ttl > 0){
		// Set table name
		sstream << "create table if not exists [" << sDir->suri << "/" << sDir->attribute << "]";
		// Set column info
		sstream << "(idx integer primary key autoincrement, timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP, value";
		switch (sDir->attributeType){
		case TYPE_NUMERIC:
			sstream << " numeric";
			break;
		case TYPE_INTEGER:
			sstream << " integer";
			break;
		case TYPE_REAL:
			sstream << " real";
			break;
		case TYPE_TEXT:
			sstream << " text";
			break;
		default:
			res = DB_E_FAIL;
			goto CLEANUP;
		}
		sstream << ");" << std::ends;
		DB_CLEANUP_ASSERT(executeSQL_NoReturn(sstream.str()));
	}
	else{
		// Create table for meta data if not exists
		// Set table name
		sstream << "create table if not exists [" << sDir->suri << "/meta]";
		// Set column info
		sstream << "(idx integer primary key autoincrement, attribute text, value text);" << std::ends;
		DB_CLEANUP_ASSERT(executeSQL_NoReturn(sstream.str()));
		sstream.str("");
	}
	res = DB_S_OK;
CLEANUP:
	return res;
}

// Do not use update query to synchronize the return value of last_update_rowid() and idx number
DBRESULT DBManager::updateSensorAttribute(sensorDB *sDir){
	DBRESULT res = DB_E_FAIL;
	/*
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;
	unsigned int index = sDir->index;

	// Make SQL string
	sstream << "update [sensorDir] set suri = ?, attribute = ? where sensorIndex = ?;";

	// Compile SQL string into sqlite stmt
	strSQL = sstream.str();
	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
	sstream.str("");

	// Bind types of input data
	CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->suri, sDir->suri.size(), SQLITE_STATIC), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_text(stmt, 2, sDir->attribute, sDir->attribute.size(), SQLITE_STATIC), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_int(stmt, 3,  sDir->index), SQLITE_OK);

	// Execute sqlite statement
	CHK_DBSQL(sqlite3_step(stmt), SQLITE_DONE);
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);

	// Get S-URI and Attribute index number
	DB_CLEANUP_ASSERT(executeSQL_IntReturn("select last_insert_rowid();", &sDir->index));

	// Update failed
	if(index != sDir->index){
	goto CLEANUP;
	}
	*/
	DB_CLEANUP_ASSERT(deleteSensorAttribute(sDir));
	DB_CLEANUP_ASSERT(addSensorAttribute(sDir));
	res = DB_S_OK;
CLEANUP:
	return res;
}

DBRESULT DBManager::deleteSensorAttribute(sensorDB *sDir){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;

	if (getSensorAttribute(sDir) != DB_S_OK)
		return DB_S_FALSE;
	
	// Drop attribute table
	if (sDir->ttl > 0){
		sstream << "drop table if exists [" << sDir->suri << "/" << sDir->attribute << "];";
		DB_CLEANUP_ASSERT(executeSQL_NoReturn(sstream.str()));
	}
	// Delete a row in the meta table
	else{
		sstream << "delete from [" << sDir->suri << "/meta] where attribute = ?";
		CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
		CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->attribute.c_str(), sDir->attribute.size(), SQLITE_STATIC), SQLITE_OK);
		CHK_DBSQL(sqlite3_step(stmt), SQLITE_DONE);
		CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	}
	sstream.str("");

	// Make SQL string base
	sstream << "delete from sensorindex where suri=? and attribute=?;";

	// Compile SQL string into sqlite statement
	strSQL = sstream.str();
	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
	sstream.str("");
	// Bind types of input data
	CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->suri.c_str(), sDir->suri.size(), SQLITE_STATIC), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_text(stmt, 2, sDir->attribute.c_str(), sDir->attribute.size(), SQLITE_STATIC), SQLITE_OK);
	// Execute sqlite statement
	CHK_DBSQL(sqlite3_step(stmt), SQLITE_DONE);
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);

	res = DB_S_OK;
CLEANUP:
	return res;
}

// OK!!
DBRESULT DBManager::getSensorAttribute(sensorDB *sDir){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;

	// Make SQL string
	sstream << "select * from sensorindex where suri=? and attribute=?";
	//sstream << "insert into [sensorindex] (suri,attribute,ttl) values ("<<pIndex->suri<<","<<pIndex->attribute<<","<<pIndex->ttl<<");";

	// Compile SQL string into sqlite stmt
	strSQL = sstream.str();
	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.size(), &stmt, NULL), SQLITE_OK);
	sstream.str("");

	// Bind types of input data
	CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->suri.c_str(), sDir->suri.size(), SQLITE_STATIC), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_text(stmt, 2, sDir->attribute.c_str(), sDir->attribute.size(), SQLITE_STATIC), SQLITE_OK);

	if (sqlite3_step(stmt) == SQLITE_ROW){
		sDir->index = sqlite3_column_int(stmt, 0);
//		sDir->suri = std::string((char*)sqlite3_column_text(stmt, 1)); // char* to string?
//		sDir->attribute = std::string((char*)sqlite3_column_text(stmt, 2)); // char* to string?
		sDir->attributeType = (Type)sqlite3_column_int(stmt, 3);
		sDir->ttl = sqlite3_column_int(stmt, 4);

		CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
		res = DB_S_OK;
		goto CLEANUP;
	}
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	res = DB_S_OK;
CLEANUP:
	return res;
}

// 
DBRESULT DBManager::getSensorAttrValueMap(sensorDB *sDir, std::map<std::string, std::string> *out){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;

	// Make SQL string
	sstream << "select * from sensorindex where suri=?";

	// Compile SQL string into sqlite stmt
	strSQL = sstream.str();
	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.size(), &stmt, NULL), SQLITE_OK);
	sstream.str("");

	// Bind types of input data
	CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->suri.c_str(), sDir->suri.size(), SQLITE_STATIC), SQLITE_OK);

	while (sqlite3_step(stmt) == SQLITE_ROW){
		sDir->attribute = std::string((char*)sqlite3_column_text(stmt, 2)); // char* to string?
		sDir->attributeType = (Type)sqlite3_column_int(stmt, 3);
		sDir->ttl = sqlite3_column_int(stmt, 4);
		DB_CLEANUP_ASSERT(getRecentSensorData(sDir));
		(*out)[sDir->attribute.c_str()] = sDir->value.c_str();
		memset(sDir, 0, sizeof(sDir));
	}
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	res = DB_S_OK;
CLEANUP:
	return res;
}

// OK!
DBRESULT DBManager::addSensorData(sensorDB *sDir){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	char timestring[30];
	int epoch;
	sqlite3_stmt *stmt = NULL;

	DB_CLEANUP_ASSERT(getSensorAttribute(sDir));

	if (sDir->ttl > 0){
		// Make SQL string
		sstream << "insert into [" << sDir->suri << "/" << sDir->attribute << "] (timestamp,value) values (?,?);";

		// Compile SQL string into sqlite stmt
		strSQL = sstream.str();
		CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.size(), &stmt, NULL), SQLITE_OK);
		sstream.str("");

		DB_CLEANUP_ASSERT(getCurEpoch(&epoch));

		epoch -= sDir->epoch_offset;
		DB_CLEANUP_ASSERT(Epoch2Time(timestring, &epoch));

		sqlite3_bind_text(stmt, 1, timestring, strlen(timestring), SQLITE_OK);
		// Bind types of input data
		switch (sDir->attributeType){
		case TYPE_NUMERIC:
		case TYPE_INTEGER:
			CHK_DBSQL(sqlite3_bind_int(stmt, 2, atoi(sDir->value.c_str())), SQLITE_OK);
			break;
		case TYPE_REAL:
			CHK_DBSQL(sqlite3_bind_double(stmt, 2, atof(sDir->value.c_str())), SQLITE_OK);
			break;
		case TYPE_TEXT:
			CHK_DBSQL(sqlite3_bind_text(stmt, 2, sDir->value.c_str(), sDir->value.size(), SQLITE_STATIC), SQLITE_OK);
			break;
		default:
			res = DB_E_FAIL;
			goto CLEANUP;
		}
	}
	else{
		sstream << "insert into [" << sDir->suri << "/meta] (attribute,value) values (?,?);";
		strSQL = sstream.str();
		CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.size(), &stmt, NULL), SQLITE_OK);
		CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->attribute.c_str(), sDir->attribute.size(), SQLITE_STATIC), SQLITE_OK);
		CHK_DBSQL(sqlite3_bind_text(stmt, 2, sDir->value.c_str(), sDir->value.size(), SQLITE_STATIC), SQLITE_OK);
	}
	// Execute sqlite statement
	CHK_DBSQL(sqlite3_step(stmt), SQLITE_DONE);
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);

	// Get S-URI and Attribute index number
	DB_CLEANUP_ASSERT(executeSQL_IntReturn("select last_insert_rowid();", &sDir->index));

	//Apply TTL cleanup
	//DB_CLEANUP_ASSERT(discardOldData(sDir));

	res = DB_S_OK;
CLEANUP:
	return res;
}

// OK!
DBRESULT DBManager::getRecentSensorData(sensorDB *sDir){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;
	char *output = (char*)malloc(sizeof(char) * 20);

	DB_CLEANUP_ASSERT(getSensorAttribute(sDir));

	if (sDir->ttl > 0){
		// Make SQL string
		sstream << "select value from [" << sDir->suri << "/" << sDir->attribute << "] order by timestamp desc limit 1;";

		// Compile SQL string into sqlite stmt
		strSQL = sstream.str();
		CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
		sstream.str("");

		if (sqlite3_step(stmt) == SQLITE_ROW){
			switch (sDir->attributeType){
			case TYPE_NUMERIC:
			case TYPE_INTEGER:
				sprintf(output, "%d", sqlite3_column_int(stmt, 0));
				sDir->value = std::string(output);
				break;
			case TYPE_REAL:
				sprintf(output, "%f", sqlite3_column_double(stmt, 0));
				sDir->value = std::string(output);
				break;
			case TYPE_TEXT:
				sDir->value = std::string((char*)sqlite3_column_text(stmt, 0));
				break;
			default:
				res = DB_E_FAIL;
				goto CLEANUP;
			}
			res = DB_S_OK;
		}
		CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	}
	else{
		sstream << "select value from [" << sDir->suri << "/meta] where attribute=?;";
		strSQL = sstream.str();
		CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
		sstream.str("");
		CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->attribute.c_str(), sDir->attribute.size(), SQLITE_STATIC), SQLITE_OK);
		if (sqlite3_step(stmt) == SQLITE_ROW){
			sDir->value = std::string((char*)sqlite3_column_text(stmt, 0));
			res = DB_S_OK;
		}
		CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	}
CLEANUP:
	return res;
}

DBRESULT DBManager::discardOldData(sensorDB *sDir){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;
	sqlite3_stmt *stmt_del = NULL;
	char timestring[30] = { 0 };
	int row_epoch, cur_epoch;

	DB_CLEANUP_ASSERT(getSensorAttribute(sDir));

	if (sDir->ttl > 0){
		// Make SQL string
		sstream << "select timestamp from [" << sDir->suri << "/" << sDir->attribute << "] order by timestamp asc;";

		// Compile SQL string into sqlite stmt
		strSQL = sstream.str();
		CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
		sstream.str("");

		while (sqlite3_step(stmt) == SQLITE_ROW){
			memset(timestring, 0, strlen(timestring));
			strcpy(timestring, (char*)sqlite3_column_text(stmt, 0));
			DB_CLEANUP_ASSERT(Time2Epoch(timestring, &row_epoch));
			DB_CLEANUP_ASSERT(getCurEpoch(&cur_epoch));
			if (row_epoch > cur_epoch - sDir->ttl){
				res = DB_S_OK;
				goto CLEANUP;
			}
			sstream << "delete from [" << sDir->suri << "/" << sDir->attribute << "] where timestamp = ?;";
			strSQL = sstream.str();
			CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt_del, NULL), SQLITE_OK);
			printf("%s\n", timestring);
			CHK_DBSQL(sqlite3_bind_text(stmt_del, 1, timestring, strlen(timestring), SQLITE_STATIC), SQLITE_OK);
			CHK_DBSQL(sqlite3_step(stmt_del), SQLITE_DONE);
			CHK_DBSQL(sqlite3_finalize(stmt_del), SQLITE_OK);
		}
	}
	res = DB_S_OK;
CLEANUP:
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	return res;
}


DBRESULT DBManager::Time2Epoch(const char *time, int* unix_epoch){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;

	sstream << "select strftime('%s',?);";
	strSQL = sstream.str();

	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_text(stmt, 1, time, strlen(time), SQLITE_STATIC), SQLITE_OK);
	if (sqlite3_step(stmt) == SQLITE_ROW){
		(*unix_epoch) = sqlite3_column_int(stmt, 0);
	}
	res = DB_S_OK;
CLEANUP:
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	return res;
}

DBRESULT DBManager::Epoch2Time(char *time, const int* unix_epoch){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;

	char *time_temp;

	sstream << "select datetime(?,'unixepoch');";
	strSQL = sstream.str();

	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
	CHK_DBSQL(sqlite3_bind_int(stmt, 1, (*unix_epoch)), SQLITE_OK);
	if (sqlite3_step(stmt) == SQLITE_ROW){
		time_temp = (char*)sqlite3_column_text(stmt, 0);
		strcpy(time, time_temp);
	}
	res = DB_S_OK;
CLEANUP:
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	return res;
}

DBRESULT DBManager::getCurEpoch(int* unix_epoch){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	sqlite3_stmt *stmt = NULL;
	sstream << "select strftime('%s','now');";
	CHK_DBSQL(executeSQL_IntReturn(sstream.str(), unix_epoch), SQLITE_OK);
	res = DB_S_OK;
CLEANUP:
	return res;
}

DBRESULT DBManager::getSensorVector(sensorDB *sDir, int epoch_offset, std::map<std::string,std::map<int, std::string>> *out){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;

	// Make SQL string
	sstream << "select * from sensorindex where suri=?";

	// Compile SQL string into sqlite stmt
	strSQL = sstream.str();
	CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.size(), &stmt, NULL), SQLITE_OK);
	sstream.str("");

	// Bind types of input data
	CHK_DBSQL(sqlite3_bind_text(stmt, 1, sDir->suri.c_str(), sDir->suri.size(), SQLITE_STATIC), SQLITE_OK);

	while (sqlite3_step(stmt) == SQLITE_ROW){
		std::map<int, std::string> temp;
		sDir->attribute = std::string((char*)sqlite3_column_text(stmt, 2)); // char* to string?
		sDir->attributeType = (Type)sqlite3_column_int(stmt, 3);
		sDir->ttl = sqlite3_column_int(stmt, 4);
		if (sDir->ttl <= 0) continue;
		DB_CLEANUP_ASSERT(getSensorData(sDir, epoch_offset, &temp));
		memset(sDir, 0, sizeof(sDir));
		(*out)[sDir->attribute] = temp;
	}
	res = DB_S_OK;
CLEANUP:
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	return res;
}

DBRESULT DBManager::getSensorData(sensorDB *sDir, int epoch_offset, std::map<int, std::string> *out){
	DBRESULT res = DB_E_FAIL;
	std::stringstream sstream;
	std::string strSQL;
	sqlite3_stmt *stmt = NULL;
	char *output = (char*)malloc(sizeof(char) * 20);
	int cur_epoch, value_epoch;
	char timestamp[21];

	DB_CLEANUP_ASSERT(getSensorAttribute(sDir));

	if (sDir->ttl > 0){
		// Make SQL string
		sstream << "select timestamp,value from [" << sDir->suri << "/" << sDir->attribute << "] order by timestamp desc;";

		// Compile SQL string into sqlite stmt
		strSQL = sstream.str();
		CHK_DBSQL(sqlite3_prepare_v2(m_pSQLite3, strSQL.c_str(), strSQL.length(), &stmt, NULL), SQLITE_OK);
		sstream.str("");

		while (sqlite3_step(stmt) == SQLITE_ROW){
			// Get value's timestamp into value_epoch
			strcpy(timestamp, (const char*)sqlite3_column_text(stmt, 0));
			DB_CLEANUP_ASSERT(Time2Epoch(timestamp, &value_epoch));
			// Get current timestamp into cur_epoch
			DB_CLEANUP_ASSERT(getCurEpoch(&cur_epoch));
			// if value's timestamp is too old, finishing get values because result rows are selected in descending order by index(timestamp)
			//printf("%d < %d - %d\n", value_epoch, cur_epoch, epoch_offset);
			if (value_epoch < cur_epoch - epoch_offset){
				//printf("exiting...\n");
				break;
			}

			switch (sDir->attributeType){
			case TYPE_NUMERIC:
			case TYPE_INTEGER:
				sprintf(output, "%d", sqlite3_column_int(stmt, 1));
				sDir->value = std::string(output);
				(*out)[value_epoch] = sDir->value.c_str();
				break;
			case TYPE_REAL:
				sprintf(output, "%f", sqlite3_column_double(stmt, 1));
				sDir->value = std::string(output);
				(*out)[value_epoch] = sDir->value.c_str();
				break;
			case TYPE_TEXT:
				sDir->value = std::string((char*)sqlite3_column_text(stmt, 1));
				(*out)[value_epoch] = sDir->value.c_str();
				break;
			default:
				res = DB_E_FAIL;
				goto CLEANUP;
			}
		}
		res = DB_S_OK;
	}
	else{
		res = DB_S_FALSE;
		goto CLEANUP;
	}
CLEANUP:
	CHK_DBSQL(sqlite3_finalize(stmt), SQLITE_OK);
	return res;
}
/*
int main(void){
	sensorDB test[10];
	int i;
	std::map<std::string, std::string> out;
	std::map<std::string, std::map<int, std::string>> out2;
	char value[10] = { 0 };

	char time[21] = { 0 };
	int epoch=1435728484;

	test[0].suri = "a/dht";
	test[0].attribute = "temp";
	test[0].attributeType = TYPE_REAL;
	test[0].ttl = 100;
	test[0].value = "28.5";

	test[1].suri = "a/dht";
	test[1].attribute = "humid";
	test[1].attributeType = TYPE_REAL;
	test[1].ttl = 100;
	test[1].value = "48.2";

	test[2].suri = "a/dht";
	test[2].attribute = "atm";
	test[2].attributeType = TYPE_INTEGER;
	test[2].ttl = 100;
	test[2].value = "1002";

	test[3].suri = "a/dht";
	test[3].attribute = "state";
	test[3].attributeType = TYPE_TEXT;
	test[3].ttl = 0;
	test[3].value = "on";

	dbm.initializeManager();

	test[4].suri = "a/dht";
	test[4].attribute = "atm";
	dbm.addSensorData(&test[2]);
	system("pause");
	dbm.discardOldData(&test[4]);
	
	for (i = 0; i < 4; i++){
		dbm.addSensorAttribute(&test[i]);
		test[i].ttl = 0;
		dbm.getSensorAttribute(&test[i]);
		printf("index = %d\nttl = %d\ntype = %d\n", test[i].index, test[i].ttl, test[i].attributeType);
		dbm.addSensorData(&test[i]);
		test[i].value = "0";
		dbm.getRecentSensorData(&test[i]);
		printf("attribute = %s\nvalue = %s\n\n", test[i].attribute.c_str(), test[i].value.c_str());
	}

	for (i = 0; i < 4; i++){
		test[i+4].suri = "a/dht";
		test[i+4].attribute = "atm";
		sprintf(value, "%d", 1100-i);
		test[i + 4].value = value;
		test[i + 4].epoch_offset = i * 1000;
		dbm.addSensorData(&test[i+4]);

		test[i + 4].suri = "a/dht";
		test[i + 4].attribute = "humid";
		sprintf(value, "%.1f", (float)80.5 - i);
		test[i + 4].value = value;
		dbm.addSensorData(&test[i + 4]);

		//if (i % 2 == 1)
			//Sleep(20000);
	}

	for (i = 0; i < 4; i++){
		test[i].attributeType = TYPE_NUMERIC;
		test[i].index = 0;
		test[i].ttl = 0;
		test[i].value = "0";
		dbm.getSensorAttrValueMap(&test[i], &out);
	}

	for (std::map<std::string, std::string>::iterator it = out.begin(); it != out.end(); it++){
		printf("%s, %s\n", it->first.c_str(), it->second.c_str());
	}

	dbm.Epoch2Time(time, &epoch);
	printf("%s, %d\n", time, epoch);
	dbm.getCurEpoch(&epoch);
	dbm.Epoch2Time(time, &epoch);
	printf("%s, %d\n", time, epoch);

	test[0].suri = "a/dht";
	dbm.getSensorVector(&test[0], 30, &out2);

	printf("\n\nTesting getSensorVector()\n");
	for (std::map<std::string, std::map<int, std::string>>::iterator it_o = out2.begin(); it_o != out2.end(); it_o++){
		printf("Attribute = %s\n", it_o->first.c_str());
		std::map<int, std::string> temp = it_o->second;
		for (std::map<int, std::string>::iterator it_i = temp.begin(); it_i != temp.end(); it_i++){
			printf("%d, %s\n", it_i->first, it_i->second.c_str());
		}
	}

	system("pause");
	return 0;
	}*/
