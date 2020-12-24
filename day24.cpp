// day24 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "day24"

//#define FIRST_STAR  ""
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
  /**/
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

  Solve(const string & inStr)
  {
    input = GetLines(inStr);

    /** /
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((\d+)-(\d+) (.): (\w+))~");
      auto               res = match_rx(line, matchExp);
      list.push_back({ stoi(res[1]), stoi(res[2]), res[3].str()[0], res[4] });
    });
    /**/

    /** /
    forEachLine(inStr, [&](string line) {
      static const regex colsSepRx(":");
      forEachRxToken(line, colsSepRx, [&](string chunk) {
        int i = 0;
        i++;
      });
    });
    /**/
  };

  /**/
  Point GetDelta (char ch, char ch2) 
  {
    Point delta;
    if (ch == 'w')
    {
      delta = { 0, -2 };
    }
    else if (ch == 'e')
    {
      delta = { 0, 2 };
    }
    else if (ch == 's')
    {
      delta.x = 1;
      if (ch2 == 'w')
      {
        delta.y = -1;
      }
      else if (ch2 == 'e')
      {
        delta.y = 1;
      }
    }
    else if (ch == 'n')
    {
      delta.x = -1;
      if (ch2 == 'w')
      {
        delta.y = -1;
      }
      else if (ch2 == 'e')
      {
        delta.y = 1;
      }
    }
    return delta;
  }
  map<Point, int> countList;

  set<Point> whites;
  set<Point> blacks;

  /**/
  string Do() { 
    


    for (auto [idx, line] : input | views::enumerate)
    {
      Point crPos = { 0, 0 };

      //Navigate(crPos, line, 0);

      for (int i = 0; i < line.size(); i++)
      {
        char ch = line[i];
        char ch2 = i == line.size()- 1 ? '-' : line[i + 1];

        Point delta = GetDelta(ch, ch2);

        crPos = crPos + delta;

        if (ch == 's' || ch == 'n')
          i++;
      }

      countList[crPos] = countList[crPos] + 1;
    }
  
    auto res = count_if(countList, [](auto l) {
      return l.second % 2 == 1;
    });

    return to_string(res);   
  }

  int    GetNeighborCount(Point pt) 
  {
    vector dir = { Point{ -1, -1 }, { -1, 1 }, { 0, 2 }, { 1, 1 }, { 1, -1 }, { 0, -2 }};
    int    ct  = 0;
    for (auto delta : dir)
    {
        Point newPt = pt + delta;
      if (blacks.contains(newPt))
        ct++;
    }
    return ct;
  }

  void Print()
  {
    return;

    set<Point> unionPt = blacks;
    unionPt.insert(whites.begin(), whites.end());

    auto ss = to2Ds(
      unionPt,
      [](auto & l, auto pos) {
        return l;
      },
      [&](auto & l) {
        return whites.contains(l) ? string(".") : string("#");
      },
      to2DsFlags::full_header, ' ', 1);

    cout << ss;
  }

  string Do2()
  {
    Do();
    for (auto i : countList)
    {
      if (i.second % 2 == 1)
        blacks.insert(i.first);
    }


    for (auto i : irange(0, 100))
    {
      /** /
      auto ss = to2Ds(
        blacks,
        [](auto & l, auto pos) {
          return l;
        },
        [&](auto & l, auto pos) {
          return string("#");
        },
        to2DsFlags::full_header, '.', 1);

      cout << ss;
      /**/

      // add whites 
      for (auto b : blacks)
      {
       

        vector dir = { Point{ -1, -1 }, { -1, 1 }, { 0, 2 }, { 1, 1 }, { 1, -1 }, { 0, -2 } };
        for (auto delta : dir)
        {
          Point newPt = b + delta;
          if (!blacks.contains(newPt))
            whites.insert(newPt);
        }
      }
      
      Print();

      map<Point, int> nc;
      vector<Point>   blacksPt;
      vector<Point>   whitesPt;
      for (auto pt : whites)
      {
        nc[pt] = GetNeighborCount(pt);
        whitesPt.push_back(pt);
      }
      for (auto pt : blacks)
      {
        auto c = GetNeighborCount(pt);

        nc[pt] = c;

        blacksPt.push_back(pt);
      }

      for (auto pt : blacksPt)
        if (nc[pt] == 0 || nc[pt] > 2)
        {
          blacks.erase(pt);
          whites.insert(pt);
        }

      for (auto pt : whitesPt)
        if (nc[pt] == 2)
        {
          whites.erase(pt);
          blacks.insert(pt);
        }
    }
    
    return to_string(blacks.size()); 
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
sesenwnenenewseeswwswswwnenewsewsw
neeenesenwnwwswnenewnwwsewnenwseswesw
seswneswswsenwwnwse
nwnwneseeswswnenewneswwnewseswneseene
swweswneswnenwsewnwneneseenw
eesenwseswswnenwswnwnwsewwnwsene
sewnenenenesenwsewnenwwwse
wenwwweseeeweswwwnwwe
wsweesenenewnwwnwsenewsenwwsesesenwne
neeswseenwwswnwswswnw
nenwswwsewswnenenewsenwsenwnesesenew
enewnwewneswsewnwswenweswnenwsenwsw
sweneswneswneneenwnewenewwneswswnese
swwesenesewenwneswnwwneseswwne
enesenwswwswneneswsenwnewswseenwsese
wnwnesenesenenwwnenwsewesewsesesew
nenewswnwewswnenesenwnesewesw
eneswnwswnwsenenwnwnwwseeswneewsenese
neswnwewnwnwseenwseesewsenwsweewe
wseweeenwnesenwwwswnew)")
            .Do2() == "2208");
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
