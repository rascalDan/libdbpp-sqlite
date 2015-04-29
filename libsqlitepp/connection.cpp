#include "connection.h"
#include "error.h"
#include "selectcommand.h"
#include "modifycommand.h"

SQLite::Connection::Connection(const std::string & str) :
	txDepth(0),
	rolledback(false)
{
	if (sqlite3_open(str.c_str(), &db) != SQLITE_OK) {
		if (db) {
			std::string err(sqlite3_errmsg(db));
			sqlite3_close(db);
			throw Error(err.c_str());
		}
		throw Error("Unknown error opening database");
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
		throw Error("Transaction still open");
	}
}

int
SQLite::Connection::beginTx() const
{
	if (txDepth == 0) {
		if (sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
			throw Error(sqlite3_errmsg(db));
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
			throw Error(sqlite3_errmsg(db));
		}
	}
	return txDepth;
}

int
SQLite::Connection::rollbackTx() const
{
	if (--txDepth == 0) {
		if (sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL) != SQLITE_OK) {
			throw Error(sqlite3_errmsg(db));
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
SQLite::Connection::newSelectCommand(const std::string & sql) const
{
	return new SelectCommand(this, sql);
}

DB::ModifyCommand *
SQLite::Connection::newModifyCommand(const std::string & sql) const
{
	return new ModifyCommand(this, sql);
}

void
SQLite::Connection::beginBulkUpload(const char *, const char *) const
{
	throw Error("Not implemented");
}

void
SQLite::Connection::endBulkUpload(const char *) const
{
	throw Error("Not implemented");
}

size_t
SQLite::Connection::bulkUploadData(const char *, size_t) const
{
	throw Error("Not implemented");
}

