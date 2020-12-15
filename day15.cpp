// day15 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day15"

#define FIRST_STAR  "595"
#define SECOND_STAR "1708310"

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
  map<int, int> hist;
  vector<int> input;

  Solve(const string & inStr){


    /**/
    forEachLine(inStr, [&](string line) {
      static const regex colsSepRx(",");
      forEachRxToken(line, colsSepRx, [&](string chunk) {
        input.push_back(stoi(chunk));
      });
    });
    /**/
  };

  string Do(int limit = 2020) { 
    vector<int> spoken;
    for (auto i : irange(0, input.size() - 1))
    {
      //spoken.push_back(input[i]);
      hist[input[i]] = i + 1;
    }

    int lastNo = input.back();
    for (auto i : irange(input.size() - 1, limit - 1))
    {
      int prevPos = hist[lastNo];
      int newVal  = prevPos == 0 ? 0 : i - prevPos +1 ;
      
      hist[lastNo] = i + 1;

      lastNo = newVal;
    }

    return to_string(lastNo); 
  }

  string Do2() { 
    return Do(30000000); 
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
0,3,6)")
            .Do() == "436");

  REQUIRE(Solve(1 + R"(
1,3,2)")
            .Do() == "1");


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
