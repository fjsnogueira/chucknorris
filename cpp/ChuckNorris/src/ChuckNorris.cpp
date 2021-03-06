#include <ChuckNorris.hpp>

#include <stdexcept>
#include <iostream>
#include <sqlite3.h>

namespace
{
  void createTable(sqlite3* db)
  {
    auto const sql = R"(
CREATE TABLE chucknorris(id PRIMARY_KEY, fact TEXT);
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris was bitten by a cobra and after five days of excruciating pain... the cobra died.");
INSERT INTO chucknorris (fact) VALUES
  ("The easiest way to determine Chuck Norris's age is to cut him in half and count the rings.");
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris doesn't dial the wrong number. You answered the wrong phone.");
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris knows Victoria's secret");
INSERT INTO chucknorris (fact) VALUES
  ("If Chuck Norris was a Spartan in the movie 300, the movie would be called 1");
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris can kill two stones with one bird.");
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris can slam a revolving door.");
INSERT INTO chucknorris (fact) VALUES
  ("There are no weapons of mass destruction in Iraq, Chuck Norris lives in Oklahoma.");
INSERT INTO chucknorris (fact) VALUES
  ("If, by some incredible space-time paradox, Chuck Norris would ever fight himself, he'd win. Period.");
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris sleeps with a pillow under his gun.");
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris threw a grenade and killed 50 people, then it exploded.");
INSERT INTO chucknorris (fact) VALUES
  ("Chuck Norris counted to infinity. Twice.");
INSERT INTO chucknorris (fact) VALUES
  ("Giraffes were created when Chuck Norris uppercutted a horse.");
INSERT INTO chucknorris (fact) VALUES
  ("When a zombie apocalypse starts, Chuck Norris doesn't try to survive. The zombies do.");
INSERT INTO chucknorris (fact) VALUES
  ("Q: How many Chuck Norris' does it take to change a light bulb?

A.None, Chuck Norris prefers to kill in the dark.");
)";

    char* errorMessage = nullptr;
    auto res = sqlite3_exec(db, sql, 0, 0, &errorMessage);
    if (res != SQLITE_OK) {
      throw std::runtime_error("Failed to create table: " + std::string(errorMessage));
    }
}
}

std::string ChuckNorris::version() {
  return "0.1";
}

ChuckNorris::ChuckNorris()
{
  sqlite3_open(":memory:", &_db);
  createTable(_db);
}


ChuckNorris::~ChuckNorris()
{
  sqlite3_close(_db);
}

std::string ChuckNorris::getFact()
{
  sqlite3_stmt* statement;
  int rc;
  rc = sqlite3_prepare_v2(_db,
      R"(SELECT fact FROM chucknorris ORDER BY RANDOM() LIMIT 1;)",
      -1, &statement, 0);
  if (rc != SQLITE_OK) {
      auto const message = std::string(sqlite3_errmsg(_db));
      throw std::runtime_error("Failed to get fact: " + message);
  }
  rc = sqlite3_step(statement);
  if (rc != SQLITE_ROW) {
    sqlite3_finalize(statement);
    throw std::runtime_error("Expected SQLITE_ROW, got: " + std::to_string(rc));
  }
  auto sqlite_row = sqlite3_column_text(statement, 0);
  auto row = reinterpret_cast<const char*>(sqlite_row);
  auto res = std::string(row);
  sqlite3_finalize(statement);
  return res;
}
