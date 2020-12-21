// day21 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "day21"

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
  struct food
  {
    set<string> allergens;
    set<string> ingredients;
    map<int, set<string>> sameallergens;
    map<int, set<string>> sameingredients;
  };

  vector<food> list;
  /**/

  vector<string> input;
  map<string, set<int>> ingredientsFoodMap;
  map<string, set<int>> allergenFoodMap;
  map<string, set<string>> allergenIngredShortList;
  map<string, string>      allergenIngredient;

  set<string> settledIng;

  Solve(const string & inStr){

    input = GetLines(inStr);

    /**/
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((.+) \(contains ([^)]+)\))~");
      auto               res = match_rx(line, matchExp);
    
      food               newFood;

      static const regex colsSepRx(" ");
      forEachRxToken(res[1], colsSepRx, [&](string ing) {
        newFood.ingredients.insert(ing);
      });

      static const regex colsSepRx2(", ");
      forEachRxToken(res[2], colsSepRx2, [&](string alergen) {
        newFood.allergens.insert(alergen);
      });

      list.push_back(std::move(newFood)); 
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

  string Do() { 
    
    auto miningredSize = 100;
    int  p1, p2;
    for (auto pos : irange(0, list.size()))
    {
      auto & food = list[pos];
      for (auto pos2 : irange(pos + 1, list.size()))
      {
        auto & food2 = list[pos2];
        auto inters = food.ingredients | views::set_intersection(food2.ingredients) | to<set>;

        if (inters.size() < miningredSize)
        {
          miningredSize = inters.size();
          p1            = pos;
          p2            = pos2;
        }
        food2.sameingredients[pos] = inters;
        food.sameingredients[pos2] = std::move(inters);

        auto inters2 = food.allergens | views::set_intersection(food2.allergens) | to<set>;

        food2.sameallergens[pos] = inters2;
        food.sameallergens[pos2] = std::move(inters2);

        
      }
    }

    for (auto pos : irange(0, list.size()))
    {
      auto & food = list[pos];
      for (auto & ing : food.ingredients)
        ingredientsFoodMap[ing].insert(pos);
      for (auto & aler : food.allergens)
        allergenFoodMap[aler].insert(pos);
    }

    for (auto & aler : allergenFoodMap)
    {
      set<string> common_ingred = list[*aler.second.begin()].ingredients;
      for (auto foodIdx : aler.second)
      {
        common_ingred =
          common_ingred | views::set_intersection(list[foodIdx].ingredients) | to<set>;
      }
      allergenIngredShortList[aler.first] = std::move(common_ingred);
    }

    while (!allergenIngredShortList.empty())
    {
      auto min = min_element(allergenIngredShortList, less{}, [](auto & e) {
        return e.second.size();
      });
  
      if (min->second.size() == 1)
      {
        string ing                    = *(min->second.begin());

        allergenIngredient[min->first] = ing;

        settledIng.insert(ing);

        for (auto & i : allergenIngredShortList)
          i.second.erase(ing);

        allergenIngredShortList.erase(min);
      }
      else
      {
        // bad
        MessageBeep(0);
      }

    };

    int count = 0;
    for (auto ing : ingredientsFoodMap)
    {
      if (settledIng.contains(ing.first))
        continue;
      
      count += ing.second.size();
    }

    return to_string(count); 
  }

  string Do2() { 
    Do();

    string s;
    
    for (auto ing : allergenIngredient)
    {
      if (!s.empty())
        s += ",";
      s += ing.second;
    }

    return s; }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
xxxxxxxxx)")
            .Do() == "sample result");
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
