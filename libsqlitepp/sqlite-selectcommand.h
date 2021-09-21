#ifndef SQLITE_SELECTCOMMAND_H
#define SQLITE_SELECTCOMMAND_H

#include "sqlite-command.h"
#include <selectcommand.h>
#include <string>

namespace SQLite {
	class Connection;
	class SelectCommand : public DB::SelectCommand, public Command {
	public:
		SelectCommand(const Connection *, const std::string & sql);

		bool fetch() override;
		void execute() override;
	};
}

#endif
