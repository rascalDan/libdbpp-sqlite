#include "sqlite-command.h"
#include "command.h"
#include "sqlite-connection.h"
#include "sqlite-error.h"

namespace Glib {
	class ustring;
}
namespace boost::posix_time {
	class time_duration;
}

SQLite::Command::Command(const Connection * conn, const std::string & sql) : DB::Command(sql), c(conn), stmt(nullptr)
{
	if (sqlite3_prepare_v2(conn->db, sql.c_str(), static_cast<int>(sql.length()), &stmt, nullptr) != SQLITE_OK) {
		throw Error(conn->db);
	}
}

SQLite::Command::~Command()
{
	sqlite3_finalize(stmt);
}

template<typename V, typename BinderFunction>
void
SQLite::Command::bindWith(unsigned int n, const V & v, BinderFunction binder) const
{
	if (binder(stmt, static_cast<int>(n + 1), v) != SQLITE_OK) {
		throw Error(c->db);
	}
}

void
SQLite::Command::bindParamI(unsigned int n, int v)
{
	bindWith(n, v, sqlite3_bind_int);
}
void
SQLite::Command::bindParamI(unsigned int n, long int v)
{
	bindWith(n, v, sqlite3_bind_int64);
}
void
SQLite::Command::bindParamI(unsigned int n, long long int v)
{
	bindWith(n, v, sqlite3_bind_int64);
}
void
SQLite::Command::bindParamI(unsigned int n, unsigned int v)
{
	bindWith(n, v, sqlite3_bind_int64);
}
void
SQLite::Command::bindParamI(unsigned int n, long unsigned int v)
{
	bindWith(n, static_cast<sqlite3_int64>(v), sqlite3_bind_int64);
}
void
SQLite::Command::bindParamI(unsigned int n, long long unsigned int v)
{
	bindWith(n, static_cast<sqlite3_int64>(v), sqlite3_bind_int64);
}
void
SQLite::Command::bindParamF(unsigned int n, double v)
{
	bindWith(n, v, sqlite3_bind_double);
}
void
SQLite::Command::bindParamF(unsigned int n, float v)
{
	bindWith(n, static_cast<double>(v), sqlite3_bind_double);
}
void
SQLite::Command::bindParamS(unsigned int n, const Glib::ustring & s)
{
	return bindParamS(n, std::string_view(s.raw()));
}
void
SQLite::Command::bindParamS(unsigned int n, const std::string_view & s)
{
	if (sqlite3_bind_text(stmt, static_cast<int>(n + 1), s.data(), static_cast<int>(s.length()), SQLITE_STATIC)
			!= SQLITE_OK) {
		throw Error(c->db);
	}
}
void
SQLite::Command::bindParamB(unsigned int, bool)
{
	throw DB::ParameterTypeNotSupported();
}
void
SQLite::Command::bindParamT(unsigned int, const boost::posix_time::time_duration &)
{
	throw DB::ParameterTypeNotSupported();
}
void
SQLite::Command::bindParamT(unsigned int, const boost::posix_time::ptime &)
{
	throw DB::ParameterTypeNotSupported();
}
void
SQLite::Command::bindNull(unsigned int n)
{
	if (sqlite3_bind_null(stmt, static_cast<int>(n + 1)) != SQLITE_OK) {
		throw Error(c->db);
	}
}
