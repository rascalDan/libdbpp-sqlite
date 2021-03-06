#include "sqlite-modifycommand.h"
#include "command.h"
#include "modifycommand.h"
#include "sqlite-command.h"
#include "sqlite-connection.h"
#include "sqlite-error.h"
#include <sqlite3.h>

SQLite::ModifyCommand::ModifyCommand(const Connection * conn, const std::string & sql) :
	DB::Command(sql), DB::ModifyCommand(sql), SQLite::Command(conn, sql)
{
}

unsigned int
SQLite::ModifyCommand::execute(bool anc)
{
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		sqlite3_reset(stmt);
		throw Error(c->db);
	}
	auto rows = static_cast<unsigned int>(sqlite3_changes(c->db));
	sqlite3_reset(stmt);
	if (rows == 0 && !anc) {
		throw DB::NoRowsAffected();
	}
	return rows;
}
