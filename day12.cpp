// day12 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day12"

#define FIRST_STAR  "998"
#define SECOND_STAR "71586"

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
  /**/
  struct data
  {
    int    nr;
    char   ch;
  };

  vector<data> list;
  /**/

  vector<string> input;

  Solve(const string & inStr){
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((.)(\d+))~");
      auto               res = match_rx(line, matchExp);
      list.push_back({ stoi(res[2]), res[1].str()[0]});
    });    
  };

  enum Directions
  {
    east, south, west, north
  };

  static Directions GetNewDirLeft(Directions dir, int nr) { 
    return static_cast<Directions>((4 - nr / 90 + dir) % 4);
  }

  static Directions GetNewDirRight(Directions dir, int nr)
  {
    auto x = static_cast<Directions>((4 + nr / 90 + dir) % 4);
    return x;
  }

  static Point GetNewWayPointLeft(Point p, int nr)
  {
    auto x = (nr / 90) % 4;
  
    for (auto i : irange(0, x ))
    {
      p = { -p.y, p.x, 0 };
    }
    return p;
  }

  static Point GetNewWayPointRight(Point p, int nr)
  {
    auto x = 4 - (nr / 90) % 4;

    for (auto i : irange(0, x))
    {
      p = { -p.y, p.x, 0 };
    }
    return p;

  }

  Point GetDelta(char ch, int nr) 
  {
    Point delta;
    switch (ch)
    {
    case 'N':
      delta = { -nr, 0, 0 };
      break;
    case 'S':
      delta = { nr, 0, 0 };
      break;
    case 'E':
      delta = { 0, nr, 0 };
      break;
    case 'W':
      delta = { 0, -nr, 0 };
      break;
    default:
      break;
    }
    return delta;
  }

  string Do() { 
    
    Point p{};
    vector     chOfDir = { 'E', 'S', 'W', 'N'};
    Directions dir = east;
    
    for (auto mv : list)
    {
      Point delta;
      switch (mv.ch)
      {
      case 'N':         
      case 'S':        
      case 'E':        
      case 'W':
        delta = GetDelta(mv.ch, mv.nr);
        break;
      case 'L':
        dir = GetNewDirLeft(dir, mv.nr);
        break;
      case 'R':
        dir = GetNewDirRight(dir, mv.nr);
        break;
      case 'F':
        delta = GetDelta(chOfDir[dir], mv.nr);
        break;
      default:
        break;
      }

      p = p + delta;
    }

    return to_string(ManhDist(p));
  }

  string Do2()
  {
    Point      waypoint = GetDelta('E', 10) + GetDelta('N', 1);
    Point      p{};
    vector     chOfDir = { 'E', 'S', 'W', 'N' };
    Directions dir     = east;
    vector<Point> lil;
    lil.push_back(p);
    for (auto mv : list)
    {
      Point delta;
      switch (mv.ch)
      {
      case 'N':
      case 'S':
      case 'E':
      case 'W':
        delta = GetDelta(mv.ch, mv.nr);
        waypoint = waypoint + delta;
        break;
      case 'L':
        waypoint = GetNewWayPointLeft(waypoint, mv.nr);
        break;
      case 'R':
        waypoint = GetNewWayPointRight(waypoint, mv.nr);
        break;

      case 'F':
        delta = { waypoint.x * mv.nr, waypoint.y * mv.nr, 0 };
        p     = p + delta;
        break;
      default:
        break;
      }

      lil.push_back(p);

    }
    
    /** /
     auto ss = to2Ds(
      lil,
      [](auto & l, auto pos) {
        return l;
      },
      [&](auto & l, auto pos) {
        return to_string(pos);
      },
      to2DsFlags::full_header, '.', 1);

     cout << ss;
     /**/
    return to_string(ManhDist(p));
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 0", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve::GetNewDirRight(Solve::north, 90) == Solve::east);

  REQUIRE(Solve::GetNewDirLeft(Solve::east, 90) == Solve::north);
  REQUIRE(Solve::GetNewDirLeft(Solve::east, 180) == Solve::west);
  REQUIRE(Solve::GetNewDirLeft(Solve::east, 270) == Solve::south);
  REQUIRE(Solve::GetNewDirLeft(Solve::east, 360) == Solve::east);
  REQUIRE(Solve::GetNewDirRight(Solve::east, 90) == Solve::south);
  REQUIRE(Solve::GetNewDirRight(Solve::east, 180) == Solve::west);
  REQUIRE(Solve::GetNewDirRight(Solve::east, 270) == Solve::north);
  REQUIRE(Solve::GetNewDirRight(Solve::east, 360) == Solve::east);

  REQUIRE(Solve::GetNewDirRight(Solve::west, 90) == Solve::north);
  REQUIRE(Solve::GetNewDirRight(Solve::west, 180) == Solve::east);
  REQUIRE(Solve::GetNewDirRight(Solve::west, 270) == Solve::south);
  REQUIRE(Solve::GetNewDirRight(Solve::west, 360) == Solve::west);
  REQUIRE(Solve::GetNewDirLeft(Solve::west, 90) == Solve::south);
  REQUIRE(Solve::GetNewDirLeft(Solve::west, 180) == Solve::east);
  REQUIRE(Solve::GetNewDirLeft(Solve::west, 270) == Solve::north);
  REQUIRE(Solve::GetNewDirLeft(Solve::west, 360) == Solve::west);

  REQUIRE(Solve::GetNewDirRight(Solve::north, 90) == Solve::east);
  REQUIRE(Solve::GetNewDirRight(Solve::north, 180) == Solve::south);
  REQUIRE(Solve::GetNewDirRight(Solve::north, 270) == Solve::west);
  REQUIRE(Solve::GetNewDirRight(Solve::north, 360) == Solve::north);
  REQUIRE(Solve::GetNewDirLeft(Solve::north, 90) == Solve::west);
  REQUIRE(Solve::GetNewDirLeft(Solve::north, 180) == Solve::south);
  REQUIRE(Solve::GetNewDirLeft(Solve::north, 270) == Solve::east);
  REQUIRE(Solve::GetNewDirLeft(Solve::north, 360) == Solve::north);
}

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
F10
N3
F7
R90
F11)")
            .Do() == "25");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
F10
N3
F7
R90
F11)")
            .Do2() == "286");
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
