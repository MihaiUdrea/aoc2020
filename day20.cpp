// day20 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "day20"

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
  struct idAndrotation
  {
    int    no;
    int    rotation;
    
    auto operator<=>(const idAndrotation &) const = default;
  };

  map<int, CharMapLimits> list;
  map<int, set<Point>> bordersList;
  map<idAndrotation, set<Point>> allBordersList;
  map<idAndrotation, set<Point>> allArreasList;
  /**/

  vector<string> input;

  Solve(const string & inStr){
    using namespace views;

    
     for (auto chunk : inStr 
       | views::tokenize(regex("\n\n"), -1) 
       | views::transform(to_string_view()))
     {
      int no;
      for (auto line : chunk | views::tokenize(regex(":\n"), -1) | views::transform(to_string_view()) | views::take(1))
      {
        auto res1 = match_rx(line, regex(R"~(Tile (\d+))~"));          
        
        no = stoi(res1[1]);
      }
      
      for (auto line : chunk | views::tokenize(regex(":\n"), -1) | views::transform(to_string_view()) | views::drop(1))
      {
        string matrix(line);
        CharMapLimits mapLimits(matrix);
        tileSize = mapLimits.limit.y;


        list[no] = std::move(mapLimits);
      }
     }

     // erase mids
     for (auto & e : list)
     {
       auto border = e.second.charMap['#'] | filter([](auto pt) {
                       return pt.x == 0 || pt.y == 0 || pt.x == 9 || pt.y == 9;
                     }) |
                     to<vector>;

      auto allArea = e.second.charMap['#'] | to<vector>;

       auto variations = [&](auto & allBordersList, auto & border) {
         for (auto i : views::iota(0, 4))
         {
           allBordersList[{ e.first, i }] = border | to<set>;

           // rotate
           actions::transform(border, [](auto p) {
             auto newPt = Solve::Rotate(p);
             newPt.x += 9;
             return newPt;
           });
         }

         // flip it on one axis
         actions::transform(border, [](auto p) {
           auto newPt = p;
           newPt.x    = 9 - newPt.x;
           return newPt;
         });

         for (auto i : views::iota(4, 8))
         {
           allBordersList[{ e.first, i }] = border | to<set>;

           // rotate
           actions::transform(border, [](auto p) {
             auto newPt = Solve::Rotate(p);
             newPt.x += 9;
             return newPt;
           });
         }
       };
       variations(allBordersList, border);
       variations(allArreasList, allArea);
     };

  };

  static Point Rotate(Point p, int nr = 90)
  {
    auto x = (nr / 90) % 4;

    for (auto i : irange(0, x))
    {
      p = { -p.y, p.x, 0 };
    }
    return p;
  }

  set<int> usedtiles;
  vector<idAndrotation> tilesNo;

  vector<vector<idAndrotation>> solutions;
  int                           squareSize;
  int                           tileSize;

  bool AlignHorizontal(set<Point> &l, set<Point> &r)
  {
    auto rightEdgeofFirst = l | views::filter([](auto p) {return p.y == 9;}) | views::transform([](auto p) { return p.x;}) | to<vector> | actions::sort;
    auto leftEdgeofSecond = r | views::filter([](auto p) {return p.y == 0;}) | views::transform([](auto p) { return p.x;}) | to<vector> | actions::sort;

    return equal(rightEdgeofFirst, leftEdgeofSecond);
  }

  bool AlignVertical(set<Point> &u, set<Point> &d)
  {
    auto botEdgeofFirst = u | views::filter([](auto p) {return p.x == 9;}) | views::transform([](auto p) { return p.y;}) | to<vector>| actions::sort;
    auto upEdgeofSecond = d | views::filter([](auto p) {return p.x == 0;}) | views::transform([](auto p) { return p.y;}) | to<vector>| actions::sort;

    return equal(botEdgeofFirst, upEdgeofSecond);
  }

  bool IsValid(pair<const idAndrotation, set<Point>> & elem)
  {
    int crPos = tilesNo.size();
  
    if (crPos == 0)
      return true;    //elem.first.rotation == 0;

    int line = crPos / squareSize;
    int col  = crPos % squareSize;

    if (line == 0)
      return AlignHorizontal(allBordersList[tilesNo[line * squareSize + col - 1]], elem.second);
    else if (col == 0)
      return AlignVertical(allBordersList[tilesNo[(line - 1) * squareSize + col]], elem.second);
    else
      return AlignHorizontal(allBordersList[tilesNo[line * squareSize + col - 1]], elem.second) && AlignVertical(allBordersList[tilesNo[(line - 1) * squareSize + col]], elem.second);

    /**
    // verify neighbor matching
    switch (crPos)
    {
    case 1:
      return AlignVertical(elem.second, allBordersList[tilesNo[0]]);
    case 2:
      return AlignHorizontal(elem.second, allBordersList[tilesNo[0]]);    
    case 3:
      return AlignVertical(allBordersList[tilesNo[0]], elem.second);
    case 4:
      return AlignHorizontal(allBordersList[tilesNo[0]], elem.second);
    case 5:
      return AlignHorizontal(elem.second, allBordersList[tilesNo[1]]) && AlignVertical(elem.second, allBordersList[tilesNo[2]]);
    case 6:
      return AlignHorizontal(allBordersList[tilesNo[3]], elem.second) && AlignVertical(allBordersList[tilesNo[4]], elem.second);
    case 7:
      return AlignHorizontal(allBordersList[tilesNo[1]], elem.second) && AlignVertical(elem.second, allBordersList[tilesNo[4]]);
    case 8:
      return AlignHorizontal(elem.second, allBordersList[tilesNo[3]]) && AlignVertical(allBordersList[tilesNo[2]], elem.second);
    }
    /**/

    //return false;
  }

  bool Parse()
  {
    if (tilesNo.size() == list.size())
    {
      solutions.push_back(tilesNo);

      return true;
    }

    for (auto & t : allBordersList)
    {
      auto nrCr = t.first.no;
      auto rotCr = t.first.rotation;

      auto pos = tilesNo.size();

      if (usedtiles.contains(t.first.no))
        continue;

      if (!IsValid(t))
        continue;

      tilesNo.push_back(t.first);
      usedtiles.insert(t.first.no);

      if (Parse())
        return true;

      tilesNo.pop_back();
      usedtiles.erase(t.first.no);
    }

    return false;
  }

  string Do() {
  
    squareSize = (int)sqrt(list.size());
    if (Parse() == true)
    {
      INT64 prod = 1;

      cout << "soutions" << solutions.size() << std::endl;
      if (0)
      for (auto corner : solutions[0] | views::reverse | views::take(4))
      {
        prod *= corner.no;
      }
      prod = solutions[0][0].no;
      cout << prod << "*";
      int next = solutions[0][squareSize - 1].no;
      cout << next << "*";
      prod *= next;
      next = solutions[0][list.size() - squareSize].no;
      cout << next << "*";
      prod *= next;

      next=solutions[0][list.size() -1].no;
      cout << next << "*";
      prod *= next;

      return to_string(prod);
    }

    cout << "soutions" << solutions.size() << std::endl;

    return to_string(1); 
  }


  string Do2() { 
    Do();

    

    set<Point> area;
    int        count = 0;
    for (auto [pos, i] : solutions[0] | views::enumerate)
    {
      auto line = pos / squareSize;
      auto col = pos % squareSize;

      Point topLeft = { line * (tileSize - 2), col * (tileSize - 2) };
      
      for (auto pt : allArreasList[i])
      {
        if (allBordersList[i].contains(pt))
          continue;

        count++;
        Point newPt = pt + topLeft;
        area.insert(newPt);
      }
    }

    return to_string(2); 
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
Tile 2311:
..##.#..#.
##..#.....
#...##..#.
####.#...#
##.##.###.
##...#.###
.#.#.#..##
..#....#..
###...#.#.
..###..###

Tile 1951:
#.##...##.
#.####...#
.....#..##
#...######
.##.#....#
.###.#####
###.##.##.
.###....#.
..#.#..#.#
#...##.#..

Tile 1171:
####...##.
#..##.#..#
##.#..#.#.
.###.####.
..###.####
.##....##.
.#...####.
#.##.####.
####..#...
.....##...

Tile 1427:
###.##.#..
.#..#.##..
.#.##.#..#
#.#.#.##.#
....#...##
...##..##.
...#.#####
.#.####.#.
..#..###.#
..##.#..#.

Tile 1489:
##.#.#....
..##...#..
.##..##...
..#...#...
#####...#.
#..#.#.#.#
...#.#.#..
##.#...##.
..##.##.##
###.##.#..

Tile 2473:
#....####.
#..#.##...
#.##..#...
######.#.#
.#...#.#.#
.#########
.###.#..#.
########.#
##...##.#.
..###.#.#.

Tile 2971:
..#.#....#
#...###...
#.#.###...
##.##..#..
.#####..##
.#..####.#
#..#.#..#.
..####.###
..#.#.###.
...#.#.#.#

Tile 2729:
...#.#.#.#
####.#....
..#.#.....
....#..#.#
.##..##.#.
.#.####...
####.#.#..
##.####...
##..#.##..
#.##...##.

Tile 3079:
#.#.#####.
.#..######
..#.......
######....
####.#..#.
.#...#.##.
#.#####.##
..#.###...
..#.......
..#.###...)")
            .Do() == "20899048083289");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(ReadFileToString("sample.txt")).Do2() == "273");
}

TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", HIDE_IF_OLD_TEST "[.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}
