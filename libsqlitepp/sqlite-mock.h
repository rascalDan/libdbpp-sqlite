#ifndef MOCKSQLITEDATASOURCE_H
#define MOCKSQLITEDATASOURCE_H

#include <c++11Helpers.h>
#include <filesystem>
#include <mockDatabase.h>
#include <visibility.h>

namespace SQLite {

	class DLL_PUBLIC Mock : public DB::MockDatabase {
	public:
		Mock(const std::string & root, const std::string & name, const std::vector<std::filesystem::path> & ss);
		Mock(const std::string & name, const std::vector<std::filesystem::path> & ss);
		~Mock() override;

		SPECIAL_MEMBERS_MOVE_RO(Mock);

	protected:
		void DropDatabase() const override;
		void CreateNewDatabase() const override;

		[[nodiscard]] DB::ConnectionPtr openConnection() const override;

	private:
		const std::filesystem::path testDbPath;
	};

}

#endif
