// day5 - 2020  Binary Boarding
#include "stdafx.h"
#include "Utils.h"


#define THISDAY "day5"

#define FIRST_STAR  "855"
#define SECOND_STAR "552"

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
struct Solve
{
  set<Point>     hist;
  vector<string> input;

  Solve(const string & inStr){
    input = GetLines(inStr);
  };

  int   GetId(Point p) { return p.y * 8 + p.x; }

  Point Calc(string str) {
    string x;
    string y;
    for (auto ch : str)
      switch (ch)
      {
      case 'F':
        y += '0';
        break;
      case 'B':
        y += '1';
        break;
      case 'R':
        x += '1';
        break;
      case 'L':
        x += '0';
      }
    Point p{ stoi(x,0,2), stoi(y,0,2), 0 };


    /*Point p;
    Point limits{ 7, 127, 0 };
    
    for (auto ch : str)
    {
      if (ch == 'F')
      {
        limits.y = p.y + (limits.y - p.y) / 2;
      }
      else if (ch == 'B')
      {
        p.y = p.y + (limits.y - p.y) / 2 + 1;       
      }
      else if (ch == 'R')
      {
        p.x = p.x + (limits.x - p.x) / 2 + 1;
      }
      else if (ch == 'L')
      {
        limits.x = p.x + (limits.x - p.x)/ 2;
      }
    }
    */

    return p;
  }

  string Do() { 
  
    for (auto str : input)
    {
      hist.insert(Calc(str));
    }

    return to_string(GetId(*max_element(hist.begin(), hist.end(), [&](auto l, auto r) {
      return GetId(l) < GetId(r);
    })));
  }

  string Do2() { 
    Do();
    for (auto ix : irange(0, 8))
      for (auto iy : irange(hist.begin()->y, hist.size() / 8 + hist.begin()->y + 1))
      { 
        Point p{ ix, iy, 0 };
      
        if (hist.find(p) == hist.end())
          return to_string( GetId(p));
      }
        
    return to_string(0); 
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 0", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
FBFBBFFRLR)")
            .Do() == "357");
}

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
BFFFBBFRRR)")
            .Do() == "567");
}

TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[.]")
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
