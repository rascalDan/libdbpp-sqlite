#ifndef SQLITE_MODIFYCOMMAND_H
#define SQLITE_MODIFYCOMMAND_H

#include "../libdbpp/modifycommand.h"
#include "command.h"

namespace SQLite {
	class Connection;
	class ModifyCommand : public DB::ModifyCommand, public Command {
		public:
			ModifyCommand(const Connection *, const std::string & sql);
			virtual ~ModifyCommand();

			unsigned int execute(bool);

		private:
			void prepare() const;
			mutable bool prepared;
	};
}

#endif



