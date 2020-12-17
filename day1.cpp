// Aoc - 2020 Day1 Report Repair
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "1"

#define FIRST_STAR "1013211"
#define SECOND_STAR "13891280"

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

namespace
{
struct Solve
{
  std::set<int>  hist;
  vector<string> data;
  Solve(const string & inStr)
  {
    data = GetLines(inStr);
    for (auto i : data)
      hist.insert(stoi(i));
  };

  string Do()
  {
    for (auto i : hist)
    {
      if (hist.find(2020 - i) != hist.end())
      {
        return to_string(i * (2020 - i));
      }
    }
    return string();
  }

  string Do2()
  {
    for (auto j : hist)
      for (auto i : hist)
      {
        if (hist.find(2020 - i - j) != hist.end())
        {
          return to_string(i * j * (2020 - i - j));
        }
      }
    return string();
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[.]")
{
  

  REQUIRE(Solve(R"(1721
979
366
299
675
1456)")
            .Do() == "514579");
}

TEST_CASE(TODAY "Part One", "[.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", "[.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}
