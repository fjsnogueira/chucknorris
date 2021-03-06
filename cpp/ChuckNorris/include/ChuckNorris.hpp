#pragma once

#include <string>

#include "chucknorris_export.h"

struct sqlite3;

class ChuckNorris
{
  public:
    CHUCKNORRIS_EXPORT ChuckNorris();
    ChuckNorris(ChuckNorris const&) = delete;
    ChuckNorris(ChuckNorris &&) = delete;
    ChuckNorris& operator=(ChuckNorris const&) = delete;
    ChuckNorris& operator=(ChuckNorris &&) = delete;
    CHUCKNORRIS_EXPORT virtual ~ChuckNorris();

    static CHUCKNORRIS_EXPORT std::string version();

    CHUCKNORRIS_EXPORT std::string getFact();


  private:
    sqlite3* _db;
};
