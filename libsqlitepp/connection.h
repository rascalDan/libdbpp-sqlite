#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include "../libdbpp/connection.h"
#include "error.h"
#include <sqlite3.h>

namespace SQLite {
	class Connection : public DB::Connection {
		public:
			Connection(const std::string & info);
			~Connection();

			void finish() const;
			int beginTx() const;
			int commitTx() const;
			int rollbackTx() const;
			bool inTx() const;
			void ping() const;
			DB::BulkDeleteStyle bulkDeleteStyle() const;
			DB::BulkUpdateStyle bulkUpdateStyle() const;

			DB::SelectCommand * newSelectCommand(const std::string & sql) const;
			DB::ModifyCommand * newModifyCommand(const std::string & sql) const;

			void	beginBulkUpload(const char *, const char *) const;
			void	endBulkUpload(const char *) const;
			size_t bulkUploadData(const char *, size_t) const;

			sqlite3 * db;

		private:
			mutable unsigned int txDepth;
			mutable bool rolledback;
	};
}

#endif

