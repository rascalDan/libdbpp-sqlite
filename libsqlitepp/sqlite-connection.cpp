#include "sqlite-connection.h"
#include "sqlite-error.h"
#include "sqlite-selectcommand.h"
#include "sqlite-modifycommand.h"

SQLite::ConnectionError::ConnectionError(sqlite3 * db) :
	SQLite::Error(db)
{
}

SQLite::Connection::Connection(const std::string & str) :
	txDepth(0),
	rolledback(false)
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
SQLite::Connection::finish() const
{
	if (txDepth != 0) {
		rollbackTx();
		throw DB::TransactionStillOpen();
	}
}

int
SQLite::Connection::beginTx() const
{
	if (txDepth == 0) {
		if (sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
			throw Error(db);
		}
		rolledback = false;
	}
	return ++txDepth;
}

int
SQLite::Connection::commitTx() const
{
	if (rolledback) {
		return rollbackTx();
	}
	if (--txDepth == 0) {
		if (sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
			throw Error(db);
		}
	}
	return txDepth;
}

int
SQLite::Connection::rollbackTx() const
{
	if (--txDepth == 0) {
		if (sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
			throw Error(db);
		}
	}
	else {
		rolledback = true;
	}
	return txDepth;
}

bool
SQLite::Connection::inTx() const
{
	return txDepth;
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


DB::SelectCommand *
SQLite::Connection::newSelectCommand(const std::string & sql)
{
	return new SelectCommand(this, sql);
}

DB::ModifyCommand *
SQLite::Connection::newModifyCommand(const std::string & sql)
{
	return new ModifyCommand(this, sql);
}

int64_t
SQLite::Connection::insertId() const
{
	return sqlite3_last_insert_rowid(db);
}

