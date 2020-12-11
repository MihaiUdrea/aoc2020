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


namespace
{
struct Solve : CharMapLimits
{
  /** /
  struct data
  {
    int    start;
    int    end;
    char   ch;
    string pass;
  };

  vector<data> list;
  /**/

  vector<string> input;

  using CharMapLimits::CharMapLimits;

  string Do() { 

    bool                  changed    = false;

    do
    {
      changed                          = false;
      map<char, set<Point>> newCharMap = charMap;
      for (auto lx : irange(0, limit.x))
        for (auto ly : irange(0, limit.y))
        {
          Point pt{ lx, ly, 0 };
          int   neighOcupied = 0;
          for (auto dlx : irange(-1, 2))
            for (auto dly : irange(-1, 2))
            {
              if (dlx == 0 && dly == 0)
                continue;

              Point deltaPt{ dlx, dly, 0 };

              Point neighPt = pt + deltaPt;
              if (charMap['#'].find(neighPt) != charMap['#'].end())
                neighOcupied++;
            }

          if (charMap['L'].find(pt) != charMap['L'].end() && neighOcupied == 0)
          {
            newCharMap['L'].erase(pt);
            newCharMap['#'].insert(pt);
            changed = true;
          }
          else if (charMap['#'].find(pt) != charMap['#'].end() && neighOcupied >= 4)
          {
            newCharMap['#'].erase(pt);
            newCharMap['L'].insert(pt);
            changed = true;
          }
        }
      charMap = std::move(newCharMap);
      if (false)
      {
        if (!charMap['#'].empty())
        {
          auto ss = to2Ds(
            charMap['#'],
            [](auto & l, auto pos) {
              return Point{ l.y, l.x, 0 };
            },
            [](auto & l) {
              return "#"s;
            },
            to2DsFlags::full_header, '.', 1);

          cout << ss;
        }

        if (!charMap['L'].empty())
        {
          auto ss = to2Ds(
            charMap['L'],
            [](auto & l, auto pos) {
              return Point{ l.y, l.x, 0 };
            },
            [](auto & l) {
              return "L"s;
            },
            to2DsFlags::full_header, '.', 1);

          cout << ss;
        }
      }
    } while (changed);

    return to_string(charMap['#'].size()); 
  }

  string Do2() {
    
    
    bool changed = false;

    do
    {
      changed                          = false;
      map<char, set<Point>> newCharMap = charMap;
      for (auto lx : irange(0, limit.x))
        for (auto ly : irange(0, limit.y))
        {
          Point pt{ lx, ly, 0 };
          int   neighOcupied = 0;
          
          int maxX = max(lx, limit.x - lx);
          int maxY = max(ly, limit.y - ly);
          int themax = max(maxX, maxY);

          set<Point> found;
          for (auto i = 1; i <= themax && found.size() < 8; i++)
          for (auto dlx : irange(-1, 2))
            for (auto dly : irange(-1, 2))
            {
              if (dlx == 0 && dly == 0)
                continue;

              Point dir{ dlx, dly, 0 };

              if (found.find(dir) != found.end())
                continue;

              Point deltaPt{ dlx * i, dly * i, 0 };

              Point neighPt = pt + deltaPt;

              if (neighPt.x < 0 || neighPt.x >= limit.x)
                continue;
              if (neighPt.y < 0 || neighPt.y >= limit.y)
                continue;


              if (charMap['#'].find(neighPt) != charMap['#'].end())
              {
                neighOcupied++;
                found.insert(dir);
              }
              else if (charMap['L'].find(neighPt) != charMap['L'].end())
              {
                found.insert(dir);
              }
            }

          if (charMap['L'].find(pt) != charMap['L'].end() && neighOcupied == 0)
          {
            newCharMap['L'].erase(pt);
            newCharMap['#'].insert(pt);
            changed = true;
          }
          else if (charMap['#'].find(pt) != charMap['#'].end() && neighOcupied >= 5)
          {
            newCharMap['#'].erase(pt);
            newCharMap['L'].insert(pt);
            changed = true;
          }
        }
      charMap = std::move(newCharMap);

      if (false)
      {
        set<Point> unionPt = charMap['#'];
        unionPt.insert(charMap['L'].begin(), charMap['L'].end());

        auto ss = to2Ds(
          unionPt,
          [](auto & l, auto pos) {
            return Point{ l.y, l.x, 0 };
          },
          [&](auto & l) {
            return charMap['L'].find(l) != charMap['L'].end() ? "L"s : "#"s;
          },
          to2DsFlags::no_header, '.', 1);

        cout << ss;
      }
    } while (changed);

    return to_string(charMap['#'].size()); 

    return to_string(2); }
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
