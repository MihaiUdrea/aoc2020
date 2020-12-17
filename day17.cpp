// day17 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day17"

#define FIRST_STAR  "306"
#define SECOND_STAR "2572"

#ifdef THISDAY
#define TODAY            THISDAY "/"
#define HIDE_IF_OLD_TEST "[.]"
#else
#define TODAY
#define HIDE_IF_OLD_TEST
#endif  // THISDAY

using namespace std;

// disable conversion warning
#pragma warning(disable : 4267)
#pragma warning(disable : 4244)

namespace
{
struct Solve : CharMapLimits
{
  using CharMapLimits::CharMapLimits;
  Point      leftSentinel;
  set<Point> directions;

  string Work(bool second)
  {
    for (auto ix : irange(-1, 2))
      for (auto iy : irange(-1, 2))
        for (auto iz : irange(-1, 2))
        {
          for (auto iw : irange(second ? -1 : 0, second ? 2 : 1))
            if (ix != 0 || iz != 0 || iy != 0 || iw != 0)
              directions.insert({ ix, iy, iz, iw });
        }

    leftSentinel = { 0, 0, 0 };

    Print();

    for (auto idx : irange(0,6))
    {
      map<char, map<Point, int>> neighCount;

      // add '.' free space to explore      
      for (auto pt : charMap['#'])
        for (auto deltaPt : directions)
        {
          Point newPt = pt + deltaPt;
          if (!charMap['#'].contains(newPt))
            charMap['.'].insert(newPt);            
        }

      for (auto ch : { '.', '#' })
        for (auto pt : charMap[ch])
          neighCount[ch][pt] = GetNeighbourCount(pt);

      for (auto [pt, neighOcupied] : neighCount['.'])
        if (neighOcupied == 3)
        {
          charMap['.'].erase(pt);
          charMap['#'].insert(pt);
        }
      for (auto [pt, neighOcupied] : neighCount['#'])
        if (neighOcupied != 2 && neighOcupied != 3)
        {
          charMap['#'].erase(pt);
          charMap['.'].insert(pt);
        }

      if (!second)
        Print();
    }

    return to_string(charMap['#'].size());
  }

  int GetNeighbourCount(Point pt)
  {
    return count_if(directions.begin(), directions.end(), [&](auto deltaPt) {
      return charMap['#'].contains(pt + deltaPt);
    });
  }

  void Print()
  {
    auto ss = to2Ds(
      charMap['#'],
      [](auto & l, auto pos) {
        return Point{ l.y, l.x, l.z };
      },
      [&](auto & l) {
        return "#"s;
      },
      to2DsFlags::depth_on | to2DsFlags::full_header, '.', 1);

    cout << ss;
  }

  string Do() { return Work(false); }
  string Do2() { return Work(true); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
.#.
..#
###)")
            .Do() == "112");
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
