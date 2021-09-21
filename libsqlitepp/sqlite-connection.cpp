#include "sqlite-connection.h"
#include "connection.h"
#include "sqlite-error.h"
#include "sqlite-modifycommand.h"
#include "sqlite-selectcommand.h"
#include <factory.h>
#include <memory>

NAMEDFACTORY("sqlite", SQLite::Connection, DB::ConnectionFactory)

SQLite::ConnectionError::ConnectionError(sqlite3 * db) : SQLite::Error(db) { }

SQLite::Connection::Connection(const std::string & str) : db(nullptr)
{
	if (sqlite3_open(str.c_str(), &db) != SQLITE_OK) {
		auto dbp = std::unique_ptr<sqlite3, decltype(&sqlite3_close)>(db, &sqlite3_close);
		throw ConnectionError(dbp.get());
	}
}

SQLite::Connection::~Connection()
{
	sqlite3_close(db);
}

void
SQLite::Connection::beginTxInt()
{
	if (sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr) != SQLITE_OK) {
		throw Error(db);
	}
}

void
SQLite::Connection::commitTxInt()
{
	if (sqlite3_exec(db, "COMMIT TRANSACTION", nullptr, nullptr, nullptr) != SQLITE_OK) {
		throw Error(db);
	}
}

void
SQLite::Connection::rollbackTxInt()
{
	if (sqlite3_exec(db, "ROLLBACK TRANSACTION", nullptr, nullptr, nullptr) != SQLITE_OK) {
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
