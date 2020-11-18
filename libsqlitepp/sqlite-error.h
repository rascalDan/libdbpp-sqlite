#ifndef SQLITE_ERROR_H
#define SQLITE_ERROR_H

#include <error.h>
#include <exception.h>
#include <sqlite3.h>
#include <visibility.h>

namespace SQLite {
	class Error : public AdHoc::Exception<DB::Error> {
	public:
		Error(sqlite3 *);

		std::string message() const noexcept override;

	private:
		std::string msg;
	};
}

#endif
