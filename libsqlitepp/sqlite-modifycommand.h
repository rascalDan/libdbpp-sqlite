#ifndef SQLITE_MODIFYCOMMAND_H
#define SQLITE_MODIFYCOMMAND_H

#include "sqlite-command.h"
#include <modifycommand.h>
#include <string>

namespace SQLite {
	class Connection;
	class ModifyCommand : public DB::ModifyCommand, public Command {
	public:
		ModifyCommand(const Connection *, const std::string & sql);

		unsigned int execute(bool) override;
	};
}

#endif
