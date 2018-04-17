#include "sqlite-connection.h"
#include "sqlite-error.h"
#include "sqlite-selectcommand.h"
#include "sqlite-modifycommand.h"

NAMEDFACTORY("sqlite", SQLite::Connection, DB::ConnectionFactory);

SQLite::ConnectionError::ConnectionError(sqlite3 * db) :
	SQLite::Error(db)
{
}

SQLite::Connection::Connection(const std::string & str)
{
	if (sqlite3_open(str.c_str(), &db) != SQLITE_OK) {
		ConnectionError err(db);
		sqlite3_close(db);
		throw err;
	}
}

SQLite::Connection::~Connection()
{
	sqlite3_close(db);
}

void
SQLite::Connection::beginTxInt()
{
	if (sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
		throw Error(db);
	}
}

void
SQLite::Connection::commitTxInt()
{
	if (sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
		throw Error(db);
	}
}

void
SQLite::Connection::rollbackTxInt()
{
	if (sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
		throw Error(db);
	}
}

DB::BulkDeleteStyle
SQLite::Connection::bulkDeleteStyle() const
{
	return DB::BulkDeleteUsingUsingAlias;
}

DB::BulkUpdateStyle
SQLite::Connection::bulkUpdateStyle() const
{
	return DB::BulkUpdateUsingJoin;
}

void
SQLite::Connection::ping() const
{
	// Can this fail?
}


DB::SelectCommandPtr
SQLite::Connection::select(const std::string & sql, const DB::CommandOptionsCPtr &)
{
	return std::make_shared<SelectCommand>(this, sql);
}

DB::ModifyCommandPtr
SQLite::Connection::modify(const std::string & sql, const DB::CommandOptionsCPtr &)
{
	return std::make_shared<ModifyCommand>(this, sql);
}

int64_t
SQLite::Connection::insertId()
{
	return sqlite3_last_insert_rowid(db);
}

