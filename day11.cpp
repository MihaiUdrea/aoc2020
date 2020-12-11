// day11 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day11"

#define FIRST_STAR  "2354"
#define SECOND_STAR "2072"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)
#pragma warning(disable : 4244)


namespace
{
struct Solve : CharMapLimits
{
  using CharMapLimits::CharMapLimits;

  set<Point> directions = { Point{ 1, 0, 0 },  { -1, 0, 0 }, { 0,  1, 0 }, {  0, -1, 0 },
                                 { 1, 1, 0 },  { -1, 1, 0 }, { 1, -1, 0 }, { -1, -1, 0 } };

  string Work(int C1, int C2, bool longSearch)
  {
    for (bool changed = true; changed;)
    {
      changed = false;
      map<char, map<Point, int>> neighCount;

      for (auto ch : { 'L', '#' })
        for (auto pt : charMap[ch])
          neighCount[ch][pt] = GetNeighbourCount(pt, longSearch);

      for (auto [pt, neighOcupied] : neighCount['L'])
        if (neighOcupied == C1)
        {
          charMap['L'].erase(pt);
          charMap['#'].insert(pt);
          changed = true;
        }
      for (auto [pt, neighOcupied] : neighCount['#'])
        if (neighOcupied >= C2)
        {
          charMap['#'].erase(pt);
          charMap['L'].insert(pt);
          changed = true;
        }

      //Print();
    }

    return to_string(charMap['#'].size());
  }

  int GetNeighbourCount(Point pt, bool longSearch)
  {
    if (!longSearch)
      return count_if(directions.begin(), directions.end(), [&](auto deltaPt) {
        return charMap['#'].contains(pt + deltaPt);
      });

    // part2
    
    return count_if(directions.begin(), directions.end(), [&](auto d) 
      {
        for (auto i = 1;; ++i)
        {
          Point deltaPt{ d.x * i, d.y * i, 0 };
          Point neighPt = pt + deltaPt;
      
          if (!neighPt.Inside(limit))
            return false;
      
          if (charMap['#'].contains(neighPt))
          {
            return true;
          }
          else if (charMap['L'].contains(neighPt))
          {
            return false;
          }
        }
      });
  }

  void Print() 
  {
    set<Point> unionPt = charMap['#'];
    unionPt.insert(charMap['L'].begin(), charMap['L'].end());

    auto ss = to2Ds(
      unionPt,
      [](auto & l, auto pos) {
        return Point{ l.y, l.x, 0 };
      },
      [&](auto & l) {
        return charMap['L'].contains(l) ? "L"s : "#"s;
      },
      to2DsFlags::no_header, '.', 1);

    cout << ss;
  }

  string Do() { return Work(0, 4, false); }
  string Do2() { return Work(0, 5, true); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL)")
            .Do() == "37");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(1 + R"(
L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL)")
            .Do2() == "26");
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
