#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include <connection.h>
#include "sqlite-error.h"
#include <sqlite3.h>

namespace SQLite {
	class ConnectionError : public virtual Error, public virtual DB::ConnectionError {
		public:
			ConnectionError(sqlite3 *);
	};

	class Connection : public DB::Connection {
		public:
			Connection(const std::string & info);
			~Connection();

			void beginTxInt() override;
			void commitTxInt() override;
			void rollbackTxInt() override;
			void ping() const override;
			DB::BulkDeleteStyle bulkDeleteStyle() const override;
			DB::BulkUpdateStyle bulkUpdateStyle() const override;

			DB::SelectCommandPtr select(const std::string & sql, const DB::CommandOptionsCPtr &) override;
			DB::ModifyCommandPtr modify(const std::string & sql, const DB::CommandOptionsCPtr &) override;

			int64_t insertId() override;

			sqlite3 * db;
	};
}

#endif

