#ifndef SQLITE_SELECTCOMMAND_H
#define SQLITE_SELECTCOMMAND_H

#include "sqlite-command.h"
#include <selectcommand.h>

namespace SQLite {
	class Connection;
	class ColumnBase;
	class SelectCommand : public DB::SelectCommand, public Command {
	public:
		SelectCommand(const Connection *, const std::string & sql);

		bool fetch() override;
		void execute() override;
	};
}

#endif
