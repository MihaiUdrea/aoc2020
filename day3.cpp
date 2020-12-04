// Aoc - 2020 Day 3: Toboggan Trajectory
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day3"

#define FIRST_STAR  "171"
#define SECOND_STAR "1206576000"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

namespace
{

struct Solve : CharMapLimits
{
  using CharMapLimits::CharMapLimits;

  string Do()
  { 
    return to_string(DoP());
  }

  int DoP(int incC = 3, int incL = 1)
  {
    int cnt = 0;
    for (auto il = 0, ic = 0; il < limit.x; il += incL, ic += incC)
    {
      if (contains(charMap['#'], Point{ il, ic % limit.y, 0 }))
        cnt++;
    }
    return cnt;
  }

  string Do2() { 
    vector vals = {
      pair{ 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 },
    };

    return to_string(accumulate(vals.begin(), vals.end(), 1ll, [&](size_t a, auto b) {
      return a * DoP(b.first, b.second);
    }));
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1+R"(
..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#)").Do() == "7");
}

TEST_CASE(TODAY "Sample 1 Part 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1+R"(
..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#)")
            .Do2() == "336");
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

