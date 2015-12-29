#include "sqlite-modifycommand.h"
#include "sqlite-error.h"
#include <stdlib.h>
#include "sqlite-connection.h"

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
		throw Error(c->db);
	}
	unsigned int rows = sqlite3_changes(c->db);
	sqlite3_reset(stmt);
	if (rows == 0 && !anc) {
		throw DB::NoRowsAffected();
	}
	return rows;
}

