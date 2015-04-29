#include "selectcommand.h"
#include "connection.h"
#include "error.h"
#include <string.h>

class Column : public DB::Column {
	public:
		Column(const Glib::ustring & n, unsigned int i, sqlite3_stmt * s) :
			DB::Column(n, i),
			stmt(s)
		{
		}

		bool isNull() const {
			return (SQLITE_NULL == sqlite3_column_type(stmt, colNo));
		}

		void apply(DB::HandleField & h) const {
			switch (sqlite3_column_type(stmt, colNo)) {
				case SQLITE_INTEGER:
					h.integer(sqlite3_column_int64(stmt, colNo));
					return;
				case SQLITE_FLOAT:
					h.floatingpoint(sqlite3_column_double(stmt, colNo));
					return;
				case SQLITE_TEXT:
					{
						auto t = sqlite3_column_text(stmt, colNo);
						auto l = sqlite3_column_bytes(stmt, colNo);
						h.string(reinterpret_cast<const char *>(t), l);
						return;
					}
				case SQLITE_NULL:
					h.null();
					return;
				case SQLITE_BLOB:
					throw std::runtime_error("Blobs not supported");
			}
			
		}

		void rebind(DB::Command*, unsigned int) const {
			throw std::runtime_error("Not implemented");
		}

	private:
		sqlite3_stmt * const stmt;
};

SQLite::SelectCommand::SelectCommand(const Connection * conn, const std::string & sql) :
	DB::Command(sql),
	DB::SelectCommand(sql),
	SQLite::Command(conn, sql)
{
}

void
SQLite::SelectCommand::execute()
{
	// No explicit execute required
}

bool
SQLite::SelectCommand::fetch()
{
	switch (sqlite3_step(stmt)) {
		case SQLITE_ROW:
			if (columns.empty()) {
				for (int c = sqlite3_data_count(stmt) - 1; c >= 0; c -= 1) {
					columns.insert(DB::ColumnPtr(new Column(sqlite3_column_name(stmt, c), c, stmt)));
				}
			}
			return true;
		case SQLITE_DONE:
			return false;
		default:
			throw Error(sqlite3_errmsg(c->db));
	}
}

