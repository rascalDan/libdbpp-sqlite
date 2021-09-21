#ifndef SQLITE_ERROR_H
#define SQLITE_ERROR_H

#include <error.h> // IWYU pragma: keep
#include <exception.h>
#include <sqlite3.h>
#include <string>

namespace SQLite {
	class Error : public AdHoc::Exception<DB::Error> {
	public:
		explicit Error(sqlite3 *);

		std::string message() const noexcept override;

	private:
		std::string msg;
	};
}

#endif
