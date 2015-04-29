#include "modifycommand.h"
#include "error.h"
#include <stdlib.h>
#include "connection.h"

SQLite::ModifyCommand::ModifyCommand(const Connection * conn, const std::string & sql) :
	DB::Command(sql),
	DB::ModifyCommand(sql),
	SQLite::Command(conn, sql)
{
}

SQLite::ModifyCommand::~ModifyCommand()
{
}

unsigned int
SQLite::ModifyCommand::execute(bool anc)
{
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		sqlite3_reset(stmt);
		throw Error(sqlite3_errmsg(c->db));
	}
	unsigned int rows = sqlite3_changes(c->db);
	sqlite3_reset(stmt);
	if (rows == 0 && !anc) {
		throw Error("No rows affected");
	}
	return rows;
}

