#ifndef MOCKSQLITEDATASOURCE_H
#define MOCKSQLITEDATASOURCE_H

#include <mockDatabase.h>
#include <filesystem>
#include <visibility.h>

namespace SQLite {

class DLL_PUBLIC Mock : public DB::MockDatabase {
	public:
		Mock(const std::string & root, const std::string & name, const std::vector<std::filesystem::path> & ss);
		Mock(const std::string & name, const std::vector<std::filesystem::path> & ss);
		~Mock();

	protected:
		void DropDatabase() const override;
		void CreateNewDatabase() const override;

		DB::ConnectionPtr openConnection() const override;

	private:
		const std::filesystem::path testDbPath;
};

}

#endif


