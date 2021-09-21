#include "sqlite-selectcommand.h"
#include "column.h"
#include "command.h"
#include "error.h"
#include "selectcommand.h"
#include "sqlite-command.h"
#include "sqlite-connection.h"
#include "sqlite-error.h"
#include <cstring>
#include <glibmm/ustring.h>
#include <memory>
#include <sqlite3.h>

namespace SQLite {
	class Column : public DB::Column {
	public:
		Column(const Glib::ustring & n, unsigned int i, sqlite3_stmt * s) : DB::Column(n, i), stmt(s) { }

		[[nodiscard]] bool
		isNull() const override
		{
			return (SQLITE_NULL == sqlite3_column_type(stmt, static_cast<int>(colNo)));
		}

		void
		apply(DB::HandleField & h) const override
		{
			switch (sqlite3_column_type(stmt, static_cast<int>(colNo))) {
				case SQLITE_INTEGER:
					h.integer(sqlite3_column_int64(stmt, static_cast<int>(colNo)));
					return;
				case SQLITE_FLOAT:
					h.floatingpoint(sqlite3_column_double(stmt, static_cast<int>(colNo)));
					return;
				case SQLITE_TEXT: {
					auto t = sqlite3_column_text(stmt, static_cast<int>(colNo));
					auto l = sqlite3_column_bytes(stmt, static_cast<int>(colNo));
					h.string({reinterpret_cast<const char *>(t), static_cast<std::size_t>(l)});
					return;
				}
				case SQLITE_NULL:
					h.null();
					return;
				case SQLITE_BLOB:
					throw DB::ColumnTypeNotSupported();
			}
		}

	private:
		sqlite3_stmt * const stmt;
	};
}

SQLite::SelectCommand::SelectCommand(const Connection * conn, const std::string & sql) :
	DB::Command(sql), DB::SelectCommand(sql), SQLite::Command(conn, sql)
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
			if (!columnCount()) {
				for (int c = sqlite3_data_count(stmt) - 1; c >= 0; c -= 1) {
					insertColumn(std::make_unique<Column>(sqlite3_column_name(stmt, c), c, stmt));
				}
			}
			return true;
		case SQLITE_DONE:
			return false;
		default:
			throw Error(c->db);
	}
}
