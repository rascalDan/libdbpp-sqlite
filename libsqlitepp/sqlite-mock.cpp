#include "sqlite-mock.h"
#include "sqlite-connection.h"
#include <boost/lexical_cast.hpp>

NAMEDFACTORY("sqlite", SQLite::Mock, DB::MockDatabaseFactory);

namespace SQLite {

Mock::Mock(const std::string & root, const std::string & name, const std::vector<std::filesystem::path> & ss) :
	testDbPath(std::filesystem::path(root) / name / boost::lexical_cast<std::string>(getpid()) / boost::lexical_cast<std::string>(++DB::MockDatabase::mocked))
{
	CreateNewDatabase();
	PlaySchemaScripts(ss);
}

Mock::Mock(const std::string & name, const std::vector<std::filesystem::path> & ss) :
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
	std::filesystem::remove(testDbPath);
}

void Mock::CreateNewDatabase() const
{
	std::filesystem::create_directories(testDbPath.parent_path());
	openConnection(); // Triggers file creation
}

}

