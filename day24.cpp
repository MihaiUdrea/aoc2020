// day24 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day24"

#define FIRST_STAR  "434"
#define SECOND_STAR "3955"

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
  vector<string> input;

  vector<Point> dir;
  string dirCh;
  map<char, Point> chPointMap;

  Solve(const string & inStr)
  {
    dir   = { Point{ -1, -1 }, { -1, 1 }, { 0, 2 }, { 1, 1 }, { 1, -1 }, { 0, -2 } };
    dirCh = "nNeSsw";
    for (auto i : views::zip(dir, dirCh))
    {
      chPointMap[i.second] = i.first;
    }    

    input = GetLines(inStr);
  };

  map<Point, int> countList;

  set<Point> whites;
  set<Point> blacks;

  /**/
  string Do() 
  { 
    for (auto line : input)
    {
      auto rd = views::zip(line, line | views::drop(1) | views::cycle) |
                views::transform([](auto l) {
                  if (l.first == 's' && l.second == 'e')
                    return 'S';
                  else if (l.first == 'n' and l.second == 'e')
                    return 'N';
                  else
                    return l.first;
                }) |
                views::adjacent_filter([](auto prev, auto next) {
                  return prev == 'e' || prev == 'w';
                });
        // cout << endl << rd;
        Point crPos = accumulate(rd, Point{}, plus{}, [&](auto ch) {
        return chPointMap[ch];
      });

      countList[crPos] = countList[crPos] + 1;
    }
  
    return to_string(count_if(countList | views::values, [](auto l) {
      return l % 2 == 1;
    }));   
  }

  int    GetNeighborCount(Point pt) 
  {
    return count_if(dir, [&](auto delta) {
      return blacks.contains(delta + pt);
    });
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

    // initial blacks
    for (auto i : countList)
      if (i.second % 2 == 1)
        blacks.insert(i.first);

    for (auto i : irange(0, 100))
    {
      // add whites

      //if (0)
      //for (auto newPt : views::cartesian_product(blacks, dir) | views::transform([](auto l) {
      //                    return get<0>(l) + get<1>(l);
      //                  }) |
      //                    views::filter([&](auto pt) {
      //                      return !blacks.contains(pt);
      //                    }))
      //  whites.insert(newPt);
      //else
      for (auto [pt, dir] : views::cartesian_product(blacks, dir))
        if (!blacks.contains(pt + dir))
          whites.insert(pt + dir);

      auto toSwitchToBlack = whites | views::filter([&](auto pt) {
        return GetNeighborCount(pt) == 2;}) | to<vector>;
      auto toSwitchToWhite = blacks | views::filter([&](auto pt) {
        auto n = GetNeighborCount(pt); 
        return n == 0 || n>2;}) | to<vector>;

      for (auto pt : toSwitchToBlack)
      {
        whites.erase(pt);
        blacks.insert(pt);
      }

      for (auto pt : toSwitchToWhite)
      {
        blacks.erase(pt);
        whites.insert(pt);
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
