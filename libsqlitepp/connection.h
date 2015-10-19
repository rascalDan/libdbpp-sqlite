#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include <connection.h>
#include "error.h"
#include <sqlite3.h>

namespace SQLite {
	class Connection : public DB::Connection {
		public:
			Connection(const std::string & info);
			~Connection();

			void finish() const override;
			int beginTx() const override;
			int commitTx() const override;
			int rollbackTx() const override;
			bool inTx() const override;
			void ping() const override;
			DB::BulkDeleteStyle bulkDeleteStyle() const override;
			DB::BulkUpdateStyle bulkUpdateStyle() const override;

			DB::SelectCommand * newSelectCommand(const std::string & sql) const override;
			DB::ModifyCommand * newModifyCommand(const std::string & sql) const override;

			void	beginBulkUpload(const char *, const char *) const override;
			void	endBulkUpload(const char *) const override;
			size_t bulkUploadData(const char *, size_t) const override;

			sqlite3 * db;

		private:
			mutable unsigned int txDepth;
			mutable bool rolledback;
	};
}

#endif

