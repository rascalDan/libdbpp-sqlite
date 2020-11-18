#ifndef SQLITE_COMMAND_H
#define SQLITE_COMMAND_H

#include <command.h>
#include <sqlite3.h>

namespace SQLite {
	class Connection;
	class Command : public virtual DB::Command {
	public:
		Command(const Connection *, const std::string & sql);
		virtual ~Command() = 0;

		void bindParamI(unsigned int, int) override;
		void bindParamI(unsigned int, long int) override;
		void bindParamI(unsigned int, long long int) override;
		void bindParamI(unsigned int, unsigned int) override;
		void bindParamI(unsigned int, long unsigned int) override;
		void bindParamI(unsigned int, long long unsigned int) override;

		void bindParamB(unsigned int, bool) override;

		void bindParamF(unsigned int, double) override;
		void bindParamF(unsigned int, float) override;

		void bindParamS(unsigned int, const Glib::ustring &) override;
		void bindParamS(unsigned int, const std::string_view &) override;

		void bindParamT(unsigned int, const boost::posix_time::time_duration &) override;
		void bindParamT(unsigned int, const boost::posix_time::ptime &) override;

		void bindNull(unsigned int) override;

	protected:
		const Connection * c;
		sqlite3_stmt * stmt;
	};
}

#endif
