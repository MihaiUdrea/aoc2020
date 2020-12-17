// day6 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day6"

#define FIRST_STAR  "6583"
#define SECOND_STAR "3290"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)


namespace
{
struct Solve
{
  vector<vector<string>> list;

  Solve(const string & inStr){

    const regex doublelineRxToken("\\n\\n");

    forEachRxToken(inStr, doublelineRxToken, [&](string groupRaw) {
      list.push_back({});
      vector<string> group;
      forEachLine(groupRaw, [&](string line) {
        std::sort(line.begin(), line.end());
        list.rbegin()->push_back(line);
      });
    });
  };

  string Do() { return Both(false); }

  string Both(bool aIntersection = false)
  {
    vector<int> results;

    for (auto group : list)
    {
      string intersection = group[0];

      for (auto people : group)
      {
        string newintersection;

        aIntersection ? std::set_intersection(people.begin(), people.end(), intersection.begin(),
                              intersection.end(), back_inserter(newintersection))
                      : std::set_union(people.begin(), people.end(), intersection.begin(),
                                              intersection.end(), back_inserter(newintersection));
        intersection = std::move(newintersection);
      }
      results.push_back(intersection.size());
    }

    auto res = accumulate(results.begin(), results.end(), 0);

    return to_string(res);
  }

  string Do2() { return Both(true); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
abc

a
b
c

ab
ac

a
a
a
a

b)")
            .Do() == "11");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
abc

a
b
c

ab
ac

a
a
a
a

b)")
            .Do2() == "6");
}

TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}
