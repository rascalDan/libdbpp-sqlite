#include "sqlite-mock.h"
#include "sqlite-connection.h"
#include <buffer.h>
#include <boost/filesystem/operations.hpp>

namespace SQLite {

Mock::Mock(const std::string & name, const std::vector<boost::filesystem::path> & ss) :
	MockDatabase(name),
	testDbPath(boost::filesystem::path("/tmp") / "sqliteut" / stringbf("%d", getpid()) / stringbf("%d", ++DB::MockDatabase::mocked))
{
	CreateNewDatabase();
	PlaySchemaScripts(ss);
}

DB::Connection *
Mock::openConnection() const
{
	return new Connection(testDbPath.string());
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
	delete openConnection();
}

}

