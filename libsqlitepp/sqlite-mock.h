#ifndef MOCKSQLITEDATASOURCE_H
#define MOCKSQLITEDATASOURCE_H

#include <mockDatabase.h>
#include <boost/filesystem/path.hpp>
#include <visibility.h>

namespace SQLite {

class DLL_PUBLIC Mock : public DB::MockDatabase {
	public:
		Mock(const std::string & root, const std::string & name, const std::vector<boost::filesystem::path> & ss);
		Mock(const std::string & name, const std::vector<boost::filesystem::path> & ss);
		~Mock();

	protected:
		void DropDatabase() const override;
		void CreateNewDatabase() const override;

		DB::Connection * openConnection() const override;

	private:
		const boost::filesystem::path testDbPath;
};

}

#endif


