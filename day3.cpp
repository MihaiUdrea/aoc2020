// Aoc - 2020 Day 3: Toboggan Trajectory
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "3"

#define FIRST_STAR  "171"
#define SECOND_STAR "1206576000"

#ifdef THISDAY
  #define TODAY DAY THISDAY "/"
#else
  #define CATCH_CONFIG_MAIN
  #define TODAY
#endif  // THISDAY

#pragma warning(disable : 4267)

namespace
{
struct Solve
{
  vector<string> lines;
  set<Point> trees;

  Point      limits;

  Solve(const string & inStr){

    lines = GetLines(inStr);

    for (auto l : irange(0, lines.size()))
      for (auto c : irange(0, lines[l].size()))
        if (lines[l][c] == '#')
          trees.insert({ l, c, 0 });
  };

  string Do()
  { 
    return to_string(DoP());
  }

  int DoP(int incC = 3, int incL = 1)
  {
    int cnt = 0;
    for (auto il = 0, ic = 0; il < lines.size(); il += incL, ic += incC)
    {
      if (contains(trees, Point{ il, ic % lines[0].size(), 0 }))
        cnt++;
    }
    return cnt;
  }

  string Do2() { 
    auto l1 = DoP(1, 1);    
    auto l2 = DoP(3, 1);
    auto l5 = DoP(5, 1);
    auto l3 = DoP(7, 1);
    auto l4 = DoP(1, 2);

    return to_string(l1*l2*l3*l4*l5); 
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
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

TEST_CASE(TODAY "Sample 1 Part 2", "[x.]")
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

TEST_CASE(TODAY "Part One", "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}

