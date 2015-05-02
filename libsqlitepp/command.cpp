#include "command.h"
#include "connection.h"
#include <stdlib.h>
#include <string.h>

SQLite::Command::Command(const Connection * conn, const std::string & sql) :
	DB::Command(sql),
	c(conn)
{
	if (sqlite3_prepare_v2(conn->db, sql.c_str(), sql.length(), &stmt, NULL) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(conn->db));
	}
}

SQLite::Command::~Command()
{
	sqlite3_finalize(stmt);
}

void
SQLite::Command::bindParamI(unsigned int n, int v)
{
	if (sqlite3_bind_int(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamI(unsigned int n, long int v)
{
	if (sqlite3_bind_int64(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamI(unsigned int n, long long int v)
{
	if (sqlite3_bind_int64(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamI(unsigned int n, unsigned int v)
{
	if (sqlite3_bind_int64(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamI(unsigned int n, long unsigned int v)
{
	if (sqlite3_bind_int64(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamI(unsigned int n, long long unsigned int v)
{
	if (sqlite3_bind_int64(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamF(unsigned int n, double v)
{
	if (sqlite3_bind_double(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamF(unsigned int n, float v)
{
	if (sqlite3_bind_double(stmt, n + 1, v) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamS(unsigned int n, const Glib::ustring & s)
{
	if (sqlite3_bind_text(stmt, n + 1, s.c_str(), s.length(), SQLITE_STATIC) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
void
SQLite::Command::bindParamB(unsigned int, bool)
{
	throw Error("Not supported");
}
void
SQLite::Command::bindParamT(unsigned int, const boost::posix_time::time_duration &)
{
	throw Error("Not supported");
}
void
SQLite::Command::bindParamT(unsigned int, const boost::posix_time::ptime &)
{
	throw Error("Not supported");
}
void
SQLite::Command::bindNull(unsigned int n)
{
	if (sqlite3_bind_null(stmt, n + 1) != SQLITE_OK) {
		throw Error(sqlite3_errmsg(c->db));
	}
}
