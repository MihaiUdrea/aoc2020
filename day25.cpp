// day25 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day25"

#define FIRST_STAR  "290487"
//#define SECOND_STAR ""

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)

using namespace ranges;

namespace
{
struct Solve
{
  vector<int> input;

  Solve(const string & inStr){
    for (auto line : inStr | views::tokenize(regex("\n"), -1))
      input.push_back(stoi(line));
  };

  string Do() { 
  
    vector<int> loopSize;
    for (auto x : input)
    {
      INT64 t = 1;

      for (auto i : views::iota(1))
      {
        t = (t * 7) % 20201227;
        if (t == x)
        {
          loopSize.push_back(i);
          break;
        }
      }
    }

    return to_string(accumulate(views::iota(0, loopSize[0]), 1ll, [&](auto t, auto unused) {
      return (t * input[1]) % 20201227;
    }));
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
5764801
17807724
)")
            .Do() == "14897079");
}

TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}
