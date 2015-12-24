#define BOOST_TEST_MODULE TestSQLite
#include <boost/test/unit_test.hpp>

#include <definedDirs.h>
#include <dbpp/modifycommand.h>
#include <dbpp/selectcommand.h>
#include <dbpp/column.h>
#include <sqlite-mock.h>
#include <testCore.h>
#include <boost/date_time/posix_time/posix_time.hpp>

class StandardMockDatabase : public SQLite::Mock {
	public:
		StandardMockDatabase() : SQLite::Mock("sqlitemock", {
				rootDir / "sqliteschema.sql" })
		{
		}
};

BOOST_GLOBAL_FIXTURE( StandardMockDatabase );

BOOST_FIXTURE_TEST_SUITE( Core, DB::TestCore );

BOOST_AUTO_TEST_CASE( transactions )
{
	auto ro = DB::MockDatabase::openConnectionTo("sqlitemock");

	BOOST_REQUIRE_EQUAL(false, ro->inTx());
	ro->beginTx();
	BOOST_REQUIRE_EQUAL(true, ro->inTx());
	ro->rollbackTx();
	BOOST_REQUIRE_EQUAL(false, ro->inTx());

	ro->beginTx();
	BOOST_REQUIRE_EQUAL(true, ro->inTx());
	ro->commitTx();
	BOOST_REQUIRE_EQUAL(false, ro->inTx());

	delete ro;
}

BOOST_AUTO_TEST_CASE( bindAndSend )
{
	auto rw = DB::MockDatabase::openConnectionTo("sqlitemock");

	auto mod = rw->newModifyCommand("INSERT INTO test VALUES(?, ?, ?)");
	mod->bindParamI(0, testInt);
	mod->bindParamF(1, testDouble);
	mod->bindParamS(2, testString);
	mod->execute();
	BOOST_REQUIRE_EQUAL(2, rw->insertId());
	delete mod;
	rw->commitTx();
	delete rw;
}

BOOST_AUTO_TEST_CASE( bindAndSelect )
{
	auto ro = DB::MockDatabase::openConnectionTo("sqlitemock");

	auto select = ro->newSelectCommand("SELECT * FROM test WHERE id = ?");
	select->bindParamI(0, testInt);
	select->execute();
	int rows = 0;
	while (select->fetch()) {
		assertColumnValueHelper(*select, 0, testInt);
		assertColumnValueHelper(*select, 1, testDouble);
		assertColumnValueHelper(*select, 2, testString);
		rows += 1;
	}
	delete select;
	BOOST_REQUIRE_EQUAL(1, rows);
	delete ro;
}

BOOST_AUTO_TEST_SUITE_END();

