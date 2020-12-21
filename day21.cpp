// day21 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day21"

#define FIRST_STAR  "2874"
#define SECOND_STAR "gfvrr,ndkkq,jxcxh,bthjz,sgzr,mbkbn,pkkg,mjbtz"

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
using namespace views;

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

  vector<food> foodList;
  /**/

  vector<string> input;
  map<string, set<int>> ingredientsFoodMap;
  map<string, set<int>> allergenFoodMap;
  map<string, set<string>> allergenIngredShortList;
  map<string, string>      allergenIngredient;

  set<string> settledIng;

  Solve(const string & inStr){
    for (auto line : inStr | tokenize(regex("\n"), -1) | views::transform(to_string_view()))
    {
      auto res = match_rx(line, regex(R"~((.+) \(contains ([^)]+)\))~"));
      auto ingredients = to_string_view::from(std::move(res[1]));
      auto allergens = to_string_view::from(std::move(res[2]));
      
      food newFood;
      
      for (auto ing : ingredients | tokenize(regex(" "), -1))
        newFood.ingredients.insert(ing);

      for (auto allergen : allergens | tokenize(regex(", "), -1))
        newFood.allergens.insert(allergen);

      foodList.push_back(std::move(newFood)); 
    }
  }

  string Do() 
  { 
    for (auto [pos, food] : foodList | views::enumerate)
    {
      for (auto ing : food.ingredients)
        ingredientsFoodMap[ing].insert(pos);
      for (auto aler : food.allergens)
        allergenFoodMap[aler].insert(pos);
    }

    for (auto & aler : allergenFoodMap)
    {
      set<string> common_ingred = foodList[front(aler.second)].ingredients;
      for (auto foodIdx : aler.second | tail)
        common_ingred = common_ingred | views::set_intersection(foodList[foodIdx].ingredients) | to<set>;
      
      allergenIngredShortList[aler.first] = std::move(common_ingred);
    }

    while (!allergenIngredShortList.empty())
    {
      auto min = min_element(allergenIngredShortList, less{}, [](auto & e) {
        return e.second.size();
      });
  
      //if (min->second.size() == 1)
      {
        string ing = *(min->second.begin());

        allergenIngredient[min->first] = ing;

        settledIng.insert(ing);

        for (auto & i : allergenIngredShortList)
          i.second.erase(ing);

        allergenIngredShortList.erase(min);
      }
    };

    auto rng = ingredientsFoodMap | filter([&](auto x) {
                 return !settledIng.contains(x.first);
               }) | values;

    return to_string(accumulate(rng, 0, plus{}, &set<int>::size)); 
  }

  string Do2() 
  { 
    Do();

    return allergenIngredient | views::values | join(',') | to<string>;
  }
};
}  // namespace

#include "catch.hpp"

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
