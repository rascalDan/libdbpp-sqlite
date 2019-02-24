#ifndef SQLITE_MODIFYCOMMAND_H
#define SQLITE_MODIFYCOMMAND_H

#include <modifycommand.h>
#include "sqlite-command.h"

namespace SQLite {
	class Connection;
	class ModifyCommand : public DB::ModifyCommand, public Command {
		public:
			ModifyCommand(const Connection *, const std::string & sql);

			unsigned int execute(bool) override;
	};
}

#endif



