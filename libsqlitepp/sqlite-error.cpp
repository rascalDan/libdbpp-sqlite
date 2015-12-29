#include "sqlite-error.h"
#include <string.h>

SQLite::Error::Error(sqlite3 * db) :
	msg(sqlite3_errmsg(db))
{
}

std::string
SQLite::Error::message() const throw()
{
	return msg;
}

