#include "sqlite-mock.h"
#include "sqlite-connection.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>

NAMEDFACTORY("sqlite", SQLite::Mock, DB::MockDatabaseFactory);

namespace SQLite {

Mock::Mock(const std::string & root, const std::string &, const std::vector<boost::filesystem::path> & ss) :
	testDbPath(boost::filesystem::path(root) / boost::lexical_cast<std::string>(getpid()) / boost::lexical_cast<std::string>(++DB::MockDatabase::mocked))
{
	CreateNewDatabase();
	PlaySchemaScripts(ss);
}

Mock::Mock(const std::string & name, const std::vector<boost::filesystem::path> & ss) :
	Mock("/tmp/sqliteut", name, ss)
{
}

DB::ConnectionPtr
Mock::openConnection() const
{
	return std::make_shared<Connection>(testDbPath.string());
}

Mock::~Mock()
{
	DropDatabase();
}

void Mock::DropDatabase() const
{
	boost::filesystem::remove(testDbPath);
}

void Mock::CreateNewDatabase() const
{
	boost::filesystem::create_directories(testDbPath.parent_path());
	openConnection(); // Triggers file creation
}

}

